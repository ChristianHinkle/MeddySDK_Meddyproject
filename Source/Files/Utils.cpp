// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK/Meddyproject/Utils.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <utility>
#include <cassert>
#include <fstream>
#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Misc/Filesystem.h>
#include <MeddySDK/Meddyproject/FilesystemUtils.h>
#include <CppUtils/Misc/String.h>
#include <CppUtils/Core/String.h>

boost::filesystem::path MeddySDK::ProjectRootToManifestFilePath(
    boost::filesystem::path&& path)
{
    return DotMeddyprojectToManifestFilePath(
        ProjectRootToDotMeddyprojectPath(
            std::move(path)));
}

boost::filesystem::path MeddySDK::ProjectRootToDotMeddyprojectPath(
    boost::filesystem::path&& path)
{
    path.append(DotMeddyprojectString);
    return std::move(path);
}

boost::filesystem::path MeddySDK::DotMeddyprojectToProjectRootPath(
    boost::filesystem::path&& path)
{
    assert(IsDotMeddyprojectPath(path));
    return std::move(path).parent_path();
}

boost::filesystem::path MeddySDK::DotMeddyprojectToManifestFilePath(
    boost::filesystem::path&& path)
{
    assert(IsDotMeddyprojectPath(path));
    path.append(ManifestFilenameString);
    return std::move(path);
}

MeddySDK::ValidProjectRootQueryResult MeddySDK::QueryWhetherPathIsValidProjectRoot(
    boost::filesystem::path&& projectRootPath)
{
    {
        const boost::filesystem::file_status projectRootStatus = boost::filesystem::status(projectRootPath);

        if (!boost::filesystem::exists(projectRootStatus))
        {
            return ValidProjectRootQueryResult::No_ProjectRootDoesNotExist;
        }

        if (!boost::filesystem::is_directory(projectRootStatus))
        {
            return ValidProjectRootQueryResult::No_ProjectRootIsNonDirectory;
        }
    }

    boost::filesystem::path dotMeddyprojectPath = ProjectRootToDotMeddyprojectPath(std::move(projectRootPath));

    {
        const boost::filesystem::file_status dotMeddyprojectStatus = boost::filesystem::status(dotMeddyprojectPath);

        if (!boost::filesystem::exists(dotMeddyprojectStatus))
        {
            return ValidProjectRootQueryResult::No_DotMeddyprojectDoesNotExist;
        }

        if (!boost::filesystem::is_directory(dotMeddyprojectStatus))
        {
            return ValidProjectRootQueryResult::No_DotMeddyprojectIsNonDirectory;
        }
    }

    boost::filesystem::path manifestFilePath = DotMeddyprojectToManifestFilePath(std::move(dotMeddyprojectPath));

    {
        const boost::filesystem::file_status manifestFileStatus = boost::filesystem::status(manifestFilePath);

        if (!boost::filesystem::exists(manifestFileStatus))
        {
            return ValidProjectRootQueryResult::No_ManifestFileDoesNotExist;
        }

        if (boost::filesystem::is_directory(manifestFileStatus))
        {
            return ValidProjectRootQueryResult::No_ManifestFileIsDirectory;
        }
    }

    return ValidProjectRootQueryResult::Yes_IsValidProjectRoot;
}

MeddySDK::UncertainProjectCreationResult MeddySDK::TryCreateNewProject(
    boost::filesystem::path&& projectRootPath)
{
    const ValidProjectRootQueryResult projectRootQueryResult = QueryWhetherPathIsValidProjectRoot(boost::filesystem::path(projectRootPath));

    // We can only create a project if there is no .meddyproject directory whatsoever.
    if (projectRootQueryResult == ValidProjectRootQueryResult::No_DotMeddyprojectDoesNotExist)
    {
        const ProjectCreationResult projectCreationResult = CreateNewProject(std::move(projectRootPath));

        switch (projectCreationResult)
        {
        case ProjectCreationResult::Failed_FilesystemFailedToCreateDotMeddyproject:
            return UncertainProjectCreationResult::Failed_FilesystemFailedToCreateDotMeddyproject;
        case ProjectCreationResult::Failed_FilesystemFailedToCreateManifestFile:
            return UncertainProjectCreationResult::Failed_FilesystemFailedToCreateManifestFile;
        }

        return UncertainProjectCreationResult::Success;
    }

    // Handle fail cases. Return a descriptive result.

    switch (projectRootQueryResult)
    {
    case ValidProjectRootQueryResult::Yes_IsValidProjectRoot:
        return UncertainProjectCreationResult::Failed_ValidProjectAlreadyExists;
    case ValidProjectRootQueryResult::No_ProjectRootIsNonDirectory:
        return UncertainProjectCreationResult::Failed_ProjectRootIsNonDirectory;
    case ValidProjectRootQueryResult::No_ProjectRootDoesNotExist:
        return UncertainProjectCreationResult::Failed_ProjectRootDoesNotExist;
    case ValidProjectRootQueryResult::No_DotMeddyprojectIsNonDirectory:
        return UncertainProjectCreationResult::Failed_DotMeddyprojectAlreadyExistsAndIsNonDirectory;
    }

    // We know that the .meddyproject directory already exists at this point. That's all that's relevant to this function.
    return UncertainProjectCreationResult::Failed_DotMeddyprojectAlreadyExists;
}

MeddySDK::ProjectCreationResult MeddySDK::CreateNewProject(
    boost::filesystem::path&& projectRootPath)
{
    assert(QueryWhetherPathIsValidProjectRoot(boost::filesystem::path(projectRootPath)) == ValidProjectRootQueryResult::No_DotMeddyprojectDoesNotExist);

    boost::filesystem::path dotMeddyprojectPath = ProjectRootToDotMeddyprojectPath(std::move(projectRootPath));

    const bool didCreateDotMeddyproject = boost::filesystem::create_directory(dotMeddyprojectPath);
    if (!didCreateDotMeddyproject)
    {
        return ProjectCreationResult::Failed_FilesystemFailedToCreateDotMeddyproject;
    }

    boost::filesystem::path manifestFilePath = DotMeddyprojectToManifestFilePath(std::move(dotMeddyprojectPath));
    std::ofstream manifestFileStream = std::ofstream(manifestFilePath.c_str());
    manifestFileStream.flush();
    manifestFileStream.close();

    if (manifestFileStream.fail())
    {
        return ProjectCreationResult::Failed_FilesystemFailedToCreateManifestFile;
    }

    return ProjectCreationResult::Success;
}

bool MeddySDK::IsDotMeddyprojectPath(const boost::filesystem::path& filesystemPath)
{
    boost::filesystem::path pathLeafName = filesystemPath.filename();
    const boost::filesystem::path::string_type& pathLeafNameString = pathLeafName.native();

    // The path's string could be storing different char types, so we need to copy and convert them to a new character
    // buffer here before comparing them. It looks complicated because we are copying the string onto the stack to
    // avoiding a free-store allocation.
    CppUtils::CharBufferString<char, MeddySDK::MaxFilenameLength> pathLeafNameCharBuffer =
        CppUtils::ConstructCharacterBufferFromString<char, MeddySDK::MaxFilenameLength>(
            CppUtils::MakeStringView(pathLeafNameString));

    return pathLeafNameCharBuffer.ToStringView() == DotMeddyprojectString;
}

CppUtils::ExpectedResult<MeddySDK::Meddyproject, MeddySDK::Error_GetOuterDotMeddyprojectPath> MeddySDK::GetOuterMeddyproject(boost::filesystem::path&& filesystemPath)
{
    if (!boost::filesystem::exists(filesystemPath))
    {
        return Error_GetOuterDotMeddyprojectPath::PathDoesntExist;
    }

    if (!boost::filesystem::is_directory(filesystemPath))
    {
        assert(filesystemPath.has_parent_path()); // I believe this isn't possible, since we already determined it's not a directory, it must be in a directory.

        filesystemPath = std::move(filesystemPath).parent_path();
    }

    // Traverse up the parent directories until we see that a .meddyproject dir exists.
    for (boost::filesystem::path currentDir = std::move(filesystemPath); currentDir.has_parent_path(); currentDir = std::move(currentDir).parent_path())
    {
        boost::filesystem::path potentialDotMeddyproject = ProjectRootToDotMeddyprojectPath(boost::filesystem::path(currentDir));
        if (boost::filesystem::exists(potentialDotMeddyproject))
        {
            return MeddySDK::Meddyproject{std::move(currentDir)};
        }
    }

    return Error_GetOuterDotMeddyprojectPath::NoDotMeddyprojectFound;
}

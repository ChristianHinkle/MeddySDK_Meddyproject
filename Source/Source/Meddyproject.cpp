// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK_Meddyproject/Meddyproject.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <utility>
#include <cassert>
#include <fstream>
#include <CppUtils_Misc/CharBufferString.h>
#include <CppUtils_Misc/Filesystem.h>
#include <MeddySDK_Meddyproject/FilesystemUtils.h>
#include <CppUtils_Misc/String.h>

using namespace MeddySDK::Meddyproject;

boost::filesystem::path MeddySDK::Meddyproject::ProjectRootToManifestFilePath(
    boost::filesystem::path&& path)
{
    return DotMeddyprojectToManifestFilePath(
        ProjectRootToDotMeddyprojectPath(
            std::move(path)));
}

boost::filesystem::path MeddySDK::Meddyproject::ProjectRootToDotMeddyprojectPath(
    boost::filesystem::path&& path)
{
    path.append(DotMeddyprojectString);
    return std::move(path);
}

boost::filesystem::path MeddySDK::Meddyproject::DotMeddyprojectToProjectRootPath(
    boost::filesystem::path&& path)
{
    assert(IsDotMeddyprojectPath(path));
    return std::move(path).parent_path();
}

boost::filesystem::path MeddySDK::Meddyproject::DotMeddyprojectToManifestFilePath(
    boost::filesystem::path&& path)
{
    assert(IsDotMeddyprojectPath(path));
    path.append(ManifestFilenameString);
    return std::move(path);
}

ValidProjectRootQueryResult MeddySDK::Meddyproject::QueryWhetherPathIsValidProjectRoot(
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

UncertainProjectCreationResult MeddySDK::Meddyproject::TryCreateNewProject(
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

ProjectCreationResult MeddySDK::Meddyproject::CreateNewProject(
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

bool MeddySDK::Meddyproject::IsDotMeddyprojectPath(const boost::filesystem::path& filesystemPath)
{
    boost::filesystem::path pathLeafName = filesystemPath.filename();
    const boost::filesystem::path::string_type& pathLeafNameString = pathLeafName.native();

    // The path's string could be storing different char types, so we need to copy and convert them to a new character
    // buffer here before comparing them. It looks complicated because we are copying the string onto the stack to
    // avoiding a free-store allocation.
    CppUtils::Misc::CharBufferString<char, FilesystemUtils::MaxFilenameLength> pathLeafNameCharBuffer =
        CppUtils::Misc::String::ConstructCharacterBufferFromString<char, FilesystemUtils::MaxFilenameLength>(
            CppUtils::Misc::String::MakeStringView(pathLeafNameString));

    return pathLeafNameCharBuffer.GetStringView() == DotMeddyprojectString;
}

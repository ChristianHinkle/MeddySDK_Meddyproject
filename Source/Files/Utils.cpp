// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK/Meddyproject/Utils.h>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <utility>
#include <cassert>
#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Core/Filesystem.h>
#include <MeddySDK/Meddyproject/FilesystemUtils.h>
#include <CppUtils/Misc/String.h>
#include <CppUtils/Core/String.h>
#include <rapidjson/document.h>
#include <MeddySDK/Meddyproject/Json.h>
#include <MeddySDK/Meddyproject/Json.inl>

boost::filesystem::path MeddySDK::ProjectRootToManifestFilePath(
    boost::filesystem::path&& path)
{
    return MeddyprojectDirToManifestFilePath(
        ProjectRootToMeddyprojectDirPath(
            std::move(path)));
}

boost::filesystem::path MeddySDK::ProjectRootToMeddyprojectDirPath(
    boost::filesystem::path&& path)
{
    path.append(MeddyprojectDirString);
    return std::move(path);
}

boost::filesystem::path MeddySDK::MeddyprojectDirToProjectRootPath(
    boost::filesystem::path&& path)
{
    assert(IsMeddyprojectDirPath(path));
    return std::move(path).parent_path();
}

boost::filesystem::path MeddySDK::MeddyprojectDirToManifestFilePath(
    boost::filesystem::path&& path)
{
    assert(IsMeddyprojectDirPath(path));
    path.append(MeddyprojectManifestFilenameString);
    return std::move(path);
}

bool MeddySDK::IsValidProjectRoot(boost::filesystem::path&& projectRootPath)
{
    ValidProjectRootQueryResult result = QueryWhetherPathIsValidProjectRoot(std::move(projectRootPath));
    return result == ValidProjectRootQueryResult::Yes_IsValidProjectRoot;
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

    boost::filesystem::path meddyprojectDirPath = ProjectRootToMeddyprojectDirPath(std::move(projectRootPath));

    {
        const boost::filesystem::file_status meddyprojectDirStatus = boost::filesystem::status(meddyprojectDirPath);

        if (!boost::filesystem::exists(meddyprojectDirStatus))
        {
            return ValidProjectRootQueryResult::No_MeddyprojectDirDoesNotExist;
        }

        if (!boost::filesystem::is_directory(meddyprojectDirStatus))
        {
            return ValidProjectRootQueryResult::No_MeddyprojectDirIsNonDirectory;
        }
    }

    boost::filesystem::path manifestFilePath = MeddyprojectDirToManifestFilePath(std::move(meddyprojectDirPath));

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

    // We can only create a project if there is no _meddyproject directory whatsoever.
    if (projectRootQueryResult == ValidProjectRootQueryResult::No_MeddyprojectDirDoesNotExist)
    {
        const ProjectCreationResult projectCreationResult = CreateNewProject(std::move(projectRootPath));

        switch (projectCreationResult)
        {
        case ProjectCreationResult::Failed_FilesystemFailedToCreateMeddyprojectDir:
            return UncertainProjectCreationResult::Failed_FilesystemFailedToCreateMeddyprojectDir;
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
    case ValidProjectRootQueryResult::No_MeddyprojectDirIsNonDirectory:
        return UncertainProjectCreationResult::Failed_MeddyprojectDirAlreadyExistsAndIsNonDirectory;
    }

    // We know that the _meddyproject directory already exists at this point. That's all that's relevant to this function.
    return UncertainProjectCreationResult::Failed_MeddyprojectDirAlreadyExists;
}

MeddySDK::ProjectCreationResult MeddySDK::CreateNewProject(
    boost::filesystem::path&& projectRootPath)
{
    assert(QueryWhetherPathIsValidProjectRoot(boost::filesystem::path(projectRootPath)) == ValidProjectRootQueryResult::No_MeddyprojectDirDoesNotExist);

    boost::filesystem::path meddyprojectDirPath = ProjectRootToMeddyprojectDirPath(std::move(projectRootPath));

    const bool didCreateMeddyprojectDir = boost::filesystem::create_directory(meddyprojectDirPath);
    if (!didCreateMeddyprojectDir)
    {
        return ProjectCreationResult::Failed_FilesystemFailedToCreateMeddyprojectDir;
    }

    boost::filesystem::path manifestFilePath = MeddyprojectDirToManifestFilePath(std::move(meddyprojectDirPath));

    const bool didCreateFile = CppUtils::TouchNewFile(manifestFilePath.native());
    if (!didCreateFile)
    {
        return ProjectCreationResult::Failed_FilesystemFailedToCreateManifestFile;
    }

    // Populate the json file.
    {
        // Write the default json contents.
        rapidjson::Document jsonDocument{};
        jsonDocument.SetObject();

        // Serialize this json document to the file.
        const bool didSaveToFile = MeddySDK::SaveJsonDocumentToFile(CppUtils::StdPathStringView{manifestFilePath.native()}, jsonDocument);
        assert(didSaveToFile);
    }

    return ProjectCreationResult::Success;
}

bool MeddySDK::IsMeddyprojectDirPath(const boost::filesystem::path& filesystemPath)
{
    boost::filesystem::path pathLeafName = filesystemPath.filename();
    return MeddySDK::IsPathEqualToString(pathLeafName, MeddyprojectDirString);
}

CppUtils::ExpectedResult<MeddySDK::Meddyproject, MeddySDK::Error_GetOuterMeddyprojectDirPath> MeddySDK::GetOuterMeddyproject(boost::filesystem::path&& filesystemPath)
{
    // TODO: Make version of this function which allows it not to exist.
    if (!boost::filesystem::exists(filesystemPath))
    {
        return Error_GetOuterMeddyprojectDirPath::PathDoesntExist;
    }

    if (!boost::filesystem::is_directory(filesystemPath))
    {
        assert(filesystemPath.has_parent_path()); // I believe this isn't possible, since we already determined it's not a directory, it must be in a directory.

        filesystemPath = std::move(filesystemPath).parent_path();
    }

    // Traverse up the parent directories until we see that a _meddyproject dir exists.
    for (boost::filesystem::path currentDir = std::move(filesystemPath); currentDir.has_parent_path(); currentDir = std::move(currentDir).parent_path())
    {
        boost::filesystem::path potentialMeddyprojectDir = ProjectRootToMeddyprojectDirPath(boost::filesystem::path(currentDir));
        if (boost::filesystem::exists(potentialMeddyprojectDir))
        {
            return MeddySDK::Meddyproject{std::move(currentDir)};
        }
    }

    return Error_GetOuterMeddyprojectDirPath::NoMeddyprojectDirFound;
}

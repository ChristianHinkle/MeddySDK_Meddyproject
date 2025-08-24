// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK_Meddyproject/Meddyproject.h>

#include <filesystem>
#include <utility>
#include <cassert>
#include <fstream>
#include <MeddySDK_Meddyproject/CharBufferString.h>
#include <MeddySDK_Meddyproject/FilesystemUtils.h>

using namespace MeddySDK::Meddyproject;

std::filesystem::path MeddySDK::Meddyproject::ProjectRootToManifestFilePath(
    std::filesystem::path&& path)
{
    return DotMeddyprojectToManifestFilePath(
        ProjectRootToDotMeddyprojectPath(
            std::move(path)));
}

std::filesystem::path MeddySDK::Meddyproject::ProjectRootToDotMeddyprojectPath(
    std::filesystem::path&& path)
{
    path.append(DotMeddyprojectString);
    return std::move(path);
}

std::filesystem::path MeddySDK::Meddyproject::DotMeddyprojectToProjectRootPath(
    std::filesystem::path&& path)
{
    assert(IsDotMeddyprojectPath(path));
    return std::move(path).parent_path();
}

std::filesystem::path MeddySDK::Meddyproject::DotMeddyprojectToManifestFilePath(
    std::filesystem::path&& path)
{
    assert(IsDotMeddyprojectPath(path));
    path.append(ManifestFilenameString);
    return std::move(path);
}

ValidProjectRootQueryResult MeddySDK::Meddyproject::QueryWhetherPathIsValidProjectRoot(
    std::filesystem::path&& projectRootPath)
{
    {
        const std::filesystem::file_status projectRootStatus = std::filesystem::status(projectRootPath);

        if (!std::filesystem::exists(projectRootStatus))
        {
            return ValidProjectRootQueryResult::No_ProjectRootDoesNotExist;
        }

        if (!std::filesystem::is_directory(projectRootStatus))
        {
            return ValidProjectRootQueryResult::No_ProjectRootIsNonDirectory;
        }
    }

    std::filesystem::path dotMeddyprojectPath = ProjectRootToDotMeddyprojectPath(std::move(projectRootPath));

    {
        const std::filesystem::file_status dotMeddyprojectStatus = std::filesystem::status(dotMeddyprojectPath);

        if (!std::filesystem::exists(dotMeddyprojectStatus))
        {
            return ValidProjectRootQueryResult::No_DotMeddyprojectDoesNotExist;
        }

        if (!std::filesystem::is_directory(dotMeddyprojectStatus))
        {
            return ValidProjectRootQueryResult::No_DotMeddyprojectIsNonDirectory;
        }
    }

    std::filesystem::path manifestFilePath = DotMeddyprojectToManifestFilePath(std::move(dotMeddyprojectPath));

    {
        const std::filesystem::file_status manifestFileStatus = std::filesystem::status(manifestFilePath);

        if (!std::filesystem::exists(manifestFileStatus))
        {
            return ValidProjectRootQueryResult::No_ManifestFileDoesNotExist;
        }

        if (std::filesystem::is_directory(manifestFileStatus))
        {
            return ValidProjectRootQueryResult::No_ManifestFileIsDirectory;
        }
    }

    return ValidProjectRootQueryResult::Yes_IsValidProjectRoot;
}

UncertainProjectCreationResult MeddySDK::Meddyproject::TryCreateNewProject(
    std::filesystem::path&& projectRootPath)
{
    const ValidProjectRootQueryResult projectRootQueryResult = QueryWhetherPathIsValidProjectRoot(std::filesystem::path(projectRootPath));

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
    std::filesystem::path&& projectRootPath)
{
    assert(QueryWhetherPathIsValidProjectRoot(std::filesystem::path(projectRootPath)) == ValidProjectRootQueryResult::No_DotMeddyprojectDoesNotExist);

    std::filesystem::path dotMeddyprojectPath = ProjectRootToDotMeddyprojectPath(std::move(projectRootPath));

    const bool didCreateDotMeddyproject = std::filesystem::create_directory(dotMeddyprojectPath);
    if (!didCreateDotMeddyproject)
    {
        return ProjectCreationResult::Failed_FilesystemFailedToCreateDotMeddyproject;
    }

    std::filesystem::path manifestFilePath = DotMeddyprojectToManifestFilePath(std::move(dotMeddyprojectPath));
    std::ofstream manifestFileStream(std::move(manifestFilePath));
    manifestFileStream.flush();
    manifestFileStream.close();

    if (manifestFileStream.fail())
    {
        return ProjectCreationResult::Failed_FilesystemFailedToCreateManifestFile;
    }

    return ProjectCreationResult::Success;
}

bool MeddySDK::Meddyproject::IsDotMeddyprojectPath(const std::filesystem::path& filesystemPath)
{
    std::filesystem::path pathLeafName = filesystemPath.filename();

    // Note: We're avoid free-store string allocation here, but the code looks a little bit busy which I don't
    // prefer. Maybe consider enabling the boost libraries again, because the standard library is extremely limited with
    // this kind of stuff.

    CharBufferString<char, FilesystemUtils::MaxFilenameLength> pathLeafNameCharBuffer =
        FilesystemUtils::ConstructCharacterBufferFromPath<FilesystemUtils::MaxFilenameLength, char>(pathLeafName);

    return pathLeafNameCharBuffer.GetStringView() == DotMeddyprojectString;
}

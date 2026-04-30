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
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>
#include <MeddySDK/Meddyproject/Json.h>

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

        CppUtils::CharBufferString manifestFilePathConverted = CppUtils::ConstructCharacterBufferFromString<char, MeddySDK::MaxFilenameLength>(
            CppUtils::StdPathStringView{manifestFilePath.native()});

        std::FILE* manifestFilePtr = std::fopen(manifestFilePathConverted.ToStringView().data(), "wb");
        assert(manifestFilePtr);

        constexpr std::size_t jsonWriteBufferSize = 65536u;
        char jsonWriteBuffer[jsonWriteBufferSize];
        rapidjson::FileWriteStream jsonFileWriteStream{manifestFilePtr, jsonWriteBuffer, jsonWriteBufferSize};

        rapidjson::PrettyWriter<rapidjson::FileWriteStream> jsonWriter{jsonFileWriteStream};
        MeddySDK::ApplyPrettyJsonDefaults(jsonWriter);

        jsonDocument.Accept(jsonWriter);

        // Write a newline at end of file.
        jsonFileWriteStream.Put('\n');
        jsonFileWriteStream.Flush();

        std::fclose(manifestFilePtr);
    }

    return ProjectCreationResult::Success;
}

bool MeddySDK::IsDotMeddyprojectPath(const boost::filesystem::path& filesystemPath)
{
    boost::filesystem::path pathLeafName = filesystemPath.filename();
    return MeddySDK::IsPathEqualToString(pathLeafName, DotMeddyprojectString);
}

CppUtils::ExpectedResult<MeddySDK::Meddyproject, MeddySDK::Error_GetOuterDotMeddyprojectPath> MeddySDK::GetOuterMeddyproject(boost::filesystem::path&& filesystemPath)
{
    // TODO: Make version of this function which allows it not to exist.
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

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include "meddysdk_meddyproject_export.h"
#include <filesystem>
#include <string_view>

#define MEDDYSDK_MEDDYPROJECT_DOT_MEDDYPROJECT_STRING_LITERAL ".meddyproject"

#define MEDDYSDK_MEDDYPROJECT_MANIFEST_FILENAME_STRING_LITERAL "Manifest.json"

/**
 * @brief General utilities for working with meddyprojects and their files.
 * @remark Note: Maybe we can change this heavy usage of enums to callback functions that
 *         get called in each specific case, as that would provide more data to the
 *         caller per case.
 */
namespace MeddySDK::Meddyproject
{
    constexpr std::string_view DotMeddyprojectString =
        MEDDYSDK_MEDDYPROJECT_DOT_MEDDYPROJECT_STRING_LITERAL;

    constexpr std::string_view ManifestFilenameString =
        MEDDYSDK_MEDDYPROJECT_MANIFEST_FILENAME_STRING_LITERAL;

    MEDDYSDK_MEDDYPROJECT_EXPORT std::filesystem::path ProjectRootToManifestFilePath(
        std::filesystem::path&& path);

    MEDDYSDK_MEDDYPROJECT_EXPORT std::filesystem::path ProjectRootToDotMeddyprojectPath(
        std::filesystem::path&& path);

    MEDDYSDK_MEDDYPROJECT_EXPORT std::filesystem::path DotMeddyprojectToProjectRootPath(
        std::filesystem::path&& path);

    MEDDYSDK_MEDDYPROJECT_EXPORT std::filesystem::path DotMeddyprojectToManifestFilePath(
        std::filesystem::path&& path);

    enum class ValidProjectRootQueryResult : unsigned char
    {
        Yes_IsValidProjectRoot,
        No_ProjectRootDoesNotExist,
        No_ProjectRootIsNonDirectory,
        No_DotMeddyprojectDoesNotExist,
        No_DotMeddyprojectIsNonDirectory,
        No_ManifestFileDoesNotExist,
        No_ManifestFileIsDirectory
    };

    enum class UncertainProjectCreationResult : unsigned char
    {
        Success,
        Failed_ValidProjectAlreadyExists,
        Failed_ProjectRootIsNonDirectory,
        Failed_ProjectRootDoesNotExist,
        Failed_DotMeddyprojectAlreadyExists,
        Failed_DotMeddyprojectAlreadyExistsAndIsNonDirectory,
        Failed_FilesystemFailedToCreateDotMeddyproject,
        Failed_FilesystemFailedToCreateManifestFile
    };

    enum class ProjectCreationResult : unsigned char
    {
        Success,
        Failed_FilesystemFailedToCreateDotMeddyproject,
        Failed_FilesystemFailedToCreateManifestFile
    };

    MEDDYSDK_MEDDYPROJECT_EXPORT ValidProjectRootQueryResult QueryWhetherPathIsValidProjectRoot(
        std::filesystem::path&& projectRootPath);

    MEDDYSDK_MEDDYPROJECT_EXPORT UncertainProjectCreationResult TryCreateNewProject(std::filesystem::path&& projectRootPath);

    MEDDYSDK_MEDDYPROJECT_EXPORT ProjectCreationResult CreateNewProject(std::filesystem::path&& projectRootPath);

    MEDDYSDK_MEDDYPROJECT_EXPORT bool IsDotMeddyprojectPath(const std::filesystem::path& filesystemPath);
}

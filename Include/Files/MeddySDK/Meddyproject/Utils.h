// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <boost/filesystem/path.hpp>
#include <string_view>
#include <CppUtils/Core/ExpectedResult.h>
#include <MeddySDK/Meddyproject/Meddyproject.h>

#define MEDDYSDK_MEDDYPROJECT_DIR_STRING_LITERAL "_meddyproject"

#define MEDDYSDK_MEDDYPROJECT_MANIFEST_FILENAME_STRING_LITERAL "manifest.json"

/**
 * @brief General utilities for working with meddyprojects and their files.
 * @remark Note: Maybe we can change this heavy usage of enums to callback functions that
 *         get called in each specific case, as that would provide more data to the
 *         caller per case.
 */
namespace MeddySDK
{
    constexpr std::string_view MeddyprojectDirString =
        MEDDYSDK_MEDDYPROJECT_DIR_STRING_LITERAL;

    constexpr std::string_view MeddyprojectManifestFilenameString =
        MEDDYSDK_MEDDYPROJECT_MANIFEST_FILENAME_STRING_LITERAL;

    MEDDYSDK_MEDDYPROJECT_EXPORT boost::filesystem::path ProjectRootToManifestFilePath(
        boost::filesystem::path&& path);

    MEDDYSDK_MEDDYPROJECT_EXPORT boost::filesystem::path ProjectRootToMeddyprojectDirPath(
        boost::filesystem::path&& path);

    MEDDYSDK_MEDDYPROJECT_EXPORT boost::filesystem::path MeddyprojectDirToProjectRootPath(
        boost::filesystem::path&& path);

    MEDDYSDK_MEDDYPROJECT_EXPORT boost::filesystem::path MeddyprojectDirToManifestFilePath(
        boost::filesystem::path&& path);

    enum class ValidProjectRootQueryResult : unsigned char
    {
        Yes_IsValidProjectRoot,
        No_ProjectRootDoesNotExist,
        No_ProjectRootIsNonDirectory,
        No_MeddyprojectDirDoesNotExist,
        No_MeddyprojectDirIsNonDirectory,
        No_ManifestFileDoesNotExist,
        No_ManifestFileIsDirectory
    };

    enum class UncertainProjectCreationResult : unsigned char
    {
        Success,
        Failed_ValidProjectAlreadyExists,
        Failed_ProjectRootIsNonDirectory,
        Failed_ProjectRootDoesNotExist,
        Failed_MeddyprojectDirAlreadyExists,
        Failed_MeddyprojectDirAlreadyExistsAndIsNonDirectory,
        Failed_FilesystemFailedToCreateMeddyprojectDir,
        Failed_FilesystemFailedToCreateManifestFile
    };

    enum class ProjectCreationResult : unsigned char
    {
        Success,
        Failed_FilesystemFailedToCreateMeddyprojectDir,
        Failed_FilesystemFailedToCreateManifestFile
    };

    MEDDYSDK_MEDDYPROJECT_EXPORT bool IsValidProjectRoot(boost::filesystem::path&& projectRootPath);

    MEDDYSDK_MEDDYPROJECT_EXPORT ValidProjectRootQueryResult QueryWhetherPathIsValidProjectRoot(
        boost::filesystem::path&& projectRootPath);

    MEDDYSDK_MEDDYPROJECT_EXPORT UncertainProjectCreationResult TryCreateNewProject(boost::filesystem::path&& projectRootPath);

    MEDDYSDK_MEDDYPROJECT_EXPORT ProjectCreationResult CreateNewProject(boost::filesystem::path&& projectRootPath);

    MEDDYSDK_MEDDYPROJECT_EXPORT bool IsMeddyprojectDirPath(const boost::filesystem::path& filesystemPath);

    enum class Error_GetOuterMeddyprojectDirPath : unsigned char
    {
        PathDoesntExist,
        NoMeddyprojectDirFound
    };

    /**
     * @brief Get path to the outer meddyproject's _meddyproject dir.
     */
    MEDDYSDK_MEDDYPROJECT_EXPORT CppUtils::ExpectedResult<MeddySDK::Meddyproject, Error_GetOuterMeddyprojectDirPath> GetOuterMeddyproject(boost::filesystem::path&& filesystemPath);
}

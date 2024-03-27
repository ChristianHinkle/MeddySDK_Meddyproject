// Copyright (c) 2023-2024 Christian Hinkle, Brian Hinkle.

#pragma once

#include "meddysdk_meddyproject_export.h"
#include <cstddef>
#include <filesystem>

#define MEDDYSDK_MEDDYPROJECT_PROJECTPATH_PROJECT_DIRECTORY_CSTRING ".meddyproject"

/**
 * @brief Filesystem path utilities for meddyprojects.
 */
namespace MeddySDK::Meddyproject::ProjectPath
{
    constexpr char ProjectDirectoryCstring[] =
        MEDDYSDK_MEDDYPROJECT_PROJECTPATH_PROJECT_DIRECTORY_CSTRING;

    constexpr std::size_t ProjectDirectoryCstringLen =
        (sizeof(ProjectDirectoryCstring) / sizeof(char)) - 1;

    /**
     * @brief Makes a path to the meddyproject for the given directory path.
     *
     * @param inDirectoryPathCopy The parent directory for meddyproject path to return.
     *
     * @return The path to the meddyproject directory.
     */
    MEDDYSDK_MEDDYPROJECT_EXPORT std::filesystem::path MakeProjectPath(
        std::filesystem::path inDirectoryPathCopy);
}

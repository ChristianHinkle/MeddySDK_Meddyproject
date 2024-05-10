// Copyright (c) 2023-2024 Christian Hinkle, Brian Hinkle.

#pragma once

#include "meddysdk_meddyproject_export.h"
#include <cstddef>
#include <filesystem>
#include <string_view>

#define MEDDYSDK_MEDDYPROJECT_PROJECTPATH_PROJECT_DIRECTORY_STRING_LITERAL ".meddyproject"

/**
 * @brief Filesystem path utilities for meddyprojects.
 */
namespace MeddySDK::Meddyproject::ProjectPath
{
    constexpr std::string_view ProjectDirectoryString =
        MEDDYSDK_MEDDYPROJECT_PROJECTPATH_PROJECT_DIRECTORY_STRING_LITERAL;

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

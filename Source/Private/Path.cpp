// Copyright (c) 2023-2024 Christian Hinkle, Brian Hinkle.

#include "Path.h"

#include <filesystem>

std::filesystem::path MeddySDK::Meddyproject::Path::MakeProjectPath(
    std::filesystem::path inDirectoryPath)
{
    inDirectoryPath.append(ProjectDirectoryString);
    return inDirectoryPath;
}

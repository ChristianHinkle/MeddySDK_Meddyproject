// Copyright (c) 2023-2024 Christian Hinkle, Brian Hinkle.

#include "ProjectPath.h"

#include <filesystem>

std::filesystem::path MeddySDK::Meddyproject::ProjectPath::MakeProjectPath(
    std::filesystem::path inDirectoryPathCopy)
{
    return inDirectoryPathCopy.append(ProjectDirectoryString);
}

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK/Meddyproject/Meddyproject.h>

#include <MeddySDK/Meddyproject/Utils.h>
#include <filesystem>
#include <utility>
#include <cassert>
#include <fstream>
#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Core/Filesystem.h>
#include <MeddySDK/Meddyproject/FilesystemUtils.h>
#include <CppUtils/Misc/String.h>
#include <CppUtils/Core/String.h>
#include <iostream>

MeddySDK::Meddyproject::Meddyproject(std::filesystem::path&& projectRootDir)
    : ProjectRootDir{std::move(projectRootDir)}
{
    if (!MeddySDK::IsValidProjectRoot(std::filesystem::path{ProjectRootDir}))
    {
        std::cerr << "Error: " << "Tried constructing a meddyproject struct with a non-meddyproject path!" << '\n';
        assert(false);
    }
}

const std::filesystem::path& MeddySDK::Meddyproject::GetRootPath() const &
{
    return ProjectRootDir;
}
std::filesystem::path MeddySDK::Meddyproject::GetRootPath() &&
{
    std::filesystem::path& result = const_cast<std::filesystem::path&>(GetRootPath());
    return std::move(result);
}

std::filesystem::path MeddySDK::Meddyproject::GetMeddyprojectDirPath() const &
{
    return MeddySDK::ProjectRootToMeddyprojectDirPath(std::filesystem::path{ProjectRootDir});
}
std::filesystem::path MeddySDK::Meddyproject::GetMeddyprojectDirPath() &&
{
    return MeddySDK::ProjectRootToMeddyprojectDirPath(std::move(ProjectRootDir));
}

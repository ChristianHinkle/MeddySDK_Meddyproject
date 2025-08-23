// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <FilesystemUtils.h>

#include <filesystem>
#include <string>
#include <algorithm>

using namespace MeddySDK::Meddyproject;

FilesystemUtils::StdPathStringView FilesystemUtils::GetStringViewFromPath(const std::filesystem::path& path)
{
    return StdPathStringView(path.native().c_str(), path.native().length());
}

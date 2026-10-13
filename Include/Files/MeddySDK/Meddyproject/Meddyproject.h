// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <filesystem>
#include <string_view>
#include <utility>

namespace MeddySDK
{
    struct MEDDYSDK_MEDDYPROJECT_EXPORT Meddyproject
    {
    public:
        explicit Meddyproject(std::filesystem::path&& projectRootDir);

    public:
        const std::filesystem::path& GetRootPath() const &;
        std::filesystem::path GetRootPath() &&;
        std::filesystem::path GetMeddyprojectDirPath() const &;
        std::filesystem::path GetMeddyprojectDirPath() &&;

    private:

        std::filesystem::path ProjectRootDir;
    };
}

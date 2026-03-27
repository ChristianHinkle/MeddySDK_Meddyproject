// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <boost/filesystem/path.hpp>
#include <string_view>
#include <utility>

namespace MeddySDK
{
    struct MEDDYSDK_MEDDYPROJECT_EXPORT Meddyproject
    {
    public:
        explicit Meddyproject(boost::filesystem::path&& projectRootDir);

    public:
        const boost::filesystem::path& GetRootPath() const &;
        boost::filesystem::path GetRootPath() &&;
        boost::filesystem::path GetDotMeddyprojectPath() const &;
        boost::filesystem::path GetDotMeddyprojectPath() &&;

    private:

        boost::filesystem::path ProjectRootDir;
    };
}

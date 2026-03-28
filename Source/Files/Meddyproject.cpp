// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK/Meddyproject/Meddyproject.h>

#include <MeddySDK/Meddyproject/Utils.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <utility>
#include <cassert>
#include <fstream>
#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Core/Filesystem.h>
#include <MeddySDK/Meddyproject/FilesystemUtils.h>
#include <CppUtils/Misc/String.h>
#include <CppUtils/Core/String.h>
#include <iostream>

MeddySDK::Meddyproject::Meddyproject(boost::filesystem::path&& projectRootDir)
    : ProjectRootDir{std::move(projectRootDir)}
{
    if (!MeddySDK::IsValidProjectRoot(boost::filesystem::path{ProjectRootDir}))
    {
        std::cerr << "Error: " << "Tried constructing a meddyproject struct with a non-meddyproject path!" << '\n';
        assert(false);
    }
}

const boost::filesystem::path& MeddySDK::Meddyproject::GetRootPath() const &
{
    return ProjectRootDir;
}
boost::filesystem::path MeddySDK::Meddyproject::GetRootPath() &&
{
    boost::filesystem::path& result = const_cast<boost::filesystem::path&>(GetRootPath());
    return std::move(result);
}

boost::filesystem::path MeddySDK::Meddyproject::GetDotMeddyprojectPath() const &
{
    return MeddySDK::ProjectRootToDotMeddyprojectPath(boost::filesystem::path{ProjectRootDir});
}
boost::filesystem::path MeddySDK::Meddyproject::GetDotMeddyprojectPath() &&
{
    return MeddySDK::ProjectRootToDotMeddyprojectPath(std::move(ProjectRootDir));
}

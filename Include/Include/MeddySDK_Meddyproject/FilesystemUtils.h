// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <boost/filesystem/path.hpp>
#include <CppUtils_Misc/String.h>
#include <string_view>
#include <CppUtils_Misc/CharBufferString.h>
#include <algorithm>
#include <CppUtils_Misc/String.h>

/**
 * @brief String literal alternative to `MeddySDK::Meddyproject::FilesystemUtils::CrossPlatformPathSeparator`.
 */
#define MEDDYSDK_MEDDYPROJECT_FILESYSTEMUTILS_CROSSPLATFORM_PATH_SEPARATOR '/'

/**
 * @brief Filesystem utilities to extend the default std::filesystem tools.
 */
namespace MeddySDK::Meddyproject::FilesystemUtils
{
    /**
     * @brief A character represents the most "cross platform" path separator.
     */
    constexpr char CrossPlatformPathSeparator = MEDDYSDK_MEDDYPROJECT_FILESYSTEMUTILS_CROSSPLATFORM_PATH_SEPARATOR;

    /**
     * @brief An arbitrarily defined number that determines the maximum supported filesystem filename length.
     * @note This is not the pathname limit, but the leaf file/directory name at the very end of the path.
     */
    constexpr std::size_t MaxFilenameLength = 1024;

    template <std::size_t bufferSize, class TChar = char, class TCharTraits = std::char_traits<TChar>>
    CppUtils::Misc::CharBufferString<TChar, bufferSize, TCharTraits> ConstructCrossPlatformPathCharacterBuffer(const boost::filesystem::path& path);

    template <class TFwdIt, class TChar = char>
    void ConvertPathStringToCrossPlatformFormat(TFwdIt begin, TFwdIt end);
}

template <std::size_t bufferSize, class TChar, class TCharTraits>
CppUtils::Misc::CharBufferString<TChar, bufferSize, TCharTraits> MeddySDK::Meddyproject::FilesystemUtils::ConstructCrossPlatformPathCharacterBuffer(
    const boost::filesystem::path& path)
{
    return CppUtils::Misc::CharBufferString<TChar, bufferSize, TCharTraits>(
        [&path](CppUtils::Misc::CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer)
        {
            CppUtils::Misc::String::AppendStringToCharacterBuffer(
                characterBuffer,
                CppUtils::Misc::String::MakeStringView(path.native()));

            ConvertPathStringToCrossPlatformFormat(characterBuffer.begin(), characterBuffer.end());
        }
        );
}

template <class TFwdIt, class TChar>
void MeddySDK::Meddyproject::FilesystemUtils::ConvertPathStringToCrossPlatformFormat(TFwdIt begin, TFwdIt end)
{
    std::replace(
        begin,
        end,
        static_cast<TChar>(boost::filesystem::path::preferred_separator),
        static_cast<TChar>(CrossPlatformPathSeparator));
}

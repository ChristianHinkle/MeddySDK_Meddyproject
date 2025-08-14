// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include "meddysdk_meddyproject_export.h"
#include <filesystem>
#include "StringUtils.h"
#include <string_view>
#include "CharBufferString.h"

/**
 * @brief String literal alternative to `MeddySDK::Meddyproject::FilesystemUtils::CrossPlatformPathSeparator`.
 */
#define MEDDYSDK_MEDDYPROJECT_FILESYSTEMUTILS_CROSSPLATFORM_PATH_SEPARATOR '/'

/**
 * @brief Filesystem utilities to extend the default std::filesystem tools.
 */
namespace MeddySDK::Meddyproject::FilesystemUtils
{
    using StdPathStringView = std::basic_string_view<const std::filesystem::path::value_type>;

    /**
     * @brief A character represents the most "cross platform" path separator.
     */
    constexpr char CrossPlatformPathSeparator = MEDDYSDK_MEDDYPROJECT_FILESYSTEMUTILS_CROSSPLATFORM_PATH_SEPARATOR;

    /**
     * @brief An arbitrarily defined number that determines the maximum supported filesystem filename length.
     * @note This is not the pathname limit, but the leaf file/directory name at the very end of the path.
     */
    constexpr std::size_t MaxFilenameLength = 1024;

    MEDDYSDK_MEDDYPROJECT_EXPORT StdPathStringView GetStringViewFromPath(const std::filesystem::path& path);

    template <std::size_t bufferSize, class TChar = char, class TCharTraits = std::char_traits<TChar>>
    CharBufferString<TChar, bufferSize, TCharTraits> ConstructCrossPlatformPathCharacterBuffer(const std::filesystem::path& path);

    template <class TFwdIt, class TChar = char>
    void ConvertPathStringToCrossPlatformFormat(TFwdIt begin, TFwdIt end);

    template <std::size_t bufferSize, class TChar = char, class TCharTraits = std::char_traits<TChar>>
    CharBufferString<TChar, bufferSize, TCharTraits> ConstructCharacterBufferFromPath(const std::filesystem::path& path);

    template <std::size_t bufferSize, class TChar = char, class TCharTraits = std::char_traits<TChar>>
    void AppendPathToCharacterBuffer(
        CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer,
        const std::filesystem::path& path);
}

template <std::size_t bufferSize, class TChar, class TCharTraits>
MeddySDK::Meddyproject::CharBufferString<TChar, bufferSize, TCharTraits> MeddySDK::Meddyproject::FilesystemUtils::ConstructCrossPlatformPathCharacterBuffer(
    const std::filesystem::path& path)
{
    return CharBufferString<TChar, bufferSize, TCharTraits>(
        [&path](CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer)
        {
            AppendPathToCharacterBuffer<bufferSize, TChar, TCharTraits>(characterBuffer, path);
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
        static_cast<TChar>(std::filesystem::path::preferred_separator),
        static_cast<TChar>(CrossPlatformPathSeparator));
}

template <std::size_t bufferSize, class TChar, class TCharTraits>
MeddySDK::Meddyproject::CharBufferString<TChar, bufferSize, TCharTraits> MeddySDK::Meddyproject::FilesystemUtils::ConstructCharacterBufferFromPath(
    const std::filesystem::path& path)
{
    return CharBufferString<TChar, bufferSize, TCharTraits>(
        [&path](CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer)
        {
            AppendPathToCharacterBuffer<bufferSize, TChar, TCharTraits>(characterBuffer, path.native());
        }
        );
}

template <std::size_t bufferSize, class TChar, class TCharTraits>
void MeddySDK::Meddyproject::FilesystemUtils::AppendPathToCharacterBuffer(
    CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer,
    const std::filesystem::path& path)
{
    using TFromChar = std::filesystem::path::string_type::value_type;
    using TFromTraits = std::filesystem::path::string_type::traits_type;

    return StringUtils::AppendStringToCharacterBuffer<TChar, bufferSize, TFromChar, TFromTraits>(
        characterBuffer,
        path.native());
}

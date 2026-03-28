// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <boost/filesystem/path.hpp>
#include <string_view>
#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Core/Concepts.h>

/**
 * @brief String literal alternative to `MeddySDK::PrettyPathSeparator`.
 */
#define MEDDYSDK_PRETTY_PATH_SEPARATOR '/'

/**
 * @brief Filesystem utilities to extend the default std::filesystem tools.
 */
namespace MeddySDK
{
    /**
     * @brief A character that represents the most widely-accepted path separator across all platforms.
     */
    constexpr char PrettyPathSeparator = MEDDYSDK_PRETTY_PATH_SEPARATOR;

    /**
     * @brief An arbitrarily defined number that determines the maximum supported filesystem filename length.
     * @note This is not the pathname limit, but the leaf file/directory name at the very end of the path.
     */
    constexpr std::size_t MaxFilenameLength = 1024;

    template <std::size_t bufferSize, CppUtils::CharLike TChar = char, class TCharTraits = std::char_traits<TChar>>
    CppUtils::CharBufferString<TChar, bufferSize, TCharTraits> ConstructPrettyPathCharacterBuffer(const boost::filesystem::path& path);

    template <class TFwdIt, CppUtils::CharLike TChar = char>
    void ConvertPathStringToPrettyFormat(TFwdIt begin, TFwdIt end);

    template <CppUtils::CharLike TChar, class TCharTraits>
    bool IsPathEqualToString(const boost::filesystem::path& path, const std::basic_string_view<TChar, TCharTraits> string);
}

#include <MeddySDK/Meddyproject/FilesystemUtils.inl>

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK/Meddyproject/FilesystemUtils.h>

#include <CppUtils/Misc/CharBufferString.h>
#include <CppUtils/Misc/String.h>
#include <CppUtils/Core/String.h>
#include <CppUtils/Core/Concepts.h>
#include <CppUtils/Core/Filesystem.h>

template <std::size_t bufferSize, CppUtils::CharLike TChar, class TCharTraits>
CppUtils::CharBufferString<TChar, bufferSize, TCharTraits> MeddySDK::ConstructPrettyPathCharacterBuffer(
    const boost::filesystem::path& path)
{
    return CppUtils::CharBufferString<TChar, bufferSize, TCharTraits>(
        [&path](CppUtils::CharBufferString<TChar, bufferSize, TCharTraits>& characterBuffer)
        {
            CppUtils::AppendStringToCharacterBuffer(
                characterBuffer,
                CppUtils::MakeStringView(path.native()));

            CppUtils::ConvertPathStringToPrettyFormat(characterBuffer.begin(), characterBuffer.end());
        }
        );
}

template <CppUtils::CharLike TChar, class TCharTraits>
bool MeddySDK::IsPathEqualToString(const boost::filesystem::path& path, const std::basic_string_view<TChar, TCharTraits> string)
{
    const boost::filesystem::path::string_type& pathString = path.native();

    // The path's string could be storing different char types, so we need to copy and convert them to a new character
    // buffer here before comparing them. It looks complicated because we are copying the string onto the stack to
    // avoiding a free-store allocation.
    CppUtils::CharBufferString<TChar, MeddySDK::MaxFilenameLength> pathCharBuffer =
        CppUtils::ConstructCharacterBufferFromString<TChar, MeddySDK::MaxFilenameLength>(
            CppUtils::MakeStringView(pathString));

    return pathCharBuffer.ToStringView() == string;
}

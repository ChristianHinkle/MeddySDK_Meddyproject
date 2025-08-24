// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <string_view>
#include <span>
#include <MeddySDK_Meddyproject/CharBufferString.h>

/**
 * @brief String utilities to extend the default standard library tools.
 */
namespace MeddySDK::Meddyproject::StringUtils
{
    template <class TChar, class TTraits = std::char_traits<TChar>>
    std::basic_string_view<TChar, TTraits> GetStringViewFromCharacterBuffer(const std::span<TChar>& characterBuffer);

    template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
    CharBufferString<TToChar, bufferSize> ConstructCharacterBufferFromString(const std::basic_string_view<TFromChar, TFromTraits>& fromString);

    template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
    void AppendStringToCharacterBuffer(
        CharBufferString<TToChar, bufferSize>& characterBuffer,
        const std::basic_string_view<TFromChar, TFromTraits>& fromString);
}

template <class TChar, class TTraits>
std::basic_string_view<TChar, TTraits> MeddySDK::Meddyproject::StringUtils::GetStringViewFromCharacterBuffer(
    const std::span<TChar>& characterBuffer)
{
    return std::basic_string_view<TChar, TTraits>(characterBuffer.data(), characterBuffer.size());
}

template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
MeddySDK::Meddyproject::CharBufferString<TToChar, bufferSize> MeddySDK::Meddyproject::StringUtils::ConstructCharacterBufferFromString(
    const std::basic_string_view<TFromChar, TFromTraits>& fromString)
{
    return CharBufferString<TToChar, bufferSize>(
        [&fromString](CharBufferString<TToChar, bufferSize>& charBufferString)
        {
            AppendStringToCharacterBuffer(charBufferString, fromString);
        }
        );
}

template <class TToChar, std::size_t bufferSize, class TFromChar, class TFromTraits>
void MeddySDK::Meddyproject::StringUtils::AppendStringToCharacterBuffer(
    CharBufferString<TToChar, bufferSize>& characterBuffer,
    const std::basic_string_view<TFromChar, TFromTraits>& fromString)
{
    const std::size_t oldLength = characterBuffer.Length;

    // Add the length.
    characterBuffer.Length += fromString.length();

    // Copy each character to the buffer, converting them in the process.
    for (std::size_t i = 0; i < fromString.length(); ++i)
    {
        // Note: The first iteration will overwrite the null-terminating character, which is good.
        characterBuffer.CharBuffer[oldLength + i] = static_cast<TToChar>(fromString.at(i));
    }

    // Finally, terminate the string with a null character.
    characterBuffer.CharBuffer[oldLength + fromString.length()] = static_cast<TToChar>('\0');
}

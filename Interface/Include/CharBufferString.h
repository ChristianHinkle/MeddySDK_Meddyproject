// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include "MeddySDK_Meddyproject_Export.h"
#include <string_view>
#include <array>
#include <concepts>
#include <utility>

namespace MeddySDK::Meddyproject
{
    /**
     * @brief An in-place character buffer which can be manipulated and viewed as a string.
     */
    template <class TChar, std::size_t bufferSize, class TTraits = std::char_traits<TChar>>
    struct CharBufferString
    {
    public:

        CharBufferString() = default;

        /**
         * @brief An overload to support prvalue semantics, allowing callers to initialize this
         *        object manually before it's returned by the constructor.
         */
        template <class TFunctor>
        CharBufferString(TFunctor&& callbackFunctor)
            : CharBufferString()
        {
            callbackFunctor(*this);
        }

    public:

        std::basic_string_view<TChar, TTraits> GetStringView()
        {
            return std::basic_string_view<TChar, TTraits>(CharBuffer.data(), Length);
        }

    public:

        std::array<TChar, bufferSize>::iterator begin()
        {
            return CharBuffer.begin();
        }

        std::array<TChar, bufferSize>::const_iterator begin() const
        {
            return CharBuffer.begin();
        }

        std::array<TChar, bufferSize>::iterator end()
        {
            return CharBuffer.begin() + Length;
        }

        std::array<TChar, bufferSize>::const_iterator end() const
        {
            return CharBuffer.begin() + Length;
        }

    public:

        std::array<TChar, bufferSize> CharBuffer;

        std::size_t Length = 0u;
    };
}

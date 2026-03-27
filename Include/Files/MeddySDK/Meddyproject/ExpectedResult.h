// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <optional>

namespace MeddySDK
{
    /**
     * @brief A poor man's C++23 `std::expected` alternative.
     */
    template <class T, class TError>
    struct ExpectedResult
    {
    public:
        ExpectedResult(T&& value)
            : Value{value}
        {
        }
        ExpectedResult(const TError& error)
            : Error{error}
        {
        }

    public:
        bool IsError() const
        {
            return !Value.has_value();
        }

        const TError& GetError() const
        {
            assert(IsError());
            return Error;
        }

        const T& GetValue() const &
        {
            assert(!IsError());

            assert(Value.has_value());
            return *Value;
        }

        T&& GetValue() &&
        {
            T& value = const_cast<T&>(GetValue());
            return std::move(value);
        }

    private:
        std::optional<T> Value{std::nullopt};
        TError Error{};
    };
}

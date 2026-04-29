// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <rapidjson/prettywriter.h>

namespace MeddySDK
{
    constexpr rapidjson::PrettyFormatOptions PrettyJsonFormatOptions = rapidjson::kFormatDefault;

    constexpr char PrettyJsonIndentChar = ' ';
    constexpr std::size_t PrettyJsonIndentCount = 4u;

    template <class JsonPrettyWriterType>
    void ApplyPrettyJsonDefaults(JsonPrettyWriterType& jsonPrettyWriter)
    {
        jsonPrettyWriter.SetFormatOptions(MeddySDK::PrettyJsonFormatOptions);
        jsonPrettyWriter.SetIndent(MeddySDK::PrettyJsonIndentChar, MeddySDK::PrettyJsonIndentCount);
    }
}

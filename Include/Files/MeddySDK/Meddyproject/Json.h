// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK_Meddyproject_Export.h>
#include <rapidjson/prettywriter.h>
#include <CppUtils/Core/Filesystem.h>
#include <string_view>

namespace rapidjson
{
    template <typename Encoding, typename Allocator, typename StackAllocator>
    class GenericDocument;
}

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

    template <class Encoding, class Allocator, class StackAllocator>
    bool SaveJsonDocumentToFile(CppUtils::StdPathStringView pathnameString, const rapidjson::GenericDocument<Encoding, Allocator, StackAllocator>& jsonDocument);

    template <class Encoding, class Allocator, class StackAllocator>
    bool SaveJsonDocumentToFile(const char* pathnameString, const rapidjson::GenericDocument<Encoding, Allocator, StackAllocator>& jsonDocument);
}

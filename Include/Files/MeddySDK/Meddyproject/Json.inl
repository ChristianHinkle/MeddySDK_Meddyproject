// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#pragma once

#include <MeddySDK/Meddyproject/Json.h>
#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filewritestream.h>
#include <CppUtils/Misc/String.h>

template <class Encoding, class Allocator, class StackAllocator>
bool MeddySDK::SaveJsonDocumentToFile(CppUtils::StdPathStringView pathnameString, const rapidjson::GenericDocument<Encoding, Allocator, StackAllocator>& jsonDocument)
{
    CppUtils::CharBufferString pathnameStringConverted =
        CppUtils::ConstructCharacterBufferFromString<char, MeddySDK::MaxFilenameLength>(pathnameString);

    return SaveJsonDocumentToFile(pathnameStringConverted.ToStringView().data(), jsonDocument);
}

template <class Encoding, class Allocator, class StackAllocator>
bool MeddySDK::SaveJsonDocumentToFile(const char* pathnameString, const rapidjson::GenericDocument<Encoding, Allocator, StackAllocator>& jsonDocument)
{
    std::FILE* filePtr = std::fopen(pathnameString, "wb");
    if (!filePtr)
    {
        return false;
    }

    constexpr std::size_t jsonWriteBufferSize = 65536u;
    char jsonWriteBuffer[jsonWriteBufferSize];
    rapidjson::FileWriteStream jsonFileWriteStream{filePtr, jsonWriteBuffer, jsonWriteBufferSize};

    rapidjson::PrettyWriter<rapidjson::FileWriteStream> jsonWriter{jsonFileWriteStream};
    MeddySDK::ApplyPrettyJsonDefaults(jsonWriter);

    jsonDocument.Accept(jsonWriter);

    // Write a newline at end of file.
    jsonFileWriteStream.Put('\n');
    jsonFileWriteStream.Flush();

    std::fclose(filePtr);
    return true;
}

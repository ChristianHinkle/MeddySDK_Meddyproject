// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK/Meddyproject/Utils.h>
#include <iostream>
#include <string_view>
#include <charconv>
#include <CppUtils/StdReimpl/utility.h>

namespace ErrorCodes
{
    constexpr int IncorrectNumberOfArguments = 1;
    constexpr int ActualVsExpectedResultMismatch = 2;
    constexpr int ResultStringToIntegerConversionFail = 3;
}

// TODO: [todo][techdebt] Review this logic and make sure that if the test expects an error but the operation succeeded, then an error code `ActualVsExpectedResultMismatch` is returned.
int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Bad arguments given. Required command synopsis: `<command-name> <new-project-directory-path> <expected-result>`." << '\n';
        std::cout.flush();
        return ErrorCodes::IncorrectNumberOfArguments;
    }

    const std::string_view newProjectDirectoryPathString = argv[1];
    const std::string_view testResultExpectedString = argv[2];

    std::cout << "New project directory path string: \"" << newProjectDirectoryPathString << "\"." << '\n';
    std::cout << "Expected result string: \"" << testResultExpectedString << "\"." << '\n';

    std::cout << '\n';

    boost::filesystem::path newProjectDirectoryPath = boost::filesystem::path{newProjectDirectoryPathString}.lexically_normal();
    MeddySDK::UncertainProjectCreationResult testResultExpectedResult{};

    {
        unsigned char testResultExpectedInt{};
        const std::from_chars_result fromCharsResult =
            std::from_chars(testResultExpectedString.data(), testResultExpectedString.data() + testResultExpectedString.length(), testResultExpectedInt);

        const bool isFromCharsSuccess = fromCharsResult.ec == std::errc{};
        if (!isFromCharsSuccess)
        {
            std::cout << "Bad arguments given. <expected-result> must be an unsigned integer." << '\n';
            std::cout.flush();
            return ErrorCodes::ResultStringToIntegerConversionFail;
        }

        testResultExpectedResult = static_cast<MeddySDK::UncertainProjectCreationResult>(testResultExpectedInt);
    }

    std::cout << "New project directory path: " << newProjectDirectoryPath << "." << '\n';
    std::cout << "Expected result integer: " << StdReimpl::to_underlying(testResultExpectedResult) << "." << '\n';

    std::cout << '\n';

    MeddySDK::UncertainProjectCreationResult testResultActualResult =
        MeddySDK::TryCreateNewProject(boost::filesystem::path{newProjectDirectoryPath});

    std::cout << "Actual result integer: " << StdReimpl::to_underlying(testResultActualResult) << "." << '\n';

    const bool didTestPass = testResultActualResult == testResultExpectedResult;
    if (!didTestPass)
    {
        std::cout.flush();
        return ErrorCodes::ActualVsExpectedResultMismatch;
    }

    std::cout.flush();
    return 0;
}

// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK_Meddyproject/Meddyproject.h>
#include <iostream>
#include <string_view>
#include <charconv>

namespace ErrorCodes
{
    constexpr int IncorrectNumberOfArguments = 1;
    constexpr int ActualVsExpectedResultMismatch = 2;
    constexpr int ResultStringToIntegerConversionFail = 3;
}

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

    std::filesystem::path newProjectDirectoryPath = std::filesystem::weakly_canonical(newProjectDirectoryPathString);
    MeddySDK::Meddyproject::UncertainProjectCreationResult testResultExpectedResult = {};

    {
        unsigned char testResultExpectedInt = 0u;
        const std::from_chars_result fromCharsResult =
            std::from_chars(testResultExpectedString.data(), testResultExpectedString.data() + testResultExpectedString.length(), testResultExpectedInt);

        const bool isFromCharsSuccess = fromCharsResult.ec == std::errc();
        if (!isFromCharsSuccess)
        {
            std::cout << "Bad arguments given. <expected-result> must be an unsigned integer." << '\n';
            std::cout.flush();
            return ErrorCodes::ResultStringToIntegerConversionFail;
        }

        testResultExpectedResult = static_cast<MeddySDK::Meddyproject::UncertainProjectCreationResult>(testResultExpectedInt);
    }

    std::cout << "New project directory path: " << newProjectDirectoryPath << "." << '\n';
    std::cout << "Expected result integer: " << static_cast<unsigned int>(testResultExpectedResult) << "." << '\n';

    std::cout << '\n';

    MeddySDK::Meddyproject::UncertainProjectCreationResult testResultActualResult =
        MeddySDK::Meddyproject::TryCreateNewProject(std::filesystem::path(newProjectDirectoryPath));

    std::cout << "Actual result integer: " << static_cast<unsigned int>(testResultActualResult) << "." << '\n';

    const bool didTestPass = testResultActualResult == testResultExpectedResult;
    if (!didTestPass)
    {
        std::cout.flush();
        return ErrorCodes::ActualVsExpectedResultMismatch;
    }

    std::cout.flush();
    return 0;
}

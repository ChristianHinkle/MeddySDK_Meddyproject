// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#include <MeddySDK_Meddyproject/Meddyproject.h>
#include <iostream>
#include <string_view>

int main(int argc, char** argv)
{
    if (argc != 3)
    {
        std::cout << "Bad arguments given. Required command synopsis: `<command-name> <input-path> <expected-result>`." << '\n';
        std::cout.flush();
        return 1;
    }

    const std::string_view testInputString = argv[1];
    const std::string_view testResultExpectedString = argv[2];

    std::cout << "Input path string: \"" << testInputString << "\"." << '\n';
    std::cout << "Expected result path string: \"" << testResultExpectedString << "\"." << '\n';

    std::cout << '\n';

    //
    // @Christian: Note: [std][boost][cpp] Using `boost::filesystem` here is important because `std::filesystem` gives
    // some inconsistent results on MSVC. Here is a breakdown of that problem:
    // - `std::filesystem::weakly_canonical("/")` returns "C:\"
    // - `std::filesystem::weakly_canonical("/MyFile")` returns "\MyFile"
    //     - We need this one to return "C:\MyFile" instead.
    // - `std::filesystem::weakly_canonical(std::filesystem::weakly_canonical("/MyFile").parent_path())` returns "C:\"
    //
    // On Linux, though, `std::filesystem` gives nice, consistent behavior.
    //
    // We switched to `boost::filesystem` for more consistent behavior.
    //

    // Note: [boost] This `lexically_normal` function doesn't have an rvalue overload, unfortunately. So it is causing
    // a useless copy.
    boost::filesystem::path testInputPath = boost::filesystem::path(testInputString).lexically_normal();
    boost::filesystem::path testResultPathExpectedPath = boost::filesystem::path(testResultExpectedString).lexically_normal();

    std::cout << "Input path: " << testInputPath << "." << '\n';
    std::cout << "Expected result path: " << testResultPathExpectedPath << "." << '\n';

    std::cout << '\n';

    boost::filesystem::path testResultActualPath = boost::filesystem::path(
        MeddySDK::Meddyproject::ProjectRootToDotMeddyprojectPath(boost::filesystem::path(testInputPath))
        ).lexically_normal();

    std::cout << "Actual result path: " << testResultActualPath << "." << '\n';

    const bool didTestPass = testResultActualPath == testResultPathExpectedPath;
    if (!didTestPass)
    {
        std::cout.flush();
        return 2;
    }

    std::cout.flush();
    return 0;
}

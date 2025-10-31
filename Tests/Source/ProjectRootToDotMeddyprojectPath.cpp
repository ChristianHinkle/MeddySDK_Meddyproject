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
    // @Christian: TODO: [todo][filesystem][cpp] Results from MSVC aren't consistent enough for us here. Here is
    // a breakdown of the problem:
    // - `std::filesystem::weakly_canonical("/")` returns "C:\"
    // - `std::filesystem::weakly_canonical("/MyFile")` returns "\MyFile"
    //     - We need this one to return "C:\MyFile" instead.
    // - `std::filesystem::weakly_canonical(std::filesystem::weakly_canonical("/MyFile").parent_path())` returns "C:\"
    //
    // On Linux, we get nice, consistent behavior.
    //
    // Consider switching back to `boost::filesystem` for, hopefully, more consistet behavior.
    //

    std::filesystem::path testInputPath = std::filesystem::weakly_canonical(testInputString);
    std::filesystem::path testResultPathExpectedPath = std::filesystem::weakly_canonical(testResultExpectedString);

    std::cout << "Input path: " << testInputPath << "." << '\n';
    std::cout << "Expected result path: " << testResultPathExpectedPath << "." << '\n';

    std::cout << '\n';

    std::filesystem::path testResultActualPath = std::filesystem::weakly_canonical(
        MeddySDK::Meddyproject::ProjectRootToDotMeddyprojectPath(std::filesystem::path(testInputPath))
        );

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

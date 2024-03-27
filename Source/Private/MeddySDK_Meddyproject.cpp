// Copyright (c) 2023-2024 Christian Hinkle, Brian Hinkle.

#include "MeddySDK_Meddyproject.h"

#include <iostream>
#include <filesystem>

void MeddySDK::Meddyproject::MyBoostFilesystemExperiment()
{
    std::filesystem::path MyPath = std::filesystem::path("C:/Yoooo");

    std::cout << "Yo. Here is MyPath: " << MyPath << "." << std::endl;

    std::filesystem::path MyOtherPath = std::filesystem::path("C:/YooooImDifferent");

    std::cout << "Yo. Here is MyOtherPath: " << MyOtherPath << "." << std::endl;

    if (MyPath != MyOtherPath)
    {
        std::cout << "Yoo. Those are different." << std::endl;
    }
}

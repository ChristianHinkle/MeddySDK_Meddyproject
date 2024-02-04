
#include "MeddySDK_Meddyproject.h"

#include <iostream>
#include "boost/filesystem/path.hpp"

void MeddySDK::Meddyproject::MyBoostFilesystemExperiment()
{
    boost::filesystem::path MyPath = boost::filesystem::path("C:/Yoooo");

    std::cout << "Yo. Here is MyPath: " << MyPath << "." << std::endl;

    boost::filesystem::path MyOtherPath = boost::filesystem::path("C:/YooooImDifferent");

    std::cout << "Yo. Here is MyOtherPath: " << MyOtherPath << "." << std::endl;

    if (MyPath != MyOtherPath)
    {
        std::cout << "Yoo. Those are different." << std::endl;
    }
}

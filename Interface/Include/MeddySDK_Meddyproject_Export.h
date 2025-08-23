// Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

#ifndef MEDDYSDK_MEDDYPROJECT_EXPORT_H
#define MEDDYSDK_MEDDYPROJECT_EXPORT_H

// Include the appropriate "export header" file depending on which type of this library is being compiled.

#if __has_include(<meddysdk_meddyproject_shared_export.h>)
#include <meddysdk_meddyproject_shared_export.h>
#elif __has_include(<meddysdk_meddyproject_object_export.h>)
#include <meddysdk_meddyproject_object_export.h>
#elif __has_include(<meddysdk_meddyproject_static_export.h>)
#include <meddysdk_meddyproject_static_export.h>
#else
// Since no generated header files exist, it must mean we are the interface library (being used as a header-only library).
#include "meddysdk_meddyproject_interface_export.h"
#endif

#endif // #ifndef MEDDYSDK_MEDDYPROJECT_EXPORT_H

# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

# Set this variable for the person including us to use.
set(FILES_CMAKE_RESULT
  # List out all our include files.
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name}_Export.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name}_include_export.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name_path_format}/Utils.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name_path_format}/FilesystemUtils.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name_path_format}/FilesystemUtils.inl"
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name_path_format}/Meddyproject.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name_path_format}/Json.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/Files/${my_project_name_path_format}/Json.inl"
  )

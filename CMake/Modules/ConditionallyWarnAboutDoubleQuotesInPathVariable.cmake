# Copyright (c) 2023-2025 Christian Hinkle, Brian Hinkle.

cmake_minimum_required(VERSION 4.0)

function(conditionally_warn_about_double_quotes_in_path_variable variable_name)

  set(variable_value ${${variable_name}})

  string(FIND "${variable_value}" "\"" DoubleQuotePos)

  if (NOT ${DoubleQuotePos} STREQUAL "-1")
    message("Warning: The variable `${variable_name}` contains double quotes. This will cause `execute_process` to fail. When defining this variable from the command line, make sure you wrap the value in single quotes instead of double quotes. Single quotes avoid getting packed into the variable's value.")
  endif()

endfunction()

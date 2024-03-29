if(DEFINED COMPILER_CLANG_CMAKE_)
  return()
else()
  set(COMPILER_CLANG_CMAKE_ 1)
endif()

# Find clang for C compilation
find_program(CMAKE_C_COMPILER clang)
if(NOT CMAKE_C_COMPILER)
  message(FATAL_ERROR "Clang compiler 'clang' not found! Please ensure LLVM is installed and 'clang' is in your PATH.")
endif()

# Find clang++ for C++ compilation
find_program(CMAKE_CXX_COMPILER clang++)
if(NOT CMAKE_CXX_COMPILER)
  message(FATAL_ERROR "Clang++ compiler 'clang++' not found! Please ensure LLVM is installed and 'clang++' is in your PATH.")
endif()

# Force CMake to use clang and clang++ for C and C++ compilation
set(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE STRING "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE STRING "C++ compiler" FORCE)

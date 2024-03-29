if(DEFINED COMPILER_CLANG_CL_CMAKE_)
  return()
else()
  set(COMPILER_CLANG_CL_CMAKE_ 1)
endif()

# Specify the path to the preferred clang-cl explicitly
set(PREFERRED_CLANG_CL "${UL_LLVM_PATH}/bin/clang-cl.exe")
set(PREFERRED_LLD_LINK "${UL_LLVM_PATH}/bin/lld-link.exe")
set(PREFERRED_LLVM_LIB "${UL_LLVM_PATH}/bin/llvm-lib.exe")

# Check if the specified clang-cl exists
if(EXISTS "${PREFERRED_CLANG_CL}")
  set(CMAKE_C_COMPILER "${PREFERRED_CLANG_CL}")
  set(CMAKE_CXX_COMPILER "${PREFERRED_CLANG_CL}")
  set(CMAKE_LINKER "${PREFERRED_LLD_LINK}")
  set(CMAKE_AR "${PREFERRED_LLVM_LIB}")
else()
  # Search for clang-cl instead of cl
  find_program(CMAKE_C_COMPILER clang-cl)
  find_program(CMAKE_CXX_COMPILER clang-cl)
  find_program(CMAKE_LINKER lld-link)
  find_program(CMAKE_AR llvm-lib)
endif()

if(NOT CMAKE_C_COMPILER)
  message(FATAL_ERROR "Clang compiler 'clang-cl' not found! Please ensure LLVM is installed and 'clang-cl' is in your PATH.")
endif()

if(NOT CMAKE_CXX_COMPILER)
  message(FATAL_ERROR "Clang compiler 'clang-cl' not found! Please ensure LLVM is installed and 'clang-cl' is in your PATH.")
endif()

# Force CMake to use clang-cl for both C and C++ compilation
set(CMAKE_C_COMPILER "${CMAKE_C_COMPILER}" CACHE STRING "C compiler" FORCE)
set(CMAKE_CXX_COMPILER "${CMAKE_CXX_COMPILER}" CACHE STRING "C++ compiler" FORCE)
set(CMAKE_LINKER "${CMAKE_LINKER}" CACHE FILEPATH "Linker" FORCE)
set(CMAKE_AR "${CMAKE_AR}" CACHE FILEPATH "Static library archiver" FORCE)

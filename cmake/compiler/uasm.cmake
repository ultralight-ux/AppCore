if(DEFINED COMPILER_UASM_CMAKE_)
  return()
else()
  set(COMPILER_UASM_CMAKE_ 1)
endif()

find_program(CMAKE_ASM_MASM_COMPILER NAMES uasm64 uasm)

if(NOT CMAKE_ASM_MASM_COMPILER)
  message(FATAL_ERROR "MASM compiler 'uasm' not found! Please ensure uasm is installed and in your PATH.")
endif()

# Force CMake to use uasm for MASM compilation
set(CMAKE_ASM_MASM_COMPILER "${CMAKE_ASM_MASM_COMPILER}" CACHE STRING "MASM compiler" FORCE)

#!/bin/bash
cd ../bin
rm -f metal_shader.h
rm -f metal_shader
rm -f obj.air

# Compile metal shader to intermediate IR (air)
xcrun -sdk macosx metal -mmacosx-version-min=10.12 -std=osx-metal1.2 -c ../src/Shaders.metal -o obj.air

# Archive air file into metallib format
xcrun -sdk macosx metallib obj.air -o metal_shader

# Convert metallib binary to C header
xxd -i metal_shader > metal_shader.h

# Clean up
rm -f metal_shader
rm -f obj.air
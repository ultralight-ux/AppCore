# AppCore

AppCore is a cross-platform app runtime module for [Ultralight](http://github.com/ultralight-ux/Ultralight)
that provides platform-specific window creation, event handling, message loops, and GPU context creation / drawing.

You should use AppCore only if you intend to build a standalone HTML-based desktop application. For lower-level integration,
(eg, for rendering directly within an existing game engine), you should instead use the raw Ultralight API and provide your
own implementations for GPUDriver and FileSystem.

The following GPU technologies are used on each platform: 

| Platform | GPU Driver  | 
|----------|-------------|
| Windows  | Direct3D 11 |
| macOS    | Metal       |
| Linux    | OpenGL      | 

## Downloading pre-built binaries

Pre-built binares are uploaded to the following S3 buckets every commit:
 * Debug Bins: https://appcore-bin-dbg.sfo2.digitaloceanspaces.com/
 * Release Bins: https://appcore-bin.sfo2.digitaloceanspaces.com/

Click any of the bucket links to get an XML file listing. Download a package by appending the filename to the bucket URL.

## You'll need the following:

 * CMake 3.2+ (https://cmake.org/)
 * Ninja (https://ninja-build.org/)
 * Recent GCC/Clang on macOS/Linux
 * Visual Studio 2015 on Windows
 
### Setting up on Windows

1. Install Visual Studio 2015 (Free Community Edition works fine). Remember to select the C++ packages.
2. Install Chocolatey (https://chocolatey.org).
3. Run the following from command line: 

```
choco install cmake ninja
```

### Setting up on macOS

1. Install Xcode 9.4.1 (Older versions of Xcode can be found here: https://developer.apple.com/download/more/)
2. Install Homebrew (https://brew.sh/)
3. Run the following from command line:

```
brew install cmake ninja
```
 
### Setting up on Linux (Debian 9.5+)
 
To get everything on Linux, just type the following:
 
```
sudo apt install cmake ninja-build clang lld-4.0
```
    
## Building from source
 
### Building on Windows

To build for Windows / 64-bit:

```
make release x64
```

OR

To build for Windows / 32-bit:

```
make release x86
```

### Building on macOS and Linux

To build for macOS/Linux:

```
./make
```

## Build products

Build products will be in `<build_dir>/out`.

If your checkout is up-to-date, a 7zip release archive will also be generated in `<build_dir>`.


# AppCore

AppCore is a cross-platform app runtime module for [Ultralight](http://github.com/ultralight-ux/Ultralight)
that provides platform-specific window creation, events, message loops, and GPU context creation / drawing.

You should use AppCore only if you intend to build a standalone HTML-based desktop application (ala Electron).

For lower-level integration, (eg, for rendering within an existing game engine), you should instead use the
lower-level Ultralight API (eg, `Renderer::Create()`) which grants you the ability to render Views offscreen
to a pixel buffer using the CPU renderer.

For more information see our [Docs](https://docs.ultralig.ht).

The following GPU back-ends are used on each platform: 

| Platform | GPU Driver        | 
|----------|-------------------|
| Windows  | Direct3D 11       |
| macOS    | Metal             |
| Linux    | OpenGL 3.2 (GLFW) | 

## Downloading pre-built binaries

Pre-built binares are uploaded to the following S3 buckets every commit:
 * Release Bins: https://appcore-bin.sfo2.digitaloceanspaces.com/

Click any of the bucket links to get an XML file listing. Download a package by appending the filename to the bucket URL.

## You'll need the following:

 * CMake 3.2+ (https://cmake.org/)
 * Ninja (https://ninja-build.org/)
 * Recent GCC/Clang on macOS/Linux
 * Visual Studio 2019 on Windows
 
### Setting up on Windows

1. Install Visual Studio 2019 (Free Community Edition works fine). Remember to select the C++ packages.
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
sudo apt install cmake ninja-build clang lld-4.0 libx11-dev xorg-dev libglu1-mesa-dev
```
    
## Building from source
 
### Building on Windows

To build AppCore for Windows / 64-bit:

```
make release x64
```

#### Building Debug on Windows

To build AppCore with debug symbols:

```
make full_debug x64
```

#### Building on Windows with locally-built dependencies

By default, the build scripts in this repo fetch pre-built dependencies from our servers.

To build AppCore using locally-built dependencies in the `/deps` folder (really only useful
if you need to build against modified modules/deps):

```
make release x64 local
```

### Building on macOS and Linux

To build for macOS/Linux:

```
./make
```

## Build products

Build products will be in `<build_dir>/out`.

If your checkout is up-to-date, a 7zip release archive will also be generated in `<build_dir>`.

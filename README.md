# AppCore

AppCore is a cross-platform app runtime module for [Ultralight](http://github.com/ultralight-ux/Ultralight)
that provides platform-specific window creation, events, message loops, and GPU context creation / drawing.

You should use AppCore only if you intend to build a standalone HTML-based desktop application (ala Electron).

For lower-level integration, (eg, for rendering within an existing game engine), you should instead use the
lower-level Ultralight API (eg, `Renderer::Create()`) which grants you the ability to render Views offscreen
to a pixel buffer using the CPU renderer.

For more information see our [Docs](https://docs.ultralig.ht).

## Architecture Overview

AppCore follows a clean abstraction pattern with platform-specific implementations in separate directories:

### Core Components

| Header        | Description                                                 |
|---------------|-------------------------------------------------------------|
| `App.h`       | Central singleton managing renderer, windows, and main loop |
| `Window.h`    | Platform window creation and management                     |
| `Overlay.h`   | Web content rendering within windows                        |
| `Monitor.h`   | Display/monitor information and management                  |
| `Platform.h`  | Platform-specific services (clipboard, fonts, etc.)         |

### Platform Implementations

| Platform | Windowing API | GPU Driver      | Shader Format  |
|----------|---------------|-----------------|----------------|
| Windows  | Win32         | Direct3D 11/12  | HLSL           |
| macOS    | Cocoa         | Metal           | MSL            |
| Linux    | GLFW          | OpenGL 3.2      | GLSL           |

### Directory Structure

```
AppCore/
├── include/AppCore/     # Public API headers
│   ├── App.h
│   ├── Window.h
│   ├── Overlay.h
│   └── ...
├── src/
│   ├── common/          # Shared implementation code
│   ├── win/             # Windows-specific implementations
│   │   ├── d3d11/       # Direct3D 11 GPU driver
│   │   ├── d3d12/       # Direct3D 12 GPU driver
│   │   └── memory/      # Windows memory profiler
│   ├── mac/             # macOS-specific implementations
│   │   └── metal/       # Metal GPU driver
│   └── linux/           # Linux-specific implementations
│       └── gl/          # OpenGL GPU driver
└── shaders/
    ├── hlsl/            # Direct3D shaders
    ├── metal/           # Metal shaders
    └── glsl/            # OpenGL shaders
```

### Shader System

The shader system supports sophisticated rendering operations across all platforms:

- **Fill Types**: Solid colors, images, patterns, gradients, rounded rectangles, etc.
- **Blend Modes**: Full Porter-Duff compositing support (Over, Add, Multiply, Screen, etc.)
- **Filters**: Blur, grayscale, sepia, hue-rotate, contrast, brightness, opacity, etc.
- **Advanced Features**: Gamma correction, clipping masks, anti-aliasing, and more.

### Platform Services

| Service           | Windows                       | macOS                 | Linux               |
|-------------------|-------------------------------|-----------------------|---------------------|
| **Clipboard**     | Win32 Clipboard API           | NSPasteboard (AppKit) | GLFW (X11/Wayland)  |
| **File System**   | Win32 File API + Mem-Mapping  | POSIX + mmap          | C++ std::ifstream   |
| **Font Loading**  | DirectWrite                   | Core Text             | Fontconfig          |
| **GPU Context**   | D3D11/D3D12                   | MetalKit              | OpenGL/GLAD         |
| **Native Dialogs**| MessageBox API                | NSAlert (AppKit)      | GTK+                |

## Basic Usage

```cpp
#include <AppCore/AppCore.h>

using namespace ultralight;

int main() {
  // Create the App singleton
  auto app = App::Create();

  // Create a window
  auto window = Window::Create(app->main_monitor(), 900, 600, false,
    kWindowFlags_Titled | kWindowFlags_Resizable);

  // Create an overlay to render HTML content
  auto overlay = Overlay::Create(window, window->width(), window->height(), 0, 0);

  // Load a URL
  overlay->view()->LoadURL("https://google.com");

  // Run the app (blocks until all active windows are closed)
  app->Run();

  return 0;
}
```

## Compiling Shaders

AppCore includes pre-compiled shaders in memory by default, but you can compile them from source if needed:

### Windows (HLSL)
```bash
cd shaders/hlsl/util
compile.bat
```
This uses the DirectX `fxc` compiler to compile HLSL shaders to bytecode and converts them to C headers.

### macOS (Metal)
```bash
cd shaders/metal/util
./compile.sh
```
This uses the Metal compiler toolchain to compile `.metal` files to a metallib and converts it to a C header.

### Linux (GLSL)
GLSL shaders are compiled at runtime by the OpenGL driver. The source files in `shaders/glsl/` are converted to C headers for embedding. 

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

1. Install Xcode Command Line Tools (or the full Xcode package).
2. Download the macOS 10.14 Platform SDK from https://github.com/phracker/MacOSX-SDKs/releases and
   extract it to `/Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk`.
3. Install Homebrew (https://brew.sh/)
4. Run the following from the Terminal to setup the needed homebrew packages:

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
make debug x64
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

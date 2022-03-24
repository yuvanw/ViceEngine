# Vice Engine

<p align="center">
  <img src="https://user-images.githubusercontent.com/21228747/159940828-27c17603-662e-4f20-b7a1-ff0af8cbf0df.png" width="400">
</p>

## A 3D game engine mostly built from scratch in C++

![ViceEngineScreenshot](https://user-images.githubusercontent.com/21228747/159939544-b374ab80-a2ae-41ab-bc00-6bca7dfcfa3a.png)

Vice Engine is a 3D game engine designed and implemented with a from-scratch mentality and cross-platform support in mind.
While still deep in development, Vice Engine's most notable features include a data-driven 3D renderer, a flexible scene editor, and a comprehensive, unit-tested standard library.
See [Features](#features) for more details.

## Build

CMake 3.16 or greater is required.

### Windows

For Visual Studio 2022, use `GenerateProjectFiles.bat`

```
GenerateProjectFiles.bat
start Build\ViceEngine.sln
```

For older versions of Visual Studio, run the following commands, substituting your version of Visual Studio. For example, for Visual Studio 2019:

```
mkdir Build
cd Build
cmake -G "Visual Studio 16 2019" -A x64 ..\Source -DGRAPHICS_API:STRING=OpenGL -DUSE_SSE:BOOL=FALSE
start ViceEngine.sln
```

### macOS

macOS support coming soon!

## Usage

Build and run the `Engine` target in Visual Studio to launch the scene editor.

### Camera Controls
- Rotate: left mouse button
- Pan: middle or right mouse button
- Zoom: scroll wheel

## Features

The following features are supported per engine module:

### Core
- Containers library
- 3D math library
- Smart pointer library
- Strings
- Hashed string IDs
- Type traits
- Module system (in development)
- File system (in development)

### Renderer
- OpenGL 3.3 forward renderer
- Data-driven scene system
- Data-driven material system
- Render hardware interface (RHI) for graphics API-independent rendering code
- Blinn-Phong reflection model
- Toon shading
- Wavefront OBJ parser

### SceneEditor
- Mouse look camera controller
- Outliner
  - Load scenes
  - Add/remove models and lights from the scene
  - Select scene objects
  - Toggle visibility per model, light, and skybox
- Inspector
  - Camera
    - Modify projection type (i.e. perspective vs. orthographic)
    - Modify near plane distance
    - Modify far plane distance
  - Models
    - Modify transform (position, rotation, and scale)
    - Modify drawing mode (filled, wireframe, or points)
    - View meshes
    - Modify material per mesh
  - Directional Lights
    - Modify direction
    - Modify intensity
    - Modify color
  - Point Lights
    - Modify position
    - Modify intensity
    - Modify color

### ApplicationCore
- Platform-abstracted application/windowing system
- Platform-abstracted message pump

### EngineCore
- Engine initialization and shutdown
- Engine loop

### GameCore
- Game loop

### InputCore
- Platform-abstracted input system

## Dependencies

Vice Engine uses the following third-party libraries:
- Catch2: https://github.com/catchorg/Catch2
- Dear ImGui: https://github.com/ocornut/imgui
- stb_image: https://github.com/nothings/stb/blob/master/stb_image.h
- json: https://github.com/nlohmann/json

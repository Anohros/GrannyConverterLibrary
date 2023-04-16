# Converter library for granny converter
This library converts scenes of granny2 files from `*.gr2` to `*.fbx`.

# Development

## External requirements
- Windows - Unix based operation systems are not supported.
- DevIL 1.8.0 SDK - http://openil.sourceforge.net/
- FBX SDK 2020.3.1 - https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-3-1
- granny2

## Setup dependencies

### Setup DevIL SDK
Download DevIL 1.8.0 SDK from http://openil.sourceforge.net/download.php "DevIL 1.8.0 SDK for Windows" archive and extract its content to `external/devilsdk`.

### Setup FBX SDK
Download installer "FBX SDK 2020.3.1 VS2019" for FBX SDK 2020.3.1 from https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2020-3-1 and install it on your computer.

Edit `external/fbxsdk/fbxsdk-config.cmake` and edit the paths of those two lines below to point to the location of your FBX SDK installation:
```bash
set(FBXSDK_LIB "C:/Program\ Files/Autodesk/FBX/FBX\ SDK/2020.3.1/lib/vs2019/x86")
set(FBXSDK_INCLUDE "C:/Program\ Files/Autodesk/FBX/FBX\ SDK/2020.3.1/include")
```

### Setup granny2
Download an application with a licensed version of granny2 and copy its `granny2.dll` to `external/granny2/granny2.dll`. The version of the `granny2.dll` should be at least 2.9 up to 2.12 - tested version is 2.11 (recommended version).

## Setup IDE
Use Qt Creator (recommended for now) or the IDE of your choice for development, build or to adapt the library in your projects - see more below under section "How to use".

Open CMakeLists.txt as project.

### Visual Studio Code
Install Build Tools for Visual Studio 20xx from:

https://visualstudio.microsoft.com/downloads/

Install extensions in [Visual Studio Code](https://code.visualstudio.com/):
- [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
- [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

Open and trust the project folder and configure the project as asked by the CMake Tools. Choose "Visual Studio Community 20xx Release - **x86**" as active build kit. Only platform "x86" is supported as build target right now.

## How to use
Go to example "ConverterExample" in folder `examples/converter` to see how to use the library in an console application.

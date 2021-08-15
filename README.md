# Converter library for granny converter
This library converts scenes of granny2 files from `*.gr2` to `*.fbx`.

# Development

## External requirements
- Windows - Unix based operation systems are not supported.
- Qt Framework 5.15.2 (msvc2019 x86) - https://www.qt.io/download-qt-installer (Alternative: https://github.com/miurahr/aqtinstall/)
- DevIL 1.8.0 SDK - http://openil.sourceforge.net/
- FBX SDK 2019.5 - https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-5
- granny2

## Setup dependencies

### Setup Qt Framework 
Install the framework in version 5.15.2 (msvc2019 x86).

Use the online installer from https://www.qt.io/download-qt-installer or as an alternative you can use the tool `aqtinstall` "Another (unofficial) Qt CLI Installer" from https://github.com/miurahr/aqtinstall/.

### Setup DevIL SDK
Download DevIL 1.8.0 SDK from http://openil.sourceforge.net/download.php "DevIL 1.8.0 SDK for Windows" archive and extract its content to `external/devilsdk`.

### Setup FBX SDK
Download installer "FBX SDK 2019.5 VS2017" for FBX SDK 2019.5 from https://www.autodesk.com/developer-network/platform-technologies/fbx-sdk-2019-5 and install it on your computer.

Edit `external/fbxsdk/fbxsdk-config.cmake` and edit the paths of those two lines below to point to the location of your FBX SDK installation:
```bash
set(FBXSDK_LIB "C:/Program\ Files/Autodesk/FBX/FBX\ SDK/2019.5/lib/vs2015/x86")
set(FBXSDK_INCLUDE "C:/Program\ Files/Autodesk/FBX/FBX\ SDK/2019.5/include")
```

### Setup granny2
Download an application with a licensed version of granny2 and copy its `granny2.dll` to `external/granny2/granny2.dll`. The version of the `granny2.dll` should be at least 2.9 up to 2.12 - tested version is 2.11 (recommended version).

## Setup IDE
Use Qt Creator for development and build.

Setup Qt Framework 5.15.2 (msvc2019 x86).

Open CMakeLists.txt as project.

# Converter library for granny converter
This library converts 3D scenes from Granny 3D (\*.gr2) files into Autodesk FBX (\*.fbx) format.

# Development

## External requirements
- **Windows**: Unix based operation systems are not supported.
- **DevIL 1.8.0 SDK for Windows**:

    Available at http://openil.sourceforge.net/

- **FBX SDK 2020.3.9 VS2022**:

    Available at https://aps.autodesk.com/developer/overview/fbx-sdk

- **Granny 3D**:

    Required dependency (see setup below).

## Setup dependencies

### Setup DevIL SDK
1. Download the "**DevIL 1.8.0 SDK for Windows**" archive from http://openil.sourceforge.net/download.php.
2. Extract the contents into the following directory: `external/devilsdk`.

### Setup FBX SDK
1. **Download and install:**

    Download the **FBX SDK 2020.3.9 VS2022** installer from https://aps.autodesk.com/developer/overview/fbx-sdk and install it on your system.

2. **Configure paths:**

    Open `external/fbxsdk/fbxsdk-config.cmake` and update the paths in the following two lines to point to the location of your local FBX SDK installation directory:

    ```bash
    set(FBXSDK_LIB "C:/Program\ Files/Autodesk/FBX/FBX\ SDK/2020.3.9/lib/x64")
    set(FBXSDK_INCLUDE "C:/Program\ Files/Autodesk/FBX/FBX\ SDK/2020.3.9/include")
    ```

### Setup Granny 3D
1. **Acquire the DLL:**

    Obtain a licensed version of `granny2_x64.dll`.

2. **Deploy:**

    Copy the file directly into the following directory: `external/granny2/granny2_x64.dll`

_(Note: The `granny2_x64.dll` must be between **2.9 and 2.12.** Version **2.11** is the recommended and tested version, which can be found online.)_

## Setup IDE
While you can use any IDE of your choice to develop, build, or integrate this library, Visual Studio Code is highly recommended for its ease of use. Alternatively, Qt Creator also works well. For more details on integration, see the "How to use" section below.

To begin, open the CMakeLists.txt file as a project in your chosen IDE.

### Visual Studio Code
1. **Install Build Tools:**

    Download and install the Build Tools for Visual Studio from: https://visualstudio.microsoft.com/downloads/

2. **Install extensions:**

    Add the following extension to [Visual Studio Code](https://code.visualstudio.com/):

    1. [C/C++ Extension Pack](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools-extension-pack)
    2. [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)

3. **Configure project:**
    1. Open and trust the project folder.
    2. Configure the project as prompted by **CMake Tools**.
    3. Select "Visual Studio Community 2022 Release - **amd64**" as your active build kit.
    
_(Note: The current version of the library exclusively supports the **amd64** platform as a build target.)_

## How to use
To see a practical implementation of the library in a console application, navigate to the ConverterExample project located in the following directory:

`examples/converter`

This example demonstrates how to properly initialize the library, load assets, and handle the conversion process.

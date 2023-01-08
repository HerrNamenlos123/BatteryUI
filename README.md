# BatteryUI
A C++17 header-only library to make your ImGui interface look good 

## Requirements:

 - Working C++ compiler and IDE with support for at least C++17
 - git and CMake installed

## Build example

### Building directly in terminal (any IDE/compiler)
```bash
cd example
mkdir build
cd build

cmake ..               # Configure the build system
cmake --build .        # Compile the executable
../bin/debug/example   # Run the resulting executable (or 'release')
```

### Building using Visual Studio (Windows)
```bash
cd example
mkdir build
cd build

cmake ..               # Configure the build system
start example.sln      # Open the solution in Visual Studio
```

Alternatively open `build/example.sln` manually in Visual Studio. 

In Visual Studio, right-click on the target `example` and click `Set as Startup Project` (German `Als Startprojekt festlegen`).
Now you can simply build and run the executable in Visual Studio by pressing `F5`.

## Troubleshooting

When you get an error similar to 
```
BatteryUI::Exception was thrown: [BatteryUI::Font::loadFont]: Failed to load font from '../resources/roboto.medium.ttf': No such file or directory
```
then you are running the executable from the wrong working directory.

Due to the way Visual Studio and other IDEs work, your working directory is meant to be the `example/build/` directory you created when building. You should run the executable from this directory with the command `../bin/debug/example` (or 'release' instead of `debug`). Visual Studio does this under the hood, its working directory is the directory in which the `example.sln` file is placed.

# TODO

>Malte Skarupke
>https://github.com/skarupke/std_function  std_function.h

>https://github.com/nlohmann/json json.hpp

>https://github.com/nlohmann/json magic_enum.h

>Coming:  
>https://github.com/JlnWntr/Lua-Adapter

>```
>sudo apt install libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev
>sudo apt install libgl1-mesa-dev
>```

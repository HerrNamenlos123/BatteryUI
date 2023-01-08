# BatteryUI
A C++17 header-only library to make your ImGui interface look good 

```
sudo apt install libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev
sudo apt install libgl1-mesa-dev
```

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

Malte Skarupke
https://github.com/skarupke/std_function  std_function.h

https://github.com/nlohmann/json json.hpp

https://github.com/nlohmann/json magic_enum.h

Coming:  
https://github.com/JlnWntr/Lua-Adapter

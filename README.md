# Elemental Draw

![C++ Builder](https://github.com/Haeri/ElementalDraw/workflows/C++%20Builder/badge.svg)
![Version](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/Haeri/ElementalDraw/master/vcpkg.json&label=version&query=$['version-string']&color=blue)
[![License](https://img.shields.io/github/license/Haeri/ElementalDraw.svg)](https://github.com/Haeri/ElementalDraw/blob/master/LICENSE)
![Size](https://img.shields.io/github/languages/code-size/haeri/elementalDraw)

Portable C++ canvas library with a Vulkan backend.


## Requirements
- [CMake 3.3^](https://cmake.org/download/) 
- [Python 3^](https://www.python.org/downloads/)

## Setup

### <img height="14" src="https://image.flaticon.com/icons/svg/888/888882.svg"> Windows
```cmd
git clone https://github.com/Haeri/ElementalDraw.git --recursive
cd ElementalDraw
./tools/install_win.bat
```
<sub>Make sure to open the Visual Studio project as administrator, as it needs to perform copy operatiosn to make resources files available to the binaries.</sub>

### <img height="16" src="https://image.flaticon.com/icons/svg/226/226772.svg"> Linux
```bash
git clone https://github.com/Haeri/ElementalDraw.git --recursive
cd ElementalDraw
sudo chmod +x ./tools/*.sh
sudo apt update
sudo apt install -y libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev cmake curl unzip tar
sudo ./tools/install_nix.sh
```

### <img height="16" src="https://image.flaticon.com/icons/svg/2/2235.svg"> Mac
```bash
git clone https://github.com/Haeri/ElementalDraw.git --recursive
cd ElementalDraw
./tools/install_nix.sh
```

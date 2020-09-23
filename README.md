# Elemental Draw

![C++ Builder](https://github.com/Haeri/ElementalDraw/workflows/C++%20Builder/badge.svg)
![Version](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/Haeri/ElementalDraw/master/vcpkg.json&label=version&query=$['version-string']&color=blue)
[![License](https://img.shields.io/github/license/Haeri/ElementalDraw.svg)](https://github.com/Haeri/ElementalDraw/blob/master/LICENSE)
![Size](https://img.shields.io/github/languages/code-size/haeri/elementalDraw)

Portable C++ canvas library with a Vulkan backend.


## Build Requirements
- [CMake 3.3^](https://cmake.org/download/) 
- [Python 3^](https://www.python.org/downloads/)
- [Vcpkg](https://github.com/microsoft/vcpkg) (Not required. Will be downloaded if `VCPKG_ROOT` is not defined)

## Setup

### <img height="14" src="https://image.flaticon.com/icons/svg/888/888882.svg"> Windows
```cmd
git clone https://github.com/Haeri/ElementalDraw.git
cd ElementalDraw
./tools/setup_win.bat
```
### <img height="16" src="https://image.flaticon.com/icons/svg/226/226772.svg"> Linux
```bash
git clone https://github.com/Haeri/ElementalDraw.git
cd ElementalDraw
sudo apt update
sudo apt install -y libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev cmake curl unzip tar
sudo ./tools/setup_nix.sh
```

### <img height="16" src="https://image.flaticon.com/icons/svg/2/2235.svg"> Mac
```bash
git clone https://github.com/Haeri/ElementalDraw.git
cd ElementalDraw
./tools/setup_nix.sh
```

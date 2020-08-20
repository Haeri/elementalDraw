# Elemental Draw

![C++ Builder](https://github.com/Haeri/ElementalDraw/workflows/C++%20Builder/badge.svg)
[![License](https://img.shields.io/github/license/Haeri/ElementalDraw.svg)](https://github.com/Haeri/ElementalDraw/blob/master/LICENSE)

Portable c++ canvas library with a Vulkan backend.


## Project Setup

### Windows
```cmd
git clone https://github.com/Haeri/ElementalDraw.git --recursive
cd ElementalDraw
./tools/install_win.bat
```

### Linux
```bash
git clone https://github.com/Haeri/ElementalDraw.git --recursive
cd ElementalDraw
sudo chmod +x ./tools/*.sh
sudo apt update
sudo apt install libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev
sudo ./tools/install_nix.sh
```

### MacOS
```bash
git clone https://github.com/Haeri/ElementalDraw.git --recursive
cd ElementalDraw
./tools/install_nix.sh
```
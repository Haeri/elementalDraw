# Elemental Draw

![C++ Builder](https://github.com/Haeri/ElementalDraw/workflows/C++%20Builder/badge.svg)
![Version](https://img.shields.io/badge/dynamic/json?url=https://raw.githubusercontent.com/Haeri/ElementalDraw/master/vcpkg.json&label=version&query=$['version-string']&color=blue)
[![License](https://img.shields.io/github/license/Haeri/ElementalDraw.svg)](https://github.com/Haeri/ElementalDraw/blob/master/LICENSE)
![Size](https://img.shields.io/github/languages/code-size/haeri/elementalDraw)

Portable C++ canvas library with a Vulkan (Windows, Linux) and OpenGL (MacOS) backend.


## Build Requirements
- [CMake 3.16^](https://cmake.org/download/) 
- [Python 3^](https://www.python.org/downloads/)

## Setup

### <img height="14" src="https://image.flaticon.com/icons/svg/888/888882.svg"> Windows
```cmd
git clone https://github.com/Haeri/ElementalDraw.git  --recursive
cd ElementalDraw
./scripts/generate_project_win.bat
```
### <img height="16" src="https://image.flaticon.com/icons/svg/226/226772.svg"> Linux
```bash
git clone https://github.com/Haeri/ElementalDraw.git  --recursive
cd ElementalDraw
sudo apt update
sudo apt install -y libxinerama-dev libxcursor-dev xorg-dev libglu1-mesa-dev cmake curl unzip tar
./scripts/generate_project_linux.sh
```

### <img height="16" src="https://image.flaticon.com/icons/svg/2/2235.svg"> Mac
```bash
git clone https://github.com/Haeri/ElementalDraw.git  --recursive
cd ElementalDraw
./scripts/generate_project_mac.sh
```

## Dependencies
Dependencies will be automatically managed through vcpkg if this repo is cloned with the `--recursive` flag.
- [glfw3](https://www.glfw.org/)
- [glslang](https://github.com/KhronosGroup/glslang)
- [stb](https://github.com/nothings/stb)
- [freetype](https://www.freetype.org/)


## Simple API
Simple API across all platforms.

![preview](./docs/preview.png)

```cpp
int main(void)
{  
    // create window and context
    elemd::Window* win = elemd::Window::create({"Hello World", 310, 240});
    elemd::Context* ctx = win->create_context();

    // load image
    elemd::image* logo = elemd::image::create("./res/logo.png");

    ctx->set_clear_color(elemd::color("#e5e9f0"));

    // main loop
    while (win->is_running())
    {
        win->poll_events();

        // draw text
        ctx->set_fill_color(elemd::color("#2e3441"));
        ctx->set_font_size(30);
        ctx->draw_text(55, 60, "elemental UI");        
        
        // draw circle
        ctx->set_fill_color(elemd::color("#c16069"));
        ctx->fill_circle(80, 135, 15);

        // draw rounded rectangle outline
        ctx->set_line_width(2);
        ctx->set_stroke_color(elemd::color("#80a0c2"));
        ctx->stroke_rounded_rect(130, 120, 30, 30, 5);

        // draw rounded image
        ctx->draw_rounded_image(200, 120, 30, 30, logo, 5);

        ctx->draw_frame();
    }
    
    return 0;
}
```
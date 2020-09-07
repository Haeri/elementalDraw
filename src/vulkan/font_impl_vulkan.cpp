#include "font_impl_vulkan.hpp"

#include <iostream>

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline fontImplVulkan* getImpl(font* ptr)
    {
        return (fontImplVulkan*)ptr;
    }
    inline const fontImplVulkan* getImpl(const font* ptr)
    {
        return (const fontImplVulkan*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    font* font::create()
    {
        return new fontImplVulkan();
    }

    fontImplVulkan::fontImplVulkan()
    {

        FT_Error error = FT_Init_FreeType(&library);
        if (error)
        {
            std::cerr << "Error: during FreeType initialization" << std::endl;
            exit(1);
        }

        error = FT_New_Face(library, "./elemd_res/font/Urbanist-Regular.ttf", 0, &face);
        if (error == FT_Err_Unknown_File_Format)
        {
            std::cerr << "Error: font not supported!" << std::endl;
            exit(1);
        }
        else if (error)
        {
            std::cerr << "Error: font not foundè" << std::endl;
            exit(1);
        }

        error = FT_Set_Char_Size(face,    /* handle to face object           */
                                 0,       /* char_width in 1/64th of points  */
                                 16 * 64, /* char_height in 1/64th of points */
                                 300,     /* horizontal device resolution    */
                                 300);
    }

    elemd::fontImplVulkan::~fontImplVulkan()
    {
    }

} // namespace elemd
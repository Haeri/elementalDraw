#ifndef ELEMD_IMAGE_HPP
#define ELEMD_IMAGE_HPP

#include <elemd/elemental_draw.hpp>

#include <map>
#include <string>

namespace elemd
{
    enum ImageFiltering
    {
        NEAREST, LINEAR
    };
    struct ImageConfig
    {
        bool mipmaps = false;
        ImageFiltering imagefiltering = LINEAR;
    };

    class ELEMD_API Image
    {
    public:
        static Image* create(std::string file_path, ImageConfig imageConfig = {});
        static Image* create(int width, int height, int components, unsigned char* data,
                             ImageConfig imageConfig = {});

        
        int get_width();
        int get_height();
        int get_channels();
        void set_name(std::string name);

        void update_data(unsigned char* data);
        void write_to_file(std::string file_path);

        void destroy();

    protected:
        static std::map<std::string, Image*> _image_index;

        int _width = -1;
        int _height = -1;
        int _components = -1;
        bool _loaded = false;
        ImageConfig _imageConfig = {};

        std::string _name;

        unsigned char* _data = nullptr;

        Image() = default;
        virtual ~Image() = default;
    };

} // namespace elemd

#endif // ELEMD_IMAGE_HPP
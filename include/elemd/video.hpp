#ifndef ELEMD_VIDEO_HPP
#define ELEMD_VIDEO_HPP

#include <elemd/elemental_draw.hpp>
#include <elemd/image.hpp>

#include <string>


namespace elemd
{

    class ELEMD_API Video
    {
    public:
        static Video* create(std::string file_path);
        
        int get_width();
        int get_height();        
        void set_name(std::string name);
        
        Image* get_frame(int index);
        void read_next();

        void destroy();

    protected:
        std::string _name;
        Image* _image;

        Video() = default;
        virtual ~Video() = default;
    };

} // namespace elemd

#endif // ELEMD_VIDEO_HPP
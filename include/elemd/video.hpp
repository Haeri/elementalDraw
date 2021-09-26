#ifndef ELEMD_VIDEO_HPP
#define ELEMD_VIDEO_HPP

#include <elemd/elemental_draw.hpp>
#include <elemd/image.hpp>

#include <string>

struct AVRational;
struct AVFormatContext;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;
struct SwsContext;

namespace elemd
{

    class ELEMD_API video
    {
    public:
        static video* create(std::string file_path);
        
        unsigned char* get_data();
        int get_width();
        int get_height();        
        void set_name(std::string name);
        
        image* get_frame(int index);

        void destroy();

    protected:
        //static std::map<std::string, video*> _video_index;

        int _width = -1;
        int _height = -1;
        std::string _name;

        unsigned char* _data = nullptr;
        image* _image;

        video() = default;
        virtual ~video() = default;

        struct VideoReaderState
        {
            // Public things for other parts of the program to read from
            int width, height;
            AVRational* time_base;

            // Private internal state
            AVFormatContext* av_format_ctx;
            AVCodecContext* av_codec_ctx;
            int video_stream_index;
            AVFrame* av_frame;
            AVPacket* av_packet;
            SwsContext* sws_scaler_ctx;
        } _vr_state;

        bool video_reader_open(video::VideoReaderState* state, const char* filename);
        bool video_reader_read_frame(video::VideoReaderState* state, uint8_t* frame_buffer,
                                     int64_t* pts);
        bool video_reader_seek_frame(video::VideoReaderState* state, int64_t ts);
        void video_reader_close(video::VideoReaderState* state);

    };

} // namespace elemd

#endif // ELEMD_VIDEO_HPP
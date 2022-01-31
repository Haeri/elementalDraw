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

    class ELEMD_API Video
    {
    public:
        static Video* create(std::string file_path);
        
        unsigned char* get_data();
        int get_width();
        int get_height();        
        void set_name(std::string name);
        
        Image* get_frame(int index);

        void destroy();

    protected:
        //static std::map<std::string, video*> _video_index;

        int _width = -1;
        int _height = -1;
        std::string _name;

        AVFrame* _gl_frame;
        //unsigned char* _data = nullptr;
        Image* _image;

        Video() = default;
        virtual ~Video() = default;

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

        bool video_reader_open(Video::VideoReaderState* state, const char* filename);
        bool video_reader_read_frame(Video::VideoReaderState* state, uint8_t* frame_buffer,
                                     int64_t* pts);
        bool video_reader_seek_frame(Video::VideoReaderState* state, int64_t ts);
        void video_reader_close(Video::VideoReaderState* state);

    };

} // namespace elemd

#endif // ELEMD_VIDEO_HPP
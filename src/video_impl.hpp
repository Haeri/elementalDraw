#ifndef ELEMD_VIDEO_IMPL_HPP
#define ELEMD_VIDEO_IMPL_HPP

#include "elemd/video.hpp"

#include <map>
#include <string>

extern "C"
{
#include <inttypes.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
}

namespace elemd
{
    class VideoImpl : public Video
    {
    public:
        VideoImpl(std::string file_path);
        ~VideoImpl();

        bool _loaded = false;

        AVFormatContext* format_context = nullptr;
        int video_stream_index = -1;
        AVCodecContext* codec_context = nullptr;
        AVCodec* codec = nullptr;
        AVCodecParameters* codec_parameters = nullptr;
        AVPacket* packet = nullptr;
        AVFrame* av_frame = nullptr;
        AVFrame* gl_frame = nullptr;
        SwsContext* conv_ctx = nullptr;
    };

} // namespace elemd

#endif // ELEMD_VIDEO_IMPL_HPP
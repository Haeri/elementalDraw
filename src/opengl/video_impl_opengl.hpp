#ifndef ELEMD_VIDEO_IMPL_OPENGL_HPP
#define ELEMD_VIDEO_IMPL_OPENGL_HPP

#include "elemd/video.hpp"

#include "opengl_shared_info.hpp"
#include <map>
#include <string>

extern "C"
{
#include <inttypes.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace elemd
{
    class videoImplOpengl : public Video
    {
    public:
        videoImplOpengl(std::string file_path);
        ~videoImplOpengl();

        bool _loaded = false;

        AVFormatContext* fmt_ctx = nullptr;
        int stream_idx = -1;
        AVStream* video_stream = nullptr;
        AVCodecContext* codec_ctx = nullptr;
        AVCodec* decoder = nullptr;
        AVPacket* packet = nullptr;
        AVFrame* av_frame = nullptr;
        AVFrame* gl_frame = nullptr;
        SwsContext* conv_ctx = nullptr;
    };

} // namespace elemd

#endif // ELEMD_VIDEO_IMPL_OPENGL_HPP
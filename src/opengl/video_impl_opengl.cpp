#include "video_impl_opengl.hpp"

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

extern "C"
{
#include <inttypes.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline videoImplOpengl* getImpl(Video* ptr)
    {
        return (videoImplOpengl*)ptr;
    }
    inline const videoImplOpengl* getImpl(const Video* ptr)
    {
        return (const videoImplOpengl*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Video* Video::create(std::string file_path)
    {
        return new videoImplOpengl(file_path);
    }

    videoImplOpengl::videoImplOpengl(std::string file_path)
    {
        
        if (!video_reader_open(&_vr_state, file_path.c_str()))
        {
            printf("Couldn't open video file (make sure you set a video file that exists)\n");
            return;
        }

        _loaded = true;

        /*
        if (memalign((void**)&frame_data, ALIGNMENT, frame_width * frame_height * 4) != 0)
        {
            printf("Couldn't allocate frame buffer\n");
            return 1;
        }
        */

        int size = _vr_state.width * _vr_state.height * 4;
        uint8_t* internal_buffer = (uint8_t*)malloc(size * sizeof(uint8_t));

        avpicture_fill((AVPicture*)data.gl_frame, internal_buffer, AV_PIX_FMT_RGB24,
                       data.codec_ctx->width, data.codec_ctx->height);
        data.packet = (AVPacket*)av_malloc(sizeof(AVPacket));

        
        //_data =
//            (unsigned char*)malloc(_vr_state.width * _vr_state.height * 4 * sizeof(unsigned char));
            
        int64_t pts;
        if (!video_reader_read_frame(&_vr_state, _data, &pts))
        {
            printf("Couldn't load video frame\n");
        }

        _image = Image::create(_vr_state.width, _vr_state.height, 4, _data);
    }

    videoImplOpengl::~videoImplOpengl()
    {
        if (_loaded)
        {
            video_reader_close(&_vr_state);
            _loaded = false;
        }

        if (_uploaded)
        {
            //glDeleteTextures(1, &_image);
            _uploaded = false;
        }
    }

    void videoImplOpengl::upload()
    { /*
        if (!_loaded)
            return;

        glGenTextures(1, &_image);
        glBindTexture(GL_TEXTURE_2D, _image);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     _data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        _uploaded = true;*/
    }

    void videoImplOpengl::bind(GLuint texture_unit)
    {
        /*
        glActiveTexture(GL_TEXTURE0 + texture_unit);
        glBindTexture(GL_TEXTURE_2D, _image);*/
    }
} // namespace elemd
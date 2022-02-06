#include "video_impl.hpp"

#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <iostream>



namespace elemd
{
    /* ------------------------ DOWNCAST ------------------------ */

    inline VideoImpl* getImpl(Video* ptr)
    {
        return (VideoImpl*)ptr;
    }
    inline const VideoImpl* getImpl(const Video* ptr)
    {
        return (const VideoImpl*)ptr;
    }

    /* ------------------------ PUBLIC IMPLEMENTATION ------------------------ */

    Video* Video::create(std::string file_path)
    {
        return new VideoImpl(file_path);
    }

    VideoImpl::VideoImpl(std::string file_path)
    {

        // initialize libav
        av_register_all();
        avformat_network_init();

        // open video
        if (avformat_open_input(&fmt_ctx, file_path.c_str(), NULL, NULL) < 0)
        {
            std::cout << "failed to open input" << std::endl;
            // clearAppData(&data);
            return;
        }

        // find stream info
        if (avformat_find_stream_info(fmt_ctx, NULL) < 0)
        {
            std::cout << "failed to get stream info" << std::endl;
            // clearAppData(&data);
            return;
        }

        // find the video stream
        for (unsigned int i = 0; i < fmt_ctx->nb_streams; ++i)
        {
            if (fmt_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                stream_idx = i;
                break;
            }
        }

        if (stream_idx == -1)
        {
            std::cout << "failed to find video stream" << std::endl;
            //clearAppData(&data);
            return;
        }

        video_stream = fmt_ctx->streams[stream_idx];
        codec_ctx = video_stream->codec;

        // find the decoder
        decoder = avcodec_find_decoder(codec_ctx->codec_id);
        if (decoder == NULL)
        {
            std::cout << "failed to find decoder" << std::endl;
            //clearAppData(&data);
            return;
        }

        // open the decoder
        if (avcodec_open2(codec_ctx, decoder, NULL) < 0)
        {
            std::cout << "failed to open codec" << std::endl;
            //clearAppData(&data);
            return;
        }

        // allocate the video frames
        av_frame = av_frame_alloc();
        gl_frame = av_frame_alloc();
        int size =
            avpicture_get_size(AV_PIX_FMT_RGB24, codec_ctx->width, codec_ctx->height);
        uint8_t* internal_buffer = (uint8_t*)av_malloc(size * sizeof(uint8_t));
        avpicture_fill((AVPicture*)gl_frame, internal_buffer, AV_PIX_FMT_RGB24,
                       codec_ctx->width, codec_ctx->height);
        packet = (AVPacket*)av_malloc(sizeof(AVPacket));


        _image = elemd::Image::create(codec_ctx->width, codec_ctx->height, 3, internal_buffer);

        _loaded = true;
    }

    VideoImpl::~VideoImpl()
    {
        if (_loaded)
        {
            avformat_close_input(&fmt_ctx);

            if (av_frame)
                av_free(av_frame);
            if (gl_frame)
                av_free(gl_frame);
            if (packet)
                av_free(packet);
            if (codec_ctx)
                avcodec_close(codec_ctx);
            if (fmt_ctx)
                avformat_free_context(fmt_ctx);

            _image->destroy();

            _loaded = false;
        }
    }

    void Video::read_next()
    {
        VideoImpl* impl = getImpl(this);
        do
        {
            if (av_read_frame(impl->fmt_ctx, impl->packet) < 0)
            {
                av_free_packet(impl->packet);
                return;
            }

            if (impl->packet->stream_index == impl->stream_idx)
            {
                int frame_finished = 0;

                if (avcodec_decode_video2(impl->codec_ctx, impl->av_frame, &frame_finished,
                                          impl->packet) < 0)
                {
                    av_free_packet(impl->packet);
                    return;
                }

                if (frame_finished)
                {
                    if (!impl->conv_ctx)
                    {
                        impl->conv_ctx =
                            sws_getContext(impl->codec_ctx->width, impl->codec_ctx->height,
                                           impl->codec_ctx->pix_fmt, impl->codec_ctx->width,
                                           impl->codec_ctx->height,
                            AV_PIX_FMT_RGB24,
                                           SWS_BICUBIC, NULL, NULL, NULL);
                    }

                    sws_scale(impl->conv_ctx, impl->av_frame->data, impl->av_frame->linesize, 0,
                              impl->codec_ctx->height, impl->gl_frame->data,
                              impl->gl_frame->linesize);

                    _image->update_data(impl->gl_frame->data[0]);
                   
                }
            }

            av_free_packet(impl->packet);
        } while (impl->packet->stream_index != impl->stream_idx);
    }
} // namespace elemd
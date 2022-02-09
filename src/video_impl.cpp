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
        format_context = avformat_alloc_context(); 

        // open video
        if (avformat_open_input(&format_context, file_path.c_str(), NULL, NULL) < 0)
        {
            std::cout << "failed to open input" << std::endl;
            // clearAppData(&data);
            return;
        }

        // find stream info
        if (avformat_find_stream_info(format_context, NULL) < 0)
        {
            std::cout << "failed to get stream info" << std::endl;
            // clearAppData(&data);
            return;
        }

        // find the video stream
        for (unsigned int i = 0; i < format_context->nb_streams; ++i)
        {
            AVCodecParameters* local_codec_parameters = format_context->streams[i]->codecpar;     
            AVCodec* local_codec = avcodec_find_decoder(local_codec_parameters->codec_id);

            if (local_codec == nullptr)
            {
                continue;
            }

            if (format_context->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
            {
                if (video_stream_index == -1)
                {
                    video_stream_index = i;
                    codec = local_codec;
                    codec_parameters = local_codec_parameters;
                }
            }
        }

        if (video_stream_index == -1)
        {
            std::cout << "failed to find video stream" << std::endl;
            return;
        }

        codec_context = avcodec_alloc_context3(codec);

        if(avcodec_parameters_to_context(codec_context, codec_parameters) < 0)
        {
            std::cout << "failed to copy codec params to codec context" << std::endl;
            return;
        }

        // open the codec
        if (avcodec_open2(codec_context, codec, NULL) < 0)
        {
            std::cout << "failed to open codec" << std::endl;
            return;
        }

        // allocate the video frames
        //av_frame = av_frame_alloc();
        gl_frame = av_frame_alloc();

        packet = av_packet_alloc();
        if (!packet)
        {
            std::cout << "failed to create packet" << std::endl;
            return;
        }

       	int frame_bytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, codec_context->width,
                                                   codec_context->height, 16);
        uint8_t* internal_buffer = (uint8_t*)av_malloc(frame_bytes * sizeof(uint8_t));
        av_image_fill_arrays(gl_frame->data, gl_frame->linesize, internal_buffer,
                             AV_PIX_FMT_RGB24, codec_context->width, codec_context->height, 1);
        

        _image = elemd::Image::create(codec_context->width, codec_context->height, 3, internal_buffer);

        _loaded = true;
    }

    VideoImpl::~VideoImpl()
    {
        if (_loaded)
        {
            avformat_close_input(&format_context);

            if (packet)
                av_packet_free(&packet);
            //if (av_frame)
                //av_frame_free(&av_frame);
            if (gl_frame)
                av_frame_free(&gl_frame);
            if (codec_context)
                avcodec_free_context(&codec_context);


            _image->destroy();

            _loaded = false;
        }
    }

    int decode_packet(elemd::Image* image, AVPacket* pPacket, AVCodecContext* pCodecContext, AVFrame* pFrame)
    {
        // Supply raw packet data as input to a decoder
        // https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3
        int response = avcodec_send_packet(pCodecContext, pPacket);

        if (response < 0)
        {
            std::cout << "Error while sending a packet to the decoder\n";
            return response;
        }

        while (response >= 0)
        {
            // Return decoded output data (into a frame) from a decoder
            // https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c
            response = avcodec_receive_frame(pCodecContext, pFrame);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF)
            {
                break;
            }
            else if (response < 0)
            {
                std::cout << "Error while receiving a frame from the decoder\n";
                return response;
            }

            if (response >= 0)
            {
                /*                std::cout << "Frame %d (type=%c, size=%d bytes, format=%d) pts %d key_frame %d [DTS %d]",
                        pCodecContext->frame_number, av_get_picture_type_char(pFrame->pict_type),
                        pFrame->pkt_size, pFrame->format, pFrame->pts, pFrame->key_frame,
                        pFrame->coded_picture_number);
                        */

           
                // save a grayscale frame into a .pgm file
                image->update_data(pFrame->data[0]);

                //save_gray_frame(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height,
                //                frame_filename);
            }
        }
        return 0;
    }

    void Video::read_next()
    {
        VideoImpl* impl = getImpl(this);

        int response = 0;

        do
        {

            if (impl->packet->stream_index == impl->video_stream_index)
            {
                response =
                    decode_packet(impl->_image, impl->packet, impl->codec_context, impl->gl_frame);

                if (response < 0)
                    break;

                //_image->update_data(impl->gl_frame->data[0]);

                /*
                if (frame_finished)
                {
                    if (!impl->conv_ctx)
                    {
                        impl->conv_ctx =
                            sws_getContext(impl->codec_context->width, impl->codec_context->height,
                                           impl->codec_context->pix_fmt, impl->codec_context->width,
                                           impl->codec_context->height,
                            AV_PIX_FMT_RGB24,
                                           SWS_BICUBIC, NULL, NULL, NULL);
                    }

                    sws_scale(impl->conv_ctx, impl->av_frame->data, impl->av_frame->linesize, 0,
                              impl->codec_context->height, impl->gl_frame->data,
                              impl->gl_frame->linesize);



                }
                */
            }

            av_packet_unref(impl->packet);
        } while (av_read_frame(impl->format_context, impl->packet));
    }
} // namespace elemd
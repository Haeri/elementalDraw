#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>


#include "player.hpp"
#include "level.hpp"
#include "audio.hpp"

#include <iostream>

/*
void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
    ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
    if (pDecoder == NULL)
    {
        return;
    }

    /* Reading PCM frames will loop based on what we specified when called
     * ma_data_source_set_looping(). * /
    ma_data_source_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

    (void)pInput;
}
*/
int main()
{
    /*
    ma_result result;
    ma_decoder decoder;
    ma_device_config deviceConfig;
    ma_device device;

    result = ma_decoder_init_file("./res/music.mp3", NULL, &decoder);
    if (result != MA_SUCCESS)
    {
        return -2;
    }

    /*
    A decoder is a data source which means we just use ma_data_source_set_looping() to set the
    looping state. We will read data using ma_data_source_read_pcm_frames() in the data callback.
    * /
    ma_data_source_set_looping(&decoder, MA_TRUE);

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format = decoder.outputFormat;
    deviceConfig.playback.channels = decoder.outputChannels;
    deviceConfig.sampleRate = decoder.outputSampleRate;
    deviceConfig.dataCallback = data_callback;
    deviceConfig.pUserData = &decoder;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS)
    {
        printf("Failed to open playback device.\n");
        ma_decoder_uninit(&decoder);
        return -3;
    }

    if (ma_device_start(&device) != MA_SUCCESS)
    {
        printf("Failed to start playback device.\n");
        ma_device_uninit(&device);
        ma_decoder_uninit(&decoder);
        return -4;
    }

    ma_device_set_master_volume(&device, 0.1f);
    */


   Audio audio;

   audio.registerSound("./audio/jump.waf", "jump");




    const int screenWidth = 18 * 16;
    const int screenHeight = 18 * 12;

    // configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Platformer", screenWidth, screenHeight};
    winc.vsync = true;
    winc.icon_file = "./res/logo.png";

    // winc.native_pixel_size = true;
    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();

    elemd::ImageConfig imgc = {false, elemd::NEAREST};

    elemd::Font* silkscreen = elemd::Font::create("./res/font/silkscreen/slkscr.ttf");
    ctx->_tmp_register_font(silkscreen);

    // load image
    elemd::Image* mapTileMap = elemd::Image::create("./res/tiles_packed.png", imgc);
    ctx->_tmp_register_image(mapTileMap);

    elemd::Image* characterTileMap = elemd::Image::create("./res/characters_packed.png", imgc);
    ctx->_tmp_register_image(characterTileMap);

    ctx->_tmp_prepare();

    ctx->set_font(silkscreen);
    ctx->set_clear_color({223, 246, 245, 255});

    Player* player = new Player(win, characterTileMap);

    Level level = Level(18);
    level.loadLevelFile("./res/levels/level_0.level", player);

    
    win->add_key_listener([&](elemd::key_event event) {        
        if (event.key == elemd::KEY_R && event.mods == elemd::KEY_MOD_CONTROL)
        {
            level.loadLevelFile("./res/levels/level_0.level", player);
        }
    });

    double target_frame_time = 1.0f / 120.0f;
    double delta_time = 0;
    double frame_start = elemd::Window::now();
    double frame_end = 0;
    double next_timestamp = 0;

    int emty_cycles = 0;

    double swap_start = 0;
    double swaptime = 0;

    float cameraPadding = 7;

    long long frame_count = -1;

    elemd::vec2 cam = {0, 0};    

    ctx->draw_frame();

    // Main renderloop
    while (win->is_running())
    {

        next_timestamp = frame_start + (target_frame_time - swaptime);

        while (elemd::Window::now() < next_timestamp)
        {
            ++emty_cycles;
        };

        swap_start = elemd::Window::now();
        ctx->present_frame();
        swaptime = elemd::Window::now() - swap_start;
        ++frame_count;

        // ----------------------- FRAME BORDER -----------------------
        frame_end = elemd::Window::now();
        delta_time = frame_end - frame_start;
        if (delta_time > 0.017)
        {
            delta_time = 0.017;
        }

        frame_start = frame_end;

        win->poll_events();
        
        player->doInput(delta_time);       
        player->simulate(delta_time);

        float padding_left =
            player->getPosition().get_x() - screenWidth / 2 - screenWidth / cameraPadding;
        float padding_right =
            player->getPosition().get_x() + 24 - screenWidth / 2 + screenWidth / cameraPadding;

        if (cam.x() < padding_left &&
            cam.x() <= level.getCols() * level.getTileSize() - screenWidth)
        {
            cam.x() = padding_left;
        }
        else if (cam.x() > padding_right && cam.x() > 0)
        {
            cam.x() = padding_right;
        }

        for (int y = 0; y < level.getRows(); ++y)
        {
            for (int x = 0; x < level.getCols(); ++x)
            {
                Block b = level.getAt(x, y);

                if (!b.render)
                    continue;

                ctx->draw_image(x * level.getTileSize() - cam.get_x(),
                                y * level.getTileSize() - cam.get_y(), level.getTileSize(),
                                level.getTileSize(), mapTileMap, b.uv.x(), b.uv.y(),
                                level.getTextureSize(), level.getTextureSize());
            }
        }

        player->render(cam, delta_time);

        // Draw Info
        ctx->set_font_size(18);
        ctx->set_fill_color({30, 30, 30, 255});
        int dth = (int)(delta_time * 1000) * 100.0f;
        int dtfr = (int)(delta_time * 100000) - dth;
        ctx->draw_text(2, 2, "DT: " + std::to_string(dth / 100) + "." + std::to_string(dtfr));

        ctx->draw_frame();

        emty_cycles = 0;
    }

    delete player;

    // Cleanup
    silkscreen->destroy();
    mapTileMap->destroy();
    characterTileMap->destroy();
    win->destroy();

    //ma_device_uninit(&device);
    //ma_decoder_uninit(&decoder);

    return 0;
}
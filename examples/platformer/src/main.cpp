#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>

#include "player.hpp"
#include "level.hpp"

#include <iostream>

int main()
{
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

    Player* player = new Player(ctx, characterTileMap);

    Level level = Level(18);
    level.loadLevelFile("./res/level_0.level", player);

    bool left_key = false, right_key = false;

    win->add_key_listener([&](elemd::key_event event) {
        int start_vel = 0;

        if (event.key == elemd::KEY_LEFT)
        {
            if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
            {
                left_key = true;
            }
            else if (event.action == elemd::ACTION_RELEASE)
            {
                left_key = false;
            }
        }
        else if (event.key == elemd::KEY_RIGHT)
        {
            if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
            {
                right_key = true;
            }
            else if (event.action == elemd::ACTION_RELEASE)
            {
                right_key = false;
            }
        }

        if (event.key == elemd::KEY_UP)
        {
            if (event.action == elemd::ACTION_PRESS)
            {
                player->jump();
            }
        }

        if (event.key == elemd::KEY_R && event.mods == elemd::KEY_MOD_CONTROL)
        {
            level.loadLevelFile("./res/level_0.level", player);
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

        if (left_key)
        {
            player->move(true, delta_time);
        }
        else if (right_key)
        {
            player->move(false, delta_time);
        }

        player->tick(delta_time);

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
                                level.getTileSize(), level.getTileSize());
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

    return 0;
}
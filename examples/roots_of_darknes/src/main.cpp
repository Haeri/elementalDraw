#include <elemd/audio/audio.hpp>
#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>
#include <elemd/ui/document.hpp>
#include <elemd/ui/heading.hpp>

#include "level.hpp"
#include "player.hpp"

#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main()
{
    elemd::Audio audio;
    audio.registerSound("./res/music.mp3", "music");
    //audio.playSound("./res/music.mp3");
    audio.registerSound("./res/audio/jump.wav", "jump");

    const int screenWidth = MAP_TILE_SIZE * 30;
    const int screenHeight = MAP_TILE_SIZE * 20;

    // configure and create window
    elemd::WindowConfig winc = elemd::WindowConfig{"Roots of Darknes", screenWidth, screenHeight};
    
#ifdef __APPLE__
    winc.native_pixel_size = true;
#endif

    winc.vsync = false;
    winc.icon_file = "./res/app.png";

    elemd::Window* win = elemd::Window::create(winc);
    elemd::Context* ctx = win->create_context();
    


    elemd::ImageConfig imgc = {false, elemd::NEAREST};

    elemd::Font* silkscreen = elemd::Font::create("./res/font/silkscreen/slkscr.ttf");
    ctx->_tmp_register_font(silkscreen);

    // load image
    elemd::Image* mapTileMap = elemd::Image::create("./res/tilemap_packed.png", imgc);
    ctx->_tmp_register_image(mapTileMap);

    elemd::Image* characterTileMap = elemd::Image::create("./res/characters_packed.png", imgc);
    ctx->_tmp_register_image(characterTileMap);

    //ctx->_tmp_prepare();

    ctx->set_font(silkscreen);
    ctx->set_clear_color({0, 0, 0, 255});



    elemd::Document doc(win, ctx);

    elemd::Element body;
    body.id = "body";
    body.style.background_color = elemd::color("#1d1d1d");
    body.style.border_radius[0] = 5;
    body.style.border_radius[1] = 5;
    body.style.border_radius[2] = 5;
    body.style.border_radius[3] = 5;
    body.style.padding[0] = 0;
    body.style.padding[1] = 6;
    body.style.padding[2] = 6;
    body.style.padding[3] = 6;
    body.style.margin[0] = screenHeight - (6 + 20);
    //body.style.height.set_percent(30);

    elemd::Heading solution;
    solution.set_text("Roots of Darknes");
    solution.style.color = elemd::color(255, 255, 255);
    solution.style.font_family = silkscreen;
    solution.style.font_size = 20;
    body.add_child(&solution);


    doc.add_child(&body);
    doc._tmp_prepare();


    Level level = Level();
    
    Player* player = new Player(win, characterTileMap, {90, 100}, &level);
    level.loadLevelFile("./res/levels/level_0.json", player);

    win->add_key_listener([&](elemd::key_event event) {
        if (event.key == elemd::KEY_R && event.mods == elemd::KEY_MOD_CONTROL)
        {
            level.loadLevelFile("./res/levels/level_0.level", player);
        }

        /*
        if (event.key == elemd::KEY_UP)
        {
            audio.playSound("./res/audio/jump.wav");
        }
        */
    });

    double target_frame_time = 1.0f / 120.0f;
    double delta_time = 0;
    double frame_start = elemd::Window::now();
    double frame_end = 0;
    double next_timestamp = 0;

    int emty_cycles = 0;

    float cameraPadding = 7;

    long long frame_count = -1;

    elemd::vec2 cam = {0, 0};

    ctx->draw_frame();

    // Main renderloop
    while (win->is_running())
    {

        next_timestamp = frame_start + target_frame_time;

        while (elemd::Window::now() < next_timestamp)
        {
            ++emty_cycles;
        };

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

        
        for (int i = 0; i < level.backgroundLayer.size(); ++i)
        {

                ctx->draw_image(level.backgroundLayer[i].row * level.getTileSize() - cam.get_x(),
                            level.backgroundLayer[i].col * level.getTileSize() - cam.get_y(), level.getTileSize(),
                            level.getTileSize(), mapTileMap,
                            level.backgroundLayer[i].offsetX * level.getTileSize(),
                            level.backgroundLayer[i].offsetY * level.getTileSize(),
                                level.getTextureSize(), level.getTextureSize());
        }        

        player->render(cam, delta_time);
                
        for (int i = 0; i < level.collisionLayer.size(); ++i)
        {

                ctx->draw_image(level.collisionLayer[i].row * level.getTileSize() - cam.get_x(),
                                level.collisionLayer[i].col * level.getTileSize() - cam.get_y(),
                                level.getTileSize(), level.getTileSize(), mapTileMap,
                                level.collisionLayer[i].offsetX * level.getTileSize(),
                                level.collisionLayer[i].offsetY * level.getTileSize(),
                                level.getTextureSize(),
                                level.getTextureSize());
        }


        // Draw Info
        ctx->set_fill_color({0, 0, 0, 160});
        ctx->fill_rect(1, 4, 90, 20);

        ctx->set_font_size(18);
        ctx->set_fill_color({230, 230, 230, 255});
        int dth = (int)(delta_time * 1000) * 100.0f;
        int dtfr = (int)(delta_time * 100000) - dth;
        ctx->draw_text(2, 2, "DT: " + std::to_string(dth / 100) + "." + std::to_string(dtfr));

        ctx->draw_text(2, 20,
                       "X: " + std::to_string(player->getPosition().get_x()) +
                           " Y:" + std::to_string(player->getPosition().get_y()));

        
        doc.paint(delta_time);
        //ctx->draw_frame();
        //ctx->present_frame();

        emty_cycles = 0;
    }

    delete player;

    // Cleanup
    silkscreen->destroy();
    mapTileMap->destroy();
    characterTileMap->destroy();
    //win->destroy();

    return 0;
}

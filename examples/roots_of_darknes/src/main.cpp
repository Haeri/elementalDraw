#include <elemd/audio/audio.hpp>
#include <elemd/color.hpp>
#include <elemd/context.hpp>
#include <elemd/window.hpp>

#include "level.hpp"
#include "player.hpp"

#include <iostream>

#include <json/json.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int main()
{
    struct TileData
    {
        unsigned char tileSymbol;
        unsigned char tilesetIdx;
        int x;
        int y;
    };

    std::ifstream ifs("./res/levels/level_0.json");
    Json::Reader reader;
    Json::Value obj;
    bool ok = reader.parse(ifs, obj); // reader can also read strings
    const Json::Value& tiles = obj["tileSets"]["2"]["tileData"];

    std::vector<TileData> tileMap;

    std::vector<std::string> keys = tiles.getMemberNames();

    for (int i = 0; i < keys.size(); ++i)
    {
        const std::string& key = keys[i];

        TileData td {};
        td.tileSymbol = tiles[key]["tileSymbol"].asCString()[0];
        td.x = tiles[key]["x"].asInt();
        td.y = tiles[key]["y"].asInt();

        tileMap.push_back(td);

        //std::cout << key << " - " << td.tileSymbol << " x:" << td.x << " y:" << td.y << std::endl;
    }


     struct MapTile
    {
        unsigned char tileSymbol;
        int offsetX;
        int offsetY;
        int row;
        int col;
    };

    std::vector<MapTile> bgLayer;
    const Json::Value& backgroundTiles = obj["maps"]["Map_1"]["layers"][0]["tiles"];
    
    std::vector<std::string> bgKeys = backgroundTiles.getMemberNames();
    
    for (int i = 0; i < bgKeys.size(); ++i)
    {
        const std::string& key = bgKeys[i];


        std::string segment;
        std::vector<std::string> seglist;
        std::stringstream test(key);

        while (std::getline(test, segment, '-'))
        {
            seglist.push_back(segment);
        }

        MapTile td{};
        td.tileSymbol = backgroundTiles[key]["tileSymbol"].asCString()[0];
        td.offsetX = backgroundTiles[key]["x"].asInt();
        td.offsetY = backgroundTiles[key]["y"].asInt();
        td.row = stoi(seglist[0]);
        td.col = stoi(seglist[1]);

        bgLayer.push_back(td);

        //std::cout << key << " - " << td.tileSymbol << " x:" << td.offsetX << " y:" << td.offsetY << std::endl;
    }







    std::vector<MapTile> collisionLayer;
    const Json::Value& collisionTiles = obj["maps"]["Map_1"]["layers"][1]["tiles"];

    bgKeys = collisionTiles.getMemberNames();

    for (int i = 0; i < bgKeys.size(); ++i)
    {
        const std::string& key = bgKeys[i];

        std::string segment;
        std::vector<std::string> seglist;
        std::stringstream test(key);

        while (std::getline(test, segment, '-'))
        {
            seglist.push_back(segment);
        }

        MapTile td{};
        td.tileSymbol = collisionTiles[key]["tileSymbol"].asCString()[0];
        td.offsetX = collisionTiles[key]["x"].asInt();
        td.offsetY = collisionTiles[key]["y"].asInt();
        td.row = stoi(seglist[0]);
        td.col = stoi(seglist[1]);

        collisionLayer.push_back(td);

        std::cout << key << " - " << td.tileSymbol << " x:" << td.offsetX << " y:" << td.offsetY
                  << std::endl;
    }

   
    

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

    //winc.native_pixel_size = true;
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

    ctx->_tmp_prepare();

    ctx->set_font(silkscreen);
    ctx->set_clear_color({0, 0, 0, 255});


    Level level = Level();
    
    Player* player = new Player(win, characterTileMap, {90, 100}, &level);
    level.loadLevelFile("./res/levels/level_0.level", player);

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


        /*
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
        */

        
        for (int i = 0; i < bgLayer.size(); ++i)
        {

                ctx->draw_image(bgLayer[i].row * level.getTileSize() - cam.get_x(),
                            bgLayer[i].col * level.getTileSize() - cam.get_y(), level.getTileSize(),
                            level.getTileSize(), mapTileMap,
                            bgLayer[i].offsetX * level.getTileSize(),
                            bgLayer[i].offsetY * level.getTileSize(),
                                level.getTextureSize(), level.getTextureSize());
        }        

        player->render(cam, delta_time);


                for (int i = 0; i < collisionLayer.size(); ++i)
        {

                ctx->draw_image(collisionLayer[i].row * level.getTileSize() - cam.get_x(),
                                collisionLayer[i].col * level.getTileSize() - cam.get_y(),
                                level.getTileSize(), level.getTileSize(), mapTileMap,
                                collisionLayer[i].offsetX * level.getTileSize(),
                                collisionLayer[i].offsetY * level.getTileSize(),
                                level.getTextureSize(),
                                level.getTextureSize());
        }


        // Draw Info
        ctx->set_font_size(18);
        ctx->set_fill_color({30, 30, 30, 255});
        int dth = (int)(delta_time * 1000) * 100.0f;
        int dtfr = (int)(delta_time * 100000) - dth;
        ctx->draw_text(2, 2, "DT: " + std::to_string(dth / 100) + "." + std::to_string(dtfr));

        ctx->draw_text(2, 20,
                       "X: " + std::to_string(player->getPosition().get_x()) +
                           " Y:" + std::to_string(player->getPosition().get_y()));

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

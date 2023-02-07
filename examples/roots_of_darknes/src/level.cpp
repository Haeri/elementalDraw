#include "level.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>

#include "player.hpp"

bool Level::getCollideAtWorld(float x, float y)
{
    int _x = std::floor(x / MAP_TILE_SIZE);
    int _y = std::floor(y / MAP_TILE_SIZE);
    int key = _y * _cols + _x;
    if (_x < 0 || _y < 0 || _x >= _cols || _y >= _rows)
    {
        return true;
    }
    return (std::binary_search(collisionIndex.begin(), collisionIndex.end(), key));
}

int Level::getTileSize()
{
    return _tileSize;
}

int Level::getTextureSize()
{
    return _textureSize;
}

int Level::getRows()
{
    return _rows;
}

int Level::getCols()
{
    return _cols;
}

void Level::loadLevelFile(std::string filePath, Player* p)
{
    std::ifstream levelFile(filePath);
    if (!levelFile.is_open())
    {
        std::cerr << "Error: Could not find map file!" << std::endl;
        return;
    }

    Json::Reader reader;
    Json::Value obj;

    if (!reader.parse(levelFile, obj)) // reader can also read strings
    {
        std::cerr << "Error: Could not parse map file!" << std::endl;
        return;
    }

    /*

    struct TileData
    {
        unsigned char tileSymbol;
        unsigned char tilesetIdx;
        int x;
        int y;
    };
    bool ok = reader.parse(levelFile, obj); // reader can also read strings
    const Json::Value& tiles = obj["tileSets"]["2"]["tileData"];

    std::vector<TileData> tileMap;

    std::vector<std::string> keys = tiles.getMemberNames();

    for (int i = 0; i < keys.size(); ++i)
    {
        const std::string& key = keys[i];

        TileData td{};
        td.tileSymbol = tiles[key]["tileSymbol"].asCString()[0];
        td.x = tiles[key]["x"].asInt();
        td.y = tiles[key]["y"].asInt();

        tileMap.push_back(td);

        // std::cout << key << " - " << td.tileSymbol << " x:" << td.x << " y:" << td.y <<
        // std::endl;
    }
    */

    _rows = obj["maps"]["level"]["mapHeight"].asInt();
    _cols = obj["maps"]["level"]["mapWidth"].asInt();

    const Json::Value& layers = obj["maps"]["level"]["layers"];

    parseLayer(layers, 0, backgroundLayer);
    parseLayer(layers, 1, folliageLayer);
    parseLayer(layers, 2, collisionLayer);
    parseLayer(layers, 3, itemLayer);

    levelFile.close();
}

void Level::parseLayer(const Json::Value& obj, int index, std::vector<MapTile>& layer)
{
    const Json::Value& tiles = obj[index]["tiles"];
    bool isCollision = obj[index]["name"].asString() == "collision";

    std::vector<std::string> indexList = tiles.getMemberNames();

    for (int i = 0; i < indexList.size(); ++i)
    {
        const std::string& key = indexList[i];

        std::string segment;
        std::vector<std::string> seglist;
        std::stringstream test(key);

        while (std::getline(test, segment, '-'))
        {
            seglist.push_back(segment);
        }

        MapTile td{};
        td.tileSymbol = tiles[key]["tileSymbol"].asCString()[0];
        td.offsetX = tiles[key]["x"].asInt();
        td.offsetY = tiles[key]["y"].asInt();
        td.row = stoi(seglist[0]);
        td.col = stoi(seglist[1]);

        layer.push_back(td);

        if (isCollision)
        {
            collisionIndex.push_back(td.col * _cols + td.row);

            rect r{};
            r.pos = {td.row * MAP_TILE_SIZE, td.col * MAP_TILE_SIZE};
            r.size = {MAP_TILE_SIZE, MAP_TILE_SIZE};
            collisionRects.push_back(r);
        }
    }
    if (isCollision)
    {
        std::sort(collisionIndex.begin(), collisionIndex.end());
    }
}

void Level::render(const elemd::vec2& cam)
{
    renderLayer(cam, backgroundLayer);
    renderLayer(cam, folliageLayer);
    renderLayer(cam, collisionLayer);
    renderLayer(cam, itemLayer);
}

void Level::renderLayer(const elemd::vec2& cam, const std::vector<MapTile>& layer)
{
    for (int i = 0; i < layer.size(); ++i)
    {

        _ctx->draw_image(layer[i].row * getTileSize() - cam.get_x(),
                         layer[i].col * getTileSize() - cam.get_y(), getTileSize(), getTileSize(),
                         _tileMap, layer[i].offsetX * getTileSize(),
                         layer[i].offsetY * getTileSize(), getTextureSize(), getTextureSize());
    }
}

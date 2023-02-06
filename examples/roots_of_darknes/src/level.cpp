#include "level.hpp"

#include <fstream>
#include <iostream>
#include <string.h>

#include <json/json.h>


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
    

    _rows = obj["maps"]["Map_1"]["mapHeight"].asInt();
    _cols = obj["maps"]["Map_1"]["mapWidth"].asInt();
    
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

        backgroundLayer.push_back(td);
    }

    
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
        collisionIndex.push_back(td.col * _cols + td.row);
    }
    std::sort(collisionIndex.begin(), collisionIndex.end());

    levelFile.close();
}

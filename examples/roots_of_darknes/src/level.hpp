#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <elemd/vec2.hpp>
#include <map>
#include <string>

#define MAP_TILE_SIZE 16
#define SPRITE_SCALAT_POT 1

class Player;

struct Block
{
    unsigned char id;
    bool collide;
    bool render;
    elemd::vec2 uv;
};

class Level
{
public:
    static std::map<unsigned int, Block> BlockDb;

    
    ~Level();
    Block getAt(int x, int y);
    bool getCollideAtWorld(float x, float y);
    int getTileSize();
    int getTextureSize();
    int getRows();
    int getCols();
    const unsigned char* getData();
    void loadLevelFile(std::string filePath, Player* p);

private:
    int _tileSize = MAP_TILE_SIZE;
    int _textureSize = MAP_TILE_SIZE * SPRITE_SCALAT_POT;
    int _rows = -1;
    int _cols = -1;

    unsigned char* _data = nullptr;
};

#endif // LEVEL_HPP

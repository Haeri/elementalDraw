#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <elemd/vec2.hpp>
#include <map>
#include <string>

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

    Level(int tileSize) : _tileSize(tileSize), _textureSize(tileSize)
    {
    }
    Level(int tileSize, int textureSize) : _tileSize(tileSize), _textureSize(textureSize)
    {
    }
    ~Level();
    Block getAt(int x, int y);
    int getTileSize();
    int getTextureSize();
    int getRows();
    int getCols();
    const unsigned char* getData();
    void loadLevelFile(std::string filePath, Player* p);

private:
    int _tileSize = -1;
    int _textureSize = -1;
    int _rows = -1;
    int _cols = -1;

    unsigned char* _data = nullptr;
};

#endif // LEVEL_HPP

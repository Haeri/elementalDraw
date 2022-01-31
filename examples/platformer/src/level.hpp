#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <elemd/vec2.hpp>
#include <string>
#include <map>

class Player;

struct Block
{
    unsigned char id;
    bool collide;
    bool render;
    elemd::vec2 uv;
};


class Level {
public:
    static std::map<unsigned int, Block> BlockDb;

    Level(int tileSize) : _tileSize(tileSize)
    {
    }
    ~Level();
    Block getAt(int x, int y);
    int getTileSize();
    int getRows();
    int getCols();
    const unsigned char* getData();
    void loadLevelFile(std::string filePath, Player* p);

private:
    int _tileSize = -1;
    int _rows = -1;
    int _cols = -1;

    unsigned char* _data = nullptr;
};

#endif // LEVEL_HPP

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <elemd/vec2.hpp>
#include <map>
#include <vector>

#define MAP_TILE_SIZE 16
#define SPRITE_SCALAT_POT 1

class Player;

struct MapTile
{
    unsigned char tileSymbol;
    int offsetX;
    int offsetY;
    int row;
    int col;
};

class Level
{
public:
    std::vector<MapTile> backgroundLayer;
    std::vector<MapTile> collisionLayer;
    std::vector<int> collisionIndex;

    bool getCollideAtWorld(float x, float y);
    int getTileSize();
    int getTextureSize();
    int getRows();
    int getCols();
    void loadLevelFile(std::string filePath, Player* p);

private:
    int _tileSize = MAP_TILE_SIZE;
    int _textureSize = MAP_TILE_SIZE * SPRITE_SCALAT_POT;
    int _rows = -1;
    int _cols = -1;
};

#endif // LEVEL_HPP

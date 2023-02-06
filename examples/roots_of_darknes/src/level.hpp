#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <elemd/vec2.hpp>
#include <elemd/image.hpp>
#include <elemd/context.hpp>
#include <map>
#include <vector>

#include <json/json.h>

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
    std::vector<MapTile> folliageLayer;
    std::vector<MapTile> collisionLayer;
    std::vector<MapTile> itemLayer;

    std::vector<int> collisionIndex;

    Level(elemd::Context* ctx, elemd::Image* mapTileMap) : _ctx(ctx), _tileMap(mapTileMap)
    {    
    }

    bool getCollideAtWorld(float x, float y);
    int getTileSize();
    int getTextureSize();
    int getRows();
    int getCols();
    void loadLevelFile(std::string filePath, Player* p);
    void render(const elemd::vec2& cam);

private:
    void renderLayer(const elemd::vec2& cam, const std::vector<MapTile>& layer);
    void parseLayer(const Json::Value& obj, int index, std::vector<MapTile>& layer);

    elemd::Context* _ctx;
    elemd::Image* _tileMap;

    int _tileSize = MAP_TILE_SIZE;
    int _textureSize = MAP_TILE_SIZE * SPRITE_SCALAT_POT;
    int _rows = -1;
    int _cols = -1;
};

#endif // LEVEL_HPP

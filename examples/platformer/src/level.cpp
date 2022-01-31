#include "level.hpp"

#include <iostream>
#include <fstream>

#include "player.hpp"

std::map<unsigned int, Block> Level::BlockDb = {
    {'.', {'.', false, false, {0, 0}}}, // Air
    {'$', {'$', false, false, {0, 0}}}, // Spawn
    {'o', {'o', false, false, {0, 0}}}, // OutofBounds
    {'#', {'#', true, true, {0, 0}}}    // Block
};

Level::~Level()
{
    delete[] _data;
}

Block Level::getAt(int x, int y)
{
    //std::cout << "At " << y * _cols + x << "\n";
    if (x < 0 || y < 0 || x >= _cols || y >= _rows)
    {
        return BlockDb['o'];        
    }    
    
    unsigned char id = _data[y * _cols + x];
    return BlockDb[id];
}

int Level::getTileSize()
{
    return _tileSize;
}

int Level::getRows()
{
    return _rows;
}

int Level::getCols()
{
    return _cols;
}

const unsigned char* Level::getData()
{
    return _data;
}

void Level::loadLevelFile(std::string filePath, Player* p)
{
    std::string line;
    std::ifstream levelFile(filePath);
    bool spawned = false;
    if (levelFile.is_open())
    {
        // rows
        std::getline(levelFile, line);
        _rows = std::stoi(line);

        // cols
        std::getline(levelFile, line);
        _cols = std::stoi(line);

        // data
        _data = new unsigned char[_rows * _cols];

        for (int y = 0; y < _rows; ++y)
        {
            std::getline(levelFile, line);

            char* token = strtok(line.data(), ", ");
            _data[y * _cols] = *token;
            std::cout << token << /*"(" << (y * _cols) << ")" <<*/ ", ";
            

            for (int x = 1; x < _cols; ++x)
            {
                token = strtok(NULL, ", ");
                _data[y * _cols + x] = *token;

                if (*token == '$')
                {
                    p->setLevelSpawn(this, {x * _tileSize, y * _tileSize});
                    spawned = true;
                }

                std::cout << token << /*"(" << (y * _cols + x) << ")" <<*/ ", ";
                
            }
            std::cout << std::endl;
        }
        
        levelFile.close();

        if (!spawned)
        {
            p->setLevelSpawn(this, {30, 30});
        }
    }
}

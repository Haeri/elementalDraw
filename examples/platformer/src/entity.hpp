#ifndef ENTITY_H
#define ENTITY_H

#include <elemd/vec2.hpp>
#include <elemd/image.hpp>

class Entity {
public:
    Entity(int min);
    void render();

private :
    elemd::vec2 _pos = elemd::vec2::ZERO;
    elemd::image* _sprite = nullptr;
    int _spriteIndex  = 0;

};

#endif // ENTITY_H
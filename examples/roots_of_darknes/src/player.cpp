#include "player.hpp"

void Player::doInput(double dt)
{
    // new potential position
    elemd::vec2 input = {
        (float) (_horizontal_input * dt),
        (float) (_vertical_input * dt)
    };
    input = input.normalize();
    input = input * PLAYER_SPEED;
    
    elemd::vec2 newPos = _rect->pos + input;
    
    // collision detection
    if(input.get_x() < 0) {
        if(
           _level->getCollideAtWorld(newPos.get_x(), newPos.get_y()) ||
           _level->getCollideAtWorld(newPos.get_x(), newPos.get_y() + _rect->size.y()-1)
        ) {
               newPos.x() = (std::floor(newPos.get_x() / MAP_TILE_SIZE) + 1) * MAP_TILE_SIZE;
        }
    } else if(input.get_x() > 0) {
        if(
           _level->getCollideAtWorld(newPos.get_x() + _rect->size.x()-1, newPos.get_y()) ||
           _level->getCollideAtWorld(newPos.get_x() + _rect->size.x()-1, newPos.get_y() + _rect->size.y()-1)
        ) {
               newPos.x() = (std::floor(newPos.get_x() / MAP_TILE_SIZE) + 2) * MAP_TILE_SIZE - _rect->size.x();
        }
    }
    
    if(input.get_y() < 0) {
        if(
           _level->getCollideAtWorld(newPos.get_x(), newPos.get_y()) ||
           _level->getCollideAtWorld(newPos.get_x() + _rect->size.x()-1, newPos.get_y())
        ) {
            newPos.y() = (std::floor(newPos.get_y() / MAP_TILE_SIZE) + 1) * MAP_TILE_SIZE;
        }
    } else if(input.get_y() > 0) {
        if(
           _level->getCollideAtWorld(newPos.get_x(), newPos.get_y() + _rect->size.y()-1) ||
           _level->getCollideAtWorld(newPos.get_x() + _rect->size.x()-1, newPos.get_y() + _rect->size.y()-1)
        ) {
            newPos.y() = (std::floor(newPos.get_y() / MAP_TILE_SIZE) + 2) * MAP_TILE_SIZE - _rect->size.y();
        }
    }
    
    // apply new position
    _rect->pos.x() = newPos.get_x();
    _rect->pos.y() = newPos.get_y();
    
}

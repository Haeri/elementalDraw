#include "player.hpp"

#include <algorithm>
#include <limits>

void Player::doInput(double dt)
{
    // new potential position
    elemd::vec2 input = {(float)(_horizontal_input), (float)(_vertical_input)};
    input = input.normalize();

    _rect->vel = _rect->vel + (input * PLAYER_SPEED * dt);
    _rect->vel = _rect->vel - (_rect->vel * PLAYER_FRICTION * dt);

    /*
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
           _level->getCollideAtWorld(newPos.get_x() + _rect->size.x()-1, newPos.get_y() +
    _rect->size.y()-1) ) { newPos.x() = (std::floor(newPos.get_x() / MAP_TILE_SIZE) + 2) *
    MAP_TILE_SIZE - _rect->size.x();
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
           _level->getCollideAtWorld(newPos.get_x() + _rect->size.x()-1, newPos.get_y() +
    _rect->size.y()-1) ) { newPos.y() = (std::floor(newPos.get_y() / MAP_TILE_SIZE) + 2) *
    MAP_TILE_SIZE - _rect->size.y();
        }
    }

    // apply new position
    _rect->pos.x() = newPos.get_x();
    _rect->pos.y() = newPos.get_y();
    */

    // Sort collisions in order of distance
    elemd::vec2 cp, cn;
    float t = 0, min_t = std::numeric_limits<double>::infinity();
    std::vector<std::pair<int, float>> z;

    // Work out collision point, add it to vector along with rect ID
    for (int i = 1; i < _level->collisionRects.size(); i++)
    {
        if (physics::dynamic_rect_to_rect(_rect, dt, _level->collisionRects[i], cp, cn, t))
        {
            z.push_back({i, t});
        }
    }

    // Do the sort
    std::sort(z.begin(), z.end(),
              [](const std::pair<int, float>& a, const std::pair<int, float>& b) {
                  return a.second < b.second;
              });

    // Now resolve the collision in correct order
    for (auto j : z)
        physics::resolve_dynamic_rect_to_rect(_rect, dt, &_level->collisionRects[j.first]);

    // Embellish the "in contact" rectangles in yellow
    for (int i = 0; i < 4; i++)
    {
        // if (_rect.contact[i])
        //     DrawRect(_rect.contact[i]->pos, _rect.contact[i]->size, olc::YELLOW);
        _rect->contact[i] = nullptr;
    }

    // UPdate the player rectangles position, with its modified velocity
    _rect->pos = _rect->pos + _rect->vel * dt;
}

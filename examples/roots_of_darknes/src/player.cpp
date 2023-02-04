#include "player.hpp"

void Player::doInput(double dt)
{
    
    _rect->pos.x() += _horizontal_input * dt * 100;
    _rect->pos.y() += _vertical_input * dt * 100;
    
}

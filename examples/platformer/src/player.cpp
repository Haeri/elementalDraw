#include "player.hpp"

void Player::doInput(double dt)
{
    if (_left_key)
    {
        move(true, dt);
    }
    else if (_right_key)
    {
        move(false, dt);
    }
}

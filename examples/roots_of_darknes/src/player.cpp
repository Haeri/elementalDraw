#include "player.hpp"

void Player::doInput(double dt)
{
    move(_horizontal_input, _vertical_input, dt);
}

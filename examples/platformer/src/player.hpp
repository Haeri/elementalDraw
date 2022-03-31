#ifndef PLAYER_H
#define PLAYER_H

#include <elemd/window.hpp>

#include "entity.hpp"

class Player : public Entity
{
public:
    Player(elemd::Window* window, elemd::Image* img)
        : Entity(window->get_context(), img), _win(window)
    {
        window->add_key_listener([&](elemd::key_event event) {
            int start_vel = 0;

            if (event.key == elemd::KEY_LEFT)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _left_key = true;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _left_key = false;
                }
            }
            else if (event.key == elemd::KEY_RIGHT)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _right_key = true;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _right_key = false;
                }
            }

            if (event.key == elemd::KEY_UP)
            {
                if (event.action == elemd::ACTION_PRESS)
                {
                    jump();
                }
            }            
        });
    }

    void doInput(double dt);

private:
    bool _left_key = false;
    bool _right_key = false;

    elemd::Window* _win = nullptr;

};

#endif // PLAYER_H
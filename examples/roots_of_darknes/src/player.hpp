#ifndef PLAYER_H
#define PLAYER_H

#include <elemd/window.hpp>

#include "entity.hpp"

class Player : public Entity
{
public:
    Player(elemd::Window* window, elemd::Image* img, elemd::vec2 spawn, Level* level)
        : Entity(window->get_context(), img, spawn), _win(window)
    {
        
        _level = level;
        
        window->add_key_listener([&](elemd::key_event event) {
            int start_vel = 0;

            if (event.key == elemd::KEY_LEFT)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _horizontal_input = -1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _horizontal_input = 0;
                }
            }
            else if (event.key == elemd::KEY_RIGHT)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _horizontal_input = 1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _horizontal_input = 0;
                }
            }

            if (event.key == elemd::KEY_UP)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _vertical_input = -1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _vertical_input = 0;
                }
            }
            else if (event.key == elemd::KEY_DOWN)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _vertical_input = 1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _vertical_input = 0;
                }
            }

            if (event.key == elemd::KEY_UP)
            {
                if (event.action == elemd::ACTION_PRESS)
                {
                    attack();
                }
            }
        });
    }

    void doInput(double dt);

private:
    int _horizontal_input = 0;
    int _vertical_input = 0;
    Level* _level = nullptr;

    elemd::Window* _win = nullptr;
};

#endif // PLAYER_H

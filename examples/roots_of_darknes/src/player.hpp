#ifndef PLAYER_H
#define PLAYER_H

#include <elemd/window.hpp>

#include "entity.hpp"

class Player : public Entity
{
public:
    const float PLAYER_SPEED = 1.2;
    bool _isLeftPresed = false;
    bool _isRightPresed = false;
    bool _isUpPresed = false;
    bool _isDownPresed = false;

    Player(elemd::Window* window, elemd::Image* img, elemd::vec2 spawn, Level* level)
        : Entity(window->get_context(), img, spawn), _win(window)
    {
        
        _level = level;
        
        window->add_key_listener([&](elemd::key_event event) {
            int start_vel = 0;

            if (event.key == elemd::KEY_LEFT || event.key == elemd::KEY_A)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _isLeftPresed = true;
                    _horizontal_input = -1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _isLeftPresed = false;
                    _horizontal_input = _isRightPresed ? 1 : 0;
                }
            }
            else if (event.key == elemd::KEY_RIGHT || event.key == elemd::KEY_D)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _isRightPresed = true;
                    _horizontal_input = 1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _isRightPresed = false;
                    _horizontal_input = _isLeftPresed ? -1 : 0;
                }
            }

            if (event.key == elemd::KEY_UP || event.key == elemd::KEY_W)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _isUpPresed = true;
                    _vertical_input = -1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _isUpPresed = false;
                    _vertical_input = _isDownPresed ? 1 : 0;
                }
            }
            else if (event.key == elemd::KEY_DOWN || event.key == elemd::KEY_S)
            {
                if (event.action == elemd::ACTION_PRESS || event.action == elemd::ACTION_REPEAT)
                {
                    _isDownPresed = true;
                    _vertical_input = 1;
                }
                else if (event.action == elemd::ACTION_RELEASE)
                {
                    _isDownPresed = false;
                    _vertical_input = _isUpPresed ? -1 : 0;
                }
            }

            /*
            if (event.key == elemd::KEY_UP)
            {
                if (event.action == elemd::ACTION_PRESS)
                {
                    attack();
                }
            }
             */
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

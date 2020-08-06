#ifndef ELEMD_EVENT_HPP
#define ELEMD_EVENT_HPP

#include "elemental_draw.hpp"

namespace elemd
{
    typedef enum mouse_button
    {
        MOUSE_BUTTON_1,
        MOUSE_BUTTON_2,
        MOUSE_BUTTON_3,
        MOUSE_BUTTON_4,
        MOUSE_BUTTON_5,
        MOUSE_BUTTON_6,
        MOUSE_BUTTON_7,
        MOUSE_BUTTON_8,
        MOUSE_BUTTON_LAST = MOUSE_BUTTON_8,
        MOUSE_BUTTON_LEFT = MOUSE_BUTTON_1,
        MOUSE_BUTTON_RIGHT = MOUSE_BUTTON_2,
        MOUSE_BUTTON_MIDDL = MOUSE_BUTTON_3,
        MOUSE_BUTTON_UNDEFINED
    } mouse_button;

    typedef enum mouse_action
    {
        MOUSE_ACTION_RELEASE,
        MOUSE_ACTION_PRESS,
        MOUSE_ACTION_REPEAT,
    } mouse_action;

    typedef enum mouse_mod
    {
        MOUSE_MOD_SHIFT = 0x0001,
        MOUSE_MOD_CONTROL = 0x0002,
        MOUSE_MOD_ALT = 0x0004,
        MOUSE_MOD_SUPER = 0x0008,
        MOUSE_MOD_CAPS_LOCK = 0x0010,
        MOUSE_MOD_NUM_LOCK = 0x0020,
    } mouse_mod;
   
    struct ELEMD_API mouse_move_event
    {
        double x;
        double y;
    };

    struct ELEMD_API mouse_button_event
    {
        mouse_button button;
        mouse_action action;
        mouse_mod mods;
        double x;
        double y;
    };

} // namespace elemd

#endif // ELEMD_EVENT_HPP
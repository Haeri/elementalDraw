#include "entity.hpp"

#include <elemd/window.hpp>
#include <cmath>

Entity::Entity(elemd::Context* ctx, elemd::Image* img)
{
    _ctx = ctx;
    _sprite = img;

    _pos.x() = _width + 20;
    _pos.y() = _height + 20;
}

int sign(float x)
{
    return (x < 0) ? -1 : (x > 0);
}


void Entity::simulate(double dt)
{
    // Accumulate Vertical velocuty
    _vel.y() = _vel.y() + _gravity * _mass * dt;
 
    // Apply velocity
    _pos = _pos + _vel * dt;

    groundCollision();

    // Accumulate Horizontal velocuty
    float friction = 0;
    if (_isGrounded)
    {
        friction = _vel.x() * -1 * _groundFriction * dt;
    }
    else
    {
        friction = _vel.x() * -1 * _airFriction * dt;
    }
    // friction
    _vel.x() = _vel.x() + friction;
}

void Entity::render(elemd::vec2 cam, double dt)
{
    float absVel = std::abs(_vel.x());    
    //int t = (int)(elemd::Window::now() * 100);
    int speedMod = (int)(10 - (absVel / 260.0f) * 8);
    //bool tik = t % 2 == 0;

    _accum += dt * 10 * (absVel / 260.0f);
    if (_accum > 1)
    {
        _tikTok = !_tikTok;
        _accum = 0;
    }

    //std::cout << _accum<< "\n";

    if (_lookDir)
    {
        if (!_moving || _tikTok)
        {
            _ctx->draw_image(_pos.get_x() - cam.get_x(), _pos.get_y() - cam.get_y(),
                            _width * SPRITE_SCALAT_POT,
                            _height * SPRITE_SCALAT_POT, _sprite, 0, 0,
                            24, 24);
        }
        else
        {
            _ctx->draw_image(_pos.get_x() - cam.get_x(), _pos.get_y() - cam.get_y(),
                            _width * SPRITE_SCALAT_POT,
                            _height * SPRITE_SCALAT_POT, _sprite, 24, 0,
                            24, 24);
        }
    }
    else
    {
        if (!_moving || _tikTok)
        {
         
        _ctx->draw_image(_pos.get_x() - cam.get_x(), _pos.get_y() - cam.get_y(),
                            _width * SPRITE_SCALAT_POT,
                            _height * SPRITE_SCALAT_POT, _sprite, 24, 0,
                            -24, 24);
        }
        else
        {
            _ctx->draw_image(_pos.get_x() - cam.get_x(), _pos.get_y() - cam.get_y(),
                            _width * SPRITE_SCALAT_POT,
                            _height * SPRITE_SCALAT_POT, _sprite, 48, 0,
                            -24, 24);
        }
    }

    // Text
    // ctx->set_font(monserat);
    //ctx->set_font_size(12);
    //ctx->set_fill_color({0, 255, 0, 255});
    //ctx->draw_text(_pos.get_x(), _pos.get_y() - 30, "Pos:" + _pos.to_string(1) + "\nVel: " + _vel.to_string(1));

    _moving = false;
}

void Entity::setLevelSpawn(Level* level, elemd::vec2 pos)
{
    _level = level;
    _pos = pos;
    _vel = elemd::vec2::ZERO;
}

void Entity::setPosition(elemd::vec2 pos)
{
    _pos = pos;
}

elemd::vec2 Entity::getPosition()
{
    return _pos;
}

void Entity::jump()
{
    if (!_isGrounded && _jumpcnt >= MAX_JMP)
    {
        return;
    }
    ++_jumpcnt;
    _vel.y() = -_jumpForce;
}

void Entity::groundCollision()
{
    /*
        tl  tm  tr
    lt  xy          rt
    lm              rm
    lb          wh  rb
        bl  bm  br
    */

    // Collision bottom
    elemd::vec2 bl = {_pos.x(), _pos.y() + _height};
    elemd::vec2 bm = {_pos.x() + _width / 2, _pos.y() + _height};
    elemd::vec2 br = {_pos.x() + _width - 1, _pos.y() + _height};

    bool b_bl =
        _level->getAt(bl.x() / _level->getTileSize(), bl.y() / _level->getTileSize()).collide;
    bool b_bm =
        _level->getAt(bm.x() / _level->getTileSize(), bm.y() / _level->getTileSize()).collide;
    bool b_br =
        _level->getAt(br.x() / _level->getTileSize(), br.y() / _level->getTileSize()).collide;

    _isGrounded = b_bl || b_bm || b_br;

    if (_isGrounded && _vel.y() > 0)
    {
        _pos.y() = (int)(bm.y() / _level->getTileSize()) * _level->getTileSize() - _height;
        _vel.y() = 0;
        _jumpcnt = 0;

        if (b_bl)
        {
            _ctx->set_fill_color({255, 0, 0, 255});
            _ctx->fill_rect(34, 34 + _height, 3, 3);
        }
        if (b_bm)
        {
            _ctx->set_fill_color({255, 0, 0, 255});
            _ctx->fill_rect((34 + _width / 2), 34 + _height, 3, 3);
        }
        if (b_br)
        {
            _ctx->set_fill_color({255, 0, 0, 255});
            _ctx->fill_rect((34 + _width), 34 + _height, 3, 3);
        }
    }

    // Collision top
    elemd::vec2 tl = {_pos.x(), _pos.y() - 1};
    elemd::vec2 tm = {_pos.x() + _width / 2, _pos.y() - 1};
    elemd::vec2 tr = {_pos.x() + _width - 1, _pos.y() - 1};

    bool b_tl =
        _level->getAt(tl.x() / _level->getTileSize(), tl.y() / _level->getTileSize()).collide;
    bool b_tm =
        _level->getAt(tm.x() / _level->getTileSize(), tm.y() / _level->getTileSize()).collide;
    bool b_tr =
        _level->getAt(tr.x() / _level->getTileSize(), tr.y() / _level->getTileSize()).collide;

    bool _collideTop = b_tl || b_tm || b_tr;

    if (_collideTop && _vel.y() < 0)
    {
        _pos.y() = (int)(tm.y() / _level->getTileSize()) * _level->getTileSize() + _level->getTileSize();
        _vel.y() = 0;
    }

        // Collidion right
    elemd::vec2 rt = {_pos.x() + _width, _pos.y()};
    elemd::vec2 rm = {_pos.x() + _width, _pos.y() + _height / 2};
    elemd::vec2 rb = {_pos.x() + _width, _pos.y() + _height - 1};

    bool b_rt =
        _level->getAt(rt.x() / _level->getTileSize(), rt.y() / _level->getTileSize()).collide;
    bool b_rm =
        _level->getAt(rm.x() / _level->getTileSize(), rm.y() / _level->getTileSize()).collide;
    bool b_rb =
        _level->getAt(rb.x() / _level->getTileSize(), rb.y() / _level->getTileSize()).collide;

    bool _collideRight = b_rt || b_rm || b_rb;

    if (_collideRight && _vel.x() > 0)
    {
        _pos.x() = ((int)(rm.x() / _level->getTileSize())) * _level->getTileSize() - _width;
        _vel.x() = 0;
    }

    // Collision left
    elemd::vec2 lt = {_pos.x() - 1, _pos.y()};
    elemd::vec2 lm = {_pos.x() - 1, _pos.y() + _height / 2};
    elemd::vec2 lb = {_pos.x() - 1, _pos.y() + _height - 1};

    bool b_lt =
        _level->getAt(lt.x() / _level->getTileSize(), lt.y() / _level->getTileSize()).collide;
    bool b_lm =
        _level->getAt(lm.x() / _level->getTileSize(), lm.y() / _level->getTileSize()).collide;
    bool b_lb =
        _level->getAt(lb.x() / _level->getTileSize(), lb.y() / _level->getTileSize()).collide;

    bool _collideLeft = b_lt || b_lm || b_lb;

    if (_collideLeft && _vel.x() < 0)
    {
        _pos.x() =
            (int)(lm.x() / _level->getTileSize()) * _level->getTileSize() + _level->getTileSize();
        _vel.x() = 0;
    }
}

void Entity::move(bool left, double dt)
{
    _moving = true;
    float factor = _isGrounded ? 1.0f : _airAuthorityFactor;
    
    _lookDir = left;
    _vel.x() = _vel.x() + (left ? -_speed : _speed) * factor * dt;
}

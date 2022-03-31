#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <elemd/vec2.hpp>
#include <elemd/image.hpp>
#include <elemd/context.hpp>

#include "physics.hpp"
#include "level.hpp"

#define SPRITE_SCALAT_POT 1


class Entity {
public:
    const int MAX_JMP = 2;

    Entity(elemd::Context* ctx, elemd::Image* img);
    void simulate(double dt);
    void render(elemd::vec2 cam, double dt);
    void setLevelSpawn(Level* level, elemd::vec2 pos);
    void setPosition(elemd::vec2 pos);
    elemd::vec2 getPosition();
    void move(bool left, double dt);
    void jump();

private :
    void groundCollision();

    Level* _level = nullptr;

    rect* rect = nullptr;

    float _width = 24;
    float _height = 24;
    elemd::vec2 _pos = elemd::vec2::ZERO;
    elemd::vec2 _vel = elemd::vec2::ZERO;
    elemd::Image* _sprite = nullptr;
   
    elemd::Context* _ctx = nullptr;

    int _jumpcnt = 0;
    bool _isGrounded = false;

    // Air Movement
    float _gravity = 9.1415f;
    float _jumpForce = 300;
    float _mass = 100;
    float _airFriction = 2.0f;
    float _airAuthorityFactor = 0.3f;

    // Ground Movement
    float _speed = 2020;
    float _groundFriction = 13.0f;
    bool _moving = false;

    // Sprite
    bool _lookDir = false;
    bool _tikTok;
    double _accum = 0;
};

#endif // ENTITY_HPP
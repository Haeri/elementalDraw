#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <elemd/context.hpp>
#include <elemd/image.hpp>
#include <elemd/vec2.hpp>

#include "level.hpp"

class Entity
{
public:
    Entity(elemd::Context* ctx, elemd::Image* img, elemd::vec2 spawn);

    void simulate(double dt);
    void render(elemd::vec2 cam, double dt);

    elemd::vec2 getPosition();

    void move(float horizontal, float vertical, double dt);
    void attack();

protected:
    elemd::Context* _ctx = nullptr;

    rect* _rect = nullptr;
    elemd::Image* _sprite = nullptr;
};

#endif // ENTITY_HPP

#include "entity.hpp"

#include <cmath>
#include <elemd/window.hpp>

Entity::Entity(elemd::Context* ctx, elemd::Image* img, elemd::vec2 spawn)
{
    _ctx = ctx;
    _sprite = img;

    _rect = new rect();
    _rect->pos.x() = spawn.get_x();
    _rect->pos.y() = spawn.get_y();
    _rect->size.x() = 24;
    _rect->size.y() = 24;
    
}

void Entity::simulate(double dt)
{

}

void Entity::render(elemd::vec2 cam, double dt)
{
    _ctx->draw_image(_rect->pos.get_x() - cam.get_x(), _rect->pos.get_y() - cam.get_y(),
                     _rect->size.get_x() * SPRITE_SCALAT_POT,
                     _rect->size.get_y() * SPRITE_SCALAT_POT, _sprite, 24,
                    0, 24, 24);
}

elemd::vec2 Entity::getPosition()
{
    return _rect->pos;
}

void Entity::move(float horizontal, float vertical, double dt)
{
    // _rect->pos.x() += horizontal * dt * 100;
    // _rect->pos.y() += vertical * dt * 100;
}

void Entity::attack()
{
}

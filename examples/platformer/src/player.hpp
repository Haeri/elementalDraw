#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.hpp"

class Player : public Entity
{
public:
    Player(elemd::Context* ctx, elemd::Image* img) : Entity(ctx, img)
    {
    }    

};

#endif // PLAYER_H
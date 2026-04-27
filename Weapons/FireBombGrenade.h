#pragma once
#include "Grenade.h"

class FireBombGrenade : public Grenade
{
public:
    FireBombGrenade() : Grenade(5) {}

    void throwGrenade(float x, float y, float angle, bool facingRight) override
    {
        // TODO:
        // explosion
        // spawn fire pool

        // Future:
        // FirePool class (radius 3 blocks, 10 sec duration)
    }
};
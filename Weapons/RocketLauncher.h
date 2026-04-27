#pragma once
#include "Weapon.h"

class RocketLauncher : public Weapon
{
public:
    RocketLauncher() : Weapon(0.5f, 5) {} // 2 sec reload

    void fire(float x, float y, float angle, bool facingRight) override
    {
        if (!canFire())
            return;

        timer = cooldown;

        // TODO:
        // spawn Rocket
        // explosion radius = 3 blocks
    }
};
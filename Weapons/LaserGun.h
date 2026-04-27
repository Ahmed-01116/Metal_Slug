#pragma once
#include "Weapon.h"

class LaserGun : public Weapon
{
public:
    LaserGun() : Weapon(1.0f, 0) {} // damage handled separately

    void fire(float x, float y, float angle, bool facingRight) override
    {
        if (!canFire())
            return;

        timer = cooldown;

        // TODO:
        // beam till end of screen
        // instant kill enemies
    }
};
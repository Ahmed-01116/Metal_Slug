#pragma once
#include "Weapon.h"

class Pistol : public Weapon
{
public:
    Pistol() : Weapon(5.0f, 3) {} // base stats

    void fire(float x, float y, float angle, bool facingRight) override
    {
        if (!canFire())
            return;

        timer = cooldown;

        // TODO:
        // spawn Bullet
        // infinite ammo
    }
};
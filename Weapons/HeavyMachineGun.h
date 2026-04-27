#pragma once
#include "Weapon.h"

class HeavyMachineGun : public Weapon
{
public:
    HeavyMachineGun() : Weapon(10.0f, 3) {} // double fire rate

    void fire(float x, float y, float angle, bool facingRight) override
    {
        if (!canFire())
            return;

        timer = cooldown;

        // TODO:
        // rapid fire bullets
    }
};
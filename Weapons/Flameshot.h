#pragma once
#include "Weapon.h"

class FlameShot : public Weapon
{
public:
    FlameShot() : Weapon(1.0f, 2) {} // DPS based

    void fire(float x, float y, float angle, bool facingRight) override
    {
        // continuous stream (no strict cooldown)

        // TODO:
        // damage = 2 HP per second
        // range = 5 blocks
        // instantly kill mummies
    }
};
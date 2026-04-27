#pragma once
#include "Grenade.h"

class HandGrenade : public Grenade
{
public:
    HandGrenade() : Grenade(5) {}

    void throwGrenade(float x, float y, float angle, bool facingRight) override
    {
        // TODO:
        // arc motion
        // explosion radius = 3 blocks
    }
};
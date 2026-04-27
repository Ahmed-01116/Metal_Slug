#pragma once

class Grenade
{
protected:
    int damage;

public:
    Grenade(int dmg)
    {
        damage = dmg;
    }

    virtual void throwGrenade(float x, float y, float angle, bool facingRight) = 0;

    virtual ~Grenade() {}
};
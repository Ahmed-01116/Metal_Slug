#pragma once

class Weapon
{
protected:
    float fireRate;
    float cooldown;
    float timer;
    int damage;

public:
    Weapon(float rate, int dmg)
    {
        fireRate = rate;
        cooldown = 1.0f / fireRate;
        timer = 0;
        damage = dmg;
    }

    virtual void update(float dt)
    {
        if (timer > 0)
            timer -= dt;
    }

    bool canFire() const
    {
        return timer <= 0;
    }

    virtual void fire(float x, float y, float angle, bool facingRight) = 0;

    virtual ~Weapon() {}
};
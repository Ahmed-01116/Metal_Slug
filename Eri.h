#pragma once
#include "Player.h"

class Eri : public Player
{
public:
    bool doubleGrenadeActive;
    float doubleGrenadeTimer;

    Eri(float startX, float startY, int pw, int ph) : Player(startX, startY, "Sprites/eri.png", pw, ph)
    {
        // PDF ke mutabiq:
        // Double grenades
        grenades = 20; // base 10 * 2 = 20

        // 20% lower fire rate — weapon mein handle hoga

        // No melee attack — melee function override

        doubleGrenadeActive = false;
        doubleGrenadeTimer = 0;
    }

    // Special power up — double grenade throw 10 seconds
    void activatePowerUp()
    {
        doubleGrenadeActive = true;
        doubleGrenadeTimer = 10.0f;
    }

    // Eri ka melee nahi hai
    bool canMelee()
    {
        return false;
    }

    void updateTimers(float dt) override
    {
        Player::updateTimers(dt);

        if (doubleGrenadeActive)
        {
            doubleGrenadeTimer -= dt;
            if (doubleGrenadeTimer <= 0)
            {
                doubleGrenadeActive = false;
                doubleGrenadeTimer = 0;
            }
        }
    }

    void draw(RenderWindow &window, float offsetX) override
    {
        Player::draw(window, offsetX);
    }
};
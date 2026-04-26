#pragma once
#include "Player.h"

class Fiolina : public Player
{
public:
    bool superchargedActive;
    float superchargedTimer;

    Fiolina(float startX, float startY, int pw, int ph) : Player(startX, startY, "Sprites/fiolina.png", pw, ph)
    {
        // PDF ke mutabiq:
        // 2 kam grenades
        grenades = 8; // base 10 - 2 = 8

        // 10% higher fire rate — weapon mein
        // 25% less melee damage — melee mein
        // 50% more ammo on pickup — weapon mein

        superchargedActive = false;
        superchargedTimer = 0;
    }

    // Special power up — 100% fire rate 10 seconds
    void activatePowerUp()
    {
        superchargedActive = true;
        superchargedTimer = 10.0f;
    }

    void updateTimers(float dt) override
    {
        Player::updateTimers(dt);

        if (superchargedActive)
        {
            superchargedTimer -= dt;
            if (superchargedTimer <= 0)
            {
                superchargedActive = false;
                superchargedTimer = 0;
            }
        }
    }

    void draw(RenderWindow &window, float offsetX) override
    {
        Player::draw(window, offsetX);
    }
};
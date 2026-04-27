#pragma once
#include "Player.h"

class Marco : public Player
{
private:
    bool dualFireActive;
    float dualFireTimer;

public:
    Marco(float startX, float startY, int pw, int ph) : Player(startX, startY, "Sprites/marco_rossi.png", pw, ph)
    {
        // 25% higher fire rate — weapon system mein handle hoga
        // 2 kam grenades
        grenades = 8; // base 10 - 2 = 8

        dualFireActive = false;
        dualFireTimer = 0;
    }

    // Special power up — dual fire 10 seconds
    void activatePowerUp()
    {
        dualFireActive = true;
        dualFireTimer = 10.0f;
    }

    void updateTimers(float dt) override
    {
        // Base class timers
        Player::updateTimers(dt);

        // Dual fire timer
        if (dualFireActive)
        {
            dualFireTimer -= dt;
            if (dualFireTimer <= 0)
            {
                dualFireActive = false;
                dualFireTimer = 0;
            }
        }
    }

    // Melee pierce — shielded enemies ke liye
    // ShieldedEnemy class mein handle hoga
    bool canPierceShield()
    {
        return true;
    }
    bool getDualFireActive() const
    {
        return dualFireActive;
    }

    void draw(RenderWindow &window, float offsetX) override
    {
        Player::draw(window, offsetX);
    }
};
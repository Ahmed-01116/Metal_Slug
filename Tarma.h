#pragma once
#include "Player.h"

class Tarma : public Player
{
public:
    bool immunityActive;
    float immunityTimer;
    bool inVehicle;

    Tarma(float startX, float startY, int pw, int ph) : Player(startX, startY, "Sprites/tarma.png", pw, ph)
    {
        // 20% slower on foot
        maxSpeed = maxSpeed * 0.8f; // 5 * 0.8 = 4

        // 20% less HP on foot
        // (HP system weapon ke baad aayega)

        immunityActive = false;
        immunityTimer = 0;
        inVehicle = false;

        // Vehicle buffs
        // 25% higher vehicle fire rate — vehicle class mein
        // 20% higher vehicle durability — vehicle class mein
    }

    // Special power up — immunity 20 seconds
    void activatePowerUp()
    {
        immunityActive = true;
        immunityTimer = 20.0f;
    }

    // Vehicle destroy hone pe Tarma bachta hai
    void onVehicleDestroyed()
    {
        // Tarma survives — kuch nahi hota usse
        inVehicle = false;
    }

    void takeDamage() override
    {
        // Immunity active hai to damage nahi lagta
        if (immunityActive)
            return;

        Player::takeDamage();
    }

    void updateTimers(float dt) override
    {
        Player::updateTimers(dt);

        if (immunityActive)
        {
            immunityTimer -= dt;
            if (immunityTimer <= 0)
            {
                immunityActive = false;
                immunityTimer = 0;
            }
        }
    }

    void draw(RenderWindow &window, float offsetX) override
    {
        Player::draw(window, offsetX);
    }
};
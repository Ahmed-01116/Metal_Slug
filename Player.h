#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Player
{
public:
    // Position
    float x, y;
    float velocityX, velocityY;

    // Stats
    float maxSpeed, acceleration, gravity;
    int lives;
    int grenades;
    bool isGrounded, facingRight;

    // Size
    float scaleX, scaleY;
    int Pwidth, Pheight;

    // Damage State
    // 0 = NORMAL, 1 = INJURED, 2 = CRITICAL, 3 = DEAD
    int damageState;
    float damageTimer;

    // Character State
    // 0 = NORMAL, 1 = UNDEAD, 2 = MUMMY
    int charState;
    float stateTimer;

    // Sprite
    Texture texture;
    Sprite sprite;

    Player(float startX, float startY, const char *spritePath, int pw, int ph, float sX = 4.0f, float sY = 4.0f)
    {
        x = startX;
        y = startY;

        velocityX = 0;
        velocityY = 0;
        maxSpeed = 5.0f;
        acceleration = 0.5f;
        gravity = 0.5f;

        lives = 2;
        grenades = 10;

        isGrounded = false;
        facingRight = true;

        damageState = 0;
        damageTimer = 0;
        charState = 0;
        stateTimer = 0;

        scaleX = sX;
        scaleY = sY;
        Pwidth = pw * scaleX;
        Pheight = ph * scaleY;

        texture.loadFromFile(spritePath);
        sprite.setTexture(texture);
        sprite.setScale(scaleX, scaleY);
    }

    void handleInput()
    {
        // Mummy state mein movement band
        if (charState == 2)
        {
            velocityX = 0;
            return;
        }

        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            velocityX += acceleration;
            if (velocityX > maxSpeed)
                velocityX = maxSpeed;
            sprite.setScale(scaleX, scaleY);
            sprite.setOrigin(0, 0);
            facingRight = true;
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            velocityX -= acceleration;
            if (velocityX < -maxSpeed)
                velocityX = -maxSpeed;
            sprite.setScale(-scaleX, scaleY);
            sprite.setOrigin(29, 0);
            facingRight = false;
        }
        else
        {
            velocityX = 0;
        }
    }

    void applyPhysics()
    {
        // Undead state mein 50% slow
        if (charState == 1)
            x += velocityX * 0.5f;
        else
            x += velocityX;

        velocityY += gravity;
        y += velocityY;
    }

    void jump()
    {
        if (isGrounded)
            velocityY = -12.0f;
    }

    // Damage system — 3 hit
    virtual void takeDamage()
    {
        if (damageState != 0)
            return;

        if (damageState == 0) // NORMAL -> INJURED
        {
            damageState = 1;
            damageTimer = 1.0f;
        }
        else if (damageState == 1) // INJURED -> CRITICAL
        {
            damageState = 2;
            damageTimer = 1.0f;
        }
        else if (damageState == 2) // CRITICAL -> DEAD
        {
            damageState = 3;
            lives--;
        }
    }

    virtual void updateTimers(float dt)
    {
        // Damage timer
        if (damageState == 1 || damageState == 2)
        {
            damageTimer -= dt;
            if (damageTimer <= 0)
            {
                damageState = 0; // Wapas normal
                damageTimer = 0;
            }
        }

        // Character state timer
        if (charState == 1 || charState == 2)
        {
            stateTimer -= dt;
            if (stateTimer <= 0)
            {
                charState = 0;
                stateTimer = 0;
            }
        }
    }

    void setUndead()
    {
        charState = 1;
        stateTimer = 10.0f; // 10 seconds
    }

    void setMummy()
    {
        charState = 2;
        stateTimer = 10.0f; // 10 seconds
    }

    virtual void draw(RenderWindow &window, float offsetX)
    {
        sprite.setPosition(x - offsetX, y);
        window.draw(sprite);
    }

    virtual ~Player() {}
};
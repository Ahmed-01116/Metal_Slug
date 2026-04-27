#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Player
{
protected:
    // Core data
    float x, y;
    float velocityX, velocityY;

    float maxSpeed, acceleration, gravity;
    int lives;
    int grenades;

    bool isGrounded, facingRight;

    float scaleX, scaleY;
    int Pwidth, Pheight;

    int damageState;
    float damageTimer;

    int charState;
    float stateTimer;

    Texture texture;
    Sprite sprite;

public:
    Player(float startX, float startY, const char *spritePath, int pw, int ph, float sX = 4.5f, float sY = 4.5f)
    {
        x = startX;
        y = startY;

        velocityX = velocityY = 0;

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
            sprite.setOrigin(Pwidth / scaleX, 0);
            facingRight = false;
        }
        else
        {
            velocityX = 0;
        }
    }

    void applyPhysics()
    {
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
            velocityY = -15.0f;
    }

    virtual void takeDamage()
    {
        if (damageState == 0)
            damageState = 1;
        else if (damageState == 1)
            damageState = 2;
        else if (damageState == 2)
        {
            damageState = 3;
            lives--;
        }

        damageTimer = 1.0f;
    }

    virtual void updateTimers(float dt)
    {
        if (damageState == 1 || damageState == 2)
        {
            damageTimer -= dt;
            if (damageTimer <= 0)
            {
                damageState = 0;
                damageTimer = 0;
            }
        }

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
        stateTimer = 10.0f;
    }

    void setMummy()
    {
        charState = 2;
        stateTimer = 10.0f;
    }

    float getX() const { return x; }
    float getY() const { return y; }

    float getVelocityX() const { return velocityX; }
    float getVelocityY() const { return velocityY; }

    int getWidth() const { return Pwidth; }
    int getHeight() const { return Pheight; }

    bool getIsGrounded() const { return isGrounded; }

    void setIsGrounded(bool b) { isGrounded = b; }

    void setX(float v) { x = v; }
    void setY(float v) { y = v; }
    void setVelocityY(float v) { velocityY = v; }
    void setVelocityX(float v) { velocityX = v; }

    virtual void draw(RenderWindow &window, float offsetX)
    {
        sprite.setPosition(x - offsetX, y);
        window.draw(sprite);
    }

    virtual ~Player() {}
};
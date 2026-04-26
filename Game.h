#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Marco.h"
#include "Tarma.h"
#include "Eri.h"
#include "Fiolina.h"
#include "Enemy.h"

using namespace sf;

class Game
{
private:
    RenderWindow window;

    int screenX = 1920;
    int screenY = 1080;

    const int cellSize = 64;
    const int height = 14;
    const int width = 110;

    char **lvl;
    Texture wallTex;
    Sprite wallSprite;

    Player *players[4];
    int currentActivePlayer;
    Player *active;

    Enemy *enemies[5];
    int enemyCount;

    float offsetX;
    Clock clock;

public:
    Game()
    {
        window.create(VideoMode(screenX, screenY), "Metal Slug");
        window.setFramerateLimit(60);

        enemyCount = 0;

        setupLevel();
        setupPlayers();
        setupEnemies();

        currentActivePlayer = 0;
        active = players[currentActivePlayer];
    }

    ~Game()
    {
        for (int i = 0; i < height; i++)
            delete[] lvl[i];
        delete[] lvl;

        for (int i = 0; i < 4; i++)
            delete players[i];

        for (int i = 0; i < enemyCount; i++)
            delete enemies[i];
    }

    void run()
    {
        Event ev;
        while (window.isOpen())
        {
            handleEvents(ev);
            update();
            render();
        }
    }

private:
    void setupLevel()
    {
        lvl = new char *[height];
        for (int i = 0; i < height; i++)
            lvl[i] = new char[width]{'\0'};

        for (int j = 0; j < width; j++)
            lvl[13][j] = 'g';

        wallTex.loadFromFile("Sprites/blocks/grass_block_side.png");
        wallSprite.setTexture(wallTex);
    }

    void setupPlayers()
    {
        players[0] = new Marco(380, 610, 29, 37);
        players[1] = new Tarma(380, 610, 32, 38);
        players[2] = new Eri(380, 610, 29, 39);
        players[3] = new Fiolina(380, 610, 28, 35);
    }

    void setupEnemies()
    {
        enemyCount = 0;
        // TODO: enemy classes later
    }

    void handleEvents(Event &ev)
    {
        while (window.pollEvent(ev))
        {
            if (ev.type == Event::Closed)
                window.close();

            if (ev.type == Event::KeyPressed)
            {
                if (ev.key.code == Keyboard::Up)
                    active->jump();

                if (ev.key.code == Keyboard::Z)
                {
                    currentActivePlayer = (currentActivePlayer + 1) % 4;
                    active = players[currentActivePlayer];
                }
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();
    }

    void update()
    {
        float dt = clock.restart().asSeconds();

        active->handleInput();
        active->applyPhysics();
        active->updateTimers(dt);

        resolveCollision();

        offsetX = active->getX() - screenX / 2;

        if (offsetX < 0)
            offsetX = 0;
        float maxOffset = (width * cellSize) - screenX;
        if (offsetX > maxOffset)
            offsetX = maxOffset;
    }

    void resolveCollision()
    {
        active->setIsGrounded(false);

        float px = active->getX();
        float py = active->getY();

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (lvl[i][j] != 'g')
                    continue;

                float tileX = j * cellSize;
                float tileY = i * cellSize;

                float pRight = px + active->getWidth();
                float pBottom = py + active->getHeight();

                float tRight = tileX + cellSize;
                float tBottom = tileY + cellSize;

                bool overlap = pRight > tileX &&
                               px < tRight &&
                               pBottom > tileY &&
                               py < tBottom;

                if (overlap)
                {
                    if (active->getVelocityY() > 0 && py + active->getHeight() - active->getVelocityY() <= tileY)
                    {
                        active->setY(tileY - active->getHeight());
                        active->setVelocityY(0);
                        active->setIsGrounded(true);
                    }
                    else if (active->getVelocityY() < 0 && py - active->getVelocityY() >= tileY + cellSize)
                    {
                        active->setY(tileY + cellSize);
                        active->setVelocityY(0);
                    }
                    else if (active->getVelocityX() > 0)
                        active->setX(tileX - active->getWidth());
                        
                    else if (active->getVelocityX() < 0)
                        active->setX(tileX + cellSize);
                }
            }
        }
    }

    void render()
    {
        window.clear();

        displayLevel();
        active->draw(window, offsetX);

        window.display();
    }

    void displayLevel()
    {
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
            {
                if (lvl[i][j] == 'g')
                {
                    wallSprite.setPosition(j * cellSize - offsetX, i * cellSize);
                    window.draw(wallSprite);
                }
            }
    }
};
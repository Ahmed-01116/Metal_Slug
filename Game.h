#pragma once
#include <SFML/Graphics.hpp>

#include "Players/Player.h"
#include "Players/Marco.h"
#include "Players/Tarma.h"
#include "Players/Eri.h"
#include "Players/Fio.h"

#include "Enemies/EnemyFactory.h"

using namespace sf;

class Game
{
private:
    RenderWindow window;

    int screenX, screenY;

    const int cellSize = 64;
    const int height = 14;
    const int width = 110;

    char **lvl;
    Texture wallTex;
    Sprite wallSprite;

    Player *players[4];
    int currentActivePlayer;
    Player *active;

    EnemySpawnFactory spawnFactory;

    float offsetX;
    Clock clock;

public:
    Game()
    {
        screenX = 1920;
        screenY = 1080;

        window.create(VideoMode(screenX, screenY), "Metal Slug");
        window.setFramerateLimit(60);

        setupLevel();
        setupPlayers();
        setupEnemies();

        currentActivePlayer = 0;
        active = players[currentActivePlayer];

        offsetX = 0;
    }

    ~Game()
    {
        for (int i = 0; i < height; i++)
            delete[] lvl[i];
        delete[] lvl;

        for (int i = 0; i < 4; i++)
            delete players[i];
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
        int marco_w = 29, marco_h = 37;
        players[0] = new Marco(380, 610, marco_w, marco_h);

        int tarma_w = 32, tarma_h = 38;
        players[1] = new Tarma(380, 610, tarma_w, tarma_h);

        int eri_w = 29, eri_h = 39;
        players[2] = new Eri(380, 610, eri_w, eri_h);

        int fio_w = 28, fio_h = 35;
        players[3] = new Fiolina(380, 610, fio_w, fio_h);
    }

    void setupEnemies()
    {
        spawnFactory.spawnLevelEnemies(1);
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

        spawnFactory.updateAll(dt, active->getX(), active->getY());

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

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (lvl[i][j] != 'g')
                    continue;

                float tileX = j * cellSize;
                float tileY = i * cellSize;

                float px = active->getX();
                float py = active->getY();

                float pRight = px + active->getWidth();
                float pBottom = py + active->getHeight();
                float tRight = tileX + cellSize;
                float tBottom = tileY + cellSize;

                bool overlap = pRight > tileX && px < tRight && pBottom > tileY && py < tBottom;

                if (overlap)
                {
                    if (active->getVelocityY() > 0 && pBottom - active->getVelocityY() <= tileY)
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
        // Enemies draw (temporary — jab Enemy mein draw aayega)
        Enemy **enemies = spawnFactory.getEnemies();
        int count = spawnFactory.getEnemyCount();
        for (int i = 0; i < count; i++)
        {
            if (enemies[i] && enemies[i]->getIsAlive())
                enemies[i]->draw(window, offsetX);
        }

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
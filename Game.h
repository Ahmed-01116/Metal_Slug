#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Marco.h"
#include "Tarma.h"
#include "Eri.h"
#include "Fiolina.h"

using namespace sf;

class Game
{
private:
    RenderWindow window;
    int screenX, screenY;

    // Level
    const int cellSize = 64;
    const int height = 14;
    const int width = 110;

    char **lvl;
    Texture wallTex;
    Sprite wallSprite;

    // Player
    Player *players[4];
    int currentActivePlayer;
    Player *active;

    // Manual camera
    float offsetX;

    Clock clock;

public:
    Game()
    {
        screenX = 1920;
        screenY = 1080;

        window.create(VideoMode(screenX, screenY), "Metal Slug", Style::Default);
        window.setFramerateLimit(60);

        setupLevel();
        setupPlayers();

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
        players[1] = new Tarma(380, 610, tarma_w, tarma_w);

        int eri_w = 29, eri_h = 39;
        players[2] = new Eri(380, 610, eri_w, eri_h);
        
        int fio_w = 28, fio_h = 35;
        players[3] = new Fiolina(380, 610, fio_w, fio_h);
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

        offsetX = active->x - screenX / 2;
        if (offsetX < 0)
            offsetX = 0;
        float maxOffset = (width * cellSize) - screenX;
        if (offsetX > maxOffset)
            offsetX = maxOffset;
    }

    void resolveCollision()
    {
        active->isGrounded = false;

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                if (lvl[i][j] == 'g')
                {
                    float tileX = j * cellSize;
                    float tileY = i * cellSize;

                    float pLeft = active->x;
                    float pRight = active->x + active->Pwidth;
                    float pTop = active->y;
                    float pBottom = active->y + active->Pheight;

                    float tLeft = tileX;
                    float tRight = tileX + cellSize;
                    float tTop = tileY;
                    float tBottom = tileY + cellSize;

                    bool overlap = pRight > tLeft &&
                                   pLeft < tRight &&
                                   pBottom > tTop &&
                                   pTop < tBottom;

                    if (overlap)
                    {
                        if (active->velocityY > 0 && pBottom - active->velocityY <= tTop)
                        {
                            active->y = tTop - active->Pheight;
                            active->velocityY = 0;
                            active->isGrounded = true;
                        }
                        else if (active->velocityY < 0 && pTop - active->velocityY >= tBottom)
                        {
                            active->y = tBottom;
                            active->velocityY = 0;
                        }
                        else if (active->velocityX > 0)
                            active->x = tLeft - active->Pwidth;
                        else if (active->velocityX < 0)
                            active->x = tRight;
                    }
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
        {
            for (int j = 0; j < width; j++)
            {
                if (lvl[i][j] == 'g')
                {
                    float drawX = j * cellSize - offsetX;
                    float drawY = i * cellSize;

                    wallSprite.setPosition(drawX, drawY);
                    window.draw(wallSprite);
                }
            }
        }
    }
};
#pragma once
#include "Enemy.h"
#include "InfantryEnemy.h"

class AerialEnemy : public Enemy
{
protected:
    float hieght;
    bool landed;

public:
    AerialEnemy(float startX, float startY, int startHp, float moveSpeed, int enemyType, int score,
                int batchMin, int batchMax)

        : Enemy(startX, startY, startHp, moveSpeed,
                enemyType, biome_Aerial, score, batchMin, batchMax),
          hieght(startY),
          landed(false)
    {
    }
};
#pragma once
// Enemy States
const int state_Idle = 0;
const int state_Patrolling = 1;
const int state_Chasing = 2;
const int state_Attacking = 3;
const int state_Dead = 4;
const int state_resurrecting = 5;

// Biomes
const int biome_Aerial = 0;
const int biome_plains = 1;
const int biome_Aquatic = 2;

// Types of enemies
const int type_Rebel_Soldier = 0;
const int type_Shielded_Soldier = 1;
const int type_Bazooka_soldier = 2;
const int type_Grenade_Soldier = 3;
const int type_Paratrooper = 4;
const int type_Mummy_warrior = 5;
const int type_Zombie = 6;
const int type_Martian = 7;
const int type_FlyingTara = 8;
const int type_M15ABradley = 9;
const int type_EnemySub = 10;

// Weapon Types
const int weapon_Bullet = 0;
const int weapon_Fire = 1;
const int weapon_Explosion = 2;
const int weapon_Melee = 3;

// Directions
const int dir_Above = 0;
const int dir_Below = 1;
const int dir_Left = 2;
const int dir_Right = 3;

class Enemy
{
protected:
    float x, y;
    int Hp, maxHp;
    float speed;
    int Biome;
    int state;
    int type;
    bool isAlive;
    int ScoreVal;
    int minBatch;
    int maxBatch;

public:
    Enemy(float startX, float startY, int startHp, float moveSpeed, int enemyType, int enemyBiome, int score, int batchMin, int batchMax)
        : x(startX), y(startY), Hp(startHp), maxHp(startHp), speed(moveSpeed), type(enemyType), Biome(enemyBiome), state(state_Idle), isAlive(true), ScoreVal(score), minBatch(batchMin), maxBatch(batchMax) {}

    virtual ~Enemy() {}

    virtual void update(float TimeChange) = 0;
    virtual void attack() = 0;
    virtual void move(float TimeChange) = 0;
    virtual Enemy *create(float x, float y, int biome) = 0;

    virtual void takeDamage(int amount, int weapontype, int hitDirection)
    {
        if (!isAlive)
        {
            return;
        }

        Hp -= amount;
        if (Hp <= 0)
        {
            Hp = 0;
            die();
        }
    }

    virtual void die()
    {
        isAlive = false;
        state = state_Dead;
    }

    // Getters
    float getX() const
    {
        return x;
    }
    float getY() const
    {
        return y;
    }
    int getHp() const
    {
        return Hp;
    }
    int getMaxHp() const
    {
        return maxHp;
    }
    int getType() const
    {
        return type;
    }
    int getBiome() const
    {
        return Biome;
    }
    int getState() const
    {
        return state;
    }
    bool getIsAlive() const
    {
        return isAlive;
    }
    int getScoreValue() const
    {
        return ScoreVal;
    }
    int getMinBatch() const
    {
        return minBatch;
    }
    int getMaxBatch() const
    {
        return maxBatch;
    }

    // Setters
    void setPosition(float newX, float newY)
    {
        x = newX;
        y = newY;
    }
    void setState(int newState)
    {
        state = newState;
    }
};
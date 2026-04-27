#pragma once
#include "Enemy.h"

class VehicleEnemy : public Enemy
{
protected:
    int vehicleHp;
    float weaponCooldown;

public:
    VehicleEnemy(float startX, float startY, int startHp, float moveSpeed, int enemyType, int enemyBiome, int score)
        : Enemy(startX, startY, startHp, moveSpeed, enemyType, enemyBiome, score, 1, 1),
          vehicleHp(startHp), weaponCooldown(0.0f) {}

    virtual ~VehicleEnemy() {}

    // Functions implemented by child
    virtual void move(float TimeChange) = 0;
    virtual void attack() = 0;
    virtual void update(float TimeChange) = 0;
    virtual Enemy *create(float x, float y, int biome) = 0;

    int getVehicleHp() const
    {
        return vehicleHp;
    }
};

class FlyingTara : public VehicleEnemy
{
private:
    float hoverHeight;
    bool isAbovePlayer;
    float grenadeCooldown;
    float grenadeCooldownMax;

public:
    // Comstructor
    FlyingTara(float startX, float startY)
        : VehicleEnemy(startX, startY, 5, 1.2f, type_FlyingTara, biome_Aerial, 150),
          hoverHeight(10.0f), isAbovePlayer(false), grenadeCooldown(0.0f), grenadeCooldownMax(2.0f)
    {
        minBatch = 2;
        maxBatch = 2;
    }
    // Deconstructor
    virtual ~FlyingTara() {}

    // movement function
    void move(float TimeChange) override
    { // Move horizontally until above player then hover
        if (!isAbovePlayer)
        {
            state = state_Patrolling;
        }
        else
        {
            state = state_Attacking;
        }
    }

    Enemy *create(float x, float y, int biome) override
    {
        return new FlyingTara(x, y);
    }
    // attack function
    void attack() override
    {
        if (!isAbovePlayer || grenadeCooldown > 0.0f)
        {
            return;
        }

        // Drop grenade straight down
        grenadeCooldown = grenadeCooldownMax;
        state = state_Attacking;
    }
    // update the positiona nd teh movement

    void update(float TimeChange) override
    {
        if (!isAlive)
            return;

        if (grenadeCooldown > 0.0f)
        {
            grenadeCooldown -= TimeChange;
        }

        move(TimeChange);
        attack();
    }
    // getters
    void setIsAbovePlayer(bool above)
    {
        isAbovePlayer = above;
    }
    float getHoverHeight() const
    {
        return hoverHeight;
    }
};

class M15ABradley : public VehicleEnemy
{
private:
    bool isStationary;
    float missileArcAngle;
    float missileCooldown;
    float missileCooldownMax;

public:
    // Constructor
    M15ABradley(float startX, float startY)
        : VehicleEnemy(startX, startY, 7, 0.3f, type_M15ABradley, biome_plains, 300),
          isStationary(true), missileArcAngle(55.0f), missileCooldown(0.0f), missileCooldownMax(3.0f) {}

    // Deconstructor
    virtual ~M15ABradley() {}
    // Movement function
    void move(float TimeChange) override
    {
        if (!isStationary)
        { // Moving horizontally on flat surface toward player
            state = state_Chasing;
        }
    }
    Enemy *create(float x, float y, int biome) override
    {
        return new M15ABradley(x, y);
    }
    // Attack function to launch misssiles
    void attack() override
    {
        if (missileCooldown > 0.0f)
        {
            return;
        }

        // Launch missile in projectile
        missileCooldown = missileCooldownMax;
        state = state_Attacking;
    }
    // Update teh movement and position
    void update(float TimeChange) override
    {
        if (!isAlive)
        {
            return;
        }

        if (missileCooldown > 0.0f)
        {
            missileCooldown -= TimeChange;
        }
        move(TimeChange);
        attack();
    }
    // Setters and getters
    void setIsStationary(bool val)
    {
        isStationary = val;
    }
    bool getIsStationary() const
    {
        return isStationary;
    }
};

class EnemySub : public VehicleEnemy
{
private:
    bool isSubmerged;
    float rocketCooldown;
    float rocketCooldownMax;

public:
    // COnstructor
    EnemySub(float startX, float startY)
        : VehicleEnemy(startX, startY, 7, 0.8f, type_EnemySub, biome_Aquatic, 300),
          isSubmerged(true), rocketCooldown(0.0f), rocketCooldownMax(2.5f) {}
    // Deconstructor
    virtual ~EnemySub() {}
    // movement function changes state
    void move(float TimeChange) override
    { // Patrol horizontally underwater
        state = state_Patrolling;
    }
    Enemy *create(float x, float y, int biome) override
    {
        return new EnemySub(x, y);
    }
    // Attacck and state change
    void attack() override
    {
        if (rocketCooldown > 0.0f)
        {
            return;
        }
        // Fire rocket toward player
        rocketCooldown = rocketCooldownMax;
        state = state_Attacking;
    }
    // update hte movement and position
    void update(float TimeChange) override
    {
        if (!isAlive)
        {
            return;
        }

        if (rocketCooldown > 0.0f)
        {
            rocketCooldown -= TimeChange;
        }
        move(TimeChange);
        attack();
    }
    // getters
    bool getIsSubmerged() const
    {
        return isSubmerged;
    }
};
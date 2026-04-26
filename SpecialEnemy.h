#pragma once
#include "Enemy.h"
#include "InfantryEnemy.h"
#include <cstdlib>

class AerialEnemy : public Enemy
{
protected:
    float height;
    bool landed;

public:
    AerialEnemy(float startX, float startY, int startHp, float moveSpeed, int enemyType, int score, int batchMin, int batchMax)
        : Enemy(startX, startY, startHp, moveSpeed, enemyType, biome_Aerial, score, batchMin, batchMax),
          height(startY), landed(false) {}

    virtual ~AerialEnemy() {}
    // functions to eb implemented by child
    virtual void move(float TimeChange) = 0;
    virtual void attack() = 0;
    virtual void update(float TimeChange) = 0;
    virtual Enemy *create(float x, float y, int biome) = 0;
    // getters
    float getHeight() const
    {
        return height;
    }
    bool getHasLanded() const
    {
        return landed;
    }
};

class Paratrooper : public AerialEnemy
{
private:
    InfantryEnemy *innerEnemy;
    float downSpeed;

public:
    // Constructor
    Paratrooper(float startX, float startY, InfantryEnemy *soldier)
        : AerialEnemy(startX, startY, soldier->getHp(), 0.5f, type_Paratrooper, soldier->getScoreValue() + 25, soldier->getMinBatch(), soldier->getMaxBatch()),
          innerEnemy(soldier), downSpeed(0.8f) {}
    // Deconstructor to clean up
    virtual ~Paratrooper()
    {
        delete innerEnemy;
        innerEnemy = nullptr;
    }
    Enemy *create(float x, float y, int biome) override
    {
        int pick = rand() % 4;
        InfantryEnemy *inner = nullptr;

        if (pick == 0)
        {
            inner = new RebelSoldier(x, y, biome_Aerial);
        }
        else if (pick == 1)
        {
            inner = new ShieldedSoldier(x, y, biome_Aerial);
        }
        else if (pick == 2)
        {
            inner = new BazookaSoldier(x, y, biome_Aerial);
        }
        else
        {
            inner = new GrenadeSoldier(x, y, biome_Aerial);
        }
        return new Paratrooper(x, y, inner);
    }
    void move(float TimeChange) override
    {
        if (!landed)
        {
            // Coming Down to ground
            height -= downSpeed * TimeChange;
            y = height;

            if (height <= 0.0f)
            { // ground level on ground
                landed = true;
                state = state_Patrolling;
            }
        }

        else
        {
            // movement of the infantry soldier
            if (innerEnemy)
            {
                innerEnemy->move(TimeChange);
            }
        }
    }

    void attack() override
    {
        if (landed && innerEnemy)
        { // Enemy on ground and is soldier then start to attack
            innerEnemy->attack();
        }
    }
    // update hte movement
    void update(float TimeChange) override
    {
        if (!isAlive)
        {
            return;
        }
        move(TimeChange);

        if (landed)
        {
            attack();
        }
    }

    // the Enemy that has landed takes the damage
    void takeDamage(int amount, int weaponType, int hitDirection) override
    {
        if (!isAlive)
        {
            return;
        }
        Hp -= amount;

        if (innerEnemy)
        { // inner enemy taking damage
            innerEnemy->takeDamage(amount, weaponType, hitDirection);
        }
        if (Hp <= 0)
        { // if hp less or equal to 0 then die
            Hp = 0;
            die();
        }
    }
    // getters
    InfantryEnemy *getInnerEnemy() const
    {
        return innerEnemy;
    }
};

class UndeadEnemy : public Enemy
{
protected:
    bool DeadFireOrExplosion;

public:
    // Constructor
    UndeadEnemy(float startX, float startY, int startHp, float moveSpeed, int enemyType, int score, int batchMin, int batchMax)
        : Enemy(startX, startY, startHp, moveSpeed, enemyType, biome_plains, score, batchMin, batchMax),
          DeadFireOrExplosion(false) {}
    // deconstructor
    virtual ~UndeadEnemy() {}
    // functions implemented by child
    virtual Enemy *create(float x, float y, int biome) = 0;
    virtual void move(float TimeChange) = 0;
    virtual void attack() = 0;
    virtual void update(float TimeChange) = 0;
};

class MummyWarrior : public UndeadEnemy
{
private:
    bool isCrumbled;
    float resurrectionTimer;
    float resurrectionCooldown; // 3 seconds

public:
    // Constructor
    MummyWarrior(float startX, float startY)
        : UndeadEnemy(startX, startY, 5, 0.4f, type_Mummy_warrior, 150, 1, 1),
          isCrumbled(false), resurrectionTimer(0.0f), resurrectionCooldown(3.0f)
    {
        DeadFireOrExplosion = true;
    }
    // Deconstructor
    virtual ~MummyWarrior() {}

    // explosion and fire kill it and bullets crumble it
    void takeDamage(int amount, int weaponType, int hitDirection) override
    {
        if (!isAlive || isCrumbled)
        {
            return;
        }

        if (weaponType == weapon_Fire || weaponType == weapon_Explosion)
        { // Permanently killed
            Hp = 0;
            die();
        }
        else
        { // Crumble — will resurrect
            isCrumbled = true;
            resurrectionTimer = resurrectionCooldown;
            state = state_resurrecting;
        }
    }
    Enemy *create(float x, float y, int biome) override
    {
        return new MummyWarrior(x, y);
    }

    void move(float TimeChange) override
    {
        if (!isAlive || isCrumbled)
        {
            return;
        }
        // Slow walks toward player
        state = state_Patrolling;
    }

    void attack() override
    {
        // Transforms player on collision — handled by collision system
        // No ranged attack
    }
    // updates the movement and respawn
    void update(float TimeChange) override
    {
        if (!isAlive)
        {
            return;
        }

        if (isCrumbled)
        {
            resurrectionTimer -= TimeChange;
            if (resurrectionTimer <= 0.0f)
            {
                isCrumbled = false;
                state = state_Patrolling;
            }
            return;
        }
        move(TimeChange);
    }
    // getters
    bool getIsCrumbled() const
    {
        return isCrumbled;
    }
    float getResurrectionTimer() const
    {
        return resurrectionTimer;
    }
};

class Zombie : public UndeadEnemy
{
private:
    bool hasPistol;
    float fireCooldown;
    float fireRate;

public:
    // Constructor
    Zombie(float startX, float startY, int enemyBiome)
        : UndeadEnemy(startX, startY, 5, 0.4f, type_Zombie, 100, 3, 5),
          hasPistol(true), fireCooldown(0.0f), fireRate(1.0f)
    {
        Biome = enemyBiome;
    }
    // Deconstructor for deletion
    virtual ~Zombie() {}

    void move(float TimeChange) override
    {
        if (!isAlive)
        {
            return;
        }
        // Slow walk toward player
        state = state_Patrolling;
    }
    Enemy *create(float x, float y, int biome) override
    {
        return new Zombie(x, y, biome);
    }

    void attack() override
    {
        if (fireCooldown > 0.0f)
        {
            return;
        }
        // Fire pistol + infect on collision
        state = state_Attacking;
        fireCooldown = 1.0f / fireRate;
    }
    // update movement and position and cause attack
    void update(float TimeChange) override
    {
        if (!isAlive)
        {
            return;
        }

        if (fireCooldown > 0.0f)
        {
            fireCooldown -= TimeChange;
        }
        move(TimeChange);
        attack();
    }

    bool getHasPistol() const { return hasPistol; }
};

class AlienEnemy : public Enemy
{
public:
    // Constructor
    AlienEnemy(float startX, float startY, int startHp, float moveSpeed, int enemyType, int enemyBiome, int score, int batchMin, int batchMax)
        : Enemy(startX, startY, startHp, moveSpeed, enemyType, enemyBiome, score, batchMin, batchMax) {}

    // Deconstructor
    virtual ~AlienEnemy() {}

    // Functions
    virtual Enemy *create(float x, float y, int biome) = 0;
    virtual void move(float TimeChange) = 0;
    virtual void attack() = 0;
    virtual void update(float TimeChange) = 0;
};

class Martian : public AlienEnemy
{
private:
    int currentPhase; // 1 or 2
    bool podisAlive;
    int podHp;
    float beamCooldown;
    float pistolCooldown;

public:
    Martian(float startX, float startY)
        : AlienEnemy(startX, startY, 3, 0.8f, type_Martian, biome_Aerial, 200, 1, 1),
          currentPhase(1), podisAlive(true), podHp(3), beamCooldown(0.0f), pistolCooldown(0.0f) {}

    virtual ~Martian() {}

    // takeDamage to handle phase switching
    void takeDamage(int amount, int weaponType, int hitDirection) override
    {
        if (!isAlive)
        {
            return;
        }

        if (currentPhase == 1)
        {
            podHp -= amount;
            if (podHp <= 0)
            {
                podHp = 0;
                podisAlive = false;
                switchToPhase2();
            }
        }
        else
        {
            // Phase 2 damage the martian directly
            Hp -= amount;
            if (Hp <= 0)
            {
                Hp = 0;
                die();
            }
        }
    }
    Enemy *create(float x, float y, int biome) override
    {
        return new Martian(x, y);
    }

    void switchToPhase2()
    { // Martian drops to ground level and uses pistol
        currentPhase = 2;
        state = state_Patrolling;
    }

    void move(float TimeChange) override
    {
        if (!isAlive)
        {
            return;
        }

        if (currentPhase == 1)
        {
            // Hover above player and position above them
        }
        else
        {
            // Walk on foot toward player
        }
    }

    void attack() override
    {
        if (!isAlive)
        {
            return;
        }

        if (currentPhase == 1 && beamCooldown <= 0.0f)
        { // Fire energy beam downward when above player
            beamCooldown = 1.5f;
            state = state_Attacking;
        }
        else if (currentPhase == 2 && pistolCooldown <= 0.0f)
        { // Fire pistol horizontally
            pistolCooldown = 1.0f;
            state = state_Attacking;
        }
    }

    void update(float TimeChange) override
    {
        if (!isAlive)
            return;

        if (beamCooldown > 0.0f)
            beamCooldown -= TimeChange;
        if (pistolCooldown > 0.0f)
            pistolCooldown -= TimeChange;

        move(TimeChange);
        attack();
    }

    int getCurrentPhase() const
    {
        return currentPhase;
    }
    bool getPodAlive() const
    {
        return podisAlive;
    }
};
#pragma once
#include <cstdlib>
#include "Enemy.h"
#include "InfantryEnemy.h"
#include "otherEnemy.h"
#include "EnemyVehicle.h"

const int MaximumEnemies   = 300;
const int MaximumEnemyTypes = 11;


class EnemySpawnFactory
{
private:

    Enemy* enemies[MaximumEnemies];
    int    enemyCount;
    Enemy* prototypes[MaximumEnemyTypes];

    void addEnemy(Enemy* e){
        if (e == nullptr) {
            return;
        }

        if (enemyCount < MaximumEnemies){ // if enemies is less then maximum
            enemies[enemyCount] = e;
            enemyCount++;
        }
        else{ // Array if full
            delete e;   
        }
    }
    //random number generator
    int randomInRange(int minVal, int maxVal) const{

        return minVal + (rand() % (maxVal - minVal + 1));

    }
    // spawn batch of enemies
    void spawnBatch(int enemyType, float startX, float startY, int biome, int minBatch, int maxBatch){
        int   batchSize = randomInRange(minBatch, maxBatch);
        float spacing   = 2.0f * 64.0f;   

        for (int i = 0; i < batchSize; i++){
            Enemy* e = createEnemy(enemyType, startX + (i * spacing), startY, biome);
            addEnemy(e);
        }
    }

    // infentry batches 
    void spawnInfantryBatches(int batchCount){
        int types[4] = { type_Rebel_Soldier, type_Shielded_Soldier, type_Bazooka_soldier, type_Grenade_Soldier };

        //distribute across all 3 biomes
        float biomeX[3] = { 20.0f  * 64,  80.0f  * 64, 150.0f * 64};

        float groundY = 13.0f * 64;   // ground row matches partner's lvl[13]

        for (int t = 0; t < 4; t++){
            for (int b = 0; b < batchCount; b++){
                int   biome = b % 3;
                float bx    = biomeX[biome] + (b * 10.0f * 64);
                spawnBatch(types[t], bx, groundY, biome, 2, 4);
            }
        }
    }

    void spawnAerialAndAlienBatches(int batchCount){
        float startX  = 20.0f * 64;
        float aerialY =  2.0f * 64;

        for (int b = 0; b < batchCount; b++){
            float bx = startX + (b * 15.0f * 64);
            spawnBatch(type_Paratrooper, bx,               aerialY, biome_Aerial, 2, 4);
            spawnBatch(type_Martian,     bx + (5.0f * 64), aerialY, biome_Aerial, 1, 1);
        }
    }

    void spawnUndeadBatches(int batchCount){
        float startX  = 80.0f * 64;
        float groundY = 13.0f * 64;

        for (int b = 0; b < batchCount; b++){
            float bx = startX + (b * 12.0f * 64);
            spawnBatch(type_Mummy_warrior, bx, groundY, biome_plains, 1, 1);
        }
    }

    void spawnZombieBatches(int batchCount){
        float startX   = 150.0f * 64;
        float aquaticY =  13.0f * 64;

        for (int b = 0; b < batchCount; b++){
            float bx = startX + (b * 10.0f * 64);
            spawnBatch(type_Zombie, bx, aquaticY, biome_Aquatic, 3, 5);
        }
    }

    void spawnFlyingTaraBatches(int batchCount){
        float startX  = 30.0f * 64;
        float aerialY =  5.0f * 64;

        for (int b = 0; b < batchCount; b++){
            float bx = startX + (b * 30.0f * 64);
            spawnBatch(type_FlyingTara, bx, aerialY, biome_Aerial, 2, 2);
        }
    }

    void spawnEnemySubs(int count){
        float startX   = 150.0f * 64;
        float aquaticY =  10.0f * 64;

        for (int i = 0; i < count; i++){
            Enemy* e = createEnemy(type_EnemySub, startX + (i * 20.0f * 64), aquaticY,  biome_Aquatic);
            addEnemy(e);
        }
    }

    void spawnBradleys(int count){
        float startX  = 60.0f * 64;
        float groundY = 13.0f * 64;

        for (int i = 0; i < count; i++){
            Enemy* e = createEnemy(type_M15ABradley, startX + (i * 30.0f * 64), groundY,  biome_plains);
            addEnemy(e);
        }
    }

public:


    EnemySpawnFactory()
    {
        enemyCount = 0;

        for (int i = 0; i < MaximumEnemies;    i++) enemies   [i] = nullptr;
        for (int i = 0; i < MaximumEnemyTypes; i++) prototypes[i] = nullptr;

        // making prototypes for each enemy type
        prototypes[type_Rebel_Soldier]    = new RebelSoldier   (0, 0, biome_plains);
        prototypes[type_Shielded_Soldier] = new ShieldedSoldier(0, 0, biome_plains);
        prototypes[type_Bazooka_soldier]  = new BazookaSoldier (0, 0, biome_plains);
        prototypes[type_Grenade_Soldier]  = new GrenadeSoldier (0, 0, biome_plains);
        prototypes[type_Paratrooper]      = new Paratrooper    (0, 0, new RebelSoldier(0, 0, biome_Aerial));
        prototypes[type_Mummy_warrior]    = new MummyWarrior   (0, 0);
        prototypes[type_Zombie]           = new Zombie         (0, 0, biome_Aquatic);
        prototypes[type_Martian]          = new Martian        (0, 0);
        prototypes[type_FlyingTara]      = new FlyingTara     (0, 0);
        prototypes[type_M15ABradley]     = new M15ABradley    (0, 0);
        prototypes[type_EnemySub]        = new EnemySub       (0, 0);
    }

    //Deconstructor
    ~EnemySpawnFactory(){
        clearAll();

        for (int i = 0; i < MaximumEnemyTypes; i++) {
            delete prototypes[i];
            prototypes[i] = nullptr;
        }
    }

    Enemy* createEnemy(int enemyType, float x, float y, int biome)
    {
        if (prototypes[enemyType] == nullptr) return nullptr;

        return prototypes[enemyType]->create(x, y, biome);
    }

    void spawnLevelEnemies(int levelNumber)
    {
        clearAll();

        if (levelNumber == 1)
        {
            spawnInfantryBatches(2);
            spawnAerialAndAlienBatches(1);
            spawnUndeadBatches(2);
            spawnZombieBatches(2);
            spawnFlyingTaraBatches(2);
            spawnEnemySubs(1);
        }
        else if (levelNumber == 2)
        {
            spawnInfantryBatches(3);
            spawnAerialAndAlienBatches(2);
            spawnUndeadBatches(3);
            spawnZombieBatches(3);
            spawnFlyingTaraBatches(3);
            spawnEnemySubs(1);
            spawnBradleys(2);
        }
        else if (levelNumber == 3)
        {
            spawnInfantryBatches(3);
            spawnAerialAndAlienBatches(3);
            spawnUndeadBatches(4);
            spawnZombieBatches(4);
            spawnFlyingTaraBatches(3);
            spawnEnemySubs(2);
            spawnBradleys(2);
        }
    }
    //update all enemies
    void updateAll(float dt, float playerX, float playerY){
        for (int i = 0; i < enemyCount; i++){
            if (enemies[i] && enemies[i]->getIsAlive())
                enemies[i]->update(dt);
        }
    }

    //delete alll enemies
    void clearAll(){
        for (int i = 0; i < enemyCount; i++){
            delete enemies[i];
            enemies[i] = nullptr;
        }
        enemyCount = 0;
    }

    //Getters
    Enemy** getEnemies() { 
        return enemies;
    }
    int getEnemyCount() const { 
        return enemyCount;
    }
};
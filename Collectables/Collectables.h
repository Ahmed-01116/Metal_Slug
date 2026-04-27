#pragma once
#include <cstdlib>  
#include "Enemy.h"
//Collectables constants
const int collectable_Turkey      = 0;
const int collectable_Fruit       = 1;
const int collectable_POWPrisoner = 2;
const int collectable_SupplyCrate = 3;
 
// constant for the weapons inside hte crates
const int crate_Weapon_HMG            = 0;
const int crate_Weapon_FlameShot      = 1;
const int crate_Weapon_RocketLauncher = 2;
const int crate_Weapon_LaserGun       = 3;
 
// Parent class for the collectables
class Collectable {
    protected:
        float x, y;
        bool  isActive;
        int   type;
    
    public:
        Collectable(float startX, float startY, int collectableType): x(startX), y(startY), isActive(true), type(collectableType) {}
    
        virtual ~Collectable() {}
    
        // Player type needed to know which player picked it this is needed for the 
        //powerups and conversion
        virtual void onPickup(int playerType) = 0;
    
        // Check if player is close enough to collect
        virtual bool isInRange(float playerX, float playerY) const {
            float dx = playerX - x;
            float dy = playerY - y;
    
            // Within 2 blocks in each direction
            return (dx * dx + dy * dy) < (2.0f * 64.0f) * (2.0f * 64.0f);
        }
    
        // Getters
        float getX() const {
             return x;
        }
        float getY() const {
             return y; 
        }
        bool  getIsActive() const {
             return isActive;
        }
        int   getType() const { 
             return type;
        }
        // Setter
        void setIsActive(bool val) { 
             isActive = val; 
        }
};
    
// Child Classes
class Food : public Collectable {
    protected:
        int saturationValue;    // saturation = fatness
    
    public:
        Food(float startX, float startY, int collectableType, int saturation) : Collectable(startX, startY, collectableType), saturationValue(saturation) {}
    
        virtual ~Food() {}
    
        // Food pickup just restores saturation
        void onPickup(int playerType) override{
            if (!isActive) {
                return;
            }
            
            isActive = false;
            
        }
    
        int getSaturationValue() const { 
            return saturationValue;
        }
};
 
 
// Child class
class Turkey : public Food {
    public:
        Turkey(float startX, float startY): Food(startX, startY, collectable_Turkey, 3) {}
    
        ~Turkey() override {}
};
 
 
// child class
 
class Fruit : public Food {
    public:
        Fruit(float startX, float startY)  : Food(startX, startY, collectable_Fruit, 2) {}
    
        ~Fruit() override {}
};
 
 
// child class
 
class POWPrisoner : public Collectable {
    private:
        bool  crateSpawned;
        float interactRange;   
    
    public:
        POWPrisoner(float startX, float startY)  : Collectable(startX, startY, collectable_POWPrisoner), crateSpawned(false), interactRange(2.0f * 64.0f) {}
    
        ~POWPrisoner() override {}
    
        void onPickup(int playerType) override {
            if (!isActive || crateSpawned) {
                return;
            }
    
            crateSpawned = true;
            isActive     = false;
            // crate spawned and game class will spawn it
        }
    
        bool  getCrateSpawned() const { 
            return crateSpawned; 
        }
        float getInteractRange() const { 
            return interactRange;
        }
};
 
// child class
 
class SupplyCrate : public Collectable {
    private:
        int  weaponInside;        
        int  ammoInside;           
        int  grenadeCount;         
        bool containsFireBombs;     
        bool playerPickedWeapon;  
        // 90% cahnce of getting the HMg Flame Shot ot rocket launcher
        void generateContents() {
            int roll = rand() % 10;     
    
            if (roll < 9) {

                int weaponRoll = rand() % 3;
                if(weaponRoll == 0){
                    weaponInside = crate_Weapon_HMG;
                }
                else if(weaponRoll == 1) {
                    weaponInside = crate_Weapon_FlameShot;
                }
                else {
                    weaponInside = crate_Weapon_RocketLauncher;
                }
    
                ammoInside = 30; 
            }
            else { // 105 chance of getting laser gun
                weaponInside = crate_Weapon_LaserGun;
                ammoInside   = 10;
            }
            // 50% cahnce of getting fire bombes
            int grenadeRoll = rand() % 2;
            if (grenadeRoll == 0){
                containsFireBombs = false;
                grenadeCount = 3 + (rand() % 3);   // 3-5
            }
            else{
                containsFireBombs = true;
                grenadeCount = 1 + (rand() % 3);   // 1-3
            }
        }
    
    public:
        SupplyCrate(float startX, float startY) : Collectable(startX, startY, collectable_SupplyCrate), weaponInside(crate_Weapon_HMG),
        ammoInside(0),  grenadeCount(0), containsFireBombs(false), playerPickedWeapon(false){
            generateContents();
        }
    
        ~SupplyCrate() override {}
    
        void onPickup(int playerType) override{
            if (!isActive) return;
    
        // constatn for eri
            if (playerType == 2){
                containsFireBombs = true;
            }
    
            isActive = true;    // stays active until weapon decision is made
            
        }
    
        // Player chose to take the weapon
        void pickupWeapon(){
            playerPickedWeapon = true;
            isActive             = false;
        }
    
        // Player ignored the weapon refill ammo
        void ignoreWeapon(){
            playerPickedWeapon = false;
            isActive             = false;
            // ammo given to player
        }
    
        // Getters
        int  getWeaponInside() const { 
            return weaponInside;
        }
        int  getAmmoInside() const { 
            return ammoInside;
        }
        int  getGrenadeCount() const { 
            return grenadeCount;
        }
        bool getContainsFireBombs() const { 
            return containsFireBombs;
        }
        bool getPlayerPickedUpWeapon() const {
            return playerPickedWeapon;
        }
};
 
 
// Manager of collectables
const int MaxCollectables = 100;
 
class CollectableManager {
    private:
        Collectable* collectables[MaxCollectables];
        int collectableCount;
        // Checking limit
        void addCollectable(Collectable* c) {
            if (c == nullptr) return;
    
            if (collectableCount < MaxCollectables){
                collectables[collectableCount] = c;
                collectableCount++;
            }
            else{
                delete c;
            }
        }
    
    public:
    
        CollectableManager(){
            collectableCount = 0;
            for (int i = 0; i < MaxCollectables; i++)
                collectables[i] = nullptr;
        }
    
        ~CollectableManager(){
            clearAll();
        }
    
        // Called when an enemy dies
        void trySpawnFood(float x, float y){
            int roll = rand() % 5;      // 20% chance
    
            if (roll == 0){
                int foodRoll = rand() % 2;
                if (foodRoll == 0){
                    addCollectable(new Turkey(x, y));
                }
                else{
                    addCollectable(new Fruit(x, y));
                }
            }
        }
    
        // when POWPrisoner is released
        void spawnSupplyCrate(float x, float y){
            addCollectable(new SupplyCrate(x, y));
        }
    
        // level setup for fixed POW positions
        void spawnPOWPrisoner(float x, float y){
            addCollectable(new POWPrisoner(x, y));
        }

        void checkPickups(float playerX, float playerY, int playerType)
        {
            for (int i = 0; i < collectableCount; i++)
            {
                if (collectables[i] == nullptr){
                        continue;
                }
                if (!collectables[i]->getIsActive()) {
                        continue;
                }
                if (collectables[i]->isInRange(playerX, playerY)){
                    collectables[i]->onPickup(playerType);
    
                    // If it was a POW prisoner draw a crate
                    if (collectables[i]->getType() == collectable_POWPrisoner){
                        POWPrisoner* pow = (POWPrisoner*)collectables[i];
                        if (pow->getCrateSpawned()){
                            spawnSupplyCrate(pow->getX(), pow->getY());
                        }
                    }
                }
            }
        }
    
        void clearAll(){
            for (int i = 0; i < collectableCount; i++){
                delete collectables[i];
                collectables[i] = nullptr;
            }
            collectableCount = 0;
        }
    
        // Getters
        Collectable** getCollectables() {
            return collectables;    
        }
        int getCollectableCount()  { 
            return collectableCount;
       }
};
 

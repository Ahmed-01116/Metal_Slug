#pragma once
#include"Enemy.h"

class InfantryEnemy : public Enemy {
    protected:
        int WeaponType;        
        float attackRange;
        float FireCooldown;
        float FireRate;
    public:
        InfantryEnemy(float startX, float startY,  int   startHp, float moveSpeed, int   enemyType,  int   enemyBiome, int   score, int   batchMin, int batchMax, int weaponType, float attackRange, float fireRate)
        : Enemy(startX, startY, startHp, moveSpeed, enemyType, enemyBiome, score, batchMin, batchMax), WeaponType(weaponType), attackRange(attackRange), FireCooldown(0.0f), FireRate(fireRate){}

        virtual ~InfantryEnemy(){}
        virtual void update(float TimeChange) override = 0;
        virtual void attack() override = 0;
        virtual void move(float TimeChange) override {
            state = state_Patrolling;
        }

        float getFireRate() const{
            return FireRate;
        }
        float getAttackRange() const{
            return attackRange;
        }
};

class RebelSoldier : public InfantryEnemy {
    public:
        RebelSoldier(float startX , float StartY , int enemyBiome)
        : InfantryEnemy(startX , StartY , 2 , 1.0f , type_Rebel_Soldier , enemyBiome , 50 , 2 , 4 , weapon_Bullet , 1.0f , 5.0f) {} 

        virtual ~RebelSoldier() {}

        void attack() override {
           state = state_Attacking;
        }
        
        void update(float TimeChange) override {
            if(!isAlive){
                return;
            }

            if(FireCooldown > 0.0f){
                FireCooldown -= TimeChange;
            }

            if(state == state_Attacking){
                attack();
            }
            else{
                move(TimeChange);
            }
      }
};

class ShieldedSoldier : public InfantryEnemy {
    private:
        bool ActiveShield;
        int shieldDirection;
    public: 
        ShieldedSoldier(float startX, float startY, int enemyBiome)
        : InfantryEnemy(startX, startY , 5, 0.8f , type_Shielded_Soldier, enemyBiome, 75, 1, 2, weapon_Bullet, 1.0f, 5.0f),
          ActiveShield(true), shieldDirection(dir_Left){}

        virtual ~ShieldedSoldier() {}

        bool canbeHit(int hitDirection , int weaponType) const {

            if(weaponType == weapon_Explosion || weaponType == weapon_Fire){
                return true;
            }

            if(hitDirection != shieldDirection){
                return true;
            }

            return false;
        }


        void takeDamage(int amount, int weaponType, int hitDirection) override {
            if (!isAlive) return;
    
            if (!canbeHit(hitDirection, weaponType))
                return;     
    
            Hp -= amount;

            if (Hp <= 0){
                Hp = 0;
                die();
            }
        }

        void attack() override {
            state = state_Attacking;
        }

        void update(float TimeChange) override {
            if(!isAlive){
                return;
            }

            if(FireCooldown > 0.0f){
                FireCooldown -= TimeChange;
            }

            if(state == state_Attacking){
                attack();
            }
            else{
                move(TimeChange);
            }
        }

        bool getShieldActive() const {
            return ActiveShield;
        }
};

class BazookaSoldier : public InfantryEnemy { 
    private:
        float reloadTime;
        float reloadcooldown;
        bool  isReloading;
        float rocketAngle;
    public:
        BazookaSoldier(float startX, float startY, int enemyBiome)
        : InfantryEnemy(startX, startY, 2 ,  0.5f, type_Bazooka_soldier, enemyBiome, 100 , 1, 2, weapon_Explosion , 0.5f ,8.0f), 
          reloadTime(0.0f), reloadcooldown(2.0f), isReloading(false), rocketAngle(60.0f) {}

          virtual ~BazookaSoldier() {}

        void attack () override {
            if(isReloading){
                return;
            }
            state = state_Attacking;
            isReloading = true;
            reloadTime = reloadcooldown;
        }

        void update(float TimeChange) override {
            if(!isAlive){
                return;
            }
            if(isReloading){
                reloadTime -= TimeChange;
                if(reloadTime <= 0.0f){
                    isReloading = false;
                    reloadTime = 0.0f;
                }
            }

            if(state == state_Attacking){
                attack();
            }
            else{
                move(TimeChange);

            }
        }

        bool getIsReloading() const{
            return isReloading;
        }

        float getReloadTime() const{
            return reloadTime;
        }

        float getRocketAngle() const{
            return rocketAngle;
        }
};

class GrenadeSoldier : public InfantryEnemy {
    private: 
        float throwAngle;
        float throwForce;
        float throwCoolDown;
        float throwTime;
    public:
        GrenadeSoldier(float startX, float startY, int enemyBiome)
        : InfantryEnemy(startX, startY, 2, 0.8f, type_Grenade_Soldier, enemyBiome, 100, 1, 2, weapon_Explosion, 0.8f, 7.0f),
          throwAngle(45.0f), throwForce(10.0f), throwCoolDown(1.5f), throwTime(0.0f){}

        virtual ~GrenadeSoldier() {}

        void attack() override {
            if(throwTime > 0.0f){
                return;
            }
            state = state_Attacking;
            throwTime = throwCoolDown;
        }

        void update(float TimeChange) override {
            if(!isAlive){
                return;
            }

            if(throwTime > 0.0f){
                throwTime -= TimeChange;
            }

            if(state == state_Attacking){
                attack();
            }
            else{
                move(TimeChange);
            }
        } 

        float getThrowAngle() const{
            return throwAngle;
        }
        float getThrowForce() const{
            return throwForce;
        }
};
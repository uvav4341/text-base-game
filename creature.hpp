#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "inventory.hpp"
#include "weapon.hpp"

#include <string>
#include <sstream>

class Creature
{
public:
    std::string name;
    std::string className;

    int health;
    int maxHealth;
    int str;
    int end;
    int dex;
    double hitRate;

    unsigned int level;
    unsigned int exp;

    Inventory inventory;
    Weapon* equippedWeapon;

    Creature(std::string name, int health, int str, int end, int dex, double hitRate,
             unsigned int level = 1, std::string className = "")
    {
        this->name = name;
        this->health = health;
        this->maxHealth = health;
        this->str = str;
        this->end = end;
        this->dex = dex;
        this->hitRate = hitRate;
        this->className = className;

        this->equippedWeapon = nullptr;
        this->level = level;
        this->exp = 0;
    }

    Creature()
    {
        this->equippedWeapon = nullptr;
        this->level = 1;
        this->exp = 0;
    }

    void equipWeapon(Weapon* weapon)
    {
        this->equippedWeapon = weapon;
    }

    unsigned int expToLevel(unsigned int level)
    {
        return 128 * level * level;
    }

    // Level-up now returns bool
    bool levelUp()
    {
        if (this->exp >= expToLevel(this->level + 1))
        {
            ++level;

            unsigned int healthBoost = (level % 3 == 0) ? 10 + (rand() % 4) + this->end / 4 : this->end / 4;
            unsigned int strBoost = 0, endBoost = 0, dexBoost = 0;

            if (this->className == "Fighter") {
                strBoost = 1;
                endBoost = 1;
                if (rand() % 2 == 0) dexBoost = 1;
            } else if (this->className == "Rogue") {
                endBoost = 1;
                dexBoost = 1;
                if (rand() % 2 == 0) strBoost = 1;
            }

            this->maxHealth += healthBoost;
            this->str += strBoost;
            this->end += endBoost;
            this->dex += dexBoost;

            return true; // Leveled up
        }

        return false; // Did not level up
    }
};

#endif /* CREATURE_HPP */

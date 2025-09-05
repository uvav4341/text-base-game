#ifndef BATTLE_HPP
#define BATTLE_HPP

#include "dialogue.hpp"
#include "creature.hpp"
#include "weapon.hpp"

#include <string>
#include <sstream>
#include <vector>

class Battle
{
public:
    Dialogue dialogue;
    Creature* creatures[2];

    Battle() {}

    Battle(Creature* player, Creature* b) {
        this->creatures[0] = player;
        this->creatures[1] = b;

        this->dialogue = Dialogue("What will you do?", {
            "Attack",
            "Defend"
        });
    }

    // Attack action: return description instead of printing
    std::string attack(Creature* a, Creature* b) {
        std::ostringstream out;
        out << a->name << " attacks!\n";

        int damage = 0;
        double hitRate = a->hitRate;

        if (a->equippedWeapon != nullptr) {
            damage += a->equippedWeapon->damage;
            hitRate += a->equippedWeapon->hitRate;
        }

        damage += a->str / 2;

        int defense = 0; // (armour not yet implemented)
        damage -= defense;
        if (damage < 1) damage = 1;

        if (rand() % 201 <= 170 + hitRate - b->dex) {
            out << b->name << " takes " << damage << " damage!\n";
            b->health -= damage;
        } else {
            out << a->name << " missed!\n";
        }

        return out.str();
    }

    // Player’s turn
    std::string playerTurn(int choice) {
        std::ostringstream out;
        if (choice == 1) {
            out << attack(creatures[0], creatures[1]);
        } else if (choice == 2) {
            out << creatures[0]->name << " defends!\n";
        }
        return out.str();
    }

    // Enemy turn
    std::string enemyTurn() {
        return attack(creatures[1], creatures[0]);
    }

    bool isdead(Creature* creature) {
        return (creature->health <= 0);
    }

    // Run one round of combat
    std::string activate(int playerChoice) {
        std::ostringstream out;

        if (creatures[0]->dex >= creatures[1]->dex) {
            out << playerTurn(playerChoice);
            if (isdead(creatures[1])) {
                out << creatures[1]->name << " was vanquished!\n";
                return out.str();
            }

            out << enemyTurn();
            if (isdead(creatures[0])) {
                out << creatures[0]->name << " was vanquished!\n";
                return out.str();
            }
        } else {
            out << enemyTurn();
            if (isdead(creatures[0])) {
                out << creatures[0]->name << " was vanquished!\n";
                return out.str();
            }

            out << playerTurn(playerChoice);
            if (isdead(creatures[1])) {
                out << creatures[1]->name << " was vanquished!\n";
                return out.str();
            }
        }

        return out.str();
    }

    // Full battle loop
    std::string run(std::function<int()> getPlayerChoice) {
        std::ostringstream out;
        out << creatures[1]->name << " appears!\n";

        while (true) {
            int choice = getPlayerChoice(); // external input
            out << activate(choice);

            if (isdead(creatures[0]) || isdead(creatures[1])) break;
        }

        if (isdead(creatures[1])) {
            unsigned int expGain = creatures[1]->expToLevel(creatures[1]->level+1) / 8;
            out << "Gained " << expGain << " exp!\n";
            creatures[0]->exp += expGain;
            while (creatures[0]->levelUp());
        }

        return out.str();
    }
};

#endif /* BATTLE_HPP */

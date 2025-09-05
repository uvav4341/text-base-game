#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "item.hpp"
#include <string>

class Weapon : public Item
{
public:
    unsigned damage;   // reasonable: 1–50
    double hitRate;    // reasonable: 5–30%

    Weapon(std::string name, std::string description, int damage, double hitRate)
        : Item(name, description)
    {
        this->damage = damage;
        this->hitRate = hitRate;
    }

    Weapon() {}

    // ---- Safe text builder ----
    std::string asText() const
    {
        return name + " (DMG: " + std::to_string(damage) +
               ", Hit: " + std::to_string(hitRate) + "%) - " + description;
    }

    // ---- JSON-ready minimal form ----
    std::string asJson() const
    {
        return "{ \"name\": \"" + name +
               "\", \"description\": \"" + description +
               "\", \"damage\": " + std::to_string(damage) +
               ", \"hitRate\": " + std::to_string(hitRate) + " }";
    }
};

#endif /* WEAPON_HPP */

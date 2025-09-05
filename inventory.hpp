#ifndef INVENTORY_HPP
#define INVENTORY_HPP

#include "item.hpp"
#include "weapon.hpp"

#include <list>
#include <utility>
#include <string>

class Inventory
{
public:
    std::list<std::pair<Item*, int>> items;
    std::list<std::pair<Weapon*, int>> weapons;

    Inventory() {}

    Inventory(std::list<std::pair<Item*, int>> items,
              std::list<std::pair<Weapon*, int>> weapons)
    {
        this->items = items;
        this->weapons = weapons;
    }

    void clear()
    {
        this->items.clear();
        this->weapons.clear();
    }

    void add_item(Item* item, int count)
    {
        for (auto& it : this->items)
        {
            if (it.first == item)
            {
                it.second += count;
                return;
            }
        }
        this->items.push_back(std::make_pair(item, count));
    }

    void add_weapon(Weapon* weapon, int count)
    {
        for (auto& it : this->weapons)
        {
            if (it.first == weapon)
            {
                it.second += count;
                return;
            }
        }
        this->weapons.push_back(std::make_pair(weapon, count));
    }

    void remove_item(Item* item, int count)
    {
        for (auto& it : this->items)
        {
            if (it.first == item) it.second -= count;
        }
        this->items.remove_if([](std::pair<Item*, int>& element)
        {
            return element.second < 1;
        });
    }

    void remove_weapon(Weapon* weapon, int count)
    {
        for (auto& it : this->weapons)
        {
            if (it.first == weapon) it.second -= count;
        }
        this->weapons.remove_if([](std::pair<Weapon*, int>& element)
        {
            return element.second < 1;
        });
    }

    void merge(Inventory* inventory)
    {
        if (inventory == this) return;

        for (auto it : inventory->items)
        {
            this->add_item(it.first, it.second);
        }
        for (auto it : inventory->weapons)
        {
            this->add_weapon(it.first, it.second);
        }
    }

    // ---- Helpers ----

    // Check if inventory is completely empty
    bool empty() const
    {
        return this->items.empty() && this->weapons.empty();
    }

    // Convert inventory into string (alias for asText)
    std::string toString(bool label = false) const
    {
        return asText(label);
    }

    std::string itemsAsText(bool label = false) const
    {
        std::string out;
        unsigned int i = 1;

        for (auto it : this->items)
        {
            if (label) out += std::to_string(i++) + ": ";
            out += it.first->name + " (" + std::to_string(it.second) + ") - " + it.first->description + "\n";
        }
        return out;
    }

    std::string weaponsAsText(bool label = false) const
    {
        std::string out;
        unsigned int i = 1;

        for (auto it : this->weapons)
        {
            if (label) out += std::to_string(i++) + ": ";
            out += it.first->name + " (" + std::to_string(it.second) + ") - " + it.first->description + "\n";
        }
        return out;
    }

    std::string asText(bool label = false) const
    {
        if (this->items.empty() && this->weapons.empty())
        {
            return "Nothing\n";
        }

        return itemsAsText(label) + weaponsAsText(label);
    }
};

#endif /* INVENTORY_HPP */

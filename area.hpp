/*
The MIT License (MIT)
... (license text unchanged)
*/

#ifndef AREA_HPP
#define AREA_HPP

#include "inventory.hpp"
#include "creature.hpp"
#include "dialogue.hpp"

#include <vector>
#include <sstream>
#include <string>

// Movement is achieved through the use of areas, which are contained
// units of space consisting of a name, dialogue, inventory, and creatures
class Area
{
public:
    // Name of the area (needed for atlas.cpp)
    std::string name;

    // Dialogue is run whenever the area is entered
    Dialogue dialogue;

    // Items contained within the area. Not split into individual containers
    // for simplicity
    Inventory items;

    // Creatures contained within the area
    std::vector<Creature*> creatures;

    // Full constructor with name
    Area(std::string name, Dialogue dialogue, Inventory items,
         std::vector<Creature*> creatures)
    {
        this->name = name;
        this->dialogue = dialogue;
        this->items = items;
        this->creatures = creatures;
    }

    // Constructor without name (older usage)
    Area(Dialogue dialogue, Inventory items,
         std::vector<Creature*> creatures)
    {
        this->name = "Unknown Area";
        this->dialogue = dialogue;
        this->items = items;
        this->creatures = creatures;
    }

    // Default constructor
    Area() {}

    // Search the area for items and give them to the searcher.
    // Instead of printing, return a string description.
    std::string search(Creature& player)
    {
        std::ostringstream desc;
        if (this->items.empty()) {
            desc << "🔍 You search the area but find nothing of value.";
        } else {
            desc << "🔍 You search the area and find:\n";
            desc << this->items.toString();  // <-- requires toString() in Inventory
            player.inventory.merge(&(this->items));
            this->items.clear();
        }
        return desc.str();
    }
};

#endif /* AREA_HPP */

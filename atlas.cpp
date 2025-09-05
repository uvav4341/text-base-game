#include "atlas.hpp"

void buildatlas_creature(std::vector<Creature>& atlas)
{
    // Fill the atlas
    // Creature(Name, Health, Str, End, Dex, Hit Rate, Level)
    atlas.push_back(Creature("Rat", 8, 8, 8, 12, 2.0, 1));
}

void buildatlas_item(std::vector<Item>& atlas)
{
    // Item(Name, Description)
    atlas.push_back(Item("Gold Coin", "A small disc made of lustrous metal"));
    atlas.push_back(Item("Iron Key", "A heavy iron key with a simple cut"));
}

void buildatlas_weapon(std::vector<Weapon>& atlas)
{
    // Weapon(Name, Description, Damage, Hit Rate)
    atlas.push_back(Weapon("Iron Dagger", "A short blade made of iron with a leather-bound hilt", 5, 10.0));
    atlas.push_back(Weapon("Excalibur", "The legendary blade, bestowed upon you by the Lady of the Lake", 35, 35.0));
}

void buildatlas_area(std::vector<Area>& atlas,
                     std::vector<Item>& items, std::vector<Weapon>& weapons,
                     std::vector<Creature>& creatures)
{
    // Area definitions with names
    atlas.push_back(Area(
        "Room 1",                            // ✅ Name
        Dialogue("You are in room 1",        // Description
                 {"Go to room 2", "Search"}),// Choices
        Inventory(                           // Area inventory
            {
                std::make_pair(&items[0], 5) // Item + quantity
            },
            {
                std::make_pair(&weapons[0], 1) // Weapon + quantity
            }),
        {
            // No creatures here
        }));

    atlas.push_back(Area(
        "Room 2", 
        Dialogue("You are in room 2",
                 {"Go to room 1", "Search"}),
        Inventory(
            {
                std::make_pair(&items[0], 10),
                std::make_pair(&items[1], 1)
            },
            {
                // No weapons here
            }),
        {
            &creatures[0] // Creature present here
        }));
}

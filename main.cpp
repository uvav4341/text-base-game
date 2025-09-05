#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "json.hpp"

#include "atlas.hpp"
#include "item.hpp"
#include "weapon.hpp"
#include "inventory.hpp"
#include "creature.hpp"
#include "dialogue.hpp"
#include "area.hpp"
#include "battle.hpp"

using json = nlohmann::json;

// ---- Global state that persists between requests ----
static std::vector<Creature> creatureAtlas;
static std::vector<Item> itemAtlas;
static std::vector<Weapon> weaponAtlas;
static std::vector<Area> areaAtlas;
static Creature player;
static Area* currentArea = nullptr;
static bool gameStarted = false;

// ---- Build atlases once ----
void init_game() {
    if (!gameStarted) {
        buildatlas_creature(creatureAtlas);
        buildatlas_item(itemAtlas);
        buildatlas_weapon(weaponAtlas);
        buildatlas_area(areaAtlas, itemAtlas, weaponAtlas, creatureAtlas);
        srand(static_cast<unsigned>(time(nullptr)));
        gameStarted = true;
    }
}

// ---- Helper to build JSON responses ----
json make_response(const std::string& description,
                   const std::vector<std::string>& choices = {}) {
    json resp;
    resp["description"] = description;
    resp["choices"] = choices;

    // Convert inventory items to vector<string>
    std::vector<std::string> items_list;
    for (auto &pair : player.inventory.items) { // assume items is vector<pair<Item*, int>>
        items_list.push_back(pair.first->name);
    }

    resp["player"] = {
        {"name",  player.name},
        {"hp",    player.health},
        {"maxHp", player.maxHealth},
        {"str",   player.str},
        {"dex",   player.dex},
        {"level", player.level},
        {"items", items_list},
        {"equippedWeapon", player.equippedWeapon ? player.equippedWeapon->name : "None"}
    };
    return resp;
}

// ---- Character creation ----
void create_new_char(const std::string &name, int choice) {
    if (choice == 1) {
        player = Creature(name, 35, 20, 10, 5, 10.0, 1, "Fighter");
    } else if (choice == 2) {
        player = Creature(name, 30, 5, 10, 20, 15.0, 1, "Rogue");
    } else {
        player = Creature(name, 30, 10, 10, 10, 10.0, 1, "Adventurer");
    }
    currentArea = &areaAtlas[0];
}

// ---- Check Win Condition ----
bool check_win(const std::string &found_item) {
    return found_item.find("Legendary Treasure") != std::string::npos;
}

// ---- Search area and collect items ----
std::string handle_search() {
    std::string found = currentArea->search(player); // adds to inventory internally

    // Auto-equip first weapon if none equipped
    for (auto &item_pair : player.inventory.items) {
        Weapon* wp = dynamic_cast<Weapon*>(item_pair.first);
        if (wp && !player.equippedWeapon) {
            player.equipWeapon(wp);
            break;
        }
    }

    return found;
}

int main() {
    init_game();

    std::string input;
    while (std::getline(std::cin, input)) {
        if (input.empty()) continue;

        json req;
        try {
            req = json::parse(input);
        } catch (const std::exception& e) {
            std::cerr << "Parse error: " << e.what() << std::endl;
            continue;
        }

        json resp;
        std::string action = req.value("action", "");

        if (action == "start") {
            resp = make_response(
                "🎮 Welcome to the Text Adventure RPG!\n\n"
                "📜 Instructions:\n"
                "1. Select 'New Game' to begin.\n"
                "2. Enter your character's name.\n"
                "3. Choose a class (1 = Fighter, 2 = Rogue).\n"
                "4. Explore rooms, search for treasure, and fight creatures.\n\n"
                "👉 What would you like to do?",
                {"New Game"});
        }
        else if (action == "newchar") {
            std::string name  = req.value("name", "Adventurer");
            int choice        = req.value("class", 1);
            create_new_char(name, choice);
            resp = make_response(
                "✨ Welcome, " + player.name + "! You are now in "
                + currentArea->dialogue.getDescription() + ".\n"
                "You can check your menu, move to another room, or search for items.\n\n👉 What will you do next?",
                {"Menu", "Move", "Search"});
        }
        else if (action == "choice") {
            int choice = req.value("value", 0);

            if (choice == 2) { // Search
                std::string found = handle_search();

                // Check win
                if (check_win(found)) {
                    resp = make_response(
                        "🏆 Congratulations, " + player.name + "! "
                        "You found the Legendary Treasure and won the game!",
                        {"End Game"});
                    std::cout << resp.dump() << std::endl;
                    std::cout.flush();
                    continue;
                }

                resp = make_response(
                    "🔎 You searched the area and found: " + found + "\n"
                    "Your inventory has been updated.\n\n👉 What next?",
                    {"Continue"});
            }
            else if (currentArea == &areaAtlas[0]) {
                if (choice == 0) {
                    resp = make_response(
                        "📦 Menu opened!\n- View your Items\n- Check your Equipment\n- See your Character stats.",
                        {"Items", "Equipment", "Character"});
                } else if (choice == 1) {
                    currentArea = &areaAtlas[1];
                    resp = make_response(
                        "🚪 You moved to " + currentArea->dialogue.getDescription() + ".\n"
                        "This room may hold secrets... What will you do?",
                        {"Menu", "Move", "Search"});
                }
            } else if (currentArea == &areaAtlas[1]) {
                if (choice == 0) {
                    resp = make_response(
                        "📦 Menu opened!\n- View your Items\n- Check your Equipment\n- See your Character stats.",
                        {"Items", "Equipment", "Character"});
                } else if (choice == 1) {
                    currentArea = &areaAtlas[0];
                    resp = make_response(
                        "🚪 You moved back to " + currentArea->dialogue.getDescription() + ".\n"
                        "What will you do now?",
                        {"Menu", "Move", "Search"});
                }
            }
        }

        std::cout << resp.dump() << std::endl;
        std::cout.flush();
    }

    return 0;
}

#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>

class Item
{
public:
    std::string name;
    std::string description;

    Item(std::string name, std::string description)
    {
        this->name = name;
        this->description = description;
    }

    Item() {}

    // ---- Make class polymorphic ----
    virtual ~Item() {}  // <-- added virtual destructor

    // ---- Safe text builder ----
    std::string asText() const
    {
        return name + " - " + description;
    }

    // ---- JSON-ready minimal form ----
    std::string asJson() const
    {
        return "{ \"name\": \"" + name + "\", \"description\": \"" + description + "\" }";
    }
};

#endif /* ITEM_HPP */

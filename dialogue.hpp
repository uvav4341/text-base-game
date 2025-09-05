#ifndef DIALOGUE_HPP
#define DIALOGUE_HPP

#include <string>
#include <vector>

// Gameplay is expressed using dialogues, which present a piece of
// information and some responses, and then ask the user to pick one.
class Dialogue
{
private:
    std::string description;
    std::vector<std::string> choices;

public:
    Dialogue(std::string description, std::vector<std::string> choices) {
        this->description = description;
        this->choices = choices;
    }

    Dialogue() {}

    std::string getDescription() const {
        return description;
    }

    std::vector<std::string> getChoices() const {
        return choices;
    }

    // Instead of printing + stdin, just package description and choices
    std::string asText() const {
        std::string result = description + "\n";
        for (size_t i = 0; i < choices.size(); ++i) {
            result += std::to_string(i + 1) + ": " + choices[i] + "\n";
        }
        return result;
    }
};

#endif /* DIALOGUE_HPP */

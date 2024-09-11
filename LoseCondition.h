#pragma once

#include "raylib.h"
#include "Player.h"

class LoseCondition {
private:
    bool loseConditionMet;

public:
    // Default constructor initializes conditionMet with false
    LoseCondition() : loseConditionMet(false) {}

    // Member function to check win condition
    bool isLoseConditionMet() const;

    // Member function to update win condition
    void updateLose(const Player& player);
};

bool LoseCondition::isLoseConditionMet() const {
    return loseConditionMet;
}

void LoseCondition::updateLose(const Player& player) {
    if (player.getLives() <= 0) {
        loseConditionMet = true;
    }
    // You can add additional win condition logic here if needed
}

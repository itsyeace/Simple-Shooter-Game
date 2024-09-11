#pragma once

#include "raylib.h"
#include "Score.h"

class WinCondition {
private:
    bool conditionMet;

public:
    // Default constructor initializes conditionMet with false
    WinCondition() : conditionMet(false) {}

    // Member function to check win condition
    bool isConditionMet() const;

    // Member function to update win condition
    void updateWin(const Score& score);
};

bool WinCondition::isConditionMet() const {
    return conditionMet;
}

void WinCondition::updateWin(const Score& score) {
    if (score.getScore() >= 100) {
        conditionMet = true;
    }
    // You can add additional win condition logic here if needed
}

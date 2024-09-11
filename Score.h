#pragma once
#include "raylib.h"

class Score {
private:
    int playerScore;

public:
    Score() : playerScore(0) {}

    // Increase the player's score when an enemy is killed
    void increaseScore() {
        // Your scoring logic goes here
        // For example, you might increase the score by a fixed amount
        playerScore += 1; // Increase the score by 10 points (adjust as needed)
    }

    // Getter method to retrieve the current player score
    int getScore() const {
        return playerScore;
    }
};

#pragma once

#include "raylib.h"

class GameLevel {
public:
    float baseEnemySpawnInterval;
    float currentEnemySpawnInterval;
    int levelNumber; // Added level number

    // Default constructor
    GameLevel() : baseEnemySpawnInterval(0.0f), currentEnemySpawnInterval(0.0f), levelNumber(1) {}

    // Parameterized constructor
    GameLevel(float baseSpawnInterval, int levelNum) : baseEnemySpawnInterval(baseSpawnInterval), currentEnemySpawnInterval(baseSpawnInterval), levelNumber(levelNum) {}

    // Adjust enemy spawn interval based on player's score and level
    void adjustSpawnInterval(int playerScore) {
        // Example: Increase spawn rate by 2 for each level
        int levelMultiplier = levelNumber * 2;

        // Example: If player score reaches a certain threshold, spawn enemies faster
        if (playerScore >= 10 && playerScore < 20) {
            currentEnemySpawnInterval = baseEnemySpawnInterval / (levelMultiplier * 2);
        } else if (playerScore >= 20) {
            currentEnemySpawnInterval = baseEnemySpawnInterval / (levelMultiplier * 3);
        } else {
            currentEnemySpawnInterval = baseEnemySpawnInterval;
        }
    }

    // Get the current enemy spawn interval for the level
    float getSpawnInterval() const {
        return currentEnemySpawnInterval;
    }
};

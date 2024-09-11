#include "raylib.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "WinCondition.h"
#include "LoseCondition.h"
#include "Score.h"
#include "GameLevel.h"

#define LEVEL_UP_DISPLAY_TIME 1.0f // Adjust the display time for "Level Up!" in seconds
#define PAUSE_DISPLAY_TIME 2.0f    // Adjust the display time for the pause message in seconds
#define INSTRUCTION_DISPLAY_TIME 3.0f // Adjust the display time for instructions in seconds

#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Simple Shooter Game");

    InitAudioDevice();

    // Load background music
    Sound backgroundSound = LoadSound("assets/sound/background.wav");

    bool musicPlaying = false;

    Sound bulletSound = LoadSound("assets/sound/bullet.wav");
    Sound explosionSound = LoadSound("assets/sound/explosion.wav");
    Sound damageSound = LoadSound("assets/sound/damage.wav");
    Sound loseSound = LoadSound("assets/sound/lose.wav");
    Sound winSound = LoadSound("assets/sound/win.wav");

    bool showInstructions = true;
    float instructionTimer = INSTRUCTION_DISPLAY_TIME;

    Score score;
    WinCondition winCondition;
    LoseCondition loseCondition;

    Player player(screenWidth / 2 - 20, screenHeight - 50, 5);
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    const int MAX_ENEMIES = 100;
    const int MAX_BULLETS = 100;

    SetTargetFPS(60);

    float spawnTimer = 0.0f;
    float levelUpTimer = 0.0f; // Add a timer for "Level Up!" display

    bool gamePaused = false;
    bool gameRestarted = false; // Added variable to track if the game has been restarted
    float pauseTimer = 0.0f;

    // Different instances of GameLevel with unique base spawn intervals
    GameLevel level(2.0f, 1); // Level 1 with a base spawn interval of 2.0 seconds
    GameLevel* currentLevel = &level;

    while (!WindowShouldClose()) {

        if (showInstructions) {
            // Display instructions at the beginning of the game
            DrawText("Arrow keys to move left and right, Space to shoot", screenWidth / 2 - MeasureText("Arrow keys to move left and right, Space to shoot", 20) / 2, screenHeight / 2, 20, BLACK);
            instructionTimer -= GetFrameTime();

            if (instructionTimer <= 0.0f) {
                showInstructions = false;
                instructionTimer = INSTRUCTION_DISPLAY_TIME; // Reset the timer
            }
        }

        while (!IsSoundPlaying(backgroundSound)) {
            PlaySound(backgroundSound);
            musicPlaying = true;
        }

        if(!gamePaused && !gameRestarted) { // Check if the game is not paused and not restarted
            player.move(IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT));

            winCondition.updateWin(score);
            loseCondition.updateLose(player);

            for (auto& enemy : enemies) {
                enemy.updatePosition(player.getX());
            }

            for (auto& bullet : bullets) {
                if (bullet.isActive()) {
                    bullet.update();
                }
            }

            // Check collisions with player and update player's lives
            for (auto it = enemies.begin(); it != enemies.end();) {
                Rectangle playerRect = { player.getX(), player.getY(), static_cast<float>(player.getWidth()), static_cast<float>(player.getHeight()) };
                Rectangle enemyRect = { it->getX(), it->getY(), static_cast<float>(it->getWidth()), static_cast<float>(it->getHeight()) };

                if (CheckCollisionRecs(playerRect, enemyRect)) {
                    player.lives--;
                    PlaySound(damageSound);
                    it = enemies.erase(it);
                } else {
                    ++it;
                }
            }

            // Check collisions with bullets and update score
            for (auto& bullet : bullets) {
                if (bullet.isActive()) {
                    for (auto it = enemies.begin(); it != enemies.end();) {
                        Rectangle bulletRect = { bullet.getX(), bullet.getY(), static_cast<float>(bullet.getWidth()), static_cast<float>(bullet.getHeight()) };
                        Rectangle enemyRect = { it->getX(), it->getY(), static_cast<float>(it->getWidth()), static_cast<float>(it->getHeight()) };

                        if (CheckCollisionRecs(bulletRect, enemyRect)) {
                            bullet.update();
                            it = enemies.erase(it);
                            PlaySound(explosionSound);
                            score.increaseScore();
                        } else {
                            ++it;
                        }
                    }
                }
            }

            // Remove inactive bullets
            bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
                return !bullet.isActive();
            }), bullets.end());

            // Update timer and spawn new enemies based on the adjusted interval for the current level
            spawnTimer += GetFrameTime();
            if (spawnTimer >= currentLevel->getSpawnInterval()) {
                if (enemies.size() < MAX_ENEMIES) {
                    enemies.emplace_back(GetRandomValue(0, screenWidth - 40), -40, 200.0f, 50.0f, 100.0f);
                }
                spawnTimer = 0.0f; // Reset the timer
            }

            // Bullet shooting
            if (IsKeyPressed(KEY_SPACE)) {
                if (bullets.size() < MAX_BULLETS) {
                    PlaySound(bulletSound);
                    bullets.emplace_back(player.getX() + player.getWidth() / 2 - 5, player.getY(), 500.0f);
                }
            }

            // Update the current level based on the player's score
            currentLevel->adjustSpawnInterval(score.getScore());

            // Draw entities
            BeginDrawing();
            ClearBackground(WHITE);

            player.draw();

            for (const auto& enemy : enemies) {
                enemy.draw();
            }

            for (const auto& bullet : bullets) {
                if (bullet.isActive()) {
                    bullet.draw();
                }
            }

            DrawText(TextFormat("Lives: %d", player.getLives()), 10, 10, 20, GRAY);
            DrawText(TextFormat("Score: %d", score.getScore()), screenWidth - MeasureText("Score: 12345", 20) - 10, 10, 20, GRAY);

            // Debug output
            //std::cout << "Enemy Spawn Multiplier: " << currentLevel->getSpawnInterval() << std::endl;
            std::cout << "Current Lvl spawn: " << currentLevel->getSpawnInterval() << ", Level spawn: " << level.getSpawnInterval() << std::endl;
            //std::cout << "Score: " << score.getScore() << ", Level Up Timer: " << levelUpTimer << std::endl;


            // Increment the level up timer
            levelUpTimer += GetFrameTime();

            // Display Level UP! texts every time player levels up
            if (score.getScore() >= 10 && score.getScore() < 20) { // Level 2
                if (levelUpTimer <= LEVEL_UP_DISPLAY_TIME) {
                    DrawText("Level Up!", screenWidth / 2 - MeasureText("Level Up!", 30) / 2, screenHeight / 2 - 30, 30, BLUE);
                }
                // Reset the level up timer when transitioning into this score range
                if (score.getScore() == 10) {
                    levelUpTimer = 0.0f;
                }
            } else if (score.getScore() >= 20) {
                if (levelUpTimer <= LEVEL_UP_DISPLAY_TIME) { // Level 3
                    DrawText("Congratulations! You're on fire!", screenWidth / 2 - MeasureText("Congratulations! You're on fire!", 30) / 2, screenHeight / 2 - 30, 30, ORANGE);
                }
                // Reset the level up timer when transitioning into this score range
                if (score.getScore() == 20) {
                    levelUpTimer = 0.0f;
                }
            }

            // Check if the player wins or loses
            if (winCondition.isConditionMet()) {
                DrawText("You Win!", screenWidth / 2 - MeasureText("You Win!", 40) / 2, screenHeight / 2 - 20, 40, GREEN);
                DrawText("Press R to restart", screenWidth / 2 - MeasureText("Press R to restart", 20) / 2, screenHeight / 2 + 20, 20, BLACK);
                PlaySound(winSound);
                gamePaused = true;
                pauseTimer = 0.0f;
            }

            if (loseCondition.isLoseConditionMet()) {
            DrawText("You Lose!", screenWidth / 2 - MeasureText("You Lose!", 40) / 2, screenHeight / 2 - 20, 40, RED);
            DrawText("Press R to restart", screenWidth / 2 - MeasureText("Press R to restart", 20) / 2, screenHeight / 2 + 20, 20, BLACK);
            PlaySound(loseSound);
            player.lives = 0;
            gamePaused = true;
            pauseTimer = 0.0f;
            }

        }
        else {
            // Increment the pause timer
            pauseTimer += GetFrameTime();

            // Check if 'R' key is pressed to restart the game
            if (IsKeyPressed(KEY_R)) {
                // Reset all game variables to their initial state
                score = Score();
                player = Player(screenWidth / 2 - 20, screenHeight - 50, 5);

                enemies.clear();
                bullets.clear();

                winCondition = WinCondition();
                loseCondition = LoseCondition();

                levelUpTimer = 0.0f;
                gamePaused = false;
                pauseTimer = 0.0f;
            } else {
                // Continue the pause if 'R' is not pressed
                gamePaused = true;
            }

            // Resume the game after the pause duration
            if (pauseTimer >= PAUSE_DISPLAY_TIME) {
                gameRestarted = false; // Reset the gameRestarted flag after the pause duration
            }
        }
        EndDrawing();
    }

    UnloadSound(bulletSound);
    UnloadSound(winSound);
    UnloadSound(loseSound);
    UnloadSound(explosionSound);
    UnloadSound(damageSound);
    UnloadSound(backgroundSound);

    // Close audio system
    CloseAudioDevice();

    CloseWindow();

    return 0;
}

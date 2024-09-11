#pragma once

#include "raylib.h"

template <typename T>
T customClamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

class Enemy {
public:
    Enemy(float startX, float startY, float startSpeed, float pursuitDelay, float maxSpeed);

    // Default constructor
    Enemy() : x(0), y(0), speed(0), pursuitDelay(0), currentDelay(0), maxSpeed(200.0f), width(40), height(40) {}

    Enemy& operator=(const Enemy& other);

    void updatePosition(float playerX);
    void draw() const;

    bool isOutsideScreen() const;

    float getX() const;     // Added function to get enemy x-coordinate
    float getY() const;     // Added function to get enemy y-coordinate
    int getWidth() const;   // Added function to get enemy width
    int getHeight() const;  // Added function to get enemy height

private:
    float x, y;
    float speed;
    float pursuitDelay;
    float currentDelay;  // Added variable to track the current delay
    float maxSpeed;     // Added variable to store the maximum speed
    const int width;
    const int height;
};

Enemy::Enemy(float startX, float startY, float startSpeed, float startPursuitDelay, float startMaxSpeed)
    : x(startX), y(startY), speed(startSpeed), pursuitDelay(startPursuitDelay), currentDelay(0), maxSpeed(startMaxSpeed), width(40), height(40) {}

Enemy& Enemy::operator=(const Enemy& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        speed = other.speed;
        pursuitDelay = other.pursuitDelay;
        maxSpeed = other.maxSpeed;
        // Copy other members as needed
    }
    return *this;
}

void Enemy::updatePosition(float playerX) {
    // Calculate direction towards the player
    float direction = (playerX > x) ? 1.0f : -1.0f;

    // Check if it's time to change direction
    if (direction * (playerX - x) < 0) {
        // Start the pursuit delay when changing direction
        currentDelay = pursuitDelay;
    }

    // Apply pursuit delay
    if (currentDelay > 0) {
        currentDelay -= GetFrameTime();
    } else {
        // Update the enemy's x-coordinate based on direction and clamped speed
        x += direction * customClamp(speed, 0.0f, maxSpeed) * GetFrameTime();
    }

    // Update the enemy's y-coordinate to descend vertically
    y += speed * GetFrameTime();
}

void Enemy::draw() const {
    // Draw the enemy as a red rectangle
    DrawRectangle(static_cast<int>(x), static_cast<int>(y), width, height, RED);
}

bool Enemy::isOutsideScreen() const {
    return y > GetScreenHeight();
}

float Enemy::getX() const {
    return x;
}

float Enemy::getY() const {
    return y;
}

int Enemy::getWidth() const {
    return width;
}

int Enemy::getHeight() const {
    return height;
}

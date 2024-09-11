#pragma once
#include "raylib.h"

class Player {
public:
    Player(float startX, float startY, int startLives);

    float x, y;
    int lives;
    float speed;
    int width;
    int height;

    void move(float deltaX);
    void draw() const;

    float getX() const;
    float getY() const;
    int getLives() const;
    int getWidth() const;
    int getHeight() const;
    float getSpeed() const;
};

Player::Player(float startX, float startY, int startLives)
    : x(startX), y(startY), lives(startLives), speed(300.0f), width(50), height(30) {}

void Player::move(float deltaX) {
    x += deltaX * speed * GetFrameTime();
    
    if (x < 0) {
        x = 0;
    } else if (x > GetScreenWidth() - width) {
        x = static_cast<float>(GetScreenWidth() - width);
    }
}

void Player::draw() const {
    DrawRectangle(static_cast<int>(x), static_cast<int>(y), width, height, GREEN);
}

float Player::getX() const {
    return x;
}

float Player::getY() const {
    return y;
}

int Player::getLives() const {
    return lives;
}

int Player::getWidth() const {
    return width;
}

int Player::getHeight() const {
    return height;
}

float Player::getSpeed() const {
    return speed;
}


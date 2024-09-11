#pragma once
#include "raylib.h"

class Bullet {
public:
    Bullet(float startX, float startY, float startSpeed);

    // Default constructor
    Bullet() : x(0), y(0), speed(0), width(5), height(10) {}


    Bullet& operator=(const Bullet& other);

    void update();
    void draw() const;

    bool isActive() const;

    float getX() const;     // Added function to get bullet x-coordinate
    float getY() const;     // Added function to get bullet y-coordinate
    int getWidth() const;   // Added function to get bullet width
    int getHeight() const;  // Added function to get bullet height

private:
    float x, y;
    float speed;
    int width;
    int height;
};

Bullet::Bullet(float startX, float startY, float startSpeed)
    : x(startX), y(startY), speed(startSpeed), width(10), height(20) {}

Bullet& Bullet::operator=(const Bullet& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        speed = other.speed;
        // Copy other members as needed
    }
    return *this;
}

void Bullet::update() {
    // Move the bullet vertically
    y -= speed * GetFrameTime();

    // Deactivate the bullet if it goes off the screen
    if (y < 0) {
        // Set the bullet to inactive
        x = -100;  // Move the bullet off-screen to avoid collision checks
        y = -100;
    }
}

void Bullet::draw() const {
    // Draw the bullet as a yellow rectangle
    DrawRectangle(static_cast<int>(x), static_cast<int>(y), width, height, YELLOW);
}

bool Bullet::isActive() const {
    return y >= 0;  // Bullet is active if it is on-screen
}

float Bullet::getX() const {
    return x;
}

float Bullet::getY() const {
    return y;
}

int Bullet::getWidth() const {
    return width;
}

int Bullet::getHeight() const {
    return height;
}

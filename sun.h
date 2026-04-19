#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
using namespace std;
using namespace sf;

class Sun
{
protected:
    static Texture sunTexture; // Make texture static so it's shared across all instances
    Sprite sunSprite;
    Vector2f position;
    bool exist;
    Clock beforeStart;
    int beforeStartSeconds;
    int currentFrame;
    static bool textureLoaded; // Track if texture was loaded successfully

public:
    Sun() : exist(true), beforeStartSeconds(0), currentFrame(0)
    {
        position.x = rand() % 700 + 260;
        position.y = rand() % 200 + 184;

        // Only try to load the texture once for all instances
        if (!textureLoaded) {
            if (!sunTexture.loadFromFile("PVZ/Plants/sun3.png")) {
                std::cout << "Failed to load sun3.png" << std::endl;
            }
            else {
                textureLoaded = true;
            }
        }

        if (textureLoaded) {
            sunSprite.setTexture(sunTexture);
            sunSprite.setScale(2.0f, 2.0f);
        }
    }

    Sun(float x, float y) : beforeStartSeconds(0), currentFrame(0)
    {
        exist = true;
        position.x = x;
        position.y = y;

        // Only try to load the texture once for all instances
        if (!textureLoaded) {
            if (!sunTexture.loadFromFile("PVZ/Plants/sun3.png")) {
                std::cout << "Failed to load sun3.png" << std::endl;
            }
            else {
                textureLoaded = true;
            }
        }

        if (textureLoaded) {
            sunSprite.setTexture(sunTexture);
            sunSprite.setScale(2.0f, 2.0f);
        }
    }

    void setBeforeStart(int n)
    {
        beforeStart.restart();
        beforeStartSeconds = n;
    }

    bool isThere()
    {
        return exist;
    }

    bool handleClick(float mouseX, float mouseY)
    {
        if (!exist) return false;
        if (sunSprite.getGlobalBounds().contains(mouseX, mouseY))
        {
            exist = false;
            return true;
        }
        return false;
    }

    void draw(RenderWindow& window)
    {
        if (!exist) return;
        if (beforeStart.getElapsedTime().asSeconds() < beforeStartSeconds)
            return;

        sunSprite.setPosition(position.x, position.y);
        window.draw(sunSprite);
    }

    // Method to spawn a new sun at specific position
    void spawn(float x, float y)
    {
        exist = true;
        position.x = x;
        position.y = y;
        beforeStart.restart();
        beforeStartSeconds = 0;
    }
};

// Initialize static members
Texture Sun::sunTexture;
bool Sun::textureLoaded = false;

class SunFactory : public Sun
{
public:
    Sun suns[10]; // Increased to handle both random and sunflower suns

    SunFactory()
    {
        // First 5 are random suns
        for (int i = 0; i < 5; i++)
        {
            suns[i] = Sun();
            suns[i].setBeforeStart((i + 1) * 5);
        }
        // Last 5 are for sunflower suns (initially inactive)
        for (int i = 5; i < 10; i++)
        {
            suns[i] = Sun(0, 0);
            suns[i].setBeforeStart(0);
            // Make them not exist initially
            suns[i].handleClick(0, 0); // This will set exist to false
        }
    }

    void draw(RenderWindow& window)
    {
        for (int i = 0; i < 10; i++)
        {
            suns[i].draw(window);
        }
    }

    // Method to spawn a sunflower sun
    void spawnSunflowerSun(float x, float y)
    {
        // Find an inactive sunflower sun slot (indices 5-9)
        for (int i = 5; i < 10; i++)
        {
            if (!suns[i].isThere())
            {
                suns[i].spawn(x, y);
                break;
            }
        }
    }
};
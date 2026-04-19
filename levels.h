#pragma once
#include <iostream>
#include "zombies.h"
#include "plants.h"
#include "SFML/Graphics.hpp"
#include <cstdlib>
#include <time.h>
using namespace std;
using namespace sf;

enum GameLevel
{
	LEVEL_1 = 1,
	LEVEL_2 = 2,
	LEVEL_3 = 3
};

class Levels
{
protected:
	ZombieFactory zombiefactory;
	PlantFactory plantfactory;
	GameLevel currentLevel;

public:
	Levels(GameLevel level = LEVEL_1) : zombiefactory(), plantfactory(), currentLevel(level)
	{
		setupLevel(level);
	}

	Levels(const Levels&) = delete;
	Levels& operator=(const Levels&) = delete;


	void setupLevel(GameLevel level)
	{
		currentLevel = level;
		// Clean up previous zombies and plants if needed
		zombiefactory.clear();
		plantfactory.clear();

		if (level == LEVEL_1)
		{
			// Only normal zombies
			zombiefactory.initNormalZombiesOnly();
			// Only peashooter and sunflower
			plantfactory.setAvailablePlants({ 1, 2 });
		}
		else if (level == LEVEL_2)
		{
			// Normal + dancing zombies
			zombiefactory.initNormalAndDancingZombies();
			// Peashooter, sunflower, repeater, snowpea
			plantfactory.setAvailablePlants({ 1, 2, 3, 4 });
		}
		else if (level == LEVEL_3)
		{
			zombiefactory.initLevel3Zombies();
			plantfactory.setAvailablePlants({ 1, 2, 3, 4, 5, 6 }); // Peashooter, Sunflower, Wallnut
		}
	
	}

	Vector2f getUpdatedLocation()
	{
		return plantfactory.getUpdatedLocation();
	}

	virtual void moveZombies(RenderWindow& window, int DiversionY, int Cell_Size_Y, int& lives)
	{
		zombiefactory.checkPlantCollision(plantfactory.plants, plantfactory.index);
		zombiefactory.moveZombies(window, DiversionY, Cell_Size_Y, lives);

		// Check bullet collisions with zombies
		plantfactory.checkAllBulletCollisions(zombiefactory.zombies, zombiefactory.numofZomb);
	}

	virtual void draw(RenderWindow& window, int number)
	{
		plantfactory.draw(window, number);
	}

	virtual void createPlant(int number, int type, int x, int y)
	{
		plantfactory.createPlant(number, type, x, y);
	}

	virtual int getIndex()
	{
		return plantfactory.getIndex();
	}

	bool allZombiesDead() const
	{
		if (!zombiefactory.zombies || zombiefactory.numofZomb <= 0)
			return true; // No zombies to check, treat as all dead

		for (int i = 0; i < zombiefactory.numofZomb; ++i)
		{
			Zombie* zombie = zombiefactory.zombies[i];
			// Defensive: check for nullptr and for obviously invalid pointers
			if (zombie != nullptr &&
				zombie != reinterpret_cast<Zombie*>(0xdddddddddddddddd) && // MSVC deleted memory pattern
				zombie->isAlive())
			{
				return false;
			}
		}
		return true;
	}

	const std::vector<int>& getAvailablePlants() const
	{
		return plantfactory.getAvailablePlants();
	}

	GameLevel getLevel() const { return currentLevel; }
};
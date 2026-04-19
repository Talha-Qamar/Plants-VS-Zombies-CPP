#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "sun.h"
using namespace std;
using namespace sf;

// Forward declaration
class Zombie;

class Plant
{
protected:
	Texture plantTexture;
	Sprite plantSprite;
	int health;
	int cost;
	Vector2f position;
	Clock animate;
	Clock bulletTime;
	int row;

public:
	bool isExist = true;
	virtual void draw(RenderWindow& window) = 0;
	virtual void shoot(RenderWindow& window) = 0;
	virtual void checkBulletCollision(Zombie** zombies, int numZombies) = 0;

	Vector2f getPosition()
	{
		return position;
	}
	int getRow()
	{
		return row;
	}
	int getHealth()
	{
		return health;
	}
	void decreaseHealth(int damage)
	{
		health -= damage;
	}
	void setExist(bool exist)
	{
		isExist = false;
	}
};

class Projectile
{
private:
	Texture projectileTexture;
	Sprite projectileSprite;
	float speed;
	int damage;
	Vector2f position;
	int row;
	bool active;

public:
	Projectile(int damage, float speed, const string& textureFile, Vector2f stPos)
	{
		projectileTexture.loadFromFile(textureFile);
		projectileSprite.setTexture(projectileTexture);
		projectileSprite.setScale(2.0f, 2.0f);
		this->speed = speed;
		this->damage = damage;
		this->position = stPos;
		projectileSprite.setPosition(stPos);
		row = (stPos.y - 174) / 98;
		active = false; // Start inactive
	}

	void draw(RenderWindow& window, int x, int y)
	{
		projectileSprite.setPosition(x, y);
		window.draw(projectileSprite);
	}

	void move(RenderWindow& window)
	{
		if (!active) return;
		position.x += speed;
		projectileSprite.setPosition(position);
		draw(window, position.x, position.y);
	}

	void setPositon(Vector2f pos)
	{
		position = pos;
		projectileSprite.setPosition(pos);
		active = true;
	}

	Vector2f getPosition()
	{
		return position;
	}

	FloatRect getBounds()
	{
		return projectileSprite.getGlobalBounds();
	}

	int getDamage()
	{
		return damage;
	}

	bool isActive()
	{
		return active;
	}

	void deactivate()
	{
		active = false;
	}

	int getRow()
	{
		return row;
	}
};

class Peashooter : public Plant
{
private:
	IntRect animationFrames[8];
	int currentFrame;
	Projectile bullet;
	Clock shootTimer;

public:
	Peashooter(int x, int y) : bullet(1, 4, "PVZ/Plants/pea.png", Vector2f(x + 45, y + 7))
	{
		position.x = x;
		position.y = y;
		row = (y - 174) / 98;
		plantTexture.loadFromFile("PVZ/Plants/Peashooter2.png");
		plantSprite.setTexture(plantTexture);
		plantSprite.setScale(2.0f, 2.0f);

		int frameWidth = 26;
		for (int i = 0; i < 8; ++i)
		{
			animationFrames[i] = IntRect(i * frameWidth + i, 0, frameWidth, 32);
		}

		currentFrame = 0;
		health = 100;
		isExist = true;
		shootTimer.restart();
	}

	bool getExist()
	{
		return isExist;
	}

	virtual void draw(RenderWindow& window)
	{
		plantSprite.setTextureRect(animationFrames[currentFrame]);
		plantSprite.setPosition(position.x, position.y);
		window.draw(plantSprite);

		if (animate.getElapsedTime().asSeconds() > 0.1f)
		{
			currentFrame = (currentFrame + 1) % 8;
			animate.restart();
		}
	}

	void shoot(RenderWindow& window)
	{
		// Only shoot every 2 seconds
		if (shootTimer.getElapsedTime().asSeconds() >= 2.0f && !bullet.isActive())
		{
			Vector2f temp = position;
			temp.x += 45;
			temp.y += 7;
			bullet.setPositon(temp);
			shootTimer.restart();
		}

		// Move existing bullet
		if (bullet.isActive())
		{
			bullet.move(window);
			// Deactivate if off screen
			if (bullet.getPosition().x >= 1200)
			{
				bullet.deactivate();
			}
		}
	}

	virtual void checkBulletCollision(Zombie** zombies, int numZombies);

	Vector2f getPosition()
	{
		return plantSprite.getPosition();
	}

	Projectile& getBullet()
	{
		return bullet;
	}
};

class Snowpea : public Plant
{
private:
	IntRect animationFrames[8];
	int currentFrame;
	Projectile bullet;
	Clock shootTimer;

public:
	Snowpea(int x, int y) : bullet(1, 4, "PVZ/Plants/snowball.png", Vector2f(x + 45, y + 7))
	{
		position.x = x;
		position.y = y;
		plantTexture.loadFromFile("PVZ/Plants/snowpea2.png");
		plantSprite.setTexture(plantTexture);
		plantSprite.setScale(2.0f, 2.0f);

		int frameWidth = 30, frameHeight = 32;
		for (int i = 0; i < 8; ++i)
		{
			animationFrames[i] = IntRect(i * frameWidth, 0, frameWidth, frameHeight);
		}
		row = (y - 174) / 98;
		currentFrame = 0;
		health = 100;
		shootTimer.restart();
	}

	virtual void draw(RenderWindow& window)
	{
		plantSprite.setTextureRect(animationFrames[currentFrame]);
		plantSprite.setPosition(position.x, position.y);
		window.draw(plantSprite);

		if (animate.getElapsedTime().asSeconds() > 0.15f)
		{
			currentFrame = (currentFrame + 1) % 8;
			animate.restart();
		}
	}

	void shoot(RenderWindow& window)
	{
		// Only shoot every 2.5 seconds
		if (shootTimer.getElapsedTime().asSeconds() >= 2.5f && !bullet.isActive())
		{
			Vector2f temp = position;
			temp.x += 45;
			temp.y += 7;
			bullet.setPositon(temp);
			shootTimer.restart();
		}

		if (bullet.isActive())
		{
			bullet.move(window);
			if (bullet.getPosition().x >= 1200)
			{
				bullet.deactivate();
			}
		}
	}

	virtual void checkBulletCollision(Zombie** zombies, int numZombies);

	Projectile& getBullet()
	{
		return bullet;
	}
};

class Repeater : public Plant
{
private:
	IntRect animationFrames[5];
	int currentFrame;
	Projectile bullet;
	Projectile bullet2;
	Clock shootTimer;
	Clock secondBulletTimer;
	bool firstBulletFired;

public:
	Repeater(int x, int y)
		: bullet(1, 5, "PVZ/Plants/pea.png", Vector2f(x + 45, y + 7)),
		bullet2(1, 5, "PVZ/Plants/pea.png", Vector2f(x + 45, y + 7))
	{
		position.x = x;
		position.y = y;
		plantTexture.loadFromFile("PVZ/Plants/repeater3.png");
		plantSprite.setTexture(plantTexture);
		plantSprite.setScale(2.0f, 2.0f);
		row = (y - 174) / 98;

		int frameWidth = 35, frameHeight = 31;
		for (int i = 0; i < 5; ++i)
		{
			animationFrames[i] = IntRect(i * frameWidth, 0, frameWidth, frameHeight);
		}
		currentFrame = 0;
		health = 100;
		shootTimer.restart();
		firstBulletFired = false;
	}

	virtual void draw(RenderWindow& window)
	{
		plantSprite.setTextureRect(animationFrames[currentFrame]);
		plantSprite.setPosition(position.x, position.y);
		window.draw(plantSprite);

		if (animate.getElapsedTime().asSeconds() > 0.17f)
		{
			currentFrame = (currentFrame + 1) % 5;
			animate.restart();
		}
	}

	void shoot(RenderWindow& window)
	{
		Vector2f temp = position;
		temp.x += 45;
		temp.y += 7;

		// Shoot every 3 seconds - fire two bullets with 0.2 second delay
		if (shootTimer.getElapsedTime().asSeconds() >= 3.0f && !bullet.isActive() && !firstBulletFired)
		{
			bullet.setPositon(temp);
			firstBulletFired = true;
			secondBulletTimer.restart();
		}

		// Fire second bullet 0.2 seconds after first
		if (firstBulletFired && secondBulletTimer.getElapsedTime().asSeconds() >= 0.2f && !bullet2.isActive())
		{
			bullet2.setPositon(temp);
			firstBulletFired = false;
			shootTimer.restart();
		}

		// Move bullets
		if (bullet.isActive())
		{
			bullet.move(window);
			if (bullet.getPosition().x >= 1200)
			{
				bullet.deactivate();
			}
		}

		if (bullet2.isActive())
		{
			bullet2.move(window);
			if (bullet2.getPosition().x >= 1200)
			{
				bullet2.deactivate();
			}
		}
	}

	virtual void checkBulletCollision(Zombie** zombies, int numZombies);

	Projectile& getBullet1()
	{
		return bullet;
	}

	Projectile& getBullet2()
	{
		return bullet2;
	}
};

class Sunflower : public Plant
{
private:
	IntRect animationFrames[6];
	int currentFrame;
	Clock sunProductionTimer;

public:
	Sunflower(int x, int y)
	{
		cout << "sunflower creating\n";
		plantTexture.loadFromFile("PVZ/Plants/Sunflower3.png");
		plantSprite.setTexture(plantTexture);
		plantSprite.setScale(2.0f, 2.0f);
		position.x = x;
		position.y = y;
		int frameWidth = 30;
		for (int i = 0; i < 6; ++i)
		{
			animationFrames[i] = IntRect(i * frameWidth, 0, frameWidth, 31);
		}

		currentFrame = 0;
		cout << "sunflower created\n";
		row = (y - 174) / 98;
		health = 100;
		sunProductionTimer.restart();
	}

	void draw(RenderWindow& window)
	{
		plantSprite.setTextureRect(animationFrames[currentFrame]);
		plantSprite.setPosition(position.x, position.y);
		window.draw(plantSprite);

		if (animate.getElapsedTime().asSeconds() > 0.1f)
		{
			currentFrame = (currentFrame + 1) % 6;
			animate.restart();
		}
	}

	void shoot(RenderWindow& window)
	{
		if (sunProductionTimer.getElapsedTime().asSeconds() > 10.0f)
		{
			extern SunFactory* gSunFactory;
			if (gSunFactory)
			{
				gSunFactory->spawnSunflowerSun(position.x + 40, position.y);
			}
			sunProductionTimer.restart();
		}
	}

	virtual void checkBulletCollision(Zombie** zombies, int numZombies)
	{
		// Sunflower doesn't shoot bullets
	}

	Vector2f getPosition()
	{
		return plantSprite.getPosition();
	}
};

class Wallnut : public Plant
{
private:
	IntRect animationFrames[5];
	int currentFrame;
	bool exist = true;
	Texture rollingTexture;
	Sprite rollingSprite;
	IntRect movingAnimation[8];
	int currentMovingFrame;
	bool isRolling;
	Clock rollingTime;

public:
	Wallnut(int x, int y)
	{
		position.x = x;
		position.y = y;
		row = (y - 174) / 98;
		plantTexture.loadFromFile("PVZ/Plants/wallnut3.png");
		plantSprite.setTexture(plantTexture);
		plantSprite.setScale(2.0f, 2.0f);

		int frameWidth = 27, frameHeight = 31;
		for (int i = 0; i < 5; ++i)
		{
			animationFrames[i] = IntRect(i * frameWidth, 0, frameWidth, frameHeight);
		}

		currentFrame = 0;
		rollingTexture.loadFromFile("PVZ/Plants/wallnut2.png");
		rollingSprite.setTexture(rollingTexture);
		rollingSprite.setScale(2.0f, 2.0f);

		for (int i = 0; i < 8; ++i)
		{
			movingAnimation[i] = IntRect(i * frameWidth + 2 * i, 0, frameWidth, frameHeight + 2);
		}

		currentMovingFrame = 0;
		isRolling = true;
		health = 4000;
	}

	virtual void draw(RenderWindow& window)
	{
		if (!isRolling)
		{
			plantSprite.setTextureRect(animationFrames[currentFrame]);
			plantSprite.setPosition(position.x, position.y);
			window.draw(plantSprite);

			if (animate.getElapsedTime().asSeconds() > 0.17f)
			{
				currentFrame = (currentFrame + 1) % 5;
				animate.restart();
			}
		}
		else
		{
			rollingSprite.setTextureRect(movingAnimation[currentMovingFrame]);
			rollingSprite.setPosition(position.x, position.y);
			window.draw(rollingSprite);

			if (position.x > 1200)
				isExist = false;

			if (rollingTime.getElapsedTime().asSeconds() > 0.17f)
			{
				position.x += 10;
				currentMovingFrame = (currentMovingFrame + 1) % 8;
				rollingTime.restart();
			}
		}
	}

	void shoot(RenderWindow& window)
	{
		// Wallnut doesn't shoot
	}

	virtual void checkBulletCollision(Zombie** zombies, int numZombies);

	bool getRolling()
	{
		return isRolling;
	}

	FloatRect getBounds()
	{
		if (isRolling)
			return rollingSprite.getGlobalBounds();
		else
			return plantSprite.getGlobalBounds();
	}
};

class Cherrybomb : public Plant
{
private:
	IntRect animationFrames[6];
	int currentFrame;
	bool hasExploded;
	bool explosionProcessed; // New flag to ensure explosion happens only once
	Clock explosionTimer;

public:
	Cherrybomb(int x, int y)
	{
		position.x = x;
		position.y = y;
		plantTexture.loadFromFile("PVZ/Plants/cherrybomb.png");
		plantSprite.setTexture(plantTexture);
		plantSprite.setScale(2.0f, 2.0f);

		animationFrames[0] = IntRect(0, 0, 34, 36);
		animationFrames[1] = IntRect(34 + 0, 0, 35, 36);
		animationFrames[2] = IntRect(115 - 46, 0, 46, 36);
		animationFrames[3] = IntRect(161 - 46, 0, 46, 36);
		animationFrames[4] = IntRect(212 - 50, 0, 50, 36);
		animationFrames[5] = IntRect(263 - 51, 0, 51, 36);

		row = (y - 174) / 98;
		currentFrame = 0;
		health = 100;
		hasExploded = false;
		explosionProcessed = false;
		explosionTimer.restart();
	}

	virtual void draw(RenderWindow& window)
	{
		if (!hasExploded)
		{
			plantSprite.setTextureRect(animationFrames[currentFrame]);
			plantSprite.setPosition(position.x, position.y);
			window.draw(plantSprite);

			if (animate.getElapsedTime().asSeconds() > 0.6f)
			{
				currentFrame = (currentFrame + 1) % 6;
				animate.restart();
			}

			// Explode after 3 seconds
			if (explosionTimer.getElapsedTime().asSeconds() > 3.0f)
			{
				hasExploded = true;
			}
		}
		else if (!explosionProcessed)
		{
			// Mark plant as dead after explosion
			isExist = false;
		}
	}

	void shoot(RenderWindow& window)
	{
		// Cherrybomb doesn't shoot
	}

	virtual void checkBulletCollision(Zombie** zombies, int numZombies);

	bool getHasExploded()
	{
		return hasExploded;
	}

	bool isExplosionProcessed()
	{
		return explosionProcessed;
	}

	void setExplosionProcessed()
	{
		explosionProcessed = true;
	}

	FloatRect getExplosionRadius()
	{
		// 3x3 grid explosion radius centered on cherry bomb
		return FloatRect(position.x - 80, position.y - 98, 240, 294);
	}
};

class PlantFactory
{
public:
	Plant** plants;
	int index;
	std::vector<int> availablePlants; // 1: Peashooter, 2: Sunflower, 3: Snowpea, 4: Repeater, 5: Wallnut, 6: Cherrybomb

	PlantFactory()
	{
		plants = new Plant * [45] {nullptr};
		index = 0;
		availablePlants = { 1, 2, 3, 4, 5, 6 }; // Default: all available
	}

	void setAvailablePlants(const std::vector<int>& plantTypes)
	{
		availablePlants = plantTypes;
	}

	const std::vector<int>& getAvailablePlants() const
	{
		return availablePlants;
	}

	void clear()
	{
		for (int i = 0; i < index; i++)
		{
			delete plants[i];
			plants[i] = nullptr;
		}
		index = 0;
	}

	void createPlant(int index, int type, int x, int y)
	{
		this->index++;
		if (type == 1)
			plants[index] = new Peashooter(x, y);
		else if (type == 2)
			plants[index] = new Sunflower(x, y);
		else if (type == 3)
			plants[index] = new Snowpea(x, y);
		else if (type == 4)
			plants[index] = new Repeater(x, y);
		else if (type == 5)
			plants[index] = new Wallnut(x, y);
		else if (type == 6)
			plants[index] = new Cherrybomb(x, y);
	}

	void finishPlant()
	{
		for (int i = 0; i < index; i++)
		{
			if (plants[i] != nullptr && plants[i]->getHealth() <= 0)
			{
				plants[i]->setExist(false);
			}
		}
	}

	void draw(RenderWindow& window, int number)
	{
		if (number < 0 || number >= index) return;
		if (plants[number] != nullptr && plants[number]->isExist)
		{
			plants[number]->draw(window);
			plants[number]->shoot(window);
			finishPlant();
		}
	}

	void checkAllBulletCollisions(Zombie** zombies, int numZombies)
	{
		for (int i = 0; i < index; i++)
		{
			if (plants[i] != nullptr && plants[i]->isExist)
			{
				plants[i]->checkBulletCollision(zombies, numZombies);
			}
		}
	}

	Vector2f getUpdatedLocation()
	{
		for (int i = 0; i < index; i++)
		{
			if (plants[i] != nullptr)
			{
				if (!plants[i]->isExist)
				{
					return plants[i]->getPosition();
				}
			}
		}
		return Vector2f(-1.f, -1.f);
	}

	int getUpdatedLocationY()
	{
		for (int i = 0; i < index; i++)
		{
			if (plants[i] != nullptr)
			{
				if (!plants[i]->isExist)
				{
					return ((plants[i]->getPosition().y - 174) / 98);
				}
			}
		}
		return -1;
	}

	int getIndex()
	{
		return index;
	}

	PlantFactory(const PlantFactory&) = delete;
	PlantFactory& operator=(const PlantFactory&) = delete;


	~PlantFactory()
	{
		if (plants) {
			for (int i = 0; i < 45; i++)
			{
				delete plants[i];
				plants[i] = nullptr;
			}
			delete[] plants;
			plants = nullptr;
		}
	}
};

// Include zombie class here for collision implementations
#include "zombies.h"

// Implement collision detection methods
void Peashooter::checkBulletCollision(Zombie** zombies, int numZombies)
{
	if (!bullet.isActive()) return;

	for (int i = 0; i < numZombies; i++)
	{
		if (zombies[i] != nullptr && zombies[i]->isAlive() && zombies[i]->getRow() == row)
		{
			if (bullet.getBounds().intersects(zombies[i]->getBounds()))
			{
				zombies[i]->takeDamage(bullet.getDamage());
				bullet.deactivate();
				break;
			}
		}
	}
}

void Snowpea::checkBulletCollision(Zombie** zombies, int numZombies)
{
	if (!bullet.isActive()) return;

	for (int i = 0; i < numZombies; i++)
	{
		if (zombies[i] != nullptr && zombies[i]->isAlive() && zombies[i]->getRow() == row)
		{
			if (bullet.getBounds().intersects(zombies[i]->getBounds()))
			{
				zombies[i]->takeDamage(bullet.getDamage());
				zombies[i]->freeze(3.0f); // Freeze for 3 seconds
				bullet.deactivate();
				break;
			}
		}
	}
}

void Repeater::checkBulletCollision(Zombie** zombies, int numZombies)
{
	// Check first bullet
	if (bullet.isActive())
	{
		for (int i = 0; i < numZombies; i++)
		{
			if (zombies[i] != nullptr && zombies[i]->isAlive() && zombies[i]->getRow() == row)
			{
				if (bullet.getBounds().intersects(zombies[i]->getBounds()))
				{
					zombies[i]->takeDamage(bullet.getDamage());
					bullet.deactivate();
					break;
				}
			}
		}
	}

	// Check second bullet
	if (bullet2.isActive())
	{
		for (int i = 0; i < numZombies; i++)
		{
			if (zombies[i] != nullptr && zombies[i]->isAlive() && zombies[i]->getRow() == row)
			{
				if (bullet2.getBounds().intersects(zombies[i]->getBounds()))
				{
					zombies[i]->takeDamage(bullet2.getDamage());
					bullet2.deactivate();
					break;
				}
			}
		}
	}
}

void Wallnut::checkBulletCollision(Zombie** zombies, int numZombies)
{
	if (!isRolling) return;

	for (int i = 0; i < numZombies; i++)
	{
		if (zombies[i] != nullptr && zombies[i]->isAlive() && zombies[i]->getRow() == row)
		{
			if (getBounds().intersects(zombies[i]->getBounds()))
			{
				zombies[i]->takeDamage(999); // Instant kill
			}
		}
	}
}

void Cherrybomb::checkBulletCollision(Zombie** zombies, int numZombies)
{
	if (!hasExploded || explosionProcessed) return;

	FloatRect explosionArea = getExplosionRadius();
	for (int i = 0; i < numZombies; i++)
	{
		if (zombies[i] != nullptr && zombies[i]->isAlive())
		{
			if (explosionArea.intersects(zombies[i]->getBounds()))
			{
				zombies[i]->takeDamage(999); // Instant kill
			}
		}
	}
	explosionProcessed = true; // Mark explosion as processed
}
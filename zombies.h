#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include <cstdlib>
#include <time.h>
using namespace std;
using namespace sf;

// Forward declaration
class Plant;

class Zombie
{
protected:
	int health;
	int maxHealth;
	float speed;
	float originalSpeed;
	int damage;
	int row;
	bool isAttacking;
	Vector2f position;
	Sprite zomSprite;
	Texture zomTexture;
	Clock zomClock;
	Clock animationClock;
	Sprite attackSprite;
	Texture attackTexture;
	Clock attackClock;
	bool exist;
	bool frozen;
	Clock freezeTimer;
	float freezeDuration;

public:
	Zombie()
	{
		health = 3;
		maxHealth = 3;
		frozen = false;
		freezeDuration = 0;
		originalSpeed = 10;
		speed = originalSpeed;
		exist = true;
	}

	bool isAlive()
	{
		if (!zomSprite.getTexture())
			return false;
		return exist && health > 0;
	}

	void setFinish()
	{
		exist = false;
	}

	void takeDamage(int dmg)
	{
		health -= dmg;
		if (health <= 0)
		{
			exist = false;
		}
	}

	void freeze(float duration)
	{
		frozen = true;
		freezeDuration = duration;
		freezeTimer.restart();
		speed = 0; // Stop movement when frozen
	}

	void updateFreeze()
	{
		if (frozen && freezeTimer.getElapsedTime().asSeconds() >= freezeDuration)
		{
			frozen = false;
			speed = originalSpeed; // Restore original speed
		}
	}

	virtual void draw(RenderWindow& window, int x, int y) {}
	virtual void move(RenderWindow& window, int DiversionY, int Cell_Size_Y) {}
	virtual void checkPlantCollision(Plant** plant, int numPlants) {}

	Vector2f getPosition()
	{
		return position;
	}

	int getRow()
	{
		return row;
	}

	FloatRect getBounds()
	{
		return zomSprite.getGlobalBounds();
	}

	int getHealth()
	{
		return health;
	}
};

// 1. Simple Zombie (NormalZombie)
class NormalZombie : public Zombie
{
	IntRect zomFrames[7];
	int currentFrame;
	IntRect attackFrames[7];
	int currentAttackFrame;

public:
	NormalZombie()
	{
		zomTexture.loadFromFile("PVZ/Zombies/movingzomb3.png");
		zomSprite.setTexture(zomTexture);
		zomSprite.setScale(2.0f, 2.0f);

		attackTexture.loadFromFile("PVZ/Zombies/simplezombie.png");
		attackSprite.setTexture(attackTexture);
		attackSprite.setScale(2.5f, 2.5f);

		int frameWidth = 47, frameHeigth = 48;
		for (int i = 0; i < 7; ++i)
		{
			zomFrames[i] = IntRect(i * frameWidth + 2 * i, 5, frameWidth, frameHeigth);
		}
		row = rand() % 5;

		position.x = 1100;
		int DiversionY = 144;
		int Cell_Size_Y = 98;

		position.y = DiversionY + row * Cell_Size_Y;
		originalSpeed = 5;
		speed = originalSpeed;
		damage = 1;
		exist = true;
		health = 3;
		maxHealth = 3;
		isAttacking = false;
		currentFrame = 0;

		int attackFrameWidth = 35, attackFrameHeigth = 41;
		for (int i = 0; i < 7; ++i)
		{
			attackFrames[i] = IntRect(i * attackFrameWidth + 2 * i, 0, attackFrameWidth, attackFrameHeigth);
		}
		currentAttackFrame = 0;
	}

	virtual void move()
	{
		updateFreeze();
		if (zomClock.getElapsedTime().asSeconds() > 0.25 && !frozen)
		{
			position.x -= speed;
			zomClock.restart();
		}
	}

	virtual void draw(RenderWindow& window, int DiversionY, int Cell_Size_Y)
	{
		if (isAttacking)
		{
			drawAtt(window);
			return;
		}

		move();
		zomSprite.setPosition(position.x, position.y);
		zomSprite.setTextureRect(zomFrames[currentFrame]);

		// Tint blue if frozen
		if (frozen)
			zomSprite.setColor(Color(150, 150, 255));
		else
			zomSprite.setColor(Color::White);

		window.draw(zomSprite);

		if (animationClock.getElapsedTime().asSeconds() > 0.2)
		{
			currentFrame++;
			if (currentFrame >= 7)
			{
				currentFrame = 0;
			}
			animationClock.restart();
		}
	}

	void drawAtt(RenderWindow& window)
	{
		attackSprite.setPosition(position.x, position.y - 12);
		attackSprite.setTextureRect(attackFrames[currentAttackFrame]);
		window.draw(attackSprite);

		if (attackClock.getElapsedTime().asSeconds() > 0.2)
		{
			currentAttackFrame++;
			if (currentAttackFrame >= 7)
			{
				currentAttackFrame = 0;
			}
			attackClock.restart();
		}
	}

	virtual void checkPlantCollision(Plant** plant, int numPlants);
};

// 2. Football Zombie
class FootballZombie : public Zombie
{
protected:
	IntRect zomFrames[8];
	int currentFrame;
	IntRect attackFrames[6];
	int currentAttackFrame;
	bool movingLeft;

public:
	FootballZombie()
	{
		zomTexture.loadFromFile("PVZ/Zombies/footballzomb.png");
		zomSprite.setTexture(zomTexture);
		zomSprite.setScale(2.5f, 2.5f);

		attackTexture.loadFromFile("PVZ/Zombies/footballzomb.png");
		attackSprite.setTexture(attackTexture);
		attackSprite.setScale(2.5f, 2.5f);

		int frameWidth = 39, frameHeigth = 100;
		for (int i = 0; i < 8; ++i)
		{
			zomFrames[i] = IntRect(i * frameWidth, 0, frameWidth, frameHeigth);
		}
		row = rand() % 5;

		position.x = 1100;
		int DiversionY = 134;
		int Cell_Size_Y = 98;

		position.y = DiversionY + row * Cell_Size_Y;
		originalSpeed = 6; // Normal speed
		speed = originalSpeed;
		exist = true;
		isAttacking = false;
		currentFrame = 0;
		health = 6; // Double protection
		maxHealth = 6;
		movingLeft = true;

		int attackFrameWidth = 39, attackFrameHeigth = 100;
		for (int i = 0; i < 6; ++i)
		{
			attackFrames[i] = IntRect(i * attackFrameWidth, 0, attackFrameWidth, attackFrameHeigth);
		}
		currentAttackFrame = 0;
		damage = 1;
	}

	virtual void move()
	{
		updateFreeze();
		if (zomClock.getElapsedTime().asSeconds() > 0.1 && !frozen)
		{
			// Move left and right randomly
			if (rand() % 100 < 90) // 90% move left, 10% right
			{
				position.x -= speed;
				movingLeft = true;
			}
			else
			{
				position.x += speed * 0.5f; // Move right slowly
				movingLeft = false;
			}
			zomClock.restart();
		}
	}

	// In FootballZombie::draw
	virtual void draw(RenderWindow& window, int DiversionY, int Cell_Size_Y)
	{
		// Always call move() to update freeze state and position
		move();

		if (isAttacking)
		{
			drawAtt(window);
			return;
		}

		zomSprite.setPosition(position.x, position.y);
		zomSprite.setTextureRect(zomFrames[currentFrame]);

		if (frozen)
			zomSprite.setColor(Color(150, 150, 255));
		else
			zomSprite.setColor(Color::White);

		window.draw(zomSprite);

		if (animationClock.getElapsedTime().asSeconds() > 0.2)
		{
			currentFrame++;
			if (currentFrame >= 8)
			{
				currentFrame = 0;
			}
			animationClock.restart();
		}
	}

	void drawAtt(RenderWindow& window)
	{
		attackSprite.setPosition(position.x, position.y);
		attackSprite.setTextureRect(attackFrames[currentAttackFrame]);
		window.draw(attackSprite);

		if (attackClock.getElapsedTime().asSeconds() > 1)
		{
			currentAttackFrame++;
			if (currentAttackFrame >= 6)
			{
				currentAttackFrame = 0;
			}
			attackClock.restart();
		}
	}

	virtual void checkPlantCollision(Plant** plant, int numPlants);
};

// 3. Flying Zombie (BalloonZombie)
class BalloonZombie : public Zombie
{
	IntRect zomFrames[10];
	int currentFrame;
	int yMax;
	int ySpeed;

public:
	BalloonZombie()
	{
		zomTexture.loadFromFile("PVZ/Zombies/balloonzomb.png");
		zomSprite.setTexture(zomTexture);
		zomSprite.setScale(2.0f, 2.0f);

		int frameWidth = 34, frameHeigth = 57;
		for (int i = 0; i < 10; ++i)
		{
			zomFrames[i] = IntRect(i * frameWidth, 0, frameWidth, frameHeigth);
		}
		row = rand() % 5;

		position.x = 1100;
		int DiversionY = 144;
		int Cell_Size_Y = 98;
		ySpeed = 2;
		position.y = DiversionY + row * Cell_Size_Y;
		yMax = position.y - 50;
		originalSpeed = 5;
		speed = originalSpeed;
		exist = true;
		health = 3;
		maxHealth = 3;
		isAttacking = false;
		currentFrame = 0;
	}

	virtual void move()
	{
		updateFreeze();
		if (zomClock.getElapsedTime().asSeconds() > 0.2 && !frozen)
		{
			position.x -= speed;
			position.y -= ySpeed;
			zomClock.restart();
		}
	}

	virtual void draw(RenderWindow& window, int DiversionY, int Cell_Size_Y)
	{
		if (position.y <= yMax)
		{
			ySpeed = 0;
		}
		if (position.x <= (260 + 80 * 2))
		{
			ySpeed = -2;
		}

		move();
		zomSprite.setPosition(position.x, position.y);
		zomSprite.setTextureRect(zomFrames[currentFrame]);

		if (frozen)
			zomSprite.setColor(Color(150, 150, 255));
		else
			zomSprite.setColor(Color::White);

		window.draw(zomSprite);

		if (animationClock.getElapsedTime().asSeconds() > 0.2)
		{
			currentFrame++;
			if (currentFrame >= 10)
			{
				currentFrame = 0;
			}
			animationClock.restart();
		}
	}

	// Flying zombie does not interact with plants
	virtual void checkPlantCollision(Plant** plant, int numPlants) override {}
};

// 4. Dancing Zombie
class DancingZombie : public Zombie
{
	IntRect zomFrames[4];
	int currentFrame;
	float diagDir; // 1 for down-right, -1 for up-right
	Clock summonClock;

public:
	DancingZombie()
	{
		zomTexture.loadFromFile("PVZ/Zombies/dancingzomb.png");
		zomSprite.setTexture(zomTexture);
		zomSprite.setScale(1.5f, 1.5f);

		int frameWidth = 50, frameHeigth = 63;
		for (int i = 0; i < 4; ++i)
		{
			zomFrames[i] = IntRect(i * frameWidth, 0, frameWidth, frameHeigth);
		}
		row = rand() % 5;

		position.x = 1100;
		int DiversionY = 144;
		int Cell_Size_Y = 98;

		position.y = DiversionY + row * Cell_Size_Y;
		originalSpeed = 6; // High speed
		speed = originalSpeed;
		exist = true;
		isAttacking = false;
		health = 3;
		maxHealth = 3;
		currentFrame = 0;
		diagDir = (rand() % 2 == 0) ? 1.0f : -1.0f;
		summonClock.restart();
	}

	virtual void move()
	{
		updateFreeze();
		if (zomClock.getElapsedTime().asSeconds() > 0.15 && !frozen)
		{
			// Move diagonally
			position.x -= speed;
			position.y += diagDir * (speed * 0.5f);

			// Clamp to field rows
			if (position.y < 144) {
				position.y = 144;
				diagDir = 1.0f;
			}
			if (position.y > 144 + 4 * 98) {
				position.y = 144 + 4 * 98;
				diagDir = -1.0f;
			}
			zomClock.restart();
		}
	}

	virtual void draw(RenderWindow& window, int DiversionY, int Cell_Size_Y)
	{
		move();
		zomSprite.setPosition(position.x, position.y);
		zomSprite.setTextureRect(zomFrames[currentFrame]);

		if (frozen)
			zomSprite.setColor(Color(150, 150, 255));
		else
			zomSprite.setColor(Color::White);

		window.draw(zomSprite);

		// Summon other zombies every 5 seconds
		if (summonClock.getElapsedTime().asSeconds() > 2.0f && exist)
		{

			summonClock.restart();
		}

		if (animationClock.getElapsedTime().asSeconds() > 0.2)
		{
			currentFrame++;
			if (currentFrame >= 4)
			{
				currentFrame = 0;
			}
			animationClock.restart();
		}
	}

	virtual void checkPlantCollision(Plant** plant, int numPlants);
};

class ZombieFactory
{
public:
	Zombie** zombies;
	int numofZomb = 5;

	ZombieFactory()
	{
		numofZomb = 5;
		srand(time(0));
		zombies = new Zombie * [numofZomb];
		for (int i = 0; i < numofZomb; ++i)
			zombies[i] = nullptr;
	}

	void initNormalZombiesOnly()
	{
		clear();
		numofZomb = 5;
		zombies = new Zombie * [numofZomb];
		for (int i = 0; i < numofZomb; ++i)
			zombies[i] = new NormalZombie();
	}

	void initNormalAndDancingZombies()
	{
		clear();
		numofZomb = 6;
		zombies = new Zombie * [numofZomb];
		for (int i = 0; i < numofZomb; ++i)
		{
			if (i < 3)
				zombies[i] = new NormalZombie();
			else
				zombies[i] = new FootballZombie();
		}
	}

	void initLevel3Zombies()
	{
		clear();
		numofZomb = 8; // 2 Normal, 2 Dancing, 2 Football, 2 Balloon
		zombies = new Zombie * [numofZomb];
		zombies[0] = new NormalZombie();
		zombies[1] = new NormalZombie();
		zombies[2] = new DancingZombie();
		zombies[3] = new DancingZombie();
		zombies[4] = new BalloonZombie();
		zombies[5] = new FootballZombie();
		zombies[6] = new BalloonZombie();
		zombies[7] = new FootballZombie();
	}

	void clear()
	{
		if (zombies)
		{
			for (int i = 0; i < numofZomb; ++i)
			{
				delete zombies[i];
				zombies[i] = nullptr;
			}
			delete[] zombies;
			zombies = nullptr;
		}
	}

	virtual void moveZombies(RenderWindow& window, int DiversionY, int Cell_Size_Y, int& lives)
	{
		for (int i = 0; i < numofZomb; i++)
		{
			if (zombies[i] && zombies[i]->isAlive())
			{
				zombies[i]->draw(window, DiversionY, Cell_Size_Y);
				if (zombies[i]->getPosition().x < 250)
				{
					zombies[i]->setFinish();
					lives--;
				}
			}
		}
	}

	void checkPlantCollision(Plant** plants, int numPlants)
	{
		for (int i = 0; i < numofZomb; i++)
		{
			if (zombies[i] && zombies[i]->isAlive())
			{
				zombies[i]->checkPlantCollision(plants, numPlants);
			}
		}
	}

	~ZombieFactory()
	{
		clear();
		delete[] zombies;
		zombies = nullptr;
		numofZomb = 0;
	}
};

// Include plants header for collision implementations
#include "plants.h"

// Implement collision detection methods
void NormalZombie::checkPlantCollision(Plant** plant, int numPlants)
{
	for (int i = 0; i < numPlants; i++)
	{
		if (plant[i] != nullptr && plant[i]->getRow() == row && plant[i]->isExist)
		{
			if (position.x - plant[i]->getPosition().x < 50)
			{
				plant[i]->decreaseHealth(damage);
				isAttacking = true;
				break;
			}
			else
			{
				isAttacking = false;
			}
		}
	}
}

// In FootballZombie::checkPlantCollision
void FootballZombie::checkPlantCollision(Plant** plant, int numPlants)
{
	bool foundCollision = false;
	for (int i = 0; i < numPlants; i++)
	{
		if (plant[i] != nullptr && plant[i]->getRow() == row && plant[i]->isExist)
		{
			if (position.x + 30 > (plant[i]->getPosition().x) && position.x - plant[i]->getPosition().x < 50)
			{
				plant[i]->decreaseHealth(damage);
				isAttacking = true;
				foundCollision = true;
				break;
			}
		}
	}
	if (!foundCollision)
	{
		isAttacking = false;
	}
}

void DancingZombie::checkPlantCollision(Plant** plant, int numPlants)
{
	// Dancing zombie can eat plants if it collides, but moves diagonally
	for (int i = 0; i < numPlants; i++)
	{
		if (plant[i] != nullptr && plant[i]->isExist)
		{
			// Check y overlap (since it moves diagonally)
			if (abs(position.y - plant[i]->getPosition().y) < 40 && abs(position.x - plant[i]->getPosition().x) < 50)
			{
				plant[i]->decreaseHealth(damage);
				isAttacking = true;
				return;
			}
		}
	}
	isAttacking = false;
}
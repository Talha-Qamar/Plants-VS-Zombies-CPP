#pragma once
#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
using namespace std;
using namespace sf;

void createBack(RenderWindow& window, Texture bgTexture)
{
	const int blackScreen = 84;
	Sprite bgSprite;
	bgSprite.setTexture(bgTexture);
	bgSprite.setPosition(0, blackScreen);
	window.draw(bgSprite);
}

void printGrid(int Field_Status[][9], const int Rows, const int Cols, Clock gridClock)
{
	if(gridClock.getElapsedTime().asSeconds() < 7)
		return;
	gridClock.restart();
	// Print top border
	cout << "+";
	for (int i = 0; i < Cols; ++i)
	{
		cout << "---+";
	}
	cout << endl;
	// Print grid content
	for (int i = 0; i < Rows; ++i)
	{
		cout << "|";
		for (int j = 0; j < Cols; ++j)
		{
			if (Field_Status[i][j] == 0)
				cout << "   |";
			else if (Field_Status[i][j] == 1)
				cout << " P |";
			else if (Field_Status[i][j] == 2)
				cout << " S |";
			else if (Field_Status[i][j] == 3)
				cout << " U |";
			else if (Field_Status[i][j] == 4)
				cout << " R |";
			else if (Field_Status[i][j] == 5)
				cout << " W |";
			else if (Field_Status[i][j] == 6)
				cout << " C |";
			else
				cout << " - |";

		}
		cout << endl;

		// Print separator between rows
		cout << "+";
		for (int k = 0; k < Cols; ++k)
		{
			cout << "---+";
		}
		cout << endl;
	}
	cout << "\n\n\n";
}

void drawMenu(RenderWindow& window, Sprite peashooterSprite, Sprite sunflowerSprite, Sprite repeaterSprite, Sprite snowpeaSprite, Sprite cherrybombSprite, Sprite wallnutSprite)
{

	int x = 80;
	int x2 = 70;
	peashooterSprite.setPosition(x, 12);
	sunflowerSprite.setPosition(x+=x2, 12);
	snowpeaSprite.setPosition(x+=x2, 12);
	repeaterSprite.setPosition(x+=x2, 12);
	wallnutSprite.setPosition(x+=x2, 12);	
	cherrybombSprite.setPosition(x+=x2, 12);

	window.draw(peashooterSprite);
	window.draw(sunflowerSprite);
	window.draw(wallnutSprite);
	window.draw(repeaterSprite);
	window.draw(snowpeaSprite);
	window.draw(cherrybombSprite);

}


void displaySun(RenderWindow& window, int number) {

	Font font;
	if (!font.loadFromFile("PVZ/Fonts/serio.TTF")) {
		// Error loading font
		return;
	}

	Text text;
	text.setFont(font);
	text.setString("Sun :" + to_string(number));
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Yellow);
	text.setPosition(500, 12);

	

		window.draw(text);
	}

void displayLives(RenderWindow& window, int number) 
{

	Font font;
	if (!font.loadFromFile("PVZ/Fonts/Samdan.TTF")) {
		return;
	}

	Text text;
	text.setFont(font);
	text.setString("Lives :" + to_string(number));
	text.setCharacterSize(24);
	text.setFillColor(sf::Color::Red);
	text.setPosition(500, 50);

	window.draw(text);
}

void displayLoose(RenderWindow& window) 
{
	Font font;
	if (!font.loadFromFile("PVZ/Fonts/SamdanEvil.TTF")) {
		return;
	}

	Text text;
	text.setFont(font);

	
	text.setString("You Lost!\nBetter Luck Next Time!");
	text.setCharacterSize(140);
	text.setFillColor(sf::Color::Red);
	text.setPosition(1366/10, 768/4);

	window.draw(text);
}

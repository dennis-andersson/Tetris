#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Screen.hpp"
#include "Game.hpp"
#include "TextElement.hpp"

class CreditsScreen : public Screen
{
private:
	TextElement Credits;
	TextElement text[4];
	std::string name[4] = { "Dennis", "Ali", "Nuri", "Leila" };
	sf::Vector2f position;
	int textSize{ 35 };
	sf::Color textColor{ sf::Color::White };
	sf::Uint8 textStyle{ sf::Text::Style::Regular };
public:
	CreditsScreen()
	{
		// Add whatever needs to be setup here.
		position.x = GameState::getInstance().WindowWidth / 2;	// Center of the window
		//position.y = 200;

		position.y = 100;
		Credits.init("CREDITS", position, textSize, textColor, textStyle);
		Credits.setOriginToCenter();

		position.y = 200;
		int textSize{ 25 };
		for (int i = 0; i < 4; i++)
		{
			text[i].init(name[i], position, textSize, textColor, textStyle);
			// Set various properties // It will be centered around its position
			text[i].setOriginToCenter();
			position.y = position.y + 40;

		}

	}

	~CreditsScreen()
	{
		// Anything that needs to be cleaned up.
	}

	void processInput(sf::RenderWindow& window)
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			Screen::processInput(event);
			if (goBack()) return;
		}
	}

	// This is the method that draws everything. Have a look at the other files or online to see
	// how it's normally implemented.
	void render(sf::RenderWindow& window)
	{
		window.clear();		// Clear the window

		// Draw stuff here.
		drawBackground(window);		// Draw background image
		drawBackButton(window);		// Draw back button

		for (int i = 0; i < 4; i++)
		{
			// Draw example text
			text[i].draw(window);
		}
		Credits.draw(window);

		Screen::render(window);

		window.display();
	}
};


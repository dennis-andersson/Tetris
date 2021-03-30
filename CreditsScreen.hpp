#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "Game.hpp"
#include "Background.hpp"
#include "TextElement.hpp"

class CreditsScreen : public Screen
{
private:
	Background background;
	TextElement test;
	sf::Vector2f position;
	int textSize{ 32 };
	sf::Color textColor{ sf::Color::White };
	sf::Uint8 textStyle{ sf::Text::Style::Regular };
public:
	CreditsScreen(sf::RenderWindow& window, Background& Background) : background(Background)
	{
		// Add whatever needs to be setup here.
		position.x = window.getSize().x / 2;	// Center of the window
		position.y = 100;

		test.init("Test", position, textSize, textColor, textStyle);	// Set various properties
		test.setOriginToCenter();				// It will be centered around its position
	}

	~CreditsScreen()
	{
		// Anything that needs to be cleaned up.
	}

	// This is the method that draws everything. Have a look at the other files or online to see
	// how it's normally implemented.
	void render(sf::RenderWindow& window)
	{
		window.clear();							// Clear the window

		// Draw stuff here.
		background.draw(window);				// Draw background image
		test.draw(window);						// Draw example text

		window.display();
	}
};


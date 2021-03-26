#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "Game.hpp"

class CreditsScreen : public Screen
{
public:
	CreditsScreen()
	{
		// Add whatever needs to be setup here.
	}

	~CreditsScreen()
	{
		// Anything that needs to be cleaned up.
	}

	// This is the method that draws everything. Have a look at the other files or online to see
	// how it's normally implemented.
	void render(sf::RenderWindow& window)
	{
		window.clear();

		// Draw stuff here.

		window.display();
	}
};


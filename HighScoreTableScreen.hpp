#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Screen.hpp"
#include "Game.hpp"
#include "HighScoreTable.hpp"
#include "TextElement.hpp"
#include "Background.hpp"

class HighScoreTableScreen : public Screen
{
private:
	HighScoreTable highScores;
	Background background;
public:
	HighScoreTableScreen(sf::RenderWindow& window, Background& Background, HighScoreTable& HighScores) : background(Background), highScores(HighScores)
	{
		// Add whatever needs to be setup here.
	}

	~HighScoreTableScreen()
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


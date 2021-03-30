#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Screen.hpp"
#include "Game.hpp"
#include "HighScoreTable.hpp"

class HighScoreTableScreen : public Screen
{
private:
	HighScoreTable highScores;
public:
	HighScoreTableScreen(HighScoreTable& HighScores) : highScores(HighScores)
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


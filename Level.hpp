#pragma once

#include <SFML/System.hpp>

class Level
{
	int levelLimit{ 1000 };
	int currentLevel;
	float speedFactor{ 0.87 };
	sf::Time speed;
public:
	Level()
	{
		currentLevel = 1;
		speed = sf::seconds(speedFactor * currentLevel);
	}

	void setLevel(int level)
	{
		// Store the level in a variable and calculate the game speed.
		currentLevel = level;
	}

	int getLevel()
	{
		// Return the current level number.
		return currentLevel;
	}

	sf::Time& getLevelSpeed()
	{
		// Return the current game speed.
		return speed;
	}

	void nextLevel(int score)
	{
		// Increase the level number and calculate the new game speed.
		if (score >= (levelLimit * currentLevel)) {
			currentLevel++;
			speed *= speedFactor;
		}
	}
};


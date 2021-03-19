#pragma once

class Level
{
	int levelLimit{ 1000 };
	int currentLevel;
public:
	Level()
	{
		currentLevel = 1;
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

	float getLevelSpeed()
	{
		// Return the current game speed.
	}

	void nextLevel(int score)
	{
		// Increase the level number and calculate the new game speed.
		if (score >= (levelLimit * currentLevel))
			currentLevel++;
	}
};


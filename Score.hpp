#pragma once

class Score
{
	bool multipleTetris{ false };
	bool tetris{ false };
public:
	int score{ 0 };

	void addSoftScore(int n)
	{
		score += n;
	}

	void addPoints(int rowsCleared, int level)
	{
		std::array<int, 4> points{ 40, 100, 300, 1200 };
		multipleTetris = tetris && (rowsCleared == 4);
		tetris = (rowsCleared == 4);

		if (multipleTetris)
			score += (3 * points[rowsCleared - 1]) * level;
		else
			score += points[rowsCleared - 1] * level;

		// TODO: Play a sound effect based on the number of rows cleared.
	}
};


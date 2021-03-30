#pragma once

#include <fstream>
#include <vector>
#include <string>

struct HighScore
{
	std::string Name;
	int Score;

	HighScore(std::string name, int score) : Name(name), Score(score) {}
};

// We can use a simple text format to begin with.
// Something like this is sufficient

// Maria
// 200000
// Victoria
// 150000
// Sarah
// 100000
// Mike
// 50000
// Anders
// 25000
// John
// 5000

class HighScoreTable
{
	std::string filename{ "highscores.dat" };
public:
	std::vector<HighScore> HighScores;

	void readHighScores()
	{
		// Read lines from a file and add the high score to HighScores.
		std::ifstream file;
		file.open(filename);
		std::string line;
		int Score;
		int counter = 10;
		while (!file.eof() && counter--) {
			getline(file, line);
			file >> Score;
			HighScores.push_back(HighScore(line, Score));
		}
		file.close();
	}

	void saveHighScores()
	{
		// Save the high scores to the high score table file.
		std::ofstream file;
		file.open(filename);
		for (int i = 0; i < HighScores.size(); ++i) {
			file << HighScores[i].Name << "\n" << HighScores[i].Score << "\n";
			/*file << v[i] << endl;*/
		}
		file.close();
	}

	void addHighScore(std::string name, int score)
	{
		// Create an instance of HighScore and insert the new high score in sorted order.
	}

	bool isScoreHighEnough(int Score)
	{
		// We should probably have a limit of high scores, say 10-15 maybe.
		// Compare the score with the high scores and return true if the score
		// is high enough to add to the high score table.
	}
};


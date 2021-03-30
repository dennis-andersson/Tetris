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
	const int headerSize{ 42 };
	const int textSize{ 26 };
	const int skip{ textSize + 9 };
	const sf::Color textColor{ sf::Color::White };
	TextElement header;
	TextElement number;
	TextElement name;
	TextElement score;
	TextElement entry;
	sf::Vector2f headerPosition{ 0, 0 };
	const sf::Vector2f numberPosition{ 100, 100 };
	const sf::Vector2f namePosition{ numberPosition.x + 60, numberPosition.y };
	const sf::Vector2f scorePosition{ namePosition.x + 170, numberPosition.y };
	const sf::Vector2f entryPosition{ 10, numberPosition.y + skip };
	const sf::Uint8 headerStyle{ sf::Text::Style::Bold };
	const sf::Uint8 textStyle{ sf::Text::Style::Regular };
public:
	HighScoreTableScreen(sf::RenderWindow& window, Background& Background, HighScoreTable& HighScores) : background(Background), highScores(HighScores)
	{
		// Add whatever needs to be setup here.
		const sf::Vector2u windowSize = window.getSize();
		const int Width = windowSize.x;
		const int Height = windowSize.y;

		headerPosition.x = Width / 2;
		headerPosition.y = 40;

		header.init("HIGH SCORES", headerPosition, headerSize, textColor, headerStyle);
		header.setOriginToCenter();

		number.init("No", numberPosition, textSize, textColor, textStyle);
		name.init("Name", namePosition, textSize, textColor, textStyle);
		score.init("Score", scorePosition, textSize, textColor, textStyle);

		entry.init("", numberPosition, textSize, textColor, textStyle);
	}

	~HighScoreTableScreen()
	{
		// Anything that needs to be cleaned up.
	}

	void renderHighScore(sf::RenderWindow& window, const int n, HighScore& highScore)
	{
		TextElement entryNumber(std::to_string(n), sf::Vector2f(numberPosition.x, numberPosition.y + (n * skip)), textSize, textColor, textStyle);
		TextElement entryName(highScore.Name, sf::Vector2f(namePosition.x, namePosition.y + (n * skip)), textSize, textColor, textStyle);
		TextElement entryScore(std::to_string(highScore.Score), sf::Vector2f(scorePosition.x, scorePosition.y + (n * skip)), textSize, textColor, textStyle);

		entryNumber.draw(window);
		entryName.draw(window);
		entryScore.draw(window);
	}

	// This is the method that draws everything. Have a look at the other files or online to see
	// how it's normally implemented.
	void render(sf::RenderWindow& window)
	{
		window.clear();

		// Draw stuff here.
		background.draw(window);

		header.draw(window);
		number.draw(window);
		name.draw(window);
		score.draw(window);

		for (int i = 0; i < highScores.HighScores.size(); ++i)
			renderHighScore(window, i + 1, highScores.HighScores[i]);

		window.display();
	}
};


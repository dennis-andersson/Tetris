#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <stack>
#include <random>
#include <chrono>
#include "MenuScreen.hpp"
#include "Tetromino.hpp"
#include "HighScoreTable.hpp"
#include "Level.hpp"
#include "TextElement.hpp"
#include "Grid.hpp"
#include "Score.hpp"
#include "Border.hpp"
#include "Direction.hpp"
#include "Screen.hpp"
#include "ScreensEnum.hpp"
#include "LevelSelectionScreen.hpp"
#include "GameScreen.hpp"
#include "HighScoreTableScreen.hpp"
#include "CreditsScreen.hpp"
#include "AddHighScoreScreen.hpp"

class Game
{
private:
	sf::RenderWindow window;
	const std::string windowTitle{ "Tetris" };
	const int WindowWidth{ 500 };
	const int WindowHeight{ 580 };
	HighScoreTable highScores;
	sf::Image titleBarIcon;

public:
	void Run()
	{
		window.create(sf::VideoMode(WindowWidth, WindowHeight), windowTitle);
		window.setFramerateLimit(60);

		titleBarIcon.loadFromFile("Icons/tetris.png");
		window.setIcon(titleBarIcon.getSize().x, titleBarIcon.getSize().y, titleBarIcon.getPixelsPtr());

		highScores.readHighScores();

		Background background;
		background.setBackground("Images/background3.jpg");
		background.setSize(WindowWidth, WindowHeight);

		MenuScreen menu(window, background);
		GameScreen game(window, background);
		LevelSelectionScreen levelSelection(window, background);
		CreditsScreen credits(window, background);
		HighScoreTableScreen highScoreTable(window, background, highScores);
		AddHighScoreScreen addHighScoreScreen(window, background, highScores);

		ScreensEnum currentScreen = ScreensEnum::Menu;

		while (currentScreen != ScreensEnum::None) {
			switch (currentScreen) {
			case ScreensEnum::Menu:
				currentScreen = menu.run(window);
				break;
			case ScreensEnum::Play:
				currentScreen = game.run(window);
				break;
			case ScreensEnum::LevelSelection:
				currentScreen = levelSelection.run(window);
				break;
			case ScreensEnum::HighScores:
				currentScreen = highScoreTable.run(window);
				break;
			case ScreensEnum::Credits:
				currentScreen = credits.run(window);
				break;
			}
		}

		highScores.saveHighScores();
	}
};


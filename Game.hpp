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

		MenuScreen menu;
		GameScreen game;
		LevelSelectionScreen levelSelection;
		CreditsScreen credits;
		HighScoreTableScreen highScoreTable;

		highScoreTable.setHighScores(highScores);

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


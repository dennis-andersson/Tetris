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
#include "GameState.hpp"

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
		titleBarIcon.loadFromFile("Icons/tetris.png");
		GameState::getInstance().Window.create(sf::VideoMode(WindowWidth, WindowHeight), windowTitle);
		GameState::getInstance().Window.setFramerateLimit(60);
		GameState::getInstance().Window.setIcon(titleBarIcon.getSize().x, titleBarIcon.getSize().y, titleBarIcon.getPixelsPtr());
		GameState::getInstance().HighScoreTable.readHighScores();
		GameState::getInstance().Sound.loadSoundEffects();

		MenuScreen menu;
		GameScreen game;
		LevelSelectionScreen levelSelection;
		CreditsScreen credits;
		HighScoreTableScreen highScoreTable;
		AddHighScoreScreen addHighScoreScreen;

		ScreensEnum currentScreen = ScreensEnum::Menu;

		while (currentScreen != ScreensEnum::None) {
			switch (currentScreen) {
			case ScreensEnum::Menu:
				currentScreen = menu.run();
				break;
			case ScreensEnum::Play:
				currentScreen = game.run();
				break;
			case ScreensEnum::LevelSelection:
				currentScreen = levelSelection.run();
				break;
			case ScreensEnum::HighScores:
				currentScreen = highScoreTable.run();
				break;
			case ScreensEnum::Credits:
				currentScreen = credits.run();
				break;
			}
		}

		GameState::getInstance().HighScoreTable.saveHighScores();
	}
};


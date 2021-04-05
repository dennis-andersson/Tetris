#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <stack>
#include <random>
#include <chrono>
#include <fstream>
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
#include "GameScreen.hpp"
#include "HighScoreTableScreen.hpp"
#include "CreditsScreen.hpp"
#include "GameState.hpp"
#include "ControlsScreen.hpp"

class Game
{
private:
	const std::string windowTitle{ "Tetris" };
	sf::Image titleBarIcon;
	std::string volumeDataFile{ "volume.dat" };

	void setVolumeLevel()
	{
		std::ifstream file;
		float volume;

		file.open(volumeDataFile);
		file >> volume;
		file.close();

		GameState::getInstance().Sound.setVolume(volume);
	}

	void saveVolumeLevel()
	{
		std::ofstream file;

		file.open(volumeDataFile);
		file << GameState::getInstance().Sound.getVolume();
		file.close();
	}
public:
	void Run()
	{
		titleBarIcon.loadFromFile("Icons/tetris.png");
		GameState::getInstance().Window.create(sf::VideoMode(GameState::getInstance().WindowWidth, GameState::getInstance().WindowHeight), windowTitle);
		GameState::getInstance().Window.setFramerateLimit(60);
		GameState::getInstance().Window.setIcon(titleBarIcon.getSize().x, titleBarIcon.getSize().y, titleBarIcon.getPixelsPtr());
		GameState::getInstance().HighScoreTable.readHighScores();
		GameState::getInstance().Sound.loadSoundEffects();
		setVolumeLevel();

		MenuScreen menu;
		GameScreen game;
		CreditsScreen credits;
		HighScoreTableScreen highScoreTable;
		ControlsScreen controlsScreen;

		ScreensEnum currentScreen = ScreensEnum::Menu;

		while (currentScreen != ScreensEnum::None) {
			switch (currentScreen) {
			case ScreensEnum::Menu:
				currentScreen = menu.run();
				break;
			case ScreensEnum::Play:
				GameState::getInstance().Sound.stopMenuMusic();
				currentScreen = game.run();
				break;
			case ScreensEnum::HighScores:
				currentScreen = highScoreTable.run();
				break;
			case ScreensEnum::Controls:
				currentScreen = controlsScreen.run();
				break;
			case ScreensEnum::Credits:
				currentScreen = credits.run();
				break;
			}
		}

		saveVolumeLevel();
		GameState::getInstance().HighScoreTable.saveHighScores();
	}
};


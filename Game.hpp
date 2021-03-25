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

public:
	void Run()
	{
		window.create(sf::VideoMode(WindowWidth, WindowHeight), windowTitle);
		window.setFramerateLimit(60);

		highScores.readHighScores();

		//Menu, LevelSelection, GameScreen, HighScores, Credits, None

//		std::array<Screen, 5> screens{ MenuScreen(window), 
//										LevelSelectionScreen(window), 
////										GameScreen(),
//										CreditsScreen(window),
//										HighScoreTableScreen(window), 
//										CreditsScreen(window) };
//

		//std::array<Screen, 5> screens;
		//screens[0] = MenuScreen();
		//auto foo = screens[0];
//		screens[1] = LevelSelectionScreen();
//		//	screens[1] = GameScreen();
//		screens[2] = CreditsScreen();
//		screens[3] = HighScoreTableScreen();
//		screens[4] = CreditsScreen();

		//std::vector<Screen> screens2{ MenuScreen(),
		//											LevelSelectionScreen(), 
		//	//										GameScreen(),
		//											CreditsScreen(),
		//											HighScoreTableScreen(), 
		//											CreditsScreen() };
		MenuScreen menu;
		GameScreen game;
		LevelSelectionScreen levelSelection;
		CreditsScreen credits;
		HighScoreTableScreen hs;
	
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
				currentScreen = hs.run(window);
				break;
			case ScreensEnum::Credits:
				currentScreen = credits.run(window);
				break;
			}
			//currentScreen = screens[currentScreen].run(window);
			//currentScreen = screen.run(window);
		}

		highScores.saveHighScores();
	}
};


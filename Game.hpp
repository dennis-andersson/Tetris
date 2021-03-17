#pragma once

#include <stack>
#include <queue>
#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "MenuScreen.hpp"
#include "Tetromino.hpp"
#include "HighScoreTable.hpp"

class Game
{
private:
	const int WindowWidth{ 724 };
	const int WindowHeight{ 774 };
	//std::stack<Screen> screens;
	std::queue<Tetromino> blocks;
	HighScoreTable highScores;

	void generateBlocks()
	{

	}
public:
	Game()
	{
		generateBlocks();
		highScores.readHighScores();
	}

	void myMethod()
	{

	}

	void Run()
	{
		sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Tetris");
		
		while (window.isOpen()) {
			sf::Event event;

			while (window.pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					window.close();
			}

			window.clear();
			//screens.top().Render();
			window.display();
		}

		highScores.saveHighScores();
	}

	//void SwitchScreen(Screen screen)
	//{
	//	screens.push(screen);
	//}
};


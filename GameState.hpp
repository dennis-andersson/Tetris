#pragma once

#include <SFML/Graphics.hpp>
#include "Background.hpp"
#include "HighScoreTable.hpp"
#include "Sound.hpp"

class GameState
{
protected:
	GameState()
	{
		Font.loadFromFile("Fonts/arial.ttf");
		Background.setBackground("Images/background3.jpg", WindowWidth, WindowHeight);
	}

	~GameState() {}

	static GameState* instance;

public:
	GameState(GameState& other) = delete;

	void operator=(const GameState&) = delete;

	static GameState& getInstance()
	{
		if (instance == nullptr)
			instance = new GameState;
		return *(instance);
	}

	int WindowWidth{ 500 };
	int WindowHeight{ 580 };
	sf::RenderWindow Window;
	sf::Font Font;
	HighScoreTable HighScoreTable;
	HighScore CurrentScore;
	Background Background;
	Sound Sound;
};

GameState* GameState::instance = nullptr;

#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <stack>
#include <random>
#include <chrono>
#include "Screen.hpp"
#include "MenuScreen.hpp"
#include "Tetromino.hpp"
#include "HighScoreTable.hpp"
#include "Level.hpp"
#include "TextElement.hpp"
#include "Grid.hpp"
#include "Score.hpp"
#include "Border.hpp"
#include "Direction.hpp"

class Rectangle
{
private:
	int x, y, width, height;
public:

};

struct Point
{
	int x, y;

	Point(int X, int Y) : x(X), y(Y) {}
	Point() {}
} a[4], b[4];


class Game
{
private:
	const int WindowWidth{ 500 };
	const int WindowHeight{ 580 };
	HighScoreTable highScores;

	// Stack area
	// 15 x 30 blocks
	const int Width { 15 };
	const int Height { 30 };
	const int BlockSize{ 18 };
	sf::Vector2f borderSize;
	sf::Vector2f borderPosition;
	Border border;
	Grid grid;

	int shapes{ 7 };
	Tetromino currentShape;
	Tetromino nextShape;
	sf::Vector2i shapeFirstPosition;
	sf::Vector2i nextShapePosition;

	int blockColors{ 8 };
	sf::Texture blockTextures;
	std::vector<sf::Sprite> blocks;
	int board[15][30]{ 0 };

	//Direction direction{Direction::Down};

	bool running;

	int textSize{ 20 };
	sf::Uint32 textStyle{ sf::Text::Bold };

	std::vector<TextElement> fixedTextElements;
	TextElement scoreText;
	TextElement levelText;
	TextElement linesText;
	TextElement gameOverText;

	Score currentScore;
	int linesCleared{ 0 };
	Level currentLevel;
	bool GameOver;

	int figures[7][4] =
	{
		1,3,5,7, // I
		2,4,5,7, // Z
		3,5,4,6, // S
		3,5,4,7, // T
		2,3,5,7, // L
		3,5,7,6, // J
		2,3,4,5, // O
	};

	std::default_random_engine engine{ static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()) };

	int getRandomShapeId()
	{
		int n = getRandomNumber(7) % 7;
		return n;
	}

	int getRandomNumber(int max) {
		std::uniform_int_distribution<int> int_distribution(max);
		return int_distribution(engine);
	}

	void generateBlocks()
	{
		blockTextures.loadFromFile("Images/blocks.png");

		for (int i = 0; i < blockColors; ++i)
			blocks.push_back(sf::Sprite(blockTextures, sf::IntRect(i * BlockSize, 0, BlockSize, BlockSize)));
	}

	void setup()
	{
		generateBlocks();

		// Setup grid
		grid.setDimensions(Width, Height);

		// Setup game border
		borderPosition.x = 40.f;
		borderPosition.y = 20.f;
		borderSize.x = Width * BlockSize;
		borderSize.y = Height * BlockSize;
		border.setSize(borderSize);
		border.setPosition(borderPosition);

		// Setup text elements
		std::vector<std::string> fixedStrings{ "Score", "Level", "Lines" };
		sf::Color textColor(193, 193, 193, 255);
		float inity{ 25.f };
		float x{ WindowWidth - 140.f };
		float y{ inity };
		float step = 90.f;

		for (int i = 0; i < fixedStrings.size(); ++i, y += step)
			fixedTextElements.push_back(TextElement(fixedStrings[i], sf::Vector2f(x, y), textSize, textColor, sf::Text::Bold));

		fixedTextElements.push_back(TextElement("Next", sf::Vector2f(x, y + 40), textSize, textColor, sf::Text::Bold));

		y = inity + 35;
		scoreText.init(sf::Vector2f(x, y), textSize, sf::Color::White, sf::Text::Bold);
		levelText.init(sf::Vector2f(x, y += step), textSize, sf::Color::White, sf::Text::Bold);
		linesText.init(sf::Vector2f(x, y += step), textSize, sf::Color::White, sf::Text::Bold);
		gameOverText.init("GAME OVER", sf::Vector2f(borderPosition.x + 0.5f * Width, WindowHeight * 0.4f), textSize * 2, sf::Color::White, sf::Text::Bold);
		gameOverText.toggleVisible();

		// Shape setup
		shapeFirstPosition.x = 6;
		shapeFirstPosition.y = 0;
		nextShapePosition.x = 20;
		nextShapePosition.y = 20;
		currentShape.setOffset(borderPosition);
		newShape(currentShape, shapeFirstPosition);
		newShape(nextShape, nextShapePosition);
	}

	void newShapes()
	{
		newShape(currentShape, shapeFirstPosition, nextShape.getId());
		newShape(nextShape, nextShapePosition);
	}

	void newShape(Tetromino& shape, sf::Vector2i position, int newId)
	{
		shape.newShape(newId);
		shape.setBlocksSprite(blocks[currentShape.getId()]);
		shape.setPosition(position);
	}

	void newShape(Tetromino& shape, sf::Vector2i position)
	{
		newShape(shape, position, getRandomShapeId());
	}

	void drawTextElements(sf::RenderWindow& window)
	{
		for (auto te : fixedTextElements)
			te.draw(window);

		scoreText.draw(window, currentScore.score);
		levelText.draw(window, currentLevel.getLevel());
		linesText.draw(window, linesCleared);
		gameOverText.draw(window);
	}

	void bar()
	{
		std::vector<float> xs;
		for (int level = 0; level < 40; ++level)
			xs.push_back(85.f / (85.f + (level * (level * 5.f))));

		setup();
	}

public:
	Game()
	{
		setup();
	}

	sf::Sprite& getBlock(int blockId)
	{
		return blocks[blockId];
	}

	void render(sf::RenderWindow &window)
	{
		window.clear();

		// Draw text
		drawTextElements(window);

		// Draw border
		border.draw(window);

		// Draw blocks
		for (int row = 0; row < Height; row++) {
			if (grid.isVisible(row)) {
				for (int column = 0; column < Width; ++column) {
					Cell cell = grid.getCell(row, column);
					int blockId = cell.id;
					if (cell.empty) continue;
					blocks[blockId].setPosition(getCellPosition(row, column));
					window.draw(blocks[blockId]);
				}
			}
		}

		// Draw shape
		currentShape.draw(window);
		nextShape.draw(window);

		window.display();
	}

	sf::Vector2f getCellPosition(int row, int column)
	{
		return sf::Vector2f(borderPosition.x + (column * BlockSize), borderPosition.y + (row * BlockSize));
	}

	void GameLoop(sf::RenderWindow &window)
	{
		sf::Clock clock;
		sf::Time deltaTime{ sf::Time::Zero };
		sf::Time elapsedTime{ sf::Time::Zero };

		while (window.isOpen()) {
			if (!GameOver) {
				sf::Time trigger{ sf::seconds(85.f / (85.f + (currentLevel.getLevel() * (currentLevel.getLevel() * 5.f)))) };
				deltaTime = clock.restart();
				elapsedTime += deltaTime;

				processEvents(window);
				update(elapsedTime, deltaTime);

				if (elapsedTime > trigger) {
					elapsedTime = sf::Time::Zero;
					moveShape(Direction::Down);
				}

				render(window);
			} else {
				sf::Event event;
				bool move;

				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						window.close();
					
					if (event.type == sf::Event::KeyPressed) {
						grid.clear();
						gameOverText.toggleVisible();
						return;
					}
				}

				gameOverText.toggleVisible();
				render(window);
			}
		}
	}

	void update(const sf::Time& elapsedTime, const sf::Time& deltaTime) {
		grid.update(deltaTime);

		if (!currentShape.isVisible()) {
			if (grid.isToRemoveBlocks())
				return;
			newShapes();
		}
	}

	void moveShape(Direction direction) {
		if (canMove(currentShape.getFutureBlockPositions(direction))) {
			currentShape.move(direction);
		} else {
			if (direction == Direction::Down || direction == Direction::SoftDown) {
				int id = currentShape.getId();
				grid.addBlock(id, currentShape.getBlockPositions());
				newShape(currentShape, shapeFirstPosition, nextShape.getId());
				newShape(nextShape, nextShapePosition);
				currentScore.addSoftScore(10);
				int rowsCleared = grid.markLinesForRemoval();
				if (rowsCleared)
					currentScore.addPoints(rowsCleared, currentLevel.getLevel());
				currentLevel.nextLevel(currentScore.score);

				if (!canMove(currentShape.getBlockPositions()))
					GameOver = true;
			}
		}
	}

	bool canMove(std::array<sf::Vector2i, 4> block) {
		for (int i = 0; i < 4; ++i) {
			if (block[i].x < 0 || block[i].x > (Width - 1) || block[i].y > (Height - 1))
				return false;

			if (!grid.getCell(block[i].y, block[i].x).empty)
				return false;
		}
		return true;
	}

	void processEvents(sf::RenderWindow &window) {
		sf::Event event;
		bool move;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			} else if (event.type == sf::Event::KeyPressed) {
				//if (e.type == sf::Event::KeyPressed)
				//	if (e.key.code == sf::Keyboard::Up) rotate = true;
				//	else if (e.key.code == sf::Keyboard::Left) dx = -1;
				//	else if (e.key.code == sf::Keyboard::Right) dx = 1;

				switch (event.key.code) {
				case sf::Keyboard::Left:
					moveShape(Direction::Left);
					break;
				case sf::Keyboard::Right:
					moveShape(Direction::Right);
					break;
				case sf::Keyboard::Down:
					moveShape(Direction::SoftDown);
					break;
				case sf::Keyboard::Up:
					rotate();
					break;
				}
			}

			if (event.type == sf::Event::JoystickMoved) {
				float X = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
				float Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

				// Left stick
				//  X > 0	Right
				//	X < 0	Left
				//	Y < 0	Up
				//	Y > 0	Down

				// Right Stick
				// U > 0	Right
				// U < 0	Left
				// V < 0	Up
				// V > 0	Down

				float limit{ 50.f };
				if (X > limit)
					moveShape(Direction::Right);
				if (X < -limit)
					moveShape(Direction::Left);
				if (Y > limit)
					moveShape(Direction::SoftDown);
			}
		}

		if (sf::Joystick::isConnected(0))
		{
			bool buttonPressed = gamepadButtonPressed();
			if (buttonPressed)
				rotate();
		}

	}

	void rotate() {
		//if (!mTetromino) return;
		currentShape.rotate();

		if (!canMove(currentShape.getBlockPositions()))
			currentShape.revertState();
	}

	bool gamepadButtonPressed()
	{
		unsigned int buttonCount = sf::Joystick::getButtonCount(0);
		bool buttonPressed{ false };
		for (int i = 0; i < sf::Joystick::getButtonCount(0); ++i)
			buttonPressed |= sf::Joystick::isButtonPressed(0, i);
		return buttonPressed;
	}

	void newGame()
	{
		sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "Tetris");

		while (true) {
			GameOver = false;
			GameLoop(window);
		}
	}

	void Run()
	{
		highScores.readHighScores();
		newGame();
		highScores.saveHighScores();
	}

/*
	bool check()
	{
		for (int i = 0; i < 4; i++)
			if (a[i].x < 0 || a[i].x >= Width || a[i].y >= Height) return 0;
			else if (board[a[i].y][a[i].x]) return 0;

		return 1;
	};
	int foo()
	{
		srand(time(0));

		sf::RenderWindow window(sf::VideoMode(320, 480), "The Game!");

		sf::Texture t1, t2, t3;
		t1.loadFromFile("images/tiles.png");
		t2.loadFromFile("images/background.png");
		t3.loadFromFile("images/frame.png");

		sf::Sprite s(t1), background(t2), frame(t3);

		int dx = 0; bool rotate = 0; int colorNum = 1;
		float timer = 0, delay = 0.3;

		sf::Clock clock;

		while (window.isOpen())
		{
			float time = clock.getElapsedTime().asSeconds();
			clock.restart();
			timer += time;

			sf::Event e;
			while (window.pollEvent(e))
			{
				if (e.type == sf::Event::Closed)
					window.close();

				if (e.type == sf::Event::KeyPressed)
					if (e.key.code == sf::Keyboard::Up) rotate = true;
					else if (e.key.code == sf::Keyboard::Left) dx = -1;
					else if (e.key.code == sf::Keyboard::Right) dx = 1;
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) delay = 0.05;

			//// <- Move -> ///
			for (int i = 0; i < 4; i++) { b[i] = a[i]; a[i].x += dx; }
			if (!check())
				for (int i = 0; i < 4; i++)
					a[i] = b[i];

			//////Rotate//////
			if (rotate)
			{
				Point p = a[1]; //center of rotation
				for (int i = 0; i < 4; i++)
				{
					int x = a[i].y - p.y;
					int y = a[i].x - p.x;
					a[i].x = p.x - x;
					a[i].y = p.y + y;
				}

				if (!check())
					for (int i = 0; i < 4; i++)
						a[i] = b[i];
			}

			///////Tick//////
			if (timer > delay)
			{
				for (int i = 0; i < 4; i++) {
					b[i] = a[i]; a[i].y += 1;
				}

				if (!check()) {
					for (int i = 0; i < 4; i++)
						board[b[i].y][b[i].x] = colorNum;

					colorNum = 1 + rand() % 7;
					int n = rand() % 7;
					for (int i = 0; i < 4; i++) {
						a[i].x = figures[n][i] % 2;
						a[i].y = figures[n][i] / 2;
					}
				}

				timer = 0;
			}

			///////check lines//////////
			int k = Height - 1;
			for (int i = Height - 1; i > 0; i--)
			{
				int count = 0;
				for (int j = 0; j < Width; j++)
				{
					if (board[i][j]) count++;
					board[k][j] = board[i][j];
				}
				if (count < Width) k--;
			}

			dx = 0; rotate = 0; delay = 0.3;

			/////////draw//////////
			window.clear(sf::Color::White);
			window.draw(background);

			for (int i = 0; i < Height; i++)
				for (int j = 0; j < Width; j++)
				{
					if (board[i][j] == 0) continue;
					s.setTextureRect(sf::IntRect(board[i][j] * 18, 0, 18, 18));
					s.setPosition(j * 18, i * 18);
					s.move(28, 31); //offset
					window.draw(s);
				}

			for (int i = 0; i < 4; i++)
			{
				s.setTextureRect(sf::IntRect(colorNum * 18, 0, 18, 18));
				s.setPosition(a[i].x * 18, a[i].y * 18);
				s.move(28, 31); //offset
				window.draw(s);
			}

			window.draw(frame);
			window.display();
		}

		return 0;
	}
	*/
};


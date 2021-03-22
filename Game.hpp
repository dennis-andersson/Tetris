#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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
		shape.setBlocksSprite(blocks[newId]);
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
		if (!GameOver) {
			currentShape.draw(window);
			nextShape.draw(window);
		}

		// Draw text
		drawTextElements(window);

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
				sf::Time trigger{ currentLevel.getLevelSpeed() };
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

				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						window.close();
					
					if (event.type == sf::Event::KeyPressed)
						return;
				}

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
				if (rowsCleared) {
					currentScore.addPoints(rowsCleared, currentLevel.getLevel());
					linesCleared += rowsCleared;
				}
				currentLevel.nextLevel(currentScore.score);

				if (!canMove(currentShape.getBlockPositions()))
					gameOver();
			}
		}
	}

	void gameOver()
	{
		GameOver = true;
		gameOverText.setVisible(true);
		for (int row = 10; row < 16; ++row)
			grid.setVisible(row, false);
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
				case sf::Keyboard::A:
					// DEBUG
					currentScore.addSoftScore(1200);
					currentLevel.nextLevel(currentScore.score);
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

		sf::SoundBuffer buffer;
		if (!buffer.loadFromFile("sounds/pause.wav")) {
			return;
		}

		sf::Sound sound;
		sound.setBuffer(buffer);
		//sound.play();

		while (true) {
			// TODO: Play some music to entertain the player.

			GameOver = false;
			gameOverText.setVisible(false);
			currentScore.score = 0;
			currentLevel.setLevel(1);
			linesCleared = 0;
			grid.clear();

			GameLoop(window);
		}
	}

	void Run()
	{
		highScores.readHighScores();
		newGame();
		highScores.saveHighScores();
	}
};


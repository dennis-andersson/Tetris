#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>
#include <vector>
#include <stack>
#include <random>
#include <chrono>
#include "Tetromino.hpp"
#include "HighScoreTable.hpp"
#include "Level.hpp"
#include "TextElement.hpp"
#include "Grid.hpp"
#include "Score.hpp"
#include "Border.hpp"
#include "Direction.hpp"
#include "Sound.hpp"
#include "ScreensEnum.hpp"
#include "Screen.hpp"
#include "Input.hpp"

enum GameMode
{
	Running, RemovingLines, Paused, GameOver
};

class GameScreen : public Screen
{
private:
	int WindowWidth;
	int WindowHeight;
	HighScoreTable highScores;

	// Stack area
	// 15 x 30 blocks
	const int Width{ 15 };
	const int Height{ 30 };
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

	const int blockColors{ 8 };
	const int blocksInAShape{ 4 };
	sf::Texture blockTextures;
	std::vector<sf::Sprite> blocks;

	Direction direction;
	bool rotateShape{ false };
	float dpadX, dpadY;
	float limit{ 15.f };
	float delta{ 0.2f };
	float X, Y;

	int textSize{ 20 };
	sf::Uint32 textStyle{ sf::Text::Bold };

	std::vector<TextElement> fixedTextElements;
	TextElement scoreText;
	TextElement levelText;
	TextElement linesText;
	TextElement gameOverText;
	TextElement pausedText;

	Score currentScore;
	int linesCleared{ 0 };
	Level currentLevel;

	Sound sound;

	bool initialized{ false };
	bool endGame{ false };

	std::default_random_engine engine{ static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()) };

	int getRandomShapeId()
	{
		int n = getRandomNumber(shapes) % shapes;
		return n;
	}

	int getRandomNumber(int max)
	{
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
		sf::Color textColor2{ sf::Color::White };
		sf::Uint32 textStyle{ sf::Text::Bold };
		float inity{ 25.f };
		float x{ WindowWidth - 140.f };
		float y{ inity };
		float step = 90.f;

		for (int i = 0; i < fixedStrings.size(); ++i, y += step)
			fixedTextElements.push_back(TextElement(fixedStrings[i], sf::Vector2f(x, y), textSize, textColor, textStyle));

		fixedTextElements.push_back(TextElement("Next", sf::Vector2f(x, y + 40), textSize, textColor, textStyle));

		y = inity + 35;
		scoreText.init(sf::Vector2f(x, y), textSize, textColor2, textStyle);
		levelText.init(sf::Vector2f(x, y += step), textSize, textColor2, textStyle);
		linesText.init(sf::Vector2f(x, y += step), textSize, textColor2, textStyle);
		gameOverText.init("GAME OVER", sf::Vector2f(borderPosition.x + 0.5f * Width, WindowHeight * 0.4f), textSize * 2, textColor2, textStyle);
		gameOverText.toggleVisible();
		pausedText.init("PAUSED", sf::Vector2f(borderPosition.x + 3.5f * Width, WindowHeight * 0.4f), textSize * 2, textColor2, textStyle);
		pausedText.toggleVisible();

		// Shape setup
		shapeFirstPosition.x = 6;
		shapeFirstPosition.y = 0;
		nextShapePosition.x = 20;
		nextShapePosition.y = 20;
		currentShape.setOffset(borderPosition);
		newShape(currentShape, shapeFirstPosition);
		newShape(nextShape, nextShapePosition);

		// Load sound effects
		sound.loadSoundEffects();

		initialized = true;
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
		shape.setVisible(true);
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
		pausedText.draw(window);
	}
public:
	sf::Sprite& getBlock(int blockId)
	{
		return blocks[blockId];
	}

	void render(sf::RenderWindow& window)
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
		if (mode == GameMode::Running) {
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

	void GameLoop(sf::RenderWindow& window)
	{
		sf::Clock clock;
		sf::Time deltaTime{ sf::Time::Zero };
		sf::Time elapsedTime{ sf::Time::Zero };

		while (window.isOpen()) {
			sf::Time trigger{ currentLevel.getLevelSpeed() };
			deltaTime = clock.restart();
			elapsedTime += deltaTime;

			switch (mode) {
			case GameMode::Running:
				processEvents(window);

				if (endGame) return;

				update(deltaTime);

				if (elapsedTime > trigger) {
					elapsedTime = sf::Time::Zero;
					moveShape(Direction::Down);
				}

				render(window);
				break;
			case GameMode::RemovingLines:
				processEvents(window);
				if (endGame) return;
				update(deltaTime);
				render(window);
				if (!grid.aboutToRemoveLines())
					mode = GameMode::Running;
				break;
			case GameMode::Paused:
				processEvents(window);
				if (endGame) return;
				update(deltaTime);
				render(window);
				break;
			case GameMode::GameOver:
				sf::Event event;

				while (window.pollEvent(event)) {
					if (event.type == sf::Event::Closed)
						window.close();

					if (event.type == sf::Event::JoystickButtonPressed) {
						mode = Running;
						return;
					}

					if (event.type == sf::Event::KeyPressed) {
						if (event.key.code == sf::Keyboard::Escape)
							endGame = true;
						else
							mode = Running;
						return;
					}
				}

				render(window);
				break;
			}
		}
	}

	void update(const sf::Time& deltaTime)
	{
		if (direction != Direction::None)
			moveShape(direction);

		if (rotateShape)
			rotate();

		grid.update(deltaTime);

		if (!currentShape.isVisible()) {
			if (grid.aboutToRemoveLines())
				return;
			newShapes();
		}
	}

	void moveShape(Direction direction)
	{
		if (canMove(currentShape.getFutureBlockPositions(direction))) {
			currentShape.move(direction);
		} else {
			if (direction == Direction::Down || direction == Direction::SoftDown) {
				int id = currentShape.getId();
				grid.addBlock(id, currentShape.getBlockPositions());
				currentShape.toggleVisible();

				currentScore.addSoftScore(10);
				int rowsToRemove = grid.markLinesForRemoval();

				if (rowsToRemove) {
					currentScore.addPoints(rowsToRemove, currentLevel.getLevel());
					linesCleared += rowsToRemove;
					mode = GameMode::RemovingLines;
				} else {
					newShapes();
					if (!canMove(currentShape.getBlockPositions()))
						gameOver();
				}

				currentLevel.nextLevel(currentScore.score);
			}
		}
	}

	void gameOver()
	{
		mode = GameMode::GameOver;
		gameOverText.setVisible(true);
		foo();
	}

	void foo()
	{
		int startRow{ 10 };
		int endRow{ 16 };

		for (int row = startRow; row < endRow; ++row)
			grid.setVisible(row, !grid.isVisible(row));
	}

	bool canMove(std::array<sf::Vector2i, 4> block)
	{
		for (int i = 0; i < blocksInAShape; ++i) {
			if (block[i].x < 0 || block[i].x >(Width - 1) || block[i].y > (Height - 1))
				return false;

			if (!grid.getCell(block[i].y, block[i].x).empty)
				return false;
		}
		return true;
	}

	void processEvents(sf::RenderWindow& window)
	{
		sf::Event event;

		rotateShape = false;
		direction = Direction::None;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Left:
					direction = Direction::Left;
					break;
				case sf::Keyboard::Right:
					direction = Direction::Right;
					break;
				case sf::Keyboard::Down:
					direction = Direction::SoftDown;
					break;
				case sf::Keyboard::Up:
					rotateShape = true;
					break;
				case sf::Keyboard::Escape:
					endGame = true;
					break;
				case sf::Keyboard::P:
					pauseGame();
					break;
				case sf::Keyboard::A:
					// DEBUG
					currentScore.addSoftScore(1200);
					currentLevel.nextLevel(currentScore.score);
					break;
				}
			}

			if (sf::Joystick::isConnected(0)) {
				bool buttonPressed = gamepadButtonPressed();

				if (buttonPressed)
					rotateShape = true;

				X = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
				Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

				dpadX = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovX);
				dpadY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);

				if (dpadX > 0.5f)	direction = Direction::Right;
				if (dpadX < -0.5f)	direction = Direction::Left;
				if (dpadY < -0.5f)	direction = Direction::SoftDown;
				if (dpadY > 0.5f)	rotateShape = true;

				if (X > limit)	direction = Direction::Right;
				if (X < -limit)	direction = Direction::Left;
				if (Y > limit)	direction = Direction::SoftDown;
				if (Y < -limit)	rotateShape = true;
			}
		}
	}

	void rotate()
	{
		currentShape.rotate();

		if (!canMove(currentShape.getBlockPositions()))
			currentShape.revertState();
	}

	GameMode mode;

	ScreensEnum run(sf::RenderWindow& window)
	{
		WindowWidth = window.getSize().x;
		WindowHeight = window.getSize().y;

		if (!initialized)
			setup();

		while (!endGame) {
			// TODO: Play some music to entertain the player.

			resetGame();
			GameLoop(window);
		}
		endGame = false;

		return ScreensEnum::Menu;
	}

	void resetGame()
	{
		mode = GameMode::Running;
		gameOverText.setVisible(false);
		currentScore.score = 0;
		currentLevel.reset();
		linesCleared = 0;
		newShapes();
		grid.makeAllRowsVisible();
		grid.clear();
	}

	void pauseGame()
	{
		mode = (mode == GameMode::Running) ? GameMode::Paused : GameMode::Running;
		pausedText.toggleVisible();
		foo();
	}
};


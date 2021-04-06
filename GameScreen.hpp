#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <unordered_map>
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
#include "GameState.hpp"


enum class GameMode
{
	Running, RemovingLines, Paused, GameOver, AddHighScore
};

class GameScreen : public Screen
{
private:
	int WindowWidth;
	int WindowHeight;
	std::stack<GameMode> mode;

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

	Movement movement;
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

	bool endGame{ false };
	bool counterClockwise;

	sf::RectangleShape addHighscoreBox;
	TextElement addHighscoreMessage;
	TextElement nameText;
	std::string name;
	bool addedHighScore{ false };

	std::default_random_engine engine{ static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()) };


	std::unordered_map<sf::Keyboard::Key, Direction> table{
		{sf::Keyboard::Left, Direction::Left},
		{sf::Keyboard::Right, Direction::Right},
		{sf::Keyboard::Down, Direction::SoftDown}
	};

	std::unordered_map<sf::Event::EventType, InputSource> eventTypes{
		{sf::Event::KeyPressed, InputSource::Keyboard},
		{sf::Event::KeyReleased, InputSource::Keyboard},
		{sf::Event::JoystickMoved, InputSource::Joystick} };

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
	GameScreen()
	{
		WindowWidth = GameState::getInstance().WindowWidth;
		WindowHeight = GameState::getInstance().WindowHeight;

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

		// Add High score stuff
		addHighscoreBox.setSize(sf::Vector2f(361,300));
		addHighscoreBox.setPosition(sf::Vector2f(50, 100));
		addHighscoreBox.setFillColor(sf::Color::Black);
		addHighscoreBox.setOutlineColor(sf::Color::Red);
		addHighscoreBox.setOutlineThickness(4.f);
		sf::Vector2f addHighscoreMessagePosition{ 80, 130 };
		sf::Vector2f nameTextPosition{ 105, 215 };
		addHighscoreMessage.init("You've got a high score!\nPlease enter your name", addHighscoreMessagePosition, 25, textColor2, textStyle);
		nameText.init(nameTextPosition, 25, textColor2, textStyle);
		sf::FloatRect box{ addHighscoreMessage.getBoundingBox() };
	}

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
		if (mode.top() == GameMode::Running) {
			currentShape.draw(window);
			nextShape.draw(window);
		}

		// Draw text
		drawTextElements(window);

		if (mode.top() == GameMode::AddHighScore) {
			window.draw(addHighscoreBox);
			addHighscoreMessage.draw(window);
			nameText.draw(window, name);
		}

		Screen::render(window);

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
			sf::Time foo = sf::milliseconds(16);

			switch (mode.top()) {
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
				if (elapsedTime > foo) {
					elapsedTime = sf::Time::Zero;

					if (endGame) return;
					processEvents(window);

					update(deltaTime);
					render(window);
					if (!grid.aboutToRemoveLines()) {
						mode.pop();
						movement.reset();
					}
				}
				break;
			case GameMode::Paused:
			case GameMode::GameOver:
			case GameMode::AddHighScore:
				processEvents(window);
				if (endGame) return;
				update(deltaTime);
				render(window);
				break;
			}
		}
	}

	void update(const sf::Time& deltaTime)
	{
		Screen::update(deltaTime);

		if (mode.top() == GameMode::Paused)
			return;

		if (mode.top() == GameMode::AddHighScore) {
			return;
		}

		movement.update(deltaTime.asSeconds());

		//if (mode.top() != GameMode::RemovingLines && movement.direction != Direction::Down)
		//	moveShape(movement.direction);

		if (mode.top() != GameMode::RemovingLines && movement.source != InputSource::None && movement.readyToMove())
			moveShape(movement.direction);

		if (rotateShape)
			rotate(counterClockwise);

		grid.update(deltaTime);

		if (!currentShape.isVisible()) {
			if (grid.aboutToRemoveLines())
				return;
			newShapes();
		}
	}

	void moveShape(Direction direction)
	{
		if (direction == Direction::HardDrop) {
			while (canMove(currentShape.getFutureBlockPositions(Direction::Down)))
				currentShape.move(Direction::Down);
			moveShape(Direction::Down);
		} else if (canMove(currentShape.getFutureBlockPositions(direction))) {
			currentShape.move(direction);
		} else {
			if (movement.direction == Direction::Down || movement.direction == Direction::SoftDown) {
				int id = currentShape.getId();
				grid.addBlock(id, currentShape.getBlockPositions());
				currentShape.toggleVisible();

				currentScore.addSoftScore(10);
				int rowsToRemove = grid.markLinesForRemoval();

				if (rowsToRemove) {
					GameState::getInstance().Sound.playSoundEffect(rowsToRemove == 4 ? SoundEffect::Tetris : SoundEffect::LinesCleared);
					currentScore.addPoints(rowsToRemove, currentLevel.getLevel());
					linesCleared += rowsToRemove;
					if (mode.top() == GameMode::RemovingLines)
						X++;
					mode.push(GameMode::RemovingLines);
				}
				else {
					GameState::getInstance().Sound.playSoundEffect(SoundEffect::SoftDrop);
					newShapes();
					if (!canMove(currentShape.getBlockPositions()))
						gameOver();
				}

				currentLevel.nextLevel(currentScore.score);
			} else {
				movement.reset();
			}
		}
	}

	void gameOver()
	{
		mode.push(GameMode::GameOver);
		gameOverText.setVisible(true);
		makeRoomForText();

		if (GameState::getInstance().HighScoreTable.isScoreHighEnough(currentScore.score) && !addedHighScore) {
			mode.push(GameMode::AddHighScore);
			addedHighScore = true;
		} else if (addedHighScore) {
			resetGame();
		}
	}

	void makeRoomForText()
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
		JoypadButtons button;
		sf::Event event;
		float position;

		if (sf::Joystick::isConnected(0) && mode.top() == GameMode::Running) {
			X = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::X);
			Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

			if (movement.source == InputSource::Joystick && movement.type == InputType::Stick && abs(X) < limit && abs(Y) < limit)
				movement.reset();

			if (movement.direction == Direction::Down && abs(X) > limit || abs(Y) > limit) {
				if (abs(X) > abs(Y))
					movement.direction = X > 0.f ? Direction::Right : Direction::Left;
				else
					if (Y > 0.f)
						movement.direction = Direction::SoftDown;

				movement.type = InputType::Stick;
				movement.source = InputSource::Joystick;
			}
		}

		rotateShape = false;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
				return;
			}

			if (event.type == sf::Event::JoystickButtonPressed)
				button = joypadButton(event.joystickButton.button);

			switch (mode.top()) {
			case GameMode::Running:
				if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
					movement.reset();
					continue;
				}

				if (movement.source != InputSource::None && eventTypes.contains(event.type) && eventTypes[event.type] != movement.source)
					continue;

				switch (event.type) {
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Down && event.key.control) {
						movement.source = InputSource::Keyboard;
						movement.direction = Direction::HardDrop;
						movement.key = event.key.code;
						return;
					}else if (table.contains(event.key.code)) {
						if (movement.direction != Direction::Down)
							break;

						movement.source = InputSource::Keyboard;
						movement.direction = table[event.key.code];
						movement.key = event.key.code;
					}else{
						switch (event.key.code) {
						case sf::Keyboard::Up:
							rotateShape = true;
							counterClockwise = event.key.control;
							break;
						case sf::Keyboard::Escape:
							endGame = true;
							break;
						case sf::Keyboard::P:
							pauseGame();
							break;
						default:
							Screen::processInput(event);
							break;
						}
					}
					break;
				case sf::Event::KeyReleased:
					if (movement.direction == Direction::Down)
						break;

					if (movement.key == event.key.code)
						movement.reset();

					break;
				case sf::Event::JoystickButtonPressed:
					switch (button) {
					case JoypadButtons::MenuB:
					case JoypadButtons::ViewB:
						pauseGame();
						break;
					case JoypadButtons::A:
					case JoypadButtons::B:
					case JoypadButtons::X:
						rotateShape = true;
						counterClockwise = (button == JoypadButtons::X);
						break;
					case JoypadButtons::Y:
						movement.source = InputSource::Joystick;
						movement.direction = Direction::HardDrop;
						return;
						break;
					}
					break;
				case sf::Event::JoystickMoved:
					position = event.joystickMove.position;

					if (movement.type == InputType::Dpad && (position < 0.5f && position > -0.5f)) {
						movement.reset();
						break;
					}

					if (movement.source == InputSource::Keyboard)
						break;

					if (movement.source == InputSource::Joystick && event.joystickMove.axis != movement.axis)
						break;

					switch (event.joystickMove.axis) {
					case sf::Joystick::Axis::PovX:
						movement.source = InputSource::Joystick;
						movement.axis = sf::Joystick::Axis::PovX;
						movement.type = InputType::Dpad;
						if (position > 0.5f)	movement.direction = Direction::Right;
						if (position < -0.5f)	movement.direction = Direction::Left;
						break;
					case sf::Joystick::Axis::PovY:
						movement.source = InputSource::Joystick;
						movement.axis = sf::Joystick::Axis::PovY;
						movement.type = InputType::Dpad;
						dpadY = event.joystickMove.position;
						if (position < 0.5f)	movement.direction = Direction::SoftDown;
						break;
					default:
						if (event.joystickMove.axis != sf::Joystick::Axis::X && event.joystickMove.axis != sf::Joystick::Axis::Y)
							break;
						break;
					}
					break;
				}
				break;
			case GameMode::Paused:
			case GameMode::RemovingLines:
				switch (event.type) {
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::P)
						pauseGame();
					else if (event.key.code == sf::Keyboard::Escape)
						endGame = true;
					break;
				case sf::Event::JoystickButtonPressed:
					if (button == JoypadButtons::MenuB || button == JoypadButtons::ViewB)
						pauseGame();
					break;
				}
				break;
			case GameMode::GameOver:
				if (event.type == sf::Event::JoystickButtonPressed) {
					mode.pop();
					return;
				}

				if (event.type == sf::Event::KeyPressed) {
					if (event.key.code == sf::Keyboard::Escape)
						endGame = true;
					else
						mode.pop();
					return;
				}
				break;
			case GameMode::AddHighScore:
				if (event.type == sf::Event::KeyPressed) {
					sf::Keyboard::Key key = event.key.code;
					const int nameLimit{ 13 };
					char ch;

					if (isLetter(key)) {
						ch = 'a' + key;
						if (event.key.shift)
							ch = std::toupper(ch);
						if (name.size() < nameLimit)
							name += ch;
					} else if (isDigit(key)) {
						ch = '0' + (key - sf::Keyboard::Num0);
						if (name.size() < nameLimit)
							name += ch;
					} else if (key == sf::Keyboard::Space) {
						if (name.size() < nameLimit)
							name += ' ';
					} else if (key == sf::Keyboard::Backspace) {
						name.resize(name.size() - 1);
					} else if (key == sf::Keyboard::Enter) {
						if (name.size() > 0) {
							GameState::getInstance().HighScoreTable.addHighScore(name, currentScore.score);
							mode.pop();
						}
					} else if (key == sf::Keyboard::Escape) {
						mode.pop();
					}
				}
				break;
			}
		}
	}

	bool isDigit(sf::Keyboard::Key key)
	{
		return (key >= sf::Keyboard::Num0 && key <= sf::Keyboard::Num9);
	}

	bool isLetter(sf::Keyboard::Key key)
	{
		return (key >= sf::Keyboard::A && key <= sf::Keyboard::Z);
	}

	void rotate(bool ccw = true)
	{
		currentShape.rotate(ccw);

		if (!canMove(currentShape.getBlockPositions()))
			currentShape.revertState();
		else
			GameState::getInstance().Sound.playSoundEffect(SoundEffect::Rotate);
	}

	ScreensEnum run()
	{
		GameState::getInstance().Sound.stopMenuMusic();
		GameState::getInstance().Sound.playBackgroundMusic();

		GameState::getInstance().Window.setMouseCursorVisible(false);
		while (!endGame) {
			resetGame();
			GameLoop(GameState::getInstance().Window);
		}
		endGame = false;

		GameState::getInstance().Sound.stopBackgroundMusic();
		GameState::getInstance().Window.setMouseCursorVisible(true);

		return ScreensEnum::Menu;
	}

	void resetGame()
	{
		name = "";
		mode.push(GameMode::Running);
		movement.reset();
		gameOverText.setVisible(false);
		addedHighScore = false;
		currentScore.score = 0;
		currentLevel.reset();
		linesCleared = 0;
		newShapes();
		grid.makeAllRowsVisible();
		grid.clear();
	}

	void pauseGame()
	{
		GameState::getInstance().Sound.pauseBackgroundMusic();
		GameState::getInstance().Sound.playSoundEffect(SoundEffect::Pause);
		if (mode.top() == GameMode::Paused) {
			mode.pop();
			grid.makeAllRowsVisible();
		} else {
			mode.push(GameMode::Paused);
			makeRoomForText();
		}
		pausedText.toggleVisible();
	}
};


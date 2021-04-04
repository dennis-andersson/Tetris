#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include "Screen.hpp"
#include "Sound.hpp"
#include "Game.hpp"
#include "TextElement.hpp"
#include "Input.hpp"
#include "Utils.hpp"
#include "GameState.hpp"

class MenuScreen : public Screen
{
private:
	sf::FloatRect menuOptionsBoundingBox;
	sf::Color selectedOptionColor{ sf::Color::Yellow };
	sf::Color nonSelectedOptionColor{ sf::Color::White };
	static const int numberOfOptions{ 4 };
	std::array<ScreensEnum, 4> choices{ ScreensEnum::LevelSelection, ScreensEnum::HighScores, ScreensEnum::Credits, ScreensEnum::None };
	ScreensEnum chosenScreen;
	bool selectedOption{ false };
	int textSize{ 26 };
	int option{ 0 };
	std::array<TextElement, MenuScreen::numberOfOptions> options;
	TextElement title;
public:
	MenuScreen()
	{
		int Width{ GameState::getInstance().WindowWidth };
		int Height{ GameState::getInstance().WindowHeight };
		std::array<sf::Vector2f, numberOfOptions> positions;

		title.init(std::string("TETRIS"), sf::Vector2f(Width / 2, Height * 0.1f), textSize * 3, nonSelectedOptionColor, sf::Text::Bold);
		title.setOriginToCenter();

		for (int i = 0; i < numberOfOptions; ++i) {
			positions[i].x = Width / 2;
			positions[i].y = (Height / 2) + (i * (textSize + 9));
		}

		options[0].init("Play", positions[0], textSize, nonSelectedOptionColor, sf::Text::Regular);
		options[1].init("High scores", positions[1], textSize, nonSelectedOptionColor, sf::Text::Regular);
		options[2].init("Credits", positions[2], textSize, nonSelectedOptionColor, sf::Text::Regular);
		options[3].init("Quit", positions[3], textSize, nonSelectedOptionColor, sf::Text::Regular);

		for (int i = 0; i < numberOfOptions; ++i)
			options[i].setOriginToCenter();

		setSelectedOption(option);

		menuOptionsBoundingBox = getMenuBoundingBox();
	}

	void nextSelectedOption(int n)
	{
		options[option].setColor(nonSelectedOptionColor);
		option = (option + n) % numberOfOptions;
		if (option < 0)
			option = numberOfOptions - 1;
		options[option].setColor(selectedOptionColor);
	}

	void setSelectedOption(int n)
	{
		options[option].setColor(nonSelectedOptionColor);
		option = n;
		options[option].setColor(selectedOptionColor);
	}

	void newScreen()
	{
		chosenScreen = choices[option];
		selectedOption = true;
	}

	void processInput(sf::RenderWindow& window)
	{
		float dpadY;
		float limit{ 15.f };
		float delta{ 0.2f };
		float leftStickY;

		sf::Event event;

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Up:
					nextSelectedOption(-1);
					break;
				case sf::Keyboard::Down:
					nextSelectedOption(1);
					break;
				case sf::Keyboard::Enter:
					newScreen();
					return;
					break;
				case sf::Keyboard::S:
					screenshot(window, "screenshot.png");
					break;
				}
				break;
			case sf::Event::MouseMoved:
				mouseMove(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
					mouseClick(event.mouseButton.x, event.mouseButton.y);
				break;
			case sf::Event::JoystickButtonPressed:
				if (gamepadButton(event.joystickButton.button) == GamepadButtons::A)
					newScreen();
				break;
			case sf::Event::JoystickMoved:
				dpadY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);

				if (dpadY > 0.5f)	nextSelectedOption(-1);
				if (dpadY < -0.5f)	nextSelectedOption(1);

				leftStickY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

				if (leftStickY < -limit)	nextSelectedOption(-1);
				if (leftStickY > limit)		nextSelectedOption(1);
				break;
			case sf::Event::Closed:
				chosenScreen = ScreensEnum::None;
				selectedOption = true;
				break;
			case sf::Event::Resized:
				menuOptionsBoundingBox = getMenuBoundingBox();
				break;
			}
		}
	}

	sf::FloatRect getMenuBoundingBox()
	{
		sf::FloatRect boundingBox{ options[0].getBoundingBox() };

		if (options.size() > 1) {
			float minx{ boundingBox.left };
			float miny{ boundingBox.top };
			float maxx{ boundingBox.left + boundingBox.width };
			float maxy{ boundingBox.left + boundingBox.height };

			for (int i = 1; i < options.size(); i++) {
				sf::FloatRect box{ options[i].getBoundingBox() };
				float x{ box.left + box.width };
				float y{ box.top + box.height };

				if (box.left < minx)	minx = box.left;
				if (box.top < miny)		miny = box.top;
				if (maxx < x)			maxx = x;
				if (maxy < y)			maxy = y;
			}

			boundingBox.left	= minx;
			boundingBox.top		= miny;
			boundingBox.width	= maxx - minx;
			boundingBox.height	= maxy - miny;
		}

		return boundingBox;
	}

	void mouseMove(const int x, const int y)
	{
		// Figure out if the mouse pointer is hovering over a menu option
		if (menuOptionsBoundingBox.contains(x, y)) {
			for (int i = 0; i < options.size(); ++i) {
				if (options[i].getBoundingBox().contains(x, y)) {
					setSelectedOption(i);
					return;
				}
			}
		}
	}

	void mouseClick(int x, int y)
	{
		// Figure out if the user clicked on a menu option
		if (getMenuBoundingBox().contains(x, y)) {
			for (int i = 0; i < options.size(); ++i) {
				if (options[i].getBoundingBox().contains(x, y)) {
					newScreen();
					return;
				}
			}
		}
	}

	void drawBoundingBoxes(sf::RenderWindow& window)
	{
		for (int i = 0; i < numberOfOptions; ++i) {
			sf::FloatRect boundingBox{ options[i].getBoundingBox() };
			sf::Vector2f position{ boundingBox.left, boundingBox.top };
			sf::Vector2f size{ boundingBox.width, boundingBox.height };
			sf::RectangleShape rect{ size };
			rect.setPosition(position);
			rect.setOutlineThickness(2.f);
			rect.setOutlineColor(sf::Color::Red);
			rect.setFillColor(sf::Color::Transparent);
			window.draw(rect);
		}

		sf::FloatRect boundingBox{ getMenuBoundingBox() };
		sf::Vector2f position{ boundingBox.left, boundingBox.top };
		sf::Vector2f size{ boundingBox.width, boundingBox.height };
		sf::RectangleShape rect{ size };
		rect.setPosition(position);
		rect.setOutlineThickness(2.f);
		rect.setOutlineColor(sf::Color::Red);
		rect.setFillColor(sf::Color::Transparent);
		window.draw(rect);
	}

	void render(sf::RenderWindow& window)
	{
		window.clear();

		drawBackground(window);

		title.draw(window);

		for (int i = 0; i < numberOfOptions; ++i)
			options[i].draw(window);

		if (false)
			drawBoundingBoxes(window);

		window.display();
	}

	ScreensEnum run()
	{
		GameState::getInstance().Sound.playMenuMusic();

		while (GameState::getInstance().Window.isOpen()) {
			processInput(GameState::getInstance().Window);

			if (selectedOption) {
				selectedOption = false;
				return chosenScreen;
			}

			render(GameState::getInstance().Window);
		}

		return ScreensEnum::None;
	}
};


#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "Game.hpp"
#include "TextElement.hpp"
#include <array>
#include "Input.hpp"
#include "Background.hpp"

class MenuScreen : public Screen
{
private:
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
	Background background;
	int Width, Height;
public:
	MenuScreen(sf::RenderWindow& window, Background& Background) : background(Background)
	{
		sf::Vector2u windowSize = window.getSize();
		std::array<sf::Vector2f, numberOfOptions> positions;
		Width = windowSize.x;
		Height = windowSize.y;

		for (int i = 0; i < numberOfOptions; ++i) {
			positions[i].x = Width / 2;
			positions[i].y = (Height / 2) + (i * (textSize + 9));
		}

		options[0].init(std::string("Play"), positions[0], textSize, selectedOptionColor, sf::Text::Regular);
		options[1].init(std::string("High scores"), positions[1], textSize, nonSelectedOptionColor, sf::Text::Regular);
		options[2].init(std::string("Credits"), positions[2], textSize, nonSelectedOptionColor, sf::Text::Regular);
		options[3].init(std::string("Quit"), positions[3], textSize, nonSelectedOptionColor, sf::Text::Regular);

		title.init(std::string("TETRIS"), sf::Vector2f(Width / 2, Height * 0.1f), textSize * 3, nonSelectedOptionColor, sf::Text::Bold);
		title.setOriginToCenter();

		for (int i = 0; i < numberOfOptions; ++i)
			options[i].setOriginToCenter();
	}

	void newOption(int n)
	{
		options[option].setColor(nonSelectedOptionColor);
		option = (option + n) % numberOfOptions;
		if (option < 0)
			option = numberOfOptions - 1;
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
		float Y;

		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				chosenScreen = ScreensEnum::None;
				selectedOption = true;
			} else if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Up:
					newOption(-1);
					break;
				case sf::Keyboard::Down:
					newOption(1);
					break;
				case sf::Keyboard::Enter:
					newScreen();
					return;
					break;
				}
			}

			if (sf::Joystick::isConnected(0)) {
				bool buttonPressed = gamepadButtonPressed();

				if (buttonPressed) {
					newScreen();
					return;
				}

				dpadY = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::PovY);

				if (dpadY > 0.5f)	newOption(-1);
				if (dpadY < -0.5f)	newOption(1);

				Y = sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y);

				if (Y < -limit)	newOption(-1);
				if (Y > limit)	newOption(1);
			}
		}
	}

	void render(sf::RenderWindow& window)
	{
		window.clear();

		background.draw(window);
		title.draw(window);

		for (int i = 0; i < numberOfOptions; ++i)
			options[i].draw(window);

		window.display();
	}

	ScreensEnum run(sf::RenderWindow& window)
	{
		while (window.isOpen()) {
			processInput(window);

			if (selectedOption) {
				selectedOption = false;
				return chosenScreen;
			}

			render(window);
		}

		return ScreensEnum::None;
	}
};


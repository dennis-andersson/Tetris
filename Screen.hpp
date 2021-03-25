#pragma once

#include "ScreensEnum.hpp"

class Screen
{
private:
	bool goBack{ false };
public:
	void processInput(sf::RenderWindow& window)
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed ||
				event.type == sf::Event::JoystickButtonPressed) {
				goBack = true;
				return;
			}
		}
	}

	void update(const sf::Time& deltaTime) {};

	void render(sf::RenderWindow& window)
	{
		window.clear();
		window.display();
	}

	ScreensEnum run(sf::RenderWindow& window)
	{
		while (window.isOpen()) {
			processInput(window);

			if (goBack)
				return ScreensEnum::Menu;

			render(window);
		}
	}
};


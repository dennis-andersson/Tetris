#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "Game.hpp"
#include "Background.hpp"

class LevelSelectionScreen : public Screen
{
private:
	bool goBack{ false };
	Background background;
public:
	LevelSelectionScreen(sf::RenderWindow& window, Background& Background) : background(Background)
	{
	}

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

	void render(sf::RenderWindow& window)
	{
		window.clear();
		window.display();
	}

	ScreensEnum run(sf::RenderWindow& window)
	{
		return ScreensEnum::Play;
		//while (window.isOpen()) {
		//	processInput(window);

		//	if (goBack)
		//		return ScreensEnum::Menu;

		//	render(window);
		//}
	}
};


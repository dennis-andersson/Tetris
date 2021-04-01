#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "Game.hpp"

class LevelSelectionScreen : public Screen
{
private:
	bool goBack{ false };
public:
	LevelSelectionScreen(sf::RenderWindow& window) : Screen(window)
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
		drawBackground(window);
		window.display();
	}

	ScreensEnum run(sf::RenderWindow& window)
	{
		Sound::GetInstance().stopMenuMusic();
		return ScreensEnum::Play;
		//while (window.isOpen()) {
		//	processInput(window);

		//	if (goBack)
		//		return ScreensEnum::Menu;

		//	render(window);
		//}
	}
};


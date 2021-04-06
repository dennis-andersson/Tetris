#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>
#include <string>
#include "Screen.hpp"
#include "Image.hpp"
#include "TextElement.hpp"
#include "ControllerMapScreen.hpp"
#include "ControlsJoypad.hpp"

class ControlsScreen : public Screen
{
private:
	std::array<Image, 2> tabImages;
	ControllerMapScreen keyboardControls;
	ControlsJoypad joypadControls;
	sf::FloatRect tabBoundingBox;
	int currentTab{ 0 };
	bool showKeyboardControls{ true };

	void nextTab()
	{
		currentTab = (currentTab + 1) % 2;
		showKeyboardControls = !showKeyboardControls;
	}
public:
	ControlsScreen()
	{
		// Tabs
		sf::Vector2f tabPosition(124, 30);
		tabImages[0].loadFile("Images/tab-keyboard.png", tabPosition);
		tabImages[1].loadFile("Images/tab-joypad.png", tabPosition);

		sf::Vector2u imageSize = tabImages[0].getSize();
		tabBoundingBox.left = tabPosition.x;
		tabBoundingBox.top = tabPosition.y;
		tabBoundingBox.width = imageSize.x;
		tabBoundingBox.height = imageSize.y;
	}

	~ControlsScreen()
	{
	}

	void processInput(sf::RenderWindow& window)
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				switch (event.key.code) {
				case sf::Keyboard::Right:
				case sf::Keyboard::Left:
					nextTab();
					break;
				default:
					Screen::processInput(event);
				}
				break;
			case sf::Event::MouseButtonPressed:
				if (tabBoundingBox.contains(event.mouseButton.x, event.mouseButton.y))
					nextTab();
				else
					Screen::processInput(event);
				break;
			default:
				Screen::processInput(event);
			}

			if (goBack()) return;
		}
	}

	void render(sf::RenderWindow& window)
	{
		window.clear();

		// Draw stuff here.
		drawBackground(window);
		drawBackButton(window);

		tabImages[currentTab].draw(window);

		if (showKeyboardControls)
			keyboardControls.render(window);
		else
			joypadControls.render(window);

		Screen::render(window);

		window.display();
	}
};


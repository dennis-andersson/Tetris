#pragma once

#include <SFML/Graphics.hpp>
#include "Background.hpp"
#include "ScreensEnum.hpp"
#include "Input.hpp"

class Screen
{
private:
	bool goBack{ false };

	static inline Background background;
	static inline sf::Texture backButtonTexture;
	static inline sf::Sprite goBackButton;
	static inline const sf::Vector2f backButtonPosition{25, 510};
	static inline sf::FloatRect backButtonBoundingBox;
public:
	Screen(sf::RenderWindow& window)
	{
		if (backButtonTexture.getSize().x == 0) {
			backButtonTexture.loadFromFile("Images/back-button.png");
			goBackButton.setTexture(backButtonTexture);
			goBackButton.setPosition(backButtonPosition);
			backButtonBoundingBox.left   = backButtonPosition.x;
			backButtonBoundingBox.top    = backButtonPosition.y;
			backButtonBoundingBox.width  = backButtonTexture.getSize().x;
			backButtonBoundingBox.height = backButtonTexture.getSize().y;
		}

		if (!background.isLoaded()) {
			background.setBackground("Images/background3.jpg");
			background.setSize(window.getSize().x, window.getSize().y);
		}
	}

	void drawBackground(sf::RenderWindow& window)
	{
		background.draw(window);
	}

	void drawBackButton(sf::RenderWindow& window)
	{
		window.draw(goBackButton);
	}

	bool clickOnBackButton(int x, int y)
	{
		return backButtonBoundingBox.contains(x, y);
	}

	virtual void processInput(sf::RenderWindow& window)
	{
		sf::Event event;

		while (window.pollEvent(event)) {
			bool pressedEscape = (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape);
			bool pressedBButton = (event.type == sf::Event::JoystickButtonPressed && (gamepadButton(event.joystickButton.button) == GamepadButtons::B));
			bool clickedOnBackButton = (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left && clickOnBackButton(event.mouseButton.x, event.mouseButton.y));

			goBack = pressedEscape || pressedBButton || clickedOnBackButton;

			if (goBack) return;
		}
	}

	virtual void update(const sf::Time& deltaTime) {};

	virtual void render(sf::RenderWindow& window)
	{
		window.clear();
		window.display();
	}

	ScreensEnum run(sf::RenderWindow& window)
	{
		while (window.isOpen()) {
			processInput(window);

			if (goBack) {
				goBack = false;
				return ScreensEnum::Menu;
			}

			render(window);
		}
	}
};


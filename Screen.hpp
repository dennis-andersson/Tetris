#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "Background.hpp"
#include "ScreensEnum.hpp"
#include "Input.hpp"
#include "GameState.hpp"
#include "TextElement.hpp"
#include "Utils.hpp"

class Screen
{
private:
	bool goBackToMainMenu{ false };

	static inline sf::Texture backButtonTexture;
	static inline sf::Sprite goBackButton;
	static inline const sf::Vector2f backButtonPosition{25, 510};
	static inline sf::FloatRect backButtonBoundingBox;
	static inline TextElement volumeText;
	const float volumeTimerLimit{ 0.7f };
	sf::Time volumeTimer;

	void newVolume(float newVolume)
	{
		GameState::getInstance().Sound.setVolume(newVolume);
		volumeTimer = sf::Time::Zero;
		showVolume();
	}

	void showVolume()
	{
		volumeText.setVisible(true);
	}
public:
	Screen()
	{
		if (backButtonTexture.getSize().x == 0) {
			backButtonTexture.loadFromFile("Images/back-button.png");
			goBackButton.setTexture(backButtonTexture);
			goBackButton.setPosition(backButtonPosition);
			backButtonBoundingBox.left   = backButtonPosition.x;
			backButtonBoundingBox.top    = backButtonPosition.y;
			backButtonBoundingBox.width  = backButtonTexture.getSize().x;
			backButtonBoundingBox.height = backButtonTexture.getSize().y;

			volumeText.init(sf::Vector2f( GameState::getInstance().WindowWidth - 225, GameState::getInstance().WindowHeight - 80 ), 36, sf::Color::Green, sf::Text::Style::Bold);
			volumeText.setVisible(false);
		}
	}

	void drawBackground(sf::RenderWindow& window)
	{
		GameState::getInstance().Background.draw(window);
	}

	void drawBackButton(sf::RenderWindow& window)
	{
		window.draw(goBackButton);
	}

	bool clickOnBackButton(int x, int y)
	{
		return backButtonBoundingBox.contains(x, y);
	}

	bool goBack()
	{
		return goBackToMainMenu;
	}

	virtual void processInput(sf::RenderWindow& window)
	{
		sf::Event event;

		while (window.pollEvent(event))
			processInput(event);
	}

	virtual void processInput(sf::Event event)
	{
		bool pressedEscape{ false };
		bool pressedBButton{ false };
		bool clickedOnBackButton{ false };

		if (event.type == sf::Event::KeyPressed) {
			switch (event.key.code) {
			case sf::Keyboard::Escape:
				pressedEscape = true;
				break;
			case sf::Keyboard::Num9:
				lowerVolume();
				break;
			case sf::Keyboard::Num0:
				raiseVolume();
				break;
			case sf::Keyboard::S:
				screenshot(GameState::getInstance().Window, "screenshot.png");
				break;
			}
		} else if (event.type == sf::Event::JoystickButtonPressed) {
			if (joypadButton(event.joystickButton.button) == JoypadButtons::B)
				pressedBButton = true;
		} else if (event.type == sf::Event::MouseButtonPressed) {
			clickedOnBackButton = (event.mouseButton.button == sf::Mouse::Left && clickOnBackButton(event.mouseButton.x, event.mouseButton.y));
		}

		goBackToMainMenu = pressedEscape || pressedBButton || clickedOnBackButton;
	}

	virtual void update(const sf::Time& deltaTime)
	{
		if (volumeText.isVisible()) {
			volumeTimer += deltaTime;
			if (volumeTimer.asSeconds() > volumeTimerLimit) {
				volumeText.setVisible(false);
				volumeTimer = sf::Time::Zero;
			}
		}
	};

	virtual void render(sf::RenderWindow& window)
	{
		if (volumeText.isVisible()) {
			std::string text{ "Volume: " + std::to_string((int) GameState::getInstance().Sound.getVolume()) };
			volumeText.draw(window, text);
		}
	}

	void lowerVolume()
	{
		newVolume(std::max(GameState::getInstance().Sound.getVolume() - 3, 0.f));
	}

	void raiseVolume()
	{
		newVolume(std::min(GameState::getInstance().Sound.getVolume() + 3, 100.f));
	}

	ScreensEnum run()
	{
		sf::Clock clock;
		sf::Time deltaTime{ sf::Time::Zero };

		while (GameState::getInstance().Window.isOpen()) {
			processInput(GameState::getInstance().Window);

			if (goBackToMainMenu) {
				goBackToMainMenu = false;
				return ScreensEnum::Menu;
			}

			deltaTime = clock.restart();
			update(deltaTime);

			render(GameState::getInstance().Window);
		}
	}
};


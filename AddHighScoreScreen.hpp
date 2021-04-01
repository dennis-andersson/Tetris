#pragma once

#include <SFML/Graphics.hpp>
#include "HighScoreTable.hpp"
#include "Screen.hpp"
#include "TextElement.hpp"

class AddHighScoreScreen : public Screen
{
private:
	HighScoreTable highScores;
	TextElement header;
	std::string headerText{ "Please enter your name" };
	sf::Vector2f headerPosition {59, 141};
	sf::Color textColor{ sf::Color::White };
	sf::Uint8 textStyle{ sf::Text::Style::Regular };
	int textSize{ 36 };
public:
	AddHighScoreScreen(sf::RenderWindow& window, HighScoreTable& HighScores) : Screen(window), highScores(HighScores)
	{
		header.init(headerText, headerPosition, textSize, textColor, textStyle);
	}

	void processInput(sf::RenderWindow& window)
	{
		sf::Event event;
		float step{ 3.f };

		while (window.pollEvent(event)) {
			if (event.type == sf::Event::KeyPressed) {
				switch (event.key.code) {
				case sf::Keyboard::Right:
					headerPosition.x += step;
					break;
				case sf::Keyboard::Left:
					headerPosition.x -= step;
					break;
				case sf::Keyboard::Down:
					headerPosition.y += step;
					break;
				case sf::Keyboard::Up:
					headerPosition.y -= step;
					break;
				}

			}
		}
	}

	void update(const sf::Time& deltaTime)
	{
		header.setPosition(headerPosition);
	}

	void render(sf::RenderWindow& window)
	{
		window.clear();
		header.draw(window);
		drawBackground(window);
		window.display();
	}

	ScreensEnum run(sf::RenderWindow& window)
	{
		sf::Clock clock;

		while (window.isOpen()) {
			processInput(window);
			update(clock.restart());
			render(window);
		}

		return ScreensEnum::HighScores;
	}
};


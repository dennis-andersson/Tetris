#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "GameState.hpp"

class TextElement
{
private:
	sf::Text text;
	bool visible{ true };
public:
	TextElement() {}

	TextElement(sf::Vector2f position, int size, sf::Color color, sf::Uint32 style)
	{
		init(position, size, color, style);
	}

	TextElement(std::string s, sf::Vector2f position, int size, sf::Color color, sf::Uint32 style)
	{
		init(position, size, color, style);
		text.setString(s);
	}

	void init(sf::Vector2f position, int size, sf::Color color, sf::Uint32 style)
	{
		text.setCharacterSize(size);
		text.setPosition(position);
		text.setFillColor(color);
		text.setStyle(style);
		text.setFont(GameState::getInstance().Font);
		visible = true;
	}

	void init(std::string s, sf::Vector2f position, int size, sf::Color color, sf::Uint32 style)
	{
		init(position, size, color, style);
		text.setString(s);
	}

	void draw(sf::RenderWindow &window)
	{
		if (visible)
			window.draw(text);
	}

	void draw(sf::RenderWindow &window, std::string s)
	{
		text.setString(s);
		draw(window);
	}

	void draw(sf::RenderWindow &window, int n)
	{
		std::string s = std::to_string(n);
		text.setString(s);
		draw(window);
	}

	void toggleVisible()
	{
		visible = !visible;
	}

	void setVisible(bool Visible)
	{
		visible = Visible;
	}

	bool isVisible()
	{
		return visible;
	}

	void setColor(sf::Color textColor)
	{
		text.setFillColor(textColor);
	}

	void setOriginToCenter()
	{
		sf::FloatRect bounds = text.getLocalBounds();

		text.setOrigin(bounds.width / 2, bounds.height / 2);
	}

	void setPosition(sf::Vector2f newPosition)
	{
		text.setPosition(newPosition);
	}

	sf::FloatRect getBoundingBox()
	{
		return text.getGlobalBounds();
	}
};


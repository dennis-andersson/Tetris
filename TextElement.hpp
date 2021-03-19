#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class TextElement
{
private:
	sf::Font font;
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
		font.loadFromFile("Fonts/arial.ttf");
		text.setCharacterSize(size);
		text.setPosition(position);
		text.setFillColor(color);
		text.setStyle(style);
		text.setFont(font);
		visible = true;
	}

	void init(std::string s, sf::Vector2f position, int size, sf::Color color, sf::Uint32 style)
	{
		init(position, size, color, style);
		text.setString(s);
	}

	void draw(sf::RenderWindow &window)
	{
		text.setFont(font);
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

	bool isVisible()
	{
		return visible;
	}
};


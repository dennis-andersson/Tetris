#pragma once

#include <SFML/Graphics.hpp>

class Border
{
private:
	sf::RectangleShape rectangle;
public:
	Border()
	{
		rectangle.setFillColor(sf::Color::Black);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setOutlineThickness(3.f);

	}

	void setPosition(sf::Vector2f position)
	{
		rectangle.setPosition(position);
	}

	void setSize(sf::Vector2f size)
	{
		rectangle.setSize(size);
	}

	void draw(sf::RenderWindow &window)
	{
		window.draw(rectangle);
	}
};


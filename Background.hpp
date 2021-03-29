#pragma once

#include <SFML/Graphics.hpp>

class Background
{
public:
	Background() {}

	Background(std::string filename)
	{
		setBackground(filename);
	}

	void setBackground(std::string filename)
	{
		if (!texture.loadFromFile(filename))
			throw std::exception("Couldn't load background image");

		texture.setRepeated(true);
		sprite.setTexture(texture);
	}

	void setSize(int Width, int Height)
	{
		sprite.setTextureRect(sf::IntRect(0, 0, Width, Height));
	}

	void draw(sf::RenderWindow& window)
	{
		window.draw(sprite);
	}
private:
	sf::Texture texture;
	sf::Sprite sprite;
};




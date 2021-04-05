#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Background
{
public:
	Background()
	{
	}

	~Background()
	{

	}

	void setBackground(std::string filename, int Width, int Height)
	{
		if (!texture.loadFromFile(filename))
			throw std::exception("Couldn't load background image");

		texture.setRepeated(true);
		sprite.setTexture(texture);
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




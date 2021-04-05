#pragma once

#include <SFML/Graphics.hpp>

class Image
{
private:
	sf::Texture texture;
	sf::Sprite sprite;
	sf::Vector2f position;
	bool loaded;
public:
	Image()
	{
		loaded = false;
	}

	Image(std::string filename)
	{
		loadFile(filename);
	}

	void loadFile(std::string filename)
	{
		texture.loadFromFile(filename);
		sprite.setTexture(texture);
		loaded = true;
	}

	void loadFile(std::string filename, sf::Vector2f position)
	{
		loadFile(filename);
		sprite.setPosition(position);
	}

	void draw(sf::RenderWindow& window)
	{
		if (!loaded) return;

		window.draw(sprite);
	}

	void setPosition(sf::Vector2f newPosition)
	{
		if (!loaded) return;

		sprite.setPosition(newPosition);
	}

	void setSize(sf::Vector2u newSize)
	{
		if (!loaded) return;

		auto scale = sprite.getScale();

	}

	void setScale(float newScale)
	{
		sprite.setScale(newScale, newScale);
	}
};


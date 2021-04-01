#pragma once

#include <SFML/Graphics.hpp>
#include <string>

void screenshot(sf::RenderWindow& window, std::string filename)
{
	sf::Vector2u windowSize = window.getSize();
	sf::Texture texture;
	texture.create(windowSize.x, windowSize.y);
	texture.update(window);
	sf::Image image = texture.copyToImage();
	image.saveToFile(filename);
}

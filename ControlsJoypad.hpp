#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>
#include <string>
#include "Screen.hpp"
#include "Image.hpp"
#include "TextElement.hpp"

class ControlsJoypad : public Screen
{
private:
	std::array<Image, 13> images;
	std::array<TextElement, 6> texts;
	int textSize{ 23 };
	sf::Color textColor{ sf::Color::White };
	sf::Uint8 textStyle{ sf::Text::Style::Regular };
public:
	ControlsJoypad()
	{
		sf::Vector2f position{ 340, 248 };
		int skip{ 40 };
		int i{ 0 }, column{ 0 }, row{ 0 };

		// Controller
		sf::Vector2f controllerPosition(117, 90);
		images[i++].loadFile("Images/xbox controller.png", controllerPosition);

		// Move the shape
		images[i++].loadFile("Images/left-stick.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// Left stick
		column++;
		images[i++].loadFile("Images/xbox-down.png", sf::Vector2f(position.x + 5 + (column * skip), position.y + 7 + (row * skip)));		// Down
		column++;
		images[i++].loadFile("Images/xbox-left.png", sf::Vector2f(position.x + 5 + (column * skip), position.y + 6 + (row * skip)));		// Left
		column++;
		images[i++].loadFile("Images/xbox-right.png", sf::Vector2f(position.x + 5 + (column * skip), position.y + 6 + (row * skip)));		// Right
		column = 0;
		row++;

		// Hard drop
		images[i++].loadFile("Images/xbox-y.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// Y
		row++;

		// Rotate Clockwise
		images[i++].loadFile("Images/xbox-a.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// A
		column++;
		images[i++].loadFile("Images/xbox-b.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// B
		column = 0;
		row++;

		// Rotate Counterclockwise
		images[i++].loadFile("Images/xbox-x.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// X
		column = 0;
		row++;

		// Pause
		images[i++].loadFile("Images/xbox-menu.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// Menu
		column++;
		images[i++].loadFile("Images/xbox-view.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// View
		column = 0;
		row++;

		images[i++].loadFile("Images/xbox-b.png", sf::Vector2f(position.x + (column * skip), position.y + (row * skip)));		// B

		position.x = 30;
		position.y = 210;
		i = 0;

		texts[i++].init("Move the shape", sf::Vector2f{ position.x, position.y + (i * skip) }, textSize, textColor, textStyle);
		texts[i++].init("Hard drop", sf::Vector2f{ position.x, position.y + (i * skip) }, textSize, textColor, textStyle);
		texts[i++].init("Rotate Clockwise", sf::Vector2f{ position.x, position.y + (i * skip) }, textSize, textColor, textStyle);
		texts[i++].init("Rotate Counterclockwise", sf::Vector2f{ position.x, position.y + (i * skip) }, textSize, textColor, textStyle);
		texts[i++].init("Pause", sf::Vector2f{ position.x, position.y + (i * skip) }, textSize, textColor, textStyle);
		texts[i++].init("Previous menu", sf::Vector2f{ position.x, position.y + (i * skip) }, textSize, textColor, textStyle);
	}

	~ControlsJoypad()
	{
	}

	void render(sf::RenderWindow& window)
	{
		for (Image image : images)
			image.draw(window);

		for (TextElement text : texts)
			text.draw(window);
	}
};


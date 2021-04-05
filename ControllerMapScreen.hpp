#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <array>
#include "Screen.hpp"
#include "Game.hpp"
#include "TextElement.hpp"
#include "Image.hpp"

class ControllerMapScreen : public Screen
{
private:
	TextElement ControllerMap;
	TextElement Up;
	TextElement CtrlUp;
	TextElement Left;
	TextElement Right;
	TextElement Down;
	TextElement CtrlDown;
	TextElement Esc;
	TextElement Pause;
	TextElement Lvl;
	sf::Vector2f position;
	int textSize{ 35 };
	int textSize1{ 23 };
	sf::Color textColor{ sf::Color::White };
	sf::Uint8 textStyle{ sf::Text::Style::Bold };
	sf::Uint8 textStyle1{ sf::Text::Style::Regular };
	std::array<Image, 8> images;

public:
	ControllerMapScreen()
	{
		position.x = GameState::getInstance().WindowWidth / 2;
		position.y = 120;
		ControllerMap.init("Keyboard controls", position, textSize, textColor, textStyle);
		ControllerMap.setOriginToCenter();

		position.x = 30;
		position.y = 180;
		Left.init("Move to the left", position, textSize1, textColor, textStyle1);

		position.y += 40;
		Right.init("Move to the right", position, textSize1, textColor, textStyle1);

		position.y += 40;
		Down.init("Move Down", position, textSize1, textColor, textStyle1);

		position.y += 40;
		CtrlDown.init("Hard drop", position, textSize1, textColor, textStyle1);

		position.y += 40;
		Up.init("Rotate Clockwise", position, textSize1, textColor, textStyle1);

		position.y += 40;
		CtrlUp.init("Rotate Counterclockwise", position, textSize1, textColor, textStyle1);

		position.y += 40;
		Esc.init("Quit game/Previous menu", position, textSize1, textColor, textStyle1);

		position.y += 40;
		Pause.init("Pause", position, textSize1, textColor, textStyle1);

		int x = GameState::getInstance().WindowWidth - 160;
		int i{ 0 };
		images[i++].loadFile("Images/arrow-left.png",		sf::Vector2f(x, 140 + (i * 40)));
		images[i++].loadFile("Images/arrow-right.png",		sf::Vector2f(x, 140 + (i * 40)));
		images[i++].loadFile("Images/arrow-down.png",		sf::Vector2f(x, 140 + (i * 40)));
		images[i++].loadFile("Images/ctrl-arrow-down.png",	sf::Vector2f(x, 140 + (i * 40)));
		images[i++].loadFile("Images/arrow-up.png",			sf::Vector2f(x, 140 + (i * 40)));
		images[i++].loadFile("Images/ctrl-arrow-up.png",	sf::Vector2f(x, 140 + (i * 40)));
		images[i++].loadFile("Images/escape.png",			sf::Vector2f(x, 140 + (i * 40)));
		images[i++].loadFile("Images/P.png",				sf::Vector2f(x, 140 + (i * 40)));
	}

	~ControllerMapScreen()
	{

	}

	void render(sf::RenderWindow& window)
	{
		ControllerMap.draw(window);
		Up.draw(window);
		CtrlUp.draw(window);
		Left.draw(window);
		Right.draw(window);
		Down.draw(window);
		CtrlDown.draw(window);
		Esc.draw(window);
		Pause.draw(window);

		for (Image image : images)
			image.draw(window);
	}
};



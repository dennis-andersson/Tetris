#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.hpp"
#include "Game.hpp"
#include "TextElement.hpp"
#include <string>

class ControllerMapScreen : public Screen
{
private:
	TextElement ControllerMap;
	TextElement Up;
	TextElement Left;
	TextElement Right;
	TextElement Down;
	TextElement Esc;
	TextElement Puse;
	TextElement Lvl;
	sf::Vector2f position;
	int textSize{ 50 };
	int textSize1{ 30 };
	sf::Color textColor{ sf::Color::White };
	sf::Uint8 textStyle{ sf::Text::Style::Bold };
	sf::Uint8 textStyle1{ sf::Text::Style::Regular };

public:
	ControllerMapScreen(sf::RenderWindow& window) : Screen(window)
	{
		position.x = window.getSize().x / 2;
		position.y = 200;
		ControllerMap.init("Controller Map", position, textSize, textColor, textStyle);
		ControllerMap.setOriginToCenter();

		position.y = 100;
		Left.init("Direction  to the left         =  \n", position, textSize1, textColor, textStyle1);
		position.y = position.y + 40;

		position.y = 100;
		Right.init("Direction  to the right       =  \n", position, textSize1, textColor, textStyle1);
		position.y = position.y + 40;

		position.y = 100;
		Up.init("Rotate Shape                     =  \n", position, textSize1, textColor, textStyle1);
		position.y = position.y + 40;

		position.y = 100;
		Down.init("Soft Down                      =  \n", position, textSize1, textColor, textStyle1);
		position.y = position.y + 40;

		position.y = 100;
		Esc.init("Escape / out                    =  \n", position, textSize1, textColor, textStyle1);
		position.y = position.y + 40;

		position.y = 100;
		Puse.init("Pause                          =  \n", position, textSize1, textColor, textStyle1);
		position.y = position.y + 40;

		position.y = 100;
		Lvl.init("Next lvl                        =  \n", position, textSize1, textColor, textStyle1);
		position.y = position.y + 40;

	}

	~ControllerMapScreen()
	{

	}
};



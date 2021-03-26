#pragma once

#include <SFML/Graphics.hpp>

bool gamepadButtonPressed()
{
	unsigned int buttonCount = sf::Joystick::getButtonCount(0);
	bool buttonPressed{ false };
	for (int i = 0; i < sf::Joystick::getButtonCount(0); ++i)
		buttonPressed |= sf::Joystick::isButtonPressed(0, i);
	return buttonPressed;
}


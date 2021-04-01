#pragma once

#include <SFML/Graphics.hpp>

enum class GamepadButtons
{
	A, B, X, Y, LeftShoulder, RightShoulder, ViewB, MenuB, LeftStick, RightStick
};

GamepadButtons gamepadButton(unsigned int button)
{
	const int numberOfButtons{ 10 };
	const GamepadButtons buttons[numberOfButtons]{ GamepadButtons::A,
												   GamepadButtons::B,
												   GamepadButtons::X,
												   GamepadButtons::Y,
												   GamepadButtons::LeftShoulder,
												   GamepadButtons::RightShoulder,
												   GamepadButtons::ViewB,
												   GamepadButtons::MenuB,
												   GamepadButtons::LeftStick,
												   GamepadButtons::RightStick };

	return buttons[button];
}


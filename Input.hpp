#pragma once

#include <SFML/Graphics.hpp>
#include "Direction.hpp"

enum class InputSource
{
	None, Keyboard, Joystick
};

enum class InputType
{
	Stick, Dpad
};

struct Movement
{
	Direction direction;
	InputSource source;
	InputType type;
	sf::Keyboard::Key key;
	sf::Joystick::Axis axis;
	float elapsedTime;
	float movementPerSecond{14.f};

	void update(float deltaTime)
	{
		elapsedTime += deltaTime;
	}

	bool readyToMove()
	{
		if ((elapsedTime * movementPerSecond) >= 1) {
			elapsedTime = 0.f;
			return true;
		} else {
			return false;
		}
	}

	Movement()
	{
		elapsedTime = 0.f;
		reset();
	}

	void reset()
	{
		direction = Direction::Down;
		source = InputSource::None;
	}
};

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


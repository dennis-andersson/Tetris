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

enum class JoypadButtons
{
	A, B, X, Y, LeftShoulder, RightShoulder, ViewB, MenuB, LeftStick, RightStick
};

JoypadButtons joypadButton(unsigned int button)
{
	const int numberOfButtons{ 10 };
	const JoypadButtons buttons[numberOfButtons]{ JoypadButtons::A,
												   JoypadButtons::B,
												   JoypadButtons::X,
												   JoypadButtons::Y,
												   JoypadButtons::LeftShoulder,
												   JoypadButtons::RightShoulder,
												   JoypadButtons::ViewB,
												   JoypadButtons::MenuB,
												   JoypadButtons::LeftStick,
												   JoypadButtons::RightStick };

	return buttons[button];
}


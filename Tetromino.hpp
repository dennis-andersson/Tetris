#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include "Game.hpp"
#include "Direction.hpp"

/*
 *  Block Shape 4 x 4
 *  [ 0   1   2   3 ]
 *  [ 4   5   6   7 ]
 *  [ 8   9   10  11]
 *  [ 12  13  14  15]
 */
const int blockTypes{ 7 };
const int blockLength{ 4 };

enum BlockNames
{
	Square = 0, ReverseL, Z, ReverseZ, T, L, I
};
	
unsigned int BlockInfo[blockTypes][blockLength] = {
	{4, 5,  8,  9},     // Square
	{5, 9, 13,  12},    // Reverse-L
	{5, 9, 10,  14},    // Z
	{5, 9,  8,  12},    // Reverse-Z
	{5, 8,  9,  10},    // T
	{5, 9,  13, 14},    // L
	{1, 5,  9,  13},    // I
};

class Tetromino
{
	int id;
	int currentRotation{ 0 };
	bool visible{ true };
	sf::Sprite sprite;
	sf::Vector2i position;
	std::array<sf::Vector2i, blockLength> block;
	std::array<sf::Vector2i, blockLength> oldBlock;
	sf::Vector2i offset{ 0, 0 };
public:
	void draw(sf::RenderWindow& window)
	{
		if (!visible) return;

		for (int i = 0; i < 4; ++i) {
			sprite.setPosition(offset.x + (block[i].x * 18) + (position.x * 18), offset.y + (block[i].y * 18) + (position.y * 18));
			window.draw(sprite);
		}
	}

	std::array<sf::Vector2i, 4> getBlockPositions() const
	{
		return getBlockPositions(block, position);
	}

	std::array<sf::Vector2i, 4> getBlockPositions(const std::array<sf::Vector2i, 4> Block, const sf::Vector2i& Position) const
	{
		std::array<sf::Vector2i, 4> blockPositions;

		for (int i = 0; i < 4; ++i)
			blockPositions[i] = sf::Vector2i{ Block[i].x + Position.x, Block[i].y + Position.y };

		return blockPositions;
	}

	std::array<sf::Vector2i, 4> getFutureBlockPositions(Direction direction)
	{
		sf::Vector2i tempPosition{ position };
		move(tempPosition, direction);
		return getBlockPositions(block, tempPosition);
	}

	void move(const Direction direction)
	{
		move(position, direction);
	}

	void move(sf::Vector2i& Position, const Direction direction)
	{
		switch (direction) {
		case Direction::Left:
			Position.x--;
			break;
		case Direction::Right:
			Position.x++;
			break;
		default:
			Position.y++;
		}
	}

	void rotate(bool counterClockwise = false)
	{
		//store state of Block in case rotation turns out to be invalid
		oldBlock = block;

		if (id == BlockNames::Square) // square: no need for rotation
			return;

		if (id == BlockNames::I) { // I: restrict "rotation" to two states (horizontal/vertical)
			currentRotation++;
			for (auto i = 0; i < 4; ++i) {
				sf::Vector2i oldPoint = block[i];    //pivot
				sf::Vector2i localVector = oldPoint - sf::Vector2i{ 1, 2 };
				sf::Vector2i nextPoint{};
				if (currentRotation % 2 == 1) {
					/* counter-clockwise
					 * [0  -1]
					 * [-1  0]*/
					nextPoint = sf::Vector2i{ (0 * localVector.x) + (-1 * localVector.y), (1 * localVector.x) + (0 * localVector.y) };
				} else {
					nextPoint = sf::Vector2i{ (0 * localVector.x) + (1 * localVector.y), (-1 * localVector.x) + (0 * localVector.y) };
				}
				block[i] = sf::Vector2i{ 1,2 } + nextPoint;
			}
			return;
		}

		for (auto i = 0; i < 4; ++i) {
			sf::Vector2i oldPoint = block[i];    //pivot
			sf::Vector2i localVector = oldPoint - sf::Vector2i{ 1,2 };   // 1, 1

			/* Clockwise Rotation Matrix
			 * [cos Degree    -sin Degree]
			 * [sin Degree     cos Degree]
			 * translates to clockwise
			 * [0   -1]
			 * [1    0]
			 *
			 * Counterclockwise Rotation Matrix
			 * [cos Degree     sin Degree]
			 * [-sin Degree    cos Degree]
			 * translates to counterclockwise
			 * [0    1]
			 * [-1   0]
			 */

			sf::Vector2i nextPoint;
			if (counterClockwise) {
				nextPoint.x = (0 * localVector.x) + (1 * localVector.y);
				nextPoint.y = (-1 * localVector.x) + (0 * localVector.y);
			} else {
				nextPoint.x = (0 * localVector.x) + (-1 * localVector.y);
				nextPoint.y = (1 * localVector.x) + (0 * localVector.y);
			}

			block[i] = sf::Vector2i{ 1,2 } + nextPoint;
		}
	}

	void revertState()
	{
		block = oldBlock;
	}

	template <class T>
	void setOffset(sf::Vector2<T>& Offset)
	{
		offset.x = (int) Offset.x;
		offset.y = (int) Offset.y;
	}

	void setPosition(const sf::Vector2i& Position)
	{
		position = Position;
	}

	void setBlocksSprite(sf::Sprite& Sprite)
	{
		sprite = Sprite;
	}

	int getId()
	{
		return id;
	}

	void newShape(int Id)
	{
		currentRotation = 0;
		id = Id;

		for (int i = 0; i < blockLength; ++i) {
			block[i].x = BlockInfo[id][i] % blockLength;
			block[i].y = BlockInfo[id][i] / blockLength;
		}
	}

	void setVisible(bool flag)
	{
		visible = flag;
	}

	void toggleVisible()
	{
		visible = !visible;
	}

	bool isVisible()
	{
		return visible;
	}
};




#pragma once

#include <vector>

struct Cell
{
	int id;
	bool empty;

	Cell(int Id)
	{
		id = Id;
		empty = false;
	}

	Cell()
	{
		empty = true;
	}
};

class Grid
{
private:
	std::vector<int> linesToRemove;
	std::vector<std::vector<Cell>> state;
	std::vector<bool> visibleLines;
	int width, height;
	float elapsedTime{ 0.f };

	void initRow(int row, int Width)
	{
		for (int column = 0; column < Width; ++column)
			state[row].push_back(Cell());
	}
public:
	void setDimensions(const int Width, const int Height)
	{
		width = Width;
		height = Height;

		for (int row = 0; row < Height; ++row) {
			state.push_back(std::vector<Cell>());
			initRow(row, Width);
			visibleLines.push_back(true);
		}
	}

	void clear()
	{
		for (int row = 0; row < height; ++row)
			for (int column = 0; column < width; ++column)
				state[row][column].empty = true;
	}

	bool isEmpty(int row, int column)
	{
		return (state[row][column].empty);
	}

	Cell& getCell(int row, int column)
	{
		return state[row][column];
	}

	void addBlock(int id, std::array<sf::Vector2i, 4> block)
	{
		for (int i = 0; i < 4; ++i) {
			int row = block[i].y;
			int col = block[i].x;
			state[row][col].id = id;
			state[row][col].empty = false;
		}
	}

	int markLinesForRemoval()
	{
		if (aboutToRemoveLines())
			return linesToRemove.size();

		for (int row = height - 1; row >= 0; --row) {
			bool removeLine{ true };
			for (Cell& cell : state[row]) {
				if (cell.empty) {
					removeLine = false;
					break;
				}
			}

			if (removeLine)
				linesToRemove.push_back(row);
		}

		return linesToRemove.size();
	}

	void cleanLines()
	{
		if (linesToRemove.empty()) return;

		for (auto i : linesToRemove)
			state.erase(state.begin() + i);

		for (int i = 0; i < linesToRemove.size(); ++i) {
			state.insert(state.begin(), std::vector<Cell>());
			initRow(0, width);
		}

		for (int i = 0; i < visibleLines.size(); ++i)
			visibleLines[i] = true;

		linesToRemove.clear();
	}

	inline bool aboutToRemoveLines() const
	{
		return !linesToRemove.empty();
	}

	void update(const sf::Time& deltaTime)
	{
		markLinesForRemoval();
		if (aboutToRemoveLines()) {
			elapsedTime += deltaTime.asSeconds();
			blink();
			if (elapsedTime > 0.6f) {
				elapsedTime = 0.f;
				cleanLines();
			}
		}
	}

	void blink()
	{
		// speeds up blinking
		int num = int(elapsedTime * 5.f);

		for (auto i : linesToRemove)
			setVisible(i, (num % 2 != 0));
	}

	bool isVisible(int row)
	{
		return visibleLines[row];
	}

	void setVisible(int row, bool visible)
	{
		visibleLines[row] = visible;
	}

	void makeAllRowsVisible()
	{
		for (int i = 0; i < visibleLines.size(); ++i)
			setVisible(i, true);
	}
};

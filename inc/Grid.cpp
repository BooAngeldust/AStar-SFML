#include "Grid.h"
#include "Path.h"


#include <iostream>



Grid::Grid(sf::RenderWindow& window, float startX, float startY, int rows, int cols, float nodeSize)
	: mGrid()
	, mNodeSize(nodeSize)
	, mWindow(window)
	, mStartX(startX)
	, mStartY(startY)
	, mFont()
{
	mFont.loadFromFile("fonts/font.otf");

	// Initialize Grid
	for (int col = 0; col < cols; col++)
	{
		mGrid.push_back(std::vector<Node::Ptr>());
		for (int row = 0; row < rows; row++)
		{
			mGrid[col].push_back(std::make_shared<Node>(row, col));
		}
	}

	// Set start and finish node
	mGrid[0][0]->type = NodeType::Start;
	mGrid[mGrid[0].size() - 1][mGrid.size() - 1]->type = NodeType::Finish;

	// Initialize color map
	mColorMap[NodeType::Path] = sf::Color::White;
	mColorMap[NodeType::Barrier] = sf::Color::Black;
	mColorMap[NodeType::Start] = sf::Color::Green;
	mColorMap[NodeType::Finish] = sf::Color::Red;
}

void Grid::drawDebug(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& col : mGrid)
	{
		for (auto& row : col)
		{
			sf::Text nodeText;
			nodeText.setString("f: " + std::to_string(row->f).substr(0, 3) +
				"\ng: " + std::to_string(row->g).substr(0,3) +
				"\nh: " + std::to_string(row->h).substr(0, 3) +
				"\n_p: " + std::to_string(int(row->parent.get())) +
				"\n_s: " + std::to_string(int(row.get())));
			nodeText.setFont(mFont);
			nodeText.setPosition(mStartX + (mNodeSize * row->row), mStartY + (mNodeSize * row->col));
			nodeText.setFillColor(sf::Color::Black);
			nodeText.setCharacterSize(10);


			target.draw(nodeText, states);
		}
	}
}

void Grid::drawPath(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Trace back the path from finish node


}

void Grid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto& col : mGrid)
	{
		for (auto& row : col)
		{
			sf::RectangleShape nodeShape;

			nodeShape.setFillColor(mColorMap.at(row->type));
			
			nodeShape.setPosition(mStartX + (mNodeSize * row->row), mStartY + (mNodeSize * row->col));
			nodeShape.setSize(sf::Vector2f(mNodeSize, mNodeSize));

			target.draw(nodeShape, states);
		}
	}

#ifdef DEBUG
	drawDebug(target, states);
#endif
	drawPath(target, states);
}

void Grid::update(float dt)
{
	// Allow placing walls 
	// Get mouse pos relative to window
	sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);



	// check if mouse is on grid
	if (mousePos.x >= mStartX && mousePos.y >= mStartY &&
		mousePos.x <= mStartX + (mGrid[0].size() * mNodeSize) - 1 &&
		mousePos.y <= mStartY + (mGrid.size() * mNodeSize) - 1)
	{
		// Adjust mousePos relative to grid
		mousePos.x -= mStartX;
		mousePos.y -= mStartY;

		// Calculate which row/col is effected
		int row = mousePos.x / mNodeSize;
		int col = mousePos.y / mNodeSize;

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (mGrid[col][row]->type != NodeType::Start && mGrid[col][row]->type != NodeType::Finish)
				mGrid[col][row]->type = NodeType::Path;
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (mGrid[col][row]->type != NodeType::Start && mGrid[col][row]->type != NodeType::Finish)
				mGrid[col][row]->type = NodeType::Barrier;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			for (auto& col : mGrid)
			{
				for (auto& row : col)
				{
					row->parent = nullptr;
				}
			}

			Path::solve(mGrid, Path::Algorithm::ASTAR);
			
		}
	}
}

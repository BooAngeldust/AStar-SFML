#include "Grid.h"
#include "Path.h"
#include <iostream>

Grid::Grid(sf::RenderWindow& window, float startX, float startY, int rows, int cols, float nodeSize)
	: mGrid()
	, mStartX(startX)
	, mStartY(startY)
	, mNodeSize(nodeSize)
	, mColorMap()
	, mWindow(window)
	, mReadyToDraw(true)
{
	// Initialize color map
	mColorMap[NodeType::Path] = sf::Color::White;
	mColorMap[NodeType::Barrier] = sf::Color::Black;
	mColorMap[NodeType::Start] = sf::Color::Green;
	mColorMap[NodeType::Finish] = sf::Color::Red;

	// Initialize Grid
	for (int col = 0; col < cols; col++)
	{
		mGrid.push_back(std::vector<Node::Ptr>());
		for (int row = 0; row < rows; row++)
		{
			Node::Ptr node = std::make_shared<Node>(row, col);
			node->drawRect.setPosition(startX + (row * nodeSize), startY + (col * nodeSize));
			node->drawRect.setSize(sf::Vector2f(nodeSize, nodeSize));
			// Start node
			if (row == 0 && col == 0)
				node->type = NodeType::Start;

			// Target node
			else if (row == rows-1 && col == cols - 1)
				node->type = NodeType::Finish;

			node->drawRect.setFillColor(mColorMap[node->type]);
			mGrid[col].push_back(std::move(node));
		}
	}
}

void Grid::drawPath()
{
	// Trace back the path from finish node
	Node::Ptr currentNode = nullptr;

	// Find finishNode
	for (auto col : mGrid)
	{
		std::vector<Node::Ptr>::iterator it = std::find_if(col.begin(), col.end(), [](Node::Ptr node) { return node->type == NodeType::Finish;});
		if (it != col.end())
		{
			currentNode = (*it);
			break;
		}
	}

	if (currentNode == nullptr)
		return;

	// Draw every node
	while (currentNode->parent != nullptr)
	{
		sf::RectangleShape drawRect;

		drawRect.setPosition(currentNode->drawRect.getPosition());
		drawRect.setFillColor(sf::Color(100,100,0,100));
		drawRect.setSize(currentNode->drawRect.getSize());

		mWindow.draw(drawRect);

		currentNode = currentNode->parent;
	}
}

void Grid::draw()
{
	if (!mReadyToDraw)
		return;

	mWindow.clear();

	for (auto& col : mGrid)
	{
		for (auto& row : col)
		{
			mWindow.draw(row->drawRect);
		}
	}

	drawPath();

	mWindow.display();
	mReadyToDraw = false;
}

void Grid::reset()
{
	for (auto& col : mGrid)
	{
		for (auto& row : col)
		{
			row->f = 99999999;
			row->g = 0;
			row->h = 0;
			row->parent = nullptr;
			row->neighbours.clear();
		}
	}
}

void Grid::solve()
{
	reset();
	Path::solve(mGrid, Path::Algorithm::ASTAR);
	mReadyToDraw = true;
}

void Grid::update(float dt)
{
	// Allow placing walls 
	// Get mouse pos relative to window
	sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);

	float mouse_x = mousePos.x * (mWindow.getDefaultView().getSize().x / mWindow.getSize().x );
	float mouse_y = mousePos.y * (mWindow.getDefaultView().getSize().y / mWindow.getSize().y);

	// check if mouse is on grid
	if (mouse_x >= mStartX && mousePos.y >= mStartY &&
		mouse_x <= mStartX + (mGrid[0].size() * mNodeSize) - 1 &&
		mouse_y <= mStartY + (mGrid.size() * mNodeSize) - 1)
	{
		// Adjust mousePos relative to grid
		mouse_x -= mStartX;
		mouse_y -= mStartY;

		// Calculate which row/col is effected
		int row = int(mouse_x / mNodeSize);
		int col = int(mouse_y / mNodeSize);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (mGrid[col][row]->type != NodeType::Start && mGrid[col][row]->type != NodeType::Finish && mGrid[col][row]->type != NodeType::Path)
			{
				mGrid[col][row]->type = NodeType::Path;
				mGrid[col][row]->drawRect.setFillColor(mColorMap[NodeType::Path]);

				std::cout << "Changed to path\n";
				mReadyToDraw = true;
			}
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (mGrid[col][row]->type != NodeType::Start && mGrid[col][row]->type != NodeType::Finish && mGrid[col][row]->type != NodeType::Barrier)
			{
				mGrid[col][row]->type = NodeType::Barrier;
				mGrid[col][row]->drawRect.setFillColor(mColorMap[NodeType::Barrier]);

				std::cout << "Changed to barrier\n";
				mReadyToDraw = true;
			}
		}
	}
}

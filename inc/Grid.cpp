#include "Grid.h"
#include "Path.h"

Grid::Grid(sf::RenderWindow& window, float startX, float startY, int rows, int cols, float nodeSize)
	: mGrid()
	, mStartX(startX)
	, mStartY(startY)
	, mNodeSize(nodeSize)
	, mColorMap()
	, mWindow(window)
	, mReadyToDraw(true)
{
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
	mGrid[mGrid.size() - 1][mGrid[0].size() - 1]->type = NodeType::Finish;

	// Initialize color map
	mColorMap[NodeType::Path] = sf::Color::White;
	mColorMap[NodeType::Barrier] = sf::Color::Black;
	mColorMap[NodeType::Start] = sf::Color::Green;
	mColorMap[NodeType::Finish] = sf::Color::Red;
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
		sf::RectangleShape nodeShape;

		nodeShape.setFillColor(sf::Color(0,255,0,100));
		nodeShape.setOutlineThickness(1);
		nodeShape.setOutlineColor(sf::Color::Black);

		nodeShape.setPosition(mStartX + (mNodeSize * currentNode->row), mStartY + (mNodeSize * currentNode->col));
		nodeShape.setSize(sf::Vector2f(mNodeSize, mNodeSize));
		mWindow.draw(nodeShape);

		currentNode = currentNode->parent;
	}
}

void Grid::draw()
{
	if (!mReadyToDraw)
		return;

	mReadyToDraw = false;

	mWindow.clear();

	for (auto& col : mGrid)
	{
		for (auto& row : col)
		{
			sf::RectangleShape nodeShape;

			nodeShape.setFillColor(mColorMap.at(row->type));
			
			nodeShape.setPosition(mStartX + (mNodeSize * row->row), mStartY + (mNodeSize * row->col));
			nodeShape.setSize(sf::Vector2f(mNodeSize, mNodeSize));

			mWindow.draw(nodeShape);
		}
	}

	drawPath();

	mWindow.display();
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
			if (mGrid[col][row]->type != NodeType::Start && mGrid[col][row]->type != NodeType::Finish)
			{
				mGrid[col][row]->type = NodeType::Path;

				solve();
			}
		}
		else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (mGrid[col][row]->type != NodeType::Start && mGrid[col][row]->type != NodeType::Finish)
			{
				mGrid[col][row]->type = NodeType::Barrier;

				solve();
			}
		}
	}
}

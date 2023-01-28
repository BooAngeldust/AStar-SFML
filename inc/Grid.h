#pragma once
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Node.hpp"

class Grid
{
public:
	typedef std::vector<std::vector<Node::Ptr>> NodeGrid;

private:
	NodeGrid mGrid;

	float mStartX;
	float mStartY;

	float mNodeSize;
	std::map<NodeType, sf::Color> mColorMap;

	sf::RenderWindow& mWindow;

	bool mReadyToDraw;

	void drawPath();

	void reset();
	void solve();

public:
	Grid(sf::RenderWindow& window, float startX, float startY, int rows, int cols, float nodeSize);

	void draw();
	void update(float dt);
};
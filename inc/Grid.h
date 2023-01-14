#pragma once
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include "Node.hpp"

class Grid : public sf::Drawable
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
	sf::Font mFont;

	void drawDebug(sf::RenderTarget& target, sf::RenderStates states) const;
	void drawPath(sf::RenderTarget& target, sf::RenderStates states) const;

public:
	Grid(sf::RenderWindow& window, float startX, float startY, int rows, int cols, float nodeSize);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void update(float dt);
};
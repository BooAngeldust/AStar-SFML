#pragma once
#include <memory>
#include <SFML/Graphics.hpp>

enum class NodeType
{
	Path,
	Barrier,
	Start,
	Finish
};

struct Node
{	
	typedef std::shared_ptr<Node> Ptr;

	Ptr parent;

	float g; // how far away the node is from the starting node
	float h; // how far away the node is from the end node
	float f; // Total cost G+H

	int row;
	int col;

	sf::RectangleShape drawRect;

	NodeType type;

	std::vector<Ptr> neighbours;

	Node(int row, int col, NodeType type = NodeType::Path)
		: parent(nullptr)
		, g(0)
		, h(0)
		, f(9999999)
		, row(row)
		, col(col)
		, type(type)
	{
	}
};
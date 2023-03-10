#pragma once
#include "Grid.h"
#include <SFML/System/Clock.hpp>
#include <iostream>

#define LOG_ALGORITHM(name) #name

class Path
{
public:
	enum class Algorithm
	{
		ASTAR,
	};

	enum class Heuristic
	{
		Manhattan,
		Euclidean
	};

private:
	static void solveAStar(Grid::NodeGrid& grid);
	static Node::Ptr findStartNode(Grid::NodeGrid& grid);
	static Node::Ptr findEndNode(Grid::NodeGrid& grid);
	static float findDistance(Node::Ptr a, Node::Ptr b, Heuristic h);

	static Node::Ptr find(Node::Ptr node, std::vector<Node::Ptr> list);

public:
	static void solve(Grid::NodeGrid& grid, Algorithm algorithm = Algorithm::ASTAR);
	
};
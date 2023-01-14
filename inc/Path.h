#pragma once
#include "Grid.h"


class Path
{
public:
	enum class Algorithm
	{
		ASTAR,
	};

private:
	static void solveAStar(Grid::NodeGrid& grid);
	static Node::Ptr findStartNode(Grid::NodeGrid& grid);
	static Node::Ptr findEndNode(Grid::NodeGrid& grid);

public:
	static void solve(Grid::NodeGrid& grid, Algorithm algorithm = Algorithm::ASTAR);
	
};
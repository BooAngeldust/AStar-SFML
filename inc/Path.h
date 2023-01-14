#pragma once
#include "Grid.h"


class Path
{
public:
	enum class Algorithm
	{
		ASTAR,
	};

	enum class Heuristic
	{
		Manhattan
	};

private:
	static void solveAStar(Grid::NodeGrid& grid);
	static Node::Ptr findStartNode(Grid::NodeGrid& grid);
	static Node::Ptr findEndNode(Grid::NodeGrid& grid);
	static float findDistance(Node::Ptr a, Node::Ptr b, Heuristic h);

public:
	static void solve(Grid::NodeGrid& grid, Algorithm algorithm = Algorithm::ASTAR);
	
};
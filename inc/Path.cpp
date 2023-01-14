#include "Path.h"
#include "globals.h"

Node::Ptr Path::findStartNode(Grid::NodeGrid& grid)
{
	for (auto& col : grid)
	{
		for (auto& row : col)
		{
			if (row->type == NodeType::Start)
			{
				return row;
			}
		}
	}
	return nullptr;
}

float Path::findDistance(Node::Ptr a, Node::Ptr b, Heuristic h = Heuristic::Manhattan)
{
	switch (h)
	{
	case Heuristic::Manhattan:
		return std::abs(a->row - b->row) + std::abs(a->col - b->col);
	}
	return 0;
}

Node::Ptr Path::findEndNode(Grid::NodeGrid& grid)
{
	for (auto& col : grid)
	{
		for (auto& row : col)
		{
			if (row->type == NodeType::Finish)
			{
				return row;
			}
		}
	}
	
	return nullptr;
}

void Path::solve(Grid::NodeGrid& grid, Algorithm algorithm)
{
	switch (algorithm)
	{
	case Algorithm::ASTAR:
		solveAStar(grid);
		break;
	default:
		break;
	}
}

void Path::solveAStar(Grid::NodeGrid& grid)
{
	std::vector<Node::Ptr> openList;
	std::vector<Node::Ptr> closedList;

	

	// Start Node
	Node::Ptr startNode = findStartNode(grid);

	// End Node
	Node::Ptr endNode = findEndNode(grid);

	// Add starting node to the open list
	openList.push_back(startNode);
	
	int iteration = 0;


	while (!openList.empty())
	{
		// find node with lowest f value on the openList
		Node::Ptr q = openList[0];

		for (auto& node : openList)
		{
			if (node->f < q->f)
			{
				q = node;
			}
		}

		// Pop q off the openList
		openList.erase(std::find(openList.begin(),openList.end(),q));

		q->neighbours.clear();


		// Left
		if (q->row > 0)
		{
			if (grid[q->col][q->row - 1]->type != NodeType::Barrier)
				q->neighbours.push_back(grid[q->col][q->row - 1]);
		}

		// Right
		if (q->row < grid[0].size() - 1)
		{
			if (grid[q->col][q->row + 1]->type != NodeType::Barrier)
				q->neighbours.push_back(grid[q->col][q->row + 1]);
		}

		// Above
		if (q->col > 0)
		{
			if (grid[q->col - 1][q->row]->type != NodeType::Barrier)
				q->neighbours.push_back(grid[q->col - 1][q->row]);
		}

		// Below
		if (q->col < grid.size() - 1)
		{
			if (grid[q->col + 1][q->row]->type != NodeType::Barrier)
				q->neighbours.push_back(grid[q->col + 1][q->row]);
		}
		

		for (auto& neighbour : q->neighbours)
		{
			if (neighbour->type == NodeType::Finish)
			{
				return;
			}

			float temp_g = q->g + 1;
			float temp_h = findDistance(neighbour, endNode);
			float temp_f = temp_h + temp_g;

			// check if neighbour in openList
			if (std::find(openList.begin(), openList.end(), neighbour) != openList.end())
			{
				if (neighbour->f < temp_f)
					continue;

			}
			else
			{
				neighbour->h = temp_h;
				neighbour->g = temp_g;
				neighbour->f = temp_f;

				openList.push_back(neighbour);
			}
		}

		closedList.push_back(q);
	}
}
#include "Path.h"
#include "globals.h"

Node::Ptr Path::findStartNode(Grid::NodeGrid& grid)
{
	std::vector<Node::Ptr>::iterator it;
	for (auto& col : grid)
	{
		it = std::find_if(col.begin(), col.end(),
			[](Node::Ptr ptr)
			{
				return ptr->type == NodeType::Start;
			});

		if (it != col.end())
			return (*it);
	}

	return nullptr;
}

float Path::findDistance(Node::Ptr a, Node::Ptr b, Heuristic h = Heuristic::Euclidean)
{
	switch (h)
	{
	case Heuristic::Manhattan:
		return std::abs(a->row - b->row) + std::abs(a->col - b->col);
	case Heuristic::Euclidean:
		return std::sqrtf(std::abs(a->row - b->row)* std::abs(a->row - b->row) + std::abs(a->col - b->col)* std::abs(a->col - b->col));
	}
	return 0;
}

Node::Ptr Path::findEndNode(Grid::NodeGrid& grid)
{
	std::vector<Node::Ptr>::iterator it;
	for (auto& col : grid)
	{
		it = std::find_if(col.begin(), col.end(),
			[](Node::Ptr ptr)
			{
				return ptr->type == NodeType::Finish;
			});

		if (it != col.end())
			return (*it);
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

Node::Ptr Path::find(Node::Ptr node, std::vector<Node::Ptr> list)
{
	std::vector<Node::Ptr>::iterator it = std::find(list.begin(), list.end(), node);

	if (it != list.end())
		return (*it);

	return nullptr;
}

void Path::solveAStar(Grid::NodeGrid& grid)
{
	std::vector<Node::Ptr> openList;    // Yet to visit nodes
	std::vector<Node::Ptr> closedList;  // Visited nodes
	

	// Start Node
	Node::Ptr startNode = findStartNode(grid);

	// End Node
	Node::Ptr endNode = findEndNode(grid);

	// Add starting node to the open list
	openList.push_back(startNode);

	while (!openList.empty())
	{
		Node::Ptr currentNode = openList[0];

		// currentNode = node with lowest f score in openList
		for (Node::Ptr node : openList)
		{
			if (node->f < currentNode->f)
				currentNode = node;
		}

		// Pop currentNode from openList
		openList.erase(std::find(openList.begin(),openList.end(),currentNode));

		// Add currentNode to closed list and check it's neighbours
		closedList.push_back(currentNode);
		
		// Find currentNode's neighbours
		currentNode->neighbours.clear();

		// Left 
		if (currentNode->row > 0)
			currentNode->neighbours.push_back(grid[currentNode->col][currentNode->row - 1]);
		// Right
		if (currentNode->row < grid[0].size() - 1)
			currentNode->neighbours.push_back(grid[currentNode->col][currentNode->row + 1]);
		// Below
		if (currentNode->col < grid.size() - 1)
			currentNode->neighbours.push_back(grid[currentNode->col + 1][currentNode->row]);
		// Above
		if (currentNode->col > 0)
			currentNode->neighbours.push_back(grid[currentNode->col - 1][currentNode->row]);

		// Loop through every neighbour
		for (Node::Ptr neighbour : currentNode->neighbours)
		{
			if (neighbour->type == NodeType::Barrier)
				continue;

			float temp_g = currentNode->g + 1;
			float temp_h = findDistance(neighbour, endNode);
			float temp_f = temp_h + temp_g;

			// Check if neighbour in closedList
			if (find(neighbour, closedList) != nullptr)
			{
				if (temp_g < neighbour->g)
				{
					neighbour->g = temp_g;
					neighbour->h = temp_h;
					neighbour->f = temp_f;
					neighbour->parent = currentNode;
				}
			}
			// Check if neighbour in openList
			else if (find(neighbour, openList) != nullptr)
			{
				if (temp_g < neighbour->g)
				{
					neighbour->g = temp_g;
					neighbour->h = temp_h;
					neighbour->f = temp_f;
					neighbour->parent = currentNode;
				}
			}
			// If not in both lists add it to openList and set it's g value
			else
			{
				neighbour->g = temp_g;
				neighbour->h = temp_h;
				neighbour->f = temp_f;
				neighbour->parent = currentNode;
				openList.push_back(neighbour);
			}
		}
		// If currentNode is target node we're done
		if (currentNode->type == NodeType::Finish)
		{
			return;
		}
	}
}
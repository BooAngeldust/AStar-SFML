#include "Path.h"

#ifdef DEBUG
	#include <iostream>
#endif

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

#ifdef DEBUG
	std::cout << "[Path::AStar]: openList.size() = " << openList.size() << "\n";
	std::cout << "[Path::AStar]: openList[0]->type = " << static_cast<int>(openList[0]->type) << "\n";
#endif
	
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
		for (auto it = openList.begin(); it != openList.end(); it++)
		{
#ifdef DEBUG
			std::cout << "[Path::AStar]: (*it == q) = (" << (*it) << " == " << q.get() << ")\n";
#endif
			if (*it == q) 
			{
				openList.erase(it);
				break;
			}
		}

		// Find q's neighbours
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

#ifdef DEBUG
		std::cout << "[Path::AStar]: q->neighbours.size() = " << q->neighbours.size() << "\n";
		for (auto& neighbour : q->neighbours)
		{
			std::cout << "[Path::AStar]: [q] neighbour row/col = " << neighbour->row << "/" << neighbour->col << "\n";
		}
#endif

		// Check every neighbour
		for (auto& neighbour : q->neighbours)
		{
			neighbour->parent = q;

			if (neighbour->type == NodeType::Finish)
			{
				std::cout << "SOLUTION FOUND!\n";
				return;
			}
			
			// Calculate g,h and f
			float temp_g = q->g + 1;
			float temp_h = std::abs(endNode->row - neighbour->row)
				+ std::abs(endNode->col - neighbour->col);

			float temp_f = temp_g + temp_h;

			// Check if node is in openList
			auto openListit = std::find(openList.begin(), openList.end(), neighbour);
			if (openListit != openList.end())
			{
				if (neighbour->f > temp_f)
				{
					neighbour->f = temp_f;
					neighbour->g = temp_g;
					neighbour->h = temp_h;
				}
			}
			else
			{
				auto closedListit = std::find(closedList.begin(), closedList.end(), neighbour);

				if (closedListit != closedList.end())
				{
					if (neighbour->f > temp_f)
					{
						openList.push_back(neighbour);
					}
				}
				else
				{
					openList.push_back(neighbour);
				}
			}
			
		}


		closedList.push_back(q);


#ifdef DEBUG
		std::cout << "[Path::AStar]: q->f = " << q->f << "\n";
		std::cout << "[Path::AStar]: iteration = " << iteration << "\n";

		std::cout << "\n[Path::AStar]: q = " << (int)q.get() << "\n";
		std::cout << "\n[Path::AStar]: q->parent = " << (int)(q->parent.get()) << "\n";
		system("pause");
#endif
	}
}
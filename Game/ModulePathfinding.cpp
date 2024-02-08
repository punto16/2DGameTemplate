#include "App.h"
#include "ModulePathfinding.h"

PathFinding::PathFinding(bool startEnabled) : Module(startEnabled), map(NULL), lastPath(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name = "pathfinding";
}

// Destructor
PathFinding::~PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	lastPath.clear();
	RELEASE_ARRAY(map);

	return true;
}

// Sets up the walkability map
void PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width * height];
	memcpy(map, data, width * height);
}

// Utility: return true if pos is inside the map boundaries
bool PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y * width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::vector<iPoint>* PathFinding::GetLastPath() const
{
	return &lastPath;
}

// To request all tiles involved in the last generated path
void PathFinding::ClearLastPath()
{
	lastPath.clear();
}

PathNode* PathList::Find(const iPoint& point) const
{
	for (auto& item : list)
	{
		if (item.pos == point)
		{
			return (PathNode*)&item;
		}
	}
	return NULL;
}

PathNode* PathList::GetNodeLowestScore() const
{
	int min = 65535;

	const PathNode* ret = NULL;
	//itering in reverse order
	for (auto rit = list.rbegin(); rit != list.rend(); ++rit)
	{
		if ((*rit).Score() < min) 
		{
			min = (*rit).Score();
			ret = &(*rit);
		}
	}

	return (PathNode*)ret;
}

PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList& listToFill) const
{
	iPoint cell;
	uint before = listToFill.list.size();

	// north
	cell.Create(pos.x, pos.y + 1);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.Create(pos.x, pos.y - 1);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.Create(pos.x + 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.Create(pos.x - 1, pos.y);
	if (app->pathfinding->IsWalkable(cell))
		listToFill.list.push_back(PathNode(-1, -1, cell, this));

	return listToFill.list.size();
}

int PathNode::Score() const
{
	return g + h;
}

int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

int PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	int ret = -1;
	int iterations = 0;

	// if origin or destination are not walkable, return -1
	if (IsWalkable(origin) && IsWalkable(destination))
	{
		// TODO 2: Create two lists: open, close
		PathList open;
		PathList closed;

		// Add the origin tile to open
		open.list.push_back(PathNode(0, 0, origin, nullptr));

		// Iterate while we have tile in the open list
		while (open.list.size() > 0)
		{
			// Move the lowest score cell from open list to the closed list
			PathNode* lowest = open.GetNodeLowestScore();
			closed.list.push_back(*lowest);
			PathNode* node = &closed.list.back();

			//this line down here may fail !!
			open.list.remove_if([lowest](const PathNode& node) { return node.pos == lowest->pos; });

			// If we just added the destination, we are done!
			if (node->pos == destination)
			{
				lastPath.clear();

				// Backtrack to create the final path
				// Use the Pathnode::parent and Flip() the path when you are finish
				const PathNode* pathNode = node;

				while (pathNode)
				{
					lastPath.push_back(pathNode->pos);
					pathNode = pathNode->parent;
				}

				//reversing the list
				lastPath.assign(lastPath.rbegin(), lastPath.rend());


				ret = lastPath.size();
				LOG("Created path of %d steps in %d iterations", ret, iterations);
				break;
			}

			// Fill a list of all adjancent nodes
			PathList adjacent;
			node->FindWalkableAdjacents(adjacent);

			// Iterate adjancent nodes:
			// If it is a better path, Update the parent
			for (auto& item : adjacent.list)
			{
				// ignore nodes in the closed list
				if (closed.Find(item.pos) != NULL)
					continue;

				// If it is NOT found, calculate its F and add it to the open list
				PathNode* adjacentInOpen = (PathNode*)open.Find(item.pos);
				if (adjacentInOpen == NULL)
				{
					item.CalculateF(destination);
					open.list.push_back(item);
				}
				else
				{
					// If it is already in the open list, check if it is a better path (compare G)
					if (adjacentInOpen->g > item.g + 1)
					{
						adjacentInOpen->parent = item.parent;
						adjacentInOpen->CalculateF(destination);
					}
				}
			}

			++iterations;
		}
	}

	return ret;
}
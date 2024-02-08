#pragma once
#include "BaseModule.h"
#include "Point.h"
#include <vector>
#include <list>

#define DEFAULT_PATH_LENGTH 50
#define INVALID_WALK_CODE 255

class PathFinding : public Module
{
public:

	PathFinding(bool startEnabled);

	// Destructor
	~PathFinding();

	// Called before quitting
	bool CleanUp();

	// Sets up the walkability map
	void SetMap(uint width, uint height, uchar* data);

	// Main function to request a path from A to B
	int CreatePath(const iPoint& origin, const iPoint& destination);

	// To request all tiles involved in the last generated path
	const std::vector<iPoint>* GetLastPath() const;

	// To request all tiles involved in the last generated path
	void ClearLastPath();

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	// Utility: returns true is the tile is walkable
	bool IsWalkable(const iPoint& pos) const;

	// Utility: return the walkability value of a tile
	uchar GetTileAt(const iPoint& pos) const;

private:

	// size of the map
	uint width;
	uint height;

	// all map walkability values [0..255]
	uchar* map;

	// we store the created path here
	std::vector<iPoint> lastPath;
};

// forward declaration
struct PathList;


struct PathNode
{
	int g;
	int h;
	iPoint pos;
	const PathNode* parent; // needed to reconstruct the path in the end

	// Convenient constructors
	PathNode();
	PathNode(int g, int h, const iPoint& pos, const PathNode* parent);
	PathNode(const PathNode& node);

	// Fills a list (PathList) of all valid adjacent pathnodes
	uint FindWalkableAdjacents(PathList& list_to_fill) const;
	// Calculates this tile score
	int Score() const;
	// Calculate the F for a specific destination tile
	int CalculateF(const iPoint& destination);
};


struct PathList
{
	// Looks for a node in this list and returns it's list node or NULL
	PathNode* Find(const iPoint& point) const;

	// Returns the Pathnode with lowest score in this list or NULL if empty
	PathNode* GetNodeLowestScore() const;

	// The list itself, note they are not pointers!
	std::list<PathNode> list;
};
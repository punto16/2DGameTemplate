#pragma once
#include "BaseModule.h"
#include <string>
#include <vector>

#define MAX_ARRAY_ELEMENTS 150
#define MAX_LEVELS 1

// L04: DONE 2: Create a struct to hold information for a TileSet
// Ignore Terrain Types and Tile Types for now, but we want the image!
struct TileSet
{
	std::string name;
	int	firstgid;
	int margin;
	int	spacing;
	int	tileWidth;
	int	tileHeight;
	int columns;
	int tilecount;

	SDL_Texture* texture;

	// L05: DONE 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int gid) const;
};

//  We create an enum for map type, just for convenience,
// NOTE: Platformer game will be of type ORTHOGONAL
enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

// L06: DONE 5: Create a generic structure to hold properties
struct Properties
{
	struct Property
	{
		std::string name;
		bool value;
	};

	~Properties()
	{
		//...
		for (auto& item : list)
		{
			RELEASE(item);
		}
		list.clear();
	}

	// L06: DONE 7: Method to ask for the value of a custom property
	Property* GetProperty(const char* name);

	std::list<Property*> list;
};

// L05: DONE 1: Create a struct for the map layer
struct MapLayer
{
	std::string	name;
	int id;
	int width;
	int height;
	uint* data;

	// L06: DONE: Store custom properties
	Properties properties;

	MapLayer() : data(NULL)
	{}

	~MapLayer()
	{
		RELEASE(data);
	}

	// L05: DONE 6: Short function to get the gid value of x,y
	inline uint Get(int x, int y) const
	{
		return data[(y * width) + x];
	}
};

// L04: DONE 1: Create a struct needed to hold the information to Map node
struct MapData
{
	int width;
	int	height;
	int	tileWidth;
	int	tileHeight;
	std::list<TileSet*> tilesets;
	MapTypes type;

	// L05: DONE 2: Add a list/array of layers to the map
	std::list<MapLayer*> maplayers;
};

class Map : public Module
{
public:

	Map(bool startEnabled);

	// Destructor
	virtual ~Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	void Collisions(pugi::xml_node mapFile);

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* scene);

	// L05: DONE 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;

	iPoint WorldToMap(int x, int y);


	//pathfinding
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

private:

	bool LoadMap(pugi::xml_node mapFile);

	// L04: DONE 4: Create and call a private function to load a tileset
	bool LoadTileSet(pugi::xml_node mapFile);

	// L05
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadAllLayers(pugi::xml_node mapNode);

	// L06: DONE 2
	TileSet* GetTilesetFromTileId(int gid) const;

	// L06: DONE 6: Load a group of properties 
	bool LoadProperties(pugi::xml_node& node, Properties& properties);

public:

	// L04: DONE 1: Declare a variable data of the struct MapData
	MapData mapData;

	std::list<PhysBody*> collisions;

private:

	std::vector<std::string> mapFileName;
	std::string mapFolder;
	bool mapLoaded;
};
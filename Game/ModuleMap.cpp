#include "App.h"
#include "ModuleMap.h"
#include "ModuleTexture.h"
#include "ModulePhysics.h"
#include "ModuleWindow.h"
#include "ModuleSceneManager.h"

Map::Map(bool startEnabled) : Module(startEnabled), mapLoaded(false)
{
    name = "map";
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake(pugi::xml_node& config)
{
    LOG("Loading Map Parser");
    bool ret = true;

    return ret;
}

void Map::Draw()
{
    if (mapLoaded == false)
        return;

    // Prepare the loop to draw all tiles in a layer + DrawTexture()

    for (auto& mapLayerItem : mapData.maplayers)
    {
        if (mapLayerItem->properties.GetProperty("Draw") != NULL && mapLayerItem->properties.GetProperty("Draw")->value) {

            for (int x = 0; x < mapLayerItem->width; x++)
            {
                for (int y = 0; y < mapLayerItem->height; y++)
                {
                    // Complete the draw function
                    int gid = mapLayerItem->Get(x, y);

                    // Obtain the tile set using GetTilesetFromTileId
                    TileSet* tileset = GetTilesetFromTileId(gid);

                    SDL_Rect r = tileset->GetTileRect(gid);
                    iPoint pos = MapToWorld(x, y);

                    if (mapLayerItem->name == "Background")//PARALAX STUFF
                    {
                        app->render->DrawTexture(tileset->texture,
                            pos.x - (app->render->camera.x) * 0.3 / app->window->GetScale(),
                            pos.y,
                            &r);

                    }
                    else
                    {
                        app->render->DrawTexture(tileset->texture,
                            pos.x,
                            pos.y,
                            &r);
                    }
                }
            }
        }
    }
}

void Map::Collisions(pugi::xml_node mapFile) // it creates the collisions lol omg xd
{
    pugi::xml_node objectGroupNode = mapFile.child("map").child("objectgroup");


    while (objectGroupNode != NULL) {

        pugi::xml_node objectNode = objectGroupNode.child("object");

        // use GetProperty method to ask each layer if your “Draw” property is true.
        if (!strcmp(objectGroupNode.attribute("name").as_string(), "FloorCollisions")) {

            while (objectNode != NULL)
            {
                PhysBody* c1 = app->physics->CreateRectangleSensor(objectNode.attribute("x").as_int() + objectNode.attribute("width").as_int() / 2,
                    objectNode.attribute("y").as_int() + objectNode.attribute("height").as_int() / 2,
                    objectNode.attribute("width").as_int(),
                    objectNode.attribute("height").as_int(),
                    bodyType::STATIC);
                c1->ctype = ColliderType::PLATFORM;
                collisions.push_back(c1);

                objectNode = objectNode.next_sibling("object");
            }
        }
        else if (!strcmp(objectGroupNode.attribute("name").as_string(), "WallCollisions"))
        {
            while (objectNode != NULL)
            {
                PhysBody* c1 = app->physics->CreateRectangle(objectNode.attribute("x").as_int() + objectNode.attribute("width").as_int() / 2,
                    objectNode.attribute("y").as_int() + objectNode.attribute("height").as_int() / 2,
                    objectNode.attribute("width").as_int(),
                    objectNode.attribute("height").as_int(),
                    bodyType::STATIC);
                c1->ctype = ColliderType::WALL;
                collisions.push_back(c1);

                objectNode = objectNode.next_sibling("object");
            }
        }
        else if (!strcmp(objectGroupNode.attribute("name").as_string(), "WaterCollisions"))
        {
            while (objectNode != NULL)
            {
                PhysBody* c1 = app->physics->CreateRectangleSensor(objectNode.attribute("x").as_int() + objectNode.attribute("width").as_int() / 2,
                    objectNode.attribute("y").as_int() + objectNode.attribute("height").as_int() / 2,
                    objectNode.attribute("width").as_int(),
                    objectNode.attribute("height").as_int(),
                    bodyType::STATIC);
                c1->ctype = ColliderType::WATER;
                collisions.push_back(c1);

                objectNode = objectNode.next_sibling("object");
            }
        }
        else if (!strcmp(objectGroupNode.attribute("name").as_string(), "WinCollisions"))
        {
            while (objectNode != NULL)
            {
                PhysBody* c1 = app->physics->CreateRectangleSensor(objectNode.attribute("x").as_int() + objectNode.attribute("width").as_int() / 2,
                    objectNode.attribute("y").as_int() + objectNode.attribute("height").as_int() / 2,
                    objectNode.attribute("width").as_int(),
                    objectNode.attribute("height").as_int(),
                    bodyType::STATIC);
                c1->ctype = ColliderType::VICTORY;
                collisions.push_back(c1);

                objectNode = objectNode.next_sibling("object");
            }
        }
        else if (!strcmp(objectGroupNode.attribute("name").as_string(), "FloatingTerrainCollisions"))
        {
            while (objectNode != NULL)
            {
                PhysBody* c1 = app->physics->CreateRectangle(objectNode.attribute("x").as_int() + objectNode.attribute("width").as_int() / 2,
                    objectNode.attribute("y").as_int() + objectNode.attribute("height").as_int() / 2,
                    objectNode.attribute("width").as_int(),
                    objectNode.attribute("height").as_int(),
                    bodyType::STATIC);
                c1->ctype = ColliderType::FLOATINGTERRAIN;
                collisions.push_back(c1);

                objectNode = objectNode.next_sibling("object");
            }
        }
        else if (!strcmp(objectGroupNode.attribute("name").as_string(), "BridgePlatformCollision"))
        {
            //getting the array of points from xml

            std::string pointsString = (objectNode.child("polygon").attribute("points").as_string());
            int size = 0;
            int points[MAX_ARRAY_ELEMENTS];
            bool negative = false;

            //capped to maximum of a coord = 9999
            int toadditerator = 0;
            int iterator = 0;
            while (pointsString[iterator] != '\0')
            {
                if (pointsString[iterator] == '-')
                {
                    negative = true;
                }
                else if (pointsString[iterator] != ',' && pointsString[iterator] != ' ')
                {
                    points[size] = (int)pointsString[iterator] - 48;

                    if (pointsString[iterator + 1] != '\0' && pointsString[iterator + 1] != ',' && pointsString[iterator + 1] != ' ')
                    {
                        points[size] *= 10;
                        points[size] += (int)pointsString[iterator + 1] - 48;
                        toadditerator++;
                        if (pointsString[iterator + 2] != '\0' && pointsString[iterator + 2] != ',' && pointsString[iterator + 2] != ' ')
                        {
                            points[size] *= 10;
                            points[size] += (int)pointsString[iterator + 2] - 48;
                            toadditerator++;
                            if (pointsString[iterator + 3] != '\0' && pointsString[iterator + 3] != ',' && pointsString[iterator + 3] != ' ')
                            {
                                points[size] *= 10;
                                points[size] += (int)pointsString[iterator + 3] - 48;
                                toadditerator++;
                            }
                        }
                    }
                    if (negative)
                    {
                        points[size] *= -1;
                    }
                    size++;
                }
                iterator += toadditerator + 1;
                toadditerator = 0;
            }
            while (objectNode != NULL)
            {
                PhysBody* c1 = app->physics->CreateChain(objectNode.attribute("x").as_int(),
                    objectNode.attribute("y").as_int(),
                    points,
                    size,
                    bodyType::STATIC,
                    true);
                c1->ctype = ColliderType::PLATFORM;
                collisions.push_back(c1);
                objectNode = objectNode.next_sibling("object");
            }
        }
        else if (!strcmp(objectGroupNode.attribute("name").as_string(), "BridgeWallCollision"))
        {
            //getting the array of points from xml

            std::string pointsString = (objectNode.child("polygon").attribute("points").as_string());
            int size = 0;
            int points[MAX_ARRAY_ELEMENTS];
            bool negative = false;

            //capped to maximum of a coord = 9999
            int toadditerator = 0;
            int iterator = 0;
            while (pointsString[iterator] != '\0')
            {
                if (pointsString[iterator] == '-')
                {
                    negative = true;
                }
                else if (pointsString[iterator] != ',' && pointsString[iterator] != ' ')
                {
                    points[size] = (int)pointsString[iterator] - 48;

                    if (pointsString[iterator + 1] != '\0' && pointsString[iterator + 1] != ',' && pointsString[iterator + 1] != ' ')
                    {
                        points[size] *= 10;
                        points[size] += (int)pointsString[iterator + 1] - 48;
                        toadditerator++;
                        if (pointsString[iterator + 2] != '\0' && pointsString[iterator + 2] != ',' && pointsString[iterator + 2] != ' ')
                        {
                            points[size] *= 10;
                            points[size] += (int)pointsString[iterator + 2] - 48;
                            toadditerator++;
                            if (pointsString[iterator + 3] != '\0' && pointsString[iterator + 3] != ',' && pointsString[iterator + 3] != ' ')
                            {
                                points[size] *= 10;
                                points[size] += (int)pointsString[iterator + 3] - 48;
                                toadditerator++;
                            }
                        }
                    }
                    if (negative)
                    {
                        points[size] *= -1;
                    }
                    size++;
                }
                iterator += toadditerator + 1;
                toadditerator = 0;
            }
            while (objectNode != NULL)
            {
                PhysBody* c1 = app->physics->CreateChain(objectNode.attribute("x").as_int(),
                    objectNode.attribute("y").as_int(),
                    points,
                    size,
                    bodyType::STATIC);
                c1->ctype = ColliderType::WALL;
                collisions.push_back(c1);
                objectNode = objectNode.next_sibling("object");
            }
        }
        objectGroupNode = objectGroupNode.next_sibling("objectgroup");
    }
}

// Create a method that translates x,y coordinates from map positions to world positions
iPoint Map::MapToWorld(int x, int y) const
{
    iPoint ret;

    ret.x = x * mapData.tileWidth;
    ret.y = y * mapData.tileHeight;

    return ret;
}

iPoint Map::WorldToMap(int x, int y)
{
    iPoint ret(0, 0);

    if (mapData.type == MAPTYPE_ORTHOGONAL)
    {
        ret.x = x / mapData.tileWidth;
        ret.y = y / mapData.tileHeight;
    }
    else if (mapData.type == MAPTYPE_ISOMETRIC)
    {
        float halfWidth = mapData.tileWidth * 0.5f;
        float halfHeight = mapData.tileHeight * 0.5f;
        ret.x = int((x / halfWidth + y / halfHeight) / 2);
        ret.y = int((y / halfHeight - x / halfWidth) / 2);
    }
    else
    {
        LOG("Unknown map type");
        ret.x = x; ret.y = y;
    }

    return ret;
}

// Create walkability map for pathfinding
bool Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer) const
{
    bool ret = false;

    for (auto& item : mapData.maplayers)
    {
        MapLayer* layer = item;

        if (layer->properties.GetProperty("Navigation") != NULL && !layer->properties.GetProperty("Navigation")->value)
            continue;

        uchar* map = new uchar[layer->width * layer->height];
        memset(map, 1, layer->width * layer->height);

        for (int y = 0; y < mapData.height; ++y)
        {
            for (int x = 0; x < mapData.width; ++x)
            {
                int i = (y * layer->width) + x;

                int tileId = layer->Get(x, y);
                TileSet* tileset = (tileId > 0) ? GetTilesetFromTileId(tileId) : NULL;

                if (tileset != NULL)
                {
                    map[i] = (tileId - tileset->firstgid) > 0 ? 0 : 1;
                }
            }
        }

        *buffer = map;
        width = mapData.width;
        height = mapData.height;
        ret = true;

        break;
    }

    return ret;
}

// Get relative Tile rectangle
SDL_Rect TileSet::GetTileRect(int gid) const
{
    SDL_Rect rect = { 0 };
    int relativeIndex = gid - firstgid;

    // Get relative Tile rectangle
    rect.w = tileWidth;
    rect.h = tileHeight;
    rect.x = margin + (tileWidth + spacing) * (relativeIndex % columns);
    rect.y = margin + (tileWidth + spacing) * (relativeIndex / columns);

    return rect;
}


// Pick the right Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
    TileSet* set = NULL;

    for (auto& item : mapData.tilesets)
    {
        set = item;
        if (gid < (item->firstgid + item->tilecount))
        {
            break;
        }
    }

    return set;
}

// Called before quitting
bool Map::CleanUp()
{
    LOG("Unloading map");

    // Make sure you clean up any memory allocated from tilesets/map
    for (auto& item : mapData.tilesets)
    {
        RELEASE(item);
    }
    mapData.tilesets.clear();

    // clean up all layer data
    // Remove all layers

    for (auto& layerItem : mapData.maplayers)
    {
        RELEASE(layerItem);
    }
    mapData.maplayers.clear();

    //REMOVE ALL COLLIDERS
    for (auto& collisionsItem : collisions)
    {
        collisionsItem->body->DestroyFixture(collisionsItem->body->GetFixtureList());
        app->physics->world->DestroyBody(collisionsItem->body);
        RELEASE(collisionsItem);
    }
    collisions.clear();

    return true;
}

// Load new map
bool Map::Load(const char* scene)
{

    pugi::xml_node configNode = app->LoadConfigFileToVar();
    pugi::xml_node config = configNode.child(scene).child(name.c_str());

    mapFolder = config.child("mapfolder").attribute("path").as_string();

    for (pugi::xml_node nodeMapPath = config.child("mapfile");
        nodeMapPath; nodeMapPath = nodeMapPath.next_sibling("mapfile"))
    {
        mapFileName.push_back(nodeMapPath.attribute("path").as_string());
    }

    for (size_t i = 0; i < mapFileName.size(); i++)
    {
        LOG("String of Path saved: %s", mapFileName[i].c_str());
    }
    bool ret = true;
    pugi::xml_document mapFileXML;
    pugi::xml_parse_result result = mapFileXML.load_file(mapFileName[app->sceneManager->currentLevel].c_str());


    if (result == NULL)
    {
        LOG("Could not load map xml file %s. pugi error: %s", mapFileName[app->sceneManager->currentLevel].c_str(), result.description());
        ret = false;
    }

    if (ret == true)
    {
        ret = LoadMap(mapFileXML);
    }

    if (ret == true)
    {
        ret = LoadTileSet(mapFileXML);
    }

    // Iterate all layers and load each of them
    if (ret == true)
    {
        ret = LoadAllLayers(mapFileXML.child("map"));
    }

    Collisions(mapFileXML);


    if (ret == true)
    {
        // LOG all the data loaded iterate all tilesets and LOG everything

        LOG("Successfully parsed map XML file :%s", mapFileName[app->sceneManager->currentLevel].c_str());
        LOG("width : %d height : %d", mapData.width, mapData.height);
        LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

        LOG("Tilesets----");

        for (auto& tileset : mapData.tilesets) {
            LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstgid);
            LOG("tile width : %d tile height : %d", tileset->tileWidth, tileset->tileHeight);
            LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
        }

        // LOG the info for each loaded layer

        for (auto& mapLayer : mapData.maplayers) {
            LOG("id : %d name : %s", mapLayer->id, mapLayer->name.c_str());
            LOG("Layer width : %d Layer height : %d", mapLayer->width, mapLayer->height);
        }
    }

    if (mapFileXML) mapFileXML.reset();

    mapLoaded = ret;

    return ret;
}

// Implement LoadMap to load the map properties
bool Map::LoadMap(pugi::xml_node mapFile)
{
    bool ret = true;
    pugi::xml_node map = mapFile.child("map");

    if (map == NULL)
    {
        LOG("Error parsing map xml file: Cannot find 'map' tag.");
        ret = false;
    }
    else
    {
        //Load map general properties
        mapData.height = map.attribute("height").as_int();
        mapData.width = map.attribute("width").as_int();
        mapData.tileHeight = map.attribute("tileheight").as_int();
        mapData.tileWidth = map.attribute("tilewidth").as_int();
    }

    mapData.type = MAPTYPE_UNKNOWN;
    if ((std::string)map.attribute("orientation").as_string() == "isometric")
    {
        mapData.type = MAPTYPE_ISOMETRIC;
    }
    if ((std::string)map.attribute("orientation").as_string() == "orthogonal")
    {
        mapData.type = MAPTYPE_ORTHOGONAL;
    }

    return ret;
}

// Implement the LoadTileSet function to load the tileset properties
bool Map::LoadTileSet(pugi::xml_node mapFile) {

    bool ret = true;

    pugi::xml_node tileset;
    for (tileset = mapFile.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
    {
        TileSet* set = new TileSet();

        // Load Tileset attributes
        set->name = tileset.attribute("name").as_string();
        set->firstgid = tileset.attribute("firstgid").as_int();
        set->margin = tileset.attribute("margin").as_int();
        set->spacing = tileset.attribute("spacing").as_int();
        set->tileWidth = tileset.attribute("tilewidth").as_int();
        set->tileHeight = tileset.attribute("tileheight").as_int();
        set->columns = tileset.attribute("columns").as_int();
        set->tilecount = tileset.attribute("tilecount").as_int();

        // Load Tileset image
        std::string tmp = mapFolder.c_str() + (std::string)tileset.child("image").attribute("source").as_string();
        set->texture = app->tex->Load(tmp.c_str());

        mapData.tilesets.push_back(set);
    }

    return ret;
}

// Implement a function that loads a single layer layer
bool Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
    bool ret = true;

    //Load the attributes
    layer->id = node.attribute("id").as_int();
    layer->name = node.attribute("name").as_string();
    layer->width = node.attribute("width").as_int();
    layer->height = node.attribute("height").as_int();

    // Call Load Propoerties
    LoadProperties(node, layer->properties);

    //Reserve the memory for the data 
    layer->data = new uint[layer->width * layer->height];
    memset(layer->data, 0, layer->width * layer->height);

    //Iterate over all the tiles and assign the values
    pugi::xml_node tile;
    int i = 0;
    for (tile = node.child("data").child("tile"); tile && ret; tile = tile.next_sibling("tile"))
    {
        layer->data[i] = tile.attribute("gid").as_int();
        i++;
    }

    return ret;
}

// Iterate all layers and load each of them
bool Map::LoadAllLayers(pugi::xml_node mapNode) {
    bool ret = true;

    for (pugi::xml_node layerNode = mapNode.child("layer"); layerNode && ret; layerNode = layerNode.next_sibling("layer"))
    {
        //Load the layer
        MapLayer* mapLayer = new MapLayer();
        ret = LoadLayer(layerNode, mapLayer);

        //add the layer to the map
        mapData.maplayers.push_back(mapLayer);
    }

    return ret;
}

// Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
    bool ret = false;

    for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
    {
        Properties::Property* p = new Properties::Property();
        p->name = propertieNode.attribute("name").as_string();
        p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

        properties.list.push_back(p);
    }

    return ret;
}


// Ask for the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
    Property* p = NULL;

    for (auto& item : list)
    {
        if (item->name == name) {
            p = item;
            break;
        }
    }
    return p;
}
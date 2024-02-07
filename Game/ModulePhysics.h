#pragma once
#include "BaseModule.h"
#include "Entity.h"
#include "Point.h"

#include "box2d/box2d.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -16.0f

#define PIXELS_PER_METER 32.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.03125f // this is 1 / PIXELS_PER_METER !
#define RAD_PER_DEG ((float)180/b2_pi)
#define DEG_PER_RAD ((float)b2_pi/180)

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)
#define RAD_TO_DEG(r) ((float) RAD_PER_DEG * r)
#define DEG_TO_RAD(r) ((float) DEG_PER_RAD * r)
#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f

// types of bodies
enum bodyType {
	DYNAMIC,
	STATIC,
	KINEMATIC
};

//HERE YOU CAN CHANGE THE COLLIDERTYPES YOU USE!!
enum class ColliderType {
	PLAYER,
	ITEM,
	PLATFORM,
	FLOATINGTERRAIN,
	WALL,
	WATER,
	VICTORY,
	TERRESTREENEMY,
	FLYINGENEMY,
	UNKNOWN
	// ..
};

// Small class to return to other modules to track position and rotation of physics bodies
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL), ctype(ColliderType::UNKNOWN)
	{}

	void GetPosition(int& x, int& y) const;
	void SetPosition(int x, int y);
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Entity* listener;
	ColliderType ctype;
};

// Module --------------------------------------
class Physics : public Module, public b2ContactListener 
{
public:
	Physics(bool startEnabled);
	~Physics();

	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();


	PhysBody* CreateRectangle(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateCircle(int x, int y, int radious, bodyType type, bool isSensor = false);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, bodyType type);
	PhysBody* CreateChain(int x, int y, int* points, int size, bodyType type, bool sensor = false);
	b2RevoluteJoint* CreateRevoluteJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float angle, bool collideConnected, bool enableLimit);
	b2PrismaticJoint* CreatePrismaticJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, b2Vec2 axys, float maxHeight, bool collideConnected, bool enableLimit);
	b2WeldJoint* CreateWeldJoint(PhysBody* A, b2Vec2 anchorA, PhysBody* B, b2Vec2 anchorB, float angle, bool collideConnected, bool enableLimit);

	void Pause();

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	b2World* world;
	bool debug = false;
	bool pause = false;

private:


	b2MouseJoint* mouse_joint;
	b2Body* ground;
};
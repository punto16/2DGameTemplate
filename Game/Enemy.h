#pragma once
#include "Entity.h"
#include "Point.h"
#include "Animation.h"

struct SDL_Texture;

enum class STATE
{
	//normal path is "random" movement of the enemy
	NORMALPATH,
	//aggressive path should use pathfinding to find player
	AGRESSIVEPATH,
	//enemy is attacking player
	ATTACKING,
	//enemy is dying
	DYING
};

class Enemy : public Entity
{
public:
	Enemy(bool startEnabled);

	//destroyer
	virtual ~Enemy();

	//called before render
	bool Awake();

	//called before first frame
	bool Start();

	//called each loop
	bool Update(float dt);

	bool onlyDraw();

	//called before opening
	bool CleanUp();

	//draw function
	void Draw();

	//oncollisionbox2d..
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	//outcollisionbox2d..
	void EndCollision(PhysBody* bodyA, PhysBody* bodyB);

	void DestroyPhysBody();


public:

	PhysBody* getEbody()
	{
		return ebody;
	}

	//is enemy in air?
	bool jumping;

	//is enemy alive?
	bool alive;

	//variables for "ai" of the enemy
	PhysBody* collisionWith;
	iPoint collisionWithPosition;

	STATE state;

private:

	//enemy parameters:
	//texture of the enemy (sprite)
	SDL_Texture* texture;
	//path of texture
	const char* texturePath;

	//animations
	//current animation the player is in
	Animation* currentAnimation = nullptr;
	Animation idleAnimation;
	Animation attackAnimation;


	//physic body of the enemy -> EnemyBODY
	PhysBody* ebody;

	//variable to call a sound
	unsigned int attackfx;
};
#pragma once
#include "Entity.h"
#include "Point.h"
#include "Animation.h"

struct SDL_Texture;

class Player : public Entity
{
public:
	Player(bool startEnabled);

	//destroyer
	virtual ~Player();

	//called before render
	bool Awake();

	//called before first frame
	bool Start();

	//called each loop
	bool Update(float dt);

	//called before opening
	bool CleanUp();

	//draw function
	void Draw();

	bool onlyDraw();

	void DestroyPhysBody();

	//oncollisionbox2d..
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	//outcollisionbox2d..
	void EndCollision(PhysBody* bodyA, PhysBody* bodyB);


public:

	PhysBody* getPbody()
	{
		return pbody;
	}

	//is player in air?
	bool jumping;

	//is player alive?
	bool alive;

private:

	//player parameters:
	//texture of the player (sprite)
	SDL_Texture* texture;
	//path of texture
	const char* texturePath;

	//animations
	//current animation the player is in
	Animation* currentAnimation = nullptr;
	Animation idleAnimation;
	Animation jumpAnimation;


	//physic body of the player -> PlayerBODY
	PhysBody* pbody;

	//variable to call a sound
	unsigned int jumpfx;
};
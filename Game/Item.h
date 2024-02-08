#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Point.h"
#include "ModulePhysics.h"

struct SDL_Texture;

enum class itemType
{
	HEARTS,
	COINS,
	CHECKPOINT
};

enum class itemState
{
	IDLE,
	TRIGGER,
	TAKEN
};


class Item : public Entity
{
public:

	Item(bool startEnabled, itemType itemtype = itemType::HEARTS);
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update();

	bool onlyDraw();

	bool CleanUp();

	void OnCollision(PhysBody* physA, PhysBody* physB);

	void Draw();

	void DestroyPhysBody();

public:

	bool isPicked = false;
	PhysBody* ibody;
	itemType itemtype;
	itemState state;

private:
	Animation* currentAnimation;
	Animation idleAnimation;	//if coin -> normal; if heart -> normal; if checkpoint -> not taken anim;
	Animation actionAnimation;	//if coin -> spin; if heart -> not used; if checkpoint -> triggeranimation;
	Animation takenAnimation;	//if coin -> counter hud; if heart -> counter hud; if checkpoint -> moving flag by wind

	unsigned int checkpointfx;
	unsigned int coinfx;
	unsigned int healfx;

	SDL_Texture* texture;
	const char* texturePath;
};
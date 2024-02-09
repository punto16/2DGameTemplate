#include "App.h"
#include "Enemy.h"
#include "ModuleTexture.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"

Enemy::Enemy(bool startEnabled) : Entity(startEnabled, EntityType::ENEMY)
{
	name = "enemy";
}

Enemy::~Enemy() {

}

// in awake you can load parameters and the animation
bool Enemy::Awake() {

	initialPosition.x = parameters.attribute("x").as_int();
	initialPosition.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//this is an example of the usage of animation
	idleAnimation.PushBack({ 0,  0,100,100 });
	idleAnimation.PushBack({ 100,  0,100,100 });
	idleAnimation.loop = true;
	idleAnimation.pingpong = false;
	//same method on other animations
	attackAnimation.PushBack({ 200,  0,100,100 });
	attackAnimation.PushBack({ 300,  0,100,100 });
	attackAnimation.loop = true;
	attackAnimation.pingpong = false;

	return true;
}

bool Enemy::Start()
{
	//init textures
	texture = app->tex->Load(texturePath);

	//set idle anim as current anim
	currentAnimation = &idleAnimation;

	//setting variables
	alive = true;
	jumping = false;

	//create physbody attached to ebody
	//for example, a circle hitbox
	//all position of box2d items are in the middle of the object
	ebody = app->physics->CreateRectangle(initialPosition.x + 14, initialPosition.y + 6, 28, 12, bodyType::DYNAMIC);
	//when oncollision/endcollision of ebody, will call oncollision/endcollision functions in ebody.cpp
	ebody->listener = this;
	ebody->ctype = ColliderType::ENEMY;
	ebody->body->SetFixedRotation(1);

	collisionWithPosition = { 0,0 };
	collisionWith = nullptr;

	state = STATE::NORMALPATH;

	//load sound effects
	attackfx = app->audio->LoadFx("Assets/Audio/Fx/attackfx.wav");

	position.x = initialPosition.x;
	position.y = initialPosition.y;

	return true;
}

bool Enemy::Update(float dt)
{
	//here goes the movement of the enemy
	
	//check player update to know more or less how to do it

	//IMPORTANT TO UPDATE THE ANIMATION
	currentAnimation->Update();

	return true;
}

void Enemy::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y + 2, &rect);
}

bool Enemy::onlyDraw()
{
	currentAnimation->Update();
	Draw();

	return true;
}

void Enemy::DestroyPhysBody()
{
	if (ebody != NULL)
	{
		//destroy physbody info
		ebody->body->DestroyFixture(ebody->body->GetFixtureList());
		app->physics->world->DestroyBody(ebody->body);
		RELEASE(ebody);
	}
}

bool Enemy::CleanUp()
{
	//unload texture
	LOG("Cleanup of the enemy");
	app->tex->UnLoad(texture);
	//audios cant be unloaded, if so, it will affect all other audios and new ones

	DestroyPhysBody();

	return true;
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physA->ctype == ColliderType::ENEMY)
	{
		switch (physB->ctype)
		{
		case ColliderType::PLAYER:
			//any method that if player jumps on top of the enemy, enemy dies, or the other way around
			break;
		case ColliderType::PLATFORM:
			LOG("Collision PLATFORM");
			jumping = false;
			break;
		case ColliderType::WALL:
			LOG("Collision Wall");
			break;
		case ColliderType::FLOATINGTERRAIN:
			LOG("Collision FloatingTerrain");
			break;
		case ColliderType::WATER:
			LOG("Collision Water");
			alive = false;
			break;
		case ColliderType::ENEMY:
			LOG("Collision Enemy");
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
	}
}

void Enemy::EndCollision(PhysBody* physA, PhysBody* physB)
{
	if (physA->ctype == ColliderType::ENEMY)
	{
		switch (physB->ctype)
		{
		case ColliderType::PLATFORM:
			LOG("End collision PLATFORM");
			jumping = true;
		case ColliderType::UNKNOWN:
			LOG("End collision UNKNOWN");
			break;
		}
	}
}
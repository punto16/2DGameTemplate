#include "App.h"
#include "Player.h"
#include "ModuleTexture.h"
#include "ModuleAudio.h"
#include "ModuleInput.h"
#include "ModulePhysics.h"

Player::Player(bool startEnabled) : Entity(EntityType::PLAYER)
{
	name = "player";

}

Player::~Player() {

}

// in awake you can load parameters and the animation
bool Player::Awake() {

	initialPosition.x = parameters.attribute("x").as_int();
	initialPosition.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	//this is an example of the usage of animation
	idleAnimation.PushBack({   0,  0,100,100 });
	idleAnimation.PushBack({ 100,  0,100,100 });
	idleAnimation.loop = true;
	idleAnimation.pingpong = false;
	//same method on other animations
	jumpAnimation.PushBack({ 200,  0,100,100 });
	jumpAnimation.PushBack({ 300,  0,100,100 });
	jumpAnimation.loop = true;
	jumpAnimation.pingpong = false;

	return true;
}

bool Player::Start()
{
	//init textures
	texture = app->tex->Load(texturePath);
	
	//set idle anim as current anim
	currentAnimation = &idleAnimation;

	//setting variables
	alive = true;
	jumping = false;

	//create physbody attached to player
	//for example, a circle hitbox
	//all position of box2d items are in the middle of the object
	pbody = app->physics->CreateCircle(initialPosition.x + 15, initialPosition.y + 15, 15, bodyType::DYNAMIC);
	//when oncollision/endcollision of player, will call oncollision/endcollision functions in player.cpp
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;

	//load sound effects
	jumpfx = app->audio->LoadFx("Assets/Audio/Fx/jumpfx.wav");

	position.x = initialPosition.x;
	position.y = initialPosition.y;
	
	return true;
}

bool Player::Update(float dt)
{
	//here use functions such as the following one to move the player
	
	b2Vec2 vel = pbody->body->GetLinearVelocity() + b2Vec2(0, -GRAVITY_Y * 0.05);
	//set gravity scale 1 (normal one) or  if you want no gravity (example if you want a godmode free moving)
	pbody->body->SetGravityScale(1);
	//jump function
	if (app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && !jumping)
	{
		app->audio->PlayFx(jumpfx);
		vel.y = -20.0f;
		jumping = true;
		idleAnimation.Reset();
		currentAnimation = &jumpAnimation;
	}
	else
	{
		jumpAnimation.Reset();
		currentAnimation = &idleAnimation;
	}

	//IMPORTANT TO UPDATE THE ANIMATION
	currentAnimation->Update();

	return true;
}

void Player::Draw()
{
	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x, position.y + 2, &rect);
}

bool Player::onlyDraw()
{
	currentAnimation->Update();
	Draw();

	return true;
}

void Player::DestroyPhysBody()
{
	//destroy physbody info
	if (pbody != NULL)
	{
		pbody->body->DestroyFixture(pbody->body->GetFixtureList());
		app->physics->world->DestroyBody(pbody->body);
		RELEASE(pbody);
	}
}

bool Player::CleanUp()
{
	//unload texture
	LOG("Cleanup of the player");
	app->tex->UnLoad(texture);
	//audios cant be unloaded, if so, it will affect all other audios and new ones

	DestroyPhysBody();

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB)
{
	if (physA->ctype == ColliderType::PLAYER)
	{
		switch (physB->ctype)
		{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
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
		case ColliderType::VICTORY:
			LOG("Collision Victory");
			break;
		case ColliderType::ENEMY:
			LOG("Collision Enemy");
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		}
	}
}

void Player::EndCollision(PhysBody* physA, PhysBody* physB)
{
	if (physA->ctype == ColliderType::PLAYER)
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
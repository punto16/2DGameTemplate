#include "Item.h"
#include "App.h"
#include "ModuleTexture.h"
#include "ModuleAudio.h"
#include "ModuleEntityManager.h"

Item::Item(bool startEnabled, itemType itemtype) : Entity(EntityType::ITEM)
{
	name = "item";
	this->itemtype = itemtype;
}

Item::~Item() {}

bool Item::Awake() {

	initialPosition.x = parameters.attribute("x").as_int();
	initialPosition.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();

	switch (itemtype)
	{
	case itemType::HEARTS:
		//this is an example of the usage of animation
		//idle anim
		idleAnimation.PushBack({   0,  0,100,100 });
		idleAnimation.PushBack({ 100,  0,100,100 });
		idleAnimation.loop = true;
		idleAnimation.pingpong = false;
		break;
	case itemType::COINS:
		//idle anim
		idleAnimation.PushBack({   0,100,100,100 });
		idleAnimation.PushBack({ 100,100,100,100 });
		idleAnimation.loop = true;
		idleAnimation.pingpong = false;
		//taken anim
		takenAnimation.PushBack({ 100,100,100,100 });
		takenAnimation.PushBack({ 200,100,100,100 });
		takenAnimation.loop = true;
		takenAnimation.pingpong = false;
		//action anim
		actionAnimation.PushBack({ 300,100,100,100 });
		actionAnimation.PushBack({ 400,100,100,100 });
		actionAnimation.loop = true;
		actionAnimation.pingpong = false;
		break;
	case itemType::CHECKPOINT:
		//idle anim
		idleAnimation.PushBack({   0,200,100,100 });
		idleAnimation.PushBack({ 100,200,100,100 });
		idleAnimation.loop = true;
		idleAnimation.pingpong = false;
		//taken anim
		takenAnimation.PushBack({ 100,200,100,100 });
		takenAnimation.PushBack({ 200,200,100,100 });
		takenAnimation.loop = true;
		takenAnimation.pingpong = false;
		//action anim
		actionAnimation.PushBack({ 300,200,100,100 });
		actionAnimation.PushBack({ 400,200,100,100 });
		actionAnimation.loop = false;
		actionAnimation.pingpong = false;
		break;
	default:
		break;
	}

	return true;
}

bool Item::Start() {
	//example of sounds
	healfx = app->audio->LoadFx("Assets/Audio/Fx/healfx.wav");
	coinfx = app->audio->LoadFx("Assets/Audio/Fx/coinfx.wav");
	checkpointfx = app->audio->LoadFx("Assets/Audio/Fx/checkpointfx.wav");


	//initilize textures
	texture = app->tex->Load(texturePath);
	switch (itemtype)
	{
	case itemType::HEARTS:
		ibody = app->physics->CreateCircle(initialPosition.x + 5, initialPosition.y + 5, 5, bodyType::STATIC, true);
		ibody->ctype = ColliderType::ITEM;
		ibody->listener = this;
		currentAnimation = &idleAnimation;
		break;
	case itemType::COINS:
		ibody = app->physics->CreateCircle(initialPosition.x + 8, initialPosition.y + 8, 8, bodyType::STATIC, true);
		ibody->ctype = ColliderType::ITEM;
		ibody->listener = this;
		currentAnimation = &actionAnimation;
		break;
	case itemType::CHECKPOINT:
		ibody = app->physics->CreateRectangleSensor(initialPosition.x, initialPosition.y, 16, 28, bodyType::STATIC);
		ibody->ctype = ColliderType::ITEM;
		ibody->listener = this;
		currentAnimation = &idleAnimation;
		break;
	default:
		break;
	}
	state = itemState::IDLE;

	position.x = initialPosition.x;
	position.y = initialPosition.y;

	return true;
}

bool Item::Update()
{
	//here goes the behaviour of the items

	//check player update to know more or less how to do it

	//IMPORTANT TO UPDATE THE ANIMATION
	currentAnimation->Update();

	return true;
}

void Item::Draw()
{
	SDL_Rect rect;
	switch (itemtype)
	{
	case itemType::HEARTS:
		position.x = METERS_TO_PIXELS(ibody->body->GetTransform().p.x) - 5;
		position.y = METERS_TO_PIXELS(ibody->body->GetTransform().p.y) - 5;
		rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		break;
	case itemType::COINS:
		position.x = METERS_TO_PIXELS(ibody->body->GetTransform().p.x) - 8;
		position.y = METERS_TO_PIXELS(ibody->body->GetTransform().p.y) - 8;
		rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		break;
	case itemType::CHECKPOINT:
		position.x = METERS_TO_PIXELS(ibody->body->GetTransform().p.x - 8);
		position.y = METERS_TO_PIXELS(ibody->body->GetTransform().p.y - 14);
		rect = currentAnimation->GetCurrentFrame();
		app->render->DrawTexture(texture, position.x, position.y, &rect);
		break;
	default:
		break;
	}
}

bool Item::onlyDraw()
{
	currentAnimation->Update();
	Draw();

	return true;
}

void Item::DestroyPhysBody()
{
	if (ibody != NULL)
	{
		//destroy physbody info
		ibody->body->DestroyFixture(ibody->body->GetFixtureList());
		app->physics->world->DestroyBody(ibody->body);
		RELEASE(ibody);
	}
}

bool Item::CleanUp()
{
	//unload texture
	LOG("Cleanup of the enemy");
	app->tex->UnLoad(texture);
	//audios cant be unloaded, if so, it will affect all other audios and new ones

	DestroyPhysBody();

	return true;
}

void Item::OnCollision(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision ITEM");
		switch (itemtype)
		{
		case itemType::HEARTS:
			if (state == itemState::IDLE)
			{
				app->audio->PlayFx(healfx);
				state = itemState::TAKEN;
				app->entity_manager->DestroyEntity(this);
			}
			break;
		case itemType::COINS:
			if (state == itemState::IDLE)
			{
				app->audio->PlayFx(coinfx);
				state = itemState::TAKEN;
				app->entity_manager->DestroyEntity(this);
			}
			break;
		case itemType::CHECKPOINT:
			if (state == itemState::IDLE)
			{
				app->audio->PlayFx(checkpointfx);
				app->SaveGameRequest();
				state = itemState::TRIGGER;
				app->entity_manager->OnlyDrawEntity(this);
			}
			break;
		default:
			break;
		}
		break;
	}
}

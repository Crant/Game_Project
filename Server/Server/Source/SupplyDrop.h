//Simon Ed�nge, 2013-02-27
#pragma once

#include "Actor.h"
#include "Item.h"

class ActorManager;
class World;
class PhysicsEngine;
class SoundHandler;

//MODEL NAME
static const std::string SUPPLY_ACTOR_FILENAME	= "media/models/Supplycrate_01_v03.obj";
static const std::string PARACHUTE_FILE_NAME	= "media/models/SupplyAddon.obj";

class SupplyDrop
{
public:
	SupplyDrop( ActorManager* actorManager, World* world, SoundHandler* soundHandler );
	virtual ~SupplyDrop();
	
	bool SpawnSupplyDrop( Vector2& landPoint, std::set<Item*>& items, float height);
	bool SpawnSupplyDrop( Vector2& landPoint, std::set<Item*>& items, const float itemCapacity, float height);

	bool SpawnAirbornSupplyDrop( Vector2& landPoint, float height, std::set<Item*>& items);
	bool SpawnAirbornSupplyDrop( Vector2& landPoint, float height, std::set<Item*>& items, const float itemCapacity);

private:
	const float CalculateTotalWeight(std::set<Item*>& items) const;
	Vector2 RandomizePos(const Vector2& landpos);

private:
	World* zWorld;
	ActorManager* zActorManager;
	PhysicsEngine* zPhyEngine;
	SoundHandler* zSoundHandler;
};
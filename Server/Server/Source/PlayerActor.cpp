#include "PlayerActor.h"
#include "ClientServerMessages.h"
#include "MaloWFileDebug.h"

#define TAKEARROWANIMLENGTH 1.0f

PlayerActor::PlayerActor(Player* player, PhysicsObject* physObj, Observer* game) 
	: BioActor(player, game)
{
	SetPhysicsObject(physObj);
	this->zInventory->AddObserver(player);

	this->SetScale(Vector3(0.05f, 0.05f, 0.05f), false);

	/*InitValues();*/

	this->zFullness = 100.0f;
	this->zHydration = 100.0f;

	this->zFullnessMax = 100;
	this->zHydrationMax = 100;

	this->zType = ACTOR_TYPE_PLAYER;
	
	this->zExhausted = false;
	this->zHasSprinted = false;
	this->zUsingBow = false;
}

PlayerActor::~PlayerActor()
{
}

float PlayerActor::GetFullness()
{
	return this->zFullness;
}

float PlayerActor::GetHydration()
{
	return this->zHydration;
}

void PlayerActor::SetFullness(float fullness, const bool notify /*notify = true*/)
{

	if( fullness != this->zFullness )
	{
		this->zFullness = fullness;

		if(this->zFullness > this->zFullnessMax)
		{
			this->zFullness = this->zFullnessMax;
		}

		if(notify)
		{
			PlayerActorPhysicalConditionHungerEvent e;
			e.zPlayerActor = this;
			NotifyObservers(&e);
		}
	}
}

void PlayerActor::SetHydration(float hydration, const bool notify /*notify = true*/)
{

	if(this->zHydration != hydration)
	{
		this->zHydration = hydration;

		if(this->zHydration > this->zHydrationMax)
		{
			this->zHydration = this->zHydrationMax;
		}

		if(notify)
		{
			PlayerActorPhysicalConditionHydrationEvent e;
			e.zPlayerActor = this;
			NotifyObservers(&e);
		}
	}
}

void PlayerActor::SetExhausted( bool exhausted )
{
	this->zExhausted = exhausted;

	if(this->zExhausted)
	{
		PlayerExhausted e;
		e.zPos = this->GetPosition();
		NotifyObservers(&e);
	}
}

void PlayerActor::SetBowStart()
{
	if(!this->zUsingBow)
	{
		this->zUsingBow = true;
		this->zBowTimeStarted = std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now());
	}
}

float PlayerActor::GetBowTimer()
{
	if(this->zUsingBow)
	{
		float timeSenseStartShot = (std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now()) - zBowTimeStarted).count() * 0.000001f;
		if(timeSenseStartShot > TAKEARROWANIMLENGTH && !this->zPlayer->GetKeys().GetKeyState(MOUSE_LEFT_PRESS))
		{
			this->zUsingBow = false;
			return timeSenseStartShot;
		}	
	}
	return -1;
}

#include "AIBearBehavior.h"
#include <time.h>


AIBearBehavior::AIBearBehavior( Actor* actor, World* world ) : AIBehavior(actor, world)
{
	InitValues();
}

bool AIBearBehavior::InitValues()
{
	this->InitPathfinder();

	/*Target vectorFilling; 
	vectorFilling.health = 0;
	vectorFilling.movementNoise = 0;
	vectorFilling.position = Vector3(0,0,0);
	vectorFilling.valid = false;

	for(int i = 0; i < 32; i++)
	{
		this->zTargets.push_back(vectorFilling);
	}*/

	this->zIntervalCounter = 0;

	if(!this->SetValuesFromFile() ) 
	{
		//Values could not be retrieved from file, set default values.
		this->zFearInterval = 1;
		this->zMinimumDistance = 40;
		this->zFieldOfView = 0.4f;
		this->zThreatMovementSpeedThreshold = 2.0f;
		this->zConfidenceKoef = 1.0f;
		this->zExtraFearWithNumberOfPlayers = 10.0f;
		this->zExtraFearWithCloseProximity = 12.0f;
		this->zExtraFearAtSight = 4.0f;
		this->zExtraFearAtThreatMovementSpeed = 8.0f;
		this->zFearDecrease = 2.0f;
		this->zFearAtDamageDone = 30.0f;
		this->zCalmToAggressiveThreshold = 20.0f;
		this->zDistanceToWalkWhenCalm = 14;
		this->zDistanceToWalkWhenSuspicious = 8;
		this->zNewTargetCloseByAFactorOf = 4;
		this->zFleeDistance = 40;
		this->zLargeSoundEventFearIncrease = 60;
		this->zCalmRandomInterval = 5;
		this->zCalmRandomAddition = 3;

		this->zWalkingVelocity = 2.0f;
		this->zAttackingVelocity = 5.0f;
		this->zFleeingVelocity = 4.8f;
	}
	this->SetFearLevel(0.0f);
	this->zFearMax = 200.0f;
	BearActor* dActor = dynamic_cast<BearActor*>(this->zActor);
	this->SetPreviousHealth( dActor->GetHealth());

	
	this->zFearIntervalCounter = 0.0f;
	this->zIntervalCounter = 0.0f;

	return true;
}

bool AIBearBehavior::SetValuesFromFile()
{
	float numberFromFile = 0;
	bool valuesRetrieved = false;
	char characters[16];

	std::ifstream infile("bearVariables.txt");
	if(infile.good()) //Getting values from a text file needs to be more properly implemented
	{
		//fear interval
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFearInterval = numberFromFile;

		//minimum distance
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zMinimumDistance = (int)numberFromFile;

		//field of view
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFieldOfView = numberFromFile;

		//Threat movement speed threshold
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zThreatMovementSpeedThreshold = numberFromFile;

		//Confidence koef
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zConfidenceKoef = numberFromFile;

		//Distance too close for comfort
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zTooCloseInDistance = numberFromFile;

		//extra fear with more players
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearWithNumberOfPlayers = numberFromFile;

		//extra fear with something too close
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearWithCloseProximity = numberFromFile;

		//extra fear when seeing threat
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearAtSight = numberFromFile;

		//extra fear when threat moves too quickly
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zExtraFearAtThreatMovementSpeed = numberFromFile;

		//amount of fear decrease
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFearDecrease = numberFromFile;

		//fear when hurt
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFearAtDamageDone = numberFromFile;

		//fear level between calm and aggressive.
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zCalmToAggressiveThreshold = numberFromFile;

		//distance to walk when calm
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zDistanceToWalkWhenCalm = (int)numberFromFile;

		//distance to walk when suspicious
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zDistanceToWalkWhenSuspicious = (int)numberFromFile;

		//factor to use when chosing a new target
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zNewTargetCloseByAFactorOf = (int)numberFromFile;

		//flee distance
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFleeDistance = (int)numberFromFile;

		//How afraid to be when a large sound even happens
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zLargeSoundEventFearIncrease = (int)numberFromFile;

		//The range of the pausing time when calm.
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zCalmRandomInterval = (int)numberFromFile;

		//The minimum value 
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zCalmRandomAddition = (int)numberFromFile;

		//the walking velocity
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zWalkingVelocity = numberFromFile;

		//the velocity when attacking
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zAttackingVelocity = numberFromFile;
		
		//the fleeing velocity
		infile.getline(characters, 256,' ');
		infile.ignore(256, '\n');
		numberFromFile = (float)atof(characters);
		this->zFleeingVelocity = numberFromFile;

		valuesRetrieved = true;

	}

	if(infile.is_open())
	{
		infile.close();
	}

	return valuesRetrieved;

}

void AIBearBehavior::SetFearLevel(float fear)
{
	this->zFearLevel = fear;
	if(this->zFearLevel > this->zFearMax)
	{
		this->zFearLevel = this->zFearMax;
	}
	else if(this->zFearLevel < 0.0f)
	{
		this->zFearLevel = 0.0f;
	}
}

bool AIBearBehavior::InitPathfinder()
{
	this->zPathfinder.InitAI(0.5f, zWorld->GetWorldSize().x);
	this->zPathfinder.SetWorldPointer(this->zWorld);
	return true;
}

Vector3 AIBearBehavior::ExaminePathfindingArea()
{
	BearActor* bActor = dynamic_cast<BearActor*>(this->zActor);

	Vector3 dest;
	//We make a check to see if the position is available, to avoid wasting time in the pathfinder functions.
	//dest = this->zMainTarget.position - dActor->GetPosition();
	dest = this->zMainActorTarget->GetPosition() - bActor->GetPosition();
	dest.Normalize();
	dest = dest * -1;
	dest *= (float)this->zFleeDistance;

	dest = bActor->GetPosition() + dest;
				
	bool getEmergencyDirection = false;
	bool foundPath = false;
	getEmergencyDirection = false;


	if(dest.x < 0)
	{
		dest.x = 0;
		foundPath = true;
	}
	else if(dest.x > this->zWorld->GetWorldSize().x)
	{
		dest.x = this->zWorld->GetWorldSize().x - 1; 
		foundPath = true;
	}
	if(dest.z < 0)
	{
		dest.z = 0;
		foundPath = true;
	}
	else if(dest.z > this->zWorld->GetWorldSize().y)
	{
		dest.z = this->zWorld->GetWorldSize().y - 1;
		foundPath = true;
	}

		getEmergencyDirection = false;
		
		//this while loop takes a lot of work. Try to optimize however possible.
		//if(this->zWorld->IsBlockingAt(Vector2(dest.x, dest.z)))
		//{
		//	getEmergencyDirection = true;
		//	//dest = (dest - dActor->GetPosition());
		//	//dest = dActor->GetPosition() + (dest - dest * 0.75);
		//				
		//}
		//if(( dest - dActor->GetPosition() ).GetLength() < 2)
		//{
		//	getEmergencyDirection = true; //There is basically a thick wall in the way...
		//}

		//if(getEmergencyDirection) //...so let's find another direction.
		//{
		//	//this->zDestination.Normalize();
		//	counter++;
		//	float angle = (-10 * counter) * 3.14f/180;
		//	float oldX, oldY;
		//	oldX = dest.x;
		//	oldY = dest.z;

		//	dest.x = cos(angle) * oldX - sin(angle) * oldY;
		//	dest.z = cos(angle) * oldY + sin(angle) * oldX;
		//	dest.Normalize();
		//	dest = dest * (float)zFleeDistance;
		//	
		//	foundPath = true;
		//	return dest;
		//			
		//}
		//else
		//{
		//	foundPath = true;
		//	return dest;
		//}
	//}
	return dest;
}

//void AIBearBehavior::SetTargetInfo(int number, Vector3 pos, float velocity, float health, Type kind)
//{
//	this->zTargets[number].position = pos;
//	this->zTargets[number].movementNoise = velocity;
//	this->zTargets[number].health = health;
//	this->zTargets[number].kind = kind;
//}

bool AIBearBehavior::Update( float dt )
{
	if ( Behavior::Update(dt) )
		return true;

	BearActor* bActor = dynamic_cast<BearActor*>(this->zActor);

	this->zIntervalCounter += dt;
	this->zFearIntervalCounter += dt;
	
	int nrOfPredators = 0;
	bool nearbyPredatorsExist = false;


	Vector3 bearTestPos = bActor->GetPosition();
	if(bearTestPos.x <= 0.0f)
	{
		int testVAlue = 0;
		testVAlue = 23;
	}


	//Perform checking for entities here.

	int shortestDistance = 99999;

	float finalDistance = 0.0f;

	int maximumNodesTest = 5;

	bool attackAnim = false;
	
	//Determine closest threat/target
	//for(int i = 0; i < this->GetCurrentTargets(); i++)
	//{
	//	xDistance = bActor->GetPosition().x - this->zTargets[i].position.x; //Math, could use optimization, I think.
	//	//yDistance = this->GetPosition().y - this->zTargets[i].position.y;
	//	zDistance = bActor->GetPosition().z - this->zTargets[i].position.z;
	//	finalDistance = sqrt(xDistance * xDistance + zDistance * zDistance);
	//	if( finalDistance < this->zMinimumDistance && bActor->GetPosition().x != this->zTargets[i].position.x && bActor->GetPosition().z != this->zTargets[i].position.z)  //Since everything could be prey to a bear, even other bears, this check is needed to make sure he does not make himself his enemy.
	//	{
	//		this->zTargets[i].valid = true;
	//
	//		if(finalDistance < shortestDistance)
	//		{
	//			shortestDistance = finalDistance;
	//			this->zMainTarget = this->zTargets[i]; //Decide which is the biggest threat here, i.e. the main target. For the moment, proximity is the deciding factor. Could use some more complexity.
	//		}
	//		nrOfPredators++;
	//	}
	//	else
	//	{
	//		this->zTargets[i].valid = false;
	//	}
	//	
	//}

	//std::set<Actor*> aSet = this->GetTargets();
	for(auto i = this->zNearDynamicActors.cbegin(); i != this->zNearDynamicActors.cend(); i++)//for(auto i = aSet.cbegin(); i != aSet.cend(); i++)
	{
		finalDistance = (bActor->GetPosition().GetXZ() - (*i)->GetPosition().GetXZ()).GetLength();
		if( finalDistance < this->zMinimumDistance && dynamic_cast<BioActor*>((*i)) && !dynamic_cast<BearActor*>((*i)) && dynamic_cast<BioActor*>((*i))->IsAlive())  //Since everything could be prey to a bear, even other bears, this check is needed to make sure he does not make himself his enemy.
		{
			dynamic_cast<BioActor*>((*i))->zValid = true;

			if(finalDistance < shortestDistance)
			{
				shortestDistance = (int)finalDistance;
				this->zMainActorTarget = (*i);//Decide which is the biggest threat here, i.e. the main target. For the moment, proximity is the deciding factor. Could use some more complexity.		 
			}
			nrOfPredators++;
		}
		else if( dynamic_cast<BioActor*>((*i)) )
		{
			dynamic_cast<BioActor*>((*i))->zValid = false;
		}
		
	}

	if(nrOfPredators > 0)
	{
		nearbyPredatorsExist = true;
	}
	else
	{
		nearbyPredatorsExist = false;
	}

	
	//Time to assess threats.

	if( bActor->GetHealth() < this->GetPreviousHealth() ) //In theory, used to check if the animal has been attacked.
	{
		this->SetFearLevel( this->GetFearLevel() + 1);
	}
	this->SetPreviousHealth( bActor->GetHealth() );
	
	if(this->zFearIntervalCounter > 1)
	{
		this->zFearIntervalCounter = 0;
		//Basically, each second the fear increases or decreases.
		//NOTE: A bear's fear basically only indicates how much he wants to kill you.

		if(nearbyPredatorsExist)
		{
			//Getting values and such, comparing animal health against that of other entities, types and more.

			//calculate current fear level:
			float fear = 0;
			//- for each dangerous entity detected.
			//fear += 2 * nrOfPredators;
			
			if(shortestDistance < this->zTooCloseInDistance) //The target is too close. Could expand this to incorporate more than one target.
			{
				fear += this->zExtraFearWithCloseProximity;
			}

			//std::set<Actor*> aSet = this->GetTargets();
			for(auto i = this->zNearDynamicActors.cbegin(); i != this->zNearDynamicActors.cend(); i++)//for(auto i = aSet.cbegin(); i != aSet.cend(); i++)
			{
				if(dynamic_cast<BioActor*>((*i)))
				{
					if(dynamic_cast<BioActor*>((*i))->zValid == true)
					{
					//Do a mathematical check, see if anyone is right in front of the bear. But... how? http://www.youtube.com/watch?v=gENVB6tjq_M
						float dotProduct = bActor->GetDir().GetDotProduct( (*i)->GetPosition() - bActor->GetPosition() );
						if(dotProduct > this->zFieldOfView)//This sight is relatively narrrow, since it is a bear. If this is true, then the bear sees a target.
						{
							//Which means, it is even more likely to target someone.
							fear += this->zExtraFearAtSight;
						}
						if(dynamic_cast<BioActor*>((*i))->GetVelocity() > this->zThreatMovementSpeedThreshold) //5 is just a temporary number right now. It is supposed to be the speed of a running target.
						{
							fear += this->zExtraFearAtThreatMovementSpeed;
						}

						if(dynamic_cast<BioActor*>((*i))->GetHealth() != 0) // No dbz here!
						{
							//fear -= (bActor->GetHealth() / dynamic_cast<BioActor*>((*i))->GetHealth()) / nrOfPredators;
						}
					}
				}
			}			

			//for(int i = 0; i < this->GetCurrentTargets(); i++)
			//{
			//	if(this->zTargets[i].valid == true)
			//	{
			//	//Do a mathematical check, see if anyone is right in front of the bear. But... how? http://www.youtube.com/watch?v=gENVB6tjq_M
			//		float dotProduct = bActor->GetDir().GetDotProduct( this->zTargets[i].position - bActor->GetPosition() );
			//		if(dotProduct > this->zFieldOfView)//This sight is relatively narrrow, since it is a bear. If this is true, then the bear sees a target.
			//		{
			//			//Which means, it is even more likely to target someone.
			//			fear += this->zExtraFearAtSight;
			//		}
			//		if(this->zTargets[i].movementNoise > this->zThreatMovementSpeedThreshold) //5 is just a temporary number right now. It is supposed to be the speed of a running target.
			//		{
			//			fear += this->zExtraFearAtThreatMovementSpeed;
			//		}
			//
			//		if(this->zTargets[i].health != 0) // No dbz here!
			//		{
			//			fear -= (bActor->GetHealth() / this->zTargets[i].health) / nrOfPredators; //If the animal is faced with a very weak player(s), it is actually less likely to attack. Like it's prey is unworthy or something.
			//		}
			//	}
			//}			

			this->SetFearLevel( this->GetFearLevel() + fear * this->zConfidenceKoef); //Possibly not good enough math. The theory is that the animal is constantly getting angrier
	
		}
		else //No threat detected. Calming down.
		{
			this->SetFearLevel( this->GetFearLevel() - this->zFearDecrease);
			if(this->GetFearLevel() <= 0.0f)
			{
				this->SetFearLevel(0.0f);
			}
		}
	}
		
	
	float fear;
	fear = this->GetFearLevel();
	fear = 0.0f;
	
	//Change state of mind.
	if(this->GetFearLevel() == 0.0f)
	{
		this->SetMentalState(CALM);
	}
	/*else if(this->GetFearLevel() > 0 && this->GetFearLevel() <= 10)
	{
		this->SetBehaviour(SUSPICIOUS);
	}*/
	else if(this->GetFearLevel() > this->zCalmToAggressiveThreshold && nearbyPredatorsExist)
	{
		if(this->GetMentalState() != AGGRESSIVE)
		{
			this->zCurrentPath.clear();
			this->SetIfNeedPath(true);
			this->SetMentalState(AGGRESSIVE);
			//this->SetScale(Vector3(0.05f, 0.05f, 0.05f));
		}
	}
	else if(this->zPanic == true)
	{
		if(this->GetMentalState() != AFRAID)
		{
			this->zCurrentPath.clear();
			this->SetIfNeedPath(true);
			this->SetMentalState(AFRAID);
			//this->SetScale(Vector3(3.09f, 3.09f, 3.09f));
		}
	}
	else
	{
		this->SetMentalState(CALM);
	}
	
	//Act based on state of mind.
	if(this->GetMentalState() == CALM) //Relaxed behaviour. No threat detected.
	{
		this->zCurrentDistanceFled = 0;
		this->zPanic = false;
		if(this->zIntervalCounter > this->zCalmActionInterval && this->GetIfNeedPath())
		{
			this->zIntervalCounter = 0;
			srand((unsigned int)time(0));
			this->zCalmActionInterval = (float)(rand() % this->zCalmRandomInterval + this->zCalmRandomAddition); 
			this->zCurrentPath.clear(); //Since a new path is gotten, and the old one might not have been completed, we clear it just in case.
			//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->GetPosition().x + rand() % 14 - 7, this->GetPosition().z + rand() % 14 - 7, this->zCurrentPath, 20); //Get a small path to walk, short and does not have to lead anywhere.
			this->zPathfinder.Pathfinding(bActor->GetPosition().x, bActor->GetPosition().z, bActor->GetPosition().x + rand() % this->zDistanceToWalkWhenCalm - this->zDistanceToWalkWhenCalm/2, bActor->GetPosition().z + rand() % this->zDistanceToWalkWhenCalm - this->zDistanceToWalkWhenCalm/2, this->zCurrentPath, maximumNodesTest);
			this->SetIfNeedPath(false);
		}
	}
	//else if(this->GetBehaviour() == SUSPICIOUS) //Might have heard something, is suspicious.
	//{
	//	this->zPanic = false;
	//	if(this->zIntervalCounter > this->zCalmActionInterval && this->GetIfNeedPath()) //The increase in time is supposed to represent listening, waiting for something to happen.
	//	{
	//		this->zIntervalCounter = 0;
	//		srand(time(NULL));
	//		this->zCalmActionInterval = rand() % 8 + 6;
	//		this->zCurrentPath.clear();
	//		//this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, this->GetPosition().x + rand() % 8 - 4, this->GetPosition().z + rand() % 8 - 4, this->zCurrentPath, 20); //Get a small path to walk, quite short (since the animal is nervous) and does not have to lead anywhere.
	//		this->zPathfinder.Pathfinding(this->GetPosition().x, this->GetPosition().z, this->GetPosition().x + rand() % 14 - 7, this->GetPosition().z + rand() % 14 - 7, this->zCurrentPath, 20);
	//		this->SetIfNeedPath(false);
	//	}
	//}
	else if(this->GetMentalState() == AGGRESSIVE) //Is outright trying to harm the target.
	{
		this->zCurrentDistanceFled = 0;
		this->zPanic = false;

		if(this->GetIfNeedPath() == true)
		{
			this->SetIfNeedPath(false);

			if(nearbyPredatorsExist)
			{
				
				this->zDestination = this->zMainActorTarget->GetPosition();	

			}
		}

		if( this->zIntervalCounter > 1.5 && this->GetIfNeedPath() == false )
		{
			this->zIntervalCounter = 0;

			this->zDestination = this->zMainActorTarget->GetPosition();

			float distance;

			distance = (bActor->GetPosition().GetXZ() - this->zDestination.GetXZ()).GetLength();

			if(distance < 1.5f)
			{
				//Vector3 direction = (*i)->GetPosition() - bActor->GetPosition();
				//direction.Normalize();
				//bActor->SetDir( direction ); 
				//float dotProduct = bActor->GetDir().GetDotProduct( (*i)->GetPosition() - bActor->GetPosition() );
				//if(dotProduct > this->zFieldOfView)//It is looking at the target.
				//{
					//Attack!
					attackAnim = true;
					Damage bearAttack;
					bearAttack.slashing = 15;
					if(dynamic_cast<BioActor*>(this->zMainActorTarget)->IsAlive())
					{
						dynamic_cast<BioActor*>(this->zMainActorTarget)->TakeDamage(bearAttack,this->GetActor());
					}
				//}	
							
			}
		}
	}
	else if(this->GetMentalState() == AFRAID) //Is afraid, needs to run.
	{

		if(this->GetIfNeedPath() == true)
		{
			this->SetIfNeedPath(false);

			if(nearbyPredatorsExist)
			{
				
				this->zDestination = this->ExaminePathfindingArea();				
				this->zCurrentPath.clear();
				if(!this->zPathfinder.Pathfinding(bActor->GetPosition().x, bActor->GetPosition().z, this->zDestination.x, this->zDestination.z,this->zCurrentPath,maximumNodesTest) ) //!this->zPathfinder.Pathfinding(this->GetPosition().z, this->GetPosition().x, awayFromThreatX, awayFromThreatZ,this->zCurrentPath,80)
				{
					this->SetIfNeedPath(true);
				}

			}
			else
			{
				this->zPanic = false;
			}
			
			
		}
	}



	//Move the animal.
		this->zPreviousVelocity = bActor->GetVelocity();
		Vector3 oldPos = bActor->GetPosition();
		this->zPanic = false;
		
		//this->zPreviousPos = this->GetPosition();
	
		if(this->GetMentalState() == CALM && this->zCurrentPath.size() > 0 || this->GetMentalState() == SUSPICIOUS && this->zCurrentPath.size() > 0)
		{

			

			bool reachedNode = false;
			if( (bActor->GetPosition().x > this->zCurrentPath.back().x - 0.2 && bActor->GetPosition().x < this->zCurrentPath.back().x + 0.2) && ( bActor->GetPosition().z > this->zCurrentPath.back().y - 0.2 && bActor->GetPosition().z < this->zCurrentPath.back().y + 0.2 ) )
			{
				reachedNode = true;
			}

			if(reachedNode)
			{
				this->zCurrentPath.pop_back();
				//reachedNode = false;
			}
			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );

			result = result;
			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */

			if(this->zCurrentPath.size() > 0)
			{
							
				Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
				Vector3 direction = goal - bActor->GetPosition();
				direction.Normalize();
				bActor->SetDir( direction ); 

				Vector2 testProperDirection;
				testProperDirection.x = bActor->GetDir().x;
				testProperDirection.y = bActor->GetDir().z;
				if(testProperDirection.GetLength() > 0.0f)
				{
					testProperDirection.Normalize();
				}
				bActor->SetDir(Vector3(testProperDirection.x, 0.0f, testProperDirection.y));

			}

			bActor->SetVelocity(this->zWalkingVelocity);
			dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_WALKING);
			bActor->SetPosition(bActor->GetPosition() + bActor->GetDir() * dt * bActor->GetVelocity());

		}
		else if(this->GetMentalState() == AGGRESSIVE /* && this->zCurrentPath.size() > 0*/)
		{

			//Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
			//Vector3 direction = goal - dActor->GetPosition();
			
			if(this->zCurrentPath.size() > 0)
			{
				bool reachedNode = false;
				if( (bActor->GetPosition().x > this->zCurrentPath.back().x - 0.2 && bActor->GetPosition().x < this->zCurrentPath.back().x + 0.2) && ( bActor->GetPosition().z > this->zCurrentPath.back().y - 0.2 && bActor->GetPosition().z < this->zCurrentPath.back().y + 0.2 ) )
				{
					reachedNode = true;
				}

				if(reachedNode)
				{
					this->zCurrentPath.pop_back();
					//reachedNode = false;
				}
			}

			if(this->zCurrentPath.size() > 0)
			{
				Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
				Vector3 direction = goal - bActor->GetPosition();
				direction.Normalize();
				bActor->SetDir( direction ); 
			}
			else
			{
				Vector3 direction = this->zDestination - bActor->GetPosition();
				direction.Normalize();
				bActor->SetDir( direction ); 
			}

			bActor->SetVelocity(this->zAttackingVelocity);

			Vector3 nextPos = bActor->GetPosition() + bActor->GetDir() * dt * bActor->GetVelocity();


			

			if(!this->zWorld->IsBlockingAt(Vector2(nextPos.x,nextPos.z)) && this->zCurrentPath.size() == 0)
			{
				Vector2 testProperDirection;
				testProperDirection.x = bActor->GetDir().x;
				testProperDirection.y = bActor->GetDir().z;
				if(testProperDirection.GetLength() > 0.0f)
				{
					testProperDirection.Normalize();
				}
				
				bActor->SetDir(Vector3(testProperDirection.x, 0.0f, testProperDirection.y));

				dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_RUNNING);
				bActor->SetPosition(bActor->GetPosition() + bActor->GetDir() * dt * bActor->GetVelocity());
				this->zCurrentDistanceFled += dt * bActor->GetVelocity();

			}
			else if(this->zCurrentPath.size() > 0)
			{
				Vector2 testProperDirection;
				testProperDirection.x = bActor->GetDir().x;
				testProperDirection.y = bActor->GetDir().z;
				if(testProperDirection.GetLength() > 0.0f)
				{
					testProperDirection.Normalize();
				}
				bActor->SetDir(Vector3(testProperDirection.x, 0.0f, testProperDirection.y));

				dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_RUNNING);
				bActor->SetPosition(bActor->GetPosition() + bActor->GetDir() * dt * bActor->GetVelocity());
				this->zCurrentDistanceFled += dt * bActor->GetVelocity();

			}
			else if(this->zCurrentPath.size() == 0)
			{
				this->zCurrentPath.clear();
				this->zPathfinder.Pathfinding(bActor->GetPosition().x, bActor->GetPosition().z,  bActor->GetPosition().x + bActor->GetDir().x * 5.0f,  bActor->GetPosition().z + bActor->GetDir().z * 5.0f, this->zCurrentPath, 40);

				//dActor->SetPosition(Vector3(50,0,50));
			}

		//	dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_RUNNING);
		

		}
		else if(this->GetMentalState() == AFRAID /*&& this->zCurrentPath.size() > 0*/)
		{
			/*double result = atan2( (this->zCurrentPath.back().y - this->GetPosition().z), (this->zCurrentPath.back().x - this->GetPosition().x) );

			result = result;
			this->SetDirection( Vector3( cos(result), 0.0f, sin(result) )); */

			
			
			//Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
			//Vector3 direction = goal - dActor->GetPosition();
			
			if(this->zCurrentPath.size() > 0)
			{
				bool reachedNode = false;
				if( (bActor->GetPosition().x > this->zCurrentPath.back().x - 0.2 && bActor->GetPosition().x < this->zCurrentPath.back().x + 0.2) && ( bActor->GetPosition().z > this->zCurrentPath.back().y - 0.2 && bActor->GetPosition().z < this->zCurrentPath.back().y + 0.2 ) )
				{
					reachedNode = true;
				}

				if(reachedNode)
				{
					this->zCurrentPath.pop_back();
					//reachedNode = false;
				}
			}

			if(this->zCurrentPath.size() > 0)
			{
				Vector3 goal(this->zCurrentPath.back().x, 0, this->zCurrentPath.back().y);
				Vector3 direction = goal - bActor->GetPosition();
				direction.Normalize();
				bActor->SetDir( direction ); 
			}
			else
			{
				Vector3 direction = this->zDestination - bActor->GetPosition();
				direction.Normalize();
				bActor->SetDir( direction ); 
			}

			bActor->SetVelocity(this->zFleeingVelocity);

			Vector3 nextPos = bActor->GetPosition() + bActor->GetDir() * dt * bActor->GetVelocity();


			

			if(!this->zWorld->IsBlockingAt(Vector2(nextPos.x,nextPos.z)) && this->zCurrentPath.size() == 0)
			{
				Vector2 testProperDirection;
				testProperDirection.x = bActor->GetDir().x;
				testProperDirection.y = bActor->GetDir().z;
				testProperDirection.Normalize();
				bActor->SetDir(Vector3(testProperDirection.x, 0.0f, testProperDirection.y));
				dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_RUNNING);
				bActor->SetPosition(bActor->GetPosition() + bActor->GetDir() * dt * bActor->GetVelocity());
				this->zCurrentDistanceFled += dt * bActor->GetVelocity();

			}
			else if(this->zCurrentPath.size() > 0)
			{
				Vector2 testProperDirection;
				testProperDirection.x = bActor->GetDir().x;
				testProperDirection.y = bActor->GetDir().z;
				testProperDirection.Normalize();
				bActor->SetDir(Vector3(testProperDirection.x, 0.0f, testProperDirection.y));
				dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_RUNNING);
				bActor->SetPosition(bActor->GetPosition() + bActor->GetDir() * dt * bActor->GetVelocity());
				this->zCurrentDistanceFled += dt * bActor->GetVelocity();

			}
			else if(this->zCurrentPath.size() == 0)
			{
				this->zCurrentPath.clear();
				this->zPathfinder.Pathfinding(bActor->GetPosition().x, bActor->GetPosition().z,  bActor->GetPosition().x + bActor->GetDir().x * 5.0f,  bActor->GetPosition().z + bActor->GetDir().z * 5.0f, this->zCurrentPath, 40);

				//dActor->SetPosition(Vector3(50,0,50));
			}
		}
		
		if(this->GetMentalState() == AFRAID && this->zCurrentDistanceFled < this->zFleeDistance)
		{
			this->SetIfNeedPath(true);
		}
		else
		{
			this->SetIfNeedPath(true);
		}
		
		float groundHeight = 0.0f;
		try
		{
			groundHeight = this->zWorld->CalcHeightAtWorldPos( Vector2(bActor->GetPosition().x, bActor->GetPosition().z));
		}
		catch(...)
		{

		}
		
		Vector3 actorPosition = bActor->GetPosition();
		Vector3 currentPos = actorPosition;
		actorPosition.y = groundHeight;
		bActor->SetPosition(actorPosition);
	
	if(oldPos == currentPos)
	{
		dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_IDLE);		
	}

	if(attackAnim)
	{
		dynamic_cast<BioActor*>(this->GetActor())->SetState(STATE_ATTACK_P);
	}

	//Rotate Animal
	static Vector3 defaultMeshDir = Vector3(0.0f, 0.0f, 1.0f);
	Vector3 meshDirection = bActor->GetDir();
	meshDirection.y = 0;
	meshDirection.Normalize();

	Vector3 around = Vector3(0.0f, 1.0f, 0.0f);
	float angle = -acos(meshDirection.GetDotProduct(defaultMeshDir));

	if (meshDirection.x > 0.0f)
	 angle *= -1;

	bActor->SetRotation(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	bActor->SetRotation(around, angle);

	return false;

}
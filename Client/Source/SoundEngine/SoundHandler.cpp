#include "SoundHandler.h"
SoundHandler::SoundHandler()
{	
	this->zSystem = 0;

	this->zSoundChannel = 0;
	this->zMusicChannel = 0;
	this->zIsPaused = 0;
	this->zIsMuted = 0;

	this->zLandGeometry = 0;
	this->zObjectGeometry = 0;

	this->zNrOfGeometryObjects = 0;

	//this->zEcho = 0;
	//this->zLowpass = 0;
	//this->zParameq = 0;

	this->zPlayerPos = FMOD_VECTOR();
	this->zPlayerPos.x = 0;
	this->zPlayerPos.y = 0;
	this->zPlayerPos.z = 0;

	this->zSoundPos = FMOD_VECTOR();
	this->zSoundPos.x = 0;
	this->zSoundPos.y = 0;
	this->zSoundPos.z = 0;

	this->zForwardVector = FMOD_VECTOR();
	this->zForwardVector.x = 0;
	this->zForwardVector.y = 0;
	this->zForwardVector.z = 0;

	FMOD_VECTOR up;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;
	
	this->zSystem->set3DListenerAttributes(0,&this->zPlayerPos,0,&this->zForwardVector,&up);
}
SoundHandler::~SoundHandler()
{
	FMOD_RESULT result;

	/*for(int i = 0; i < this->zNrOfSounds; i++)
	{
		result = this->zSounds[i]->release();
		ERRCHECK(result);
	}*/

	for(auto it = zSoundList.begin(); it != zSoundList.end(); it++)
	{
		it->sound->release();
	}

	for(auto it = zMusicList.begin(); it != zMusicList.end(); it++)
	{
		it->sound->release();
	}

	/*result = this->zTrack1->release();
	ERRCHECK(result);*/

	result = this->zSoundGroup->release();
	ERRCHECK(result);

	result = this->zMusicGroup->release();
	ERRCHECK(result);

	//TODO: Release geometry here.

	if ( zLandGeometry ) 
	{
		result = this->zLandGeometry->release();
		ERRCHECK(result);
	}

	for(int i = 0; i < this->zNrOfGeometryObjects; i++)
	{
		if ( zObjectGeometry[i] )
		{
			result = this->zObjectGeometry[i]->release();
			ERRCHECK(result);
		}
	}

	delete this->zObjectGeometry; //It might be that I need to do delete [] this->zObjectGeometry, I'm not sure.
	this->zObjectGeometry = 0;


	result = this->zSystem->close();
    ERRCHECK(result);

	result = this->zSystem->release();
    ERRCHECK(result);

	/*delete this->zLandscape;
	this->zLandscape = 0;

	delete this->zObjects;
	this->zObjects = 0;*/


}

int SoundHandler::Init() //This is a barebones initialization.
{
	FMOD_RESULT result;
	result = FMOD::System_Create(&this->zSystem);
	ERRCHECK(result);

	result = this->zSystem->getVersion(&zVersion);
	ERRCHECK(result);

	/*if(this->zVersion < FMOD_VERSION)
	{
		printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
        return 1;
	}*/
	result = this->zSystem->init(100, /*FMOD_INIT_3D_LEFTHANDED |*/ FMOD_INIT_OCCLUSION_LOWPASS | FMOD_INIT_HRTF_LOWPASS | FMOD_INIT_GEOMETRY_USECLOSEST, 0);	ERRCHECK(result);
	if (result == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        result = this->zSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        ERRCHECK(result);
                
        result = this->zSystem->init(100, /*FMOD_INIT_3D_RIGHTHANDED |*/ FMOD_INIT_OCCLUSION_LOWPASS | FMOD_INIT_HRTF_LOWPASS | FMOD_INIT_GEOMETRY_USECLOSEST, 0);/* ... and re-init. */
        ERRCHECK(result);
    };

	
	//Initiating sounds

	//result = this->zSystem->createSound("Media/Sounds/SoundEffects/Ambient/forestTest.mp3", FMOD_SOFTWARE | FMOD_3D, 0, &this->zSounds[0]);
 //   ERRCHECK(result);
	//result = zSounds[0]->set3DMinMaxDistance(1.0f,10000.0f);
	//ERRCHECK(result);

 //   result = this->zSounds[0]->setMode(FMOD_LOOP_OFF);    // drumloop.wav has embedded loop points which automatically makes looping turn on,
 //   ERRCHECK(result);                           // so turn it off here.  We could have also just put FMOD_LOOP_OFF in the above CreateSound call. 

	//result = this->zSystem->createSound("Media/Sounds/Songs/america_fuck_yeah.mp3",FMOD_SOFTWARE,0,&this->zTrack1);
	//ERRCHECK(result);
	//result = this->zTrack1->setMode(FMOD_LOOP_NORMAL); //To make sure the song loops. Songs should have loop points to automatically loop at the right places.
	//ERRCHECK(result);

	

	
	//Create the groups. This will allow us to at least set volumes on all sounds.

	result = this->zSystem->createChannelGroup("Sound group",&this->zSoundGroup);
	ERRCHECK(result);

	result = this->zSystem->createChannelGroup("Music group",&this->zMusicGroup);
	ERRCHECK(result);

	result = this->zSystem->getMasterChannelGroup(&this->zMasterGroup);
	ERRCHECK(result);

	
	result = this->zMasterGroup->addGroup(this->zSoundGroup);
	ERRCHECK(result);

	result = this->zMasterGroup->addGroup(this->zMusicGroup);
	ERRCHECK(result);

	//Initializing geometry objects(might not be used here)
	//this->InitLandGeometry();
	
	//this->InitObjectGeometry();



	////Initiating effects

	//result = this->zSystem->createDSPByType(FMOD_DSP_TYPE_ECHO, &this->zEcho);
	//ERRCHECK(result);

	//result = this->zSystem->createDSPByType(FMOD_DSP_TYPE_LOWPASS,&this->zLowpass);
	//ERRCHECK(result);

	//result = this->zSystem->createDSPByType(FMOD_DSP_TYPE_PARAMEQ,&this->zParameq);
	//ERRCHECK(result);


	result = this->zSystem->setGeometrySettings(10000.0f);
	ERRCHECK(result);

	return 0;
}

bool SoundHandler::LoadSoundIntoSystem(const char* filename, bool loop) throw (...)
{

	FMOD_RESULT result;


	std::ifstream checkForFile(filename);
	if(checkForFile.good())
	{
		FMOD::Sound* soundToInsert = NULL;

		SoundStruct soundStructToInsert(soundToInsert, filename);

		this->zSoundList.push_front( soundStructToInsert );

		result = this->zSystem->createSound(filename, FMOD_SOFTWARE | FMOD_3D, 0, &this->zSoundList.front().sound);
		ERRCHECK(result);
		result = this->zSoundList.front().sound->set3DMinMaxDistance(1.0f,10000.0f);
		ERRCHECK(result);

		if(!loop)
		{
			result = this->zSoundList.front().sound->setMode(FMOD_LOOP_OFF);
			ERRCHECK(result);                          
		}
		else
		{
			result = this->zSoundList.front().sound->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		if(checkForFile.is_open())
		{
			checkForFile.close();
		}
		return true;
	}
	else
	{
		throw("File using filepath given was not found");
		return false;
		//File did not exist. That's just plain wrong.
	}

	
}

bool SoundHandler::LoadMusicIntoSystem(const char* filename, bool loop) throw (...)
{
	FMOD_RESULT result;

	std::ifstream checkForFile(filename);
	if(checkForFile.good())
	{
		FMOD::Sound* soundToInsert = NULL;

		SoundStruct soundStructToInsert(soundToInsert, filename);

		this->zMusicList.push_front( soundStructToInsert );

		result = this->zSystem->createSound(filename, FMOD_SOFTWARE | FMOD_3D, 0, &this->zMusicList.front().sound);
		ERRCHECK(result);
		result = this->zMusicList.front().sound->set3DMinMaxDistance(1.0f,10000.0f);
		ERRCHECK(result);

		if(!loop)
		{
			result = this->zMusicList.front().sound->setMode(FMOD_LOOP_OFF);
			ERRCHECK(result);                          
		}
		else
		{
			result = this->zMusicList.front().sound->setMode(FMOD_LOOP_NORMAL);
			ERRCHECK(result);
		}
		if(checkForFile.is_open())
		{
			checkForFile.close();
		}
		return true;
	}
	else
	{
		throw("File using filepath given was not found");
		return false;
		
		//File did not exist. That's just plain wrong.
	}
}


//void SoundHandler::InitLandGeometry()
//{
//	FMOD_RESULT result;
//
//	//Get the number of vertices.
//	int numberOfVertices = 1200; //simple test value;
//
//	//I think that based on topology, the number of polygons and vertices can be different in unexpected ways, maybe.
//	//I might need to simply ask for help when I come to that stage.
//	int numberOfPolygons = 400; //simple test value;
//
//
//	//Get a way to read vertices from the ground.
//		//Probably read from a file.
//
//
//
//
//	//Creating ground geometry to block sound.
//	result = this->zSystem->createGeometry(numberOfPolygons, numberOfVertices, &this->zLandGeometry);
//	ERRCHECK(result);
//
//	int polygonIndex = 0;
//
//	//Add the polygons to the geometry
//	//this->zLandGeometry->addPolygon(
//	
//
//
//
//}

//void SoundHandler::InitObjectGeometry()
//{
//	//get the number of all trees, rocks, buildings, whatever in the level.
//	this->zNrOfGeometryObjects = 1; //dummy thing
//	this->zObjectGeometry = new FMOD::Geometry*[this->zNrOfGeometryObjects];
//
//	for(int i = 0; i < this->zNrOfGeometryObjects; i++)
//	{
//		//Get number of vertices for object number i
//		int nrOfVertices = 3;
//		
//		//Decide polygons
//		int nrOfPolygons = 1;
//
//		
//
//
//		this->zSystem->createGeometry(nrOfPolygons, nrOfVertices, &this->zObjectGeometry[i]);
//
//		int polygonIndex = 0;
//
//		//add polygons to the geometry thing.
//		//this->zObjectGeometry[i]->addPolygon
//
//	}
//
//}

//void SoundHandler::clearGeometry()
//{
//	
//}

void SoundHandler::PlaySounds(char* filename, Vector3 p_sPos)
{
	FMOD_RESULT result;
	this->zSoundPos.x = p_sPos.x;
	this->zSoundPos.y = p_sPos.y;
	this->zSoundPos.z = p_sPos.z;

	//FMOD_VECTOR vel = {0.0f,0.0f,0.0f}; //This is not currently necessary.

	bool found = false;
	bool rectified = false;

	for(auto it = zSoundList.begin(); it != zSoundList.end(); it++)
	{
		if(it->name == filename)
		{
				result = this->zSystem->playSound(FMOD_CHANNEL_FREE, it->sound, false, &this->zSoundChannel);
				ERRCHECK(result);
				
				result = this->zSoundChannel->setChannelGroup(this->zSoundGroup);
				ERRCHECK(result);
			
				result = this->zSoundChannel->set3DAttributes(&this->zSoundPos,0);
				ERRCHECK(result);
			
				found = true;
				break;
				//result = this->zSoundChannel->set3DSpread(40.0f);//Unsure how to properly use this, needs to check more.
				//ERRCHECK(result);
		}
	}

	if(!found) //Specific sound could not be found out of loaded sounds, checking to see if it exists anyway.
	{
		rectified = LoadSoundIntoSystem(filename, false);     

		//Playing it,
		if(rectified) //if it exists.
		{
			result = this->zSystem->playSound(FMOD_CHANNEL_FREE, this->zSoundList.front().sound, false, &this->zSoundChannel);
			ERRCHECK(result);
			result = this->zSoundChannel->setChannelGroup(this->zSoundGroup);
			ERRCHECK(result);
			result = this->zSoundChannel->set3DAttributes(&this->zSoundPos,0);
			ERRCHECK(result);
		}

	}

	
	//switch (sound)
	//{
	//case 0: //test sound basically
	//	{
	//		
	//			result = this->zSystem->playSound(FMOD_CHANNEL_FREE, this->zSounds[0], false, &this->zSoundChannel);
	//			ERRCHECK(result);
	//			
	//			result = this->zSoundChannel->setChannelGroup(this->zSoundGroup);
	//			ERRCHECK(result);
	//			
	//			result = this->zSoundChannel->set3DAttributes(&this->zSoundPos,0);
	//			ERRCHECK(result);
	//			
	//			//result = this->zSoundChannel->set3DSpread(40.0f);//Unsure how to properly use this, needs to check more.
	//			//ERRCHECK(result);
	//		break;
	//	}
	//	
 //   
	//}
	//this->zSystem->update();
}

void SoundHandler::PauseAllAudio() //It's a bit odd right now.
{
	FMOD_RESULT result;

	if(this->zIsPaused)
	{
		this->zIsPaused = false;
	}
	else
	{
		this->zIsPaused = true;
	}

    bool         playing = 0;
    bool         paused = 0;
    int          channelsplaying = 0;

	if(this->zSoundChannel)
	{
		FMOD::Sound *currentsound = 0;

		result = this->zSoundChannel->isPlaying(&playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}
		this->zSoundChannel->setPaused(this->zIsPaused);
		result = zSoundChannel->getPaused(&paused);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}

	}
	if(this->zMusicChannel)
	{
		FMOD::Sound *currentsound = 0;

		result = this->zMusicChannel->isPlaying(&playing);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}
		this->zMusicChannel->setPaused(this->zIsPaused);
		result = this->zMusicChannel->getPaused(&paused);
		if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE) && (result != FMOD_ERR_CHANNEL_STOLEN))
		{
			ERRCHECK(result);
		}
	}
}

void SoundHandler::PlayMusic(char* filename)
{

	FMOD_RESULT result;

	//FMOD_VECTOR vel = {0.0f,0.0f,0.0f}; //This is not currently necessary.

	bool found = false;
	bool rectified = false;

	for(auto it = zSoundList.begin(); it != zSoundList.end(); it++)
	{
		if(it->name == filename)
		{
				result = this->zSystem->playSound(FMOD_CHANNEL_FREE, it->sound, false, &this->zSoundChannel);
				ERRCHECK(result);
				
				result = this->zSoundChannel->setChannelGroup(this->zSoundGroup);
				ERRCHECK(result);
			
				result = this->zSoundChannel->set3DAttributes(&this->zSoundPos,0);
				ERRCHECK(result);
			
				found = true;
				break;
				//result = this->zSoundChannel->set3DSpread(40.0f);//Unsure how to properly use this, needs to check more.
				//ERRCHECK(result);
		}
	}

	if(!found) //Sound could not be found among loaded sounds, checking to see if it exists anyway.
	{
		rectified = LoadMusicIntoSystem(filename, false);     

		//Playing it,
		if(rectified) //if it exists
		{
			result = this->zSystem->playSound(FMOD_CHANNEL_FREE, this->zMusicList.front().sound, false, &this->zMusicChannel);
			ERRCHECK(result);
			result = this->zMusicChannel->setChannelGroup(this->zMusicGroup);
			ERRCHECK(result);
		}

	}
	/*FMOD_RESULT result;

	for(auto it = zMusicList.begin(); it != zMusicList.end(); it++)
	{
		if(it->name == filename)
		{
				result = this->zSystem->playSound(FMOD_CHANNEL_FREE, it->sound, false, &this->zMusicChannel);
				ERRCHECK(result);
				
				result = this->zMusicChannel->setChannelGroup(this->zMusicGroup);
				ERRCHECK(result);

		}
	}*/



	/*switch (music)
	{
	case 0 :
		{
			result = this->zSystem->playSound(FMOD_CHANNEL_FREE, this->zTrack1, false, &this->zMusicChannel);
			ERRCHECK(result);
			result = this->zMusicChannel->setChannelGroup(this->zMusicGroup);
			ERRCHECK(result);
			break;
		}
	}*/
}

void SoundHandler::StopMusic()
{
	FMOD_RESULT result;

	if(zMusicChannel)
	{
		result = this->zMusicChannel->stop();
		ERRCHECK(result);
	}
}

//void SoundHandler::SetEffect(int effectNumber)
//{
//	FMOD_RESULT result;
//
//	switch(effectNumber)
//	{
//		case 1: //Echo
//		{
//			bool active;
//		
//			result = this->zEcho->getActive(&active);
//			ERRCHECK(result);
//
//			if(active)
//			{
//				result = this->zEcho->remove();
//				ERRCHECK(result);
//			}
//			else
//			{
//				result = this->zSoundGroup->addDSP(this->zEcho,0); 
//				ERRCHECK(result);
//
//				result = this->zEcho->setParameter(FMOD_DSP_ECHO_DELAY, 150.0f);
//                ERRCHECK(result);
//			}
//			break;
//		}
//		/*
//		//case 2: //Lowpass
//		//{
//		//	bool active;
//
//		//	result = dsplowpass->getActive(&active);
//		//	ERRCHECK(result);
//
//		//	if(active)
//		//	{
//		//		result = dsplowpass->remove();
//		//		ERRCHECK(result);
//		//	}
//		//	else
//		//	{
//		//		result = soundGroup->addDSP(dsplowpass,0);
//		//		ERRCHECK(result);	
//		//	}
//
//		//	break;
//		//}
//		//case 3: //parameq
//		//{
//		//	bool active;
//
//		//	result = dspparameq->getActive(&active);
//		//	ERRCHECK(result);
//
//		//	if(active)
//		//	{
//		//		result = dspparameq->remove();
//		//		ERRCHECK(result);
//		//	}
//		//	else
//		//	{
//		//		result = soundGroup->addDSP(dspparameq,0);
//		//		ERRCHECK(result);
//
//		//		result = dspparameq->setParameter(FMOD_DSP_PARAMEQ_CENTER, 5000.0f);
//		//		ERRCHECK(result);
//
//		//		result = dspparameq->setParameter(FMOD_DSP_PARAMEQ_GAIN, 0.0f);
//		//		ERRCHECK(result);
//		//	
//		//	}
//		//	break;
//		//}
//		*/
//
//
//	}
//
//
//}

float SoundHandler::GetSoundVolume()
{
	float volume;
	this->zSoundGroup->getVolume(&volume);
	return volume;
}

float SoundHandler::GetMusicVolume()
{
	float volume;
	this->zMusicGroup->getVolume(&volume);
	return volume;
}

float SoundHandler::GetMasterVolume()
{
	float volume;
	this->zMasterGroup->getVolume(&volume);
	return volume;
}

void SoundHandler::SetSoundVolume(float volume)
{
	FMOD_RESULT result;
	result = this->zSoundGroup->setVolume(volume);
	ERRCHECK(result);
}

void SoundHandler::SetMusicVolume(float volume)
{
	FMOD_RESULT result;
	result = this->zMusicGroup->setVolume(volume);
	ERRCHECK(result);
}

void SoundHandler::SetMasterVolume(float volume)
{
	FMOD_RESULT result;
	result = this->zMasterGroup->setVolume(volume);
	ERRCHECK(result);
}

void SoundHandler::Mute()
{
	FMOD_RESULT result;
	if(this->zIsMuted == 0)
	{
		this->zIsMuted = 1;
		result = this->zMasterGroup->setMute(this->zIsMuted);
	}
	else
	{
		this->zIsMuted = 0;
		result = this->zMasterGroup->setMute(this->zIsMuted);
	}
	ERRCHECK(result);
}


void SoundHandler::updateSystem()
{
	this->zSystem->update();
}

void SoundHandler::UpdateListenerPos(Vector3 p_pPos, Vector3 forward, Vector3 up)
{
	FMOD_RESULT result;
	this->zPlayerPos.x = p_pPos.x;
	this->zPlayerPos.y = p_pPos.y;
	this->zPlayerPos.z = p_pPos.z;

	
	this->zForwardVector.x = forward.x;
	this->zForwardVector.y = forward.y;
	this->zForwardVector.z = forward.z;

	
	this->zUpVector.x = up.x;
	this->zUpVector.y = up.y;	
	this->zUpVector.z = up.z;


									//Forward and up need to be perpendicular to each other.
	result = this->zSystem->set3DListenerAttributes(0,&this->zPlayerPos,0,&this->zForwardVector,&this->zUpVector);

	ERRCHECK(result);

}



void SoundHandler::ERRCHECK(FMOD_RESULT result)
{
	if(result != FMOD_OK)
	{
		//FMOD_ErrorString a=FMOD_ErrorString(result);
		//std::string error;
		//error = result;
		printf("FMOD error! (%d) %s\n", result, (char*)(result));
		exit(-1);
	}
}


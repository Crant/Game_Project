/*
	Made by Ed�nge Simon And Christopher Jensen Datum(23/11/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "Vector.h"
#include <vector>
#include <string>

enum MESSAGE_TYPE
{
	MESSAGE_TYPE_POSITION,
	MESSAGE_TYPE_DIRECTION,
	MESSAGE_TYPE_UP,
	MESSAGE_TYPE_SCALE,
	MESSAGE_TYPE_ROTATION,
	MESSAGE_TYPE_STATE,
	MESSAGE_TYPE_MESH_MODEL,
	MESSAGE_TYPE_USER_NAME,
	MESSAGE_TYPE_CONNECTION_CLOSED,
	MESSAGE_TYPE_CONNECTED,
	MESSAGE_TYPE_LOAD_MAP,
	MESSAGE_TYPE_SERVER_FULL,
	MESSAGE_TYPE_SERVER_SHUTDOWN,
	MESSAGE_TYPE_SERVER_RESTART,
	MESSAGE_TYPE_ERROR_MESSAGE,
	MESSAGE_TYPE_SERVER_ANNOUNCEMENT,
	MESSAGE_TYPE_IMPORTANT_MESSAGE,
	MESSAGE_TYPE_ACKNOWLEDGE,
	MESSAGE_TYPE_NEW_ACTOR,
	MESSAGE_TYPE_UPDATE_ACTOR,
	MESSAGE_TYPE_REMOVE_ACTOR,
	MESSAGE_TYPE_DEAD_ACTOR,
	MESSAGE_TYPE_KEY_DOWN,
	MESSAGE_TYPE_KEY_UP,
	MESSAGE_TYPE_PING,
	MESSAGE_TYPE_SELF_ID,
	MESSAGE_TYPE_KICKED,
	MESSAGE_TYPE_USER_DATA,
	MESSAGE_TYPE_FRAME_TIME,
	MESSAGE_TYPE_CLIENT_DATA,
	MESSAGE_TYPE_PICKUP_ITEM,
	MESSAGE_TYPE_LOOT_ITEM,
	MESSAGE_TYPE_LOOT_OBJECT,
	MESSAGE_TYPE_LOOT_OBJECT_RESPONSE,
	MESSAGE_TYPE_DROP_ITEM,
	MESSAGE_TYPE_ACTOR_TAKE_DAMAGE,
	MESSAGE_TYPE_HEALTH,
	MESSAGE_TYPE_HUNGER,
	MESSAGE_TYPE_HYDRATION,
	MESSAGE_TYPE_STAMINA,
	MESSAGE_TYPE_ITEM_TYPE,
	MESSAGE_TYPE_OBJECT_ID,
	MESSAGE_TYPE_REMOVE_INVENTORY_ITEM,
	MESSAGE_TYPE_ADD_INVENTORY_ITEM,
	MESSAGE_TYPE_EQUIPMENT_SLOT,
	MESSAGE_TYPE_REMOVE_EQUIPMENT,
	MESSAGE_TYPE_ITEM_USE,
	MESSAGE_TYPE_ITEM_FILL,
	MESSAGE_TYPE_ITEM_CRAFT,
	MESSAGE_TYPE_ITEM_NAME,
	MESSAGE_TYPE_ITEM_DESCRIPTION,
	MESSAGE_TYPE_ITEM_ICON_PATH,
	MESSAGE_TYPE_ITEM_SLOT_SIZE,
	MESSAGE_TYPE_ITEM_WEIGHT,
	MESSAGE_TYPE_ITEM_STACK_SIZE,
	MESSAGE_TYPE_WEAPON_DAMAGE,
	MESSAGE_TYPE_WEAPON_RANGE,
	MESSAGE_TYPE_CONTAINER_MAX,
	MESSAGE_TYPE_CONTAINER_CURRENT,
	MESSAGE_TYPE_WEAPON_USE,
	MESSAGE_TYPE_EQUIP_ITEM,
	MESSAGE_TYPE_UNEQUIP_ITEM,
	MESSAGE_TYPE_PROJECTILE_DAMAGE,
	MESSAGE_TYPE_PROJECTILE_VELOCITY,
	MESSAGE_TYPE_START_GAME,
	MESSAGE_TYPE_PLAYER_READY,
	MESSAGE_TYPE_ITEM_SUB_TYPE,
	MESSAGE_TYPE_ITEM_FINISHED,
	MESSAGE_TYPE_WEAPON_EQUIPMENT_SWAP,
	/*	
		NONE = 0
		HUMAN = 1
		GHOST = 2
		ANIMAL = 3
	*/
	MESSAGE_TYPE_ACTOR_TYPE,
	MESSAGE_TYPE_ACTOR_KILL,
	MESSAGE_TYPE_RESTART_GAME_REQUEST,
	/*
		DEER = 0
		WOLF = 1
		BEAR = 2
	*/
	MESSAGE_TYPE_PLAY_AS_ANIMAL,
	MESSAGE_TYPE_TRY_TO_POSSESS_ANIMAL,
	MESSAGE_TYPE_LEAVE_ANIMAL,
	MESSAGE_TYPE_MAKE_NOISE,
	MESSAGE_TYPE_DEER_EAT_OBJECT,
	MESSAGE_TYPE_PLAY_SOUND,
	MESSAGE_TYPE_PLAY_ANIMATION,
	MESSAGE_TYPE_SUN_DIRECTION,
	MESSAGE_TYPE_ANIMAL_ATTACK,
	MESSAGE_TYPE_CLIENT_LATENCY,
	MESSAGE_TYPE_SERVER_UPDATES_PER_SEC,
	MESSAGE_TYPE_MESH_BINDING,
	MESSAGE_TYPE_MESH_UNBIND,
	MESSAGE_TYPE_FOG_ENCLOSEMENT,
	MESSAGE_TYPE_PRINT_INVENTORY,
	MESSAGE_TYPE_DRINK_FROM_WATER
};

static const std::string M_PRINT_INVENTORY			= "PI";
static const std::string M_FOG_ENCLOSEMENT			= "FL";
static const std::string M_MESH_UNBIND				= "MUB";
static const std::string M_MESH_BINDING				= "MB";
static const std::string M_UPDATE_ACTOR				= "UA";
static const std::string M_REMOVE_ACTOR				= "RA";
static const std::string M_DEAD_ACTOR				= "DA";
static const std::string M_ACTOR_TAKE_DAMAGE		= "ATD";
/*	
	NONE = 0
	HUMAN = 1
	GHOST = 2
	ANIMAL = 3
*/
static const std::string M_ACTOR_TYPE				= "AT";
static const std::string M_ACTOR_KILL				= "AK";
static const std::string M_RESTART_GAME_REQUEST		= "RGR";
static const std::string M_DRINK_WATER				= "DW";
/*
	DEER = 0
	WOLF = 1
	BEAR = 2
*/
static const std::string M_PLAY_AS_ANIMAL			= "PAD";
static const std::string M_ATTEMPT_POSSESS_ANIMAL	= "PAA";
static const std::string M_LEAVE_ANIMAL				= "LA";
static const std::string M_MAKE_NOISE				= "MSN";
static const std::string M_DEER_EAT_OBJECT			= "AE";
static const std::string M_SUN_DIRECTION			= "SD";
//Data Messages
static const std::string M_LOAD_MAP					= "LM";
static const std::string M_CONNECTED				= "CON";
static const std::string M_POSITION					= "P";
static const std::string M_ROTATION					= "R";
static const std::string M_DIRECTION				= "D";
static const std::string M_UP						= "U";
static const std::string M_MESH_MODEL				= "M";
static const std::string M_USER_NAME				= "UN";
static const std::string M_KICKED					= "K";
static const std::string M_SCALE					= "SC";
static const std::string M_HUNGER					= "HU";
static const std::string M_HEALTH					= "HE";
static const std::string M_HYDRATION				= "HY";
static const std::string M_FRAME_TIME				= "FT";
static const std::string M_STAMINA					= "STAM";
static const std::string M_STATE					= "STAT";
static const std::string M_ITEM_TYPE				= "IT";
static const std::string M_ITEM_SUB_TYPE			= "IST";
static const std::string M_OBJECT_ID				= "OID";
static const std::string M_ITEM_NAME				= "IN";
static const std::string M_ITEM_DESCRIPTION			= "IDE";
static const std::string M_ITEM_ICON_PATH			= "IIP";
static const std::string M_ITEM_WEIGHT				= "IW";
static const std::string M_ITEM_USE					= "IU";
static const std::string M_ITEM_CRAFT				= "IC";
static const std::string M_ITEM_FILL				= "IF";
static const std::string M_ITEM_STACK_SIZE			= "ISTSI";
static const std::string M_ITEM_SLOT_SIZE			= "ISLSI";
static const std::string M_WEAPON_DAMAGE			= "WD";
static const std::string M_WEAPON_RANGE				= "WR";
static const std::string M_PROJECTILE_DAMAGE		= "PD";
static const std::string M_PROJECTILE_VELOCITY		= "PV";
static const std::string M_WEAPON_USE				= "WU";
static const std::string M_CONTAINER_CURRENT		= "COCU";
static const std::string M_CONTAINER_MAX			= "COMA";
//Item Messages
static const std::string M_EQUIPMENT_SLOT			= "ES";
static const std::string M_REMOVE_EQUIPMENT			= "RE";
static const std::string M_EQUIP_ITEM				= "EI";
static const std::string M_UNEQUIP_ITEM				= "UEI";
static const std::string M_PICKUP_ITEM				= "PI";
static const std::string M_LOOT_ITEM				= "LI";
static const std::string M_DROP_ITEM				= "DI";
static const std::string M_ADD_INVENTORY_ITEM		= "AII";
static const std::string M_REMOVE_INVENTORY_ITEM	= "RII";
static const std::string M_LOOT_OBJECT				= "LO";
static const std::string M_LOOT_OBJECT_RESPONSE		= "LOR";
static const std::string M_ITEM_FINISHED			= "IF";
static const std::string M_WEAPON_EQUIPMENT_SWAP	= "WEQS";

//Animal Messages
static const std::string M_ANIMAL_ATTACK			= "AA";
//Key Messages
static const std::string M_KEY_DOWN					= "KD";
static const std::string M_KEY_UP					= "KU";
//Initial Data Message
static const std::string M_USER_DATA				= "UD";
//Restart Messages
static const std::string M_SERVER_RESTART			= "SR";
//Shutdown Messages
static const std::string M_CONNECTION_CLOSED		= "CC";
static const std::string M_SERVER_FULL				= "SF";
static const std::string M_SERVER_SHUTDOWN			= "SS";
//Error Message
static const std::string M_ERROR_MESSAGE			= "EM";
static const std::string M_SERVER_ANNOUNCEMENT		= "SA";
//Important Message Tag
static const std::string M_IMPORTANT_MESSAGE		= "IM";
static const std::string M_ACKNOWLEDGE_MESSAGE		= "ACK";
//Player Messages
static const std::string M_CLIENT_DATA				= "CD";
static const std::string M_NEW_ACTOR				= "NA";
static const std::string M_READY_PLAYER				= "PRM";
//Confirmation Message
static const std::string M_SELF_ID					= "SELF";
//Ping Message
static const std::string M_PING						= "PING";
static const std::string M_CLIENT_LATENCY			= "CL";
static const std::string M_SERVER_UPDATES_PER_SEC	= "SUPC";
static const std::string M_START_GAME				= "SG";
static const std::string M_PLAY_SOUND				= "PS";
static const std::string M_PLAY_ANIMATION			= "PLAN";

/*
	This class creates string messages used for communication
	between sockets.
	Returns an empty string if wrong function is used.
*/
class NetworkMessageConverter
{
public:
	NetworkMessageConverter();
	virtual ~NetworkMessageConverter();

	/*! Converts parameters to correctly formated strings sent between client and server.*/
	std::string Convert(const unsigned int ID);
	std::string Convert(const unsigned int ID, const float fp);
	std::string Convert(const unsigned int ID, const std::string& word);
	std::string Convert(const unsigned int ID, const Vector3& vec);
	std::string Convert(const unsigned int ID, const Vector4& vec);
	
	std::string CombineMessage(const std::vector<std::string>& msgArray);
	std::vector<std::string> SplitMessage(const std::string& msg);
	Vector3 ConvertStringToVector(const std::string& type, const std::string& msg);
	Vector4 ConvertStringToQuaternion(const std::string& type, const std::string& msg);
	int ConvertStringToInt(const std::string& type, const std::string& msg);
	float ConvertStringToFloat(const std::string& type, const std::string& msg);
	std::string ConvertStringToSubstring(const std::string& type, const std::string& msg, bool bReplaceUnderscore = false);

private:

};
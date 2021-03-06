#include "Item.h"

static unsigned int NextItemID = 1;

Item::Item() : 
	zID(0)
{
	this->zStacks = 1;
	this->zWeight = 0;
	this->zItemType = 0;
	this->zItemName = "none";
	this->zIconPath = "none";
	this->zMeshModel = "none";
	this->zItemDescription = "none";
}

Item::Item(const unsigned int itemType, const unsigned int itemSubType) : 
	zID(NextItemID++)
{
	this->zStacks = 1;
	this->zWeight = 0;
	this->zItemName = "none";
	this->zIconPath = "none";
	this->zMeshModel = "none";
	this->zItemType = itemType;
	this->zItemDescription = "none";
	this->zItemSubType = itemSubType;
}

Item::Item(const unsigned int ID, const unsigned int itemType, const unsigned int itemSubType)
{
	this->zID = ID;
	this->zStacks = 1;
	this->zWeight = 0;
	this->zItemName = "none";
	this->zIconPath = "none";
	this->zMeshModel = "none";
	this->zItemType = itemType;
	this->zItemDescription = "none";
	this->zItemSubType = itemSubType;
}

std::string Item::ToMessageString(NetworkMessageConverter* NMC)
{
	std::string msg;

	msg = NMC->Convert(MESSAGE_TYPE_OBJECT_ID, (float)this->zID);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_TYPE, (float)this->zItemType);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_SUB_TYPE, (float)this->zItemSubType);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_DESCRIPTION, this->zItemDescription);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_NAME, this->zItemName);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_WEIGHT, (float) this->zWeight);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_STACK_SIZE, (float) this->zStacks);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_ICON_PATH, this->zIconPath);
	msg += NMC->Convert(MESSAGE_TYPE_ITEM_SLOT_SIZE, (float)this->zSlotSize);
	return msg;
}

void Item::GenerateID()
{
	this->zID = NextItemID++;
}

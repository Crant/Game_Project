/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "RangedWeapon.h"
#include "MeleeWeapon.h"
#include "Projectile.h"
#include "Gear.h"
#include "Food.h"
#include "Container.h"
#include "Material.h"
#include "Bandage.h"
#include "Misc.h"
#include <vector>
#include <World/Observer.h>
#include "InventoryEvents.h"

enum EQUIPMENT_SLOT
{
	EQUIPMENT_SLOT_MELEE_WEAPON,
	EQUIPMENT_SLOT_RANGED_WEAPON,
	EQUIPMENT_SLOT_PROJECTILE,
	EQUIPMENT_SLOT_HEAD,
	EQUIPMENT_SLOT_CHEST,
	EQUIPMENT_SLOT_LEGS,
	EQUIPMENT_SLOT_BOOTS
};

/*! Class That keeps track of the inventory data*/
class Inventory : public Observed
{
public:
	Inventory(const unsigned int ownerID = 0);
	Inventory(const float inventorySize, const unsigned int ownerID = 0);
	virtual ~Inventory();
	/*! Search for the Item in and returns its position*/
	int Search(const unsigned int ID) const;
	int Search(Item* item) const;

	// Adds an item to the array
	// if succesfull will return true
	// if successfull outstacked will return if the item has been stacked
	bool AddItem(Item* item, bool* outStacked = 0);

	/*! Calculate how many slots this item can use.*/
	int CalcMaxAvailableSlots(Item* item);
	/*! Check if the item will stack if added.*/
	bool IsStacking(Item* item);
	/*! Returns the item at the position */
	Item* GetItem(const unsigned int index) const;
	/*! Returns the Items*/
	std::vector<Item*> GetItems() const;
	/*! Search for the item and returns it if found or NULL if not found*/
	Item* SearchAndGetItem(const unsigned int ID) const;
	/*! Removes the item from the list. Returns the removed item.*/
	Item* RemoveItem(const unsigned int ID);
	Item* RemoveItem(Item* item);

	unsigned int GetSlotsAvailable() {return this->zSlotsAvailable;}
	/*! Returns the Inventory Capacity == Max Weight allowed*/
	float GetInventoryCapacity() const;
	/*! Returns the total Weight.*/
	inline float GetTotalWeight() const {return this->zWeightTotal;}
	/*! Returns amount of Items the player has in the inventory with itemType = TYPE*/
	int SearchForItemType(const int Type, const int SubType);
	/*! Removes weight from Inventory depending on item stacks and Weight.*/
	bool RemoveItemStack(const unsigned int ID, const unsigned int numberOfStacks = 1);
	/*! Search after the item and returns it if found else return NULL.*/
	Item* SearchAndGetItemFromType(const int Type, const int SubType);

	//Equipment
	/*! Equips Weapon, returns previous equipped item.*/
	Item* EquipRangedWeapon(RangedWeapon* weapon, bool& success);
	Item* EquipMeleeWeapon(MeleeWeapon* weapon, bool& success);

	Item* EquipProjectile(Projectile* projectile);
	void UnEquipProjectile();
	Projectile* GetProjectile();

	RangedWeapon* GetRangedWeapon();
	MeleeWeapon* GetMeleeWeapon();
	Item* GetLastUnequipped() {return this->zLastUnequipped;}
	void UnEquipRangedWeapon();
	void UnEquipMeleeWeapon();
	void UnEquipAll();

	/*! Returns the primary equipped item.*/
	Item* GetPrimaryEquip() const {return this->zPrimaryEquip;}
	Item* GetSecondaryEquip() const {return this->zSecondaryEquip;}
	/*! */
	void SetPrimaryEquip(unsigned int ID); 
	void SetSecondaryEquip(unsigned int ID);

	bool EquipGear(const unsigned int type, Gear* item);
	Gear* GetGear(const unsigned int type);
	void UnEquipGear(const unsigned int type);

	/*! Swaps between the Primary and secondary Weapons.*/
	bool SwapWeapon();

	void ClearAll();
private:
	/*Removes the item from the list.*/
	Item* Erase(const unsigned int Index);
	/*! Array of items*/
	std::vector<Item*> zItems;
	/*! Nr of slots available in inventory.*/
	unsigned int zSlotsAvailable;
	/*! Max nr of slots in The Inventory array.*/
	float zInventoryCap;
	/*! Nr of slots Occupied == Current Weight.*/
	float zWeightTotal;

	//Equipment Variables
	Item* zPrimaryEquip;
	Item* zSecondaryEquip;
	Item* zLastUnequipped;
	Projectile* zProjectile;
	RangedWeapon* zRangedWeapon;
	MeleeWeapon* zMeleeWeapon;
	std::vector<Gear*> zGear;

	unsigned int zOwnerID;
};

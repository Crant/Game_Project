/*
	Made by Jensen Christopher Datum(11/12/12 created) 
	for project Not Dead Yet at Blekinge tekniska h�gskola
*/

#pragma once

#include "Weapon.h"

class RangedWeapon : public Weapon
{
public:
	RangedWeapon();
	RangedWeapon(const unsigned int ID, const unsigned int itemType, const int itemSubType, const float damage, const float range);	
	RangedWeapon(const unsigned int itemType, const int itemSubType, const float damage, const float range);	
	RangedWeapon(const RangedWeapon& other);
	virtual ~RangedWeapon();

	virtual bool Use();
	void UseWeapon(float& range, float& damage);

private:

};
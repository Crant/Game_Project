#pragma once

#include "Keybinds.h"
#include "KeyStates.h"

class KeyHandler
{
private:
	Keybinds zBinds;
	KeyStates zStates;
public:
	KeyHandler();
	virtual ~KeyHandler();
	/*! Reads the bindings from the .INI file.
	    Called before use.
	*/
	void InitKeyBinds();
	/*! Returns the binding for the Key eg. 'w' or 's'*/
	char GetKey(const unsigned int key) const;
	/*! Returns true if the key is pressed*/
	bool GetKeyState(const unsigned int key) const;
	/*! Change the value for the Key State*/
	void SetKeyState(const unsigned int key, const bool value);

};
#ifndef BUDDY_HPP
#define BUDDY_HPP

#pragma once

#include "../main.hpp"

class cBuddy {
	int Buddies, iSlot;
	const char *name;
	char *UniqueID;
	bool bIsConnected, bIsAlive, bSlotUsed;

public:
	virtual ~cBuddy();
	static cBuddy*	Get();
	cBuddy();

	void AddBuddy();
	void SearchBuddy();
	void RegisterCmds();

	char *GetUniqueID() {
		return UniqueID;
	}

	const char *GetName() {
		return name;
	}

	bool GetIsConnected() {
		return bIsConnected;
	}

	int GetNBuddies() {
		return Buddies;
	}
};

extern cBuddy g_Buddy[ 256 ];

#endif // BUDDY_HPP
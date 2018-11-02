#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "main.hpp"

class CLocalPlayer {
public:
	Vector m_vecForward, m_vecRight, m_vecEyePosition, m_vecViewHeight, m_vecOrigin, m_vecVelocity, m_vecViewAngles, m_vecMins[ 4 ], m_vecBaseVelocity, m_vecViewOrg;

	float m_flFrametime, m_flFallSpeed, m_flVelocity, m_vecPunchAngle[ 3 ], m_flClientMaxSpeed, m_flMaxSpeed;

	bool m_bInDuckCmd, m_bInDuckppmove, m_bIsAlive, m_bIML, m_bIMR, m_bIF, m_bIB, m_bIA, m_bOnSteam, m_bIsSteam, bDBhop, m_bIsConnected;

	// m_iUseHull - 0 = regular player hull, 1 = ducked player hull, 2 = point hull
	// m_iWaterLevel - 0 = not touching water, 1 = water at feet, 2 = covered in water to chest can swim if ducking, 3 = covered in water/swimming
	// m_iGravity - 0 = in ladder or similar, 1 = regular standing if gravity is applied or smth

	int m_iGravity, m_iHealth, m_iOldButtons, m_iFPS, m_iClip, m_iUseHull, m_iWeaponID, m_iFlags, m_iMoveType, m_iWaterLevel, m_iTeam, m_iIndex, m_iFrameN, m_iAH, m_iMX, m_iOldDir,
		m_iArmor, m_iShootTimer;

	char m_szBaseDir[ 256 ];
};

struct SOtherPlayers {
public:
	Vector m_vecOrigin, m_vecVelocity, m_vec;

	float m_flDistance, m_flFrametime;

	int m_iTeam, m_iHealth;

	bool m_bUpdated, m_bVisible;

	hud_player_info_t m_Info;
};

inline CLocalPlayer* g_pLocalPlayer() {
	static CLocalPlayer local;
	return &local;
}

extern SOtherPlayers g_OtherPlayers[ 33 ];

#endif // ENTITY_HPP
#ifndef INSTRUMENTS_HPP
#define INSTRUMENTS_HPP

#pragma once

#include "../main.hpp"

class Instruments {
	Instruments();

public:
	virtual ~Instruments();
	static Instruments* Get();

	std::vector<scrollpattern> getPatterns();

	float			flGroundHeight();
	float			flEdgeDist();
	float			flAngleAtGround();
	float			get_edge_inair();
	float			PlayerHeight( int usehull );
	float			flGroundDistMeasuredInFrames();

	bool			is_above_facing_wall();
	bool			bIsValidEnt( int idx );
	bool			HookCvarVariable( char *szNewCvarName, char *szOrigCvarName, float **ppCvarValue, bool createfcvar );
	bool			WorldToScreen( float *pflOrigin, float *pflVecScreen );
	bool			bIsVisible( float *pflFrom, float *pflTo );
	bool			bSurfStrafeHelper();
	bool			bSurf();
	bool			HoldingPistol();

	int				generaterandomintzeroandone( int Reason );
	int				ByteControl( float in );
	int				autodirwithvelocity();

	char			*PrefHack( char *IfCmd, char *Name );

	std::string		szDirFile( char* pszName );
	std::string		Prefix_ini();
};

#endif // !INSTRUMENTS_HPP
#ifndef INSTRUMENTS_HPP
#define INSTRUMENTS_HPP

#pragma once

#include "../main.hpp"

class Instruments {
	Instruments();

	float GetTraceEndPosN( Vector start, Vector end, int N );

public:
	virtual ~Instruments();
	static Instruments* Get();

	std::vector<scrollpattern> getPatterns();

	float			flGroundHeight();
	float			flEdgeDist();
	bool			bIsValidEnt( int idx );
	int				generaterandomintzeroandone( int Reason );
	bool			HookCvarVariable( char *szNewCvarName, char *szOrigCvarName, float **ppCvarValue, bool createfcvar );
	bool			WorldToScreen( float *pflOrigin, float *pflVecScreen );
	bool			bIsVisible( float *pflFrom, float *pflTo );
	float			PlayerHeight( int usehull );
	int				autodir();
	bool			bSurfStrafeHelper();
	bool			bSurf();
	bool			HoldingPistol();
	int				ByteControl( float in );
	float			flAngleAtGround();
	float			get_edge_inair();
	bool			is_above_facing_wall();
	void __fastcall	VectorAngles( const float *forward, float *angles );
	char			*PrefHack( char *IfCmd, char *Name );
	std::string		szDirFile( char* pszName );
	std::string		Prefix_ini();
	int				autodirwithvelocity();

	float	flGroundDistMeasuredInFrames();
};

#endif // !INSTRUMENTS_HPP
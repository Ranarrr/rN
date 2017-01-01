#ifndef _INSTRUMENTS_H_
#define _INSTRUMENTS_H_

#include "main.hpp"

class Instruments {
private:
	Instruments();

	float GetTraceEndPosN( float *start, float *end, int N );
public:
	virtual ~Instruments();
	static Instruments* Get();

	float flGroundHeight();
	float flEdgeDist();
	bool bIsValidEnt( int idx );
	int generaterandomintzeroandone( int Reason );
	bool HookCvarVariable( char *szNewCvarName, char *szOrigCvarName, float **ppCvarValue, bool createfcvar );
	bool WorldToScreen( float *pflOrigin, float *pflVecScreen );
	bool bIsVisible( float *pflFrom, float *pflTo );
	float PlayerHeight( int usehull );
	int autodir();
	bool bSurfStrafeHelper();
	bool bSurf();
	bool HoldingPistol();
	int ByteControl( float in );
	float flAngleAtGround();

	float flGroundDistMeasuredInFrames();
};

#endif // INSTRUMENTS_H
#ifndef JUMPSTATS_HPP
#define JUMPSTATS_HPP

#include "main.hpp"

class CJumpStats {
private:
	CJumpStats();

public:
	virtual ~CJumpStats();
	static CJumpStats*	Get();

	void CL_CreateMove( struct usercmd_s *pCmd );
	void HUD_Redraw();
private:
	Vector vecStart;
	Vector vecEnd;
	float flMaxspeed;
	float flPrestrafe;
	float flDistance;
	bool bInJump;
	bool bEdgeJumpoff;
	int iTimer;
	int iFPS;
	int iStrafes;
	//jumpoff
	bool bJumpoffResetJump;
	static int iJumpoffBlock;
	static float flJumpoffEdge, flJumpoffLandingEdge;
	int iTimer1, iTimer2, iTimer3;
	int CalculateDistances( Vector vStart, Vector vStop, float flJumpoffFootHeight, float flDistance );
};

#endif // JUMPSTATS_HPP
#include "jumpstats.hpp"

int CJumpStats::iJumpoffBlock;
float CJumpStats::flJumpoffEdge, CJumpStats::flJumpoffLandingEdge;

CJumpStats::CJumpStats() {}

CJumpStats::~CJumpStats() {}

CJumpStats* CJumpStats::Get() {
	static CJumpStats sJumpStats;
	return &sJumpStats;
}

int CJumpStats::CalculateDistances( Vector vStart, Vector vStop, float flJumpoffFootHeight, float flDistance ) {
	Vector vVertexOrigin, vJumpoffEdgeOrigin, vLandingEdgeOrigin;

	vVertexOrigin[ 0 ] = ( vStart[ 0 ] + vStop[ 0 ] ) * 0.5;
	vVertexOrigin[ 1 ] = ( vStart[ 1 ] + vStop[ 1 ] ) * 0.5;
	vVertexOrigin[ 2 ] = vStop[ 2 ] - 1.0;

	vStart[ 2 ] = vVertexOrigin[ 2 ];
	vStop[ 2 ] = vVertexOrigin[ 2 ];

	pmplane_t       vecPlaneNormal;
	pmtrace_t *trace = g_pEngine->PM_TraceLine( vVertexOrigin, vStart, 1, g_pLocalPlayer()->m_iUseHull, -1 );
	vecPlaneNormal = trace->plane;

	if( vecPlaneNormal.normal.z != 0.0 )
		return 0;

	VectorCopy( trace->endpos, vJumpoffEdgeOrigin );

	if( abs( vecPlaneNormal.normal[ 0 ] ) == 1.0 && !vecPlaneNormal.normal[ 1 ] )	flJumpoffEdge = abs( vJumpoffEdgeOrigin[ 0 ] - vStart[ 0 ] ) - 0.03125;
	else if( !vecPlaneNormal.normal[ 0 ] && abs( vecPlaneNormal.normal[ 1 ] ) == 1.0 )	flJumpoffEdge = abs( vJumpoffEdgeOrigin[ 1 ] - vStart[ 1 ] ) - 0.03125;
	else return 0;

	iTimer3 = 300;

	if( flDistance < 0 ) {
		Vector vEnd;

		if( abs( vecPlaneNormal.normal[ 0 ] ) == 1.0 && !vecPlaneNormal.normal[ 1 ] ) {
			vEnd[ 0 ] = vStop[ 0 ] + vecPlaneNormal.normal[ 0 ] * 300;
			vEnd[ 1 ] = vStop[ 1 ];
			vEnd[ 2 ] = vStop[ 2 ];
		} else if( !vecPlaneNormal.normal[ 0 ] && abs( vecPlaneNormal.normal[ 1 ] ) == 1.0 ) {
			vEnd[ 0 ] = vStop[ 0 ];
			vEnd[ 1 ] = vStop[ 1 ] + vecPlaneNormal.normal[ 1 ] * 300;
			vEnd[ 2 ] = vStop[ 2 ];
		} else return 0;

		pmtrace_t *trace2 = g_pEngine->PM_TraceLine( vVertexOrigin, vEnd, 1, g_pLocalPlayer()->m_iUseHull, -1 );
		vecPlaneNormal = trace2->plane;

		if( vecPlaneNormal.normal[ 2 ] != 0.0 ) return 0;

		VectorCopy( trace2->endpos, vLandingEdgeOrigin );

		if( abs( vecPlaneNormal.normal[ 0 ] ) == 1.0 && !vecPlaneNormal.normal[ 1 ] ) {
			vEnd[ 0 ] = vLandingEdgeOrigin[ 0 ] - vecPlaneNormal.normal[ 0 ] * 16.1;
			vEnd[ 1 ] = vLandingEdgeOrigin[ 1 ];
			vEnd[ 2 ] = vLandingEdgeOrigin[ 2 ] + 37.0;
		} else if( !vecPlaneNormal.normal[ 0 ] && abs( vecPlaneNormal.normal[ 1 ] ) == 1.0 ) {
			vEnd[ 0 ] = vLandingEdgeOrigin[ 0 ];
			vEnd[ 1 ] = vLandingEdgeOrigin[ 1 ] - vecPlaneNormal.normal[ 1 ] * 16.1;
			vEnd[ 2 ] = vLandingEdgeOrigin[ 2 ] + 37.0;
		} else return 0;

		vVertexOrigin[ 2 ] += 37.0;

		pmtrace_t *trace3 = g_pEngine->PM_TraceLine( vVertexOrigin, vEnd, 1, g_pLocalPlayer()->m_iUseHull, -1 );

		static float fraction;
		fraction = trace3->fraction;

		if( fraction < 1.0 ) return 0;
	} else {
		pmtrace_t *trace4 = g_pEngine->PM_TraceLine( vVertexOrigin, vStop, 1, g_pLocalPlayer()->m_iUseHull, -1 );
		vecPlaneNormal = trace4->plane;

		if( vecPlaneNormal.normal[ 2 ] != 0.0 ) return 0;

		VectorCopy( trace4->endpos, vLandingEdgeOrigin );
	}

	if( abs( vecPlaneNormal.normal[ 0 ] ) == 1.0 && !vecPlaneNormal.normal[ 1 ] ) {
		flJumpoffLandingEdge = abs( vLandingEdgeOrigin[ 0 ] - vStop[ 0 ] ) - 0.03125;
		iJumpoffBlock = ceil( abs( vLandingEdgeOrigin[ 0 ] - vJumpoffEdgeOrigin[ 0 ] ) ) + 32;
	} else if( !vecPlaneNormal.normal[ 0 ] && abs( vecPlaneNormal.normal[ 1 ] ) == 1.0 ) {
		flJumpoffLandingEdge = abs( vLandingEdgeOrigin[ 1 ] - vStop[ 1 ] ) - 0.03125;
		iJumpoffBlock = ceil( abs( vLandingEdgeOrigin[ 1 ] - vJumpoffEdgeOrigin[ 1 ] ) ) + 32;
	} else return 0;

	return 0;
}

void CJumpStats::HUD_Redraw() {
	if( this->flDistance > CCVars::Get()->js_min->value ) {
		if( this->iTimer > 0 ) {
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.25 + 15, Color::White(), XString( /*Distance: %.0f*/ 0x04, 0x0E, 0x1A, 0x5E726F69, 0x7F714344, 0x1803010B, 0x16410000 ).c(), this->flDistance );
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.25 + 30, Color::White(), XString( /*Maxspeed: %.0f*/ 0x04, 0x0E, 0x38, 0x75584248, 0x4C585B5B, 0x7A61676D, 0x74230000 ).c(), this->flMaxspeed );
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.25 + 45, Color::White(), XString( /*Prestrafe: %.0f*/ 0x04, 0x0F, 0xFF, 0xAF726471, 0x77766460, 0x6232292F, 0x253C6B00 ).c(), this->flPrestrafe );
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.25 + 60, Color::White(), XString( /*Strafes: %i*/ 0x03, 0x0B, 0xBC, 0xEFC9CCDE, 0xA6A4B1F9, 0xE4E0AF00 ).c(), this->iStrafes );
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.25 + 75, Color::White(), XString( /*FPS: %i*/ 0x02, 0x07, 0xF2, 0xB4A3A7CF, 0xD6D29100 ).c(), this->iFPS );
			this->iTimer--;
		}
	}
	if( iTimer1 ) {
		CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.65 + CDrawing::Get()->GetStringHeight() * 7, Color::Green(), XString( /*Block: %i Jumpoff: %f Landing: %f*/ 0x09, 0x21, 0x62, 0x200F0B06, 0x0D5D484C, 0x034B2618, 0x031F1F17, 0x14495450, 0x10573418, 0x141F1513, 0x1945A0A4, 0xE4000000 ).c(), iJumpoffBlock, flJumpoffEdge, flJumpoffLandingEdge );
		iTimer1--; iTimer2 = 0; iTimer3 = 0;
	}
	if( iTimer2 ) {
		CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.65 + CDrawing::Get()->GetStringHeight() * 7, Color::Red(), XString( /*Block: %i Jumpoff: %f*/ 0x06, 0x15, 0x59, 0x1B36343F, 0x36647F45, 0x08422911, 0x0816080E, 0x0F504B49, 0x0B000000 ).c(), iJumpoffBlock, flJumpoffEdge );
		iTimer2--; iTimer1 = 0; iTimer3 = 0;
	}
	if( iTimer3 ) {
		CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.65 + CDrawing::Get()->GetStringHeight() * 7, Color::Green(), XString( /*Jumpoff: %f*/ 0x03, 0x0B, 0xF9, 0xB38F968C, 0x9298993A, 0x21276500 ).c(), flJumpoffEdge );
		iTimer3--; iTimer1 = 0; iTimer2 = 0;
	}
}

void CJumpStats::CL_CreateMove( usercmd_s *pCmd ) {
	if( Instruments::Get()->flEdgeDist() < CCVars::Get()->edge_jumpoff_dist->value && g_pLocalPlayer()->m_flFallSpeed <= 0 && !( g_pLocalPlayer()->m_bInDuckCmd ) && bEdgeJumpoff )
		pCmd->buttons |= IN_JUMP;

	if( CCVars::Get()->js->value ) {
		static int strafes;
		static float prestrafe;
		if( !( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) ) {
			static bool bhalfstr;
			float viewangles[ 3 ], check;
			static float oldviewangles;
			g_Engine.GetViewAngles( viewangles );
			check = viewangles[ 1 ] - oldviewangles;
			if( check > 0 && check != 0 && pCmd->buttons&IN_MOVELEFT && bhalfstr ) {
				strafes++;
				bhalfstr = false;
			} else if( check < 0 && check != 0 && pCmd->buttons&IN_MOVERIGHT && !bhalfstr ) {
				strafes++;
				bhalfstr = true;
			}
			oldviewangles = viewangles[ 1 ];

			if( !this->bInJump && GetAsyncKeyState( 32 ) ) {
				strafes = 1;
				this->vecStart = g_pLocalPlayer()->m_vecOrigin;
				prestrafe = g_pLocalPlayer()->m_flVelocity;
				this->bInJump = true;
			}
		} else {
			if( this->bInJump ) {
				this->vecEnd = g_pLocalPlayer()->m_vecOrigin;
				this->flDistance = sqrt( POW( this->vecStart[ 0 ] - this->vecEnd[ 0 ] ) + POW( this->vecStart[ 1 ] - this->vecEnd[ 1 ] ) + POW( this->vecStart[ 2 ] - this->vecEnd[ 2 ] ) ) + 33;
				this->flMaxspeed = g_pLocalPlayer()->m_flVelocity;
				this->iTimer = CCVars::Get()->js_time->value;
				this->iStrafes = strafes;
				this->flPrestrafe = prestrafe;
				this->iFPS = g_pLocalPlayer()->m_iFPS;
				if( this->flDistance > CCVars::Get()->js_min->value ) {
					if( CCVars::Get()->js_in_console->value ) {
						g_Engine.Con_Printf( XString( /*[Jumpstats]*/ 0x03, 0x0B, 0x4D, 0x16043A3D, 0x21212735, 0x21250A00 ).c() );
						g_Engine.Con_Printf( "\n" );
						g_Engine.Con_Printf( XString( /*Distance: %f Maxspeed: %f Prestrafe: %f Strafes: %i*/ 0x0D, 0x33, 0xE8, 0xAC80999F, 0x8D838D8A, 0xCAD1D795, 0xD4B8978F, 0x8B899F9E, 0x98C7DEDA, 0x66215271, 0x61767275, 0x696F6F31, 0x2C28682F, 0x43656072, 0x7270652D, 0x383C7300 ).c(), this->flDistance, this->flMaxspeed, this->flPrestrafe, this->iStrafes );
						g_Engine.Con_Printf( "\n" );
						g_Engine.Con_Printf( XString( /*rN v0.17 by Ranarrr*/ 0x05, 0x13, 0xB7, 0xC5F699CC, 0x8B928C89, 0x9FA2B8E2, 0x91A5ABA7, 0xB5BABB00 ).c() );
						g_Engine.Con_Printf( "\n" );
					}
				}
			}
			this->bInJump = false;
		}
	}

	static bool bInAir, bFirstFrame, bFailStats;
	static float flJumpoffFootHeight;
	static Vector vJumpoffOrigin, vFailOrigin, vFrameOrigin[ 2 ], vFrameVelocity[ 2 ], vOrigin;

	if( bJumpoffResetJump || g_pLocalPlayer()->m_iMoveType == MOVETYPE_FLY ) {
		bEdgeJumpoff = true;
		bJumpoffResetJump = false;
		bInAir = false;
	}

	if( bInAir ) {
		if( g_pLocalPlayer()->m_iFlags & FL_ONGROUND && !bFailStats ) {
			vOrigin = g_pLocalPlayer()->m_vecOrigin;

			if( flJumpoffFootHeight == vOrigin[ 2 ] + g_pLocalPlayer()->m_vecMins[ g_pLocalPlayer()->m_iUseHull ][ 2 ] ) {
				static float flGravity, flTemp, flDistance1, flDistance2;
				Vector vLandingOrigin;

				flGravity = 800 * g_pLocalPlayer()->m_iGravity;
				flTemp = ( -sqrt( POW( vFrameVelocity[ 0 ][ 2 ] ) + ( 2 * flGravity * ( vFrameOrigin[ 0 ][ 2 ] - vOrigin[ 2 ] + 0.1 ) ) ) - vFrameVelocity[ 1 ][ 2 ] ) / -flGravity;

				if( vFrameOrigin[ 1 ][ 0 ] < vOrigin[ 0 ] ) vLandingOrigin[ 0 ] = vFrameOrigin[ 1 ][ 0 ] + flTemp * abs( vFrameVelocity[ 1 ][ 0 ] );
				else vLandingOrigin[ 0 ] = vFrameOrigin[ 1 ][ 0 ] - flTemp * abs( vFrameVelocity[ 1 ][ 0 ] );

				if( vFrameOrigin[ 1 ][ 1 ] < vOrigin[ 1 ] )   vLandingOrigin[ 1 ] = vFrameOrigin[ 1 ][ 1 ] + flTemp * abs( vFrameVelocity[ 1 ][ 1 ] );
				else vLandingOrigin[ 1 ] = vFrameOrigin[ 1 ][ 1 ] - flTemp * abs( vFrameVelocity[ 1 ][ 1 ] );

				flDistance1 = sqrt( pow( vOrigin[ 0 ] - vJumpoffOrigin[ 0 ], 2 ) + pow( vOrigin[ 1 ] - vJumpoffOrigin[ 1 ], 2 ) ) + 32.0;
				flDistance2 = sqrt( pow( vLandingOrigin[ 0 ] - vJumpoffOrigin[ 0 ], 2 ) + pow( vLandingOrigin[ 1 ] - vJumpoffOrigin[ 1 ], 2 ) ) + 32.0;

				if( flDistance1 > flDistance2 ) CalculateDistances( vJumpoffOrigin, vLandingOrigin, flJumpoffFootHeight, flDistance2 );
				else CalculateDistances( vJumpoffOrigin, vOrigin, flJumpoffFootHeight, flDistance1 );
			}

			bJumpoffResetJump = true;
		} else if( bFailStats ) {
			bJumpoffResetJump = true;
			bFailStats = false;

			CalculateDistances( vJumpoffOrigin, vFailOrigin, flJumpoffFootHeight, -1.0 );
		} else {
			vOrigin = g_pLocalPlayer()->m_vecOrigin;

			bFailStats = ( ( vOrigin[ 2 ] + 18 ) < vJumpoffOrigin[ 2 ] );

			if( ( g_pLocalPlayer()->m_bInDuckppmove ? ( vOrigin[ 2 ] + 18 ) : vOrigin[ 2 ] ) >= vJumpoffOrigin[ 2 ] ) {
				static float flTemp;
				flTemp = ( vJumpoffOrigin[ 2 ] - vFrameOrigin[ 1 ][ 2 ] ) / ( vOrigin[ 2 ] - vFrameOrigin[ 1 ][ 2 ] );

				vFailOrigin[ 0 ] = flTemp * ( vOrigin[ 0 ] - vFrameOrigin[ 1 ][ 0 ] ) + vFrameOrigin[ 1 ][ 0 ];
				vFailOrigin[ 1 ] = flTemp * ( vOrigin[ 1 ] - vFrameOrigin[ 1 ][ 1 ] ) + vFrameOrigin[ 1 ][ 1 ];
				vFailOrigin[ 2 ] = flJumpoffFootHeight;
			}
			if( bFirstFrame ) {
				bFirstFrame = false;

				VectorCopy( vOrigin, vFrameOrigin[ 0 ] );
				vFrameVelocity[ 0 ] = g_pLocalPlayer()->m_vecVelocity;
			} else {
				VectorCopy( vOrigin, vFrameOrigin[ 1 ] );
				vFrameVelocity[ 1 ] = g_pLocalPlayer()->m_vecVelocity;
			}
		}
	}

	if( pCmd->buttons & IN_JUMP && g_pLocalPlayer()->m_iOldButtons != IN_JUMP ) {
		bInAir = true;
		bFirstFrame = true;
		bEdgeJumpoff = false;

		if( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) {
			vJumpoffOrigin = g_pLocalPlayer()->m_vecOrigin;
			flJumpoffFootHeight = vJumpoffOrigin[ 2 ] + g_pLocalPlayer()->m_vecMins[ g_pLocalPlayer()->m_iUseHull ][ 2 ];
		} else {
			Vector vVelocity;
			vVelocity = g_pLocalPlayer()->m_vecVelocity;

			if( !vVelocity[ 2 ] ) {
				vOrigin = g_pLocalPlayer()->m_vecOrigin;

				vJumpoffOrigin[ 0 ] = vOrigin[ 0 ];
				vJumpoffOrigin[ 1 ] = vOrigin[ 1 ];
			}
		}
	}
}
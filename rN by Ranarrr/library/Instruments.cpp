#include "Instruments.hpp"

Instruments::Instruments() {}

Instruments::~Instruments() {}

Instruments* Instruments::Get() {
	static Instruments sInstruments;
	return &sInstruments;
}

float Instruments::flGroundDistMeasuredInFrames() {
	pmtrace_t *tr = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.0f, 0.0f, 8192.0f ), 1, 0, -1 );
	return ( g_pLocalPlayer()->m_vecOrigin[ 2 ] - tr->endpos[ 2 ] ) / ( ( g_pLocalPlayer()->m_flFallSpeed > 0 ? g_pLocalPlayer()->m_flFallSpeed : 1.f ) * g_pLocalPlayer()->m_flFrametime );
}

float Instruments::flGroundHeight() {
	pmtrace_t *trace = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.0f, 0.0f, 8192.0f ), 1, 0, -1 );
	return g_pLocalPlayer()->m_vecOrigin[ 2 ] - trace->endpos[ 2 ];
}

float Instruments::flAngleAtGround() {
	pmtrace_t *trace = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.0f, 0.0f, 8192.0f ), 1, g_pLocalPlayer()->m_iUseHull, -1 );
	return acos( trace->plane.normal[ 2 ] ) / M_PI * 180;
}

bool Instruments::bSurf() // if on surf or not
{
	if( flAngleAtGround() >= 45.000001 && Instruments::Get()->flGroundHeight() < 30.0f )
		return true;
	else
		return false;
}

bool Instruments::bSurfStrafeHelper() {
	if( flAngleAtGround() >= 45.000001 && CCVars::Get()->strafe_control_helper_surffix->value && Instruments::Get()->flGroundHeight() < CCVars::Get()->strafe_control_helper_surffix_height->value )
		return true;
	else
		return false;
}

// N == vector axis of end-position, 0 == x, 1 == y, 2 == z | Credits to RIscRIpt
float Instruments::GetTraceEndPosN( float *start, float *end, int N ) {
	pmtrace_t t;
	if( g_pLocalPlayer()->m_bInDuckCmd || g_pLocalPlayer()->m_iUseHull == 1 )
		g_Engine.pEventAPI->EV_SetTraceHull( 1 );
	else
		g_Engine.pEventAPI->EV_SetTraceHull( 0 );
	g_Engine.pEventAPI->EV_PlayerTrace( start, end, PM_STUDIO_BOX, -1, &t );
	return t.endpos[ N ];
}

float Instruments::flEdgeDist() {
	float result = FLT_MAX;
	Vector vBase;

	vBase = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.0f, 0.0f, 8182.0f ), 1, 0, -1 )->endpos - Vector( 0.0f, 0.0f, 1.0f );

	result = min( result, GetTraceEndPosN( vBase + Vector( 36.0f, 0.0f, 0.0f ), vBase, 0 ) - vBase[ 0 ] );
	result = min( result, vBase[ 0 ] - GetTraceEndPosN( vBase - Vector( 36.0f, 0.0f, 0.0f ), vBase, 0 ) );
	result = min( result, GetTraceEndPosN( vBase + Vector( 0.0f, 36.0f, 0.0f ), vBase, 1 ) - vBase[ 1 ] );
	result = min( result, vBase[ 1 ] - GetTraceEndPosN( vBase - Vector( 0.0f, 36.0f, 0.0f ), vBase, 1 ) );
	return result;
}
//-------------------------

int Instruments::ByteControl( float in ) {
	int out;
	if( in > 255 )
		out = 255;
	else if( in < 0 )
		out = 0;
	else
		out = in;
	return out;
}

// reason == 1 = gs, 2 = sgs, 3 anything
int Instruments::generaterandomintzeroandone( int Reason ) {
	float betweenzeroandone = 0.0f;
	if( Reason == 1 ) {
		if( g_pLocalPlayer()->m_bInDuckCmd = true )
			betweenzeroandone += g_Engine.pfnRandomFloat( 0.1, 0.4 );

		if( CCVars::Get()->gs_max_speed_random->value )
			if( g_pLocalPlayer()->m_flVelocity > g_Engine.pfnRandomFloat( CCVars::Get()->gs_max_speed_random_min->value, CCVars::Get()->gs_max_speed_random_max->value ) )
				betweenzeroandone -= g_Engine.pfnRandomFloat( 0.2, 0.6 );
			else
				if( g_pLocalPlayer()->m_flVelocity > CCVars::Get()->gs_max_speed->value )
					betweenzeroandone -= g_Engine.pfnRandomFloat( 0.2, 0.6 );

		if( POW( g_Engine.pfnRandomFloat( 0.12395787, 0.9315677 ) ) < 1 )
			betweenzeroandone += g_Engine.pfnRandomFloat( 0.1, 0.4 );

		if( betweenzeroandone <= 0.8f && betweenzeroandone >= 0.2f )
			betweenzeroandone = 1;
		else
			betweenzeroandone = 0;
	}
	if( Reason == 2 ) {
		if( g_pLocalPlayer()->m_flFallSpeed > 0.0f )
			betweenzeroandone += g_Engine.pfnRandomFloat( 0.1, 0.4 );

		if( g_pLocalPlayer()->m_bInDuckCmd = true )
			betweenzeroandone += g_Engine.pfnRandomFloat( 0.1, 0.4 );

		if( g_pLocalPlayer()->m_flVelocity > 295.0f )
			betweenzeroandone -= g_Engine.pfnRandomFloat( 0.2, 0.6 );

		if( POW( g_Engine.pfnRandomFloat( 0.12395787, 0.9315677 ) ) < 1 )
			betweenzeroandone += g_Engine.pfnRandomFloat( 0.1, 0.4 );

		if( betweenzeroandone <= 0.8f && betweenzeroandone >= 0.2f )
			betweenzeroandone = 1;
		else
			betweenzeroandone = 0;
	}
	if( Reason == 3 ) {
		float a; float b; float c; float d; float e; float f; float g;
		a = g_Engine.pfnRandomFloat( 0, 9 );
		b = g_Engine.pfnRandomFloat( 0, 9 );
		c = g_Engine.pfnRandomFloat( 0, 9 );
		d = g_Engine.pfnRandomFloat( 0, 9 );
		e = g_Engine.pfnRandomFloat( 0, 9 );
		f = g_Engine.pfnRandomFloat( 0, 9 );
		g = g_Engine.pfnRandomFloat( 0, 9 );
		a += b + c + d + e + f + g;
		a = a / 7;

		if( a <= 5 && a >= 3 )
			betweenzeroandone = 1;
		else
			betweenzeroandone = 0;
	}
	return ( int ) betweenzeroandone;
}

bool Instruments::bIsValidEnt( int idx ) {
	cl_entity_s *ent = g_pEngine->GetEntityByIndex( idx );

	if( ent == nullptr )
		return false;

	SOtherPlayers& COP = g_OtherPlayers[ ent->index ];
	if( ent->player && g_pLocalPlayer()->m_iIndex != ent->index && ent->curstate.movetype != MOVETYPE_NONE && !( ent->curstate.messagenum < g_Engine.GetLocalPlayer()->curstate.messagenum ) &&
		!( g_Engine.GetLocalPlayer()->curstate.iuser1 == 4 && g_Engine.GetLocalPlayer()->curstate.iuser2 == ent->index ) && COP.m_iTeam > 0 && COP.m_iTeam <= 2 )
		return true;
	else
		return false;
}

bool Instruments::HookCvarVariable( char *szNewCvarName, char *szOrigCvarName, float **ppCvarValue, bool createfcvar ) {
	cvar_s *pcvar;
	pcvar = g_Engine.pfnGetCvarPointer( szOrigCvarName );

	if( pcvar == NULL ) {
		return false;
		g_Engine.Con_Printf( "\t\t\t" );
		g_Engine.Con_Printf( XString( /*<rN> Could not find cvar: %s*/ 0x07, 0x1C, 0x39, 0x05487502, 0x1D7D5035, 0x2D26632A, 0x2A32672E, 0x20242F6C, 0x2E382E22, 0x6B727627 ).c(), pcvar->name );
	}
	pcvar->name = szNewCvarName;
	*ppCvarValue = &pcvar->value;
	if( createfcvar ) {
		g_Engine.pfnRegisterVariable( szOrigCvarName, pcvar->string, NULL );
		g_Engine.Con_Printf( "\t\t\t" );
		g_Engine.Con_Printf( XString( /*<rN> Registered fake cvar with name: %s*/ 0x0A, 0x27, 0xBC, 0x80CFF081, 0xE093A7A4, 0xADB6B2A2, 0xBAACAEEB, 0xAAACA5AA, 0xF0B2A4B2, 0xA6F5A1BE, 0xACB1FAB5, 0xBDB0BBE5, 0xC0C49100 ).c(), szOrigCvarName );
		g_Engine.Con_Printf( "\n" );
	}
	return true;
}

bool Instruments::HoldingPistol() {
	if( g_pLocalPlayer()->m_iWeaponID == WEAPONLIST_ELITE || WEAPONLIST_GLOCK18 || WEAPONLIST_USP
		|| WEAPONLIST_P228 || WEAPONLIST_DEAGLE || WEAPONLIST_FIVESEVEN ) {
		return true;
	}

	return false;
}

bool Instruments::bIsVisible( float *pflFrom, float *pflTo ) {
	if( !pflFrom || !pflTo ) { return false; }
	pmtrace_t pTrace;

	g_Engine.pEventAPI->EV_SetTraceHull( 2 );
	g_Engine.pEventAPI->EV_PlayerTrace( pflFrom, pflTo, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_pLocalPlayer()->m_iIndex, &pTrace );
	return ( pTrace.fraction == 1.0f );
}

bool Instruments::WorldToScreen( float *pflOrigin, float *pflVecScreen ) {
	int iResult = g_Engine.pTriAPI->WorldToScreen( pflOrigin, pflVecScreen );
	if( pflVecScreen[ 0 ] < 1 && pflVecScreen[ 1 ] < 1 && pflVecScreen[ 0 ] > -1 && pflVecScreen[ 1 ] > -1 && !iResult ) {
		pflVecScreen[ 0 ] = pflVecScreen[ 0 ] * ( g_Screen.m_iWidth / 2 ) + ( g_Screen.m_iWidth / 2 );
		pflVecScreen[ 1 ] = -pflVecScreen[ 1 ] * ( g_Screen.m_iHeight / 2 ) + ( g_Screen.m_iHeight / 2 );
		return true;
	}

	return false;
}

float Instruments::PlayerHeight( int usehull ) //credits to terazoid i think
{
	Vector vTemp = g_pLocalPlayer()->m_vecOrigin;
	vTemp[ 2 ] = -4096;
	pmtrace_t *trace = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, vTemp, 1, usehull, -1 );
	Vector vDistance = ( vTemp - g_pLocalPlayer()->m_vecOrigin ) * trace->fraction;
	float ret = -vDistance[ 2 ];
	vTemp = trace->endpos;
	pmtrace_t pTrace;
	g_Engine.pEventAPI->EV_SetTraceHull( usehull );
	g_Engine.pEventAPI->EV_PlayerTrace( g_pLocalPlayer()->m_vecOrigin, vTemp, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_pLocalPlayer()->m_iIndex, &pTrace );
	if( pTrace.fraction < 1.0f ) {
		vDistance = ( vTemp - g_pLocalPlayer()->m_vecOrigin ) * pTrace.fraction;
		ret = -vDistance[ 2 ];
		int index = g_Engine.pEventAPI->EV_IndexFromTrace( &pTrace );
		SOtherPlayers& COP = g_OtherPlayers[ index ];
		if( index >= 1 && index <= 32 ) {
			float dst = g_pLocalPlayer()->m_vecOrigin.z - ( g_pLocalPlayer()->m_iUseHull == 0 ? 32 : 18 ) - COP.m_vecOrigin.z - ret;
			if( dst < 30 )
				ret -= 14.0f;
		}
	}
	return ret;
}

int Instruments::autodir()					// if (mx > 0) mouse moves to the right, if (mx < 0) mouse moves to the left
{
	int dir1 = 0;

	if( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) {
		if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIF && g_pLocalPlayer()->m_bIMR ) { dir1 = 1; }
		if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIB && g_pLocalPlayer()->m_bIML ) { dir1 = 2; }
		if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIB && g_pLocalPlayer()->m_bIMR ) { dir1 = 3; }
		if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIF && g_pLocalPlayer()->m_bIML ) { dir1 = 4; }
		if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIF && g_pLocalPlayer()->m_bIML ) { dir1 = 1; }
		if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIB && g_pLocalPlayer()->m_bIMR ) { dir1 = 2; }
		if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIF && g_pLocalPlayer()->m_bIMR ) { dir1 = 3; }
		if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIB && g_pLocalPlayer()->m_bIML ) { dir1 = 4; }
	}

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIML && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 1; } // mouse moves left
	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIMR && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 1; } // mouse moves right

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIMR && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 2; } // mouse moves left
	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIML && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 2; } // mouse moves right

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIF && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 3; } // mouse moves left
	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIB && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 3; } // mouse moves right

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIB && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 4; } // mouse moves left
	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIF && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) { dir1 = 4; } // mouse moves right

	if( g_pLocalPlayer()->m_iMoveType == MOVETYPE_FLY ) {
		dir1 = 1;
		g_pLocalPlayer()->m_iOldDir = dir1;
	}

	if( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) {
		g_pLocalPlayer()->m_iOldDir = dir1;
	} else {
		dir1 = g_pLocalPlayer()->m_iOldDir;
	}
	return dir1;
}
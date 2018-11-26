#include "Instruments.hpp"

Instruments::Instruments() {}

Instruments::~Instruments() {}

Instruments* Instruments::Get() {
	static Instruments sInstruments;
	return &sInstruments;
}
																			
float Instruments::flGroundDistMeasuredInFrames() {
	pmtrace_s *tr = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.f, 0.f, 8192.0f ), 1, PM_NORMAL, -1 );
	return ( g_pLocalPlayer()->m_vecOrigin[ 2 ] - tr->endpos[ 2 ] ) / ( ( g_pLocalPlayer()->m_flFallSpeed > 0 ? g_pLocalPlayer()->m_flFallSpeed : 1.f ) * g_pLocalPlayer()->m_flFrametime );
}

float Instruments::flGroundHeight() {
	 return ( g_pLocalPlayer()->m_vecOrigin[ 2 ] - g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.f, 0.f, 8192.0f ), 1, PM_NORMAL, -1 )->endpos[ 2 ] );
}

float Instruments::flAngleAtGround() {
	return acos( g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.0f, 0.0f, 8192.0f ), 1, PM_NORMAL, -1 )->plane.normal[ 2 ] ) / M_PI * 180;
}

bool Instruments::bSurf() { // if on surf or not
	return ( flAngleAtGround() >= 45.000001 && Instruments::Get()->flGroundHeight() < 30.0f );
}

bool Instruments::bSurfStrafeHelper() {
	return ( flAngleAtGround() >= 45.000001 && CCVars::Get()->strafe_control_helper_surffix->value && Instruments::Get()->flGroundHeight() < CCVars::Get()->strafe_control_helper_surffix_height->value );
}

pmtrace_s *get_facing_wall() { // it's not 100% a wall per se (doesn't check the plane or if it's world.), it's just the function name, but should be safe
	Vector maxforward = ( g_pLocalPlayer()->m_vecForward * Vector( 8192.f ) ), rightoversurface; // consider using velocity angle instead, but is it safe?
	//Vector maxforward = VectorAngles
	pmtrace_s *t = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.f, 0.f, 8192.f ), 1, PM_NORMAL, -1 );

	rightoversurface = t->endpos;
	rightoversurface += Vector( 0.f, 0.f, 1.f ); // locate the start for next trace right above ground so you don't get the edge if there is a cliff in front of you.

	return g_Engine.PM_TraceLine( rightoversurface, rightoversurface + maxforward, 1, PM_STUDIO_IGNORE, -1 );
}

float Instruments::get_edge_inair() {
	if( g_pLocalPlayer()->m_iFlags & FL_ONGROUND )
		return 0.f;

	Vector2D start;
	pmtrace_s *t = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.f, 0.f, 8192.f ), 1, PM_STUDIO_IGNORE , -1 );

	start = t->endpos.Make2D();

	t = get_facing_wall();
	
	return start.DistTo( t->endpos.Make2D() );
}

bool Instruments::is_above_facing_wall() {
	pmtrace_s *t = get_facing_wall();

	t = g_Engine.PM_TraceLine( t->endpos, t->endpos - Vector( 0.f, 0.f, 8192.f ), 1, PM_STUDIO_IGNORE, -1 );

	return t->endpos[ 2 ] > g_pLocalPlayer()->m_vecOrigin[ 2 ] ? false : true; // should i use - 14.f or not? There were no noticeable differences.
}

// N == vector axis of end-position of trace, 0 == x, 1 == y, 2 == z | Credits to RIscRIpt
float Instruments::GetTraceEndPosN( Vector start, Vector end, int N ) {
	pmtrace_s t;
	g_pLocalPlayer()->m_iUseHull == 1 ? g_Engine.pEventAPI->EV_SetTraceHull( 1 ) : g_Engine.pEventAPI->EV_SetTraceHull( 0 );
	g_Engine.pEventAPI->EV_PlayerTrace( start, end, PM_STUDIO_BOX, -1, &t );
	return t.endpos[ N ];
}

float Instruments::flEdgeDist() {
	float result = FLT_MAX;
	Vector vBase;

	vBase = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, g_pLocalPlayer()->m_vecOrigin - Vector( 0.0f, 0.0f, 8192.0f ), 1, 0, -1 )->endpos - Vector( 0.0f, 0.0f, 1.0f );

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
		if( g_pLocalPlayer()->m_flFallSpeed > 0.f )
			betweenzeroandone += g_Engine.pfnRandomFloat( 0.1f, 0.4f );

		if( CCVars::Get()->gs_max_speed_random->value ) {
			if( g_pLocalPlayer()->m_flVelocity > g_Engine.pfnRandomFloat( CCVars::Get()->gs_max_speed_random_min->value, CCVars::Get()->gs_max_speed_random_max->value ) )
				betweenzeroandone -= g_Engine.pfnRandomFloat( 0.2f, 0.6f );
		} else
			if( g_pLocalPlayer()->m_flVelocity > CCVars::Get()->gs_max_speed->value )
				betweenzeroandone -= g_Engine.pfnRandomFloat( 0.2f, 0.6f );

		betweenzeroandone += g_Engine.pfnRandomFloat( 0.1f, 0.4f );

		if( g_pLocalPlayer()->m_flVelocity > 350.f )
			betweenzeroandone = 0.5f;

		if( betweenzeroandone <= 0.8f && betweenzeroandone >= 0.2f )
			betweenzeroandone = 1.f;
		else
			betweenzeroandone = 0.f;
	}

	if( Reason == 2 ) {
		if( g_pLocalPlayer()->m_flVelocity > 383.f )
			return 1;

		return ( 200 < g_Engine.pfnRandomLong( 0, 100000 ) / 100 < 700 ) ? 1 : 0;
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

// credits to ir0n, bONE87
// trims whitespace from beginning and end of a string
static std::string Trim( std::string& str ) {
	constexpr static char whitespace[ 7 ] = "\t\n\v\f\r ";

	auto not_whitespace = str.find_first_not_of( whitespace );
	if( not_whitespace == std::string::npos )
		return str;

	str.erase( 0, not_whitespace );

	auto last_not_whitespace = str.find_last_not_of( whitespace );
	if( last_not_whitespace == std::string::npos )
		return str;

	str.erase( last_not_whitespace + 1 );

	return str;
}

// credits to ir0n, bONE87
// returned strings have been stripped of whitespace from beginning and end
static std::vector< std::string > SplitString( const std::string& text, const char seperator = ',' ) {
	constexpr static char kEscapeSequence = '\\';

	std::vector< std::string > tokens;
	std::string current;

	bool is_escaping = false;

	for( const auto& ch : text ) {
		if( is_escaping ) {
			current += ch;
			is_escaping = false;
		} else if( ch == kEscapeSequence ) {
			is_escaping = true;
		} else if( ch == seperator ) {
			tokens.emplace_back( Trim( current ) );
			current.clear();
		} else {
			current += ch;
		}
	}

	if( !current.empty() || text[ text.size() - 1 ] == seperator )
		tokens.emplace_back( Trim( current ) );

	return tokens;
}

std::vector<scrollpattern> Instruments::getPatterns() {
	FILE *pFile = fopen( szDirFile( XString( /*patterns.rn*/ 0x03, 0x0B, 0x41, 0x31233730, 0x2034293B, 0x67382500 ).c() ).c_str(), XString( /*r*/ 0x01, 0x01, 0x34, 0x46000000 ).c() );
	std::vector<scrollpattern> ret;

	if( pFile == NULL ) {
		g_Engine.Con_Printf( XString( /*[rN] Could not get patterns.*/ 0x07, 0x1C, 0x9D, 0xC6ECD1FD, 0x81E1CCD1, 0xC9C287C6, 0xC6DE8BCB, 0xC8DA8FC0, 0xD0C6C7D1, 0xC7D8C496 ).c() );
		return ret;
	}

	char str[ 255 ];
	std::vector<scrollframestruct> temp;

	while( !feof( pFile ) ) {
		fgets( str, sizeof( str ), pFile );

		if( strlen( str ) <= 0 || strcmp(str, "\n") == 0 ) {
			ret.push_back( temp );
			if( temp.size() > 0 )
				temp.clear();

			continue;
		}

		std::vector<std::string> strarr = SplitString( str );

		std::string endstr;

		// could replace with a switch statement.
		if( strarr.size() == 4 )
			endstr = XString( stoul( strarr.at( 0 ), nullptr, 16 ), stoul( strarr.at( 1 ), nullptr, 16 ), stoul( strarr.at( 2 ), nullptr, 16 ), stoul( strarr.at( 3 ), nullptr, 16 ) ).s();
		else if( strarr.size() == 5 )
			endstr = XString( stoul( strarr.at( 0 ), nullptr, 16 ), stoul( strarr.at( 1 ), nullptr, 16 ), stoul( strarr.at( 2 ), nullptr, 16 ), stoul( strarr.at( 3 ), nullptr, 16 ),
										  stoul( strarr.at( 4 ), nullptr, 16 ) ).s();
		else if( strarr.size() == 6 )
			endstr = XString( stoul( strarr.at( 0 ), nullptr, 16 ), stoul( strarr.at( 1 ), nullptr, 16 ), stoul( strarr.at( 2 ), nullptr, 16 ), stoul( strarr.at( 3 ), nullptr, 16 ),
										  stoul( strarr.at( 4 ), nullptr, 16 ), stoul( strarr.at( 5 ), nullptr, 16 ) ).s();

		std::vector<std::string> last = SplitString( endstr, ' ' );

		temp.push_back( scrollframestruct( atof( last.at( 1 ).c_str() ), atoi( last.at( 0 ).c_str() ) == 1 ) );
	}

	g_Engine.Con_Printf( XString( /*<rN> Successfully loaded %i patterns!*/ 0x0A, 0x25, 0xFB, 0xC78EB3C0, 0xDF537461, 0x60617675, 0x617D6566, 0x722C6161, 0x6E747476, 0x33317C36, 0x67796D6E, 0x7E6E736D,
								  0x3E000000 ).c(), ret.size() );

	return ret;
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

	pcvar->name = PrefHack( "", szNewCvarName );
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
	if( !pflFrom || !pflTo )
		return false;

	pmtrace_s pTrace;

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

float Instruments::PlayerHeight( int usehull ) { // credits to terazoid i think
	Vector vTemp( 0.f, 0.f, -8192.f );
	pmtrace_s pTrace;
	float ret;
	pmtrace_s *trace = g_Engine.PM_TraceLine( g_pLocalPlayer()->m_vecOrigin, vTemp, 1, usehull, -1 );

	ret	= g_pLocalPlayer()->m_vecOrigin.DistTo( trace->endpos );
	vTemp = trace->endpos;

	g_Engine.pEventAPI->EV_SetTraceHull( usehull );
	g_Engine.pEventAPI->EV_PlayerTrace( g_pLocalPlayer()->m_vecOrigin, vTemp, PM_GLASS_IGNORE | PM_STUDIO_BOX, g_pLocalPlayer()->m_iIndex, &pTrace );

	if( pTrace.fraction < 1.0f ) {
		int index = g_Engine.pEventAPI->EV_IndexFromTrace( &pTrace );
		SOtherPlayers& COP = g_OtherPlayers[ index ];

		ret	= g_pLocalPlayer()->m_vecOrigin.DistTo( pTrace.endpos );

		if( index >= 1 && index <= 32 ) {
			float dst = g_pLocalPlayer()->m_vecOrigin.z - ( g_pLocalPlayer()->m_iUseHull == 1 ? 32 : 18 ) - COP.m_vecOrigin.z - ret;

			if( dst < 30 )
				ret -= 14.0f;
		}
	}

	return ret;
}

inline char *Instruments::PrefHack( char *IfCmd, char *Name ) {
	char *o = new char[ 64 ];
	strcpy( o, IfCmd );
	strcat( o, Prefix_ini().c_str() );
	strcat( o, Name );
	return o;
}

// angles is output, forward is input
inline void Instruments::VectorAngles( const float *forward, float *angles ) {
	float tmp, yaw, pitch;

	if( forward[ 1 ] == 0 && forward[ 0 ] == 0 ) {
		yaw = 0;
		if( forward[ 2 ] > 0 )
			pitch = 270;
		else
			pitch = 90;
	} else {
		yaw = ( atan2( forward[ 1 ], forward[ 0 ] ) * 180 / M_PI );
		if( yaw < 0 )
			yaw += 360;
		tmp = sqrt( forward[ 0 ] * forward[ 0 ] + forward[ 1 ] * forward[ 1 ] );
		pitch = ( atan2( -forward[ 2 ], tmp ) * 180 / M_PI );
		if( pitch < 0 )
			pitch += 360;
	}

	angles[ 0 ] = pitch;
	angles[ 1 ] = yaw;
	angles[ 2 ] = 0;

	while( angles[ 0 ] < -89 ) { angles[ 0 ] += 180; angles[ 1 ] += 180; }
	while( angles[ 0 ] > 89 ) { angles[ 0 ] -= 180; angles[ 1 ] += 180; }
	while( angles[ 1 ] < -180 ) { angles[ 1 ] += 360; }
	while( angles[ 1 ] > 180 ) { angles[ 1 ] -= 360; }
}

std::string Instruments::szDirFile( char *pszName ) {
	std::string szRet = g_pLocalPlayer()->m_szBaseDir;
	return ( szRet + pszName );
}

std::string Instruments::Prefix_ini() {
	std::string prefix_ini;
	std::ifstream myfile( szDirFile( XString( /*prefix.ini*/ 0x03, 0x0A, 0x0D, 0x7D7C6A76, 0x786A3D7D, 0x7B7F0000 ).c() ) );

	if( !( myfile ) ) {
		prefix_ini = XString( /*rN^^*/ 0x01, 0x04, 0x5B, 0x29120300 ).c();
	}

	while( myfile ) {
		std::getline( myfile, prefix_ini );
	}

	myfile.close();

	return prefix_ini;
}

int Instruments::autodir() {		// if (mx > 0) mouse moves to the right, if (mx < 0) mouse moves to the left
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

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIML && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves left
		dir1 = 1;

	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIMR && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves right
		dir1 = 1;

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIMR && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves left
		dir1 = 2;

	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIML && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves right
		dir1 = 2;

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIF && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves left
		dir1 = 3;
	
	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIB && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves right
		dir1 = 3;

	if( g_pLocalPlayer()->m_iMX < 0 && g_pLocalPlayer()->m_bIB && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves left
		dir1 = 4;

	if( g_pLocalPlayer()->m_iMX > 0 && g_pLocalPlayer()->m_bIF && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) ) // mouse moves right
		dir1 = 4; 

	if( g_pLocalPlayer()->m_iMoveType == MOVETYPE_FLY ) {
		dir1 = 1;
		g_pLocalPlayer()->m_iOldDir = dir1;
	}

	if( g_pLocalPlayer()->m_iFlags&FL_ONGROUND )
		g_pLocalPlayer()->m_iOldDir = dir1;
	else
		dir1 = g_pLocalPlayer()->m_iOldDir;

	return dir1;
}
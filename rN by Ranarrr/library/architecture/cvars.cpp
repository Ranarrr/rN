#include "main.hpp"

CCVars::CCVars() {}

CCVars::~CCVars() {}

CCVars* CCVars::Get() {
	static CCVars sCVars;
	return &sCVars;
}

inline void CCVars::strreplace( char *buf, const char *search, const char *replace ) {
	char *p = buf;
	int l1 = strlen( search );
	int l2 = strlen( replace );

	while( p = strstr( p, search ) ) {
		memmove( p + l2, p + l1, strlen( p + l1 ) + 1 );
		memcpy( p, replace, l2 );
		p += l2;
	}
}

inline void CCVars::ExecFunc( char *str ) {
	char *name;
	FILE *pFile;
	name = str;

	if( !strstr( name, ".cfg" ) && !strstr( name, ".txt" ) ) {
		strcat( name, ".cfg" );

		if( !( pFile = fopen( Instruments::Get()->szDirFile( name ).c_str(), XString( /*r*/ 0x01, 0x01, 0x34, 0x46000000 ).c() ) ) ) {
			char *temp;
			strcpy( temp, name );
			strcpy( name, str );
			strcat( name, ".txt" );

			g_Engine.Con_Printf( XString( /*Couldn't find %s. Trying %s.*/ 0x07, 0x1C, 0x81, 0xC2EDF6E8, 0xE1E8A0FC, 0xA9ECE2E2, 0xE9AEAAE3, 0xBFB2C7E6, 0xECFFF9FF, 0xB9BFE8B2 ).c(), temp, name );
			
			if( !( pFile = fopen( Instruments::Get()->szDirFile( name ).c_str(), XString( /*r*/ 0x01, 0x01, 0x34, 0x46000000 ).c() ) ) ) {
				g_Engine.Con_Printf( XString( /*File: %s, */ 0x03, 0x0A, 0x86, 0xC0EEE4EC, 0xB0ABA9FE, 0x8EAF0000 ).c(), name );
				g_Engine.Con_Printf( ", " );
				g_Engine.Con_Printf( XString( /*not found.*/ 0x03, 0x0A, 0xAE, 0xC0C0C491, 0xD4DCC1DB, 0xD2990000 ).c() );
				g_Engine.Con_Printf( "\n" );
				return;
			}
		}
	} else
		pFile = fopen( Instruments::Get()->szDirFile( str ).c_str(), XString( /*r*/ 0x01, 0x01, 0x34, 0x46000000 ).c() );

	if( !pFile ) {
		g_Engine.Con_Printf( XString( /*Could not find the file.*/ 0x06, 0x18, 0x19, 0x5A756E70, 0x793E714F, 0x5502454D, 0x4B42075C, 0x414F0B4A, 0x44424A1E ).c() );
		return;
	}

	char strline[ 255 ];

	while( !feof( pFile ) ) {
		fgets( strline, sizeof( strline ), pFile );
		strreplace( strline, XString( /*#rN^*/ 0x01, 0x04, 0x63, 0x40162B38 ).c(), Instruments::Get()->Prefix_ini().c_str() );
		g_Engine.pfnClientCmd( strline );
	}

	fclose( pFile );
}

void exec() {
	if( g_pEngine->Cmd_Argc() == 2 )
		CCVars::Get()->ExecFunc( g_Engine.Cmd_Argv( 1 ) );
	else {
		g_pEngine->Con_Printf( "\t\t\t<rN> Usage: \n" );
		g_pEngine->Con_Printf( "\t\t\t\t\t\t<rN> Execfile {NAMEOFCFG}.cfg\n\n" );
	}
}

void CCVars::Init() {
	CCVars::Get()->ExecFunc( XString( /*rN.cfg*/ 0x02, 0x06, 0x1E, 0x6C510E42, 0x44440000 ).c() );

	g_Engine.Con_Printf( "\n\t\t" );
	g_Engine.Con_Printf( XString( /*rN v0.2 by Ranarrr*/ 0x05, 0x12, 0x83, 0xF1CAA5F0, 0xB7A6BBAA, 0xE9F5ADDC, 0xEEFEF0E0, 0xE1E60000 ).c() );
	g_Engine.Con_Printf( "\n\t\t" );
	g_Engine.Con_Printf( XString( /*Want to thank <3:*/ 0x05, 0x11, 0x36, 0x6156564D, 0x1A4F531D, 0x4A57212F, 0x29637876, 0x7C000000 ).c() );
	g_Engine.Con_Printf( "\n\n\t\t\t\t\t\t" );
	g_Engine.Con_Printf( XString( /*RIscRIpt(rissie) - Thanks for pseudocode C:*/ 0x0B, 0x2B, 0xB4, 0xE6FCC5D4, 0xEAF0CACF, 0x94CFD7CC, 0xB3A8A7EA, 0xE4E8E693, 0xA0A8A4A0, 0xBFEDA8A0, 0xA2F1A2A0, 0xB1A0B2B8, 0xBBB6BEBE, 0xFC9EE400 ).c() );
	g_Engine.Con_Printf( "\n\t\t\t\t\t\t" );
	g_Engine.Con_Printf( XString( /*GreySkull(CrapY) - gl on your game :D <3*/ 0x0A, 0x28, 0xBA, 0xFDC9D9C4, 0xEDD4B5AD, 0xAEEB87B7, 0xA7B791E0, 0xEAE6ECAA, 0xA2EFBFBF, 0xF2AABBA0, 0xA4F7BFB8, 0xB7BEFCE7, 0x9AFFDCD2 ).c() );
	g_Engine.Con_Printf( "\n\t\t\t\t\t\t" );
	g_Engine.Con_Printf( XString( /*ir0n - Insane coder :3*/ 0x06, 0x16, 0xCB, 0xA2BEFDA0, 0xEFFDF19B, 0xBDA7B4B8, 0xB2F8BAB5, 0xBFB9AFFE, 0xE5D30000 ).c() );
	g_Engine.Con_Printf( "\n\n\t\t\t" );
	g_Engine.Con_Printf( XString( /*<rN> Your prefix is %s*/ 0x06, 0x16, 0xE2, 0xDE91AADB, 0xC6BE879C, 0x98CB9C9F, 0x8B899989, 0xD29A87D5, 0xD3840000 ).c(), Instruments::Get()->Prefix_ini().c_str() );
	g_Engine.Con_Printf( "\n\t\t\t" );
	g_Engine.Con_Printf( XString( /*<rN> GL & HF*/ 0x03, 0x0C, 0xA9, 0x95D8E592, 0x8DE9E390, 0x9792FBF2 ).c() );
	g_Engine.Con_Printf( "\n" );
	
	// initialize patterns (intentionally vague)
	g_Engine.Con_Printf( "\t\t\t" );
	patterns = Instruments::Get()->getPatterns();
	g_Engine.Con_Printf( "\n" );

	// okey honestly, thinking back, much better to use a wrapper for this, but not a struct or a class and push back into vectors
	// just a nice define macro

	/*rN v0.10*/ bhop = reg_cvar( "bhop", "1" );
	/*rN v0.10*/ bhop_start = reg_cvar( "bhop_start", "12" );
	/*rN v0.10*/ bhop_end = reg_cvar( "bhop_end", "11" );
	/*rN v0.10*/ thirdperson = reg_cvar( "thirdperson", "0" );
	/*rN v0.10*/ thirdperson_side = reg_cvar( "thirdperson_side", "-2" );
	/*rN v0.10*/ thirdperson_up = reg_cvar( "thirdperson_up", "0" );
	/*rN v0.10*/ thirdperson_back = reg_cvar( "thirdperson_back", "100" );
	/*rN v0.10*/ thirdperson_alpha = reg_cvar( "thirdperson_alpha", "128" );
	/*rN v0.10*/ strafe_control_helper_althreshold = reg_cvar( "strafe_control_helper_althreshold", "-2" );
	/*rN v0.10*/ strafe_control_helper_arthreshold = reg_cvar( "strafe_control_helper_arthreshold", "2" ); 
	/*rN v0.10*/ strafe_control_helper_adelay = reg_cvar( "strafe_control_helper_adelay", "8" );
	/*rN v0.10*/ strafe_control_helper_dir = reg_cvar( "strafe_control_helper_dir", "1" );
	/*rN v0.10*/ strafe_control_helper_dir_auto = reg_cvar( "strafe_control_helper_dir_auto", "0" );
	/*rN v0.10*/ strafe_control_helper_forwardmove = reg_cvar( "strafe_control_helper_forwardmove", "0" ); 
	/*rN v0.10*/ strafe_control_helper_forwardmove_random = reg_cvar( "strafe_control_helper_forwardmove_random", "0" );
	/*rN v0.10*/ strafe_control_helper_forwardmove_random_max = reg_cvar( "strafe_control_helper_forwardmove_random_max", "110" );
	/*rN v0.10*/ strafe_control_helper_forwardmove_random_min = reg_cvar( "strafe_control_helper_forwardmove_random_min", "90" ); 
	/*rN v0.10*/ strafe_control_helper_max_strafe = reg_cvar( "strafe_control_helper_max_strafe", "8" );
	/*rN v0.10*/ strafe_control_helper_sidemove = reg_cvar( "strafe_control_helper_sidemove", "250" );
	/*rN v0.10*/ strafe_control_helper_sidemove_random = reg_cvar( "strafe_control_helper_sidemove_random", "1" );
	/*rN v0.10*/ strafe_control_helper_sidemove_random_max = reg_cvar( "strafe_control_helper_sidemove_random_max", "250" );
	/*rN v0.10*/ strafe_control_helper_sidemove_random_min = reg_cvar( "strafe_control_helper_sidemove_random_min", "100" );
	/*rN v0.10*/ speed = reg_cvar( "speed", "1000" );
	/*rN v0.10*/ fullbright = reg_cvar( "fullbright", "0" );
	/*rN v0.10*/ strafe_control_speed = reg_cvar( "strafe_control_speed", "0.8" );
	/*rN v0.10*/ strafe_control_helper = reg_cvar( "strafe_control_helper", "0" );
	/*rN v0.10*/ strafe_control_helper_add_strafe = reg_cvar( "strafe_control_helper_add_strafe", "0" );
	/*rN v0.10*/ strafe_hack_fps_boost = reg_cvar( "strafe_hack_fps_boost", "1" );
	/*rN v0.10*/ strafe_hack_skip_frames = reg_cvar( "strafe_hack_skip_frames", "12" );
	/*rN v0.10*/ strafe_hack_fps_helper = reg_cvar( "strafe_hack_fps_helper", "1000" );
	/*rN v0.10*/ strafe_hack_forwardmove = reg_cvar( "strafe_hack_forwardmove", "35891" );
	/*rN v0.10*/ strafe_hack_sidemove = reg_cvar( "strafe_hack_sidemove", "10000" );
	/*rN v0.10*/ edgebug = reg_cvar( "edgebug", "0" );
	/*rN v0.10*/ edgebug_edge_dist = reg_cvar( "edgebug_edge_dist", "10" );
	/*rN v0.10*/ edgebug_ground_dist = reg_cvar( "edgebug_ground_dist", "10" );
	/*rN v0.10*/ edgebug_intense = reg_cvar( "edgebug_intense", "0" );
	/*rN v0.10*/ fps_helper = reg_cvar( "fps_helper", "0" );
	/*rN v0.10*/ screeninfo = reg_cvar( "screeninfo", "1" );
	/*rN v0.10*/ strafe_control_helper_type = reg_cvar( "strafe_control_helper_type", "1" );
	/*rN v0.10*/ gs = reg_cvar( "gs", "1" );
	/*rN v0.10*/ gs_disable_jump = reg_cvar( "gs_disable_jump", "1" );
	/*rN v0.10*/ gs_max_speed = reg_cvar( "gs_max_speed", "303" );
	/*rN v0.10*/ gs_max_speed_random = reg_cvar( "gs_max_speed_random", "1" );
	/*rN v0.10*/ gs_max_speed_random_max = reg_cvar( "gs_max_speed_random_max", "335" );
	/*rN v0.10*/ gs_max_speed_random_min = reg_cvar( "gs_max_speed_random_min", "290" );
	/*rN v0.10*/ gs_nsd = reg_cvar( "gs_nsd", "0" );
	/*rN v0.10*/ gs_scroll = reg_cvar( "gs_scroll", "1" );
	/*rN v0.10*/ gs_scroll_fallspeed = reg_cvar( "gs_scroll_fallspeed", "5" );
	/*rN v0.10*/ gs_scroll_dist_end = reg_cvar( "gs_scroll_dist_end", "8" );
	/*rN v0.10*/ freelook_speed = reg_cvar( "freelook_speed", "700" );
	/*rN v0.10*/ reg_cmd( "Execfile", true, exec, nullptr )
	/*rN v0.10*/ /*r_norefresh*/		Instruments::Get()->HookCvarVariable( "norefresh", "r_norefresh", &r_norefresh, true );
	/*rN v0.10*/ /*fps_max*/			Instruments::Get()->HookCvarVariable( "fps_max", "fps_max", &fps_max, true );
	/*rN v0.10*/ /*fps_override*/	if( Instruments::Get()->HookCvarVariable( "fps_override", "fps_override", &fps_override, true ) )
		g_pLocalPlayer()->m_bIsSteam = true;
	/*rN v0.10*/ /*r_drawviewmodel*/	Instruments::Get()->HookCvarVariable( "r_drawviewmodel", "r_drawviewmodel", &r_drawviewmodel, true );
	/*rN v0.10*/ /*ex_interp*/		Instruments::Get()->HookCvarVariable( "ex_interp", "ex_interp", &ex_interp, true );

	/*rN v0.12*/ recoil_crosshair = reg_cvar( "recoil_crosshair", "0" );
	/*rN v0.12*/ strafe_control_speed_const = reg_cvar( "strafe_control_speed_const", "1" );
	/*rN v0.12*/ edge_jumpoff_dist = reg_cvar( "edge_jumpoff_dist", "0" );
	/*rN v0.12*/ strafe_control_helper_surffix = reg_cvar( "strafe_control_helper_surffix", "1" );
	/*rN v0.12*/ strafe_control_helper_surffix_height = reg_cvar( "strafe_control_helper_surffix_height", "10" );
	if( !g_pLocalPlayer()->m_bIsSteam )
		/*rN v0.12*/ /*developer*/		Instruments::Get()->HookCvarVariable( "developer", "developer", &developer, true );

	/*rN v0.13*/ recoil_control = reg_cvar( "recoil_control", "0" );
	/*rN v0.13*/ auto_pistol = reg_cvar( "auto_pistol", "0" );
	/*rN v0.13*/ auto_pistol_speed = reg_cvar( "auto_pistol_speed", "0" );
	/*rN v0.13*/ auto_duck = reg_cvar( "auto_duck", "0" );
	/*rN v0.13*/ auto_duck_dist = reg_cvar( "auto_duck_dist", "5" );

	//*rN v0.14*/ knifebot = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot"), "1", NULL);
	//*rN v0.14*/ knifebot_stab = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_stab"), "1", NULL);
	//*rN v0.14*/ knifebot_stab_dist = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_stab_dist"), "32", NULL);
	//*rN v0.14*/ knifebot_slash_dist = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_slash_dist"), "62", NULL);
	//*rN v0.14*/ knifebot_teammates = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_teammates"), "0", NULL);
	//*rN v0.14*/ knifebot_origin = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_origin"), "0", NULL);
	//*rN v0.14*/ knifebot_aim = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_aim"), "0", NULL);
	//*rN v0.14*/ knifebot_fov = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_fov"), "180", NULL);
	//*rN v0.14*/ knifebot_follow = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_follow"), "1", NULL);
	//*rN v0.14*/ knifebot_follow_dist = g_Engine.pfnRegisterVariable(PrefHack("", Prefix_ini(), "knifebot_follow_dist"), "100", NULL);

	/*rN v0.16*/ strafe_control_helper_rayish_fix = reg_cvar( "strafe_control_helper_rayish_fix", "1" );
	/*rN v0.16*/ bhop_wog = reg_cvar( "bhop_wog", "299" );
	/*rN v0.16*/ bhop_standup = reg_cvar( "bhop_standup", "0" );
	/*rN v0.16*/ bhop_standup_10aa = reg_cvar( "bhop_standup_10aa", "0" );

	/*rN v0.18*/ screeninfo_mult_x = reg_cvar( "screeninfo_mult_x", "0.5" );
	/*rN v0.18*/ screeninfo_mult_y = reg_cvar( "screeninfo_mult_y", "0.67" );
	/*rN v0.18*/ screeninfo_velocity = reg_cvar( "screeninfo_velocity", "01" );
	/*rN v0.18*/ screeninfo_fallspeed = reg_cvar( "screeninfo_fallspeed", "0" );
	/*rN v0.18*/ screeninfo_groundheight = reg_cvar( "screeninfo_groundheight", "1" );
	/*rN v0.18*/ screeninfo_edgedist = reg_cvar( "screeninfo_edgedist", "1" );

	/*rN v0.19*/ bhop_standup_dist = reg_cvar( "bhop_standup_dist", "2" );
	/*rN v0.19*/ //strafe_control_helper_sync_min
	/*rN v0.19*/ //strafe_control_helper_sync_max
	/*rN v0.19*/ //strafe_control_helper_sync_ticks
	/*rN v0.19*/ auto_duck_onlyblocks = reg_cvar( "auto_duck_onlyblocks", "1" );

	//cBuddy::Get()->RegisterCmds();
	CClient::Get()->RegisterCmds();
	CEsp::Get()->RegisterCVars();
	CNoFlash::Get()->RegisterCVars();
}
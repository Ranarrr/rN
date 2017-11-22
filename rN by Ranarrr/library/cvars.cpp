#include "main.hpp"

CCVars::CCVars() {}

CCVars::~CCVars() {}

CCVars* CCVars::Get() {
	static CCVars sCVars;
	return &sCVars;
}

inline void CCVars::strreplace( char* buf, const char* search, const char* replace ) {
	char* p = buf;
	int l1 = strlen( search );
	int l2 = strlen( replace );
	while( p = strstr( p, search ) ) {
		memmove( p + l2, p + l1, strlen( p + l1 ) + 1 );
		memcpy( p, replace, l2 );
		p += l2;
	}
}

inline void CCVars::ExecFunc( char *name ) {
	FILE *pFile = fopen( szDirFile( name ).c_str(), XString( /*r*/ 0x01, 0x01, 0x34, 0x46000000 ).c() );
	if( pFile == NULL ) {
		g_Engine.Con_Printf( XString( /*File: %s, */ 0x03, 0x0A, 0x86, 0xC0EEE4EC, 0xB0ABA9FE, 0x8EAF0000 ).c(), name );
		g_Engine.Con_Printf( ", " );
		g_Engine.Con_Printf( XString( /*not found.*/ 0x03, 0x0A, 0xAE, 0xC0C0C491, 0xD4DCC1DB, 0xD2990000 ).c() );
		g_Engine.Con_Printf( "\n" );
		return;
	}

	char str[ 255 ];

	while( !feof( pFile ) ) {
		fgets( str, sizeof( str ), pFile );
		strreplace( str, XString( /*#rN^*/ 0x01, 0x04, 0x63, 0x40162B38 ).c(), Prefix_ini().c_str() );
		g_Engine.pfnClientCmd( str );
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
	g_Engine.Con_Printf( XString( /*rN v0.17 by Ranarrr*/ 0x05, 0x13, 0xB7, 0xC5F699CC, 0x8B928C89, 0x9FA2B8E2, 0x91A5ABA7, 0xB5BABB00 ).c() );
	g_Engine.Con_Printf( "\n\t\t" );
	g_Engine.Con_Printf( XString( /*Want to thank <3:*/ 0x05, 0x11, 0x36, 0x6156564D, 0x1A4F531D, 0x4A57212F, 0x29637876, 0x7C000000 ).c() );
	g_Engine.Con_Printf( "\n\n\t\t\t\t\t\t" );
	g_Engine.Con_Printf( XString( /*RIscRIpt(rissie) - Thanks for pseudocode C:*/ 0x0B, 0x2B, 0xB4, 0xE6FCC5D4, 0xEAF0CACF, 0x94CFD7CC, 0xB3A8A7EA, 0xE4E8E693, 0xA0A8A4A0, 0xBFEDA8A0, 0xA2F1A2A0, 0xB1A0B2B8, 0xBBB6BEBE, 0xFC9EE400 ).c() );
	g_Engine.Con_Printf( "\n\t\t\t\t\t\t" );
	g_Engine.Con_Printf( XString( /*GreySkull(CrapY) - gl on your game :D <3*/ 0x0A, 0x28, 0xBA, 0xFDC9D9C4, 0xEDD4B5AD, 0xAEEB87B7, 0xA7B791E0, 0xEAE6ECAA, 0xA2EFBFBF, 0xF2AABBA0, 0xA4F7BFB8, 0xB7BEFCE7, 0x9AFFDCD2 ).c() );
	g_Engine.Con_Printf( "\n\t\t\t\t\t\t" );
	g_Engine.Con_Printf( XString( /*ir0n - Insane coder :3*/ 0x06, 0x16, 0xCB, 0xA2BEFDA0, 0xEFFDF19B, 0xBDA7B4B8, 0xB2F8BAB5, 0xBFB9AFFE, 0xE5D30000 ).c() );
	g_Engine.Con_Printf( "\n\n\t\t\t" );
	g_Engine.Con_Printf( XString( /*<rN> Your prefix is %s*/ 0x06, 0x16, 0xE2, 0xDE91AADB, 0xC6BE879C, 0x98CB9C9F, 0x8B899989, 0xD29A87D5, 0xD3840000 ).c(), Prefix_ini().c_str() );
	g_Engine.Con_Printf( "\n\t\t\t" );
	g_Engine.Con_Printf( XString( /*<rN> GL & HF*/ 0x03, 0x0C, 0xA9, 0x95D8E592, 0x8DE9E390, 0x9792FBF2 ).c() );
	g_Engine.Con_Printf( "\n" );

	// me and GreySkull discussing implementation of custom prefix through TeamViewer (because the one i have now is absolute garbage)

	// what flags could you have for RegisterVar? serveronly or not not sure what it does, but that's the desc of that param
	//Ah yes. Ok that makes sense. I know it's a bit of an overccomplication kinda
	// BUt I would just make a vector push for everycommand which then executes the register variable for them
	// not saying this is not good. Just saying :D
	// i hate this shit hahaha it looks so bad
	// like

	/*
	struct commands{
		commands(char* name, int defval, char flag){
		all = all;
		}
	    char* name;
		int defvalue;
		char flag;
	};

	vector<struct> commandlist;

	then you can do

	commandList.push_back(commands(Prefix, "name", "defval", flag));
	or smtn like that.
	Or you can wrap it into a class :D
	seems nice, but.. yea xD i mean.. if i knew this shit before i started, i would've prob done it that way, cause i cba to do this shit now hahaha

	And you can wrap it up like this:

	typename(something) registerCommand(char* name, int value){
	   commands tempCommand(catonate(prefix, name), value);
	   commandList.pushback(tempCommand);
	   
	}
	And then that can register everything and you only have to do: registerCommand("asdf", 12);
	It will auto make the prefix and everything :D
	true :p woosp


	*/

	/*rN v0.10*/ bhop = g_pEngine->pfnRegisterVariable( PrefHack( "", Prefix_ini(), "bhop" ), "1", NULL );
	/*rN v0.10*/ bhop_start = g_pEngine->pfnRegisterVariable( PrefHack( "", Prefix_ini(), "bhop_start" ), "12", NULL );
	/*rN v0.10*/ bhop_end = g_pEngine->pfnRegisterVariable( PrefHack( "", Prefix_ini(), "bhop_end" ), "11", NULL );
	/*rN v0.10*/ thirdperson = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "thirdperson" ), "0", NULL );
	/*rN v0.10*/ thirdperson_side = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "thirdperson_side" ), "-2", NULL );
	/*rN v0.10*/ thirdperson_up = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "thirdperson_up" ), "0", NULL );
	/*rN v0.10*/ thirdperson_back = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "thirdperson_back" ), "100", NULL );
	/*rN v0.10*/ thirdperson_alpha = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "thirdperson_alpha" ), "128", NULL );
	/*rN v0.10*/ js = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "jumpstats" ), "0", NULL );
	/*rN v0.10*/ js_min = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "jumpstats_min_jump" ), "190", NULL );
	/*rN v0.10*/ js_time = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "jumpstats_time" ), "400", NULL );
	/*rN v0.10*/ js_in_console = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "jumpstats_in_console" ), "0", NULL );
	/*rN v0.10*/ strafe_control_helper_althreshold = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_althreshold" ), "-2", NULL );
	/*rN v0.10*/ strafe_control_helper_arthreshold = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_arthreshold" ), "2", NULL );
	/*rN v0.10*/ strafe_control_helper_adelay = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_adelay" ), "8", NULL );
	/*rN v0.10*/ strafe_control_helper_dir = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_dir" ), "1", NULL );
	/*rN v0.10*/ strafe_control_helper_dir_auto = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_dir_auto" ), "0", NULL );
	/*rN v0.10*/ strafe_control_helper_forwardmove = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_forwardmove" ), "0", NULL );
	/*rN v0.10*/ strafe_control_helper_forwardmove_random = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_forwardmove_random" ), "0", NULL );
	/*rN v0.10*/ strafe_control_helper_forwardmove_random_max = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_forwardmove_random_max" ), "110", NULL );
	/*rN v0.10*/ strafe_control_helper_forwardmove_random_min = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_forwardmove_random_min" ), "90", NULL );
	/*rN v0.10*/ strafe_control_helper_max_strafe = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_max_strafe" ), "8", NULL );
	/*rN v0.10*/ strafe_control_helper_sidemove = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_sidemove" ), "250", NULL );
	/*rN v0.10*/ strafe_control_helper_sidemove_random = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_sidemove_random" ), "1", NULL );
	/*rN v0.10*/ strafe_control_helper_sidemove_random_max = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_sidemove_random_max" ), "250", NULL );
	/*rN v0.10*/ strafe_control_helper_sidemove_random_min = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_sidemove_random_min" ), "100", NULL );
	/*rN v0.10*/ speed = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "speed" ), "1000", NULL );
	/*rN v0.10*/ fullbright = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "fullbright" ), "0", NULL );
	/*rN v0.10*/ strafe_control_speed = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_speed" ), "1.2", NULL );
	/*rN v0.10*/ strafe_control_helper = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper" ), "0", NULL );
	/*rN v0.10*/ strafe_control_helper_add_strafe = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_add_strafe" ), "0", NULL );
	/*rN v0.10*/ strafe_hack_fps_boost = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_hack_fps_boost" ), "1", NULL );
	/*rN v0.10*/ strafe_hack_skip_frames = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_hack_skip_frames" ), "12", NULL );
	/*rN v0.10*/ strafe_hack_fps_helper = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_hack_fps_helper" ), "1000", NULL );
	/*rN v0.10*/ strafe_hack_forwardmove = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_hack_forwardmove" ), "35891", NULL );
	/*rN v0.10*/ strafe_hack_sidemove = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_hack_sidemove" ), "10000", NULL );
	/*rN v0.10*/ edgebug = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "edgebug" ), "0", NULL );
	/*rN v0.10*/ edgebug_edge_dist = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "edgebug_edge_dist" ), "10", NULL );
	/*rN v0.10*/ edgebug_ground_dist = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "edgebug_ground_dist" ), "10", NULL );
	/*rN v0.10*/ edgebug_intense = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "edgebug_intense" ), "0", NULL );
	/*rN v0.10*/ fps_helper = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "fps_helper" ), "0", NULL );
	/*rN v0.10*/ screeninfo = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "screeninfo" ), "1", NULL );
	/*rN v0.10*/ strafe_control_helper_type = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_type" ), "1", NULL );
	/*rN v0.10*/ gs = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs" ), "1", NULL );
	/*rN v0.10*/ gs_disable_jump = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_disable_jump" ), "1", NULL );
	/*rN v0.10*/ gs_max_speed = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_max_speed" ), "289", NULL );
	/*rN v0.10*/ gs_max_speed_random = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_max_speed_random" ), "1", NULL );
	/*rN v0.10*/ gs_max_speed_random_max = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_max_speed_random_max" ), "303", NULL );
	/*rN v0.10*/ gs_max_speed_random_min = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_max_speed_random_min" ), "256", NULL );
	/*rN v0.10*/ gs_nsd = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_nsd" ), "0", NULL );
	/*rN v0.10*/ gs_scroll = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_scroll" ), "1", NULL );
	/*rN v0.10*/ gs_scroll_fallspeed = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_scroll_fallspeed" ), "5", NULL );
	/*rN v0.10*/ gs_scroll_dist_end = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "gs_scroll_dist_end" ), "8", NULL );
	/*rN v0.10*/ freelook_speed = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "freelook_speed" ), "700", NULL );
	/*rN v0.10*/ g_pEngine->pfnAddCommand( PrefHack( "", Prefix_ini(), "Execfile" ), exec );
	/*rN v0.10*/ /*r_norefresh*/		Instruments::Get()->HookCvarVariable( PrefHack( "", Prefix_ini(), "norefresh" ), "r_norefresh", &r_norefresh, true );
	/*rN v0.10*/ /*fps_max*/			Instruments::Get()->HookCvarVariable( PrefHack( "", Prefix_ini(), "fps_max" ), "fps_max", &fps_max, true );
	/*rN v0.10*/ /*fps_override*/	if( Instruments::Get()->HookCvarVariable( PrefHack( "", Prefix_ini(), "fps_override" ), "fps_override", &fps_override, true ) )
		g_pLocalPlayer()->m_bIsSteam = true;
	/*rN v0.10*/ /*r_drawviewmodel*/	Instruments::Get()->HookCvarVariable( PrefHack( "", Prefix_ini(), "r_drawviewmodel" ), "r_drawviewmodel", &r_drawviewmodel, true );
	/*rN v0.10*/ /*ex_interp*/		Instruments::Get()->HookCvarVariable( PrefHack( "", Prefix_ini(), "ex_interp" ), "ex_interp", &ex_interp, true );

	/*rN v0.12*/ recoil_crosshair = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "recoil_crosshair" ), "0", NULL );
	/*rN v0.12*/ strafe_control_speed_const = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_speed_const" ), "1", NULL );
	/*rN v0.12*/ edge_jumpoff_dist = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "edge_jumpoff_dist" ), "0", NULL );
	/*rN v0.12*/ strafe_control_helper_surffix = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_surffix" ), "1", NULL );
	/*rN v0.12*/ strafe_control_helper_surffix_height = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_surffix_height" ), "10", NULL );
	if( !g_pLocalPlayer()->m_bIsSteam )
		/*rN v0.12*/ /*developer*/		Instruments::Get()->HookCvarVariable( PrefHack( "", Prefix_ini(), "developer" ), "developer", &developer, true );

	/*rN v0.13*/ recoil_control = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "recoil_control" ), "0", NULL );
	/*rN v0.13*/ auto_pistol = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "auto_pistol" ), "0", NULL );
	/*rN v0.13*/ auto_pistol_speed = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "auto_pistol_speed" ), "0", NULL );
	/*rN v0.13*/ auto_duck = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "auto_duck" ), "0", NULL );
	/*rN v0.13*/ auto_duck_dist = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "auto_duck_dist" ), "5", NULL );

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

	/*rN v0.16*/ strafe_control_helper_rayish_fix = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "strafe_control_helper_rayish_fix" ), "1", NULL );
	/*rN v0.16*/ bhop_wog = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "bhop_wog" ), "299", NULL );
	/*rN v0.16*/ bhop_standup = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "bhop_standup" ), "1", NULL );
	/*rN v0.16*/ bhop_standup_10aa = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "bhop_standup_10aa" ), "0", NULL );

	//cBuddy::Get()->RegisterCmds();
	CClient::Get()->RegisterCmds();
	CEsp::Get()->RegisterCVars();
	CNoFlash::Get()->RegisterCVars();
}
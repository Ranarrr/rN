#include "client.hpp"

// rN v0.2 by Ranarrr source release.
// Credits:
//
// - RIscRIpt
// - Terazoid
// - ir0n ( big love to a big man <3 )
// - _or_75 ( cause ur base helped me a little with angleatground, theres a bug btw :) ( hint: 45.000001 ! 45 ) )
// - GreySkull ( CrapY ) where are you?
// - Others whom i've excluded..

//Other stuff
DWORD_PTR g_dwSpeed;
POINT current_m_pos, last_m_pos;

// autopistol
bool shoot;

// bhop
int shouldkeepjumping;

// strafe controller and helper
static int istrs, idelay;

// sync based helper
int ticks, tickstostr, tickspassed;
int right, prevright;

// pattern
bool shouldgetPattern = true;
int tick = 0;
scrollpattern pattern;

//ints
int mxl, mxr, addstr_delay;
int tickssincelastgs;

//bools
bool bDGstrafe, FreeLookEnabled, DSTR, DJB;

// freelook
Vector FreeLookOffset;

//Floats
float scale;
float oldfps;
float oldoverrideordev;

//Vectors
Vector vecPrevpunch( 0, 0, 0 );

CClient::CClient() {}

CClient::~CClient() {}

CClient* CClient::Get() {
	static CClient sClient;
	return &sClient;
}

void gson() { bDGstrafe = true; }
void gsoff() { bDGstrafe = false; }

void jbon() { DJB = true; }
void jboff() { DJB = false; }

void stron() {
	if( CCVars::Get()->strafe_hack_fps_boost->value ) {
		oldfps = *CCVars::Get()->fps_max;
		*CCVars::Get()->fps_max = 1000.0f;
		oldoverrideordev = g_pLocalPlayer()->m_bIsSteam ? *CCVars::Get()->fps_override : *CCVars::Get()->developer;
		g_pLocalPlayer()->m_bIsSteam ? *CCVars::Get()->fps_override = 1.0f : *CCVars::Get()->developer = 1.0f;
		CCVars::Get()->ExecFunc( XString( /*+fps.cfg*/ 0x02, 0x08, 0x04, 0x2F637674, 0x266A6C6C ).c() );
	}

	DSTR = true;
}

void stroff() {
	if( CCVars::Get()->strafe_hack_fps_boost->value ) {
		if( oldfps > 20 ) {
			*CCVars::Get()->fps_max = oldfps;
			g_pEngine->Cvar_SetValue( "fps_max", oldfps );
		}
			
		g_pLocalPlayer()->m_bIsSteam ? *CCVars::Get()->fps_override = oldoverrideordev : *CCVars::Get()->developer = oldoverrideordev;
		CCVars::Get()->ExecFunc( XString( /*-fps.cfg*/ 0x02, 0x08, 0x8F, 0xA2F6E1E1, 0xBDF7F3F1 ).c() );
	}

	DSTR = false;
}

void FreeLook() {
	FreeLookEnabled = !FreeLookEnabled;

	if( FreeLookEnabled ) {
		FreeLookOffset.Init();
		FreeLookOffset = g_pLocalPlayer()->m_vecOrigin + Vector( 0.f, 0.f, 80.0f );
	}
}

void reload_patterns() {
	CCVars::Get()->patterns = Instruments::Get()->getPatterns();
}

void CClient::HUD_Frame( double time ) {
	static bool bFirstTime = true;

	if( bFirstTime ) {
		CModule::Get()->Hide( g_hInstance );
		CModule::Get()->HideExtra( g_hInstance );
		CModule::Get()->HideFromPEB( g_hInstance );
		CModule::Get()->RemovePE( ( DWORD ) g_hInstance );
		CModule::Get()->DestroyHeader( g_hInstance );

		CCVars::Get()->Init();

		g_dwSpeed = ( DWORD_PTR ) COffsets::Get()->GetSpeedPtr();
		g_Screen.m_iSize = sizeof( SCREENINFO_s );
		g_Engine.pfnGetScreenInfo( &g_Screen );

		g_Engine.pfnClientCmd( XString( /*toggleconsole*/ 0x04, 0x0D, 0x29, 0x5D454C4B, 0x414B4C5F, 0x5F415C58, 0x50000000 ).c() );

		bFirstTime = false;
	}

	g_Client.HUD_Frame( time );
}

void CClient::RegisterCmds() {
	reg_cmd( "gs", false, gson, gsoff )
	reg_cmd( "strafehack", false, stron, stroff )
	reg_cmd( "jumpbug", false, jbon, jboff )
	reg_cmd( "freelook", true, FreeLook, nullptr )
	reg_cmd( "patterns_reload", true, reload_patterns, nullptr )
}

void CClient::HUD_Redraw( float time, int intermission ) {
	g_Client.HUD_Redraw( time, intermission );

	if( CCVars::Get()->screeninfo->value ) {
		float spacing = 0.f;

		if( CCVars::Get()->screeninfo_velocity->value ) {
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth * CCVars::Get()->screeninfo_mult_x->value, g_Screen.m_iHeight * CCVars::Get()->screeninfo_mult_y->value, Color::White(),
											   XString( /*Velocity: %f*/ 0x03, 0x0C, 0xDE, 0x88BA8C8E, 0x818A909C, 0xDCC7CD8F ).c(), g_pLocalPlayer()->m_flVelocity );
			spacing += 0.02f;
		}

		if( CCVars::Get()->screeninfo_fallspeed->value ) {
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth * CCVars::Get()->screeninfo_mult_x->value, g_Screen.m_iHeight * ( CCVars::Get()->screeninfo_mult_y->value + spacing ), Color::White(),
											   XString( /*FallSpeed: %f*/ 0x04, 0x0D, 0xAC, 0xEACCC2C3, 0xE3C1D7D6, 0xD08F9692, 0xDE000000 ).c(), g_pLocalPlayer()->m_flFallSpeed );
			spacing += 0.02f;
		}
		
		if( CCVars::Get()->screeninfo_groundheight->value ) {
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth * CCVars::Get()->screeninfo_mult_x->value, g_Screen.m_iHeight * ( CCVars::Get()->screeninfo_mult_y->value + spacing ), Color::White(),
											   XString( /*GroundHeight: %f*/ 0x04, 0x10, 0xCB, 0x8CBEA2BB, 0xA1B499B7, 0xBAB3BDA2, 0xEDF8FCBC ).c(), Instruments::Get()->flGroundHeight() );
			spacing += 0.02f;
		}
		
		if( CCVars::Get()->screeninfo_edgedist->value )
			CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth * CCVars::Get()->screeninfo_mult_x->value, g_Screen.m_iHeight * ( CCVars::Get()->screeninfo_mult_y->value + spacing ), Color::White(),
										   XString( /*Edge dist: %f*/ 0x04, 0x0D, 0xC8, 0x8DADADAE, 0xECA9A7BC, 0xA4EBF2F6, 0xB2000000 ).c(), Instruments::Get()->flEdgeDist() );

		if( CCVars::Get()->noflash->value )
			CNoFlash::Get()->NoFlashOn();

		if( CCVars::Get()->recoil_crosshair->value ) {
			Vector punchangle = g_pLocalPlayer()->m_vecPunchAngle;

			int x = g_Screen.m_iWidth / 2;
			int y = g_Screen.m_iHeight / 2;
			int dy = g_Screen.m_iHeight / 90;
			int dx = g_Screen.m_iWidth / 90;
			x -= ( dx * ( punchangle.y ) );
			y += ( dy * ( punchangle.x ) );

			CDrawing::Get()->DrawRectangleOutlined( g_Screen.m_iWidth / 2 - g_pLocalPlayer()->m_vecPunchAngle[ 1 ] * 10 - 1, g_Screen.m_iHeight / 2 + g_pLocalPlayer()->m_vecPunchAngle[ 0 ] * 10 - 1, 3, 3, Color::White(), Color::Black() );
		}
	}

	if( CCVars::Get()->esp->value ) {
		for( int iIndex = 1; iIndex <= g_pEngine->GetMaxClients(); ++iIndex ) {
			if( iIndex == g_pLocalPlayer()->m_iIndex )
				continue;

			SOtherPlayers& COP = g_OtherPlayers[ iIndex ];

			if( !( COP.m_bUpdated ) )
				continue;

			CEsp::Get()->PlayerEsp( iIndex );
			COP.m_bUpdated = false;
		}
	}
}

void CClient::HUD_PlayerMove( playermove_s *ppmove, int server ) {
	g_Client.HUD_PlayerMove( ppmove, server );

	cl_entity_s *pLocal = g_Engine.GetLocalPlayer();
	g_pLocalPlayer()->m_iIndex = pLocal->index;
	
	g_pLocalPlayer()->m_vecOrigin = ppmove->origin;
	g_pLocalPlayer()->m_flVelocity = sqrt( POW( ppmove->velocity[ 0 ] ) + POW( ppmove->velocity[ 1 ] ) );
	g_pLocalPlayer()->m_iFlags = ppmove->flags;
	g_pLocalPlayer()->m_flFrametime = ppmove->frametime;
	g_pLocalPlayer()->m_iWaterLevel = ppmove->waterlevel;
	g_pLocalPlayer()->flSwimTime = ppmove->flSwimTime;
	g_pLocalPlayer()->m_iMoveType = ppmove->movetype;
	g_pLocalPlayer()->m_iUseHull = ppmove->usehull;
	g_pLocalPlayer()->m_iGravity = ppmove->gravity;
	g_pLocalPlayer()->m_bInDuckppmove = ppmove->bInDuck;
	g_pLocalPlayer()->m_vecBaseVelocity = ppmove->basevelocity;
	g_pLocalPlayer()->m_flFallSpeed = ppmove->flFallVelocity;
	g_pLocalPlayer()->m_vecVelocity = ppmove->velocity;
	g_pLocalPlayer()->m_iOldButtons = ppmove->oldbuttons;

	g_pLocalPlayer()->m_flClientMaxSpeed = ppmove->clientmaxspeed;
	g_pLocalPlayer()->m_flMaxSpeed = ppmove->maxspeed;

	g_pEngine->GetMaxClients() <= 1 ? g_pLocalPlayer()->m_bIsConnected = false : g_pLocalPlayer()->m_bIsConnected = true;
	g_Engine.GetViewAngles( g_pLocalPlayer()->m_vecViewAngles );
	g_Engine.pEventAPI->EV_LocalPlayerViewheight( g_pLocalPlayer()->m_vecViewHeight );
	g_pLocalPlayer()->m_vecEyePosition = g_pLocalPlayer()->m_vecViewHeight + ppmove->origin;
}

void CClient::V_CalcRefdef( ref_params_s *pParams ) {
	if( pParams->nextView == 0 )
		 g_pLocalPlayer()->m_vecPunchAngle = pParams->punchangle;

	g_pLocalPlayer()->m_vecViewOrg = pParams->vieworg;

	g_Client.V_CalcRefdef( pParams );

	if( g_pLocalPlayer()->m_bIA && CCVars::Get()->recoil_control->value ) {
		pParams->cl_viewangles[ 0 ] -= ( g_pLocalPlayer()->m_vecPunchAngle[ 0 ] - vecPrevpunch[ 0 ] ) * 2;
		pParams->cl_viewangles[ 1 ] -= ( g_pLocalPlayer()->m_vecPunchAngle[ 1 ] - vecPrevpunch[ 1 ] ) * 2;
		vecPrevpunch = g_pLocalPlayer()->m_vecPunchAngle;
	} else
		vecPrevpunch = { 0, 0, 0 };

	g_pLocalPlayer()->m_vecForward = pParams->forward;
	g_pLocalPlayer()->m_vecRight = pParams->right;

	if( CCVars::Get()->thirdperson->value ) {
		Vector vecOffset;
		Vector vecSide, vecUp, vecForward;

		vecSide = ( pParams->right * CCVars::Get()->thirdperson_side->value );
		vecUp = ( pParams->up * CCVars::Get()->thirdperson_up->value );
		vecForward = ( pParams->forward * -( CCVars::Get()->thirdperson_back->value ) );

		vecOffset += ( vecSide + vecUp + vecForward );

		pParams->vieworg += vecOffset;
	}

	if( FreeLookEnabled ) {
		memcpy( pParams->vieworg, FreeLookOffset, sizeof( float ) * 3 );
		memcpy( pParams->simorg, FreeLookOffset, sizeof( float ) * 3 );
	}
}

int CClient::HUD_AddEntity( int type, cl_entity_s *ent, const char *modelname ) {
	int iRet = g_Client.HUD_AddEntity( type, ent, modelname );
	cl_entity_s *pLocal = g_Engine.GetLocalPlayer();
	SOtherPlayers& COP = g_OtherPlayers[ ent->index ];

	if( ent && ent->player && ent->index >= 1 && ent->index <= g_pEngine->GetMaxClients() && ( ent->index != g_pLocalPlayer()->m_iIndex ) ) {
		COP.m_bUpdated = Instruments::Get()->bIsValidEnt( ent->index );

		g_Engine.pfnGetPlayerInfo( ent->index, &COP.m_Info );

		COP.m_flFrametime = ent->curstate.animtime - ent->prevstate.animtime;

		COP.m_vecVelocity = ent->origin - ent->prevstate.origin;

		COP.m_vecOrigin = ent->origin;

		COP.m_bVisible = Instruments::Get()->bIsVisible( g_pLocalPlayer()->m_vecEyePosition, ent->origin );

		COP.m_flDistance = COP.m_vecOrigin.DistTo( g_pLocalPlayer()->m_vecOrigin );

		COP.m_iHealth = ent->curstate.health;
	}

	if( pLocal && CCVars::Get()->thirdperson->value ) {
		pLocal->curstate.rendermode = kRenderTransAlpha;
		pLocal->curstate.renderamt = CCVars::Get()->thirdperson_alpha->value;
	}

	return iRet;
}

void CClient::HookEngine() {
	memcpy( &g_Engine, ( LPVOID ) g_pEngine, sizeof( cl_enginefuncs_s ) );
	/* all engine hooks here */
}

void CClient::HookStudio() {
	memcpy( &g_Studio, ( LPVOID ) g_pStudio, sizeof( engine_studio_api_s ) );
	/* all studio hooks here */
}

int CClient::CL_IsThirdPerson() {
	if( CCVars::Get()->thirdperson->value )
		return 1;

	return g_Client.CL_IsThirdPerson();
}

void CClient::CL_CreateMove( float flFrameTime, usercmd_s *pCmd, int iActive ) {
	GetCursorPos( &current_m_pos );

	// if (mx > 0) mouse moves to the right, if (mx < 0) mouse moves to the left
	if( g_pLocalPlayer()->m_bIsSteam ) {
		if( g_Engine.pfnGetCvarPointer( "m_rawinput" )->value ) {
			if( current_m_pos.x >= ( g_Engine.GetWindowCenterX() * 2 ) - 1 )
				g_pLocalPlayer()->m_iMX = 100;
			else if( current_m_pos.x <= 0 )
				g_pLocalPlayer()->m_iMX = -100;
			else
				g_pLocalPlayer()->m_iMX = ( current_m_pos.x - last_m_pos.x );

			GetCursorPos( &last_m_pos );
		} else
			g_pLocalPlayer()->m_iMX = ( current_m_pos.x - g_Engine.GetWindowCenterX() );
	} else
		g_pLocalPlayer()->m_iMX = ( current_m_pos.x - g_Engine.GetWindowCenterX() );
	
	g_Client.CL_CreateMove( flFrameTime, pCmd, iActive );

	/*

	movetype 6 MAKE WHEN U DIE CMD <-----

	*/

	pCmd->buttons & IN_DUCK ? g_pLocalPlayer()->m_bInDuckCmd = true : g_pLocalPlayer()->m_bInDuckCmd = false;

	*( double* ) g_dwSpeed = max( 1, ( CCVars::Get()->speed->value ) );

	g_pLocalPlayer()->m_bIA = pCmd->buttons & IN_ATTACK ? true : false;
	g_pLocalPlayer()->m_bIML = pCmd->buttons & IN_MOVELEFT ? true : false; g_pLocalPlayer()->m_bIMR = pCmd->buttons & IN_MOVERIGHT ? true : false;
	g_pLocalPlayer()->m_bIF = pCmd->buttons & IN_FORWARD ? true : false; g_pLocalPlayer()->m_bIB = pCmd->buttons & IN_BACK ? true : false;

	//Auto-Pistol---------
	if( CCVars::Get()->auto_pistol->value && pCmd->buttons & IN_ATTACK && g_pLocalPlayer()->m_iClip > 0 && CCVars::Get()->auto_pistol_speed->value && Instruments::Get()->HoldingPistol() ) {
		if( g_pLocalPlayer()->m_iShootTimer >= CCVars::Get()->auto_pistol_speed->value ) {
			shoot ? pCmd->buttons &= ~IN_ATTACK : pCmd->buttons |= IN_ATTACK;
			shoot = !shoot;
			g_pLocalPlayer()->m_iShootTimer = 0;
		}
		++g_pLocalPlayer()->m_iShootTimer;
	}
	//-------------------

	// for althreshold and arthreshold
	if( g_pLocalPlayer()->m_iMX > 0 )
		mxr += g_pLocalPlayer()->m_iMX;
	else if( g_pLocalPlayer()->m_iMX < 0 )
		mxl -= g_pLocalPlayer()->m_iMX;
	//-------------------------

	//Auto-Duck----------
	if( CCVars::Get()->auto_duck->value )
		if( CCVars::Get()->auto_duck_dist->value > Instruments::Get()->flGroundHeight() && !CCVars::Get()->auto_duck_onlyblocks->value ) {
			if( ~g_pLocalPlayer()->m_iFlags & FL_ONGROUND )
				pCmd->buttons |= IN_DUCK;
		} else {
			if( Instruments::Get()->is_above_facing_wall() && Instruments::Get()->get_edge_inair() < 25.f && ~g_pLocalPlayer()->m_iFlags & FL_ONGROUND )
				pCmd->buttons |= IN_DUCK;
		}
			
	//-------------------

	if( CCVars::Get()->strafe_control_helper_dir_auto->value )
		CCVars::Get()->strafe_control_helper_dir->value = Instruments::Get()->autodir() > 0 ? Instruments::Get()->autodir() : 1;

	//fps helper
	if( CCVars::Get()->fps_helper->value ) {
		*( double* ) g_dwSpeed = ( g_pLocalPlayer()->m_iFPS / CCVars::Get()->fps_helper->value ) * 1000;
	}
	//-----------

	//EDGEBUG
	if( CCVars::Get()->edgebug->value && g_pLocalPlayer()->m_iWaterLevel < 2 ) {
		if( g_pLocalPlayer()->m_flFallSpeed > 250.0f && Instruments::Get()->flEdgeDist() <= CCVars::Get()->edgebug_edge_dist->value && Instruments::Get()->flGroundHeight() <= CCVars::Get()->edgebug_ground_dist->value ) {
			if( !( Instruments::Get()->bSurf() ) ) {
				CCVars::Get()->edgebug_intense->value ? *( double* ) g_dwSpeed = 10000.0f : *( double* ) g_dwSpeed = 3500.0f;
			}
		}
	}
	//---------

	// my try at a speedhack type jumpbug, didn't try too hard
	/*static int state = 0;
	if( DJB && g_pLocalPlayer()->m_flFallSpeed > 0 && !( Instruments::Get()->bSurf() ) ) {
	pCmd->buttons |= IN_DUCK;
	pCmd->buttons &= ~IN_JUMP;



	switch( state ) {
	case 1:
	pCmd->buttons &= ~IN_DUCK;
	pCmd->buttons |= IN_JUMP;
	state = 2;
	break;

	case 2:
	state = 0;
	break;

	default:
	if( Instruments::Get()->flGroundDistMeasuredInFrames() <= 15.f ) {
	float scale = ( Instruments::Get()->flGroundDistMeasuredInFrames() / Instruments::Get()->flGroundHeight() ) * 1000;
	*( double* ) g_dwSpeed = scale > 1000 ? scale : 1000;
	state = 1;
	}
	break;
	}
	}*/

	static int state = 0;
	if( DJB && g_pLocalPlayer()->m_flFallSpeed > 0 && !( Instruments::Get()->bSurf() ) ) // JumpBug c&p from terazoid
	{
		float flpheight = Instruments::Get()->PlayerHeight( 2 );
		float h2 = Instruments::Get()->PlayerHeight( 1 );
		if( flpheight - h2 > 25 )
			flpheight = h2;
		else
			flpheight -= 18.0f;
		static float last_h = 0.0f;
		float cur_frame_zdist = g_pLocalPlayer()->m_flFallSpeed * flFrameTime;
		pCmd->buttons |= IN_DUCK;
		pCmd->buttons &= ~IN_JUMP;
		switch( state ) {
		case 1:
			pCmd->buttons &= ~IN_DUCK;
			pCmd->buttons |= IN_JUMP;
			state = 2;
			break;
		case 2:
			state = 0;
			break;
		default:
			if( flpheight - cur_frame_zdist * 1.5 <= ( 20.0 ) && cur_frame_zdist > 0.0f ) {
				float needspd = flpheight - ( 19.0f );
				scale = needspd / cur_frame_zdist;
				*( double* ) g_dwSpeed = scale * 1000;
				state = 1;
			}
			break;
		}
		last_h = flpheight;
	}

	if( DSTR ) {
		if( g_pLocalPlayer()->m_iFrameN < CCVars::Get()->strafe_hack_skip_frames->value && CCVars::Get()->strafe_hack_fps_boost->value ) {
			*CCVars::Get()->r_norefresh = 1.0f;
			g_pLocalPlayer()->m_iFrameN++;
		} else {
			*CCVars::Get()->r_norefresh = 0.0f;
			g_pLocalPlayer()->m_iFrameN = 0;
		}
	} else
		*CCVars::Get()->r_norefresh = 0.0f;

	if( DSTR && !( g_pLocalPlayer()->m_iFlags&FL_ONGROUND ) && ( g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY ) ) {
		static bool strafing;
		strafing = !strafing;
		if( CCVars::Get()->strafe_hack_fps_helper->value )
			*( double* ) g_dwSpeed = ( g_pLocalPlayer()->m_iFPS / CCVars::Get()->strafe_hack_fps_helper->value ) * 1000;
		g_pLocalPlayer()->m_flVelocity ? pCmd->forwardmove = CCVars::Get()->strafe_hack_forwardmove->value / g_pLocalPlayer()->m_flVelocity : pCmd->forwardmove = CCVars::Get()->strafe_hack_forwardmove->value;
		pCmd->sidemove = strafing ? +CCVars::Get()->strafe_hack_sidemove->value : -CCVars::Get()->strafe_hack_sidemove->value;
	}

	if( FreeLookEnabled && g_pLocalPlayer()->m_bIsAlive && g_pLocalPlayer()->m_iTeam != 0 ) {
		Vector curMove( 0.0f, 0.0f, 0.0f );
		Vector vr = g_pLocalPlayer()->m_vecRight;
		Vector vf = g_pLocalPlayer()->m_vecForward;

		if( pCmd->buttons&IN_MOVELEFT )
			curMove = curMove + vr * -1.0f;

		if( pCmd->buttons&IN_MOVERIGHT )
			curMove = curMove + vr * 1.0f;

		if( pCmd->buttons&IN_FORWARD )
			curMove = curMove + vf * 1.0f;

		if( pCmd->buttons&IN_BACK )
			curMove = curMove + vf * -1.0f;

		curMove = curMove * ( CCVars::Get()->freelook_speed->value * flFrameTime );
		FreeLookOffset[ 0 ] += curMove.x;
		FreeLookOffset[ 1 ] += curMove.y;
		FreeLookOffset[ 2 ] += curMove.z;
		pCmd->buttons = 0;
		pCmd->sidemove = 0;
		pCmd->forwardmove = 0;
	}

	g_Engine.OverrideLightmap( CCVars::Get()->fullbright->value );

	// sync based strafe helper with control feature
	// idk what i tried to do here, but i tried.
	/*if( CCVars::Get()->strafe_control_helper->value == 3 && !( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) && ( g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY ) && !( Instruments::Get()->bSurf() ) ) {
		float tmpva[ 3 ] = {};
		g_Engine.GetViewAngles( tmpva );

		float min = CCVars::Get()->strafe_control_helper_sync_min->value, max = CCVars::Get()->strafe_control_helper_sync_max->value, sync;
		bool isstrafing, previsstrafing, shouldgetnewsync = false;

		if( g_pLocalPlayer()->m_iMX > 0 ) {
			right = 1;
			isstrafing = true;
			ticks++;
		} else if( g_pLocalPlayer()->m_iMX < 0 ) {
			right = 0;
			isstrafing = true;
			ticks++;
		} else {
			right = -1;
			isstrafing = false;
			ticks = 0;
		}

		if( prevright != right ) {
			tickstostr = 0;
			tickspassed = 0;
			ticks = 0;
			shouldgetnewsync = true;
		}

		//if( !previsstrafing && isstrafing )
		//	shouldgetnewsync = true;

		if( shouldgetnewsync ) {
			sync = g_pEngine->pfnRandomFloat( min, max );
			tickstostr = ceil( ( CCVars::Get()->strafe_control_helper_sync_ticks->value / 100 ) * sync );
			shouldgetnewsync = false;
		}

		switch( ( int ) CCVars::Get()->strafe_control_helper_dir->value ) {
			case 1:

				if( right == 1 ) {
					if( --tickstostr >= 0 ) {
						tickspassed++;
						pCmd->buttons |= IN_MOVERIGHT;
					} else {
						pCmd->sidemove = 250;
					}

					tmpva[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
				} else if( right == 0 ) {
					if( --tickstostr >= 0 ) {
						tickspassed++;
						pCmd->buttons |= IN_MOVELEFT;
					} else {
						pCmd->sidemove = -250;
					}

					tmpva[ 1 ] += CCVars::Get()->strafe_control_speed->value;
				}

				break;
		}

		if( ticks > tickstostr + tickspassed ) {
			tickstostr = ceil( ( ( ticks / 100 ) * sync ) - tickspassed );
		}

		prevright = right;
		previsstrafing = isstrafing;

		g_pEngine->SetViewAngles( tmpva );
	}*/
	// -----------------------------------------

	// xHancer style strafe helper with control feature
	if( CCVars::Get()->strafe_control_helper->value == 2 && !( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) && ( g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY ) && !( Instruments::Get()->bSurf() ) ) {
		Vector tmpva;
		g_Engine.GetViewAngles( tmpva );
		
		switch( ( int ) CCVars::Get()->strafe_control_helper_dir->value ) {
			case 1:
				if( CCVars::Get()->strafe_control_helper_forwardmove_random->value )
					pCmd->forwardmove = ( g_pEngine->pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value ) / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
				else {
					if( CCVars::Get()->strafe_control_helper_forwardmove->value )
						pCmd->forwardmove = ( CCVars::Get()->strafe_control_helper_forwardmove->value / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
					else
						pCmd->forwardmove = 0.f;
				}

				if( g_pLocalPlayer()->m_iMX > 0 ) {
					pCmd->sidemove += CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
				} else if( g_pLocalPlayer()->m_iMX < 0 ) {
					pCmd->sidemove -= CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] += CCVars::Get()->strafe_control_speed->value;
				}

				break;

			case 2:
				if( CCVars::Get()->strafe_control_helper_forwardmove_random->value )
					pCmd->forwardmove = -( g_pEngine->pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value ) / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
				else {
					if( CCVars::Get()->strafe_control_helper_forwardmove->value )
						pCmd->forwardmove = -( CCVars::Get()->strafe_control_helper_forwardmove->value / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
					else
						pCmd->forwardmove = 0.f;
				}
					
				if( g_pLocalPlayer()->m_iMX > 0 ) {
					pCmd->sidemove -= CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
				} else if( g_pLocalPlayer()->m_iMX < 0 ) {
					pCmd->sidemove += CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] += CCVars::Get()->strafe_control_speed->value;
				}

				break;

			case 3:
				if( CCVars::Get()->strafe_control_helper_forwardmove_random->value )
					pCmd->sidemove = ( g_pEngine->pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value ) / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
				else {
					if( CCVars::Get()->strafe_control_helper_forwardmove->value )
						pCmd->sidemove = ( CCVars::Get()->strafe_control_helper_forwardmove->value / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
					else
						pCmd->sidemove = 0.f;
				}
					
				if( g_pLocalPlayer()->m_iMX > 0 ) {
					pCmd->forwardmove -= CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
				} else if( g_pLocalPlayer()->m_iMX < 0 ) {
					pCmd->forwardmove += CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] += CCVars::Get()->strafe_control_speed->value;
				}

				break;

			case 4:
				if( CCVars::Get()->strafe_control_helper_forwardmove_random->value )
					pCmd->sidemove = -( g_pEngine->pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value ) / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
				else {
					if( CCVars::Get()->strafe_control_helper_forwardmove->value )
						pCmd->sidemove = ( CCVars::Get()->strafe_control_helper_forwardmove->value / ( g_pLocalPlayer()->m_flMaxSpeed / ( g_pLocalPlayer()->m_flVelocity ? g_pLocalPlayer()->m_flVelocity : 1.f ) ) );
					else
						pCmd->sidemove = 0.f;
				}

				if( g_pLocalPlayer()->m_iMX > 0 ) {
					pCmd->forwardmove += CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
				} else if( g_pLocalPlayer()->m_iMX < 0 ) {
					pCmd->forwardmove -= CCVars::Get()->strafe_control_helper_sidemove->value;
					tmpva[ 1 ] += CCVars::Get()->strafe_control_speed->value;
				}

				break;
		}

		g_pEngine->SetViewAngles( tmpva );
	}
	// -----------------------------------------

	//Strafe control-------------------------------
	if( CCVars::Get()->strafe_control_helper->value == 1 && !( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) && ( g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY ) && !( Instruments::Get()->bSurf() ) ) {
		static bool bstrs;
		Vector tmp_va;
		g_Engine.GetViewAngles( tmp_va );

		switch( ( int ) CCVars::Get()->strafe_control_helper_dir->value ) {
		case 1:		// -------------------------------------------------------------------------------------FW
		{
			if( g_pLocalPlayer()->m_iMX < CCVars::Get()->strafe_control_helper_althreshold->value && mxl > CCVars::Get()->strafe_control_helper_adelay->value ) // MOUSE MOVES LEFT <------------------------- mx = -mx
			{
				mxr = 0;
				if( CCVars::Get()->strafe_control_helper_sidemove_random->value )
					pCmd->sidemove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_sidemove_random->value == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )
					pCmd->sidemove = -CCVars::Get()->strafe_control_helper_sidemove->value;

				if( CCVars::Get()->strafe_control_helper_forwardmove_random->value )
					pCmd->forwardmove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_forwardmove_random->value == 0 /*&& CCVars::Get()->strafe_control_helper_forwardmove->value*/ )
					pCmd->forwardmove = +CCVars::Get()->strafe_control_helper_forwardmove->value;

				if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
					if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
						pCmd->buttons &= ~IN_MOVERIGHT;
						pCmd->buttons |= IN_MOVELEFT;
						if( pCmd->buttons&IN_MOVELEFT && bstrs ) {
							++istrs;
							bstrs = false;
						}
					}
				}

				if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
					tmp_va[ 1 ] += CCVars::Get()->strafe_control_speed->value;

			} else if( g_pLocalPlayer()->m_iMX > CCVars::Get()->strafe_control_helper_arthreshold->value && mxr > CCVars::Get()->strafe_control_helper_adelay->value ) { // MOUSE MOVES RIGHT <------------------------ mx = +mx
				mxl = 0;
				if( CCVars::Get()->strafe_control_helper_sidemove_random->value ) // Random == 1
					pCmd->sidemove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_sidemove_random->value == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )
					pCmd->sidemove = +CCVars::Get()->strafe_control_helper_sidemove->value;

				if( CCVars::Get()->strafe_control_helper_forwardmove_random->value ) // Random == 1
					pCmd->forwardmove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_forwardmove_random->value == 0 /*&& CCVars::Get()->strafe_control_helper_forwardmove->value*/ )
					pCmd->forwardmove = +CCVars::Get()->strafe_control_helper_forwardmove->value;

				if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
					if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
						pCmd->buttons &= ~IN_MOVELEFT;
						pCmd->buttons |= IN_MOVERIGHT;
						if( pCmd->buttons&IN_MOVERIGHT && !bstrs ) {
							++istrs;
							bstrs = true;
						}
					}
				}

				if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
					tmp_va[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
			}
			break;
		}
		case 2:			// -------------------------------------------------------------------------------------BW
		{
			if( g_pLocalPlayer()->m_iMX < CCVars::Get()->strafe_control_helper_althreshold->value && mxl > CCVars::Get()->strafe_control_helper_adelay->value ) { // MOUSE MOVES LEFT <-------------------------
				mxr = 0;
				if( CCVars::Get()->strafe_control_helper_sidemove_random->value )//Random == 1
					pCmd->sidemove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_sidemove_random->value == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )//Random == 0
					pCmd->sidemove = +CCVars::Get()->strafe_control_helper_sidemove->value;

				if( CCVars::Get()->strafe_control_helper_forwardmove_random->value )//Random == 1
					pCmd->forwardmove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_forwardmove_random->value == 0 && CCVars::Get()->strafe_control_helper_forwardmove->value )//Random == 0
					pCmd->forwardmove = -CCVars::Get()->strafe_control_helper_forwardmove->value;

				if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
					if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
						pCmd->buttons &= ~IN_MOVELEFT;
						pCmd->buttons |= IN_MOVERIGHT;
						if( pCmd->buttons&IN_MOVERIGHT && bstrs ) {
							++istrs;
							bstrs = false;
						}
					}
				}

				if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
					tmp_va[ 1 ] += CCVars::Get()->strafe_control_speed->value;

			} else if( g_pLocalPlayer()->m_iMX > CCVars::Get()->strafe_control_helper_arthreshold->value && mxr > CCVars::Get()->strafe_control_helper_adelay->value ) { // MOUSE MOVES RIGHT <------------------------
				mxl = 0;
				if( CCVars::Get()->strafe_control_helper_sidemove_random->value )
					pCmd->sidemove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_sidemove_random->value == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )
					pCmd->sidemove = -CCVars::Get()->strafe_control_helper_sidemove->value;

				if( CCVars::Get()->strafe_control_helper_sidemove_random->value )
					pCmd->forwardmove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );
				else if( CCVars::Get()->strafe_control_helper_forwardmove_random->value == 0 && CCVars::Get()->strafe_control_helper_forwardmove->value )
					pCmd->forwardmove = -CCVars::Get()->strafe_control_helper_forwardmove->value;

				if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
					if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
						pCmd->buttons &= ~IN_MOVERIGHT;
						pCmd->buttons |= IN_MOVELEFT;
						if( pCmd->buttons&IN_MOVELEFT && !bstrs ) {
							++istrs;
							bstrs = true;
						}
					}
				}

				if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
					tmp_va[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
			}
			break;
		}
		case 3:			// -------------------------------------------------------------------------------------SW RIGHT
		{
			if( g_pLocalPlayer()->m_iMX < CCVars::Get()->strafe_control_helper_althreshold->value && mxl > CCVars::Get()->strafe_control_helper_adelay->value ) { // MOUSE MOVES LEFT
				mxr = 0;
				if( CCVars::Get()->strafe_control_helper_sidemove_random )
					pCmd->forwardmove = +CCVars::Get()->strafe_control_helper_sidemove->value;
				else if( CCVars::Get()->strafe_control_helper_sidemove_random == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )
					pCmd->forwardmove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );

				if( CCVars::Get()->strafe_control_helper_forwardmove_random )
					pCmd->sidemove = +CCVars::Get()->strafe_control_helper_forwardmove->value;
				else if( CCVars::Get()->strafe_control_helper_forwardmove_random == 0 && CCVars::Get()->strafe_control_helper_forwardmove->value )
					pCmd->sidemove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );

				if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
					if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
						pCmd->buttons &= ~IN_BACK;
						pCmd->buttons |= IN_FORWARD;
						if( pCmd->buttons&IN_FORWARD && bstrs ) {
							++istrs;
							bstrs = false;
						}
					}
				}

				if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
					tmp_va[ 1 ] += CCVars::Get()->strafe_control_speed->value;

			} else if( g_pLocalPlayer()->m_iMX > CCVars::Get()->strafe_control_helper_arthreshold->value && mxr > CCVars::Get()->strafe_control_helper_adelay->value ) { // MOUSE MOVES RIGHT
				mxl = 0;
				if( CCVars::Get()->strafe_control_helper_sidemove_random )
					pCmd->forwardmove = -CCVars::Get()->strafe_control_helper_sidemove->value;
				else if( CCVars::Get()->strafe_control_helper_sidemove_random == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )
					pCmd->forwardmove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );

				if( CCVars::Get()->strafe_control_helper_forwardmove_random )
					pCmd->sidemove = +CCVars::Get()->strafe_control_helper_forwardmove->value;
				else if( CCVars::Get()->strafe_control_helper_forwardmove_random == 0 && CCVars::Get()->strafe_control_helper_forwardmove->value )
					pCmd->sidemove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );

				if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
					if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
						pCmd->buttons &= ~IN_FORWARD;
						pCmd->buttons |= IN_BACK;
						if( pCmd->buttons&IN_BACK && !bstrs ) {
							++istrs;
							bstrs = true;
						}
					}
				}

				if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
					tmp_va[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
			}
			break;
		}
		case 4:
			{
				if( g_pLocalPlayer()->m_iMX < CCVars::Get()->strafe_control_helper_althreshold->value && mxl > CCVars::Get()->strafe_control_helper_adelay->value ) { // MOUSE MOVES LEFT
					mxr = 0;
					if( CCVars::Get()->strafe_control_helper_sidemove_random )
						pCmd->forwardmove = -CCVars::Get()->strafe_control_helper_sidemove->value;
					else if( CCVars::Get()->strafe_control_helper_sidemove_random == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )
						pCmd->forwardmove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );

					if( CCVars::Get()->strafe_control_helper_forwardmove_random )
						pCmd->sidemove = -CCVars::Get()->strafe_control_helper_forwardmove->value;
					else if( CCVars::Get()->strafe_control_helper_forwardmove_random == 0 && CCVars::Get()->strafe_control_helper_forwardmove->value )
						pCmd->sidemove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );

					if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
						if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
							pCmd->buttons &= ~IN_FORWARD;
							pCmd->buttons |= IN_BACK;
							if( pCmd->buttons&IN_BACK && bstrs ) {
								++istrs;
								bstrs = false;
							}
						}
					}
					if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
						tmp_va[ 1 ] += CCVars::Get()->strafe_control_speed->value;
				} else if( g_pLocalPlayer()->m_iMX > CCVars::Get()->strafe_control_helper_arthreshold->value && mxr > CCVars::Get()->strafe_control_helper_adelay->value ) { // MOUSE MOVES RIGHT
					mxl = 0;
					if( CCVars::Get()->strafe_control_helper_sidemove_random )
						pCmd->forwardmove = +CCVars::Get()->strafe_control_helper_sidemove->value;
					else if( CCVars::Get()->strafe_control_helper_sidemove_random == 0 && CCVars::Get()->strafe_control_helper_sidemove->value )
						pCmd->forwardmove = +g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_sidemove_random_min->value, CCVars::Get()->strafe_control_helper_sidemove_random_max->value );

					if( CCVars::Get()->strafe_control_helper_forwardmove_random )
						pCmd->sidemove = -CCVars::Get()->strafe_control_helper_forwardmove->value;
					else if( CCVars::Get()->strafe_control_helper_forwardmove_random == 0 && CCVars::Get()->strafe_control_helper_forwardmove->value )
						pCmd->sidemove = -g_Engine.pfnRandomFloat( CCVars::Get()->strafe_control_helper_forwardmove_random_min->value, CCVars::Get()->strafe_control_helper_forwardmove_random_max->value );

					if( CCVars::Get()->strafe_control_helper_add_strafe->value ) {
						if( istrs < CCVars::Get()->strafe_control_helper_max_strafe->value ) {
							pCmd->buttons &= ~IN_BACK;
							pCmd->buttons |= IN_FORWARD;
							if( pCmd->buttons&IN_FORWARD && !bstrs ) {
								++istrs;
								bstrs = true;
							}
						}
					}
					if( !( CCVars::Get()->strafe_control_speed_const->value ) && CCVars::Get()->strafe_control_helper_type->value != 2 )
						tmp_va[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
				}
				break;
			}
		}
		g_Engine.SetViewAngles( tmp_va );
	} else if( g_pLocalPlayer()->m_iFlags & FL_ONGROUND )
		istrs = 0;

	if( !( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) && CCVars::Get()->strafe_control_helper->value ) {
		if( CCVars::Get()->strafe_control_speed_const->value && g_pLocalPlayer()->m_iMX > 0 && CCVars::Get()->strafe_control_helper_type->value != 2 ) {
			Vector tmp_va;
			g_Engine.GetViewAngles( tmp_va );
			tmp_va[ 1 ] -= CCVars::Get()->strafe_control_speed->value;
			g_Engine.SetViewAngles( tmp_va );
		} else if( CCVars::Get()->strafe_control_speed_const->value && g_pLocalPlayer()->m_iMX < 0 && CCVars::Get()->strafe_control_helper_type->value != 2 ) {
			Vector tmp_va;
			g_Engine.GetViewAngles( tmp_va );
			tmp_va[ 1 ] += CCVars::Get()->strafe_control_speed->value;
			g_Engine.SetViewAngles( tmp_va );
		}
	}
	//--------------------------------------------------------

	//BHOP ---------------------------------------------------
	// Bhop method 1
	if( CCVars::Get()->bhop->value == 1.f && g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY && g_pLocalPlayer()->m_iWaterLevel < 2 && g_pLocalPlayer()->flSwimTime == 0.f
		&& pCmd->buttons & IN_JUMP && GetAsyncKeyState( VK_SPACE ) ) {

		float playerheight = Instruments::Get()->PlayerHeight( 0 );

		if( Instruments::Get()->bSurf() || ( CCVars::Get()->bhop_wog->value && g_pLocalPlayer()->m_flVelocity > CCVars::Get()->bhop_wog->value ) )
			pCmd->buttons &= ~IN_JUMP;

		if( CCVars::Get()->bhop_standup->value && !GetAsyncKeyState( VK_CONTROL ) ) { // most simple fix you could ever encounter.. this standup shit doesn't even really work lol..
			// it is not encorporated perfectly..
			if( CCVars::Get()->bhop_standup_10aa->value ) {
				if( Instruments::Get()->flGroundDistMeasuredInFrames() <= CCVars::Get()->bhop_standup_dist->value && g_pLocalPlayer()->m_flFallSpeed > 0 ) // pls fix
					pCmd->buttons |= IN_DUCK;
				else
					pCmd->buttons &= ~IN_DUCK;
			} else {
				if( playerheight <= CCVars::Get()->bhop_standup_dist->value )
					pCmd->buttons &= ~IN_DUCK;
				else
					pCmd->buttons |= IN_DUCK;
			}
		}

		// otherwise it won't jump when you stand on ground, because of the on/off switch
		if( playerheight <= CCVars::Get()->bhop_start->value )
			pCmd->buttons |= IN_JUMP;

		if( playerheight <= CCVars::Get()->bhop_start->value ) {
			g_pLocalPlayer()->bDBhop ? pCmd->buttons |= IN_JUMP : pCmd->buttons &= ~IN_JUMP;
			// if( g_pEngine->pfnRandomLong( 0, 2 ) ) // to make it really random. not for democheckers, somehow they detect it either way.. something about sending through cmd?
			g_pLocalPlayer()->bDBhop = !g_pLocalPlayer()->bDBhop;
		}

		if( g_pLocalPlayer()->m_flFallSpeed > 0 ) {
			if( playerheight >= CCVars::Get()->bhop_start->value )
				pCmd->buttons &= ~IN_JUMP;
		} else if( g_pLocalPlayer()->m_flFallSpeed < 0 ) {
			if( playerheight >= CCVars::Get()->bhop_end->value )
				pCmd->buttons &= ~IN_JUMP;
		}
	}

	// Bhop method 2
	if( CCVars::Get()->bhop->value == 2.f && g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY && g_pLocalPlayer()->m_iWaterLevel < 2 && GetAsyncKeyState( VK_SPACE ) && CCVars::Get()->patterns.size() != 0 ) {
		if( shouldgetPattern ) {
			pattern = CCVars::Get()->patterns.at( g_pEngine->pfnRandomLong( 0, CCVars::Get()->patterns.size() - 1 ) );
            tick = 0;
			shouldgetPattern = false;
		}

		float playerheight = Instruments::Get()->PlayerHeight( 0 );

		if( Instruments::Get()->bSurf() || ( CCVars::Get()->bhop_wog->value && g_pLocalPlayer()->m_flVelocity > CCVars::Get()->bhop_wog->value ) )
			pCmd->buttons &= ~IN_JUMP;

		if( g_pLocalPlayer()->m_flFallSpeed >= 0.f ) {
			if( playerheight <= pattern.getStart() ) {
				if( tick < pattern.getscrollframes().size() && pattern.getscrollframes().at( tick ).didframescroll() ) {
					g_pEngine->pfnClientCmd( "+jump -1" );
					g_pEngine->pfnClientCmd( "-jump -1" );
				}
				tick++;
			}
		} else {
			if( playerheight <= pattern.getEnd() ) {
				if( tick < pattern.getscrollframes().size() && pattern.getscrollframes().at( tick ).didframescroll() ) {
					g_pEngine->pfnClientCmd( "+jump -1" );
					g_pEngine->pfnClientCmd( "-jump -1" );
				}
				tick++;
			}
		}

		if( tick >= pattern.getTickLength() )
			shouldgetPattern = true;
	} else if( CCVars::Get()->patterns.size() == 0 ) {
		g_pEngine->pfnConsolePrint( "\t\t\t" );
		g_pEngine->pfnConsolePrint( XString( /*<rN> Patterns failed to load! Please reload the patterns using the command #rN^pattern_reload!*/ 0x18, 0x5E, 0x11, 0x2D605D2A, 0x3546766C, 0x6D7F6972, 0x6E3E7941,
											 0x484E4640, 0x05524808, 0x45454A48, 0x0C0E7F5C, 0x54534051, 0x15445254, 0x565B5F1C, 0x49565A60, 0x31233730, 0x2034293B, 0x693F3825, 0x23296F24, 0x39377337, 0x3A3B3A39,
											 0x373E7B7F, 0x2F100110, 0x00161701, 0x1708381A, 0x0C06040D, 0x094F0000 ).c() );
		g_pEngine->pfnConsolePrint("\n\t\t\t");
		g_pEngine->pfnConsolePrint( XString( /*<rN> Bhop value has been set to 1!*/ 0x09, 0x22, 0xC3, 0xFFB68BF8, 0xE78AA1A5, 0xBBECBBAF, 0xA3A5B4F2, 0xBBB5A6F6, 0xB5BDBCB4, 0xFBAFB8AA, 0xFF948EC2, 0xD2C50000 ).c() );
		CCVars::Get()->bhop->value = 1;
	}

	// Bhop method 3
	if( CCVars::Get()->bhop->value == 3.f && g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY && g_pLocalPlayer()->m_iWaterLevel < 2 && GetAsyncKeyState( VK_SPACE ) ) {
		float playerheight = Instruments::Get()->PlayerHeight( 1 ), groundheightinframes = Instruments::Get()->flGroundDistMeasuredInFrames();

		if( Instruments::Get()->bSurf() || ( CCVars::Get()->bhop_wog->value && g_pLocalPlayer()->m_flVelocity > CCVars::Get()->bhop_wog->value ) )
			pCmd->buttons &= ~IN_JUMP;

		if( g_pLocalPlayer()->m_flFallSpeed >= 0 ) {
			if( playerheight <= CCVars::Get()->bhop_start->value ) {
				if( g_pLocalPlayer()->bDBhop ) {
					g_pEngine->pfnClientCmd( "+jump -1" );
					g_pEngine->pfnClientCmd( "-jump -1" );
				}

				if( g_pEngine->pfnRandomLong( 0, 3 ) && groundheightinframes > 1.f )
					g_pLocalPlayer()->bDBhop = !g_pLocalPlayer()->bDBhop;
				else if( groundheightinframes <= 1.f )
					g_pLocalPlayer()->bDBhop = !g_pLocalPlayer()->bDBhop;
			} else
				pCmd->buttons &= ~IN_JUMP;
		} else if( g_pLocalPlayer()->m_flFallSpeed <= 0 ) {
			if( playerheight <= CCVars::Get()->bhop_end->value ) {
				if( g_pLocalPlayer()->bDBhop ) {
					g_pEngine->pfnClientCmd( "+jump -1" );
					g_pEngine->pfnClientCmd( "-jump -1" );
				}

				if( g_pEngine->pfnRandomLong( 0, 3 ) && groundheightinframes > 1.f )
					g_pLocalPlayer()->bDBhop = !g_pLocalPlayer()->bDBhop;
				else if( groundheightinframes <= 1.f )
					g_pLocalPlayer()->bDBhop = !g_pLocalPlayer()->bDBhop;
			} else
				pCmd->buttons &= ~IN_JUMP;
		}
	}

	if( ( CCVars::Get()->bhop->value == 2.f || CCVars::Get()->bhop->value == 3.f ) && ( g_pLocalPlayer()->m_iWaterLevel >= 2 || g_pLocalPlayer()->flSwimTime > 0.f ) && GetAsyncKeyState( VK_SPACE ) ) {
		shouldkeepjumping = 8;
	}

	if( ( CCVars::Get()->bhop->value == 2.f || CCVars::Get()->bhop->value == 3.f ) && !GetAsyncKeyState( VK_CONTROL ) && CCVars::Get()->bhop_standup->value && g_pLocalPlayer()->m_iMoveType != MOVETYPE_FLY
		&& g_pLocalPlayer()->m_iWaterLevel < 2 && GetAsyncKeyState( VK_SPACE ) ) {
		// most simple fix you could ever encounter.. this standup shit doesn't even really work lol..
		// it is not encorporated perfectly..
		float playerheight = Instruments::Get()->PlayerHeight( 1 );

		if( CCVars::Get()->bhop_standup_10aa->value ) {
			if( Instruments::Get()->flGroundDistMeasuredInFrames() <= CCVars::Get()->bhop_standup_dist->value && g_pLocalPlayer()->m_flFallSpeed > 0 ) {
				g_pEngine->pfnClientCmd( "+duck -1" );
				g_pEngine->pfnClientCmd( "-duck -1" );
			}
		} else {
			if( playerheight >= CCVars::Get()->bhop_standup_dist->value ) {
				g_pEngine->pfnClientCmd( "+duck -1" );
				g_pEngine->pfnClientCmd( "-duck -1" );
			}
		}
	}

	if( ( CCVars::Get()->bhop->value == 2.f || CCVars::Get()->bhop->value == 3.f ) && g_pLocalPlayer()->m_iMoveType == MOVETYPE_FLY && GetAsyncKeyState( VK_SPACE ) ) {
		shouldkeepjumping = 8;
	}

	if( --shouldkeepjumping > 0 ) {
		g_pEngine->pfnClientCmd( "+jump -1" );
		g_pEngine->pfnClientCmd( "-jump -1" );
	}
	//--------------------------------------------------------

	//GS + SGS------------------------------------------------ don't ask me what i did here, im not sure either, cause most of this shit doesn't fucking make sense lol..
	if( bDGstrafe && CCVars::Get()->gs->value && g_pLocalPlayer()->m_iWaterLevel < 2 ) {
		float randomflfallspeed = g_pEngine->pfnRandomFloat( CCVars::Get()->gs_scroll_fallspeed->value, CCVars::Get()->gs_scroll_fallspeed->value + 40 );

		if( CCVars::Get()->gs_disable_jump->value ) {
			if( pCmd->buttons & IN_JUMP && GetAsyncKeyState( VK_SPACE ) ) {
				pCmd->buttons &= ~IN_JUMP;
			}
		}

		float playerheight = Instruments::Get()->PlayerHeight( 1 );

		if( CCVars::Get()->gs_nsd->value ) { // wtf, not perfect, how the fuck?? please help.. do i have to set the netspeed? :(
			if( playerheight <= 0.001f ) {
				*( double* ) g_dwSpeed = 1.0;
			}
		}

		static bool DG;

		if( CCVars::Get()->gs_scroll->value ) {
			if( DG ) {
				if( pCmd->buttons & IN_DUCK && Instruments::Get()->generaterandomintzeroandone( 2 ) ) {
					if( tickssincelastgs >= 10 ) {
						if( playerheight <= CCVars::Get()->gs_scroll_dist_end->value ) {
							tickssincelastgs = 0;
							pCmd->buttons &= ~IN_DUCK;
						}
					} else if( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) {
						tickssincelastgs = 0;
						pCmd->buttons &= ~IN_DUCK;
					}
				} else if( ~pCmd->buttons & IN_DUCK ) {
					if( g_pLocalPlayer()->m_flFallSpeed > randomflfallspeed ) {
						if( playerheight >= CCVars::Get()->gs_scroll_dist_end->value ) {

							if( Instruments::Get()->generaterandomintzeroandone( 1 ) )
								pCmd->buttons &= ~IN_DUCK;

						} else
							pCmd->buttons |= IN_DUCK;

					} else if( g_pLocalPlayer()->m_flFallSpeed < randomflfallspeed && !( g_pLocalPlayer()->m_flFallSpeed < 0 ) && Instruments::Get()->generaterandomintzeroandone( 1 ) )
						pCmd->buttons |= IN_DUCK;

					if( playerheight <= CCVars::Get()->gs_scroll_dist_end->value ) {
						pCmd->buttons &= ~IN_DUCK;

						if( Instruments::Get()->generaterandomintzeroandone( 1 ) )
							pCmd->buttons |= IN_DUCK;
					}
				}
			}
		} else {
			if( DG ) {
				if( g_pLocalPlayer()->m_iFlags & FL_ONGROUND ) {
					if( ( pCmd->buttons & IN_DUCK ) )
						pCmd->buttons &= ~IN_DUCK;
					else if( Instruments::Get()->generaterandomintzeroandone( 1 ) )
						pCmd->buttons |= IN_DUCK;
				}
			}
		}
		DG = !DG;
	}

	tickssincelastgs += 1;
	//----------------------------------------------------
	
	//CJumpStats::Get()->CL_CreateMove( pCmd );

	g_pLocalPlayer()->m_iFPS = 1 / flFrameTime;

	// Rayish fix.
	// Remember to use 250 sidemove only (no random on rayish, you can use random between 0 - 250 on dofrag), no forwardmove (unless sideways ofcourse).
	// idiots gonna get banned either way, lol idc

	if( pCmd->sidemove > 0.f && CCVars::Get()->strafe_control_helper_rayish_fix->value
		&& ( CCVars::Get()->strafe_control_helper_dir->value == 1 || CCVars::Get()->strafe_control_helper_dir->value == 2 ) )

		pCmd->buttons |= IN_MOVERIGHT;

	else if( pCmd->sidemove < 0.f && CCVars::Get()->strafe_control_helper_rayish_fix->value
		&& ( CCVars::Get()->strafe_control_helper_dir->value == 1 || CCVars::Get()->strafe_control_helper_dir->value == 2 ) )

		pCmd->buttons |= IN_MOVELEFT;

	if( pCmd->forwardmove > 0.f && CCVars::Get()->strafe_control_helper_rayish_fix->value
		&& ( CCVars::Get()->strafe_control_helper_dir->value == 3 || CCVars::Get()->strafe_control_helper_dir->value == 4 ) )

		pCmd->buttons |= IN_FORWARD;

	else if( pCmd->sidemove < 0.f && CCVars::Get()->strafe_control_helper_rayish_fix->value
		&& ( CCVars::Get()->strafe_control_helper_dir->value == 3 || CCVars::Get()->strafe_control_helper_dir->value == 4 ) )

		pCmd->buttons |= IN_BACK;

	// -------------------------------
}

void CClient::HookClient() {
	memcpy( &g_Client, ( LPVOID ) g_pClient, sizeof( cl_clientfuncs_s ) );
	g_pClient->HUD_Frame = &this->HUD_Frame;
	g_pClient->HUD_Redraw = &this->HUD_Redraw;
	g_pClient->CL_CreateMove = &this->CL_CreateMove;
	g_pClient->HUD_PlayerMove = &this->HUD_PlayerMove;
	g_pClient->V_CalcRefdef = &this->V_CalcRefdef;
	g_pClient->CL_IsThirdPerson = &this->CL_IsThirdPerson;
	g_pClient->HUD_AddEntity = &this->HUD_AddEntity;
}
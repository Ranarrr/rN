#ifndef CDLL_INT_HPP
#define CDLL_INT_HPP

#pragma once

#include "const.hpp"
#include "cvardef.hpp"
#include "cl_entity.hpp"
#include "demo_api.hpp"
#include "ivoicetweak.hpp"
#include "screenfade.hpp"
#include "con_nprint.hpp"
#include "event_args.hpp"
#include "rect.hpp"
#include "cl_dll.hpp"
#include "com_model.hpp"

// client dll interface declarations

// this file is included by both the engine and the client-dll,
// so make sure engine declarations aren't done twice

#define SCRINFO_SCREENFLASH 1
#define SCRINFO_STRETCHED	2

#define CLDLL_INTERFACE_VERSION	7

typedef int( *pfnUserMsgHook )( const char *pszName, int iSize, void *pbuf );

struct pmtrace_s;
struct triangleapi_s;
struct efx_api_s;
struct event_api_s;
struct net_api_s;
struct cmd_s;

struct SCREENINFO_s {
	int		m_iSize;
	int		m_iWidth;
	int		m_iHeight;
	int		m_iFlags;
	int		m_iCharHeight;
	short	m_charWidths[ 256 ];
};

struct client_data_s {
	// fields that cannot be modified  (ie. have no effect if changed)
	Vector	origin;

	// fields that can be changed by the cldll
	Vector	viewangles;
	int		iWeaponBits;
	float	fov;	// field of view
};

struct client_sprite_s {
	char	szName[ 64 ];
	char	szSprite[ 64 ];
	int		hspr;
	int		iRes;
	rect_s	rc;
};

struct client_textmessage_s {
	int				effect;
	unsigned char	r1, g1, b1, a1;		// 2 colors for effects
	unsigned char	r2, g2, b2, a2;
	float			x;
	float			y;
	float			fadein;
	float			fadeout;
	float			holdtime;
	float			fxtime;
	const char		*pName;
	const char		*pMessage;
};

struct hud_player_info_s {
	char			*name;
	short			ping;
	unsigned char	thisplayer;  // TRUE if this is the calling player

  // stuff that's unused at the moment,  but should be done
	unsigned char	spectator;
	unsigned char	packetloss;

	char			*model;
	short			topcolor;
	short			bottomcolor;
};

struct cl_enginefuncs_s {
	// sprite handlers
	int						( *pfnSPR_Load )			( const char *szPicName );
	int						( *pfnSPR_Frames )			( int hPic );
	int						( *pfnSPR_Height )			( int hPic, int frame );
	int						( *pfnSPR_Width )			( int hPic, int frame );
	void					( *pfnSPR_Set )				( int hPic, int r, int g, int b );
	void					( *pfnSPR_Draw )			( int frame, int x, int y, const rect_s *prc );
	void					( *pfnSPR_DrawHoles )		( int frame, int x, int y, const rect_s *prc );
	void					( *pfnSPR_DrawAdditive )	( int frame, int x, int y, const rect_s *prc );
	void					( *pfnSPR_EnableScissor )	( int x, int y, int width, int height );
	void					( *pfnSPR_DisableScissor )	();
	client_sprite_s			*( *pfnSPR_GetList )		( char *psz, int *piCount );

	// screen handlers
	void					( *pfnFillRGBA )			( int x, int y, int width, int height, int r, int g, int b, int a );
	int						( *pfnGetScreenInfo ) 		( SCREENINFO_s *pscrinfo );
	void					( *pfnSetCrosshair )		( int hspr, rect_s rc, int r, int g, int b );

	// cvar handlers
	cvar_s					*( *pfnRegisterVariable )	( char *szName, char *szValue, int flags );
	float					( *pfnGetCvarFloat )		( char *szName );
	char					*( *pfnGetCvarString )		( char *szName );

	// command handlers
	int						( *pfnAddCommand )			( char *cmd_name, void( *function )() );
	int						( *pfnHookUserMsg )			( char *szMsgName, pfnUserMsgHook pfn );
	int						( *pfnServerCmd )			( char *szCmdString );
	int						( *pfnClientCmd )			( char *szCmdString );

	void					( *pfnGetPlayerInfo )		( int ent_num, hud_player_info_s *pinfo );

	// sound handlers
	void					( *pfnPlaySoundByName )		( char *szSound, float volume );
	void					( *pfnPlaySoundByIndex )	( int iSound, float volume );

	// vector helpers
	void					( *pfnAngleVectors )		( const float *vecAngles, float *forward, float *right, float *up );

	// text message system
	client_textmessage_s	*( *pfnTextMessageGet )		( const char *pName );
	int						( *pfnDrawCharacter )		( int x, int y, int number, int r, int g, int b );
	int						( *pfnDrawConsoleString )	( int x, int y, char *string );
	void					( *pfnDrawSetTextColor )	( float r, float g, float b );
	void					( *pfnDrawConsoleStringLen )( const char *string, int *length, int *height );

	void					( *pfnConsolePrint )		( const char *string );
	void					( *pfnCenterPrint )			( const char *string );

// Added for user input processing
	int						( *GetWindowCenterX )		();
	int						( *GetWindowCenterY )		();
	void					( *GetViewAngles )			( float *vecAngles );
	void					( *SetViewAngles )			( float *vecAngles );
	int						( *GetMaxClients )			();
	void					( *Cvar_SetValue )			( char *cvar, float value );

	int						( *Cmd_Argc )				();
	char					*( *Cmd_Argv )				( int arg );
	void					( *Con_Printf )				( char *fmt, ... );
	void					( *Con_DPrintf )			( char *fmt, ... );
	void					( *Con_NPrintf )			( int pos, char *fmt, ... );
	void					( *Con_NXPrintf )			( con_nprint_s *info, char *fmt, ... );

	const char				*( *PhysInfo_ValueForKey )	( const char *key );
	const char				*( *ServerInfo_ValueForKey )( const char *key );
	float					( *GetClientMaxspeed )		();
	int						( *CheckParm )				( char *parm, char **ppnext );
	void					( *Key_Event )				( int key, int down );
	void					( *GetMousePosition )		( int *mx, int *my );
	int						( *IsNoClipping )			();

	cl_entity_s				*( *GetLocalPlayer )		();
	cl_entity_s				*( *GetViewModel )			();
	cl_entity_s				*( *GetEntityByIndex )		( int idx );

	float					( *GetClientTime )			();
	void					( *V_CalcShake )			();
	void					( *V_ApplyShake )			( float *origin, float *angles, float factor );

	int						( *PM_PointContents )		( float *point, int *truecontents );
	int						( *PM_WaterEntity )			( float *p );
	pmtrace_s				*( *PM_TraceLine )			( float *start, float *end, int flags, int usehull, int ignore_pe );

	model_s					*( *CL_LoadModel )			( const char *modelname, int *index );
	int						( *CL_CreateVisibleEntity )	( int type, cl_entity_s *ent );

	const model_s 			*( *GetSpritePointer )		( int hSprite );
	void					( *pfnPSByNameAtLocation )	( char *szSound, float volume, float *origin ); // PlaySoundByNameAtLocation

	unsigned short			( *pfnPrecacheEvent )		( int type, const char* psz );
	void					( *pfnPlaybackEvent )		( int flags, const edict_s *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	void					( *pfnWeaponAnim )			( int iAnim, int body );
	float					( *pfnRandomFloat )			( float flLow, float flHigh );
	long					( *pfnRandomLong )			( long lLow, long lHigh );
	void					( *pfnHookEvent )			( char *name, void( *pfnEvent )( event_args_s *args ) );
	int						( *Con_IsVisible )			();
	const char				*( *pfnGetGameDirectory )	();
	cvar_s					*( *pfnGetCvarPointer )		( const char *szName );
	const char				*( *Key_LookupBinding )		( const char *pBinding );
	const char				*( *pfnGetLevelName )		();
	void					( *pfnGetScreenFade )		( screenfade_s *fade );
	void					( *pfnSetScreenFade )		( screenfade_s *fade );
	void                    *( *VGui_GetPanel )         ();
	void					( *VGui_VportPaintBg )		( int extents[ 4 ] ); // VGui_ViewportPaintBackground

	unsigned char			*( *COM_LoadFile )			( char *path, int usehunk, int *pLength );
	char					*( *COM_ParseFile )			( char *data, char *token );
	void					( *COM_FreeFile )			( void *buffer );

	triangleapi_s			*pTriAPI;
	efx_api_s				*pEfxAPI;
	event_api_s				*pEventAPI;
	demo_api_s				*pDemoAPI;
	net_api_s				*pNetAPI;
	IVoiceTweak_s			*pVoiceTweak;

	// returns 1 if the client is a spectator only (connected to a proxy), 0 otherwise or 2 if in dev_overview mode
	int						( *IsSpectateOnly )			();
	model_s					*( *LoadMapSprite )			( const char *filename );

	// file search functions
	void					( *COM_AddAppDirectoryToSearchPath ) ( const char *pszBaseDir, const char *appName );
	int						( *COM_ExpandFilename )				 ( const char *fileName, char *nameOutBuffer, int nameOutBufferSize );

	// User info
	// playerNum is in the range (1, MaxClients)
	// returns NULL if player doesn't exit
	// returns "" if no value is set
	const char				*( *PlayerInfo_ValueForKey )	( int playerNum, const char *key );
	void					( *PlayerInfo_SetValueForKey )	( const char *key, const char *value );

	// Gets a unique ID for the specified player. This is the same even if you see the player on a different server.
	// iPlayer is an entity index, so client 0 would use iPlayer=1.
	// Returns false if there is no player on the server in the specified slot.
	int						( *GetPlayerUniqueID )		( int iPlayer, char playerID[ 16 ] );

	// TrackerID access
	int						( *GetTrackerIDForPlayer )	( int playerSlot );
	int						( *GetPlayerForTrackerID )	( int trackerID );

	// Same as pfnServerCmd, but the message goes in the unreliable stream so it can't clog the net stream
	// (but it might not get there).
	int						( *pfnServerCmdUnreliable )	( char *szCmdString );

	void					( *pfnGetMousePos )			( void *ppt );
	void					( *pfnSetMousePos )			( int x, int y );
	void					( *pfnSetMouseEnable )		( int fEnable );
	cvar_s					*( *pfnGetCvarList )		();
	cmd_s					*( *pfnGetCmdList )			();

	char					*( *pfnGetCvarName )		( cvar_s *cvar );
	char					*( *pfnGetCmdName )			( cmd_s *cmd );

	float					( *pfnGetServerTime )		();
	float					( *pfnGetGravity )			();
	const model_s			*( *pfnPrecacheSprite )		( int spr );
	void					( *OverrideLightmap )		( int override );
	void					( *SetLightmapColor )		( float r, float g, float b );
	void					( *SetLightmapDarkness )	( float dark );

	//this will always fail with the current engine
	int						( *pfnGetSequenceByName )	( int flags, const char* seq );

	void					( *pfnSPR_DrawGeneric )		( int frame, int x, int y, const rect_s *prc, int blendsrc, int blenddst, int unknown1, int unknown2 );

	//this will always fail with engine, don't call
	//it actually has paramenters but i dunno what they do
	void					( *pfnLoadSentence )		();

	//localizes hud string, uses Legacy font from skin def
	// also supports unicode strings
	int						( *pfnDrawLocalizedHudString )( int x, int y, const char* str, int r, int g, int b );

	//i can't get this to work for some reason, don't use this
	int						( *pfnDrawLocalizedConsoleString )( int x, int y, const char* str );

	//gets keyvalue for local player, useful for querying vgui menus or autohelp
	const char				*( *LocalPlayerInfo_ValueForKey )( const char* key );

	//another vgui2 text drawing function, i dunno how it works
	//it doesn't localize though
	void					( *pfnDrawText_0 )			( int x, int y, const char* text, unsigned long font );

	int						( *pfnDrawUnicodeCharacter )( int x, int y, short number, int r, int g, int b, unsigned long hfont );

	//checks sound header of a sound file, determines if its a supported type
	int						( *pfnCheckSoundFile )		( const char *path );

	//for condition zero, returns interface from GameUI
	void					*( *GetCareerGameInterface )();

	void					( *pfnCvar_Set )			( const char *cvar, const char *value );

	//this actually checks for if the CareerGameInterface is found
	//and if a server is being run
	int						( *IsSinglePlayer )			();

	void					( *pfnPlaySound )			( const char *sound, float vol, float pitch );

	void					( *pfnPlayMp3 )				( const char *mp3, int flags );

	//get the systems current time as a float
	float					( *Sys_FloatTime )			();

	void					( *pfnSetArray )			( int *arr, int size );
	void					( *pfnSetClearArray )		( int *arr, int size );
	void					( *pfnClearArray )			();

	void					( *pfnPlaySound2 )			( const char *sound, float vol, float pitch );

	void					( *pfnTintRGBA )			( int x, int y, int width, int height, int r, int g, int b, int a );
};

#endif // !CDLL_INT_HPP
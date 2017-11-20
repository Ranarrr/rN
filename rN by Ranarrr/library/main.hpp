#ifndef _MAIN_H_
#define _MAIN_H_

#define WIN32_LEAN_AND_MEAN

#define M_PI_F 3.14159265358979323846f
#define RAD2DEG( x ) x * ( 180 / M_PI_F )
#define DEG2RAD( x ) x * ( M_PI_F / 180.f )

#define VectorTransform(a,b,c){(c)[0]=DotProduct((a),(b)[0])+(b)[0][3];(c)[1]=DotProduct((a),(b)[1])+(b)[1][3];(c)[2]=DotProduct((a),(b)[2])+(b)[2][3];}

#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <list>
#include <map>

#include "ValveSDK/engine/wrect.h"
#include "ValveSDK/engine/cl_dll.h"
#include "ValveSDK/engine/r_studioint.h"
#include "ValveSDK/engine/cl_entity.h"
#include "ValveSDK/misc/com_model.h"
#include "ValveSDK/engine/triangleapi.h"
#include "ValveSDK/engine/pmtrace.h"
#include "ValveSDK/engine/pm_defs.h"
#include "ValveSDK/engine/pm_info.h"
#include "ValveSDK/common/ref_params.h"
#include "ValveSDK/common/studio_event.h"
#include "ValveSDK/common/net_api.h"
#include "ValveSDK/common/r_efx.h"
#include "ValveSDK/engine/cvardef.h"
#include "ValveSDK/engine/util_vector.h"
#include "ValveSDK/misc/parsemsg.h"
#include "ValveSDK/engine/studio.h"
#include "ValveSDK/engine/event_args.h"
#include "ValveSDK/engine/event_flags.h"
#include "ValveSDK/common/event_api.h"
#include "ValveSDK/common/screenfade.h"
#include "ValveSDK/common/entity_types.h"
#include "ValveSDK/engine/keydefs.h"
#include "ValveSDK/common/engine_launcher_api.h"

#include "color.hpp"
#include "entity.hpp"
#include "hiding.hpp"
#include "offsets.hpp"
#include "client.hpp"
#include "drawing.hpp"
#include "hook.hpp"
#include "cvars.hpp"
#include "jumpstats.hpp"
#include "Command.hpp"
#include "Instruments.hpp"
#include "NoFlash.hpp"
#include "esp.hpp"
#include "xstring.hpp"
#include "cBuddy.hpp"

using namespace std;

typedef struct cl_clientfuncs_s {
	int( *Initialize ) ( cl_enginefunc_t *pEnginefuncs, int iVersion );
	int( *HUD_Init ) ( void );
	int( *HUD_VidInit ) ( void );
	void( *HUD_Redraw ) ( float time, int intermission );
	int( *HUD_UpdateClientData ) ( client_data_t *pcldata, float flTime );
	int( *HUD_Reset ) ( void );
	void( *HUD_PlayerMove ) ( struct playermove_s *ppmove, int server );
	void( *HUD_PlayerMoveInit ) ( struct playermove_s *ppmove );
	char( *HUD_PlayerMoveTexture ) ( char *name );
	void( *IN_ActivateMouse ) ( void );
	void( *IN_DeactivateMouse ) ( void );
	void( *IN_MouseEvent ) ( int mstate );
	void( *IN_ClearStates ) ( void );
	void( *IN_Accumulate ) ( void );
	void( *CL_CreateMove ) ( float frametime, struct usercmd_s *cmd, int active );
	int( *CL_IsThirdPerson ) ( void );
	void( *CL_CameraOffset ) ( float *ofs );
	struct kbutton_s *( *KB_Find ) ( const char *name );
	void( *CAM_Think ) ( void );
	void( *V_CalcRefdef ) ( struct ref_params_s *pparams );
	int( *HUD_AddEntity ) ( int type, struct cl_entity_s *ent, const char *modelname );
	void( *HUD_CreateEntities ) ( void );
	void( *HUD_DrawNormalTriangles ) ( void );
	void( *HUD_DrawTransparentTriangles ) ( void );
	void( *HUD_StudioEvent ) ( const struct mstudioevent_s *event, const struct cl_entity_s *entity );
	void( *HUD_PostRunCmd ) ( struct local_state_s *from, struct local_state_s *to, struct usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
	void( *HUD_Shutdown ) ( void );
	void( *HUD_TxferLocalOverrides ) ( struct entity_state_s *state, const struct clientdata_s *client );
	void( *HUD_ProcessPlayerState ) ( struct entity_state_s *dst, const struct entity_state_s *src );
	void( *HUD_TxferPredictionData ) ( struct entity_state_s *ps, const struct entity_state_s *pps, struct clientdata_s *pcd, const struct clientdata_s *ppcd, struct weapon_data_s *wd, const struct weapon_data_s *pwd );
	void( *Demo_ReadBuffer ) ( int size, unsigned char *buffer );
	int( *HUD_ConnectionlessPacket ) ( struct netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
	int( *HUD_GetHullBounds ) ( int hullnumber, float *mins, float *maxs );
	void( *HUD_Frame ) ( double time );
	int( *HUD_Key_Event ) ( int down, int keynum, const char *pszCurrentBinding );
	void( *HUD_TempEntUpdate ) ( double frametime, double client_time, double cl_gravity, struct tempent_s **ppTempEntFree, struct tempent_s **ppTempEntActive, int( *Callback_AddVisibleEntity )( struct cl_entity_s *pEntity ), void( *Callback_TempEntPlaySound )( struct tempent_s *pTemp, float damp ) );
	struct cl_entity_s *( *HUD_GetUserEntity ) ( int index );
	int( *HUD_VoiceStatus ) ( int entindex, qboolean bTalking );
	int( *HUD_DirectorMessage ) ( unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags );
	int( *HUD_GetStudioModelInterface ) ( int version, struct r_studio_interface_s **ppinterface, struct engine_studio_api_s *pstudio );
	void( *HUD_CHATINPUTPOSITION_FUNCTION ) ( int *x, int *y );
	int( *HUD_GETPLAYERTEAM_FUNCTION ) ( int iplayer );
	void( *CLIENTFACTORY ) ( void );
} cl_clientfunc_t;

#define M_PI 3.14159265358979323846
#define DEG2RAD(DEG) ((DEG)*((M_PI)/(180.0)))
#define VectorScale(a,b) {(a)[0]*=b;(a)[1]*=b;(a)[2]*=b;}
#define VectorDistance(a,b) sqrt(POW((a)[0]-(b)[0])+POW((a)[1]-(b)[1])+POW((a)[2]-(b)[2]))
#define VectorMul(vec,num,res){(res)[0]=(vec)[0]*(num);(res)[1]=(vec)[1]*(num);(res)[2]=(vec)[2]*(num);}
#define POW(x) ((x)*(x))

//Weapon ids
#define WEAPONLIST_P228			1
#define	WEAPONLIST_UNKNOWN1		2
#define	WEAPONLIST_SCOUT		3
#define	WEAPONLIST_HEGRENADE	4
#define	WEAPONLIST_XM1014		5
#define	WEAPONLIST_C4			6
#define	WEAPONLIST_MAC10		7
#define	WEAPONLIST_AUG			8
#define	WEAPONLIST_SMOKEGRENADE	9
#define	WEAPONLIST_ELITE		10
#define	WEAPONLIST_FIVESEVEN	11
#define	WEAPONLIST_UMP45		12
#define	WEAPONLIST_SG550		13
#define	WEAPONLIST_GALIL		14
#define	WEAPONLIST_FAMAS		15
#define	WEAPONLIST_USP			16
#define	WEAPONLIST_GLOCK18		17
#define	WEAPONLIST_AWP			18
#define	WEAPONLIST_MP5			19
#define	WEAPONLIST_M249			20
#define	WEAPONLIST_M3			21
#define	WEAPONLIST_M4A1			22
#define	WEAPONLIST_TMP			23
#define	WEAPONLIST_G3SG1		24
#define	WEAPONLIST_FLASHBANG	25
#define	WEAPONLIST_DEAGLE		26
#define	WEAPONLIST_SG552		27
#define	WEAPONLIST_AK47			28
#define	WEAPONLIST_KNIFE		29
#define	WEAPONLIST_P90			30

int pfnHookUserMsg( char *szMsgName, pfnUserMsgHook pfn );

extern HINSTANCE g_hInstance;
extern SCREENINFO g_Screen;
extern cl_clientfunc_t *g_pClient;
extern cl_clientfunc_t g_Client;
extern cl_enginefunc_t *g_pEngine;
extern cl_enginefunc_t g_Engine;
extern engine_studio_api_t *g_pStudio;
extern engine_studio_api_t g_Studio;

string szDirFile( char* pszName );
string Prefix_ini();
void add_log( int num, char * fmt, ... );

inline char *PrefHack( char *IfCmd, string Prefix, char *Name ) {
	char *o = new char[ 64 ];
	strcpy( o, IfCmd );
	strcat( o, Prefix.c_str() );
	strcat( o, Name );
	return o;
}

// angles is output, forward is input
inline void _fastcall VectorAngles( const float *forward, float *angles ) {
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
#endif

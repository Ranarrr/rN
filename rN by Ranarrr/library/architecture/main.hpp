#ifndef MAIN_HPP
#define MAIN_HPP

#define WIN32_LEAN_AND_MEAN

#define M_PI 3.14159265358979323846f
#define RAD2DEG( x ) x * ( 180 / M_PI )
#define DEG2RAD( x ) x * ( M_PI / 180.f )
#define POW(x) ((x)*(x))

#include <windows.h>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <list>
#include <map>

#include "sdk/rect.hpp"
#include "sdk/cl_dll.hpp"
#include "sdk/r_studioint.hpp"
#include "sdk/cl_entity.hpp"
#include "sdk/com_model.hpp"
#include "sdk/triangleapi.hpp"
#include "sdk/pmtrace.hpp"
#include "sdk/pm_defs.hpp"
#include "sdk/ref_params.hpp"
#include "sdk/net_api.hpp"
#include "sdk/r_efx.hpp"
#include "sdk/cvardef.hpp"
#include "sdk/util_vector.hpp"
#include "sdk/parsemsg.hpp"
#include "sdk/studio.hpp"
#include "sdk/event_args.hpp"
#include "sdk/event_flags.hpp"
#include "sdk/event_api.hpp"
#include "sdk/screenfade.hpp"
#include "sdk/entity_types.hpp"
#include "sdk/keydefs.hpp"
#include "sdk/engine_launcher_api.hpp"
#include "sdk/con_nprint.hpp"
#include "sdk/edict.hpp"

#include "features/scrollstructs.hpp"
#include "sdk/color.hpp"
#include "sdk/entity.hpp"
#include "tools/hiding.hpp"
#include "tools/offsets.hpp"
#include "hooks/client.hpp"
#include "tools/drawing.hpp"
#include "hooks/hook.hpp"
#include "tools/Instruments.hpp"
#include "cvars.hpp"
#include "tools/Command.hpp"
#include "features/NoFlash.hpp"
#include "features/esp.hpp"
#include "tools/xstring.hpp"
#include "features/cBuddy.hpp"

struct cl_clientfuncs_s {
	int			( *Initialize )						( cl_enginefuncs_s *pEnginefuncs, int iVersion );
	int			( *HUD_Init )						();
	int			( *HUD_VidInit )					();
	void		( *HUD_Redraw )						( float time, int intermission );
	int			( *HUD_UpdateClientData )			( client_data_s *pcldata, float flTime );
	int			( *HUD_Reset )						();
	void		( *HUD_PlayerMove )					( playermove_s *ppmove, int server );
	void		( *HUD_PlayerMoveInit )				( playermove_s *ppmove );
	char		( *HUD_PlayerMoveTexture )			( char *name );
	void		( *IN_ActivateMouse )				();
	void		( *IN_DeactivateMouse )				();
	void		( *IN_MouseEvent )					( int mstate );
	void		( *IN_ClearStates )					();
	void		( *IN_Accumulate )					();
	void		( *CL_CreateMove )					( float frametime, usercmd_s *cmd, int active );
	int			( *CL_IsThirdPerson )				();
	void		( *CL_CameraOffset )				( float *ofs );
	kbutton_s	*( *KB_Find )						( const char *name );
	void		( *CAM_Think )						();
	void		( *V_CalcRefdef )					( ref_params_s *pparams );
	int			( *HUD_AddEntity )					( int type, cl_entity_s *ent, const char *modelname );
	void		( *HUD_CreateEntities )				();
	void		( *HUD_DrawNormalTriangles )		();
	void		( *HUD_DrawTransparentTriangles )	();
	void		( *HUD_StudioEvent )				( const mstudioevent_s *event, const cl_entity_s *entity );
	void		( *HUD_PostRunCmd )					( local_state_s *from, local_state_s *to, usercmd_s *cmd, int runfuncs, double time, unsigned int random_seed );
	void		( *HUD_Shutdown )					();
	void		( *HUD_TxferLocalOverrides )		( entity_state_s *state, const clientdata_s *client );
	void		( *HUD_ProcessPlayerState )			( entity_state_s *dst, const entity_state_s *src );
	void		( *HUD_TxferPredictionData )		( entity_state_s *ps, const entity_state_s *pps, clientdata_s *pcd, const clientdata_s *ppcd, weapon_data_s *wd, const weapon_data_s *pwd );
	void		( *Demo_ReadBuffer )				( int size, unsigned char *buffer );
	int			( *HUD_ConnectionlessPacket )		( netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );
	int			( *HUD_GetHullBounds )				( int hullnumber, float *mins, float *maxs );
	void		( *HUD_Frame )						( double time );
	int			( *HUD_Key_Event )					( int down, int keynum, const char *pszCurrentBinding );
	void		( *HUD_TempEntUpdate )				( double frametime, double client_time, double cl_gravity, tempent_s **ppTempEntFree, tempent_s **ppTempEntActive, int( *Callback_AddVisibleEntity )( cl_entity_s *pEntity ), void( *Callback_TempEntPlaySound )( tempent_s *pTemp, float damp ) );
	cl_entity_s *( *HUD_GetUserEntity )				( int index );
	int			( *HUD_VoiceStatus )				( int entindex, int bTalking );
	int			( *HUD_DirectorMessage )			( unsigned char command, unsigned int firstObject, unsigned int secondObject, unsigned int flags );
	int			( *HUD_GetStudioModelInterface )	( int version, r_studio_interface_s **ppinterface, engine_studio_api_s *pstudio );
	void		( *HUD_CHATINPUTPOSITION_FUNCTION ) ( int *x, int *y );
	int			( *HUD_GETPLAYERTEAM_FUNCTION )		( int iplayer );
	void		( *CLIENTFACTORY )					();
};

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

extern HINSTANCE g_hInstance;
extern SCREENINFO_s g_Screen;
extern cl_clientfuncs_s *g_pClient;
extern cl_clientfuncs_s g_Client;
extern cl_enginefuncs_s *g_pEngine;
extern cl_enginefuncs_s g_Engine;
extern engine_studio_api_s *g_pStudio;
extern engine_studio_api_s g_Studio;

#endif // MAIN_HPP
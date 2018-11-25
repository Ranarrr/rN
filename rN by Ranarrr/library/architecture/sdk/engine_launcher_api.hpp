#ifndef ENGINE_LAUNCHER_API_HPP
#define ENGINE_LAUNCHER_API_HPP

#pragma once

// engine/launcher interface

#define RENDERTYPE_UNDEFINED		0
#define RENDERTYPE_SOFTWARE			1
#define RENDERTYPE_HARDWARE			2

#define ENGINE_LAUNCHER_API_VERSION 1

struct engine_api_s {
	int		version;
	int		rendertype;
	int		size;

	// Functions
	int		( *GetEngineState )				();
	void	( *Cbuf_AddText )				( char *text ); // append cmd at end of buf
	void	( *Cbuf_InsertText )			( char *text ); // insert cmd at start of buf
	void	( *Cmd_AddCommand )				( char *cmd_name, void( *funcname )( void ) );
	int		( *Cmd_Argc )					();
	char	*( *Cmd_Args )					();
	char	*( *Cmd_Argv )					( int arg );
	void	( *Con_Printf )					( char *, ... );
	void	( *Con_SafePrintf )				( char *, ... );
	void	( *Cvar_Set )					( char *var_name, char *value );
	void	( *Cvar_SetValue )				( char *var_name, float value );
	int		( *Cvar_VariableInt )			( char *var_name );
	char	*( *Cvar_VariableString )		( char *var_name );
	float	( *Cvar_VariableValue )			( char *var_name );
	void	( *ForceReloadProfile )			();
	int		( *GetGameInfo )				( void *pGI, char *pszChannel );
	void	( *GameSetBackground )			( int bBack );
	void	( *GameSetState )				( int iState );
	void	( *GameSetSubState )			( int iState );
	int		( *GetPauseState )				();
	int		( *Host_Frame )					( float time, int iState, int *stateInfo );
	void	( *Host_GetHostInfo )			( float *fps, int *nActive, int *nSpectators, int *nMaxPlayers, char *pszMap );
	void	( *Host_Shutdown )				();
	int		( *Game_Init )					( char *lpCmdLine, unsigned char *pMem, int iSize, void *pef, void *, int );
	void	( *IN_ActivateMouse )			();
	void	( *IN_ClearStates )				();
	void	( *IN_DeactivateMouse )			();
	void	( *IN_MouseEvent )				( int mstate );
	void	( *Keyboard_ReturnToGame )		();
	void	( *Key_ClearStates )			();
	void	( *Key_Event )					( int key, int down );
	int		( *LoadGame )					( const char *pszSlot );
	void	( *S_BlockSound )				();
	void	( *S_ClearBuffer )				();
	void	( *S_GetDSPointer )				( void **lpDS, void **lpDSBuf );
	void 	*( *S_GetWAVPointer )			();
	void	( *S_UnblockSound )				();
	int		( *SaveGame )					( const char *pszSlot, const char *pszComment );
	void	( *SetAuth )					( void *pobj );
	void	( *SetMessagePumpDisableMode )	( int bMode );
	void	( *SetPauseState )				( int bPause );
	void	( *SetStartupMode )				( int bMode );
	void	( *SNDDMA_Shutdown )			();
	void	( *Snd_AcquireBuffer )			();
	void	( *Snd_ReleaseBuffer )			();
	void	( *StoreProfile )				();
	double	( *Sys_FloatTime )				();
	void	( *VID_UpdateWindowVars )		( void *prc, int x, int y );
	void	( *VID_UpdateVID )				( void *pvid );

	void	( *VGui_CallEngineSurfaceProc )	( void *hwnd, unsigned int msg, unsigned int wparam, long lparam ); // VGUI interfaces

	void	( *EngineTakingFocus )			(); // notifications that the launcher is taking/giving focus to the engine
	void	( *LauncherTakingFocus )		();

	// Only filled in by rendertype RENDERTYPE_HARDWARE
	void	( *GL_Init )					();
	int		( *GL_SetMode )					( void *hwndGame, void **pmaindc, void **pbaseRC, int fD3D, const char *p, const char *pszCmdLine );
	void	( *GL_Shutdown )				( void *hwnd, void *hdc, void *hglrc );

	void	( *QGL_D3DShared )				( void *d3dGShared );

	int		( __stdcall *glSwapBuffers )	( void *dc );
	void	( *DirectorProc )				( unsigned int cmd, void *params );
};

#endif // !ENGINE_LAUNCHER_API_HPP
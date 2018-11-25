#ifndef EIFACE_HPP
#define EIFACE_HPP

#pragma once

#include <stdio.h>
#include "custom.hpp"
#include "cvardef.hpp"
#include "pm_defs.hpp"
#include "entity_state.hpp"
#include "netadr.hpp"
#include "weaponinfo.hpp"
#include "progdefs.hpp"
#include "usercmd.hpp"

#define MAX_LEVEL_CONNECTIONS	16		// These are encoded in the lower 16bits of ENTITYTABLE->flags

//
// Defines entity interface between engine and DLLs.
// This header file included by engine files and DLL files.
//
// Before including this header, DLLs must:
//		include progdefs.h
// This is conveniently done for them in extdll.h
//

enum ALERT_TYPE {
	at_notice,
	at_console,		// same as at_notice, but forces a ConPrintf, not a message box
	at_aiconsole,	// same as at_console, but only shown if developer level is 2!
	at_warning,
	at_error,
	at_logged		// Server print to console (only in multiplayer games).
};

// added for use in pfnClientPrintf
enum PRINT_TYPE {
	print_console,
	print_center,
	print_chat,
};

// For integrity checking of content on clients
enum FORCE_TYPE {
	force_exactfile,			// File on client must exactly match server's file
	force_model_samebounds,		// For model files only, the geometry must fit in the same bbox
	force_model_specifybounds,	// For model files only, the geometry must fit in the specified bbox
};

// Returned by TraceLine
struct TraceResult_s {
	int		fAllSolid;			// if true, plane is not valid
	int		fStartSolid;		// if true, the initial point was in a solid area
	int		fInOpen;
	int		fInWater;
	float	flFraction;			// time completed, 1.0 = didn't hit anything
	Vector	vecEndPos;			// final position
	float	flPlaneDist;
	Vector	vecPlaneNormal;		// surface normal at impact
	edict_s	*pHit;				// entity the surface is on
	int		iHitgroup;			// 0 == generic, non zero is specific body part
};

// CD audio status
struct CDStatus_s {
	int		fPlaying;		// is sound playing right now?
	int		fWasPlaying;	// if not, CD is paused if WasPlaying is true.
	int		fInitialized;
	int		fEnabled;
	int		fPlayLooping;
	float	cdvolume;
	//BYTE 	remap[100];
	int		fCDRom;
	int		fPlayTrack;
};

// Engine hands this to DLLs for functionality callbacks
struct enginefuncs_s {
	int				( *pfnPrecacheModel )			( char* s );
	int				( *pfnPrecacheSound )			( char* s );
	void			( *pfnSetModel )				( edict_s *e, const char *m );
	int				( *pfnModelIndex )				( const char *m );
	int				( *pfnModelFrames )				( int modelIndex );
	void			( *pfnSetSize )					( edict_s *e, const float *rgflMin, const float *rgflMax );
	void			( *pfnChangeLevel )				( char *s1, char *s2 );
	void			( *pfnGetSpawnParms )			( edict_s *ent );
	void			( *pfnSaveSpawnParms )			( edict_s *ent );
	float			( *pfnVecToYaw )				( const float *rgflVector );
	void			( *pfnVecToAngles )				( const float *rgflVectorIn, float *rgflVectorOut );
	void			( *pfnMoveToOrigin )			( edict_s *ent, const float *pflGoal, float dist, int iMoveType );
	void			( *pfnChangeYaw )				( edict_s *ent );
	void			( *pfnChangePitch )				( edict_s *ent );
	edict_s			*( *pfnFindEntityByString )		( edict_s *pEdictStartSearchAfter, const char *pszField, const char *pszValue );
	int				( *pfnGetEntityIllum )			( edict_s *pEnt );
	edict_s			*( *pfnFindEntityInSphere )		( edict_s *pEdictStartSearchAfter, const float *org, float rad );
	edict_s			*( *pfnFindClientInPVS )		( edict_s *pEdict );
	edict_s			*( *pfnEntitiesInPVS )			( edict_s *pplayer );
	void			( *pfnMakeVectors )				( const float *rgflVector );
	void			( *pfnAngleVectors )			( const float *rgflVector, float *forward, float *right, float *up );
	edict_s			*( *pfnCreateEntity )			();
	void			( *pfnRemoveEntity )			( edict_s *e );
	edict_s			*( *pfnCreateNamedEntity )		( int className );
	void			( *pfnMakeStatic )				( edict_s *ent );
	int				( *pfnEntIsOnFloor )			( edict_s *e );
	int				( *pfnDropToFloor )				( edict_s *e );
	int				( *pfnWalkMove )				( edict_s *ent, float yaw, float dist, int iMode );
	void			( *pfnSetOrigin )				( edict_s *e, const float *rgflOrigin );
	void			( *pfnEmitSound )				( edict_s *entity, int channel, const char *sample, /*int*/ float volume, float attenuation, int fFlags, int pitch );
	void			( *pfnEmitAmbientSound )		( edict_s *entity, float *pos, const char *samp, float vol, float attenuation, int fFlags, int pitch );
	void			( *pfnTraceLine )				( const float *v1, const float *v2, int fNoMonsters, edict_s *pentToSkip, TraceResult_s *ptr );
	void			( *pfnTraceToss )				( edict_s *pent, edict_s *pentToIgnore, TraceResult_s *ptr );
	int				( *pfnTraceMonsterHull )		( edict_s *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_s *pentToSkip, TraceResult_s *ptr );
	void			( *pfnTraceHull )				( const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_s *pentToSkip, TraceResult_s *ptr );
	void			( *pfnTraceModel )				( const float *v1, const float *v2, int hullNumber, edict_s *pent, TraceResult_s *ptr );
	const char		*( *pfnTraceTexture )			( edict_s *pTextureEntity, const float *v1, const float *v2 );
	void			( *pfnTraceSphere )				( const float *v1, const float *v2, int fNoMonsters, float radius, edict_s *pentToSkip, TraceResult_s *ptr );
	void			( *pfnGetAimVector )			( edict_s *ent, float speed, float *rgflReturn );
	void			( *pfnServerCommand )			( char* str );
	void			( *pfnServerExecute )			();
	void			( *pfnClientCommand )			( edict_s *pEdict, char* szFmt, ... );
	void			( *pfnParticleEffect )			( const float *org, const float *dir, float color, float count );
	void			( *pfnLightStyle )				( int style, char* val );
	int				( *pfnDecalIndex )				( const char *name );
	int				( *pfnPointContents )			( const float *rgflVector );
	void			( *pfnMessageBegin )			( int msg_dest, int msg_type, const float *pOrigin, edict_s *ed );
	void			( *pfnMessageEnd )				();
	void			( *pfnWriteByte )				( int iValue );
	void			( *pfnWriteChar )				( int iValue );
	void			( *pfnWriteShort )				( int iValue );
	void			( *pfnWriteLong )				( int iValue );
	void			( *pfnWriteAngle )				( float flValue );
	void			( *pfnWriteCoord )				( float flValue );
	void			( *pfnWriteString )				( const char *sz );
	void			( *pfnWriteEntity )				( int iValue );
	void			( *pfnCVarRegister )			( cvar_s *pCvar );
	float			( *pfnCVarGetFloat )			( const char *szVarName );
	const char		*( *pfnCVarGetString )			( const char *szVarName );
	void			( *pfnCVarSetFloat )			( const char *szVarName, float flValue );
	void			( *pfnCVarSetString )			( const char *szVarName, const char *szValue );
	void			( *pfnAlertMessage )			( ALERT_TYPE atype, char *szFmt, ... );
	void			( *pfnEngineFprintf )			( FILE *pfile, char *szFmt, ... );
	void			*( *pfnPvAllocEntPrivateData )	( edict_s *pEdict, long cb );
	void			*( *pfnPvEntPrivateData )		( edict_s *pEdict );
	void			( *pfnFreeEntPrivateData )		( edict_s *pEdict );
	const char		*( *pfnSzFromIndex )			( int iString );
	int				( *pfnAllocString )				( const char *szValue );
	entvars_s		*( *pfnGetVarsOfEnt )			( edict_s *pEdict );
	edict_s			*( *pfnPEntityOfEntOffset )		( int iEntOffset );
	int				( *pfnEntOffsetOfPEntity )		( const edict_s *pEdict );
	int				( *pfnIndexOfEdict )			( const edict_s *pEdict );
	edict_s			*( *pfnPEntityOfEntIndex )		( int iEntIndex );
	edict_s			*( *pfnFindEntityByVars )		( entvars_s* pvars );
	void			*( *pfnGetModelPtr )			( edict_s *pEdict );
	int				( *pfnRegUserMsg )				( const char *pszName, int iSize );
	void			( *pfnAnimationAutomove )		( const edict_s *pEdict, float flTime );
	void			( *pfnGetBonePosition )			( const edict_s *pEdict, int iBone, float *rgflOrigin, float *rgflAngles );
	unsigned long	( *pfnFunctionFromName )		( const char *pName );
	const char		*( *pfnNameForFunction )		( unsigned long function );
	void			( *pfnClientPrintf )			( edict_s *pEdict, PRINT_TYPE ptype, const char *szMsg ); // JOHN: engine callbacks so game DLL can print messages to individual clients
	void			( *pfnServerPrint )				( const char *szMsg );
	const char		*( *pfnCmd_Args )				();		// these 3 added
	const char		*( *pfnCmd_Argv )				( int argc );	// so game DLL can easily
	int				( *pfnCmd_Argc )				();		// access client 'cmd' strings
	void			( *pfnGetAttachment )			( const edict_s *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles );
	void			( *pfnCRC32_Init )				( unsigned long *pulCRC );
	void			( *pfnCRC32_ProcessBuffer )		( unsigned long *pulCRC, void *p, int len );
	void			( *pfnCRC32_ProcessByte )		( unsigned long *pulCRC, unsigned char ch );
	unsigned long	( *pfnCRC32_Final )				( unsigned long pulCRC );
	long			( *pfnRandomLong )				( long lLow, long lHigh );
	float			( *pfnRandomFloat )				( float flLow, float flHigh );
	void			( *pfnSetView )					( const edict_s *pClient, const edict_s *pViewent );
	float			( *pfnTime )					();
	void			( *pfnCrosshairAngle )			( const edict_s *pClient, float pitch, float yaw );
	unsigned char	*( *pfnLoadFileForMe )			( char *filename, int *pLength );
	void			( *pfnFreeFile )				( void *buffer );
	void			( *pfnEndSection )				( const char *pszSectionName ); // trigger_endsection
	int				( *pfnCompareFileTime )			( char *filename1, char *filename2, int *iCompare );
	void			( *pfnGetGameDir )				( char *szGetGameDir );
	void			( *pfnCvar_RegisterVariable )	( cvar_s *variable );
	void			( *pfnFadeClientVolume )		( const edict_s *pEdict, int fadePercent, int fadeOutSeconds, int holdTime, int fadeInSeconds );
	void			( *pfnSetClientMaxspeed )		( const edict_s *pEdict, float fNewMaxspeed );
	edict_s 		*( *pfnCreateFakeClient )		( const char *netname );	// returns NULL if fake client can't be created
	void			( *pfnRunPlayerMove )			( edict_s *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, unsigned char impulse, unsigned char msec );
	int				( *pfnNumberOfEntities )		();
	char			*( *pfnGetInfoKeyBuffer )		( edict_s *e );	// passing in NULL gets the serverinfo
	char			*( *pfnInfoKeyValue )			( char *infobuffer, char *key );
	void			( *pfnSetKeyValue )				( char *infobuffer, char *key, char *value );
	void			( *pfnSetClientKeyValue )		( int clientIndex, char *infobuffer, char *key, char *value );
	int				( *pfnIsMapValid )				( char *filename );
	void			( *pfnStaticDecal )				( const float *origin, int decalIndex, int entityIndex, int modelIndex );
	int				( *pfnPrecacheGeneric )			( char *s );
	int				( *pfnGetPlayerUserId )			( edict_s *e ); // returns the server assigned userid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients
	void			( *pfnBuildSoundMsg )			( edict_s *entity, int channel, const char *sample, /*int*/ float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_s *ed );
	int				( *pfnIsDedicatedServer )		();// is this a dedicated server?
	cvar_s			*( *pfnCVarGetPointer )			( const char *szVarName );
	unsigned int	( *pfnGetPlayerWONId )			( edict_s *e ); // returns the server assigned WONid for this player.  useful for logging frags, etc.  returns -1 if the edict couldn't be found in the list of clients

	// Physics additions
	void			( *pfnInfo_RemoveKey )			( char *s, const char *key );
	const char		*( *pfnGetPhysicsKeyValue )		( const edict_s *pClient, const char *key );
	void			( *pfnSetPhysicsKeyValue )		( const edict_s *pClient, const char *key, const char *value );
	const char		*( *pfnGetPhysicsInfoString	)	( const edict_s *pClient );
	unsigned short	( *pfnPrecacheEvent )			( int type, const char *psz );
	void			( *pfnPlaybackEvent )			( int flags, const edict_s *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );

	unsigned char	*( *pfnSetFatPVS )				( float *org );
	unsigned char	*( *pfnSetFatPAS )				( float *org );

	int				( *pfnCheckVisibility )			( const edict_s *entity, unsigned char *pset );

	void			( *pfnDeltaSetField )			( void *pFields, const char *fieldname ); // delta_s ??? this is undefined everywhere, can't find. pFields
	void			( *pfnDeltaUnsetField )			( void *pFields, const char *fieldname ); // delta_s ??? this is undefined everywhere, can't find. pFields
	void			( *pfnDeltaAddEncoder )			( char *name, void( *conditionalencode )( void *pFields, const unsigned char *from, const unsigned char *to ) ); // delta_s ??? this is undefined everywhere, can't find. pFields
	int				( *pfnGetCurrentPlayer )		();
	int				( *pfnCanSkipPlayer )			( const edict_s *player );
	int				( *pfnDeltaFindField )			( void *pFields, const char *fieldname ); // delta_s ??? this is undefined everywhere, can't find. pFields
	void			( *pfnDeltaSetFieldByIndex )	( void *pFields, int fieldNumber ); // delta_s ??? this is undefined everywhere, can't find. pFields
	void			( *pfnDeltaUnsetFieldByIndex )	( void *pFields, int fieldNumber ); // delta_s ??? this is undefined everywhere, can't find. pFields

	void			( *pfnSetGroupMask )			( int mask, int op );

	int				( *pfnCreateInstancedBaseline ) ( int classname, entity_state_s *baseline );
	void			( *pfnCvar_DirectSet )			( cvar_s *var, char *value );

	// Forces the client and server to be running with the same version of the specified file
	//  (e.g., a player model).
	// Calling this has no effect in single player
	void			( *pfnForceUnmodified )			( FORCE_TYPE type, float *mins, float *maxs, const char *filename );

	void			( *pfnGetPlayerStats )			( const edict_s *pClient, int *ping, int *packet_loss );

	void			( *pfnAddServerCommand )		( char *cmd_name, void( *function ) () );

	// For voice communications, set which clients hear eachother.
	// NOTE: these functions take player entity indices (starting at 1).
	int				( *pfnVoice_GetClientListening )( int iReceiver, int iSender );
	int				( *pfnVoice_SetClientListening )( int iReceiver, int iSender, int bListen );
};
// ONLY ADD NEW FUNCTIONS TO THE END OF THIS STRUCT.  INTERFACE VERSION IS FROZEN AT 138

// Passed to pfnKeyValue
struct KeyValueData_s {
	char	*szClassName;	// in: entity classname
	char	*szKeyName;		// in: name of key
	char	*szValue;		// in: value of key
	long	fHandled;		// out: DLL sets to true if key-value pair was understood
};

struct LEVELLIST_s {
	char		mapName[ 32 ];
	char		landmarkName[ 32 ];
	edict_s		*pentLandmark;
	Vector		vecLandmarkOrigin;
};

struct ENTITYTABLE_s {
	int			id;				// Ordinal ID of this entity (used for entity <--> pointer conversions)
	edict_s		*pent;			// Pointer to the in-game entity

	int			location;		// Offset from the base data of this entity
	int			size;			// Byte size of this entity's data
	int			flags;			// This could be a short -- bit mask of transitions that this entity is in the PVS of
	int			classname;		// entity class name
};

struct saverestore_s {
	char			*pBaseData;		// Start of all entity save data
	char			*pCurrentData;	// Current buffer pointer for sequential access
	int				size;			// Current data size
	int				bufferSize;		// Total space for data
	int				tokenSize;		// Size of the linear list of tokens
	int				tokenCount;		// Number of elements in the pTokens table
	char			**pTokens;		// Hash table of entity strings (sparse)
	int				currentIndex;	// Holds a global entity table ID
	int				tableCount;		// Number of elements in the entity table
	int				connectionCount;// Number of elements in the levelList[]
	ENTITYTABLE_s	*pTable;		// Array of ENTITYTABLE elements (1 for each entity)
	LEVELLIST_s		levelList[ MAX_LEVEL_CONNECTIONS ];		// List of connections from this level

	// smooth transition
	int			fUseLandmark;
	char		szLandmarkName[ 20 ];// landmark we'll spawn near in next level
	Vector		vecLandmarkOffset;// for landmark transitions
	float		time;
	char		szCurrentMapName[ 32 ];	// To check global entities
};

enum fieldtypes_t {
	FIELD_FLOAT,		// Any floating point value
	FIELD_STRING,			// A string ID (return from ALLOC_STRING)
	FIELD_ENTITY,			// An entity offset (EOFFSET)
	FIELD_CLASSPTR,			// CBaseEntity *
	FIELD_EHANDLE,			// Entity handle
	FIELD_EVARS,			// EVARS *
	FIELD_EDICT,			// edict_t *, or edict_t *  (same thing)
	FIELD_VECTOR,			// Any vector
	FIELD_POSITION_VECTOR,	// A world coordinate (these are fixed up across level transitions automagically)
	FIELD_POINTER,			// Arbitrary data pointer... to be removed, use an array of FIELD_CHARACTER
	FIELD_INTEGER,			// Any integer or enum
	FIELD_FUNCTION,			// A class function pointer (Think, Use, etc)
	FIELD_BOOLEAN,			// boolean, implemented as an int, I may use this as a hint for compression
	FIELD_SHORT,			// 2 byte integer
	FIELD_CHARACTER,		// a byte
	FIELD_TIME,				// a floating point time (these are fixed up automatically too!)
	FIELD_MODELNAME,		// Engine string that is a model name (needs precache)
	FIELD_SOUNDNAME,		// Engine string that is a sound name (needs precache)

	FIELD_TYPECOUNT,		// MUST BE LAST
};
 
struct TYPEDESCRIPTION_s {
	fieldtypes_t	fieldType;
	char			*fieldName;
	int				fieldOffset;
	short			fieldSize;
	short			flags;
};

struct DLL_FUNCTIONS_s {
	// Initialize/shutdown the game (one-time call after loading of game .dll)
	void			( *pfnGameInit )				();
	int				( *pfnSpawn )					( edict_s *pent );
	void			( *pfnThink )					( edict_s *pent );
	void			( *pfnUse )						( edict_s *pentUsed, edict_s *pentOther );
	void			( *pfnTouch )					( edict_s *pentTouched, edict_s *pentOther );
	void			( *pfnBlocked )					( edict_s *pentBlocked, edict_s *pentOther );
	void			( *pfnKeyValue )				( edict_s *pentKeyvalue, KeyValueData_s *pkvd );
	void			( *pfnSave )					( edict_s *pent, saverestore_s *pSaveData );
	int				( *pfnRestore )					( edict_s *pent, saverestore_s *pSaveData, int globalEntity );
	void			( *pfnSetAbsBox )				( edict_s *pent );

	void			( *pfnSaveWriteFields )			( saverestore_s *pSaveData, const char *pszName, void *pBaseData, TYPEDESCRIPTION_s *pFields, int fieldCount );
	void			( *pfnSaveReadFields )			( saverestore_s *pSaveData, const char *pszName, void *pBaseData, TYPEDESCRIPTION_s *pFields, int fieldCount );

	void			( *pfnSaveGlobalState )			( saverestore_s *pSaveData );
	void			( *pfnRestoreGlobalState )		( saverestore_s *pSaveData );
	void			( *pfnResetGlobalState )		();

	int				( *pfnClientConnect )			( edict_s *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ] );

	void			( *pfnClientDisconnect )		( edict_s *pEntity );
	void			( *pfnClientKill )				( edict_s *pEntity );
	void			( *pfnClientPutInServer )		( edict_s *pEntity );
	void			( *pfnClientCommand )			( edict_s *pEntity );
	void			( *pfnClientUserInfoChanged )	( edict_s *pEntity, char *infobuffer );

	void			( *pfnServerActivate )			( edict_s *pEdictList, int edictCount, int clientMax );
	void			( *pfnServerDeactivate )		();

	void			( *pfnPlayerPreThink )			( edict_s *pEntity );
	void			( *pfnPlayerPostThink )			( edict_s *pEntity );

	void			( *pfnStartFrame )				();
	void			( *pfnParmsNewLevel )			();
	void			( *pfnParmsChangeLevel )		();

	 // Returns string describing current .dll.  E.g., TeamFotrress 2, Half-Life
	const char		*( *pfnGetGameDescription )		();

	// Notify dll about a player customization.
	void			( *pfnPlayerCustomization )		( edict_s *pEntity, customization_s *pCustom );

	// Spectator funcs
	void			( *pfnSpectatorConnect )		( edict_s *pEntity );
	void			( *pfnSpectatorDisconnect )		( edict_s *pEntity );
	void			( *pfnSpectatorThink )			( edict_s *pEntity );

	// Notify game .dll that engine is going to shut down.  Allows mod authors to set a breakpoint.
	void			( *pfnSys_Error )				( const char *error_string );

	void			( *pfnPM_Move )					( playermove_s *ppmove, int server );
	void			( *pfnPM_Init )					( playermove_s *ppmove );
	char			( *pfnPM_FindTextureType )		( char *name );
	void			( *pfnSetupVisibility )			( edict_s *pViewEntity, edict_s *pClient, unsigned char **pvs, unsigned char **pas );
	void			( *pfnUpdateClientData )		( const edict_s *ent, int sendweapons, clientdata_s *cd );
	int				( *pfnAddToFullPack )			( entity_state_s *state, int e, edict_s *ent, edict_s *host, int hostflags, int player, unsigned char *pSet );
	void			( *pfnCreateBaseline )			( int player, int eindex, struct entity_state_s *baseline, edict_s *entity, int playermodelindex, Vector player_mins, Vector player_maxs );
	void			( *pfnRegisterEncoders )		();
	int				( *pfnGetWeaponData )			( edict_s *player, weapon_data_s *info );

	void			( *pfnCmdStart )				( const edict_s *player, const usercmd_s *cmd, unsigned int random_seed );
	void			( *pfnCmdEnd )					( const edict_s *player );

	// Return 1 if the packet is valid.  Set response_buffer_size if you want to send a response packet.  Incoming, it holds the max
	//  size of the response_buffer, so you must zero it out if you choose not to respond.
	int				( *pfnConnectionlessPacket )	( const netadr_s *net_from, const char *args, char *response_buffer, int *response_buffer_size );

	// Enumerates player hulls.  Returns 0 if the hull number doesn't exist, 1 otherwise
	int				( *pfnGetHullBounds )			( int hullnumber, float *mins, float *maxs );

	// Create baselines for certain "unplaced" items.
	void			( *pfnCreateInstancedBaselines )();

	// One of the pfnForceUnmodified files failed the consistency check for the specified player
	// Return 0 to allow the client to continue, 1 to force immediate disconnection (with an optional disconnect message of up to 256 characters)
	int				( *pfnInconsistentFile )		( const edict_s *player, const char *filename, char *disconnect_message );

	// The game .dll should return 1 if lag compensation should be allowed (could also just set
	//  the sv_unlag cvar.
	// Most games right now should return 0, until client-side weapon prediction code is written
	//  and tested for them.
	int				( *pfnAllowLagCompensation )	();
};

extern DLL_FUNCTIONS_s		gEntityInterface;

struct NEW_DLL_FUNCTIONS_s {
	// Called right before the object's memory is freed.
	// Calls its destructor.
	void			( *pfnOnFreeEntPrivateData )	( edict_s *pEnt );
	void			( *pfnGameShutdown )			();
	int				( *pfnShouldCollide )			( edict_s *pentTouched, edict_s *pentOther );
};

// Pointers will be null if the game DLL doesn't support this API.
extern NEW_DLL_FUNCTIONS_s	gNewDLLFunctions;

typedef int( *NEW_DLL_FUNCTIONS_FN )( NEW_DLL_FUNCTIONS_s *pFunctionTable, int *interfaceVersion );

typedef int( *APIFUNCTION )( DLL_FUNCTIONS_s *pFunctionTable, int interfaceVersion );
typedef int( *APIFUNCTION2 )( DLL_FUNCTIONS_s *pFunctionTable, int *interfaceVersion );

#endif // !EIFACE_HPP
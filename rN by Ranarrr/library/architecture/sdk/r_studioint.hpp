#ifndef R_STUDIOINT_HPP
#define R_STUDIOINT_HPP

#pragma once

#include "com_model.hpp"
#include "entity_state.hpp"
#include "cl_entity.hpp"
#include "cvardef.hpp"

struct engine_studio_api_s {
	void			*( *Mem_Calloc )				( int number, size_t size );								// Allocate number*size bytes and zero it
	void			*( *Cache_Check )				( void *c );												// Check to see if pointer is in the cache
	void			( *LoadCacheFile )				( char *path, void *cu );									// Load file into cache (can be swapped out on demand)
	model_s			*( *Mod_ForName )				( const char *name, int crash_if_missing );					// Retrieve model pointer for the named model
	void			*( *Mod_Extradata )				( model_s *mod );											// Retrieve pointer to studio model data block from a model
	model_s			*( *GetModelByIndex )			( int index );												// Retrieve indexed model from client side model precache list
	cl_entity_s		*( *GetCurrentEntity )			();															// Get entity that is set for rendering
	player_info_s	*( *PlayerInfo )				( int index );												// Get referenced player_info_t
	entity_state_s	*( *GetPlayerState )			( int index );												// Get most recently received player state data from network system
	cl_entity_s		*( *GetViewEntity )				();															// Get viewentity
	void			( *GetTimes )					( int *framecount, double *current, double *old );			// Get current frame count, and last two timestampes on client
	cvar_s			*( *GetCvar )					( const char *name );										// Get a pointer to a cvar by name
	void			( *GetViewInfo )				( float *origin, float *upv, float *rightv, float *vpnv );	// Get current render origin and view vectors (up, right and vpn)
	model_s			*( *GetChromeSprite )			();															// Get sprite model used for applying chrome effect
	void			( *GetModelCounters )			( int **s, int **a );										// Get model counters so we can incement instrumentation
	void			( *GetAliasScale )				( float *x, float *y );										// Get software scaling coefficients

// Get bone, light, alias, and rotation matrices
	float			****( *StudioGetBoneTransform ) ();
	float			****( *StudioGetLightTransform )();
	float			***( *StudioGetAliasTransform ) ();
	float			***( *StudioGetRotationMatrix ) ();

	void			( *StudioSetupModel )			( int bodypart, void **ppbodypart, void **ppsubmodel );		// Set up body part, and get submodel pointers
	int				( *StudioCheckBBox )			();															// Check if entity's bbox is in the view frustum

// Apply lighting effects to model
	void			( *StudioDynamicLight )			( cl_entity_s *ent, alight_s *plight );						
	void			( *StudioEntityLight )			( alight_s *plight );
	void			( *StudioSetupLighting )		( alight_s *plighting );

	void			( *StudioDrawPoints )			();															// Draw mesh vertices
	void			( *StudioDrawHulls )			();															// Draw hulls around bones
	void			( *StudioDrawAbsBBox )			();															// Draw bbox around studio models
	void			( *StudioDrawBones )			();															// Draws bones
	void			( *StudioSetupSkin )			( void *ptexturehdr, int index );							// Loads in appropriate texture for model
	void			( *StudioSetRemapColors )		( int top, int bottom );									// Sets up for remapped colors
	model_s			*( *SetupPlayerModel )			( int index );												// Set's player model and returns model pointer
	void			( *StudioClientEvents )			();															// Fires any events embedded in animation

// Retrieve/set forced render effects flags
	int				( *GetForceFaceFlags )			();															
	void			( *SetForceFaceFlags )			( int flags );
	
	void			( *StudioSetHeader )			( void *header );											// Tell engine the value of the studio model header
	void			( *SetRenderModel )				( model_s *model );											// Tell engine which model_t * is being renderered

	// Final state setup and restore for rendering
	void			( *SetupRenderer )				( int rendermode );
	void			( *RestoreRenderer )			();

	void			( *SetChromeOrigin )			();															// Set render origin for applying chrome effect
	int				( *IsHardware )					();															// True if using D3D/OpenGL

	// Only called by hardware interface
	void			( *GL_StudioDrawShadow )		();
	void			( *GL_SetRenderMode )			( int mode );
};

struct r_studio_interface_s {
	int		version;
	int		( *StudioDrawModel )		( int flags );
	int		( *StudioDrawPlayer )		( int flags, entity_state_s *pplayer );
};

extern r_studio_interface_s *pStudioAPI;

#endif // !R_STUDIOINT_HPP
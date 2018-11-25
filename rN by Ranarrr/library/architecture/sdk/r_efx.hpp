#ifndef R_EFX_HPP
#define R_EFX_HPP

#pragma once

#include "particledef.hpp"
#include "beamdef.hpp"
#include "dlight.hpp"
#include "cl_entity.hpp"
#include "pmtrace.hpp"

/*
// FOR REFERENCE, These are the built-in tracer colors.  Note, color 4 is the one
//  that uses the tracerred/tracergreen/tracerblue and traceralpha cvar settings
color24 gTracerColors[] =
{
	{255, 255, 255},	// White
	{255, 0, 0},		// Red
	{0, 255, 0},		// Green
	{0, 0, 255},		// Blue
	{0, 0, 0},			// Tracer default, filled in from cvars, etc.
	{255, 167, 17},		// Yellow-orange sparks
	{255, 130, 90},		// Yellowish streaks (garg)
	{55, 60, 144},		// Blue egon streak
	{255, 130, 90},		// More Yellowish streaks (garg)
	{255, 140, 90},		// More Yellowish streaks (garg)
	{200, 130, 90},		// More red streaks (garg)
	{255, 120, 70},		// Darker red streaks (garg)
};
*/

// Temporary entity array
#define TENTPRIORITY_LOW	0
#define TENTPRIORITY_HIGH	1

// TEMPENTITY flags
#define	FTENT_NONE				0x00000
#define	FTENT_SINEWAVE			0x00001
#define	FTENT_GRAVITY			0x00002
#define FTENT_ROTATE			0x00004
#define	FTENT_SLOWGRAVITY		0x00008
#define FTENT_SMOKETRAIL		0x00010
#define FTENT_COLLIDEWORLD		0x00020
#define FTENT_FLICKER			0x00040
#define FTENT_FADEOUT			0x00080
#define FTENT_SPRANIMATE		0x00100
#define FTENT_HITSOUND			0x00200
#define FTENT_SPIRAL			0x00400
#define FTENT_SPRCYCLE			0x00800
#define FTENT_COLLIDEALL		0x01000 // will collide with world and slideboxes
#define FTENT_PERSIST			0x02000 // tent is not removed when unable to draw
#define FTENT_COLLIDEKILL		0x04000 // tent is removed upon collision with anything
#define FTENT_PLYRATTACHMENT	0x08000 // tent is attached to a player (owner)
#define FTENT_SPRANIMATELOOP	0x10000 // animating sprite doesn't die when last frame is displayed
#define FTENT_SPARKSHOWER		0x20000
#define FTENT_NOMODEL			0x40000 // Doesn't have a model, never try to draw (it just triggers other things)
#define FTENT_CLIENTCUSTOM		0x80000 // Must specify callback.  Callback function is responsible for killing tempent and updating fields (unless other flags specify how to do things)

struct tempent_s {
	int			flags;
	float		die;
	float		frameMax;
	float		x;
	float		y;
	float		z;
	float		fadeSpeed;
	float		bounceFactor;
	int			hitSound;
	void		( *hitcallback )	( tempent_s *ent, pmtrace_s *ptr );
	void		( *callback )		( tempent_s *ent, float frametime, float currenttime );
	tempent_s	*next;
	int			priority;
	short		clientIndex;	// if attached, this is the index of the client to stick to
								// if COLLIDEALL, this is the index of the client to ignore
								// TENTS with FTENT_PLYRATTACHMENT MUST set the clientindex!

	Vector		tentOffset;		// if attached, client origin + tentOffset = tent origin.
	cl_entity_s	entity;

	// baseline.origin		- velocity
	// baseline.renderamt	- starting fadeout intensity
	// baseline.angles		- angle velocity
};

struct efx_api_s {
	particle_s  *( *R_AllocParticle )			( void( *callback ) ( particle_s *particle, float frametime ) );
	void		( *R_BlobExplosion )			( Vector org );
	void		( *R_Blood )					( Vector org, Vector dir, int pcolor, int speed );
	void		( *R_BloodSprite )				( Vector org, int colorindex, int modelIndex, int modelIndex2, float size );
	void		( *R_BloodStream )				( Vector org, Vector dir, int pcolor, int speed );
	void		( *R_BreakModel )				( Vector pos, Vector size, Vector dir, float random, float life, int count, int modelIndex, char flags );
	void		( *R_Bubbles )					( Vector mins, Vector maxs, float height, int modelIndex, int count, float speed );
	void		( *R_BubbleTrail )				( Vector start, Vector end, float height, int modelIndex, int count, float speed );
	void		( *R_BulletImpactParticles )	( Vector pos );
	void		( *R_EntityParticles )			( cl_entity_s *ent );
	void		( *R_Explosion )				( Vector pos, int model, float scale, float framerate, int flags );
	void		( *R_FizzEffect )				( cl_entity_s *pent, int modelIndex, int density );
	void		( *R_FireField ) 				( Vector org, int radius, int modelIndex, int count, int flags, float life );
	void		( *R_FlickerParticles )			( Vector org );
	void		( *R_FunnelSprite )				( Vector org, int modelIndex, int reverse );
	void		( *R_Implosion )				( Vector end, float radius, int count, float life );
	void		( *R_LargeFunnel )				( Vector org, int reverse );
	void		( *R_LavaSplash )				( Vector org );
	void		( *R_MultiGunshot )				( Vector org, Vector dir, Vector noise, int count, int decalCount, int *decalIndices );
	void		( *R_MuzzleFlash )				( Vector pos1, int type );
	void		( *R_ParticleBox )				( Vector mins, Vector maxs, unsigned char r, unsigned char g, unsigned char b, float life );
	void		( *R_ParticleBurst )			( Vector pos, int size, int color, float life );
	void		( *R_ParticleExplosion )		( Vector org );
	void		( *R_ParticleExplosion2 )		( Vector org, int colorStart, int colorLength );
	void		( *R_ParticleLine )				( Vector start, Vector end, unsigned char r, unsigned char g, unsigned char b, float life );
	void		( *R_PlayerSprites )			( int client, int modelIndex, int count, int size );
	void		( *R_Projectile )				( Vector  origin, Vector velocity, int modelIndex, int life, int owner, void( *hitcallback )( tempent_s *ent, pmtrace_s *ptr ) );
	void		( *R_RicochetSound )			( Vector pos );
	void		( *R_RicochetSprite )			( Vector pos, model_s *pmodel, float duration, float scale );
	void		( *R_RocketFlare )				( Vector pos );
	void		( *R_RocketTrail )				( Vector start, Vector end, int type );
	void		( *R_RunParticleEffect )		( Vector org, Vector  dir, int color, int count );
	void		( *R_ShowLine )					( Vector start, Vector end );
	void		( *R_SparkEffect )				( Vector pos, int count, int velocityMin, int velocityMax );
	void		( *R_SparkShower )				( Vector pos );
	void		( *R_SparkStreaks )				( Vector pos, int count, int velocityMin, int velocityMax );
	void		( *R_Spray )					( Vector pos, Vector  dir, int modelIndex, int count, int speed, int spread, int rendermode );
	void		( *R_Sprite_Explode )			( tempent_s *pTemp, float scale, int flags );
	void		( *R_Sprite_Smoke )				( tempent_s *pTemp, float scale );
	void		( *R_Sprite_Spray )				( Vector pos, Vector dir, int modelIndex, int count, int speed, int iRand );
	void		( *R_Sprite_Trail )				( int type, Vector start, Vector end, int modelIndex, int count, float life, float size, float amplitude, int renderamt, float speed );
	void		( *R_Sprite_WallPuff )			( tempent_s *pTemp, float scale );
	void		( *R_StreakSplash )				( Vector pos, Vector dir, int color, int count, float speed, int velocityMin, int velocityMax );
	void		( *R_TracerEffect )				( Vector start, Vector end );
	void		( *R_UserTracerParticle )		( Vector org, Vector vel, float life, int colorIndex, float length, unsigned char deathcontext, void( *deathfunc )( particle_s *particle ) );
	particle_s	*( *R_TracerParticles )			( Vector org, Vector vel, float life );
	void		( *R_TeleportSplash )			( Vector org );
	void		( *R_TempSphereModel )			( Vector pos, float speed, float life, int count, int modelIndex );
	tempent_s	*( *R_TempModel )				( Vector pos, Vector dir, Vector angles, float life, int modelIndex, int soundtype );
	tempent_s	*( *R_DefaultSprite )			( Vector pos, int spriteIndex, float framerate );
	tempent_s	*( *R_TempSprite )				( Vector pos, Vector dir, float scale, int modelIndex, int rendermode, int renderfx, float a, float life, int flags );
	int			( *Draw_DecalIndex )			( int id );
	int			( *Draw_DecalIndexFromName )	( char *name );
	void		( *R_DecalShoot )				( int textureIndex, int entity, int modelIndex, Vector position, int flags );
	void		( *R_AttachTentToPlayer )		( int client, int modelIndex, float zoffset, float life );
	void		( *R_KillAttachedTents )		( int client );
	beam_s		*( *R_BeamCirclePoints )		( int type, Vector start, Vector end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b );
	beam_s		*( *R_BeamEntPoint )			( int startEnt, Vector end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b );
	beam_s		*( *R_BeamEnts )				( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b );
	beam_s		*( *R_BeamFollow )				( int startEnt, int modelIndex, float life, float width, float r, float g, float b, float brightness );
	void		( *R_BeamKill )					( int deadEntity );
	beam_s		*( *R_BeamLightning )			( Vector start, Vector end, int modelIndex, float life, float width, float amplitude, float brightness, float speed );
	beam_s		*( *R_BeamPoints )				( Vector start, Vector end, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b );
	beam_s		*( *R_BeamRing )				( int startEnt, int endEnt, int modelIndex, float life, float width, float amplitude, float brightness, float speed, int startFrame, float framerate, float r, float g, float b );
	dlight_s	*( *CL_AllocDlight )			( int key );
	dlight_s	*( *CL_AllocElight )			( int key );
	tempent_s	*( *CL_TempEntAlloc )			( Vector org, struct model_s *model );
	tempent_s	*( *CL_TempEntAllocNoModel )	( Vector org );
	tempent_s	*( *CL_TempEntAllocHigh )		( Vector org, struct model_s *model );
	tempent_s	*( *CL_TentEntAllocCustom )		( Vector origin, struct model_s *model, int high, void( *callback ) ( struct tempent_s *ent, float frametime, float currenttime ) );
	void		( *R_GetPackedColor )			( short *packed, short color );
	short		( *R_LookupColor )				( unsigned char r, unsigned char g, unsigned char b );
	void		( *R_DecalRemoveAll )			( int textureIndex ); //textureIndex points to the decal index in the array, not the actual texture index.
};

extern efx_api_s efx;

#endif // !R_EFX_HPP
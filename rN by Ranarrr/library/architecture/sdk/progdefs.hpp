#ifndef PROGDEFS_HPP
#define PROGDEFS_HPP

#pragma once

#include "util_vector.hpp"
#include "edict.hpp"

struct globalvars_s {
	float		time;
	float		frametime;
	float		force_retouch;
	int			mapname;
	int			startspot;
	float		deathmatch;
	float		coop;
	float		teamplay;
	float		serverflags;
	float		found_secrets;
	Vector		v_forward;
	Vector		v_up;
	Vector		v_right;
	float		trace_allsolid;
	float		trace_startsolid;
	float		trace_fraction;
	Vector		trace_endpos;
	Vector		trace_plane_normal;
	float		trace_plane_dist;
	edict_s		*trace_ent;
	float		trace_inopen;
	float		trace_inwater;
	int			trace_hitgroup;
	int			trace_flags;
	int			msg_entity;
	int			cdAudioTrack;
	int			maxClients;
	int			maxEntities;
	const char	*pStringBase;

	void		*pSaveData;
	Vector		vecLandmarkOffset;
};

struct entvars_s {
	int			classname;
	int			globalname;

	Vector		origin;
	Vector		oldorigin;
	Vector		velocity;
	Vector		basevelocity;
	Vector      clbasevelocity;  // Base velocity that was passed in to server physics so
								 //  client can predict conveyors correctly.  Server zeroes it, so we need to store here, too.
	Vector		movedir;

	Vector		angles;			// Model angles
	Vector		avelocity;		// angle velocity (degrees per second)
	Vector		punchangle;		// auto-decaying view angle adjustment
	Vector		v_angle;		// Viewing angle (player only)

	// For parametric entities
	Vector		endpos;
	Vector		startpos;
	float		impacttime;
	float		starttime;

	int			fixangle;		// 0:nothing, 1:force view angles, 2:add avelocity
	float		idealpitch;
	float		pitch_speed;
	float		ideal_yaw;
	float		yaw_speed;

	int			modelindex;
	int			model;

	int			viewmodel;		// player's viewmodel
	int			weaponmodel;	// what other players see

	Vector		absmin;		// BB max translated to world coord
	Vector		absmax;		// BB max translated to world coord
	Vector		mins;		// local BB min
	Vector		maxs;		// local BB max
	Vector		size;		// maxs - mins

	float		ltime;
	float		nextthink;

	int			movetype;
	int			solid;

	int			skin;
	int			body;				// sub-model selection for studiomodels
	int 		effects;

	float		gravity;			// % of "normal" gravity
	float		friction;			// inverse elasticity of MOVETYPE_BOUNCE

	int			light_level;

	int			sequence;			// animation sequence
	int			gaitsequence;		// movement animation sequence for player (0 for none)
	float		frame;				// % playback position in animation sequences (0..255)
	float		animtime;			// world time when frame was set
	float		framerate;			// animation playback rate (-8x to 8x)
	unsigned char control[ 4 ];		// bone control setting (0..255)
	unsigned char blending[ 2 ];	// blending amount between sub-sequences (0..255)

	float		scale;				// sprite rendering scale (0..255)

	int			rendermode;
	float		renderamt;
	Vector		rendercolor;
	int			renderfx;

	float		health;
	float		frags;
	int			weapons;			// bit mask for available weapons
	float		takedamage;

	int			deadflag;
	Vector		view_ofs;	// eye position

	int			button;
	int			impulse;

	edict_s		*chain;			// Entity pointer when linked into a linked list
	edict_s		*dmg_inflictor;
	edict_s		*enemy;
	edict_s		*aiment;		// entity pointer when MOVETYPE_FOLLOW
	edict_s		*owner;
	edict_s		*groundentity;

	int			spawnflags;
	int			flags;

	int			colormap;		// lowbyte topcolor, highbyte bottomcolor
	int			team;

	float		max_health;
	float		teleport_time;
	float		armortype;
	float		armorvalue;
	int			waterlevel;
	int			watertype;

	int			target;
	int			targetname;
	int			netname;
	int			message;

	float		dmg_take;
	float		dmg_save;
	float		dmg;
	float		dmgtime;

	int			noise;
	int			noise1;
	int			noise2;
	int			noise3;

	float		speed;
	float		air_finished;
	float		pain_finished;
	float		radsuit_finished;

	edict_s		*pContainingEntity;

	int			playerclass;
	float		maxspeed;

	float		fov;
	int			weaponanim;

	int			pushmsec;

	int			bInDuck;
	int			flTimeStepSound;
	int			flSwimTime;
	int			flDuckTime;
	int			iStepLeft;
	float		flFallVelocity;

	int			gamestate;

	int			oldbuttons;

	int			groupinfo;

	// For mods
	int			iuser1;
	int			iuser2;
	int			iuser3;
	int			iuser4;
	float		fuser1;
	float		fuser2;
	float		fuser3;
	float		fuser4;
	Vector		vuser1;
	Vector		vuser2;
	Vector		vuser3;
	Vector		vuser4;
	edict_s		*euser1;
	edict_s		*euser2;
	edict_s		*euser3;
	edict_s		*euser4;
};

#endif // !PROGDEFS_HPP
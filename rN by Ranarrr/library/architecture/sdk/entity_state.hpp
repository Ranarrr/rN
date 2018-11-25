#ifndef ENTITY_STATE_HPP
#define ENTITY_STATE_HPP

#pragma once

#include "weaponinfo.hpp"
#include "util_vector.hpp"
#include "const.hpp"

// For entityType below
#define ENTITY_NORMAL		( 1 << 0 )
#define ENTITY_BEAM			( 1 << 1 )

// Entity state is used for the baseline and for delta compression of a packet of
// entities that is sent to a client.

struct entity_state_s {
// Fields which are filled in by routines outside of delta compression
	int				entityType;
	// Index into cl_entities array for this entity.
	int				number;
	float			msg_time;

	int				messagenum; // Message number last time the player/entity state was updated.

	// Fields which can be transitted and reconstructed over the network stream
	Vector			origin;
	Vector			angles;

	int				modelindex;
	int				sequence;
	float			frame;
	int				colormap;
	short			skin;
	short			solid;
	int				effects;
	float			scale;

	unsigned char	eflags;

	// Render information
	int				rendermode;
	int				renderamt;
	color24			rendercolor;
	int				renderfx;

	int				movetype;
	float			animtime;
	float			framerate;
	int				body;
	unsigned char	control[ 4 ];
	unsigned char	blending[ 4 ];
	Vector			velocity;

	Vector		mins, maxs;		// Send bbox down to client for use during prediction.
	int			aiment;
	int			owner;			// If owned by a player, the index of that player (for projectiles).
	float		friction;		// Friction, for prediction.
	float		gravity;		// Gravity multiplier

// PLAYER SPECIFIC
	int			team;
	int			playerclass;
	int			health;
	int			spectator;
	int         weaponmodel;
	int			gaitsequence;
	Vector		basevelocity;	// If standing on conveyor, e.g.
	int			usehull;		// Use the crouched hull, or the regular player hull.
	int			oldbuttons;		// Latched buttons last time state updated.
	int			onground;		// -1 = in air, else pmove entity number
	int			iStepLeft;
	float		flFallVelocity; // How fast we are falling

	float		fov;
	int			weaponanim;

	// Parametric movement overrides
	Vector				startpos;
	Vector				endpos;
	float				impacttime;
	float				starttime;

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
};

struct clientdata_s {
	Vector				origin;
	Vector				velocity;

	int					viewmodel;
	Vector				punchangle;
	int					flags;
	int					waterlevel;
	int					watertype;
	Vector				view_ofs;
	float				health;

	int					bInDuck;

	int					weapons; // remove?

	int					flTimeStepSound;
	int					flDuckTime;
	int					flSwimTime;
	int					waterjumptime;

	float				maxspeed;

	float				fov;
	int					weaponanim;

	int					m_iId;
	int					ammo_shells;
	int					ammo_nails;
	int					ammo_cells;
	int					ammo_rockets;
	float				m_flNextAttack;

	int					tfstate;

	int					pushmsec;

	int					deadflag;

	char				physinfo[ MAX_PHYSINFO_STRING ];

	// For mods
	int					iuser1;
	int					iuser2;
	int					iuser3;
	int					iuser4;
	float				fuser1;
	float				fuser2;
	float				fuser3;
	float				fuser4;
	Vector				vuser1;
	Vector				vuser2;
	Vector				vuser3;
	Vector				vuser4;
};

struct local_state_s {
	entity_state_s playerstate;
	clientdata_s   client;
	weapon_data_s  weapondata[ 32 ];
};

#endif // !ENTITY_STATE_HPP
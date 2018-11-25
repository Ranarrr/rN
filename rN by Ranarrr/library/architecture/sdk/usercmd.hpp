#ifndef USERCMD_HPP
#define USERCMD_HPP

#pragma once

struct usercmd_s {
	short			lerp_msec;      // Interpolation time on client
	unsigned char	msec;           // Duration in ms of command
	Vector			viewangles;     // Command view angles.

// intended velocities
	float			forwardmove;    // Forward velocity.
	float			sidemove;       // Sideways velocity.
	float			upmove;         // Upward velocity.
	unsigned char	lightlevel;     // Light level at spot where we are standing.
	unsigned short  buttons;  // Attack buttons
	unsigned char   impulse;          // Impulse command issued.
	unsigned char	weaponselect;	// Current weapon id

// Experimental player impact stuff.
	int				impact_index;
	Vector			impact_position;
};

#endif // !USERCMD_HPP
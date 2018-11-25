#ifndef REF_PARAMS_HPP
#define REF_PARAMS_HPP

#pragma once

#include "util_vector.hpp"
#include "usercmd.hpp"
#include "movevars.hpp"

struct ref_params_s {
	// Output
	Vector		vieworg;
	Vector		viewangles;

	Vector		forward;
	Vector		right;
	Vector		up;

	float		frametime; // Client frametime;
	float		time; // Client time

	// Misc
	int			intermission;
	int			paused;
	int			spectator;
	int			onground;
	int			waterlevel;

	Vector		simvel;
	Vector		simorg;
	Vector		viewheight;

	float		idealpitch;

	Vector		cl_viewangles;

	int			health;

	Vector		crosshairangle;

	float		viewsize;

	Vector		punchangle;

	int			maxclients;
	int			viewentity;
	int			playernum;
	int			max_entities;
	int			demoplayback;
	int			hardware;
	int			smoothing;

	usercmd_s	*cmd;				// Last issued usercmd

	movevars_s	*movevars;			// Movevars

	int			viewport[ 4 ];		// the viewport coordinates x ,y , width, height
	int			nextView;			// the renderer calls ClientDLL_CalcRefdef() and Renderview so long in cycles until this value is 0 (multiple views)
	int			onlyClientDraw;		// if !=0 nothing is drawn by the engine except clientDraw functions
};

#endif // !REF_PARAMS_HPP
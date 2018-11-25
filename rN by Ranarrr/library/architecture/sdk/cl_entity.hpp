#ifndef CL_ENTITY_HPP
#define CL_ENTITY_HPP

#pragma once

#include "entity_state.hpp"
#include "edict.hpp"
#include "com_model.hpp"

#define HISTORY_MAX		64  // Must be power of 2
#define HISTORY_MASK	( HISTORY_MAX - 1 )

struct cl_entity_s;

struct efrag_s {
	mleaf_s		*leaf;
	efrag_s		*leafnext;
	cl_entity_s	*entity;
	efrag_s		*entnext;
};

struct mouth_s {
	unsigned char			mouthopen;		// 0 = mouth closed, 255 = mouth agape
	unsigned char			sndcount;		// counter for running average
	int						sndavg;			// running average
};

struct latchedvars_s {
	float					prevanimtime;
	float					sequencetime;
	unsigned char			prevseqblending[ 2 ];
	Vector					prevorigin;
	Vector					prevangles;

	int						prevsequence;
	float					prevframe;

	unsigned char			prevcontrol[ 4 ];
	unsigned char			prevblending[ 2 ];
};

struct position_history_s {
	// Time stamp for this movement
	float					animtime;

	Vector					origin;
	Vector					angles;
};

struct cl_entity_s {
	int						index;      // Index into cl_entities (should match actual slot, but not necessarily)

	int						player;     // True if this entity is a "player"

	entity_state_s			baseline;   // The original state from which to delta during an uncompressed message
	entity_state_s			prevstate;  // The state information from the penultimate message received from the server
	entity_state_s			curstate;   // The state information from the last message received from server

	int						current_position;  // Last received history update index
	position_history_s		ph[ HISTORY_MAX ];   // History of position and angle updates for this player

	mouth_s					mouth;			// For synchronizing mouth movements.

	latchedvars_s			latched;		// Variables used by studio model rendering routines

	// Information based on interplocation, extrapolation, prediction, or just copied from last msg received.
	//
	float					lastmove;

	// Actual render position and angles
	Vector					origin;
	Vector					angles;

	// Attachment points
	Vector					attachment[ 4 ];

	// Other entity local information
	int						trivial_accept;

	model_s					*model;			// cl.model_precache[curstate.modelindes];  all visible entities have a model
	efrag_s					*efrag;			// linked list of efrags
	mnode_s					*topnode;		// for bmodels, first world node that splits bmodel, or NULL if not split

	float					syncbase;		// for client-side animations -- used by obsolete alias animation system, remove?
	int						visframe;		// last frame this entity was found in an active leaf
	colorVec				cvFloorColor;
};

#endif // !CL_ENTITY_HPP
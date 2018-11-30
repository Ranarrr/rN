#ifndef PMTRACE_HPP
#define PMTRACE_HPP

#pragma once

#include "util_vector.hpp"

enum Hull {
	hull_point,		// Point hull.
	hull_human,		// Human hull. Used by players when standing.
	hull_large,		// Large hull.
	hull_head		// Head hull. Used by players when crouching.
};

struct pmplane_s {
	Vector	normal;
	float	dist;
};

struct pmtrace_s {
	int			allsolid;			// if true, plane is not valid
	int			startsolid;			// if true, the initial point was in a solid area
	int			inopen, inwater;	// End point is in empty space or in water
	float		fraction;			// time completed, 1.0 = didn't hit anything
	Vector		endpos;				// final position
	pmplane_s	plane;				// surface normal at impact
	int			ent;				// entity at impact
	Vector      deltavelocity;		// Change in player's velocity caused by impact.
	int         hitgroup;			// Only run on server.
};

#endif // !PMTRACE_HPP
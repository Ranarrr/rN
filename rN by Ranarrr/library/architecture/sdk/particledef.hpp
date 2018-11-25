#ifndef PARTICLEDEF_HPP
#define PARTICLEDEF_HPP

#pragma once

#include "util_vector.hpp"

enum ptype_t {
	pt_static,
	pt_grav,
	pt_slowgrav,
	pt_fire,
	pt_explode,
	pt_explode2,
	pt_blob,
	pt_blob2,
	pt_vox_slowgrav,
	pt_vox_grav,
	pt_clientcustom   // Must have callback function specified
};

// !!! if this is changed, it must be changed in d_ifacea.h too !!!
struct particle_s {
// driver-usable fields
	Vector		org;
	short		color;
	short		packedColor;
// drivers never touch the following fields
	particle_s	*next;
	Vector		vel;
	float		ramp;
	float		die;
	ptype_t		type;
	void		( *deathfunc )	( particle_s *particle );

// for pt_clientcusttom, we'll call this function each frame
	void		( *callback )	( particle_s *particle, float frametime );

// For deathfunc, etc.
	unsigned char context;
};

#endif // !PARTICLEDEF_HPP
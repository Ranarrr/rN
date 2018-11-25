#ifndef DLIGHT_HPP
#define DLIGHT_HPP

#pragma once

#include "util_vector.hpp"
#include "const.hpp"

struct dlight_s {
	Vector		origin;
	float		radius;
	color24		color;
	float		die;				// stop lighting after this time
	float		decay;				// drop this each second
	float		minlight;			// don't add when contributing less
	int			key;
	int			dark;				// subtracts light instead of adding
};

#endif // !DLIGHT_HPP
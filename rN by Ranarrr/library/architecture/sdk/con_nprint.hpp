#ifndef CON_NPRINT_HPP
#define CON_NPRINT_HPP

#pragma once

struct con_nprint_s {
	int		index;			// Row #
	float	time_to_live;	// # of seconds before it dissappears
	float	color[ 3 ];		// RGB colors (0.0 -> 1.0 scale)
};

void Con_NPrintf( int idx, char *fmt, ... );
void Con_NXPrintf( con_nprint_s *info, char *fmt, ... );

#endif // !CON_NPRINT_HPP
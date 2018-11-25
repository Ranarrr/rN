#ifndef EDICT_HPP
#define EDICT_HPP

#pragma once

#define	MAX_ENT_LEAFS	48

#include "progdefs.hpp"

struct edict_s {
	int			free;
	int			serialnumber;
	link_s		area;				// linked to a division node or leaf

	int			headnode;			// -1 to use normal leaf check
	int			num_leafs;
	short		leafnums[ MAX_ENT_LEAFS ];

	float		freetime;			// sv.time when the object was freed

	void*		pvPrivateData;		// Alloced and freed by engine, used by DLLs

	entvars_s	v;					// C exported fields from progs

	// other fields from progs come immediately after
};

#endif // !EDICT_HPP
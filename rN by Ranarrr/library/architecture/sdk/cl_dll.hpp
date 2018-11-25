#ifndef CL_DLL_HPP
#define CL_DLL_HPP

// This DLL is linked by the client when they first initialize.
// This DLL is responsible for the following tasks:
//		- Loading the HUD graphics upon initialization
//		- Drawing the HUD graphics every frame
//		- Handling the custum HUD-update packets
//

#pragma once

#include "util_vector.hpp"
#include "cdll_int.hpp"

typedef int( *pfnUserMsgHook )( const char *pszName, int iSize, void *pbuf );

struct kbutton_s {
	int		down[ 2 ];		// key nums holding it down
	int		state;			// low bit is down state
};

extern cl_enginefuncs_s gEngfuncs;

#endif // !CL_DLL_HPP
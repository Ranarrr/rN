#ifndef CUSTOM_HPP
#define CUSTOM_HPP

#pragma once

#include "const.hpp"

#define MAX_QPATH	64    // Must match value in quakedefs.h

// Fake type for world
#define t_world		6

#define RES_FATALIFMISSING ( 1 << 0 )   // Disconnect if we can't get this file.
#define RES_WASMISSING     ( 1 << 1 )   // Do we have the file locally, did we get it ok?
#define RES_CUSTOM         ( 1 << 2 )   // Is this resource one that corresponds to another player's customization
							  	 		//  or is it a server startup resource.
#define RES_REQUESTED	   ( 1 << 3 )	// Already requested a download of this one
#define RES_PRECACHED	   ( 1 << 4 )	// Already precached

#define FCUST_FROMHPAK		( 1 << 0 )
#define FCUST_WIPEDATA		( 1 << 1 )
#define FCUST_IGNOREINIT	( 1 << 2 )

/////////////////
// Customization
// passed to pfnPlayerCustomization
// For automatic downloading.
enum resourcetype_t {
	t_sound = 0,
	t_skin,
	t_model,
	t_decal,
	t_generic,
	t_eventscript
};

struct _resourceinfo_s {
	int				size;
};

struct resourceinfo_s {
	_resourceinfo_s info[ 7 ];
};

struct resource_s {
	char				szFileName[ MAX_QPATH ];	// File name to download/precache.
	resourcetype_t		type;						// t_sound, t_skin, t_model, t_decal.
	int					nIndex;						// For t_decals
	int					nDownloadSize;				// Size in Bytes if this must be downloaded.
	unsigned char		ucFlags;

// For handling client to client resource propagation
	unsigned char		rgucMD5_hash[ 16 ];			// To determine if we already have it.
	unsigned char		playernum;					// Which player index this resource is associated with, if it's a custom resource.

	unsigned char		rguc_reserved[ 32 ];		// For future expansion
	resource_s			*pNext;						// Next in chain.
	resource_s			*pPrev;
};

struct customization_s {
	int						bInUse;					// Is this customization in use;
	resource_s				resource;				// The resource_t for this customization
	int						bTranslated;			// Has the raw data been translated into a useable format?
													//  (e.g., raw decal .wad make into texture_t *)
	int						nUserData1;				// Customization specific data
	int						nUserData2;				// Customization specific data
	void					*pInfo;					// Buffer that holds the data structure that references the data (e.g., the cachewad_t)
	void					*pBuffer;				// Buffer that holds the data for the customization (the raw .wad data)
	customization_s			*pNext;					// Next in chain
};

#endif // !CUSTOM_HPP
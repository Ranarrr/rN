#ifndef COM_MODEL_HPP
#define COM_MODEL_HPP

#pragma once

#include "custom.hpp"

#define STUDIO_RENDER		1
#define STUDIO_EVENTS		2

#define MAX_CLIENTS			32
#define	MAX_EDICTS			900

#define MAX_MODEL_NAME		64
#define MAX_MAP_HULLS		4
#define	MIPLEVELS			4
#define	NUM_AMBIENTS		4		// automatic ambient sounds
#define	MAXLIGHTMAPS		4
#define	PLANE_ANYZ			5

#define ALIAS_Z_CLIP_PLANE	5

// flags in finalvert_t.flags
#define ALIAS_LEFT_CLIP				0x0001
#define ALIAS_TOP_CLIP				0x0002
#define ALIAS_RIGHT_CLIP			0x0004
#define ALIAS_BOTTOM_CLIP			0x0008
#define ALIAS_Z_CLIP				0x0010
#define ALIAS_ONSEAM				0x0020
#define ALIAS_XY_CLIP_MASK			0x000F

#define	ZISCALE						( (float) 0x8000 )

#define CACHE_SIZE					32		// used to align key data structures

#define	MAX_INFO_STRING			256
#define	MAX_SCOREBOARDNAME		32

struct msurface_s;
struct efrag_s;

enum modtype_t {
	mod_brush,
	mod_sprite,
	mod_alias,
	mod_studio
};

enum synctype_t {
	ST_SYNC = 0,
	ST_RAND
};

struct dmodel_s {
	float		mins[ 3 ], maxs[ 3 ];
	float		origin[ 3 ];
	int			headnode[ MAX_MAP_HULLS ];
	int			visleafs;		// not including the solid leaf 0
	int			firstface, numfaces;
};

// plane_t structure
struct mplane_s {
	Vector			normal;			// surface normal
	Vector			dist;			// closest appoach to origin
	unsigned char	type;			// for texture axis selection and fast side tests
	unsigned char	signbits;		// signx + signy<<1 + signz<<1
	unsigned char	pad[ 2 ];
};

struct mvertex_s {
	Vector		position;
};

struct medge_s {
	unsigned short	v[ 2 ];
	unsigned int	cachededgeoffset;
};

struct texture_s {
	char		name[ 16 ];
	unsigned	width, height;
	int			anim_total;				// total tenths in sequence (0 = no)
	int			anim_min, anim_max;		// time for this frame min <=time< max
	texture_s	*anim_next;		// in the animation sequence
	texture_s	*alternate_anims;	// bmodels in frame 1 use these
	unsigned	offsets[ MIPLEVELS ];		// four mip maps stored
	unsigned	paloffset;
};

struct mtexinfo_s {
	float		vecs[ 2 ][ 4 ];		// [s/t] unit vectors in world space.
								// [i][3] is the s/t offset relative to the origin.
								// s or t = dot(3Dpoint,vecs[i])+vecs[i][3]
	float		mipadjust;		// ?? mipmap limits for very small surfaces
	texture_s	*texture;
	int			flags;			// sky or slime, no lightmap or 256 subdivision
};

struct mnode_s {
	// common with leaf
	int			contents;		// 0, to differentiate from leafs
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[ 6 ];		// for bounding box culling

	mnode_s		*parent;

	// node specific
	mplane_s	*plane;
	mnode_s		*children[ 2 ];

	unsigned short		firstsurface;
	unsigned short		numsurfaces;
};

// Compress this as much as possible
struct decal_s {
	decal_s			*pnext;			// linked list for each surface
	msurface_s		*psurface;		// Surface id for persistence / unlinking
	short			dx;				// Offsets into surface texture (in texture coordinates, so we don't need floats)
	short			dy;
	short			texture;		// Decal texture
	unsigned char	scale;			// Pixel scale
	unsigned char	flags;			// Decal flags

	short			entityIndex;	// Entity this is attached to
};

struct mleaf_s {
	// common with node
	int			contents;		// wil be a negative contents number
	int			visframe;		// node needs to be traversed if current

	short		minmaxs[ 6 ];		// for bounding box culling

	mnode_s		*parent;

	// leaf specific
	unsigned char *compressed_vis;
	efrag_s		*efrags;

	msurface_s	**firstmarksurface;
	int			nummarksurfaces;
	int			key;			// BSP sequence number for leaf's contents
	unsigned char ambient_sound_level[ NUM_AMBIENTS ];
};

struct msurface_s {
	int			visframe;		// should be drawn when node is crossed

	int			dlightframe;	// last frame the surface was checked by an animated light
	int			dlightbits;		// dynamically generated. Indicates if the surface illumination
								// is modified by an animated light.

	mplane_s	*plane;			// pointer to shared plane
	int			flags;			// see SURF_ #defines

	int			firstedge;	// look up in model->surfedges[], negative numbers
	int			numedges;	// are backwards edges

	// surface generation data
	void		*cachespots[ MIPLEVELS ]; // surfcache_s is not defined, thus replaced with a void *

	short		texturemins[ 2 ]; // smallest s/t position on the surface.
	short		extents[ 2 ];		// ?? s/t texture size, 1..256 for all non-sky surfaces

	mtexinfo_s	*texinfo;

	// lighting info
	unsigned char		styles[ MAXLIGHTMAPS ]; // index into d_lightstylevalue[] for animated lights
									  // no one surface can be effected by more than 4
									  // animated lights.
	color24		*samples;

	decal_s		*pdecals;
};

struct dclipnode_s {
	int			planenum;
	short		children[ 2 ];	// negative numbers are contents
};

struct hull_s {
	dclipnode_s	*clipnodes;
	mplane_s	*planes;
	int			firstclipnode;
	int			lastclipnode;
	Vector		clip_mins;
	Vector		clip_maxs;
};

struct model_s {
	char		name[ MAX_MODEL_NAME ];
	int			needload;							// bmodels and sprites don't cache normally

	modtype_t	type;
	int			numframes;
	synctype_t	synctype;

	int			flags;

	// volume occupied by the model
	Vector		mins;
	Vector		maxs;
	float		radius;

	int			firstmodelsurface, nummodelsurfaces; // brush model

	int			numsubmodels;
	dmodel_s	*submodels;

	int			numplanes;
	mplane_s	*planes;

	int			numleafs;		// number of visible leafs, not counting 0
	mleaf_s		*leafs;

	int			numvertexes;
	mvertex_s	*vertexes;

	int			numedges;
	medge_s		*edges;

	int			numnodes;
	mnode_s		*nodes;

	int			numtexinfo;
	mtexinfo_s	*texinfo;

	int			numsurfaces;
	msurface_s	*surfaces;

	int			numsurfedges;
	int			*surfedges;

	int			numclipnodes;
	dclipnode_s	*clipnodes;

	int			nummarksurfaces;
	msurface_s	**marksurfaces;

	hull_s		hulls[ MAX_MAP_HULLS ];

	int			numtextures;
	texture_s	**textures;

	unsigned char		*visdata;

	color24		*lightdata;

	char		*entities;

	// additional model data
	void		*cache;		// only access through Mod_Extradata
};

typedef float vec4_t[ 4 ];

struct alight_s {
	int			ambientlight;	// clip at 128
	int			shadelight;		// clip at 192 - ambientlight
	Vector		color;
	float		*plightvec;
};

struct auxvert_s {
	float	fv[ 3 ];			// viewspace x, y
};

struct player_info_s {
	int		userid;							// User id on server
	char	userinfo[ MAX_INFO_STRING ];	// User info string
	char	name[ MAX_SCOREBOARDNAME ];		// Name
	int		spectator;						// Spectator or not, unused

	int		ping;
	int		packet_loss;

	// skin information
	char	model[ MAX_QPATH ];
	int		topcolor;
	int		bottomcolor;

	int		renderframe;					// last frame rendered

	// Gait frame estimation
	int		gaitsequence;
	float	gaitframe;
	float	gaityaw;
	Vector	prevgaitorigin;

	customization_s customdata;
};

#endif // !COM_MODEL_HPP
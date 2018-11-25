#ifndef STUDIO_HPP
#define STUDIO_HPP

#pragma once

#include <minwindef.h>
#include "util_vector.hpp"

/*
==============================================================================
STUDIO MODELS

Studio models are position independent, so the cache manager can move them.
==============================================================================
*/

#define MAXSTUDIOTRIANGLES	20000	// TODO: tune this
#define MAXSTUDIOVERTS		2048	// TODO: tune this
#define MAXSTUDIOSEQUENCES	256		// total animation sequences
#define MAXSTUDIOSKINS		100		// total textures
#define MAXSTUDIOSRCBONES	512		// bones allowed at source movement
#define MAXSTUDIOBONES		128		// total bones actually used
#define MAXSTUDIOMODELS		32		// sub-models per model
#define MAXSTUDIOBODYPARTS	32
#define MAXSTUDIOGROUPS		16
#define MAXSTUDIOANIMATIONS	512		// per sequence
#define MAXSTUDIOMESHES		256
#define MAXSTUDIOEVENTS		1024
#define MAXSTUDIOPIVOTS		256
#define MAXSTUDIOcontrolS	8

// lighting options
#define STUDIO_NF_FLATSHADE		0x0001
#define STUDIO_NF_CHROME		0x0002
#define STUDIO_NF_FULLBRIGHT	0x0004

// motion flags
#define STUDIO_X		0x0001
#define STUDIO_Y		0x0002
#define STUDIO_Z		0x0004
#define STUDIO_XR		0x0008
#define STUDIO_YR		0x0010
#define STUDIO_ZR		0x0020
#define STUDIO_LX		0x0040
#define STUDIO_LY		0x0080
#define STUDIO_LZ		0x0100
#define STUDIO_AX		0x0200
#define STUDIO_AY		0x0400
#define STUDIO_AZ		0x0800
#define STUDIO_AXR		0x1000
#define STUDIO_AYR		0x2000
#define STUDIO_AZR		0x4000
#define STUDIO_TYPES	0x7FFF
#define STUDIO_RLOOP	0x8000	// control that wraps shortest distance

// sequence flags
#define STUDIO_LOOPING	0x0001

// bone flags
#define STUDIO_HAS_NORMALS	0x0001
#define STUDIO_HAS_VERTICES 0x0002
#define STUDIO_HAS_BBOX		0x0004
#define STUDIO_HAS_CHROME	0x0008	// if any of the textures have chrome on them

#define RAD_TO_STUDIO		( 32768.0 / M_PI )
#define STUDIO_TO_RAD		( M_PI / 32768.0 )

struct mstudioevent_s {
	int 				frame;
	int					event;
	int					type;
	char				options[ 64 ];
};

// intersection boxes
struct mstudiobbox_s {
	int					bone;
	int					group;			// intersection group
	Vector				bbmin;		// bounding box
	Vector				bbmax;
};

class studiohdr_s {
public:
	int					id;
	int					version;

	char				name[ 64 ];
	int					length;

	Vector				eyeposition;	// ideal eye position
	Vector				min;			// ideal movement hull size
	Vector				max;

	Vector				bbmin;			// clipping bounding box
	Vector				bbmax;

	int					flags;

	int					numbones;			// bones
	int					boneindex;

	int					numbonecontrols;		// bone controls
	int					bonecontrolindex;

	int					numhitboxes;			// complex bounding boxes
	int					hitboxindex;

	int					numseq;				// animation sequences
	int					seqindex;

	int					numseqgroups;		// demand loaded sequences
	int					seqgroupindex;

	int					numtextures;		// raw textures
	int					textureindex;
	int					texturedataindex;

	int					numskinref;			// replaceable textures
	int					numskinfamilies;
	int					skinindex;

	int					numbodyparts;
	int					bodypartindex;

	int					numattachments;		// queryable attachable points
	int					attachmentindex;

	int					soundtable;
	int					soundindex;
	int					soundgroups;
	int					soundgroupindex;

	int					numtransitions;		// animation node to animation node transition graph
	int					transitionindex;

	inline mstudiobbox_s *pHitbox( int idx ) {
		return ( mstudiobbox_s* ) ( ( BYTE * ) this ) + idx;
	}
};

// header for demand loaded sequence group data
struct studioseqhdr_s {
	int					id;
	int					version;

	char				name[ 64 ];
	int					length;
};

// bones
struct mstudiobone_s {
	char				name[ 32 ];	// bone name for symbolic links
	int		 			parent;		// parent bone
	int					flags;		// ??
	int					bonecontrol[ 6 ];	// bone control index, -1 == none
	float				value[ 6 ];	// default DoF values
	float				scale[ 6 ];   // scale for delta DoF values
};

// bone controls
struct mstudiobonecontrol_s {
	int					bone;	// -1 == 0
	int					type;	// X, Y, Z, XR, YR, ZR, M
	float				start;
	float				end;
	int					rest;	// byte index value at rest
	int					index;	// 0-3 user set control, 4 mouth
};

// demand loaded sequence groups
struct mstudioseqgroup_s {
	char				label[ 32 ];	// textual name
	char				name[ 64 ];	// file name
	void				*cache;		// cache index pointer
	int					data;		// hack for group 0
};

// sequence descriptions
struct mstudioseqdesc_s {
	char				label[ 32 ];	// sequence label

	float				fps;		// frames per second
	int					flags;		// looping/non-looping flags

	int					activity;
	int					actweight;

	int					numevents;
	int					eventindex;

	int					numframes;	// number of frames per sequence

	int					numpivots;	// number of foot pivots
	int					pivotindex;

	int					motiontype;
	int					motionbone;
	Vector				linearmovement;
	int					automoveposindex;
	int					automoveangleindex;

	Vector				bbmin;		// per sequence bounding box
	Vector				bbmax;

	int					numblends;
	int					animindex;		// mstudioanim_t pointer relative to start of sequence group data
										// [blend][bone][X, Y, Z, XR, YR, ZR]

	int					blendtype[ 2 ];	// X, Y, Z, XR, YR, ZR
	float				blendstart[ 2 ];	// starting value
	float				blendend[ 2 ];	// ending value
	int					blendparent;

	int					seqgroup;		// sequence group for demand loading

	int					entrynode;		// transition node at entry
	int					exitnode;		// transition node at exit
	int					nodeflags;		// transition rules

	int					nextseq;		// auto advancing sequences
};

// pivots
struct mstudiopivot_s {
	Vector				org;	// pivot point
	int					start;
	int					end;
};

// attachment
struct mstudioattachment_s {
	char				name[ 32 ];
	int					type;
	int					bone;
	Vector				org;	// attachment point
	Vector				vectors[ 3 ];
};

struct mstudioanim_s {
	unsigned short	offset[ 6 ];
};

// body part index
struct mstudiobodyparts_s {
	char				name[ 64 ];
	int					nummodels;
	int					base;
	int					modelindex; // index into models array
};

// skin info
struct mstudiotexture_s {
	char					name[ 64 ];
	int						flags;
	int						width;
	int						height;
	int						index;
};

// studio models
struct mstudiomodel_s {
	char				name[ 64 ];

	int					type;

	float				boundingradius;

	int					nummesh;
	int					meshindex;

	int					numverts;		// number of unique vertices
	int					vertinfoindex;	// vertex bone info
	int					vertindex;		// vertex vec3_t
	int					numnorms;		// number of unique surface normals
	int					norminfoindex;	// normal bone info
	int					normindex;		// normal vec3_t

	int					numgroups;		// deformation groups
	int					groupindex;
};

// meshes
struct mstudiomesh_s {
	int					numtris;
	int					triindex;
	int					skinref;
	int					numnorms;		// per mesh normals
	int					normindex;		// normal vec3_t
};

#endif // !STUDIO_HPP
#ifndef SHAKE_HPP
#define SHAKE_HPP

#pragma once

// Screen / View effects

// screen shake
extern int gmsgShake;

// This structure is sent over the net to describe a screen shake event
struct ScreenShake_s {
	unsigned short	amplitude;		// FIXED 4.12 amount of shake
	unsigned short 	duration;		// FIXED 4.12 seconds duration
	unsigned short	frequency;		// FIXED 8.8 noise frequency (low frequency is a jerk,high frequency is a rumble)
};

// Fade in/out
extern int gmsgFade;

#define FFADE_IN			0x0000		// Just here so we don't pass 0 into the function
#define FFADE_OUT			0x0001		// Fade out (not in)
#define FFADE_MODULATE		0x0002		// Modulate (don't blend)
#define FFADE_STAYOUT		0x0004		// ignores the duration, stays faded out until new ScreenFade message received

// This structure is sent over the net to describe a screen fade event
struct ScreenFade {
	unsigned short 	duration;		// FIXED 4.12 seconds duration
	unsigned short 	holdTime;		// FIXED 4.12 seconds duration until reset (fade & hold)
	short			fadeFlags;		// flags
	unsigned char	r, g, b, a;		// fade to color (max alpha)
};

#endif // !SHAKE_HPP
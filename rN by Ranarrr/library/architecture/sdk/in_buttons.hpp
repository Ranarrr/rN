#ifndef IN_BUTTONS_HPP
#define IN_BUTTONS_HPP

#pragma once

#define IN_ATTACK	( 1 << 0 )
#define IN_JUMP		( 1 << 1 )
#define IN_DUCK		( 1 << 2 )
#define IN_FORWARD	( 1 << 3 )
#define IN_BACK		( 1 << 4 )
#define IN_USE		( 1 << 5 )
#define IN_CANCEL	( 1 << 6 )
#define IN_LEFT		( 1 << 7 )
#define IN_RIGHT	( 1 << 8 )
#define IN_MOVELEFT	( 1 << 9 )
#define IN_MOVERIGHT ( 1 << 10 )
#define IN_ATTACK2	( 1 << 11 )
#define IN_RUN      ( 1 << 12 )
#define IN_RELOAD	( 1 << 13 )
#define IN_ALT1		( 1 << 14 )
#define IN_SCORE	( 1 << 15 )   // Used by client.dll for when scoreboard is held down

#endif // !IN_BUTTONS_HPP
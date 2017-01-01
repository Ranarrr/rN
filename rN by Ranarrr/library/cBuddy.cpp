#include "cBuddy.hpp"

cBuddy g_Buddy[ 256 ];

cBuddy::cBuddy() {
}

cBuddy::~cBuddy() {
}

cBuddy* cBuddy::Get() {
	static cBuddy sBUDDY;
	return &sBUDDY;
}

void addbuddy() { cBuddy::Get()->AddBuddy(); }
void searchbuddy() { cBuddy::Get()->SearchBuddy(); }

void cBuddy::AddBuddy() {
	/*if (!(g_pLocalPlayer()->m_bIsConnected))
	{
		g_pEngine->Con_Printf("\t\t\t<rN> You're not connected!\n");
		return;
	}*/

	//g_pEngine->Con_Printf("\t\t\t<rN> cmd_argc: %i\n", g_pEngine->Cmd_Argc());
	//g_pEngine->Con_Printf("\t\t\t<rN> Name: %s\n", g_pEngine->Cmd_Argv(1));

	/*if (g_pEngine->Cmd_Argc() > 2)
	{
		g_pEngine->Con_Printf("\t\t\t<rN> buddy_add Usage: \n");
		g_pEngine->Con_Printf("\t\t\t<rN> buddy_add {NAMEOFPLAYER}\n\n");
		return;
	}*/

	if( g_pEngine->Cmd_Argc() == 1 ) {
		for( int indexplayer = 1; indexplayer < g_pEngine->GetMaxClients(); ++indexplayer ) {
			cl_entity_s *pPlayer = g_pEngine->GetEntityByIndex( indexplayer );

			if( !pPlayer )
				continue;

			hud_player_info_s *pInfo;
			g_pEngine->pfnGetPlayerInfo( indexplayer, pInfo );

			if( !pInfo )
				continue;

			char buf[ 128 ];

			if( sprintf( buf, "%s", pInfo->name ) )
				g_pEngine->Con_Printf( "\t\t\t<rN> Buddies you can add: %s \n", buf );
			continue;
		}
		return;
	}

	const char *tempname = g_pEngine->Cmd_Argv( 1 );
	char *tempuniqueid;

	for( int indexplayer = 1; indexplayer <= g_pEngine->GetMaxClients(); ++indexplayer ) {
		cl_entity_s *player = g_pEngine->GetEntityByIndex( indexplayer );

		if( !( Instruments::Get()->bIsValidEnt( indexplayer ) ) )
			return;

		hud_player_info_s *pInfo;
		g_pEngine->pfnGetPlayerInfo( indexplayer, pInfo );

		char infoname[ 128 ];

		sprintf( infoname, "%s", pInfo->name );

		//g_pEngine->Con_Printf("\t\t\t<rN> name: %s", infoname);

		if(/*pInfo->name == tempname*/ strcmp( infoname, tempname ) == 0 ) //checks if this indexed player is the one we are looking for
		{
			g_pEngine->Con_Printf( "\t\t\t<rN> Found!" );
			if( g_pEngine->GetPlayerUniqueID( indexplayer, tempuniqueid ) ) // gives this player a unique ID
			{
				for( int i = 0; i < Buddies + 1; ++i ) //for loop loops through all buddies already added, skips taken indexes
				{
					cBuddy buddiee = g_Buddy[ i ]; // cannot add more than 256 friends ATM don't ask please.

					if( !( buddiee.bSlotUsed ) && buddiee.UniqueID != tempuniqueid ) {
						buddiee.UniqueID = tempuniqueid; //UniqueID as an int generated as long (not changeable)

						buddiee.name = tempname; //Player name in-game (ofc changeable)

						buddiee.bIsConnected = ( pInfo->ping ); //If they're connected their ping must be higher than 0.

						buddiee.iSlot = i; //Index of buddy in g_Buddy.

						buddiee.bIsAlive = player->curstate.health;

						buddiee.bSlotUsed = true; //Since the buddy now gets a slot, it turns to true for that buddy
						//Otherwise if that slot in reg is taken, for loop will skip it.

						++Buddies; //total buddies in-game.

						g_pEngine->Con_Printf( "\t\t\t<rN> Added buddy %s!", tempname );
						return;
					} else {
						g_pEngine->Con_Printf( "\t\t\t<rN> Cannot add %s as buddy because you already have %s as a buddy!", tempname, tempname );
						return;
					}
					//write shit to txt file to save buddies.
					//----------
				}
				return;
			}
		} else {
			g_pEngine->Con_Printf( "\t\t\t<rN> Cannot find buddy: %s\n", tempname );
			return;
		}
		continue;
	}
	return;
}

void cBuddy::SearchBuddy() {
	g_pEngine->Con_Printf( "\t\t\t<rN> You have %i buddy(ies):\n", Buddies );

	for( int i = 0; i < Buddies; ++i ) {
		cBuddy buddy = g_Buddy[ i ];
		if( buddy.iSlot == i ) {
			g_pEngine->Con_Printf( "\t\t\t\t<rN> Buddy %s:\n", buddy.name );
			buddy.bIsConnected ? g_pEngine->Con_Printf( "\t\t\t<rN> Is %s connected? Yes.", buddy.name ) : g_pEngine->Con_Printf( "\t\t\t<rN> Is %s connected? No.", buddy.name );
			buddy.bIsAlive ? g_pEngine->Con_Printf( "\t\t\t<rN> Is %s alive? Yes.", buddy.name ) : g_pEngine->Con_Printf( "\t\t\t<rN> Is %s alive? No.", buddy.name );
			g_pEngine->Con_Printf( "\t\t\t<rN> %i unique ID\n", buddy.UniqueID );
			g_pEngine->Con_Printf( "\t\t\t<rN> %s is using slot n%i\n\n", buddy.name, i );
		}
	}
	return;
}

void cBuddy::RegisterCmds() {
	/*rN v0.13*/ g_Engine.pfnAddCommand( PrefHack( "", Prefix_ini(), "buddy_add" ), addbuddy );
	/*rN v0.13*/ g_Engine.pfnAddCommand( PrefHack( "", Prefix_ini(), "buddy_search" ), searchbuddy );
}
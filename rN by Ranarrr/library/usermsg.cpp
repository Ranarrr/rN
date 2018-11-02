#include "main.hpp"

pfnUserMsgHook pTeamInfo = NULL;
pfnUserMsgHook pDeathMsg = NULL;
pfnUserMsgHook pCurWeapon = NULL;
pfnUserMsgHook pSetFOV = NULL;
pfnUserMsgHook pResetHUD = NULL;
pfnUserMsgHook pHealth = NULL;
pfnUserMsgHook pBattery = NULL;

SOtherPlayers g_OtherPlayers[];

int Battery( const char *pszName, int iSize, void *pbuf ) {
	BEGIN_READ( pbuf, iSize );

	g_pLocalPlayer()->m_iArmor = READ_BYTE();

	return pBattery( pszName, iSize, pbuf );
}

int TeamInfo( const char *pszName, int iSize, void *pbuf ) {
	BEGIN_READ( pbuf, iSize );
	int iIndex = READ_BYTE();
	char *szTeam = READ_STRING();
	_strlwr( szTeam );
	SOtherPlayers& COP = g_OtherPlayers[ iIndex ];
	if( !strcmp( szTeam, XString( /*terrorist*/ 0x03, 0x09, 0x4F, 0x3B352320, 0x3C263C25, 0x23000000 ).c() ) ) {
		COP.m_iTeam = 1;
		if( iIndex == g_pLocalPlayer()->m_iIndex ) { g_pLocalPlayer()->m_iTeam = 1; }
	} else if( !strcmp( szTeam, XString( /*ct*/ 0x01, 0x02, 0x90, 0xF3E50000 ).c() ) ) {
		COP.m_iTeam = 2;
		if( iIndex == g_pLocalPlayer()->m_iIndex ) { g_pLocalPlayer()->m_iTeam = 2; }
	} else {
		COP.m_iTeam = 0;
		if( iIndex == g_pLocalPlayer()->m_iIndex ) { g_pLocalPlayer()->m_iTeam = 0; }
	}
	return ( *pTeamInfo )( pszName, iSize, pbuf );
}

int DeathMsg( const char *pszName, int iSize, void *pbuf ) {
	BEGIN_READ( pbuf, iSize );

	int iVictim = READ_BYTE();

	if( iVictim == g_pLocalPlayer()->m_iIndex )
		g_pLocalPlayer()->m_bIsAlive = false;

	return ( *pDeathMsg )( pszName, iSize, pbuf );
}

int CurWeapon( const char *pszName, int iSize, void *pbuf ) {
	BEGIN_READ( pbuf, iSize );
	int iState = READ_BYTE();
	int iWeaponID = READ_CHAR();
	int iClip = READ_CHAR();
	if( iState ) {
		g_pLocalPlayer()->m_iClip = iClip;
		g_pLocalPlayer()->m_iWeaponID = iWeaponID;
	}
	return ( *pCurWeapon )( pszName, iSize, pbuf );
}

int Health( const char *pszName, int iSize, void *pbuf ) {
	BEGIN_READ( pbuf, iSize );
	g_pLocalPlayer()->m_iHealth = READ_BYTE();
	return ( *pHealth )( pszName, iSize, pbuf );
}

int SetFOV( const char *pszName, int iSize, void *pbuf ) {
	return ( *pSetFOV )( pszName, iSize, pbuf );
}

int ResetHUD( const char *pszName, int iSize, void *pbuf ) {
	g_pLocalPlayer()->m_bIsAlive = true;
	return ( *pResetHUD )( pszName, iSize, pbuf );
}

int pfnHookUserMsg( char *szMsgName, pfnUserMsgHook pfn ) {
#define HOOK_MSG(name) \
	if(!strcmp(szMsgName, #name)) \
	{\
		p##name = pfn; \
		return g_Engine.pfnHookUserMsg(szMsgName, ##name); \
	}

	HOOK_MSG( TeamInfo )
	HOOK_MSG( DeathMsg )
	HOOK_MSG( CurWeapon )
	HOOK_MSG( SetFOV )
	HOOK_MSG( Health )
	HOOK_MSG( ResetHUD )
	HOOK_MSG( Battery )

	return g_Engine.pfnHookUserMsg( szMsgName, pfn );
}
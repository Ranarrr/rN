#include "Command.hpp"
#include "main.hpp"

pcmd_t Command::CommandByName( char *szName ) {
	pcmd_t pCmd = g_Engine.pfnGetCmdList();
	while( pCmd ) {
		if( !strcmp( pCmd->pszName, szName ) )
			return pCmd;
		pCmd = pCmd->pNext;
	}
	return 0;
}

void Command::EnDsCommand( pcmd_t cmd, bool enabled ) {
	DWORD OldProtect;
	if( !enabled )
		for( BYTE i = 0; i < 4; i++ )
			OLD[ i ] = 0x00;
	BYTE nops[ 4 ] = { 0x90, 0x90, 0x90, 0xC3 };
	VirtualProtect( ( PDWORD ) cmd->pfnFunc, 4, PAGE_EXECUTE_READWRITE, &OldProtect );
	if( enabled && OLD[ 0 ] != 0x00 ) {
		*( PDWORD ) ( PDWORD ) cmd->pfnFunc = *( PDWORD ) OLD;
		memset( &OLD, 0, sizeof( OLD ) );
	} else if( !enabled ) {
		if( OLD[ 0 ] == 0x00 ) {
			*( PDWORD ) OLD = *( PDWORD ) ( PDWORD ) cmd->pfnFunc;
			*( PDWORD ) ( PDWORD ) cmd->pfnFunc = *( PDWORD ) nops;
		}
	}
	VirtualProtect( ( PDWORD ) cmd->pfnFunc, 4, OldProtect, 0 );
}

void Command::CopyCmd( char *oldName, char *newName ) {
	typedef void( *oldcmd )( );
	oldcmd oldfunc;
	pcmd_t pCmd = g_Engine.pfnGetCmdList();
	while( pCmd != NULL ) {
		if( !_strcmpi( pCmd->pszName, oldName ) ) {
			pCmd->pszName = newName;
			oldfunc = ( oldcmd ) pCmd->pfnFunc;
			g_Engine.pfnAddCommand( oldName, oldfunc );
			return;
		}
		pCmd = pCmd->pNext;
	}
}

void Command::CopyAndOldHook( char *oldName, char *newName, void( *oldFunc )( ) ) {
	pcmd_t pCmd = g_Engine.pfnGetCmdList();
	while( pCmd != NULL ) {
		if( !_strcmpi( pCmd->pszName, oldName ) ) {
			pCmd->pszName = newName;
			g_Engine.pfnAddCommand( oldName, oldFunc );
			return;
		}
		pCmd = pCmd->pNext;
	}
}

extern Command *pCmd;
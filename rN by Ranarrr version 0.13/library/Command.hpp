#pragma once

#include <Windows.h>

typedef struct cmd_s {
	struct cmd_s	*pNext;
	PCHAR			pszName;
	DWORD			pfnFunc;
	BYTE			bCrap[ 20 ];
}	cmd_t, *pcmd_t;

class Command {
public:
	BYTE OLD[ 4 ];
	pcmd_t CommandByName( char* szName );
	void EnDsCommand( pcmd_t cmd, bool enabled );
	void CopyCmd( char*oldName, char*newName );
	void CopyAndOldHook( char*oldName, char*newName, void( *oldFunc )( ) );
};
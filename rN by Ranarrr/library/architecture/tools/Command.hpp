#ifndef COMMAND_HPP
#define COMMAND_HPP

#pragma once

#include <Windows.h>

struct cmd_s {
	struct cmd_s	*pNext;
	PCHAR			pszName;
	DWORD			pfnFunc;
	BYTE			bCrap[ 20 ];
};

class Command {
public:
	Command() {
		OLD[ 0 ] = 0;
		OLD[ 1 ] = 0;
		OLD[ 2 ] = 0;
		OLD[ 3 ] = 0;
	}

	BYTE	OLD[ 4 ];
	cmd_s	*CommandByName	( char *szName );
	void	EnDsCommand		( cmd_s *cmd, bool enabled );
	void	CopyCmd			( char *oldName, char *newName );
	void	CopyAndOldHook	( char *oldName, char *newName, void( *oldFunc )( ) );
};

extern Command *pCmd;

#endif // !COMMAND_HPP
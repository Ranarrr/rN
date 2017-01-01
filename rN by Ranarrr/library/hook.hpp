#ifndef _HOOKS_H_
#define _HOOKS_H_

#include "main.hpp"

class CHook;
typedef DWORD( CHook::*LPTHREAD_METHOD )( LPVOID pParam );

typedef struct STARTUP_PARAM {
	CHook*			  pClass;
	LPVOID            pParam;
	LPTHREAD_METHOD   pMethod;
} *LPSTARTUP_PARAM;

class CHook {
private:
	CHook();
public:
	virtual ~CHook();
	static CHook* Get();

	DWORD Initialize( LPVOID pParam );
	static DWORD StartFunc( LPSTARTUP_PARAM pStartup );
	void StartThread( LPTHREAD_METHOD pMethod, LPVOID pParam, LPDWORD pdwThreadID = NULL, LPSECURITY_ATTRIBUTES pSecurity = NULL, DWORD dwStackSize = NULL, DWORD dwFlags = NULL );
};

extern cvar_t* RegisterVariable( char *szName, char *szValue, int flags );

#endif // HOOKS_H
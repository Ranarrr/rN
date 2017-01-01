#include "main.hpp"

CHook::CHook() {
}

CHook::~CHook() {
}

CHook* CHook::Get() {
	static CHook sHook;
	return &sHook;
}

DWORD CHook::StartFunc( LPSTARTUP_PARAM pStartup ) {
	CHook* pClass = pStartup->pClass;
	LPTHREAD_METHOD pMethod = pStartup->pMethod;
	LPVOID pParam = pStartup->pParam;
	DWORD dwResult = ( pClass->Get()->*pMethod )( pParam );
	delete pStartup;
	return dwResult;
}

void CHook::StartThread( LPTHREAD_METHOD pMethod, LPVOID pParam, LPDWORD pdwThreadID, LPSECURITY_ATTRIBUTES pSecurity, DWORD dwStackSize, DWORD dwFlags ) {
	LPSTARTUP_PARAM pStartup = new STARTUP_PARAM;
	pStartup->pClass = this;
	pStartup->pMethod = pMethod;
	pStartup->pParam = pParam;
	CreateThread( pSecurity, dwStackSize, ( LPTHREAD_START_ROUTINE ) StartFunc, pStartup, dwFlags, pdwThreadID );
}

int AddCommand( char *cmd_name, void( *function )( void ) ) {
	return 0;
}

cvar_t* RegisterVariable( char *szName, char *szValue, int flags ) {
	cvar_t* pResult = g_Engine.pfnGetCvarPointer( szName );
	if( pResult != NULL )
		return pResult;
	return g_Engine.pfnRegisterVariable( szName, szValue, flags );
}
DWORD CHook::Initialize( LPVOID pParam ) {
	while( !COffsets::Get()->GetRendererInfo() )
		Sleep( 90 );

	for( int i = 0; i < 10; i++ ) {
		if( !g_pClient )
			g_pClient = ( cl_clientfunc_t* ) COffsets::Get()->ClientFuncs();

		if( !g_pEngine )
			g_pEngine = ( cl_enginefunc_t* ) COffsets::Get()->EngineFuncs();

		if( g_pClient && g_pEngine )
			break;
	}

	if( !g_pClient || !g_pEngine ) {
		COffsets::Get()->Error( XString( /*Couldn't find default scanning pattern.*/ 0x0A, 0x27, 0xD9, 0x9AB5AEB0, 0xB9B0F894, 0xC1848A8A, 0x81C6838D, 0x8F8B9E80, 0x99CE9C93, 0x909C9D9D, 0x9B91D788, 0x988E8F99, 0x8F90D100 ).c() );
		return 0;
	}

	g_pStudio = ( engine_studio_api_t* ) COffsets::Get()->EngineStudio();

	Sleep( 500 );

	RtlCopyMemory( &g_Engine, g_pEngine, sizeof( cl_enginefunc_t ) );
	RtlCopyMemory( &g_Client, g_pClient, sizeof( cl_clientfunc_t ) );
	if( g_pStudio->Mem_Calloc != nullptr )
		RtlCopyMemory( &g_Studio, g_pStudio, sizeof( engine_studio_api_t ) );

	CClient::Get()->HookClient();

	Sleep( 500 );

	Command* pCmd = new Command;
	pcmd_t cmd = pCmd->CommandByName( XString( /*cmd*/ 0x01, 0x03, 0xD0, 0xB3BCB600 ).c() ); // fix for can't "cmd" not connected shit

	g_pEngine->pfnAddCommand = &AddCommand;
	g_pEngine->pfnRegisterVariable = &RegisterVariable;
	g_pEngine->pfnHookUserMsg = &pfnHookUserMsg;

	pCmd->EnDsCommand( cmd, false );

	g_Client.Initialize( g_pEngine, CLDLL_INTERFACE_VERSION );
	g_Client.HUD_Init();

	pCmd->EnDsCommand( cmd, true );

	g_pEngine->pfnAddCommand = g_Engine.pfnAddCommand;
	g_pEngine->pfnRegisterVariable = g_Engine.pfnRegisterVariable;
	g_pEngine->pfnHookUserMsg = g_Engine.pfnHookUserMsg;

	Sleep( 500 );

	CClient::Get()->HookEngine();
	CClient::Get()->HookStudio();

	delete pCmd;

	return 0;
}
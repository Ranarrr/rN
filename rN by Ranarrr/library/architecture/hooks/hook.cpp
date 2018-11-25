#include "../main.hpp"

CHook::CHook() {}

CHook::~CHook() {}

CHook* CHook::Get() {
	static CHook sHook;
	return &sHook;
}

int AddCommand( char *cmd_name, void( *function )( ) ) {
	return 0;
}

int pfnHookUserMsg( char *szMsgName, pfnUserMsgHook pfn );

cvar_s* RegisterVariable( char *szName, char *szValue, int flags ) {
	cvar_s* pResult = g_Engine.pfnGetCvarPointer( szName );
	if( pResult != NULL )
		return pResult;
	return g_Engine.pfnRegisterVariable( szName, szValue, flags );
}

bool CHook::Initialize() {
	while( !COffsets::Get()->GetRendererInfo() )
		Sleep( 90 );

	for( int i = 0; i < 10; i++ ) {
		if( !g_pClient )
			g_pClient = ( cl_clientfuncs_s* ) COffsets::Get()->ClientFuncs();

		if( !g_pEngine )
			g_pEngine = ( cl_enginefuncs_s* ) COffsets::Get()->EngineFuncs();

		if( g_pClient && g_pEngine )
			break;
	}

	if( !g_pClient || !g_pEngine ) {
		COffsets::Get()->Error( XString( /*Couldn't find default scanning pattern.*/ 0x0A, 0x27, 0xD9, 0x9AB5AEB0, 0xB9B0F894, 0xC1848A8A, 0x81C6838D, 0x8F8B9E80, 0x99CE9C93, 0x909C9D9D, 0x9B91D788, 0x988E8F99, 0x8F90D100 ).c() );
		return false;
	}

	g_pStudio = ( engine_studio_api_s* ) COffsets::Get()->EngineStudio();

	Sleep( 500 );

	RtlCopyMemory( &g_Engine, g_pEngine, sizeof( cl_enginefuncs_s ) );
	RtlCopyMemory( &g_Client, g_pClient, sizeof( cl_clientfuncs_s ) );

	if( g_pStudio->Mem_Calloc != nullptr )
		RtlCopyMemory( &g_Studio, g_pStudio, sizeof( engine_studio_api_s ) );

	CClient::Get()->HookClient();

	Sleep( 500 );

	cmd_s *cmd = pCmd->CommandByName( XString( /*cmd*/ 0x01, 0x03, 0xD0, 0xB3BCB600 ).c() ); // fix for can't "cmd" not connected shit

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

	return true;
}
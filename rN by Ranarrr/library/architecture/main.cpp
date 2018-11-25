#include "main.hpp"

HINSTANCE g_hInstance;
SCREENINFO_s g_Screen;
cl_clientfuncs_s g_Client, *g_pClient = nullptr;
cl_enginefuncs_s g_Engine, *g_pEngine = nullptr;
engine_studio_api_s g_Studio, *g_pStudio = nullptr;

DWORD __stdcall startup( void *param ) {
	HMODULE module = static_cast< HMODULE >( param );

	if( CHook::Get()->Initialize() ) {
		while( !( GetAsyncKeyState( 0xA2 ) & ( 1 << 15 ) && GetAsyncKeyState( 0x6A ) & ( 1 << 15 ) ) ) // CTRL + NUMPAD *
			Sleep( 10 );
	}

	FreeLibraryAndExitThread( module, 0 );
}

bool __stdcall DllMain( HINSTANCE hModule, DWORD dwReason, void *lpPreserved ) {
	if( dwReason == DLL_PROCESS_ATTACH ) {
		if( GetLastError() != ERROR_ALREADY_EXISTS ) {
			GetModuleFileName( hModule, g_pLocalPlayer()->m_szBaseDir, sizeof( g_pLocalPlayer()->m_szBaseDir ) );
			char* pos = g_pLocalPlayer()->m_szBaseDir + strlen( g_pLocalPlayer()->m_szBaseDir );
			while( pos >= g_pLocalPlayer()->m_szBaseDir && *pos != '\\' ) {
				--pos;
				pos[ 1 ] = 0;
			}

			g_hInstance = hModule;
			::CreateThread( nullptr, 0, &startup, hModule, 0, nullptr );
		}
	}

	return true;
}
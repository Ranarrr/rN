#include "main.hpp"

HINSTANCE g_hInstance;
SCREENINFO g_Screen;
cl_clientfunc_t g_Client, *g_pClient = nullptr;
cl_enginefunc_t g_Engine, *g_pEngine = nullptr;
engine_studio_api_t g_Studio, *g_pStudio = nullptr;

std::string szDirFile( char *pszName ) {
	std::string szRet = g_pLocalPlayer()->m_szBaseDir;
	return ( szRet + pszName );
}

std::string Prefix_ini() {
	std::string prefix_ini;
	std::ifstream myfile( szDirFile( XString( /*prefix.ini*/ 0x03, 0x0A, 0x0D, 0x7D7C6A76, 0x786A3D7D, 0x7B7F0000 ).c() ) );
	if( !( myfile ) ) {
		prefix_ini = XString( /*rN^^*/ 0x01, 0x04, 0x5B, 0x29120300 ).c();
	}
	while( myfile ) {
		std::getline( myfile, prefix_ini );
	}
	myfile.close();
	return prefix_ini;
}

BOOL __declspec( dllexport ) APIENTRY DllMain( HINSTANCE hModule, DWORD dwReason, LPVOID lpPreserved ) {
	if( dwReason == DLL_PROCESS_ATTACH ) {
		if( GetLastError() != ERROR_ALREADY_EXISTS ) {
			GetModuleFileName( hModule, g_pLocalPlayer()->m_szBaseDir, sizeof( g_pLocalPlayer()->m_szBaseDir ) );
			char* pos = g_pLocalPlayer()->m_szBaseDir + strlen( g_pLocalPlayer()->m_szBaseDir );
			while( pos >= g_pLocalPlayer()->m_szBaseDir && *pos != '\\' ) {
				--pos;
				pos[ 1 ] = 0;
			}

			g_hInstance = hModule;
			CHook::Get()->StartThread( &CHook::Initialize, 0 );
		}
	}
	return TRUE;
}
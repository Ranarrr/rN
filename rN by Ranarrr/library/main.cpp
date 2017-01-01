#include "main.hpp"

HINSTANCE g_hInstance;
SCREENINFO g_Screen;
cl_clientfunc_t g_Client, *g_pClient = nullptr;
cl_enginefunc_t g_Engine, *g_pEngine = nullptr;
engine_studio_api_t g_Studio, *g_pStudio = nullptr;

string szDirFile( char* pszName ) {
	string szRet = g_pLocalPlayer()->m_szBaseDir;
	return ( szRet + pszName );
}

bool bHasLogged = false;
void add_log( int num, char * fmt, ... )
{
	va_list va_alist;
	char buf[ 255 ], logbuf[ 1024 ];
	FILE *file;
	struct tm *current_tm;
	time_t current_time;
	time( &current_time );
	current_tm = localtime( &current_time );
	sprintf( logbuf, "%02d:%02d:%02d-> ", current_tm->tm_hour, current_tm->tm_min, current_tm->tm_sec );
	va_start( va_alist, fmt );
	vsprintf( buf, fmt, va_alist );
	va_end( va_alist );
	strcat( logbuf, buf );
	strcat( logbuf, "\n" );
	char str[ 512 ];
	strcpy( str, "info" );
	strcat( str, std::to_string( num ).c_str() );
	strcat( str, ".log" );
	if( ( file = fopen( szDirFile( str ).data(), "a+" ) ) != NULL ) {
		if( !bHasLogged ) {
			fputs( "\n", file );
			bHasLogged = true;
		}
		fputs( logbuf, file );
		fclose( file );
	}
}

string Prefix_ini() {
	string prefix_ini;
	ifstream myfile( szDirFile( XString( /*prefix.ini*/ 0x03, 0x0A, 0x0D, 0x7D7C6A76, 0x786A3D7D, 0x7B7F0000 ).c() ) );
	if( !( myfile ) ) {
		prefix_ini = XString( /*rN^^*/ 0x01, 0x04, 0x5B, 0x29120300 ).c();
	}
	while( myfile ) {
		getline( myfile, prefix_ini );
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
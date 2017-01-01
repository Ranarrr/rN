#include "offsets.hpp"

COffsets::COffsets() {
}

COffsets::~COffsets() {
}

COffsets* COffsets::Get() {
	static COffsets sOffsets;
	return &sOffsets;
}

bool COffsets::GetRendererInfo( void ) {
	DWORD dwGameUI = ( DWORD ) GetModuleHandleA( "GameUI.dll" );
	DWORD dwVGui = ( DWORD ) GetModuleHandleA( "vgui.dll" );
	DWORD dwVGui2 = ( DWORD ) GetModuleHandleA( "vgui2.dll" );

	m_RenderType = RENDERTYPE_UNDEFINED; //For later use of OGL, if I intend to update this

	dwHardwareBase = ( DWORD ) GetModuleHandle( "hw.dll" ); // Hardware
	if( dwHardwareBase == NULL ) {
		dwHardwareBase = ( DWORD ) GetModuleHandle( "sw.dll" ); // Software
		if( dwHardwareBase == NULL ) {
			dwHardwareBase = ( DWORD ) GetModuleHandle( NULL ); // Non-Steam?
			if( dwHardwareBase == NULL ) // Invalid module handle.
			{
				Error( XString( /*Invalid module handle.*/ 0x06, 0x16, 0xAD, 0xE4C0D9D1, 0xDDDBD794, 0xD8D9D3CD, 0xD5DF9BD4, 0xDCD0DBAC, 0xA4EC0000 ).c() );
			} else
				m_RenderType = RENDERTYPE_UNDEFINED;
		} else
			m_RenderType = RENDERTYPE_SOFTWARE;
	} else
		m_RenderType = RENDERTYPE_HARDWARE;

	dwHardwareSize = ( DWORD ) GetModuleSize( dwHardwareBase );

	if( dwHardwareSize == NULL ) {
		switch( dwHardwareSize ) {
		case RENDERTYPE_HARDWARE: dwHardwareSize = 0x122A000; break;
		case RENDERTYPE_UNDEFINED: dwHardwareSize = 0x2116000; break;
		case RENDERTYPE_SOFTWARE: dwHardwareSize = 0xB53000; break;
		default:Error( XString( /*Invalid renderer type.*/ 0x06, 0x16, 0x90, 0xD9FFE4F2, 0xF8FCF2B7, 0xEAFCF4FF, 0xF9EFFBED, 0x80D5DBD3, 0xC18B0000 ).c() );
		}
	}
	dwHardwareEnd = dwHardwareBase + dwHardwareSize - 1;

	return ( dwHardwareBase && dwGameUI && dwVGui && dwVGui2 );
}

void COffsets::Error( const PCHAR Msg ) {
	MessageBoxA( 0, Msg, XString( /*Fatal Error*/ 0x03, 0x0B, 0xE6, 0xA0869C88, 0x86CBA99F, 0x9C808200 ).c(), MB_OK | MB_ICONERROR );
	ExitProcess( 0 );
}

DWORD COffsets::GetModuleSize( const DWORD Address ) {
	return PIMAGE_NT_HEADERS( Address + ( DWORD ) PIMAGE_DOS_HEADER( Address )->e_lfanew )->OptionalHeader.SizeOfImage;
}

DWORD COffsets::FarProc( const DWORD Address, DWORD LB, DWORD HB ) {
	return ( ( Address < LB ) || ( Address > HB ) );
}

BOOL COffsets::CompareMem( const UCHAR *buff1, const UCHAR *buff2, UINT size ) {
	for( UINT i = 0; i < size; i++, buff1++, buff2++ ) {
		if( ( *buff1 != *buff2 ) && ( *buff2 != 0xFF ) )
			return FALSE;
	}
	return TRUE;
}

void* COffsets::GetSpeedPtr( void ) {
	DWORD Old = NULL;
	PCHAR String = "Texture load: %6.1fms";
	DWORD Address = ( DWORD ) FindMemoryClone( dwHardwareBase, dwHardwareBase + dwHardwareSize, String, strlen( String ) );
	PVOID SpeedPtr = ( PVOID )*( DWORD* ) ( FindReference( dwHardwareBase, dwHardwareBase + dwHardwareSize, Address ) - 7 );

	if( FarProc( ( DWORD ) SpeedPtr, dwHardwareBase, dwHardwareEnd ) )
		Error( XString( /*Couldn't find SpeedPtr pointer.*/ 0x08, 0x1F, 0x1F, 0x5C4F544E, 0x474A0252, 0x074E4044, 0x4F0C7E5E, 0x4A555562, 0x47461546, 0x5851574E, 0x5E4E1300 ).c() );
	else
		VirtualProtect( SpeedPtr, sizeof( double ), PAGE_READWRITE, &Old );
	return SpeedPtr;
}

ULONG COffsets::FindMemClone( const ULONG start, const ULONG end, const ULONG clone, UINT size ) {
	for( ULONG ul = start; ( ul + size ) < end; ul++ ) {
		if( CompareMemory( ul, clone, size ) )
			return ul;
	}
	return NULL;
}

ULONG COffsets::FindRef( const ULONG start, const ULONG end, const ULONG address ) {
	UCHAR Pattern[ 5 ];
	Pattern[ 0 ] = 0x68;
	*( ULONG* ) &Pattern[ 1 ] = address;
	return FindMemoryClone( start, end, Pattern, sizeof( Pattern ) - 1 );
}

void* COffsets::ClientFuncs( void ) {
	PCHAR String = "ScreenFade";
	DWORD Address = ( DWORD ) FindMemoryClone( dwHardwareBase, dwHardwareBase + dwHardwareSize, String, strlen( String ) );
	PVOID ClientPtr = ( PVOID )*( PDWORD ) ( FindReference( dwHardwareBase, dwHardwareBase + dwHardwareSize, Address ) + 0x13 );

	if( FarProc( ( DWORD ) ClientPtr, dwHardwareBase, dwHardwareEnd ) )
		Error( XString( /*Couldn't find ClientPtr pointer.*/ 0x08, 0x20, 0xAA, 0xE9C4D9C1, 0xCAC197C5, 0x92D5DDDB, 0xD297FBD5, 0xD3DED2C9, 0xEECBB2E1, 0xB2ACADAB, 0xB2A2BAE7 ).c() );

	return ClientPtr;
}

void* COffsets::EngineFuncs( void ) {
	PCHAR String = "ScreenFade";
	DWORD Address = FindMemoryClone( dwHardwareBase, dwHardwareBase + dwHardwareSize, String, strlen( String ) );
	PVOID EnginePtr = ( PVOID )*( PDWORD ) ( FindReference( dwHardwareBase, dwHardwareBase + dwHardwareSize, Address ) + 0x0D );

	if( FarProc( ( DWORD ) EnginePtr, dwHardwareBase, dwHardwareEnd ) )
		Error( XString( /*Couldn't find EnginePtr pointer.*/ 0x08, 0x20, 0xD7, 0x94B7ACB6, 0xBFB2FAAA, 0xFF86888C, 0x87C4A088, 0x8081878F, 0xBB989FCE, 0x9F9F989C, 0x879187D8 ).c() );

	return EnginePtr;
}

bool CheckByte( DWORD Address, BYTE Value, int Offset ) {
	if( *( PBYTE ) ( ( DWORD ) Address + Offset ) == Value )
		return true;
	else
		return false;
}

DWORD COffsets::EngineStudio( void ) {
start_ptr:
	bool badptr = false;
	DWORD dwStudio = 0;
	PCHAR String = "Couldn't get client .dll studio model rendering interface.";

	DWORD Address = FindMemoryClone( dwHardwareBase, dwHardwareBase + dwHardwareSize, String, strlen( String ) );
	PVOID EngineStudioPtr = ( PVOID )*( PDWORD ) ( FindReference( dwHardwareBase, dwHardwareBase + dwHardwareSize, Address ) - 0x14 );

	if( FarProc( ( DWORD ) EngineStudioPtr, dwHardwareBase, dwHardwareEnd ) )
		Error( XString( /*Couldn't find EngineStudioPtr #1 pointer.*/ 0x0B, 0x29, 0x2D, 0x6E415A5C, 0x555C1440, 0x15505E56, 0x5D1A7E52, 0x5A575125, 0x12363620, 0x2C29173C, 0x3B6A687D, 0x6D3E2039, 0x3F263626, 0x7B000000 ).c() );

	for( DWORD i = 0; i <= 60; i++ ) {
		dwStudio = ( DWORD ) g_pClient->HUD_GetStudioModelInterface + i;
		if( CheckByte( dwStudio, 0xB8, 0 ) || CheckByte( dwStudio, 0xBF, 0 ) ) {
			dwStudio++;

			if( IsBadReadPtr( ( PDWORD ) ( *( PDWORD ) dwStudio ), sizeof( engine_studio_api_s ) ) )
				badptr = true;
			else
				badptr = false;

			if( badptr == false ) {
				if( *( PWORD ) ( PDWORD ) EngineStudioPtr == *( PWORD ) ( PDWORD ) ( *( PDWORD ) dwStudio ) ) {
					return *( PDWORD ) dwStudio;
				}
			}
		}
	}
	goto start_ptr;
	return  0;
}
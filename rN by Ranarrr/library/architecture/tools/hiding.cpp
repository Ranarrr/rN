#include <windows.h>
#include "hiding.hpp"

CModule::CModule() {}

CModule::~CModule() {}

CModule* CModule::Get() {
	static CModule sModule;
	return &sModule;
}

PEB_DUMMY_s __declspec( naked ) *GetPEB() {
	_asm
	{
		mov eax, fs:[0x30];
		retn;
	}
}

void CModule::HideFromPEB( HINSTANCE hInstance ) {
	PEB_DUMMY_s *PEB = GetPEB();

	PLIST_ENTRY FirstEntryInLoadOrder = ( PLIST_ENTRY ) &PEB->LDR_Data->InLoadOrderModuleList;
	PLIST_ENTRY FirstEntryInMemoryOrder = ( PLIST_ENTRY ) &PEB->LDR_Data->InMemoryOrderModuleList;
	PLIST_ENTRY FirstEntryInInitOrder = ( PLIST_ENTRY ) &PEB->LDR_Data->InInitializationOrderModuleList;

	for( PLIST_ENTRY TempEntry = FirstEntryInLoadOrder->Flink; TempEntry != FirstEntryInLoadOrder; TempEntry = TempEntry->Flink ) {
		LDR_MODULE_DUMMY_s *TempModule = ( LDR_MODULE_DUMMY_s * ) ( ( DWORD ) TempEntry - 0 * sizeof( LIST_ENTRY ) );
		
		if( TempModule->BaseAddress == hInstance ) {
			TempEntry->Blink->Flink = TempEntry->Flink;
			TempEntry->Flink->Blink = TempEntry->Blink;
			break;
		}
	}

	for( PLIST_ENTRY TempEntry = FirstEntryInMemoryOrder->Flink; TempEntry != FirstEntryInMemoryOrder; TempEntry = TempEntry->Flink ) {
		LDR_MODULE_DUMMY_s *TempModule = ( LDR_MODULE_DUMMY_s * ) ( ( DWORD ) TempEntry - 1 * sizeof( LIST_ENTRY ) );
		
		if( TempModule->BaseAddress == hInstance ) {
			TempEntry->Blink->Flink = TempEntry->Flink;
			TempEntry->Flink->Blink = TempEntry->Blink;
			break;
		}
	}

	for( PLIST_ENTRY TempEntry = FirstEntryInInitOrder->Flink; TempEntry != FirstEntryInInitOrder; TempEntry = TempEntry->Flink ) {
		LDR_MODULE_DUMMY_s *TempModule = ( LDR_MODULE_DUMMY_s * ) ( ( DWORD ) TempEntry - 2 * sizeof( LIST_ENTRY ) );
		
		if( TempModule->BaseAddress == hInstance ) {
			TempEntry->Blink->Flink = TempEntry->Flink;
			TempEntry->Flink->Blink = TempEntry->Blink;
			break;
		}
	}
}

void CModule::RemovePE( DWORD ModuleBase ) {
	PIMAGE_DOS_HEADER pDosHeader = ( PIMAGE_DOS_HEADER ) ModuleBase;
	PIMAGE_NT_HEADERS pNTHeader = ( PIMAGE_NT_HEADERS ) ( ( DWORD ) pDosHeader + ( DWORD ) pDosHeader->e_lfanew );

	if( pNTHeader->Signature != IMAGE_NT_SIGNATURE )
		return;

	if( pNTHeader->FileHeader.SizeOfOptionalHeader ) {
		DWORD Protect;
		WORD Size = pNTHeader->FileHeader.SizeOfOptionalHeader;
		VirtualProtect( ( void * ) ModuleBase, Size, PAGE_EXECUTE_READWRITE, &Protect );
		RtlZeroMemory( ( void * ) ModuleBase, Size );
		VirtualProtect( ( void * ) ModuleBase, Size, Protect, &Protect );
	}
}

void CModule::Hide( HANDLE hModule ) {
	PIMAGE_DOS_HEADER pDOSHeader = ( PIMAGE_DOS_HEADER ) hModule;

	if( pDOSHeader->e_magic == IMAGE_DOS_SIGNATURE ) {
		PIMAGE_NT_HEADERS pNTHeader = ( PIMAGE_NT_HEADERS ) ( ( DWORD ) pDOSHeader + ( DWORD ) pDOSHeader->e_lfanew );

		if( pNTHeader->Signature == IMAGE_NT_SIGNATURE ) {
			DWORD dwOld, dwSize = pNTHeader->OptionalHeader.SizeOfHeaders;

			if( VirtualProtect( ( LPVOID ) pDOSHeader, dwSize, PAGE_READWRITE, &dwOld ) ) {
				memset( ( void * ) pDOSHeader, 0, dwSize );
				VirtualProtect( ( LPVOID ) pDOSHeader, dwSize, dwOld, &dwOld );
			}
		}

		TEB_s *pTeb = nullptr;

		_asm
		{
			mov eax, fs:[0x18]
			mov pTeb, eax
		}

		PLIST_ENTRY pList = &pTeb->Peb->LoaderData->InLoadOrderModuleList;

		for( PLIST_ENTRY pEntry = pList->Flink; pEntry != pList; pEntry = pEntry->Flink ) {
			LDR_MODULE_DUMMY_s *pModule = ( LDR_MODULE_DUMMY_s * ) pEntry;

			if( pModule->BaseAddress == hModule ) {
				pEntry = &pModule->InLoadOrderModuleList; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
				memset( pEntry, 0, sizeof( LIST_ENTRY ) );
				pEntry = &pModule->InMemoryOrderModuleList; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
				memset( pEntry, 0, sizeof( LIST_ENTRY ) );
				pEntry = &pModule->InInitializationOrderModuleList; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
				memset( pEntry, 0, sizeof( LIST_ENTRY ) );
				pEntry = &pModule->HashTableEntry; pEntry->Blink->Flink = pEntry->Flink; pEntry->Flink->Blink = pEntry->Blink;
				memset( pEntry, 0, sizeof( LIST_ENTRY ) );
				memset( pModule->FullDllName.Buffer, 0, pModule->FullDllName.Length );
				memset( pModule, 0, sizeof( LDR_MODULE_DUMMY_s ) );
				break;
			}
		}
	}
}

void CModule::HideExtra( HINSTANCE hModule ) {
	DWORD dwPEB_LDR_DATA = 0;

	_asm
	{
		pushad;
		pushfd;
		mov eax, fs:[30h]							// PEB
			mov eax, [ eax + 0Ch ]					// PEB->ProcessModuleInfo
			mov dwPEB_LDR_DATA, eax					// Save ProcessModuleInfo
			mov esi, [ eax + 0Ch ]					// ProcessModuleInfo->InLoadOrderModuleList[FORWARD]
			mov edx, [ eax + 10h ]					//  ProcessModuleInfo->InLoadOrderModuleList[BACKWARD]

			LoopInLoadOrderModuleList :
			lodsd									//  Load First Module
			mov esi, eax		    				//  ESI points to Next Module
			mov ecx, [ eax + 18h ]		    		//  LDR_MODULE->BaseAddress
			cmp ecx, hModule		    			//  Is it Our Module ?
			jne SkipA		    		    		//  If Not, Next Please (@f jumps to nearest Unamed Lable @@:)
			mov ebx, [ eax ]						//  [FORWARD] Module
			mov ecx, [ eax + 4 ]    		    	//  [BACKWARD] Module
			mov[ ecx ], ebx							//  Previous Module's [FORWARD] Notation, Points to us, Replace it with, Module++
			mov[ ebx + 4 ], ecx						//  Next Modules, [BACKWARD] Notation, Points to us, Replace it with, Module--
			jmp InMemoryOrderModuleList				//  Hidden, so Move onto Next Set
			SkipA :
		cmp edx, esi								//  Reached End of Modules ?
			jne LoopInLoadOrderModuleList			//  If Not, Re Loop

			InMemoryOrderModuleList :
		mov eax, dwPEB_LDR_DATA						//  PEB->ProcessModuleInfo
			mov esi, [ eax + 14h ]					//  ProcessModuleInfo->InMemoryOrderModuleList[START]
			mov edx, [ eax + 18h ]					//  ProcessModuleInfo->InMemoryOrderModuleList[FINISH]

			LoopInMemoryOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [ eax + 10h ]
			cmp ecx, hModule
			jne SkipB
			mov ebx, [ eax ]
			mov ecx, [ eax + 4 ]
			mov[ ecx ], ebx
			mov[ ebx + 4 ], ecx
			jmp InInitializationOrderModuleList
			SkipB :
		cmp edx, esi
			jne LoopInMemoryOrderModuleList

			InInitializationOrderModuleList :
		mov eax, dwPEB_LDR_DATA						//  PEB->ProcessModuleInfo
			mov esi, [ eax + 1Ch ]					//  ProcessModuleInfo->InInitializationOrderModuleList[START]
			mov edx, [ eax + 20h ]					//  ProcessModuleInfo->InInitializationOrderModuleList[FINISH]

			LoopInInitializationOrderModuleList :
			lodsd
			mov esi, eax
			mov ecx, [ eax + 08h ]
			cmp ecx, hModule
			jne SkipC
			mov ebx, [ eax ]
			mov ecx, [ eax + 4 ]
			mov[ ecx ], ebx
			mov[ ebx + 4 ], ecx
			jmp Finished
			SkipC :
		cmp edx, esi
			jne LoopInInitializationOrderModuleList

			Finished :
		popfd;
		popad;
	}
}

bool CModule::DestroyHeader( HMODULE hModule ) {
	DWORD dwStartOffset = ( DWORD ) hModule;
	IMAGE_DOS_HEADER *pDosHeader = ( PIMAGE_DOS_HEADER ) dwStartOffset;
	DWORD dwOldProtection = NULL;
	IMAGE_NT_HEADERS *pNtHeader;

	if( pDosHeader->e_magic != IMAGE_DOS_SIGNATURE )
		return false;

	pNtHeader = ( PIMAGE_NT_HEADERS ) ( dwStartOffset + pDosHeader->e_lfanew );
	if( pNtHeader->Signature != IMAGE_NT_SIGNATURE )
		return false;

	// overwrite
	if( !VirtualProtect( ( PVOID ) hModule, pNtHeader->OptionalHeader.SizeOfHeaders, PAGE_EXECUTE_READWRITE, &dwOldProtection ) )
		return false;

	ZeroMemory( ( PVOID ) hModule, pNtHeader->OptionalHeader.SizeOfHeaders );
	VirtualProtect( ( PVOID ) hModule, pNtHeader->OptionalHeader.SizeOfHeaders, dwOldProtection, &dwOldProtection );

	return true;
}
#ifndef MODULE_SECURITY_HPP
#define MODULE_SECURITY_HPP

class CModule {
	CModule();

public:
	virtual ~CModule();
	static CModule*	Get();

	void HideFromPEB( HINSTANCE hInstance );
	void RemovePE( DWORD ModuleBase );
	void Hide( HANDLE hModule );
	void HideExtra( HINSTANCE hModule );
	bool DestroyHeader( HMODULE hModule );
};

typedef LPVOID *PPVOID;

struct UNICODE_STRING_s {
	USHORT					Length;
	USHORT					MaximumLength;
	PWSTR					Buffer;
};

typedef struct LDR_MODULE_DUMMY_s {
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID BaseAddress;

	PVOID                   EntryPoint;
	ULONG                   SizeOfImage;
	UNICODE_STRING_s        FullDllName;
	UNICODE_STRING_s        BaseDllName;
	ULONG                   Flags;
	SHORT                   LoadCount;
	SHORT                   TlsIndex;
	LIST_ENTRY              HashTableEntry;
	ULONG                   TimeDateStamp;
};

struct PEB_LDR_DATA_s {
	ULONG Length;
	UCHAR Initialized;
	PVOID SsHandle;
	LIST_ENTRY InLoadOrderModuleList;
	LIST_ENTRY InMemoryOrderModuleList;
	LIST_ENTRY InInitializationOrderModuleList;
	PVOID EntryInProgress;
};

struct PEB_DUMMY_s {
	PVOID Dummy1;
	PVOID Dummy2;
	PVOID Dummy3;
	PEB_LDR_DATA_s *LDR_Data;
};

struct CLIENT_ID_s {
	HANDLE					UniqueProcess;
	HANDLE					UniqueThread;
};

struct RTL_DRIVE_LETTER_CURDIR_s {
	USHORT					Flags;
	USHORT					Length;
	ULONG					TimeStamp;
	UNICODE_STRING_s		DosPath;
};

struct RTL_USER_PROCESS_PARAMETERS_s {
	ULONG						MaximumLength;
	ULONG						Length;
	ULONG						Flags;
	ULONG						DebugFlags;
	PVOID						ConsoleHandle;
	ULONG						ConsoleFlags;
	HANDLE						StdInputHandle;
	HANDLE						StdOutputHandle;
	HANDLE						StdErrorHandle;
	UNICODE_STRING_s			CurrentDirectoryPath;
	HANDLE						CurrentDirectoryHandle;
	UNICODE_STRING_s			DllPath;
	UNICODE_STRING_s			ImagePathName;
	UNICODE_STRING_s			CommandLine;
	PVOID						Environment;
	ULONG						StartingPositionLeft;
	ULONG						StartingPositionTop;
	ULONG						Width;
	ULONG						Height;
	ULONG						CharWidth;
	ULONG						CharHeight;
	ULONG						ConsoleTextAttributes;
	ULONG						WindowFlags;
	ULONG						ShowWindowFlags;
	UNICODE_STRING_s			WindowTitle;
	UNICODE_STRING_s			DesktopName;
	UNICODE_STRING_s			ShellInfo;
	UNICODE_STRING_s			RuntimeData;
	RTL_DRIVE_LETTER_CURDIR_s	DLCurrentDirectory[ 0x20 ];
};

typedef void( *PPEBLOCKROUTINE )( PVOID PebLock );

struct PEB_FREE_BLOCK_s {
	PEB_FREE_BLOCK_s		*Next;
	DWORD					Size;
};

struct PEB_s { // 0x7ffdf000
	BOOLEAN							InheritedAddressSpace;
	BOOLEAN							ReadImageFileExecOptions;
	BOOLEAN							BeingDebugged;
	BOOLEAN							Spare;
	HANDLE							Mutant;
	PVOID							ImageBaseAddress;
	PEB_LDR_DATA_s					*LoaderData;
	RTL_USER_PROCESS_PARAMETERS_s	*ProcessParameters;
	PVOID							SubSystemData;
	PVOID							ProcessHeap;
	PVOID							FastPebLock;
	PPEBLOCKROUTINE					FastPebLockRoutine;
	PPEBLOCKROUTINE					FastPebUnlockRoutine;
	ULONG							EnvironmentUpdateCount;
	PVOID							*KernelCallbackTable;
	PVOID							EventLogSection;
	PVOID							EventLog;
	PEB_FREE_BLOCK_s				*FreeList;
	ULONG							TlsExpansionCounter;
	PVOID							TlsBitmap;
	ULONG							TlsBitmapBits[ 0x2 ];
	PVOID							ReadOnlySharedMemoryBase;
	PVOID							ReadOnlySharedMemoryHeap;
	PVOID							*ReadOnlyStaticServerData;
	PVOID							AnsiCodePageData;
	PVOID							OemCodePageData;
	PVOID							UnicodeCaseTableData;
	ULONG							NumberOfProcessors;
	ULONG							NtGlobalFlag;
	BYTE							Spare2[ 0x4 ];
	LARGE_INTEGER					CriticalSectionTimeout;
	ULONG							HeapSegmentReserve;
	ULONG							HeapSegmentCommit;
	ULONG							HeapDeCommitTotalFreeThreshold;
	ULONG							HeapDeCommitFreeBlockThreshold;
	ULONG							NumberOfHeaps;
	ULONG							MaximumNumberOfHeaps;
	PVOID							**ProcessHeaps;
	PVOID							GdiSharedHandleTable;
	PVOID							ProcessStarterHelper;
	PVOID							GdiDCAttributeList;
	PVOID							LoaderLock;
	ULONG							OSMajorVersion;
	ULONG							OSMinorVersion;
	ULONG							OSBuildNumber;
	ULONG							OSPlatformId;
	ULONG							ImageSubSystem;
	ULONG							ImageSubSystemMajorVersion;
	ULONG							ImageSubSystemMinorVersion;
	ULONG							GdiHandleBuffer[ 0x22 ];
	ULONG							PostProcessInitRoutine;
	ULONG							TlsExpansionBitmap;
	BYTE							TlsExpansionBitmapBits[ 0x80 ];
	ULONG							SessionId;
};

struct TEB_s { // 0x7ffde000

	NT_TIB                  Tib;
	PVOID                   EnvironmentPointer;
	CLIENT_ID_s             Cid;
	PVOID                   ActiveRpcInfo;
	PVOID                   ThreadLocalStoragePointer;
	PEB_s                   *Peb;
	ULONG                   LastErrorValue;
	ULONG                   CountOfOwnedCriticalSections;
	PVOID                   CsrClientThread;
	PVOID                   Win32ThreadInfo;
	ULONG                   Win32ClientInfo[ 0x1F ];
	PVOID                   WOW32Reserved;
	ULONG                   CurrentLocale;
	ULONG                   FpSoftwareStatusRegister;
	PVOID                   SystemReserved1[ 0x36 ];
	PVOID                   Spare1;
	ULONG                   ExceptionCode;
	ULONG                   SpareBytes1[ 0x28 ];
	PVOID                   SystemReserved2[ 0xA ];
	ULONG                   GdiRgn;
	ULONG                   GdiPen;
	ULONG                   GdiBrush;
	CLIENT_ID_s             RealClientId;
	PVOID                   GdiCachedProcessHandle;
	ULONG                   GdiClientPID;
	ULONG                   GdiClientTID;
	PVOID                   GdiThreadLocaleInfo;
	PVOID                   UserReserved[ 5 ];
	PVOID                   GlDispatchTable[ 0x118 ];
	ULONG                   GlReserved1[ 0x1A ];
	PVOID                   GlReserved2;
	PVOID                   GlSectionInfo;
	PVOID                   GlSection;
	PVOID                   GlTable;
	PVOID                   GlCurrentRC;
	PVOID                   GlContext;
	ULONG					LastStatusValue;		//it's NTSTATUS actually
	UNICODE_STRING_s        StaticUnicodeString;
	WCHAR                   StaticUnicodeBuffer[ 0x105 ];
	PVOID                   DeallocationStack;
	PVOID                   TlsSlots[ 0x40 ];
	LIST_ENTRY              TlsLinks;
	PVOID                   Vdm;
	PVOID                   ReservedForNtRpc;
	PVOID                   DbgSsReserved[ 0x2 ];
	ULONG                   HardErrorDisabled;
	PVOID                   Instrumentation[ 0x10 ];
	PVOID                   WinSockData;
	ULONG                   GdiBatchCount;
	ULONG                   Spare2;
	ULONG                   Spare3;
	ULONG                   Spare4;
	PVOID                   ReservedForOle;
	ULONG                   WaitingOnLoaderLock;
	PVOID                   StackCommit;
	PVOID                   StackCommitMax;
	PVOID                   StackReserved;
};

#endif // !MODULE_SECURITY_HPP
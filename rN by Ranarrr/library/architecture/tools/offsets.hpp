#ifndef OFFSETS_HPP
#define OFFSETS_HPP

#pragma once

#include "../main.hpp"

#define CompareMemory(Buff1, Buff2, Size) CompareMem((const UCHAR *)Buff1, (const UCHAR *)Buff2, (UINT)Size)
#define FindMemoryClone(Start, End, Clone, Size) FindMemClone((const ULONG)Start, (const ULONG)End, (const ULONG)Clone, (UINT)Size)
#define FindReference(Start, End, Address) FindRef((const ULONG)Start, (const ULONG)End, (const ULONG)Address)

class COffsets {
	COffsets();

	BOOL CompareMem( const UCHAR *buff1, const UCHAR *buff2, UINT size );
	ULONG FindMemClone( const ULONG start, const ULONG end, const ULONG clone, UINT size );
	ULONG FindRef( const ULONG start, const ULONG end, const ULONG address );
	unsigned char m_RenderType; //to be used for ogl ?

public:
	virtual ~COffsets();
	static COffsets*	Get();

	DWORD GetModuleSize( const DWORD Address );
	DWORD FarProc( const DWORD Address, DWORD LB, DWORD HB );
	DWORD EngineStudio();

	void Error( const PCHAR Msg );
	void *ClientFuncs();
	void *EngineFuncs();
	void *GetSpeedPtr();

	DWORD dwHardwareBase, dwHardwareSize, dwHardwareEnd;
	bool GetRendererInfo();
};

#endif // !OFFSETS_HPP
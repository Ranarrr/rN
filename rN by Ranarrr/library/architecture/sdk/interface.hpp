#ifndef INTERFACE_HPP
#define INTERFACE_HPP

#pragma once

#include <string.h>
#include <stdlib.h>
#include "windows.h"

// This header defines the interface convention used in the valve engine.
// To make an interface and expose it:
//    1. Derive from IBaseInterface.
//    2. The interface must be ALL pure virtuals, and have no data members.
//    3. Define a name for it.
//    4. In its implementation file, use EXPOSE_INTERFACE or EXPOSE_SINGLE_INTERFACE.

// Versioning
// There are two versioning cases that are handled by this:
// 1. You add functions to the end of an interface, so it is binary compatible with the previous interface. In this case,
//    you need two EXPOSE_INTERFACEs: one to expose your class as the old interface and one to expose it as the new interface.
// 2. You update an interface so it's not compatible anymore (but you still want to be able to expose the old interface
//    for legacy code). In this case, you need to make a new version name for your new interface, and make a wrapper interface and
//    expose it for the old interface.

#define CREATEINTERFACE_PROCNAME	"CreateInterface"

// All interfaces derive from this.
class IBaseInterface {
public:
	virtual			~IBaseInterface() {}
};

typedef IBaseInterface*( *CreateInterfaceFn )( const char *pName, int *pReturnCode );
typedef IBaseInterface*( *InstantiateInterfaceFn )( );

// Used internally to register classes.
class InterfaceReg {
public:
	InterfaceReg( InstantiateInterfaceFn fn, const char *pName );

	InstantiateInterfaceFn	m_CreateFn;
	const char				*m_pName;

	InterfaceReg			*m_pNext; // For the global list.
	static InterfaceReg		*s_pInterfaceRegs;
};

// This function is automatically exported and allows you to access any interfaces exposed with the above macros.
// if pReturnCode is set, it will return one of the following values
// extend this for other error conditions/code
enum {
	IFACE_OK = 0,
	IFACE_FAILED
};

IBaseInterface* CreateInterface( const char *pName, int *pReturnCode );

// Use these to load and unload a module.
extern void					*Sys_LoadModule( const char *pModuleName );
extern void					Sys_FreeModule( void *hModule );

// Use these to get the factory function from either a loaded module or the current module.
extern CreateInterfaceFn	Sys_GetFactory( void *hModule );
extern CreateInterfaceFn	Sys_GetFactoryThis();

#endif // !INTERFACE_HPP
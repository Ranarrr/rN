#include "interface.hpp"

#define WIN32_LEAN_AND_MEAN

// ------------------------------------------------------------------------------------ //
// InterfaceReg.
// ------------------------------------------------------------------------------------ //
InterfaceReg *InterfaceReg::s_pInterfaceRegs = NULL;

InterfaceReg::InterfaceReg( InstantiateInterfaceFn fn, const char *pName ) :
	m_pName( pName ) {
	m_CreateFn = fn;
	m_pNext = s_pInterfaceRegs;
	s_pInterfaceRegs = this;
}

// ------------------------------------------------------------------------------------ //
// CreateInterface.
// ------------------------------------------------------------------------------------ //
IBaseInterface *CreateInterface( const char *pName, int *pReturnCode ) {
	InterfaceReg *pCur;

	for( pCur = InterfaceReg::s_pInterfaceRegs; pCur; pCur = pCur->m_pNext ) {
		if( strcmp( pCur->m_pName, pName ) == 0 ) {
			if( pReturnCode )
				*pReturnCode = IFACE_OK;

			return pCur->m_CreateFn();
		}
	}

	if( pReturnCode )
		*pReturnCode = IFACE_FAILED;

	return NULL;
}

void *Sys_LoadModule( const char *pModuleName ) {
	return ( void * ) LoadLibrary( pModuleName );
}

void Sys_FreeModule( void *hModule ) {
	if( !hModule )
		return;

	FreeLibrary( ( HMODULE ) hModule );
}

//-----------------------------------------------------------------------------
// Purpose: returns the instance of this module
// Output : interface_instance_t
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactoryThis() {
	return CreateInterface;
}

//-----------------------------------------------------------------------------
// Purpose: returns the instance of the named module
// Input  : *pModuleName - name of the module
// Output : interface_instance_t - instance of that module
//-----------------------------------------------------------------------------
CreateInterfaceFn Sys_GetFactory( void *hModule ) {
	if( !hModule )
		return NULL;

	return ( CreateInterfaceFn ) GetProcAddress( ( HMODULE ) hModule, CREATEINTERFACE_PROCNAME );
}
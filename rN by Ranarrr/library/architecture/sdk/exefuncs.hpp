#ifndef EXEFUNCS_HPP
#define EXEFUNCS_HPP

#pragma once

// Engine hands this to DLLs for functionality callbacks
struct exefuncs_s {
	int		fMMX;
	int		iCPUMhz;
	void	( *unused1 )						();
	void	( *unused2 )						();
	void	( *unused3 )						();
	void	( *unused4 )						();
	void	( *VID_ForceLockState )				( int lk );
	int		( *VID_ForceUnlockedAndReturnState )();
	void	( *unused5 )						();
	void	( *unused6 )						();
	void	( *unused7 )						();
	void	( *unused8 )						();
	void	( *unused9 )						();
	void	( *unused10 )						();
	void	( *unused11 )						();
	void	( *unused12 )						();
	void	( *unused13 )						();
	void	( *unused14 )						();
	void	( *unused15 )						();
	void	( *ErrorMessage )					( int nLevel, const char *pszErrorMessage );
	void	( *unused16 )						();
	void	( *Sys_Printf )						( char *fmt, ... );
	void	( *unused17 )						();
	void	( *unused18 )						();
	void	( *unused19 )						();
	void	( *unused20 )						();
	void	( *unused21 )						();
	void	( *unused22 )						();
	void	( *unused23 )						();
	void	( *unused24 )						();
	void	( *unused25 )						();
};

#endif // !EXEFUNCS_HPP
#ifndef HOOKS_HPP
#define HOOKS_HPP

#include "../main.hpp"

class CHook {
	CHook();

public:
	virtual ~CHook();
	static CHook* Get();

	bool			Initialize();
};

extern cvar_s *RegisterVariable( char *szName, char *szValue, int flags );

#endif // !HOOKS_HPP
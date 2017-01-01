#ifndef _NOFLASH_H_
#define _NOFLASH_H_

#include "main.hpp"

class CNoFlash {
private:
	CNoFlash();
	float flFlashedPercent;
	float flFlashed;
	float fFadeEnd;
	bool bFlashedOn;
public:
	virtual ~CNoFlash();
	static CNoFlash*	Get();

	void NoFlashOn();
	void RegisterCVars();
};

#endif // NOFLASH_H
#ifndef _MEASURE_H_
#define _MEASURE_H_

#include "main.h"

class CMeasure
{
private:
	CMeasure();
public:
	virtual ~CMeasure();
	static CMeasure*	Get();

	void CL_CreateMove(struct usercmd_s *pCmd);
	void HUD_Redraw(void);
	int HUD_Key_Event(int down, int keynum);
	void RegisterCmd();
	bool bMeasureMenuDisplay;
	Vector vecMeasureFirstLoc;
	Vector vecMeasureSecondLoc;
	Vector GetAimOrigin();
private:

	bool bMeasureShowBeams = false;
	bool bMeasureDetailedResults = false;
};

#endif // MEASURE_H
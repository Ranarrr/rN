#include "measure.h"

float VecDistance;

CMeasure::CMeasure()
{
}

CMeasure::~CMeasure()
{
}

CMeasure* CMeasure::Get()
{
	static CMeasure sMeasure;
	return &sMeasure;
}

int CMeasure::HUD_Key_Event(int down, int keynum)
{
	if (bMeasureMenuDisplay)
	{
		if (keynum == '1' && down)
		{
			vecMeasureFirstLoc = GetAimOrigin();
			return 0;
		}
		if (keynum == '2' && down)
		{
			vecMeasureSecondLoc = GetAimOrigin();
			return 0;
		}
		if (keynum == '3' && down)
		{
			bMeasureDetailedResults = !bMeasureDetailedResults;
			return 0;
		}
		if (keynum == '4'  && down)
		{
			bMeasureShowBeams = !bMeasureShowBeams;
			return 0;
		}
		if (keynum == '0' && down)
		{
			bMeasureMenuDisplay = !bMeasureMenuDisplay;
			bMeasureShowBeams = false;
			return 0;
		}
	}
}

void GetDistFromOrigToAimorig()
{
	VecDistance = VectorDistance(g_pLocalPlayer()->m_vecOrigin, CMeasure::Get()->GetAimOrigin());
}

void MeasureDistance()
{
	vec3_t vOrigin;
	vOrigin = g_pLocalPlayer()->m_vecOrigin;

	vOrigin[2] += g_pLocalPlayer()->m_vecMins[g_pLocalPlayer()->m_iUseHull][2];

	CMeasure::Get()->vecMeasureFirstLoc[0] = CMeasure::Get()->vecMeasureSecondLoc[0] = vOrigin[0];
	CMeasure::Get()->vecMeasureFirstLoc[1] = CMeasure::Get()->vecMeasureSecondLoc[1] = vOrigin[1];
	CMeasure::Get()->vecMeasureFirstLoc[2] = CMeasure::Get()->vecMeasureSecondLoc[2] = vOrigin[2];

	CMeasure::Get()->bMeasureMenuDisplay = true;
}

void CMeasure::HUD_Redraw()
{
	CDrawing::Get()->DrawString(300, 300, 246, 58, 150, /*Distance to aim: %f*/XorStr<0xF9, 20, 0x41C606A2>("\xBD\x93\x88\x88\x9C\x90\x9C\x65\x21\x76\x6C\x24\x64\x6F\x6A\x32\x29\x2F\x6D" + 0x41C606A2).s, VecDistance);
	if (bMeasureMenuDisplay)
	{
		int Y = 0;

		CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 * Y, 255, 0, 0, /*Measure*/XorStr<0x95, 8, 0xBA8EF129>("\xD8\xF3\xF6\xEB\xEC\xE8\xFE" + 0xBA8EF129).s); Y++; Y++;

		CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*1. Set Loc #1 < %.3f | %.3f | %.3f >*/XorStr<0xB8, 37, 0x111E47D0>("\x89\x97\x9A\xE8\xD9\xC9\x9E\xF3\xAF\xA2\xE2\xE0\xF5\xE5\xFA\xE7\xED\xE7\xF9\xAD\xEC\xCD\xEE\xEA\xFE\xE2\xB4\xF3\xD4\xF5\xF3\xF9\xEB\xBF\xFA\xE5" + 0x111E47D0).s, vecMeasureFirstLoc[0], vecMeasureFirstLoc[1], vecMeasureFirstLoc[2]); Y++;
		CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*2. Set Loc #2 < %.3f | %.3f | %.3f >*/XorStr<0x26, 37, 0x064FA452>("\x14\x09\x08\x7A\x4F\x5F\x0C\x61\x41\x4C\x10\x12\x00\x13\x08\x15\x13\x19\x0B\x5F\x1A\x3B\x1C\x18\x10\x0C\x26\x61\x42\x63\x61\x6B\x75\x21\x68\x77" + 0x064FA452).s, vecMeasureSecondLoc[0], vecMeasureSecondLoc[1], vecMeasureSecondLoc[2]); Y++; Y++;
	
		CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*3. Detailed results: %s*/XorStr<0x9D, 24, 0x2D7A05A1>("\xAE\xB0\xBF\xE4\xC4\xD6\xC2\xCD\xC9\xC3\xC3\x88\xDB\xCF\xD8\xD9\xC1\xDA\xDC\x8A\x91\x97\xC0" + 0x2D7A05A1).s, bMeasureDetailedResults ? /*on*/XorStr<0xC1,3,0xC88CA28D>("\xAE\xAC"+0xC88CA28D).s : /*off*/XorStr<0x2A, 4, 0x38AEB705>("\x45\x4D\x4A" + 0x38AEB705).s); Y++;
		CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*    Results:*/XorStr<0xDF, 13, 0x2E69CEB5>("\xFF\xC0\xC1\xC2\xB1\x81\x96\x93\x8B\x9C\x9A\xD0" + 0x2E69CEB5).s); Y++;

		if (bMeasureDetailedResults)
		{
			CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*        X-Distance: %f*/XorStr<0x33, 23, 0x7E548816>("\x13\x14\x15\x16\x17\x18\x19\x1A\x63\x11\x79\x57\x4C\x34\x20\x2C\x20\x21\x7F\x66\x62\x2E" + 0x7E548816).s, abs(vecMeasureFirstLoc[0] - vecMeasureSecondLoc[0])); Y++;
			CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*        Y-Distance: %f*/XorStr<0xF7, 23, 0xC08E1EC8>("\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xA6\x2D\x45\x6B\x70\x70\x64\x68\x64\x6D\x33\x2A\x2E\x6A" + 0xC08E1EC8).s, abs(vecMeasureFirstLoc[1] - vecMeasureSecondLoc[1])); Y++;
			CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*        Z-Distance: %f*/XorStr<0x08, 23, 0x3B2FD6BE>("\x28\x29\x2A\x2B\x2C\x2D\x2E\x2F\x4A\x3C\x56\x7A\x67\x61\x77\x79\x7B\x7C\x20\x3B\x39\x7B" + 0x3B2FD6BE).s, abs(vecMeasureFirstLoc[2] - vecMeasureSecondLoc[2])); Y++;
			CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*        Length: %f*/XorStr<0x6E, 19, 0xFA71AC0E>("\x4E\x4F\x50\x51\x52\x53\x54\x55\x3A\x12\x16\x1E\x0E\x13\x46\x5D\x5B\x19" + 0xFA71AC0E).s, VectorDistance(vecMeasureFirstLoc, vecMeasureSecondLoc)); Y++; Y++;
		}
		else
		{
			CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*        Height difference: %f*/XorStr<0x66, 30, 0x0A2C45E9>("\x46\x47\x48\x49\x4A\x4B\x4C\x4D\x26\x0A\x19\x16\x1A\x07\x54\x11\x1F\x11\x1E\x1C\x08\x1E\x12\x1E\x1B\x45\xA0\xA4\xE4" + 0x0A2C45E9).s, abs(vecMeasureFirstLoc[2] - vecMeasureSecondLoc[2])); Y++;
			CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*        Distance: %f*/XorStr<0x18, 21, 0xD4F09485>("\x38\x39\x3A\x3B\x3C\x3D\x3E\x3F\x64\x48\x51\x57\x45\x4B\x45\x42\x12\x09\x0F\x4D" + 0xD4F09485).s, VectorDistance(vecMeasureFirstLoc, vecMeasureSecondLoc)); Y++; Y++;
		}

		CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*4. Show beams: %s*/XorStr<0xFA, 18, 0xC585545D>("\xCE\xD5\xDC\xAE\x96\x90\x77\x21\x60\x66\x65\x68\x75\x3D\x28\x2C\x79" + 0xC585545D).s, bMeasureShowBeams ? /*on*/XorStr<0xB8, 3, 0x62D99663>("\xD7\xD7" + 0x62D99663).s : /*off*/XorStr<0x07, 4, 0x134E835F>("\x68\x6E\x6F" + 0x134E835F).s); Y++; Y++;
		CDrawing::Get()->DrawString(g_Screen.m_iWidth * 0.1, g_Screen.m_iHeight * 0.1 + CDrawing::Get()->GetStringHeight() * Y, 255, 255, 255, /*0. Exit*/XorStr<0x12, 8, 0x5D7BABA3>("\x22\x3D\x34\x50\x6E\x7E\x6C" + 0x5D7BABA3).s); Y++;
	}
}

Vector CMeasure::GetAimOrigin()
{
	Vector vecStart, vecViewHeight, vecOrigin, vecForward, vecDest;

	VectorCopy(g_pLocalPlayer()->m_vecOrigin, vecStart);
	VectorCopy(g_pLocalPlayer()->m_vecViewHeight, vecViewHeight);
	VectorAdd(vecStart, vecViewHeight, vecStart);

	g_Engine.pfnAngleVectors(g_pLocalPlayer()->m_vecViewAngles, vecForward, NULL, NULL);

	VectorScale(vecForward, 9999.0, vecForward);

	vecDest = g_pLocalPlayer()->m_vecEyePosition + vecForward;

	pmtrace_t *trace = g_pEngine->PM_TraceLine(vecStart, vecDest, 0, 2, g_pLocalPlayer()->m_iIndex);
	VectorCopy(trace->endpos, vecOrigin);

	return vecOrigin;
}

void CMeasure::RegisterCmd()
{
	g_pEngine->pfnAddCommand(PrefHack(/**/XorStr<0xBF, 1, 0xB43E28ED>("" + 0xB43E28ED).s, Prefix_ini(/*prefix.ini*/XorStr<0x53, 11, 0xF8D2B91E>("\x23\x26\x30\x30\x3E\x20\x77\x33\x35\x35" + 0xF8D2B91E).s), /*mesr*/XorStr<0xD6, 5, 0x6C7D5B89>("\xBB\xB2\xAB\xAB" + 0x6C7D5B89).s), MeasureDistance);
	g_pEngine->pfnAddCommand(PrefHack(/**/XorStr<0x47, 1, 0x7F828B24>("" + 0x7F828B24).s, Prefix_ini(/*prefix.ini*/XorStr<0xDE, 11, 0x532C1728>("\xAE\xAD\x85\x87\x8B\x9B\xCA\x8C\x88\x8E" + 0x532C1728).s), /*MesrDistanceAim*/XorStr<0xAE, 16, 0x08E8AB21>("\xE3\xCA\xC3\xC3\xF6\xDA\xC7\xC1\xD7\xD9\xDB\xDC\xFB\xD2\xD1" + 0x08E8AB21).s), GetDistFromOrigToAimorig);
}
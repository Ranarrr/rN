#include "NoFlash.hpp"

CNoFlash::CNoFlash() {}

CNoFlash::~CNoFlash() {}

CNoFlash* CNoFlash::Get() {
	static CNoFlash sNoFlash;
	return &sNoFlash;
}

void CNoFlash::NoFlashOn() {
	screenfade_s pScreenFade;
	g_Engine.pfnGetScreenFade( &pScreenFade );
	if( CCVars::Get()->noflash->value ) {
		pScreenFade.fadealpha = CCVars::Get()->noflash->value;
		pScreenFade.fadeReset;
		g_Engine.pfnSetScreenFade( &pScreenFade );
	}

	flFlashedPercent = ( flFlashed / 255 ) * 100;

	if( CCVars::Get()->noflash->value )
		if( pScreenFade.fadeEnd > g_Engine.GetClientTime() )
			fFadeEnd = pScreenFade.fadeEnd;

	if( fFadeEnd > g_Engine.GetClientTime() ) {
		float PrE = fFadeEnd - g_Engine.GetClientTime();
		flFlashed = PrE * pScreenFade.fadeSpeed;
	} else
		flFlashed = 0.0f;

	if( flFlashed > 255.0f )
		flFlashed = 255.0f;

	if( flFlashed <= 0.0f ) {
		flFlashed = 0.0f;
		bFlashedOn = false;
	}

	if( flFlashed > 0.0f )
		bFlashedOn = true;

	if( bFlashedOn )
		CDrawing::Get()->DrawStringCenter( g_Screen.m_iWidth / 2, g_Screen.m_iHeight * 0.2, Color::Red(), "Flashed: %.0f%%", Get()->flFlashedPercent );
}

void CNoFlash::RegisterCVars() {
	CCVars::Get()->noflash = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "noflash" ), "120", NULL );
}
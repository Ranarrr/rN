#include "main.hpp"

CEsp::CEsp() {
}

CEsp::~CEsp() {
}

CEsp* CEsp::Get() {
	static CEsp sEsp;
	return &sEsp;
}

void CEsp::PlayerEsp( int iIndex ) {
	SOtherPlayers& COP = g_OtherPlayers[ iIndex ];
	Color color( 255, 255, 255, 255 );
	float flDrawPos[ 2 ];

	float dist = COP.m_flDistance;

	dist /= 55.4f;

	dist = max( dist, 1.0f );

	if( COP.m_iTeam == 1 ) {
		if( COP.m_bVisible ) {
			color.ApplyR( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_r_vis->value ) );
			color.ApplyG( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_g_vis->value ) );
			color.ApplyB( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_b_vis->value ) );
			color.ApplyA( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_a_vis->value ) );
		} else {
			color.ApplyR( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_r_nvis->value ) );
			color.ApplyG( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_g_nvis->value ) );
			color.ApplyB( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_b_nvis->value ) );
			color.ApplyA( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_a_nvis->value ) );
		}
	} else if( COP.m_iTeam == 2 ) {
		if( COP.m_bVisible ) {
			color.ApplyR( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_r_vis->value ) );
			color.ApplyG( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_g_vis->value ) );
			color.ApplyB( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_b_vis->value ) );
			color.ApplyA( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_a_vis->value ) );
		} else {
			color.ApplyR( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_r_nvis->value ) );
			color.ApplyG( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_g_nvis->value ) );
			color.ApplyB( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_b_nvis->value ) );
			color.ApplyA( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_a_nvis->value ) );
		}
	}

	if( CCVars::Get()->esp_visible->value ) {
		if( COP.m_iTeam == 1 ) {
			color.ApplyR( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_r_vis->value ) );
			color.ApplyG( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_g_vis->value ) );
			color.ApplyB( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_b_vis->value ) );
			color.ApplyA( Instruments::Get()->ByteControl( CCVars::Get()->esp_t_a_vis->value ) );
		} else if( COP.m_iTeam == 2 ) {
			color.ApplyR( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_r_vis->value ) );
			color.ApplyG( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_g_vis->value ) );
			color.ApplyB( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_b_vis->value ) );
			color.ApplyA( Instruments::Get()->ByteControl( CCVars::Get()->esp_ct_a_vis->value ) );
		}
	}

	if( Instruments::Get()->WorldToScreen( COP.m_vecOrigin, flDrawPos ) ) {
		if( CCVars::Get()->esp_dist->value )
			CDrawing::Get()->DrawStringCenter( flDrawPos[ 0 ], flDrawPos[ 1 ] + CDrawing::Get()->GetStringHeight(), color, "Dist: %.0fm", dist );

		float l = VectorDistance( COP.m_vecOrigin, g_pLocalPlayer()->m_vecViewOrg );
		l = max( 100, l );
		const float pw = 16.0f;
		const float ph = 36.0f;
		int bw = ( 30.0f*pw ) / l * 15;
		int bh = ( 30.0f*ph ) / l * 15;
		int bx = flDrawPos[ 0 ] - bw;
		int by = flDrawPos[ 1 ] - ( ( 30.0f*( ph + 10 ) ) / l * 15 );

		if( CCVars::Get()->esp_box->value )
			CDrawing::Get()->DrawBorderBoxOutlined( bx, by, bw * 2.4f, bh * 3.2, 1, color, Color::Black() );

		if( CCVars::Get()->esp_name->value )
			CDrawing::Get()->DrawStringCenter( flDrawPos[ 0 ], flDrawPos[ 1 ], color, "%s", COP.m_Info.name );

		COP.m_bVisible ? CDrawing::Get()->DrawStringCenter( flDrawPos[ 0 ], flDrawPos[ 1 ] + CDrawing::Get()->GetStringHeight() * 2, color, "VISIBLE" ) : CDrawing::Get()->DrawStringCenter( flDrawPos[ 0 ], flDrawPos[ 1 ] + CDrawing::Get()->GetStringHeight() * 2, color, "NOT VISIBLE" );
	}
}

void CEsp::RegisterCVars() {
	CCVars::Get()->esp_ct_r_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_r_nvis" ), "0", NULL );
	CCVars::Get()->esp_ct_g_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_g_nvis" ), "0", NULL );
	CCVars::Get()->esp_ct_b_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_b_nvis" ), "255", NULL );
	CCVars::Get()->esp_ct_a_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_a_nvis" ), "155", NULL );

	CCVars::Get()->esp_ct_r_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_r_vis" ), "150", NULL );
	CCVars::Get()->esp_ct_g_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_g_vis" ), "255", NULL );
	CCVars::Get()->esp_ct_b_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_b_vis" ), "0", NULL );
	CCVars::Get()->esp_ct_a_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_ct_a_vis" ), "255", NULL );

	CCVars::Get()->esp_t_r_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_r_nvis" ), "255", NULL );
	CCVars::Get()->esp_t_g_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_g_nvis" ), "0", NULL );
	CCVars::Get()->esp_t_b_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_b_nvis" ), "0", NULL );
	CCVars::Get()->esp_t_a_nvis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_a_nvis" ), "155", NULL );

	CCVars::Get()->esp_t_r_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_r_vis" ), "0", NULL );
	CCVars::Get()->esp_t_g_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_g_vis" ), "255", NULL );
	CCVars::Get()->esp_t_b_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_b_vis" ), "0", NULL );
	CCVars::Get()->esp_t_a_vis = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_t_a_vis" ), "255", NULL );

	CCVars::Get()->esp_box = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_box" ), "1", NULL );
	CCVars::Get()->esp_name = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_name" ), "1", NULL );
	CCVars::Get()->esp_dist = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_dist" ), "1", NULL );
	CCVars::Get()->esp_visible = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp_visible" ), "1", NULL );
	CCVars::Get()->esp = g_Engine.pfnRegisterVariable( PrefHack( "", Prefix_ini(), "esp" ), "1", NULL );
}
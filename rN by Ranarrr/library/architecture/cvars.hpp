#ifndef CVARS_HPP
#define CVARS_HPP

#include "main.hpp"

// wrapper for registering cvar (variable)
#define reg_cvar( name, defval )						( g_Engine.pfnRegisterVariable( Instruments::Get()->PrefHack( "", name ), defval, NULL ) )
// if the command is a toggle function, just pass a nullptr as the 4th arg ( funcoff )
#define reg_cmd( name, btogglefunc, funcon, funcoff )	g_Engine.pfnAddCommand( Instruments::Get()->PrefHack( btogglefunc ? "" : "+", name ), funcon );			\
														if( funcoff != nullptr && !btogglefunc ) { g_Engine.pfnAddCommand( Instruments::Get()->PrefHack( "-", name ), funcoff ); }

class CCVars {
	CCVars();

public:
	virtual ~CCVars();
	static CCVars*	Get();

	inline void strreplace( char *buf, const char *search, const char *replace );
	inline void ExecFunc( char *name );

	std::vector<scrollpattern> patterns;

	float *r_norefresh;
	float *fps_max;
	float *fps_override;
	float *r_drawviewmodel;
	float *ex_interp;
	float *developer;

	cvar_s *bhop_start, *thirdperson, *thirdperson_up, *thirdperson_back, *thirdperson_side, *thirdperson_alpha, *bhop, *fullbright,
		*strafe_control_helper, *strafe_control_helper_forwardmove, *strafe_control_helper_adelay, *strafe_control_helper_sidemove, *strafe_control_speed, *strafe_control_helper_surffix_height,
		*strafe_control_helper_dir, *strafe_control_helper_add_strafe, *strafe_control_helper_max_strafe, *strafe_control_helper_type, *speed, *strafe_control_helper_forwardmove_random,
		*strafe_control_helper_forwardmove_random_min, *strafe_control_speed_const, *strafe_control_helper_althreshold, *strafe_control_helper_forwardmove_random_max, *bhop_end, *antihit_eff,
		*strafe_control_helper_sidemove_random, *strafe_control_helper_sidemove_random_min, *strafe_control_helper_sidemove_random_max, *fps_helper, *strafe_control_helper_arthreshold, *gs,
		*gs_disable_jump, *gs_max_speed, *gs_max_speed_random, *gs_max_speed_random_max, *gs_max_speed_random_min, *strafe_hack_sidemove, *screeninfo, *gs_nsd, *gs_scroll, *gs_scroll_fallspeed,
		*gs_scroll_dist_end, *freelook_speed, *recoil_crosshair, *strafe_hack_fps_boost, *strafe_hack_skip_frames, *edgebug, *edgebug_edge_dist, *edgebug_ground_dist, *edgebug_intense,
		*noflash, *strafe_hack_fps_helper, *strafe_hack_forwardmove, *sidemove, *forwardmove, *strafe_control_helper_dir_auto, *strafe_control_helper_surffix, *esp, *esp_crosshair_player_info,
		*auto_duck, *auto_duck_dist, *knifebot, *knifebot_stab, *knifebot_stab_dist, *auto_pistol, *auto_pistol_speed, *edge_jumpoff_dist, *esp_name, *esp_dist, *esp_box, *esp_ct_r_nvis,
		*esp_ct_r_vis, *esp_t_r_nvis, *esp_t_r_vis, *esp_ct_g_nvis, *esp_ct_g_vis, *esp_t_g_nvis, *esp_t_g_vis, *esp_ct_b_nvis, *esp_ct_b_vis, *esp_t_b_nvis, *esp_t_b_vis, *esp_ct_a_nvis,
		*esp_ct_a_vis, *esp_t_a_nvis, *esp_t_a_vis, *recoil_control, *bhop_scroll_delay, /**knifebot_slash_dist, *knifebot_teammates, *knifebot_origin, *knifebot_fov, *knifebot_aim,*/ *esp_visible/*,
		*knifebot_follow_dist, *knifebot_follow*/, *strafe_control_helper_rayish_fix, *bhop_wog, *bhop_standup, *bhop_standup_10aa, *screeninfo_mult_x, *screeninfo_mult_y, *screeninfo_velocity,
		*screeninfo_fallspeed, *screeninfo_groundheight, *screeninfo_edgedist, *bhop_standup_dist, *auto_duck_onlyblocks/**strafe_control_helper_sync_min, *strafe_control_helper_sync_max, *strafe_control_helper_sync_ticks*/;

	void Init();
};
#endif // !CVARS_HPP
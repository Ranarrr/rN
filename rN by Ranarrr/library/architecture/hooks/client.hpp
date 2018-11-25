#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "../main.hpp"

class CClient {
	CClient();

	static void		HUD_Frame( double time );
	static void		HUD_Redraw( float time, int intermission );
	static void		HUD_PlayerMove( playermove_s *ppmove, int server );
	static void		CL_CreateMove( float flFrameTime, usercmd_s *pCmd, int iActive );
	static void		V_CalcRefdef( ref_params_s *pParams );
	static int		HUD_AddEntity( int type, cl_entity_s *ent, const char *modelname );
	static int		CL_IsThirdPerson();

public:
	virtual ~CClient();
	static CClient*	Get();

	void HookEngine();
	void HookStudio();
	void HookClient();
	void RegisterCmds();
};

#endif // !CLIENT_HPP
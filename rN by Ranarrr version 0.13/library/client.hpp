#ifndef _CLIENT_H_
#define _CLIENT_H_

class CClient {
private:
	CClient();
public:
	virtual ~CClient();
	static CClient*	Get();

	void HookEngine( void );
	void HookStudio( void );
	void HookClient( void );
	void RegisterCmds();
private:
	static void HUD_Frame( double time );
	static void HUD_Redraw( float time, int intermission );
	static void HUD_PlayerMove( struct playermove_s *ppmove, int server );
	static void CL_CreateMove( float flFrameTime, struct usercmd_s *pCmd, int iActive );
	static void V_CalcRefdef( struct ref_params_s *pParams );
	static int HUD_AddEntity( int type, struct cl_entity_s *ent, const char *modelname );
	static int CL_IsThirdPerson();
};

#endif // CLIENT_H
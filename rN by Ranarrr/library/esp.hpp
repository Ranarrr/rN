#ifndef _ESP_H_
#define _ESP_H_

class CEsp {
private:
	CEsp();

public:
	virtual ~CEsp();
	static CEsp*	Get();

	void PlayerEsp( int iIndex );
	void RegisterCVars();
};

#endif // ESP_H_
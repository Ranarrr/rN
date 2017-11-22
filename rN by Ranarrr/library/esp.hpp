#ifndef ESP_HPP
#define ESP_HPP

class CEsp {
private:
	CEsp();

public:
	virtual ~CEsp();
	static CEsp*	Get();

	void PlayerEsp( int iIndex );
	void RegisterCVars();
};

#endif // ESP_HPP
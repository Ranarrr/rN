#ifndef ESP_HPP
#define ESP_HPP

#pragma once

#include "../main.hpp"

class CEsp {
	CEsp();

public:
	virtual ~CEsp();
	static CEsp*	Get();

	void PlayerEsp( int iIndex );
	void RegisterCVars();
};

#endif // !ESP_HPP
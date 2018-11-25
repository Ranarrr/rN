#ifndef XSTRING_HPP
#define XSTRING_HPP

#include "../main.hpp"

class XString {
	char	*m_charList;
	char	*m_retBuf;
	int		m_strLength;
	int		m_hashLength;
	int		m_key;

public:
	XString( int hashlen, int strlen, int key, ... );
	virtual ~XString();

	char		*c();
	std::string s();
};

#endif // !XSTRING_HPP
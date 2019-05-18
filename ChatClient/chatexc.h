#ifndef _CHAT_EXCEPTION_H_
#define _CHAT_EXCEPTION_H_

#include <exception>
#include <string>

class send_exc: public std::exception
{
	std::string m_str;
public:
	send_exc(const std::string& str): m_str(str) {};
	virtual const char* what() const { return m_str.c_str();}
};

#endif
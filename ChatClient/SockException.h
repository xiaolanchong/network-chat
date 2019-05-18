// SockException.h: interface for the SockException class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SOCKEXCEPTION_H__127377D9_CD8C_413A_AFDF_7FC67FD89366__INCLUDED_)
#define AFX_SOCKEXCEPTION_H__127377D9_CD8C_413A_AFDF_7FC67FD89366__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <exception>
#include <string>

using std::exception;
using std::string;

class SockException : public exception  
{
	string		m_str;
public:
	SockException(int err);
	virtual ~SockException();

	virtual const char* what() const
	{
		return m_str.c_str();
	}
};

class TimerException : public exception  
{
	string		m_str;
public:
	TimerException(string str) : m_str(str){};
	virtual ~TimerException() {};

	virtual const char* what() const
	{
		return m_str.c_str();
	}
};

class ProtoException : public exception  
{
	string		m_str;
public:
	ProtoException(string str) : m_str(str){};
	virtual ~ProtoException() {};

	virtual const char* what() const
	{
		return m_str.c_str();
	}
};

#endif // !defined(AFX_SOCKEXCEPTION_H__127377D9_CD8C_413A_AFDF_7FC67FD89366__INCLUDED_)

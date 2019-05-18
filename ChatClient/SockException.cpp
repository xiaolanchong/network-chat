// SockException.cpp: implementation of the SockException class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SockException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SockException::SockException(int err)
{
	char* pMsgBuf;
	::FormatMessage( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				    FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					err,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPTSTR) &pMsgBuf,
				    0,
				    NULL 
					);
	char b[256];
	CharToOem(pMsgBuf, b);
	m_str = b;
	LocalFree(pMsgBuf);
}

SockException::~SockException()
{

}

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
	::FormatMessageA( 
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				    FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL,
					err,
					MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
					(LPSTR) &pMsgBuf,
				    0,
				    NULL 
					);
	m_str = pMsgBuf;
	LocalFree(pMsgBuf);
}

SockException::~SockException()
{

}

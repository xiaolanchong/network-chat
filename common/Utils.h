
#pragma once

#include "StdAfx.h"

namespace utils
{
	inline int SendString(SOCKET socket, const CString& str)
	{
		return ::send(socket, reinterpret_cast<const char*>((LPCTSTR)str), str.GetLength() * sizeof(TCHAR), 0);
	}
}

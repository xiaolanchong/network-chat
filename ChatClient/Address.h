// Address.h: interface for the Address class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADDRESS_H__8528A0A7_5687_40B9_B4B3_CF5678C51420__INCLUDED_)
#define AFX_ADDRESS_H__8528A0A7_5687_40B9_B4B3_CF5678C51420__INCLUDED_

#include <afxsock.h>

struct SockAddr : public sockaddr_in
{
	SockAddr(unsigned short port, in_addr addr)
	{
		memset(static_cast<sockaddr_in*>(this), 0, sizeof(sockaddr_in));
		sin_family	=	AF_INET;
		sin_port	=	htons(port);
		sin_addr	=	addr;
	}
	SockAddr(unsigned short port, unsigned long addr)
	{
		memset(static_cast<sockaddr_in*>(this), 0, sizeof(sockaddr_in));
		sin_family	=	AF_INET;
		sin_port	=	htons(port);
		sin_addr.S_un.S_addr	=	addr;
	}
	SockAddr(unsigned short port, const char* str)
	{
		memset(static_cast<sockaddr_in*>(this), 0, sizeof(sockaddr_in));
		sin_family	= AF_INET;
		sin_port	= htons(port);
/*		if(isalpha(str[0]))
		{
			hostent* he = gethostbyname(str);
			memcpy(&sin_addr, &he->h_addr_list[0], sizeof(sin_addr));
		}
		else
		{
			sin_addr.S_un.S_addr = inet_addr(str);
		}*/
		DWORD ip;
		if(( ip = inet_addr(str)) == INADDR_NONE)
		{
			hostent* ph = gethostbyname(str);
			memcpy(&ip, ph->h_addr, 4);
		}
		memcpy(&sin_addr, &ip, 4);

	}

	const char* inetaddr()	const
	{
		return inet_ntoa(sin_addr); 
	}

	const char*           host() const
	{
		hostent* ph = gethostbyaddr((const char*)&sin_addr, 4, AF_INET);
		return ph->h_name;
	}

	operator		sockaddr_in*()	{ return static_cast<sockaddr_in*>(this); }

};

struct AnyAddr : public SockAddr
{
	AnyAddr(unsigned short port):
		SockAddr(port, INADDR_ANY) 
	{};
};

struct BroadAddr : public SockAddr
{
	BroadAddr(unsigned short port):
		SockAddr(port, INADDR_BROADCAST) 
	{};
};

#endif // !defined(AFX_ADDRESS_H__8528A0A7_5687_40B9_B4B3_CF5678C51420__INCLUDED_)

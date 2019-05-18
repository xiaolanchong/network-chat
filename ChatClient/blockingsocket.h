#ifndef _BLOCKSOCKET_H_
#define _BLOCKSOCKET_H_

typedef const struct sockaddr_in* LPCSOCKADDR;

#include "SockException.h"
#include "Address.h"

class CBlockingSocket 
{
	enum 
	{
		DEFAULT_TIMEOUT=100
	};
	SOCKET m_hSocket;
public:
	CBlockingSocket():m_hSocket(NULL){}
	CBlockingSocket(SOCKET s): m_hSocket(s){}
	void Cleanup();
	void Create(int nType = SOCK_STREAM);
	void Close();
	void Bind(const SockAddr& sa);
	void Listen();
	void Connect(const SockAddr& sa);
	bool Accept(CBlockingSocket& s, SockAddr& sa);
	int Send			(const char* buf, size_t len, 
						 int nSecs=DEFAULT_TIMEOUT);
	int Receive			(char* buf, size_t len, 
						 int nSecs=DEFAULT_TIMEOUT);
	int SendDatagram	(const char* buf, size_t len, 
						 const SockAddr& sa, int nSecs=DEFAULT_TIMEOUT);
	int ReceiveDatagram	(char* buf, size_t len, 
					     SockAddr& psa, int nSecs=DEFAULT_TIMEOUT);
	operator SOCKET()
		{ return m_hSocket; }
};

#endif // #ifndef

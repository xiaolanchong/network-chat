// Copyright (c) 1999 Lee Patterson
// leepatterson@home.com

// blocksock.cpp (CBlockingSocketException, CBlockingSocket, CHttpBlockingSocket)
#include "stdafx.h"
#include "blockingsocket.h"


// Class CBlockingSocket
/*#ifndef ASSERT
#define ASSERT*/

void CBlockingSocket::Cleanup()
{
	// doesn't throw an exception because it's called in a catch block
	if(m_hSocket == NULL) return;
	closesocket(m_hSocket);
	m_hSocket = NULL;
}

void CBlockingSocket::Create(int nType /* = SOCK_STREAM */)
{
	ASSERT(m_hSocket == NULL);
	if((m_hSocket = socket(AF_INET, nType, 0)) == INVALID_SOCKET) 
	{
		throw SockException(WSAGetLastError());
	}
}

void CBlockingSocket::Bind(const SockAddr& sa)
{
	ASSERT(m_hSocket != NULL);
	if(bind(m_hSocket, (sockaddr*)&sa, sizeof(SockAddr)) == SOCKET_ERROR) 
	{
		throw SockException(WSAGetLastError());
	}
}

void CBlockingSocket::Listen()
{
	ASSERT(m_hSocket != NULL);
	if(listen(m_hSocket, 1) == SOCKET_ERROR) 
	{
		throw SockException(WSAGetLastError());
	}
}

bool CBlockingSocket::Accept(CBlockingSocket& sConnect, SockAddr& sa)
{
	ASSERT(m_hSocket != NULL);
	ASSERT(sConnect.m_hSocket == NULL);
	int nLengthAddr = sizeof(SockAddr);
	sConnect.m_hSocket = accept(m_hSocket, (sockaddr*)&sa, &nLengthAddr);
	if(sConnect == INVALID_SOCKET) 
	{
		// no exception if the listen was canceled
		if(WSAGetLastError() != WSAEINTR) 
		{
			throw SockException(WSAGetLastError());
		}
		return FALSE;
	}
	return TRUE;
}

void CBlockingSocket::Close()
{
	ASSERT(m_hSocket != NULL);
	if(closesocket(m_hSocket) == SOCKET_ERROR) 
	{
		// should be OK to close if closed already
	//	throw "Close";
	}
	m_hSocket = NULL;
}

void CBlockingSocket::Connect(const SockAddr& sa)
{
	ASSERT(m_hSocket != NULL);
	// should timeout by itself
	if(connect(m_hSocket, (sockaddr*)&sa, sizeof(SOCKADDR)) == SOCKET_ERROR) 
	{
		throw SockException(WSAGetLastError());
	}
}

int CBlockingSocket::Send(const char* buf, size_t len, int nSecs)
{
	ASSERT(m_hSocket != NULL);
	// returned value will be less than nSize if client cancels the reading
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) 
	{
		throw TimerException("Send : time out");
	}
	int nBytesSent;
	if((nBytesSent = send(m_hSocket, buf, len, 0)) == SOCKET_ERROR) 
	{
		throw SockException(WSAGetLastError());
	}
	return nBytesSent;
}

int CBlockingSocket::Receive(char* buf, size_t len, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, &fd, NULL, NULL, &tv) == 0) 
	{
		throw TimerException("Receive : time out");
	}

	int nBytesReceived;
	if((nBytesReceived = recv(m_hSocket, buf, len, 0)) 
					== SOCKET_ERROR) 
	{
		throw SockException(WSAGetLastError());
	}
	return nBytesReceived;
}

int CBlockingSocket::ReceiveDatagram(char* buf, size_t len, SockAddr& sa, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, &fd, NULL, NULL, &tv) == 0) 
	{
		throw TimerException("Receive : time out");
	}

	// input buffer should be big enough for the entire datagram
	int nFromSize = sizeof(SOCKADDR);
	int nBytesReceived = recvfrom(	m_hSocket, buf, len, 
									0, (sockaddr*)&sa, &nFromSize);
	if(nBytesReceived == SOCKET_ERROR) 
	{
		throw SockException(WSAGetLastError());
	}
	return nBytesReceived;
}

int CBlockingSocket::SendDatagram(const char* buf, size_t len, const SockAddr& sa, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) 
	{
		throw TimerException("Send : time out");
	}

	int nBytesSent = sendto(m_hSocket, buf, len, 0, (sockaddr*)&sa, sizeof(SOCKADDR));
	if(nBytesSent == SOCKET_ERROR)
	{
		throw SockException(WSAGetLastError());
	}
	return nBytesSent;
}


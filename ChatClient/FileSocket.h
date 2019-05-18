// FileSocket.h: interface for the CFileSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILESOCKET_H__42A1D195_0D19_43A4_A11D_B2F0F7042615__INCLUDED_)
#define AFX_FILESOCKET_H__42A1D195_0D19_43A4_A11D_B2F0F7042615__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "blockingsocket.h"
#include "chatexc.h"

class CSignal
{
public:
	virtual ~CSignal(){}
	virtual void setlen(int len)		= 0;
	virtual void add(int len)			= 0;
	virtual void quit()					= 0;
	virtual	bool report(CString file)	= 0;
	virtual CString getdir()			= 0;
	virtual void error()				= 0;
};

class CFileSocket : public CBlockingSocket  
{
	enum
	{
		hdr_name		= 1,
		hdr_size		= 2,
		hdr_fulldata	= 3,
		hdr_enddata		= 4,
		hdr_ack			= 5,
		hdr_notack		= 6
	};

	int	SendWithResponse(const char* buf, size_t len)
	{
		char resp[10];
		Send(buf, len);
	//	Sleep(10);
		do
		{
			Receive(resp, 10);
		}
		while(resp[0] != hdr_ack);
		return resp[1];
	};
	void	SendResponse(bool res=true)
	{
		char resp[10];
		resp[0] = hdr_ack;
		resp[1] = res?hdr_ack : hdr_notack;
		Send(resp, 10);
	};
	bool	Valid(bool server)
	{
		HANDLE hEv = OpenEvent(EVENT_ALL_ACCESS, TRUE, server? _T("SyncServer"): _T("SyncClient"));
		if(!hEv)  return false;
		CloseHandle(hEv);
		return true;
	}
public:
	CFileSocket();
	virtual ~CFileSocket();

	bool	ReceiveFile(CSignal& sign);
	bool	SendFile( CString& name, CSignal& sign);
};

#endif // !defined(AFX_FILESOCKET_H__42A1D195_0D19_43A4_A11D_B2F0F7042615__INCLUDED_)

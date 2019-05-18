// TSocket.h: interface for the CTSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TSOCKET_H__13DED9E7_A30E_475F_BA7E_C539A8320065__INCLUDED_)
#define AFX_TSOCKET_H__13DED9E7_A30E_475F_BA7E_C539A8320065__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define IP_PORT 60000
#define BYTES   64

class CTSocket  
{
	SOCKET m_Socket;
	CProgressCtrl *m_Progress;
	int m_Port;
	int m_BytesSend;
	struct sockaddr_in m_sockaddr;
public:
	CTSocket();
	CTSocket(const SOCKET& sc);
	virtual ~CTSocket();
	BOOL SendText(const wchar_t* Buf, int Len);
	BOOL ReceiveText(wchar_t* Buf);
	BOOL Connect(char* ServerName);
	BOOL Listen();
	BOOL Bind();
	SOCKET Accept();
	void SetProgrBar(CProgressCtrl* pProgrCtrl);
	BOOL ReceiveFile(CString FileName);
	BOOL SendFile(CString FileName);
	void Close() { closesocket(m_Socket);}

};

#endif // !defined(AFX_TSOCKET_H__13DED9E7_A30E_475F_BA7E_C539A8320065__INCLUDED_)

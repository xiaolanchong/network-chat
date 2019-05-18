// TSocket.cpp: implementation of the CTSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "TCP.h"
#include "TSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTSocket::CTSocket()
{
	m_Port = IP_PORT;
	m_BytesSend = BYTES;
	m_Socket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if (m_Socket==INVALID_SOCKET) 
	{
		AfxMessageBox(_T("Error create socket :-(\n"));
		exit(1);
	}
}
CTSocket::CTSocket(const SOCKET& sc)
{
	m_Port = IP_PORT;
	m_BytesSend = BYTES;
	m_Socket = sc;
}

CTSocket::~CTSocket()
{
//	closesocket(m_Socket);
	shutdown(m_Socket, 2);
}

BOOL CTSocket::Bind()
{
	SOCKADDR_IN socketaddr;
	socketaddr.sin_family = AF_INET;
	socketaddr.sin_addr.s_addr  = INADDR_ANY; 
	socketaddr.sin_port = htons(m_Port);

	if (bind(m_Socket,(LPSOCKADDR)&socketaddr,sizeof(socketaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Error bind socket"));
		exit(1);
	}
	return TRUE;

}

BOOL CTSocket::Listen()
{
	int Errors;
	Errors = listen(m_Socket,10);
	if ( Errors == SOCKET_ERROR)
	{
		AfxMessageBox(_T(" Listen BAD !!! "));
		exit(1);
	}
	return TRUE;
}

SOCKET CTSocket::Accept()
{	
	SOCKADDR_IN from;
	int fromlen = sizeof(from);
	return accept(m_Socket, (LPSOCKADDR)&from, &fromlen);
}

BOOL CTSocket::Connect(const char* ServerName)
{
	SOCKADDR_IN socketaddr;
	

	socketaddr.sin_family = AF_INET;
	socketaddr.sin_port = htons(m_Port);
	socketaddr.sin_addr.S_un.S_addr = inet_addr(ServerName);
	
	
	if (connect(m_Socket, (LPSOCKADDR)&socketaddr, sizeof(socketaddr)) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("Error connect to socket"));
		exit(1);
	}
	return TRUE;
}

void CTSocket::SetProgrBar(CProgressCtrl *pProgrCtrl)
{
	m_Progress = pProgrCtrl;
}

BOOL CTSocket::SendText(const wchar_t* Buf, int Len)
{
	send(m_Socket, reinterpret_cast<const char*>(Buf) , Len * sizeof(wchar_t), 0);
	return TRUE;
}

BOOL CTSocket::SendData(const char* Buf, int Len)
{
	send(m_Socket, reinterpret_cast<const char*>(Buf), Len, 0);
	return TRUE;
}


BOOL CTSocket::ReceiveText(wchar_t* Buf)
{
	ZeroMemory(Buf, m_BytesSend * sizeof(wchar_t));
	recv(m_Socket, reinterpret_cast<char*>(Buf), m_BytesSend * sizeof(wchar_t),0);
	return TRUE;
}

BOOL CTSocket::ReceiveData(char* Buf)
{
	ZeroMemory(Buf, m_BytesSend);
	recv(m_Socket, reinterpret_cast<char*>(Buf), m_BytesSend * sizeof(wchar_t), 0);
	return TRUE;
}

BOOL CTSocket::SendFile(CString FileName)
{
#ifdef FILE_SENDING
	CFile File;
	char* buffor = new char[BYTES];
	CString tempfor;
	int blockSize = BYTES ;
	CString temp;
	CString tempString;
	char* buf = new char[blockSize];
	ZeroMemory(buf, sizeof(wchar_t)* blockSize);
	File.Open(FileName, CFile::modeRead);

	int FileLength = File.GetLength();
		
	int blockCount = FileLength/blockSize;
	int LastBlockSize = FileLength%blockSize;
	char* LastBuf = new char[LastBlockSize];
	ZeroMemory(LastBuf, sizeof(LastBuf));

//��� �����
	SendText((FileName), FileName.GetLength());
	ReceiveText(buffor);
	tempfor = buffor;
	if(tempfor != "OK_NAME")
	{
		AfxMessageBox(_T("������ �����"));
		return FALSE;
	}
	tempString.Format(_T("%d"), FileLength);
//����� �����
	SendText((tempString), tempString.GetLength());
	ReceiveText(buffor);
	tempfor = buffor;
	if(tempfor != "OK_LEN")
	{
		AfxMessageBox(_T("������ �����"));
		return FALSE;
	}
	temp.Format(_T("%d"), LastBlockSize);
//����� ���������� �����
	SendText(temp, temp.GetLength());
	ReceiveText(buffor);
	tempfor = buffor;
	if(tempfor != "OK_LASTLEN")
	{
		AfxMessageBox(_T("������ ������. �����"));
		return FALSE;
	}
	int i=0;
	while(i<blockCount)
	{

		m_Progress->SetPos(double(i)/blockCount*100);
		File.Read(buf , blockSize);
		SendText(buf, blockSize);
		ReceiveText(buffor);
		tempfor = buffor;
		if(tempfor != "OK")
		{
			AfxMessageBox(_T("������ ������"));
			return FALSE;
		}
		i++;
	}

	SendText(_T("LAST_BLOCK"), 10);
	ReceiveText(buffor);
		tempfor = buffor;
		if(tempfor != "LASTBLOCK")
		{
			AfxMessageBox(_T("LASTBLOCK"));
			return FALSE;
		}
	if(LastBlockSize!=0)
	{
		File.Read(LastBuf, LastBlockSize);
		SendData(LastBuf, LastBlockSize);
		ReceiveText(buffor);
		tempfor = buffor;
		if(tempfor != "OK_LASTBLOCK")
		{
			AfxMessageBox(_T("������ ���������� �����"));
			return FALSE;
		}
	}

	SendText(_T("FILE_END"), 8);
	File.Close();
	m_Progress->SetPos(0);
	delete buf;
	delete buffor;
	delete LastBuf;
	return TRUE;
#else
	return FALSE;
#endif
}

BOOL CTSocket::ReceiveFile(CString FileName)
{
#ifdef FILE_SENDING
	CFile file;
	CFileException e;
	CString buf, tempString;
	char* tempBuf= new char[BYTES];
	int LastBlockCount;

	int addrlen = sizeof(m_sockaddr);
//����� �����
	ReceiveText(tempBuf);
	SendText(_T("OK_LEN"), 6);
	int FileLength =  atoi(tempBuf);
	int BlockCount = FileLength/m_BytesSend;
	int i=0;

//����� ���������� �����
	ReceiveText(tempBuf);
	SendText(_T("OK_LASTLEN"), 10);
	LastBlockCount =  atoi(tempBuf);
	file.Open(FileName, CFile::modeCreate | CFile::modeWrite, &e);
	for(;;)
	{
		m_Progress->SetPos(double(i)/BlockCount*100);
		ReceiveText(tempBuf);
		tempString = tempBuf;
		if(tempString == "LAST_BLOCK")break;
		SendText(_T("OK"), 2);
		file.Write(tempBuf, m_BytesSend);
		i++;		
	}
	SendText(_T("LASTBLOCK"), 9);
	if(i != BlockCount)
		AfxMessageBox(_T("LOST DATE"));
	if(LastBlockCount!=0)
	{
		
		ReceiveText(tempBuf);
		SendText(_T("OK_LASTBLOCK"), 12);
		file.Write(tempBuf, LastBlockCount);
		
	}

	ReceiveText(tempBuf);
	tempString = tempBuf;
	while(tempString != "FILE_END")
	{
		AfxMessageBox(_T("��� ����� �����"));
		AfxMessageBox(tempBuf);
		exit(1);
	}
	file.Close();
	m_Progress->SetPos(0);
	delete[] tempBuf;
	return TRUE;
#else
	return FALSE;
#endif
}
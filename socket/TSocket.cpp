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

BOOL CTSocket::Connect(char* ServerName)
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

BOOL CTSocket::ReceiveText(wchar_t* Buf)
{
	ZeroMemory(Buf, m_BytesSend * sizeof(wchar_t));
	recv(m_Socket, Buf, m_BytesSend,0);
	return TRUE;
}

BOOL CTSocket::SendFile(CString FileName)
{
	CFile File;
	wchar_t* buffor = new wchar_t[BYTES];
	CString tempfor;
	int blockSize = BYTES * ;
	CString temp;
	CString tempString;
	wchar_t* buf = new wchar_t[blockSize];
	ZeroMemory(buf, sizeof(wchar_t)* blockSize);
	File.Open(FileName, CFile::modeRead);

	int FileLength = File.GetLength();
		
	int blockCount = FileLength/blockSize;
	int LastBlockSize = FileLength%blockSize;
	char* LastBuf = new char[LastBlockSize];
	ZeroMemory(LastBuf, sizeof(LastBuf));

//имя файла
	SendText(CT2A(FileName), FileName.GetLength());
	ReceiveText(buffor);
	tempfor = buffor;
	if(tempfor != "OK_NAME")
	{
		AfxMessageBox(_T("потеря имени"));
		return FALSE;
	}
	tempString.Format(_T("%d"), FileLength);
//длина файла
	SendText(CT2A(tempString), tempString.GetLength());
	ReceiveText(buffor);
	tempfor = buffor;
	if(tempfor != "OK_LEN")
	{
		AfxMessageBox("потеря длины");
		return FALSE;
	}
	temp.Format(_T("%d"), LastBlockSize);
//длина последнего блока
	SendText(temp, temp.GetLength());
	ReceiveText(buffor);
	tempfor = buffor;
	if(tempfor != "OK_LASTLEN")
	{
		AfxMessageBox("потеря послед. длины");
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
			AfxMessageBox("потеря данных");
			return FALSE;
		}
		i++;
	}

	SendText("LAST_BLOCK", 10);
	ReceiveText(buffor);
		tempfor = buffor;
		if(tempfor != "LASTBLOCK")
		{
			AfxMessageBox("LASTBLOCK");
			return FALSE;
		}
	if(LastBlockSize!=0)
	{
		File.Read(LastBuf, LastBlockSize);
		SendText(LastBuf, LastBlockSize);
		ReceiveText(buffor);
		tempfor = buffor;
		if(tempfor != "OK_LASTBLOCK")
		{
			AfxMessageBox("потеря последнего блока");
			return FALSE;
		}
	}

	SendText("FILE_END", 8);
	File.Close();
	m_Progress->SetPos(0);
	delete buf;
	delete buffor;
	delete LastBuf;
	return TRUE;
}

BOOL CTSocket::ReceiveFile(CString FileName)
{
	CFile file;
	CFileException e;
	CString buf, tempString;
	char* tempBuf= new char[BYTES];
	int LastBlockCount;

	int addrlen = sizeof(m_sockaddr);
//длина файла
	ReceiveText(tempBuf);
	SendText("OK_LEN", 6);
	int FileLength =  atoi(tempBuf);
	int BlockCount = FileLength/m_BytesSend;
	int i=0;

//длина последнего блока
	ReceiveText(tempBuf);
	SendText("OK_LASTLEN", 10);
	LastBlockCount =  atoi(tempBuf);
	file.Open(FileName, CFile::modeCreate | CFile::modeWrite, &e);
	for(;;)
	{
		m_Progress->SetPos(double(i)/BlockCount*100);
		ReceiveText(tempBuf);
		tempString = tempBuf;
		if(tempString == "LAST_BLOCK")break;
		SendText("OK", 2);
		file.Write(tempBuf, m_BytesSend);
		i++;		
	}
	SendText("LASTBLOCK", 9);
	if(i != BlockCount)
		AfxMessageBox(_T("LOST DATE"));
	if(LastBlockCount!=0)
	{
		
		ReceiveText(tempBuf);
		SendText("OK_LASTBLOCK", 12);		
		file.Write(tempBuf, LastBlockCount);
		
	}

	ReceiveText(tempBuf);
	tempString = tempBuf;
	while(tempString != "FILE_END")
	{
		AfxMessageBox(_T("нет конца файла"));
		AfxMessageBox(tempBuf);
		exit(1);
	}
	file.Close();
	m_Progress->SetPos(0);
	delete[] tempBuf;
	return TRUE;
}
// FileSocket.cpp: implementation of the CFileSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Socket_UDP.h"
#include "FileSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileSocket::CFileSocket()
{

}

CFileSocket::~CFileSocket()
{

}

bool	CFileSocket::ReceiveFile( CSignal& sign)
{
	CString FileName,SavePath1;
	const int buflen = 1300;
	char    buf[buflen];
	CFile* pFile = NULL;

	try
	{
	for(;;)
	{
		int r = Receive(buf,buflen);
		if(buf[0] == hdr_name)
		{
			SendResponse();			

			char str[128];
			memcpy(str, buf+1, r -1);
			str[r-1] = '\0';
			FileName = str;
		//	G += str;
	/*		int res = AfxMessageBox(G, MB_YESNO|MB_ICONQUESTION);
			if(res == IDNO)
				return FALSE;*/
			if(!sign.report(CString(str)))
				return false;
			SavePath1 = sign.getdir() + "\\";
			SECURITY_ATTRIBUTES lpSecurityAttributes;
			lpSecurityAttributes.nLength=sizeof(lpSecurityAttributes);
			lpSecurityAttributes.lpSecurityDescriptor=NULL;
			lpSecurityAttributes.bInheritHandle=true;
			CreateDirectory(SavePath1,&lpSecurityAttributes);
			break;
		}
		
	}
	SavePath1 += FileName;
	pFile = new CFile(SavePath1,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	int fr = Receive(buf, 5, 1000);
	if(buf[0] == hdr_size)
	{
		DWORD fsize;
		memcpy(&fsize, buf+1, 4);
		sign.setlen(fsize);
		SendResponse();
	}
	switch(SOCK_STREAM)
	{
		case SOCK_STREAM:
			for(;;)
			{
				int fr = Receive(buf,buflen, 1);

				if(buf[0] == hdr_enddata)	
				{
					pFile->Write(buf + 1, fr -1);
					sign.add(fr-1);
					if(!Valid(true))
							throw send_exc("You cancel sending");
					SendResponse();
					break;
				}
				else if(buf[0] == hdr_fulldata)
				{
					pFile->Write(buf + 1, fr -1);	
					SendResponse();
					if(!Valid(true))
						throw send_exc("You cancel sending");
					sign.add(fr-1);
				}
		//		Sleep(100);	
			}

	}
	}
	catch(SockException& ex)
	{
		sign.error();
		if(pFile)
			pFile->Close();
		throw ex; 
	}
	catch(TimerException& ex)
	{
		sign.error();
		if(pFile)
			pFile->Close();
		throw ex; 
	}
	catch(send_exc& ex)
	{
		//sign.error();
		if(pFile)
			pFile->Close();
		throw ex; 
	}
	sign.quit();
	if(pFile) 
	{
		pFile->Close();
		delete pFile;
	}
	return true;	
}

bool	CFileSocket::SendFile(CString& name, CSignal& sign)
{
#ifdef FILE_SENDING
	const int buflen = 1200;
	CFile	File;
	CString temp;
	char	buf[buflen];
	char	filebuf[buflen];
	int		curbuf = 0;
	try
	{
	BOOL xz = File.Open(name,CFile::modeRead|CFile::typeBinary);
	buf[0] = hdr_name; // send name
	curbuf += 1;
	int i;
	for(i=name.GetLength()-2;i>=0;i--)
		if(name.GetAt(i)=='\\')
			break;
	temp = name.Right(name.GetLength()-i-1);
	//ןמסûכאול טלÿ פאיכא
	memcpy( buf+curbuf , (const char*)temp, temp.GetLength());
	curbuf += temp.GetLength();
	DWORD fsize = File.GetLength();
	
	int res = SendWithResponse(buf, curbuf);
	//Receive(buf, buflen);
	if(res != hdr_ack)
		throw send_exc("Peer not acknowledged sending");
	buf[0] = hdr_size; // send size
	curbuf = 1;
	memcpy(buf+curbuf, (char*)&fsize, 4);
	curbuf += 4;
	SendWithResponse(buf, curbuf);
	if(!Valid(false))
		throw send_exc("You cancel sending");
	int Type = SOCK_STREAM;
	sign.setlen(File.GetLength());
	switch(Type)
	{
			case SOCK_STREAM:
				for(;;)
				{
					
					int fr = File.Read(filebuf, buflen - 1);
					if(fr < buflen - 1)
					{
						buf[0] = hdr_enddata; // end
						memcpy(buf + 1, filebuf, fr);
						SendWithResponse(buf, fr + 1);
						if(!Valid(false))
							throw send_exc("You cancel sending");
						sign.add(fr);
						break;
					}
					else
					{
						buf[0] = hdr_fulldata; // data
						memcpy(buf + 1, filebuf, fr);
						SendWithResponse(buf, fr + 1);
						if(!Valid(false))
							throw send_exc("You cancel sending");
						sign.add(fr);
					}
		//			Sleep(50);
				}
	}	
	}
	catch(SockException& ex)
	{
		sign.error();
		File.Close();
		throw ex; 
	}
	catch(TimerException& ex)
	{
		sign.error();
		File.Close();
		throw ex; 
	}
	catch(send_exc& ex)
	{
		//sign.error();
		File.Close();
		throw ex; 
	}
	sign.quit();
	File.Close();
	return true;
#else
	return false;
#endif
}


// ChatServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatServer.h"
#include "ChatServerDlg.h"
#include "../common/Utils.h"
#include "../common/Commands.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog

CChatServerDlg::CChatServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatServerDlg)
	DDX_Control(pDX, IDC_LISTPEOPLE, m_ListPeople);
	DDX_Control(pDX, IDC_LISTCHAT, m_ListChat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatServerDlg, CDialog)
	//{{AFX_MSG_MAP(CChatServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_KICK, OnKick)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg message handlers

BOOL CChatServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
//	CImageList img;
//	img.Create(IDB_FACE, 16, 100, RGB(255,0,255));
	m_wndTask.Create(this);
	m_wndTask.SetIcon(AfxGetApp()->LoadIcon(IDI_SERVER), FALSE);
	m_wndTask.SetTooltipText(CString("Chat server started"));
	m_wndTask.Show();



	m_pMainServerThread = AfxBeginThread((AFX_THREADPROC)ServerMainThreadFunc, this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	if(nID == SC_MINIMIZE )
		ShowWindow(SW_HIDE);
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CChatServerDlg::OnKick() 
{
	CString Nick;
	CString IP;
	for(int i=0;i<m_ListPeople.GetCount();i++)
		if(m_ListPeople.GetSel(i))
		{
			m_ListPeople.GetText(i, Nick);
			for(int j=0;j<m_ServerList.size();j++)
				if(m_ServerList.at(j).m_Nick == Nick)
				{
					TerminateThread (m_ListThread.at(j).m_pServerThread,0);
					m_ListThread.erase(m_ListThread.begin()+j);
					CString Message = commands::c_disconnectUser;
					utils::SendString(m_ServerList.at(j).m_Socket, Message);
					m_ServerList.erase(m_ServerList.begin()+j);
					break;
				}
			

			for(int k=0;k<m_ServerList.size();k++)
			{
				CString Message = "REMOVE " + Nick;
				utils::SendString(m_ServerList.at(k).m_Socket, Message);
			}

		

			m_ListPeople.DeleteString(i);
			m_ClientCount--;
			break;
		}
	
}

/////////////////////////////////////////////////////
void CChatServerDlg::ServerThreadFunc(LPVOID param)
{
	CChatServerDlg* pWnd = (CChatServerDlg*)param;
	wchar_t* buf = new wchar_t[1024];
	wchar_t* Nick = new wchar_t[1024];
	CString csCommand;
	CString IP;
	BOOL AlwaysExist;
	CString csNick;
	int p=0;
	for(;p<pWnd->m_ServerList.size();p++)
		if(pWnd->m_ServerList.at(p).m_New == TRUE)
		{
			pWnd->m_ServerList.at(p).m_New = FALSE;
			
			break;
		}
	CTSocket sc(pWnd->m_ServerList.at(p).m_Socket);
	try
	{
	while(1)
	{
		
		ZeroMemory(buf, 1024);
		AlwaysExist = FALSE;
		
		sc.ReceiveText(buf);
		csCommand = buf;
		csCommand.Delete(7, csCommand.GetLength()-7);
		Nick = buf+7;

////////////////Disconnect/////////////////////////////////////
		if(csCommand == "CLIDISC")
		{
			CString Name;
			for(int p=0;p<pWnd->m_ServerList.size();p++)
			{
				if(pWnd->m_ServerList.at(p).m_Nick == Nick)
				{
					pWnd->m_ServerList.erase(pWnd->m_ServerList.begin()+p);
					break;
			//		throw std::exception();
				}
			}
			for(int k=0;k<pWnd->m_ServerList.size();k++)
			{
				CString Message = "REMOVE ";
				Message+=Nick;
				utils::SendString(pWnd->m_ServerList.at(k).m_Socket, Message);
			}
			for(int j=0;j<pWnd->m_ListPeople.GetCount();j++)
			{
				pWnd->m_ListPeople.GetText(j, Name);
				if(Name == Nick)
				{
					pWnd->m_ListPeople.DeleteString(j);
					break;
				//	throw std::exception();
				}
			}
			throw std::exception();

		}
////////////////Message////////////////////////////
		if(csCommand == "MESSAGE")
		{
			pWnd->m_ListChat.AddString(CString(Nick));
			for(int k=0;k<pWnd->m_ServerList.size();k++)
			{
				CString Message = "MESSAGE";
				Message+=Nick;
				utils::SendString(pWnd->m_ServerList.at(k).m_Socket,Message);
			}
		}
		if(csCommand == "PRIVMES")
		{
		
			CString t(Nick);
			CString from  = t.Left (t.Find(_T("\n")));
			t.Delete(0, t.Find(_T("\n"))+1);
			CString to = t.Left(t.Find(_T("\n")));
			CString all = _T("PM to ");
			all += Nick;
			pWnd->m_ListChat.AddString(all);
			for(int k=0;k<pWnd->m_ServerList.size();k++)
			{
				CString Message = _T("PRIVMES");
				Message+=Nick;
			//	if(to == pWnd->m_ServerList.at(k).m_Nick)
				utils::SendString(pWnd->m_ServerList.at(k).m_Socket,Message);
			}
		}
		if(csCommand == "GETHOST")
		{
		
			CString to(Nick);
			CString from  = to.Left (to.Find(_T("\n")));
			to.Delete(0, to.Find(_T("\n"))+1);
			CString all = _T("GH to ");
			all += Nick;
			pWnd->m_ListChat.AddString(all);
			char buf[commands::c_commandLength + sizeof(sockaddr)]="SETHOST";
			SOCKET peer;
			for(int k=0;k<pWnd->m_ServerList.size();k++)
			{
				if(to == pWnd->m_ServerList.at(k).m_Nick)
				{
					peer = pWnd->m_ServerList.at(k).m_Socket;
				}
			}
			for(int k=0;k<pWnd->m_ServerList.size();k++)
			{
				if(from == pWnd->m_ServerList.at(k).m_Nick)
				{
					int namlen = sizeof(sockaddr);
					sockaddr nam ;
					getpeername(peer, &nam, &namlen);
					memcpy(buf+ commands::c_commandLength, &nam, sizeof(sockaddr));
					send(pWnd->m_ServerList.at(k).m_Socket,buf, sizeof(buf), 0);
				}
			}
		}
	}
	}
	catch(std::exception&)
	{
		delete[] buf;
		return;
	}
	delete[] buf;
}

void CChatServerDlg::ServerMainThreadFunc(LPVOID param)
{
	CChatServerDlg* pWnd = (CChatServerDlg*)param;
	wchar_t* buf = new wchar_t[1024];
	CString IP="";
	CString Nick="";
	BOOL Exist = FALSE;
	pWnd->m_ClientCount = 0;
	pWnd->m_pMainSocket = new CTSocket();
	pWnd->m_pMainSocket->Bind();

	while(1){
		Exist = FALSE;
		SOCKET New;
		
		pWnd->m_pMainSocket->Listen();
		/*IP*/ New = pWnd->m_pMainSocket->Accept(/*&New*/);
		pWnd->m_pSocket = new CTSocket(New);
		pWnd->m_ClientCount++;
				
		pWnd->m_pSocket->ReceiveText(buf);
		Nick = buf;
		for(int p=0;p<pWnd->m_ServerList.size();p++)
			if(pWnd->m_ServerList.at(p).m_Nick == Nick)
			{
				pWnd->m_pSocket->SendText(_T("EXIST  "), 8);
				Exist = TRUE;
				break;
			}
		if(!Exist)
		{
			pWnd->m_ListPeople.AddString(CString(buf));

			for(int i=0;i<pWnd->m_ServerList.size();i++)
			{
				CString Message = "ADD    " + Nick;
				utils::SendString(pWnd->m_ServerList.at(i).m_Socket ,Message);
			}
			pWnd->m_ServerList.push_back(SERVERLISTCLIENT(New, IP, buf, TRUE));

			CString All = _T("ALL    ");
			for(int j=0;j<pWnd->m_ServerList.size();j++)
			{
				All+= pWnd->m_ServerList.at(j).m_Nick;
				All+='\n';
			}
			utils::SendString(New ,All);
			CWinThread* thread = AfxBeginThread((AFX_THREADPROC)ServerThreadFunc, param);
			pWnd->m_ListThread.push_back(LISTTHREAD(thread,Nick));
		}
		
	}
	delete[] buf;
}

void CChatServerDlg::OnClose() 
{
	
	for(int j=0;j<m_ServerList.size();j++)
	{
		CString Message = commands::c_disconnectUser;
		utils::SendString(m_ServerList.at(j).m_Socket, Message);
		
	}
	for(int j=0;j<m_ServerList.size();j++)
	{
		TerminateThread (m_ListThread.at(j).m_pServerThread,0);
		m_ListThread.erase(m_ListThread.begin()+j);
		m_ServerList.erase(m_ServerList.begin()+j);
	}
	TerminateThread (m_pMainServerThread,0);
	CDialog::OnClose();
	PostQuitMessage(0);
}


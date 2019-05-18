// ChatClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChatClient.h"
#include "ChatClientDlg.h"
#include "Address.h"

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
// CChatClientDlg dialog

CChatClientDlg::CChatClientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChatClientDlg::IDD, pParent),
		m_dlg(false, AnyAddr(0), this)
{
	//{{AFX_DATA_INIT(CChatClientDlg)
	m_sMessage = _T("");
	m_sName = _T("User");
	m_sServer = _T("");
	m_bConnect = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatClientDlg)
	DDX_Control(pDX, IDC_LISTPEOPLE, m_ListPeople);
	DDX_Control(pDX, IDC_LISTCHAT, m_ListChat);
	DDX_Text(pDX, IDC_EDITMESSAGE, m_sMessage);
	DDX_Text(pDX, IDC_EDITNAME, m_sName);
	DDX_Text(pDX, IDC_EDITSERVER, m_sServer);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatClientDlg, CDialog)
	//{{AFX_MSG_MAP(CChatClientDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CONNECT, OnConnect)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PRIVATE, OnPrivate)
	ON_WM_CHAR()
	ON_BN_CLICKED(IDC_FILE, OnSendfile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatClientDlg message handlers

BOOL CChatClientDlg::OnInitDialog()
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
	GetDlgItem(IDC_SEND)->EnableWindow( FALSE );
	GetDlgItem(IDC_DISCONNECT)->EnableWindow( FALSE );
	GetDlgItem(IDC_PRIVATE)->EnableWindow( FALSE );
	GetDlgItem(IDC_FILE)->EnableWindow( FALSE );
	char buf[100];
	gethostname(buf, 100);
	SetDlgItemText(IDC_EDITSERVER, buf);

	m_dlg.Create(IDD_SOCKET_UDP_DIALOG);
	m_dlg.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CChatClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatClientDlg::OnPaint() 
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
HCURSOR CChatClientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CChatClientDlg::OnConnect() 
{
	UpdateData();
	char* Buf = new char[BYTES];
	Buf = m_sServer.GetBuffer(1);
	m_pClient = new CTSocket;
	CString Message = m_sName;
	int   nAdapter = 0;
	CString IP = "";
	HOSTENT* sh = gethostbyname(m_sServer);
	while (	sh->h_addr_list[nAdapter]  )
	{
		struct   sockaddr_in   adr;
		memcpy(&adr.sin_addr,sh->h_addr_list[nAdapter],sh->h_length); 
		IP+=inet_ntoa(adr.sin_addr);
		nAdapter++;
	
	}
	if(m_pClient->Connect(IP.GetBuffer(IP.GetLength())))
	{
		m_bConnect = TRUE;
		GetDlgItem(IDC_SEND)->EnableWindow( m_bConnect );
		GetDlgItem(IDC_PRIVATE)->EnableWindow( m_bConnect );
		GetDlgItem(IDC_FILE)->EnableWindow( m_bConnect );
		GetDlgItem(IDC_CONNECT)->EnableWindow( FALSE );
		GetDlgItem(IDC_DISCONNECT)->EnableWindow( TRUE );
		GetDlgItem(IDC_EDITNAME)->EnableWindow( FALSE );
		m_pClient->SendText(Message.GetBuffer(Message.GetLength()), Message.GetLength());
		m_pClientThread = AfxBeginThread((AFX_THREADPROC)ClientThreadFunction, this);
	}
	
}

void CChatClientDlg::OnSend() 
{
	UpdateData();
	CString Message = "MESSAGE";
	Message+=m_sName;
	Message+=" : ";
	CString temp;
	if(m_bConnect)
	{
		GetDlgItemText(IDC_EDITMESSAGE, temp);
		Message+=temp;
		m_pClient->SendText(Message, Message.GetLength());
		SetDlgItemText(IDC_EDITMESSAGE, "");
	}	
}

void CChatClientDlg::ClientThreadFunction(LPVOID Param)
{
	CChatClientDlg* pWnd = (CChatClientDlg*)Param;
	
	char* buf = new char[1024];
	
	CString csCommand="";
	CString csNick;

	while(1)
	{
		pWnd->m_pClient->ReceiveText(buf);
		if(WSAGetLastError() != 0) return;
		csCommand = buf;
		csCommand.Delete(7,csCommand.GetLength()-7);
		char* Nick = buf+7;
		csNick = Nick;


		if(csCommand == "ALL    ")
		{
			csNick = "";
			for(int i=0;i< (strlen(buf)-7);i++)
			{	
				if(Nick[i] == '\n')
				{
					pWnd->m_ListPeople.AddString(csNick);
					csNick="";
					continue;
				}
				csNick+=Nick[i];
				
			}
		}

		if(csCommand == "ADD    ")
				pWnd->m_ListPeople.AddString(Nick);
		
		if(csCommand == "EXIST  ")
		{
			AfxMessageBox("пользователь с таким именем уже существует");
			pWnd->m_bConnect = FALSE;
			pWnd->GetDlgItem(IDC_SEND)->EnableWindow( FALSE );
			pWnd->GetDlgItem(IDC_PRIVATE)->EnableWindow( FALSE );
			pWnd->GetDlgItem(IDC_FILE)->EnableWindow( FALSE );
			pWnd->GetDlgItem(IDC_CONNECT)->EnableWindow( TRUE );
			pWnd->GetDlgItem(IDC_DISCONNECT)->EnableWindow( FALSE );
			pWnd->GetDlgItem(IDC_EDITNAME)->EnableWindow( TRUE );
			pWnd->m_ListPeople.ResetContent();
			TerminateThread(pWnd->m_pClientThread, 0);

		}
		if(csCommand == "DISCONN")
		{
			AfxMessageBox("Вы отключены администратором чата");
			pWnd->m_bConnect = FALSE;
			pWnd->GetDlgItem(IDC_SEND)->EnableWindow( FALSE );
			pWnd->GetDlgItem(IDC_CONNECT)->EnableWindow( TRUE );
			pWnd->GetDlgItem(IDC_DISCONNECT)->EnableWindow( FALSE );
			pWnd->GetDlgItem(IDC_EDITNAME)->EnableWindow( TRUE );
			pWnd->m_ListPeople.ResetContent();
			TerminateThread(pWnd->m_pClientThread, 0);
		}
		if(csCommand == "REMOVE ")
		{
			CString temp;
			for(int i=0;i<pWnd->m_ListPeople.GetCount();i++){
				pWnd->m_ListPeople.GetText(i,temp);
				if(temp == csNick)	
				{
					pWnd->m_ListPeople.DeleteString(i);
					break;
				}
			}		
		}
		if(csCommand == "MESSAGE")
			pWnd->m_ListChat.AddString(Nick);
		
		if(csCommand == "PRIVMES")
		{
			CString mes;
			CString n(Nick);
			CString to = n.Left(n.Find("\n"));
			if(pWnd->m_sName != to)
				continue;
			n.Delete(0, to.GetLength()+1); // cut  /n
			CString from = n.Left(n.Find("\n"));
			n.Delete(0, from.GetLength()+1); // cut  /n
			mes = "PM from ";
			mes += from;
			mes += ":";
			mes += n;		
			pWnd->m_ListChat.AddString(mes);
		}
		if(csCommand == "SETHOST")			// with server
		{
			SockAddr sa(1, "");
			memcpy(&sa, Nick, sizeof(sa));
//			AfxMessageBox(sa.host());
		/*	CSocket_UDPDlg* pDlg = new CSocket_UDPDlg(true, sa, pWnd); 
			pDlg->Create(IDD_SOCKET_UDP_DIALOG);*/
			CSocket_UDPDlg dlg(true, sa, pWnd);
			dlg.DoModal();
		}
	}
	
	delete[] buf;
	
}

void CChatClientDlg::OnDisconnect() 
{
	CString Message = "CLIDISC"+m_sName;
	m_bConnect = FALSE;
	GetDlgItem(IDC_SEND)->EnableWindow( m_bConnect );
	GetDlgItem(IDC_PRIVATE)->EnableWindow( m_bConnect );
	GetDlgItem(IDC_FILE)->EnableWindow( m_bConnect );
	GetDlgItem(IDC_CONNECT)->EnableWindow( TRUE );
	GetDlgItem(IDC_DISCONNECT)->EnableWindow( FALSE );
	GetDlgItem(IDC_EDITNAME)->EnableWindow( TRUE );
	m_pClient->SendText(Message, Message.GetLength());
	m_ListPeople.ResetContent();
//	TerminateThread(m_pClientThread, 0);
	m_pClient->Close();
}

void CChatClientDlg::OnClose() 
{
	if(m_bConnect)
	{
		m_bConnect = FALSE;
		CString Message = "CLIDISC"+m_sName;
		m_pClient->SendText(Message, Message.GetLength());
	//	TerminateThread(m_pClientThread, 0);	
		m_pClient->Close();
	}
	m_dlg.DestroyWindow();
	CDialog::OnClose();
}

void CChatClientDlg::OnPrivate() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString Message;
	int sel = m_ListPeople.GetCurSel();
	if(sel == LB_ERR)
	{
		AfxMessageBox("You must select user in list");
		return;
	}
	CString to;
	m_ListPeople.GetText(sel, to);
/*	if(to == m_sName)
	{
		AfxMessageBox("You can't send private message to yourself");
		return;
	}*/
	if(m_bConnect)
	{
	//	GetDlgItemText(IDC_EDITMESSAGE, temp);
		// PRIVMES to \n from(us) \n message
		CString text;
		GetDlgItemText(IDC_EDITMESSAGE, text);
		Message="PRIVMES";
		Message+=to;
		Message+="\n";
		Message+=m_sName;
		Message+="\n";
		Message+=text;
		//Message+=" : ";
		m_pClient->SendText(Message, Message.GetLength());
		SetDlgItemText(IDC_EDITMESSAGE, "");
		Message = "PM to ";
		Message += to;
		Message += " : ";
		Message += text;
		m_ListChat.AddString(Message);
	}	
}

void CChatClientDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
//	GetDlgItem(IDC_EDITMESSAGE)->SendMessage(WM_CHAR, nChar, WORD(nRepCnt)|nFlags);
/*	char buf[2] = { nChar, 0};
	AfxMessageBox(buf);*/
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CChatClientDlg::OnSendfile() 
{
	// TODO: Add your control notification handler code here
	CString Message("GETHOST");
	int sel = m_ListPeople.GetCurSel();
	if(sel == LB_ERR)
	{
		AfxMessageBox("You must select user in list");
		return;
	}
	CString to;
	m_ListPeople.GetText(sel, to);
/*	if(to == m_sName)
	{
		AfxMessageBox("You can't send private message to yourself");
		return;
	}*/
	// GETHOST from /n to
	Message += m_sName;
	Message += "\n";
	Message += to;
	if(m_bConnect)
		m_pClient->SendText(Message, Message.GetLength());
}

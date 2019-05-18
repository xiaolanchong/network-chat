// Socket_UDPDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "Socket_UDP.h"
#include "Socket_UDPDlg.h"
#include "Address.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSocket_UDPDlg dialog

CSocket_UDPDlg::CSocket_UDPDlg(bool bSend, SockAddr sa, CWnd* pParent /*=NULL*/)
	: CDialog(CSocket_UDPDlg::IDD, pParent),
	m_SendSign(&m_wndSend, true),
	m_RecvSign(&m_wndRecv, false),
	m_bSend(bSend),
	m_sa(sa)
	
{
	//{{AFX_DATA_INIT(CSocket_UDPDlg)
	m_SaveDir = _T("");
	m_Send_IP = _T("");
	m_File = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	if(!m_bSend)
		m_hEvent = CreateEvent(NULL, FALSE, FALSE, _T("SyncEv15"));
}

void CSocket_UDPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSocket_UDPDlg)
	DDX_Control(pDX, IDC_RECVFILE, m_wndRecv);
	DDX_Control(pDX, IDC_SENDFILE, m_wndSend);
	DDX_Text(pDX, IDC_EDIT_SAVEDIR, m_SaveDir);
	DDX_Text(pDX, IDC_ADDRESS, m_Send_IP);
	DDX_Text(pDX, IDC_EDIT_SEND, m_File);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSocket_UDPDlg, CDialog)
	//{{AFX_MSG_MAP(CSocket_UDPDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_FILE, OnButtonFile)
	ON_BN_CLICKED(IDC_PATH, OnPath)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSocket_UDPDlg message handlers

BOOL CSocket_UDPDlg::OnInitDialog()
{

	CDialog::OnInitDialog();
	SOCKADDR_IN st;
	st.sin_family=AF_INET;
	st.sin_port=5001;
	st.sin_addr.S_un.S_un_b.s_b1=127;
	st.sin_addr.S_un.S_un_b.s_b2=0;
	st.sin_addr.S_un.S_un_b.s_b3=0;
	st.sin_addr.S_un.S_un_b.s_b4=1;
	
	//s.CreateSocket(PF_INET,SOCK_DGRAM,0,&st);

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
//	m_wndSend.SetSnake();
//	m_wndSend.SetBkColor(RGB(256,128,128));
//	m_wndRecv.SetWindowText("Done");
//	m_wndRecv.SetShowPercent(TRUE);
	m_wndRecv.SetTextFormat(_T("Received %d%%"), PBS_SHOW_PERCENT);
	m_wndSend.SetTextFormat(_T("Sent %d%%"), PBS_SHOW_PERCENT);
	m_wndRecv.SetGradientColors(RGB(255,0,0), RGB(0, 255,0));
	m_wndSend.SetGradientColors(RGB(0,127,127), RGB(0, 0, 255));
	m_wndRecv.SetRubberBar();


	if(!m_bSend)
	{

		// TODO: Add your specialized creation code here
		//GetDlgItem(IDC_SEND)->ShowWindow(SW_HIDE);
		m_pServerThread=AfxBeginThread((AFX_THREADPROC)ServerThreadFunc, this);
		GetDlgItem(IDC_EDIT_SEND)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SEND)->SetWindowText(_T("Принять"));
		m_hValid = CreateEvent(NULL, FALSE, FALSE, _T("SyncServer"));
	}
	else
	{
//		GetDlgItem(IDC_BUTTON_SEND)->ShowWindow(SW_HIDE);
		ShowWindow(SW_HIDE);
		GetDlgItem(IDC_EDIT_SAVEDIR)->EnableWindow(FALSE);
		GetDlgItem(IDC_PATH)->EnableWindow(FALSE);
		m_hValid = CreateEvent(NULL, FALSE, FALSE, _T("SyncClient"));
//		GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSocket_UDPDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
/*		CAboutDlg dlgAbout;
		dlgAbout.DoModal();*/
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSocket_UDPDlg::OnPaint() 
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
HCURSOR CSocket_UDPDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSocket_UDPDlg::OnButtonSend() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if(m_SaveDir.IsEmpty() && !m_bSend)
	{
		AfxMessageBox(_T("Write directory for files"));
		return;
	}
	if(m_File.IsEmpty() && m_bSend)
	{
		AfxMessageBox(_T("Write file name"));
		return;
	}
	GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(FALSE);
	if(m_bSend)
		m_pClientThread=AfxBeginThread((AFX_THREADPROC)ClientThreadFunc, this);	
	else
		SetEvent(m_hEvent);
}

int CSocket_UDPDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
//	m_Signal.Len=0;
//	m_Signal.Signal=true;
//	m_Signal.Quit=false;
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(!m_bSend)
		m_SaveDir="C:\\";
	else
		m_SaveDir = "";
	m_Send_IP = m_sa.host();

	return 0;
}

void CSocket_UDPDlg::ServerThreadFunc(LPVOID pParam)
{
	CSocket_UDPDlg* pWnd = (CSocket_UDPDlg*)pParam;
	CFileSocket bs, cs;
	char szPath[127];
	gethostname(szPath, 127);
//	SockAddr sa(SERVER_PORT, szPath);
	pWnd->GetDlgItem(IDC_BUTTON_SEND)->EnableWindow(TRUE);
	pWnd->ShowWindow(SW_HIDE);
	AnyAddr sa(SERVER_PORT);
	try
	{
		bs.Create(SOCK_STREAM);
		bs.Bind(sa);
		bs.Listen();
		bs.Accept(cs, AnyAddr(111));
		pWnd->ShowWindow(SW_NORMAL);
		cs.ReceiveFile(pWnd->m_RecvSign);

	}
	catch(std::exception& ex)
	{
		AfxMessageBox(CA2T(ex.what()));
		return;
	}
	cs.Close();
	bs.Close();
	pWnd->m_pServerThread=AfxBeginThread((AFX_THREADPROC)ServerThreadFunc, pWnd);
}

void CSocket_UDPDlg::ClientThreadFunc(LPVOID  Param)
{
	CSocket_UDPDlg* pWnd = (CSocket_UDPDlg*)Param;
	CFileSocket bs;
	SockAddr sa(SERVER_PORT, pWnd->m_Send_IP);
	bs.Create();
	try
	{
		bs.Connect(sa);
		bs.SendFile(pWnd->m_File,pWnd->m_SendSign);
	}
	catch(std::exception& ex)
	{
		AfxMessageBox(CA2T(ex.what()));
	}
}

void CSocket_UDPDlg::OnButtonFile() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	UpdateData(true);
	CFileDialog FileDialog(TRUE, NULL , _T("*.*"), OFN_ALLOWMULTISELECT, _T("All files(*.*)"));
	if(FileDialog.DoModal() == IDOK)
		m_File=FileDialog.GetPathName();
	UpdateData(false);
	
}

void CSocket_UDPDlg::OnPath() 
{
	// TODO: Add your control notification handler code here
	TCHAR szPath[MAX_PATH];
	BROWSEINFO bi;
	LPITEMIDLIST pidlSelect;
	bi.hwndOwner = GetSafeHwnd();
    bi.pidlRoot = NULL;
    bi.pszDisplayName = szPath;
    bi.lpszTitle = _T("Choose a folder");
    bi.ulFlags = 0;
    bi.lpfn = NULL;
    bi.lParam = 0;

    pidlSelect = SHBrowseForFolder(&bi);
	if(pidlSelect)
	{
		SHGetPathFromIDList(pidlSelect, szPath) ;
		m_SaveDir = szPath;
	//	UpdateData(TRUE);
		SetDlgItemText(IDC_EDIT_SAVEDIR, szPath);
	}
}

void CSocket_UDPDlg::OnClose()
{
//	AfxMessageBox("А по башке?");
	CDialog::OnClose();
	CloseHandle(m_hValid);
}
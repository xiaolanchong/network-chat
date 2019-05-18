// Socket_UDPDlg.h : header file
//

#if !defined(AFX_SOCKET_UDPDLG_H__0B26E45B_5261_4629_80EF_B57A50F82176__INCLUDED_)
#define AFX_SOCKET_UDPDLG_H__0B26E45B_5261_4629_80EF_B57A50F82176__INCLUDED_

//#include "MyAsyncSocket.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "MyAsyncSocket.h"
#include "Signal.h"	// Added by ClassView
#include "ProgressCtrlX.h"
#include "resudp.h"

#define   SERVER_PORT   11191
#define   CLIENT_PORT   11101
#define   CURRENT_TYPE  SOCK_STREAM


/////////////////////////////////////////////////////////////////////////////
// CSocket_UDPDlg dialog

class CSocket_UDPDlg : public CDialog
{
// Construction
	bool		m_bSend;
	SockAddr	m_sa;
	HANDLE		m_hEvent;
	HANDLE		m_hValid;
public:
	CSignalImpl m_SendSign;
	CSignalImpl m_RecvSign;
	static void ClientThreadFunc(LPVOID Param);
	CWinThread* m_pClientThread;
	CWinThread* m_pServerThread;
	HANDLE m_ServerThread;
	static void ServerThreadFunc(LPVOID pParam);
	CBlockingSocket m_Client;
	CBlockingSocket m_Server;
	CSocket_UDPDlg(bool bSend, SockAddr, CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSocket_UDPDlg)
	enum { IDD = IDD_SOCKET_UDP_DIALOG };
	virtual ~CSocket_UDPDlg()
	{
		if(!m_bSend)
			CloseHandle(m_hEvent);
	}
	CProgressCtrlX	m_wndRecv;
	CProgressCtrlX	m_wndSend;
	CString	m_SaveDir;
	CString	m_Send_IP;
	CString	m_File;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSocket_UDPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSocket_UDPDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSend();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnButtonFile();
	afx_msg void OnPath();
	afx_msg void	OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SOCKET_UDPDLG_H__0B26E45B_5261_4629_80EF_B57A50F82176__INCLUDED_)

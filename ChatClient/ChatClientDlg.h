// ChatClientDlg.h : header file
//

#if !defined(AFX_CHATCLIENTDLG_H__E250585E_4616_42F7_B286_25E9DEE78EAA__INCLUDED_)
#define AFX_CHATCLIENTDLG_H__E250585E_4616_42F7_B286_25E9DEE78EAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChatClientDlg dialog
#include "..\socket\TSocket.h"
#include "Socket_UDPDlg.h"

class CChatClientDlg : public CDialog
{
	static void ClientThreadFunction(LPVOID Param);

	CSocket_UDPDlg		m_dlg;
// Construction
public:
	CChatClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChatClientDlg)
	enum { IDD = IDD_CHATCLIENT_DIALOG };
	CListBox	m_ListPeople;
	CListBox	m_ListChat;
	CString	m_sMessage;
	CString	m_sName;
	CString	m_sServer;
	CTSocket* m_pClient;
	BOOL	m_bConnect;
	CWinThread* m_pClientThread;
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChatClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnConnect();
	afx_msg void OnSend();
	afx_msg void OnDisconnect();
	afx_msg void OnClose();
	afx_msg void OnPrivate();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSendfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATCLIENTDLG_H__E250585E_4616_42F7_B286_25E9DEE78EAA__INCLUDED_)

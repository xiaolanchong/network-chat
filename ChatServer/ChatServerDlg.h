// ChatServerDlg.h : header file
//

#if !defined(AFX_CHATSERVERDLG_H__D23E0BA7_4597_4364_9A82_573F2AD1EE00__INCLUDED_)
#define AFX_CHATSERVERDLG_H__D23E0BA7_4597_4364_9A82_573F2AD1EE00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChatServerDlg dialog
#include "..\socket\TSocket.h"
#include <vector>
#include "TaskIcon.h"

using namespace std;

struct SERVERLISTCLIENT
{
	SOCKET m_Socket;
	CString m_IP;
	CString m_Nick;
	BOOL m_New;
	SERVERLISTCLIENT(){};
	SERVERLISTCLIENT(SOCKET sock, CString IP, CString Nick, BOOL bNew):
	m_Socket(sock), m_IP(IP), m_Nick(Nick), m_New(bNew){};
};

struct LISTTHREAD
{
	CWinThread* m_pServerThread;
	CString		m_sName;
	LISTTHREAD(CWinThread* thread, CString str):m_sName(str)
	{
		m_pServerThread = new CWinThread;
		m_pServerThread=thread;
	}
};
class CChatServerDlg : public CDialog
{
	CTSocket* m_pSocket;
	CTSocket* m_pMainSocket;
	CWinThread* m_pMainServerThread;
	int m_ClientCount;
	vector<SERVERLISTCLIENT> m_ServerList;
	vector<LISTTHREAD> m_ListThread;
	CTaskIcon			m_wndTask;
// Construction
public:
	static void ServerThreadFunc(LPVOID param);
	static void ServerMainThreadFunc(LPVOID param);
	CChatServerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CChatServerDlg)
	enum { IDD = IDD_CHATSERVER_DIALOG };
	CListBox	m_ListPeople;
	CListBox	m_ListChat;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChatServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnKick();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSERVERDLG_H__D23E0BA7_4597_4364_9A82_573F2AD1EE00__INCLUDED_)

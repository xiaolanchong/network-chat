// Signal.h: interface for the CSignal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIGNAL_H__CFEC79B2_2740_4506_920F_AA1FFE6F1401__INCLUDED_)
#define AFX_SIGNAL_H__CFEC79B2_2740_4506_920F_AA1FFE6F1401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "filesocket.h"
#include "resudp.h"


class CSignalImpl : public CSignal 
{
	int		Len;
	int		Cur;
	CProgressCtrl* m_pWnd;
	bool		m_bSend;
public:
	virtual void setlen(int len) 
	{
		Len = Len;
		m_pWnd->SetRange32(0, len);
		m_pWnd->SetPos(0);
	}
	virtual void add(int c)
	{ 
		//Cur += c;
		m_pWnd->OffsetPos(c);
	}
	CSignalImpl(CProgressCtrl* pWnd, bool b) : Len(0), Cur(0), m_pWnd(pWnd), m_bSend(b)
	{
		
	};
	virtual void	quit()
	{
//		AfxMessageBox("Transfer completed");
		m_pWnd->SetPos(0);
//		PostMessage(GetParent(m_pWnd->GetSafeHwnd()), WM_CLOSE, 0, 0);
		if(!m_bSend)
			ShowWindow(GetParent(m_pWnd->GetSafeHwnd()), SW_HIDE);
		else
			PostMessage(GetParent(m_pWnd->GetSafeHwnd()), WM_CLOSE, 0, 0);
	}
	virtual void	error()
	{
//		AfxMessageBox("Error was occured");
		m_pWnd->SetPos(0);
	}

	virtual bool report(CString name)
	{
/*		int res = AfxMessageBox(name, MB_YESNO|MB_ICONQUESTION);
		if(res == IDNO)
			return false;
		else
			return true;*/

		CWnd* pWnd = CWnd::FromHandle(GetParent(m_pWnd->GetSafeHwnd()));
		//CString str;
		pWnd->SetDlgItemText(IDC_EDIT_SEND, name);
		pWnd->UpdateData(TRUE);
		HANDLE Event = OpenEvent(EVENT_ALL_ACCESS,TRUE,"SyncEv15");
		DWORD res = WaitForSingleObject(Event, 20*1000) ;

		if(res == WAIT_TIMEOUT || res == WAIT_ABANDONED)
			return false;
		else
		{
			pWnd->SetDlgItemText(IDC_EDIT_SEND, name);
		//	pWnd->UpdateData(TRUE);
			return true;
		}
	}

	virtual CString getdir();

	virtual ~CSignalImpl()
	{
	
	};

};

#endif // !defined(AFX_SIGNAL_H__CFEC79B2_2740_4506_920F_AA1FFE6F1401__INCLUDED_)

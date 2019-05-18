// Signal.cpp: implementation of the CSignal class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Socket_UDP.h"
#include "Signal.h"
#include "resudp.h"
//#include "Socket_udpdlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CString CSignalImpl::getdir()
{
	CWnd* pWnd = CWnd::FromHandle(GetParent(m_pWnd->GetSafeHwnd()));
	pWnd->UpdateData(TRUE);
//	CString str(((CSocket_UDPDlg*)pWnd)->m_SaveDir);
	CString str;
	pWnd->GetDlgItemText(IDC_EDIT_SAVEDIR, str);
	return str;
}
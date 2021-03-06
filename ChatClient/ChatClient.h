// ChatClient.h : main header file for the CHATCLIENT application
//

#if !defined(AFX_CHATCLIENT_H__5C3373CC_9964_4961_988C_0BFCD2861765__INCLUDED_)
#define AFX_CHATCLIENT_H__5C3373CC_9964_4961_988C_0BFCD2861765__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CChatClientApp:
// See ChatClient.cpp for the implementation of this class
//

class CChatClientApp : public CWinApp
{
public:
	CChatClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatClientApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL ProcessMessageFilter(int code, LPMSG lpMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CChatClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATCLIENT_H__5C3373CC_9964_4961_988C_0BFCD2861765__INCLUDED_)

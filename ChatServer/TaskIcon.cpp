// TaskIcon.cpp: implementation of the CTaskIcon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TaskIcon.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTaskIcon::CTaskIcon()
{

}

CTaskIcon::~CTaskIcon()
{

}

/////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CTaskIcon, COXTaskbarIcon)
	//{{AFX_MSG_MAP(CMyListBox)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////

void CTaskIcon::OnLButtonDblClk(UINT nFlags, CPoint point)
{
//	return COXTaskbarIcon::OnLButtonDblClk(nFlags, point);
	m_pParent->ShowWindow(SW_SHOW);
	m_pParent->SetActiveWindow();
}

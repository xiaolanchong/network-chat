// TaskIcon.h: interface for the CTaskIcon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKICON_H__9444B14D_6ECB_4D40_B29C_6B90E42E1DE0__INCLUDED_)
#define AFX_TASKICON_H__9444B14D_6ECB_4D40_B29C_6B90E42E1DE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OXTaskbarIcon.h"

class CTaskIcon : public COXTaskbarIcon  
{
	CWnd*	m_pParent;
public:
	bool	Create(CWnd* pWnd, UINT id = 0)
	{
		m_pParent = pWnd;
		return COXTaskbarIcon::Create(id) == TRUE;
	}

	CTaskIcon();
	virtual ~CTaskIcon();
	

protected:
	//{{AFX_MSG(CMyListBox)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_TASKICON_H__9444B14D_6ECB_4D40_B29C_6B90E42E1DE0__INCLUDED_)

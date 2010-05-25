// melfviz.h : main header file for the MELFVIZ application
//

#if !defined(AFX_MELFVIZ_H__5179614C_BDFE_4F9A_9266_B0A12CAA14CE__INCLUDED_)
#define AFX_MELFVIZ_H__5179614C_BDFE_4F9A_9266_B0A12CAA14CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMelfvizApp:
// See melfviz.cpp for the implementation of this class
//

class CMelfvizApp : public CWinApp
{
public:
	CMelfvizApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMelfvizApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMelfvizApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileExportTxt();
	afx_msg void OnFileExportXml();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL DoExportFilePrompt(LPCTSTR lpszExt, LPCTSTR lpszFilter);
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MELFVIZ_H__5179614C_BDFE_4F9A_9266_B0A12CAA14CE__INCLUDED_)

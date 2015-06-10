// GridDemo.h : main header file for the GRIDDEMO application
//

#if !defined(AFX_GRIDDEMO_H__FA124E04_0BEB_4AD2_B552_388216AE57F6__INCLUDED_)
#define AFX_GRIDDEMO_H__FA124E04_0BEB_4AD2_B552_388216AE57F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CGridDemoApp:
// See GridDemo.cpp for the implementation of this class
//

class CGridDemoApp : public CWinApp
{
public:
	CGridDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CGridDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDEMO_H__FA124E04_0BEB_4AD2_B552_388216AE57F6__INCLUDED_)

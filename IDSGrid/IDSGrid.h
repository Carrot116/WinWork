// IDSGrid.h : main header file for the IDSGRID DLL
//

#if !defined(AFX_IDSGRID_H__BD5F904C_1E96_460C_8B74_FDA40D747C37__INCLUDED_)
#define AFX_IDSGRID_H__BD5F904C_1E96_460C_8B74_FDA40D747C37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CIDSGridApp
// See IDSGrid.cpp for the implementation of this class
//

class CIDSGridApp : public CWinApp
{
public:
	CIDSGridApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIDSGridApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIDSGridApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IDSGRID_H__BD5F904C_1E96_460C_8B74_FDA40D747C37__INCLUDED_)

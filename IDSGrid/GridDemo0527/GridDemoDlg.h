// GridDemoDlg.h : header file
//

#if !defined(AFX_GRIDDEMODLG_H__15D14162_6748_41AF_9EE3_B361D319671A__INCLUDED_)
#define AFX_GRIDDEMODLG_H__15D14162_6748_41AF_9EE3_B361D319671A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CGridDemoDlg dialog

class CGridDemoDlg : public CDialog
{
// Construction
public:
	CGridDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGridDemoDlg)
	enum { IDD = IDD_GRIDDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGridDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGridDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnGrid1VScroll(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnGrid1DoubleClick(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnGrid1RButtonDown(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnGrid1LButtonDown(NMHDR * pNMHDR, LRESULT * pResult);
	afx_msg void OnGrid1LButtonUp(NMHDR * pNMHDR, LRESULT * pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CIDSGridCtrl * m_pGridCtrl;
	void AdJustGridCell();
	BOOL m_bLButtonDown;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRIDDEMODLG_H__15D14162_6748_41AF_9EE3_B361D319671A__INCLUDED_)

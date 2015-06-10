// GridDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GridDemo.h"
#include "GridDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

struct tagColInfo
{
	int nItemID;
	TCHAR szCaption[16];
	int nColWidth;
};
enum emGridItem
{
	em_ItemNone = 0,
		em_ItemCode,
		em_ItemName,
		em_ItemLast,
};

#define  IDS_GRIDCTRL1_ID		1
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDemoDlg dialog

CGridDemoDlg::CGridDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGridDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGridDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pGridCtrl = NULL;
	m_bLButtonDown = FALSE;
}

void CGridDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGridDemoDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGridDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CGridDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_NOTIFY( NM_DBLCLK, IDS_GRIDCTRL1_ID, OnGrid1DoubleClick )
	ON_NOTIFY( WM_RGRIDRBUTTONDOWN, IDS_GRIDCTRL1_ID, OnGrid1RButtonDown )
	ON_NOTIFY( WM_GRID_VSCOLL, IDS_GRIDCTRL1_ID, OnGrid1VScroll )
	ON_NOTIFY( WM_RGRIDLBUTTONDOWN, IDS_GRIDCTRL1_ID, OnGrid1LButtonDown )
	ON_NOTIFY( WM_RGRIDLBUTTONUP, IDS_GRIDCTRL1_ID, OnGrid1LButtonUp )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridDemoDlg message handlers
BOOL CGridDemoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	srand(time(NULL));
	const int COLWIDTH = 80;
	tagColInfo tColArr[] = 
	{
		{0, _T("代码"), COLWIDTH},
		{0, _T("名称"), COLWIDTH},
		{0, _T("最新价"), COLWIDTH},
		{0, _T("开"), COLWIDTH},
		{0, _T("高"), COLWIDTH},
		{0, _T("低"), COLWIDTH},
		{0, _T("昨收"), COLWIDTH},
		{0, _T("涨幅"), COLWIDTH},
		{0, _T("总手"), COLWIDTH},
		{0, _T("现手"), COLWIDTH},
		{0, _T("买价"), COLWIDTH},
		{0, _T("卖价"), COLWIDTH},
		{0, _T("买量"), COLWIDTH},
		{0, _T("卖量"), COLWIDTH},
		{0, _T("流入"), COLWIDTH},
		{0, _T("流出"), COLWIDTH},
		{0, _T("净流入"), COLWIDTH},
		{0, _T("净流出"), COLWIDTH},
		{0, _T("持仓"), COLWIDTH}		
	};
	int nGridItemNum = sizeof(tColArr)/sizeof(tagColInfo);
	int nRowCount = 80;
	m_pGridCtrl = new CIDSGridCtrl;
	CRect rcClient;
	this->GetClientRect(&rcClient);
	m_pGridCtrl->Create(rcClient, this, IDS_GRIDCTRL1_ID);
	
	m_pGridCtrl->SetBkColor(RGB(0,0,0));
	m_pGridCtrl->SetColumnCount(nGridItemNum);
	m_pGridCtrl->SetRowCount(nRowCount);
	m_pGridCtrl->SetFixedRowCount(1);
	m_pGridCtrl->SetFixedColumnCount(2);
//	m_pGridCtrl->SetFocusCell(0,0);;
	m_pGridCtrl->SetFixedBkColor(RGB(255, 255, 255));
	m_pGridCtrl->SetGridLineColor(0);
	m_pGridCtrl->SetTextColor(RGB(255, 255,255));
	m_pGridCtrl->SetRowFullSelect(TRUE);
	m_pGridCtrl->AllowSelectRowInFixedCol(TRUE);
	m_pGridCtrl->SetSingleRowSelection(TRUE);
	m_pGridCtrl->SetHeaderSort(TRUE);
	m_pGridCtrl->SetGridLines(GVL_NONE);
//	m_pGridCtrl->ExpandColumnsToFit(FALSE);
//	m_pGridCtrl->SetFixed
//	m_pGridCtrl->SetSelectItemBKColor(RGB(54, 59,70));
	int ii = 0;
	for(ii = 0; ii < nGridItemNum; ii++)
	{
		m_pGridCtrl->SetItemText(0, ii, tColArr[ii].szCaption);
		m_pGridCtrl->SetColumnWidth(ii, tColArr[ii].nColWidth);
	}
	for(ii = 0; ii < nRowCount; ii++)
	{		
		m_pGridCtrl->SetRowHeight(ii, 30);
	}
	COLORREF clRed = RGB(255, 0, 0);
	COLORREF clGreen = RGB(0, 255, 0);
	for(ii = 0; ii < nRowCount ; ii ++)
	{
		if(0 == ii)
		{	
			for(int jj = 0; jj < nGridItemNum; jj++)
			{
				m_pGridCtrl->SetItemBkColour(ii, jj, 0);
				m_pGridCtrl->SetItemFgColour(ii,jj, RGB(255, 255,255));
				int nAlign = DT_CENTER;	
				if(jj%3==0)
				{
					nAlign = DT_LEFT;
				}
				m_pGridCtrl->SetItemFormat(ii, jj, nAlign);
			}
		}
		else
		{	
			for(int jj = 0; jj < nGridItemNum; jj++)
			{

				COLORREF clText = clRed;
				if(rand()%2)
				{
					clText = clGreen;
				}
				m_pGridCtrl->SetItemBkColour(ii, jj, 0);
				m_pGridCtrl->SetItemFgColour(ii,jj, clText);
				int nAlign = DT_CENTER;	
				if(jj%3==0)
				{
					nAlign = DT_LEFT;
				}
				m_pGridCtrl->SetItemFormat(ii, jj, nAlign);
				int nVal = rand();
				if(jj == 0)
				{
					nVal = ii ;
				}
				CString strText;
				strText.Format(_T("%d"), nVal);
				m_pGridCtrl->SetItemText(ii, jj, strText);
			}
		}
	}








	m_pGridCtrl->ShowWindow(SW_SHOW);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGridDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGridDemoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGridDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CGridDemoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CRect rcClient;
	this->GetClientRect(&rcClient);
	if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
	{
		m_pGridCtrl->MoveWindow(&rcClient);
	}
}

void CGridDemoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

LRESULT CGridDemoDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}

void CGridDemoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
//	CDialog::OnOK();
}

void CGridDemoDlg::OnDestroy() 
{
	if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
	{
		m_pGridCtrl->DestroyWindow();
	}
	delete m_pGridCtrl;
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}


void CGridDemoDlg::AdJustGridCell()
{
	
}


void CGridDemoDlg::OnGrid1VScroll(NMHDR * pNMHDR, LRESULT * pResult)
{
	SHORT  wButtonStat = ::GetAsyncKeyState(VK_LBUTTON); 

	if(wButtonStat)
	{
		return;
	}
	NM_GRIDVIEW* pGridView= (NM_GRIDVIEW*)pNMHDR;
	CString strInfo;
	strInfo.Format(_T("[VScroll] cell start pos:%d\r\n"), pGridView->iRow);
	OutputDebugString((LPCTSTR)strInfo);

	*pResult = 0;
}

void CGridDemoDlg::OnGrid1DoubleClick(NMHDR * pNMHDR, LRESULT * pResult)
{
	NM_GRIDVIEW* pGridView= (NM_GRIDVIEW*)pNMHDR;
	*pResult = 0;
}

void CGridDemoDlg::OnGrid1RButtonDown(NMHDR * pNMHDR, LRESULT * pResult)
{
	NM_GRIDVIEW* pGridView= (NM_GRIDVIEW*)pNMHDR;

	*pResult = 0;
}

void CGridDemoDlg::OnGrid1LButtonDown(NMHDR * pNMHDR, LRESULT * pResult)
{
	m_bLButtonDown = TRUE;
}

void CGridDemoDlg::OnGrid1LButtonUp(NMHDR * pNMHDR, LRESULT * pResult)
{
	m_bLButtonDown = FALSE;
}

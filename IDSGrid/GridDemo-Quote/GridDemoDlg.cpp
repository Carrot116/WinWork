// GridDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GridDemo.h"
#include "GridDemoDlg.h"
#include  <MATH.H>
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

#define  IDS_GRIDCTRL1_ID		100

#define  CL_RED			RGB(255, 0 , 0)
#define  CL_GREEN		RGB(0, 255 , 0)
#define  CL_BLUE		RGB(0, 0 , 255)

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
	m_pGridCtrlTick = NULL;

	m_nTickNum = 0;
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

void CGridDemoDlg::InitGridCtrl(CIDSGridCtrl * pGridCtrl, int nRow, int nCol, int nRowHeight, int nColWidth)
{
	if(NULL == pGridCtrl)
	{
		return;
	}	
//	pGridCtrl->SetAutoShowScrollBar(FALSE);
	
	pGridCtrl->SetBkColor(RGB(0,0,0));
	pGridCtrl->SetColumnCount(nCol);
	pGridCtrl->SetRowCount(nRow);
	pGridCtrl->SetFixedRowCount(0);
	pGridCtrl->SetFixedColumnCount(0);
	pGridCtrl->SetFixedBkColor(RGB(255, 255, 255));
	pGridCtrl->SetGridLineColor(0);
	pGridCtrl->SetTextColor(RGB(255, 255,255));

	
	pGridCtrl->SetRowFullSelect(TRUE);
	pGridCtrl->EnableSelection(FALSE);
	//m_pGridCtrl->AllowSelectRowInFixedCol(TRUE);
	pGridCtrl->SetSingleRowSelection(TRUE);
	pGridCtrl->SetHeaderSort(FALSE);
	pGridCtrl->SetGridLines(GVL_NONE);


	int ii = 0;
	for(ii = 0; ii < nCol; ii++)
	{
		pGridCtrl->SetColumnWidth(ii, nColWidth);
	}
	for(ii = 0; ii < nRow; ii++)
	{		
		pGridCtrl->SetRowHeight(ii, nRowHeight);
	}
	COLORREF clRed = RGB(255, 0, 0);
	COLORREF clGreen = RGB(0, 255, 0);
	for(ii = 0; ii < nRow ; ii ++)
	{
		for(int jj = 0; jj < nCol; jj++)
		{			
			pGridCtrl->SetItemBkColour(ii, jj, 0);
			pGridCtrl->SetItemFgColour(ii,jj, RGB(255, 255,255));
		}
	}

}
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
	int nRowCount = 20;
	int nColNum = 4;
	m_pGridCtrl = new CIDSGridCtrl(FALSE);
	CRect rcClient(0,0,10,10);
	//this->GetClientRect(&rcClient);
	m_pGridCtrl->Create(rcClient, this, IDS_GRIDCTRL1_ID);

	InitGridCtrl(m_pGridCtrl, nRowCount, nColNum, 30, 80);
	for(int ii = 0; ii < nRowCount; ii ++)
	{
		for(int jj = 0; jj < nColNum; jj ++)
		{
			int nAlign = DT_RIGHT;	
			if(jj%2==0)
			{
				nAlign = DT_LEFT;
			}
			m_pGridCtrl->SetItemFormat(ii, jj, nAlign);
		}
	}
	m_pGridCtrl->SetUserDrawFunc(CustomizeDrawQuote, (DWORD)m_pGridCtrl);
	
	m_pGridCtrlTick = new CIDSGridCtrl(FALSE);
	m_pGridCtrlTick->Create(rcClient, this, IDS_GRIDCTRL1_ID + 1);
	InitGridCtrl(m_pGridCtrlTick, MAX_TICK_NUM, nColNum, 30, 80);	
	for(ii = 0; ii < MAX_TICK_NUM; ii++)
	{		
		m_pGridCtrlTick->SetItemFormat(ii, 0, DT_LEFT);
		m_pGridCtrlTick->SetItemFormat(ii, 1, DT_CENTER);
		m_pGridCtrlTick->SetItemFormat(ii, 2, DT_CENTER);
		m_pGridCtrlTick->SetItemFormat(ii, 3, DT_RIGHT);

		 
		m_pGridCtrlTick->SetItemFgColour(ii, 1, CL_RED);
		m_pGridCtrlTick->SetItemFgColour(ii, 2, CL_GREEN);
		m_pGridCtrlTick->SetItemFgColour(ii, 3, CL_BLUE);
	}
	m_pGridCtrlTick->SetUserDrawFunc(CustomizeDrawTicks, (DWORD)m_pGridCtrlTick);
	
	m_pGridCtrlTick->EnableSelection(TRUE);

	this->OnSizeChange();
	m_pGridCtrl->ShowWindow(SW_SHOW);
	m_pGridCtrlTick->ShowWindow(SW_SHOW);
	this->SetTimer(1, 1000, NULL);
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
	OnSizeChange();
}

void CGridDemoDlg::OnSizeChange()
{
	
	CRect rcClient;
	this->GetClientRect(&rcClient);
	
	rcClient.left += 1;
	rcClient.right -= 1;
	rcClient.top += 1;
	rcClient.bottom -= 1;
	if(m_pGridCtrl && m_pGridCtrl->GetSafeHwnd())
	{
		CRect rcGrid1(rcClient);
		rcGrid1.bottom = rcGrid1.top + 20 * 30 + 5;
		m_pGridCtrl->MoveWindow(&rcGrid1);
		CRect rcGrid2(rcClient);
		rcGrid2.top = rcGrid1.bottom;
		//rcGrid2.bottom = rcGrid2.top + 905;
		m_pGridCtrlTick->MoveWindow(&rcGrid2);
	}
}


void CGridDemoDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	ShowQuoteAllInfo();
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
	if(m_pGridCtrlTick && m_pGridCtrlTick->GetSafeHwnd())
	{
		m_pGridCtrlTick->DestroyWindow();
	}
	delete m_pGridCtrlTick;
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
	
	NM_GRIDVIEW* pGridViewItem= (NM_GRIDVIEW*)pNMHDR;

	m_bLButtonDown = TRUE;
}

void CGridDemoDlg::OnGrid1LButtonUp(NMHDR * pNMHDR, LRESULT * pResult)
{
	m_bLButtonDown = FALSE;
}



void CGridDemoDlg::ShowQuoteAllInfo()
{	

	this->FillWeituoData();
	this->FillBuy5StepData();
	this->FillSell5StepData();
	this->FillBaseQuetoData();
	this->FillInOutSizeData();

	m_pGridCtrl->Invalidate(TRUE);

	AddOneTick();
	m_pGridCtrlTick->Invalidate(TRUE);
}

void CGridDemoDlg::FillWeituoData()
{
	m_pGridCtrl->SetItemText(0, 0, _T("Î¯ÍÐ"));
	m_pGridCtrl->SetItemFgColour(0,0, RGB(255, 255, 255));
	COLORREF clText = RGB(255, 0, 0);
	CString strToken = _T("");
	if(rand()%2)
	{
		clText = RGB(0, 255, 0);
		CString strToken = _T("-");
	}
	CString strText;
	strText.Format(_T("%s%d.%d"), (LPCTSTR)strToken, rand()%100, rand()%100);
	m_pGridCtrl->SetItemText(0, 1,  (LPCTSTR)strText);
	m_pGridCtrl->SetItemFgColour(0, 1, clText);

	strText.Format(_T("%d"), rand());
	m_pGridCtrl->SetItemText(0, 3,  (LPCTSTR)strText);
	m_pGridCtrl->SetItemFgColour(0,3, clText);

}
void CGridDemoDlg::FillBuy5StepData()
{
	int nStartPos = 1;
	const TCHAR szText[][8] = 
	{
		_T("Âô¢Ý"),
		_T("Âô¢Ü"),
		_T("Âô¢Û"),
		_T("Âô¢Ú"),
		_T("Âô¢Ù")
	};
	int nBasePrice = 900;
	int nBuy5Step[5];
	int ii = 0;
	for(ii = 0; ii < 5; ii++)
	{
		if(rand()%2)
		{
			nBuy5Step[ii] = nBasePrice + rand()%10;
		}
		else
		{
			nBuy5Step[ii] = nBasePrice - rand()%10;
		}
	}
	for(ii = 0; ii < 4; ii ++)
	{
		int nMaxVal = nBuy5Step[ii];
		for(int jj = ii + 1; jj <5; jj++)
		{
			if(nBuy5Step[jj] > nMaxVal)
			{
				nMaxVal = nBuy5Step[jj];
				nBuy5Step[jj] = nBuy5Step[ii];
				nBuy5Step[ii] = nMaxVal;
			}
		}
	}

	for(ii = 0; ii < 5; ii++)
	{ 
		COLORREF clText = RGB(255, 255, 255);
		int nRow = nStartPos + ii;
		m_pGridCtrl->SetItemText(nRow, 0, szText[ii]);
		m_pGridCtrl->SetItemFgColour(nRow, 0, clText);

		clText = RGB(255, 0,0);
		if(nBuy5Step[ii] < nBasePrice)
		{
			clText = RGB(0, 255, 0);
		}
		if(nBasePrice == nBuy5Step[ii])
		{
			clText = RGB(255, 255, 255);
		}
		CString strText;
		float fPrice = nBuy5Step[ii] /100.0;
		strText.Format(_T("%.2f"), fPrice);
		m_pGridCtrl->SetItemText(nRow, 1, strText);
		m_pGridCtrl->SetItemFgColour(nRow, 1, clText);

		clText = RGB(0, 0, 255);
		
		strText.Format(_T("%d"), rand()%100);
		m_pGridCtrl->SetItemText(nRow, 3, strText);
		m_pGridCtrl->SetItemFgColour(nRow, 3, clText);
	
	}
}
void CGridDemoDlg::FillSell5StepData()
{
	int nStartPos = 6;
	const TCHAR szText[][8] = 
	{
		_T("Âò¢Ù"),
		_T("Âò¢Ú"),
		_T("Âò¢Û"),
		_T("Âò¢Ü"),
		_T("Âò¢Ý")
	};
	int nBasePrice = 900;
	int nBuy5Step[5];
	int ii = 0;
	for(ii = 0; ii < 5; ii++)
	{
		if(rand()%2)
		{
			nBuy5Step[ii] = nBasePrice + rand()%10;
		}
		else
		{
			nBuy5Step[ii] = nBasePrice - rand()%10;
		}
	}
	for(ii = 0; ii < 4; ii ++)
	{
		int nMaxVal = nBuy5Step[ii];
		for(int jj = ii + 1; jj <5; jj++)
		{
			if(nBuy5Step[jj] > nMaxVal)
			{
				nMaxVal = nBuy5Step[jj];
				nBuy5Step[jj] = nBuy5Step[ii];
				nBuy5Step[ii] = nMaxVal;
			}
		}
	}
	
	for(ii = 0; ii < 5; ii++)
	{ 
		COLORREF clText = RGB(255, 255, 255);
		int nRow = nStartPos + ii;
		m_pGridCtrl->SetItemText(nRow, 0, szText[ii]);
		m_pGridCtrl->SetItemFgColour(nRow, 0, clText);
		
		clText = RGB(255, 0,0);
		if(nBuy5Step[ii] < nBasePrice)
		{
			clText = RGB(0, 255, 0);
		}
		if(nBasePrice == nBuy5Step[ii])
		{
			clText = RGB(255, 255, 255);
		}
		CString strText;
		float fPrice = nBuy5Step[ii] /100.0;
		strText.Format(_T("%.2f"), fPrice);
		m_pGridCtrl->SetItemText(nRow, 1, strText);
		m_pGridCtrl->SetItemFgColour(nRow, 1, clText);
		
		clText = RGB(0, 0, 255);
		
		strText.Format(_T("%d"), rand()%100);
		m_pGridCtrl->SetItemText(nRow, 3, strText);
		m_pGridCtrl->SetItemFgColour(nRow, 3, clText);		
	}
}
void CGridDemoDlg::FillBaseQuetoData()
{
	int nRow = 11;
	m_pGridCtrl->SetItemText(nRow, 0, _T("×îÐÂ"));
	CString strText;
	int nOpenPrice = 900;
	int nLastPrice =  RoundOneRand(nOpenPrice, 0, 2);
	COLORREF clText = GetCompareColor(nLastPrice, nOpenPrice);
	strText = FormatIntVal2String(nLastPrice);
	m_pGridCtrl->SetItemText(nRow, 1, (LPCTSTR)strText);
	m_pGridCtrl->SetItemFgColour(nRow, 1, clText);

	m_pGridCtrl->SetItemText(nRow, 2, _T("¿ªÅÌ"));
	strText = FormatIntVal2String(nOpenPrice);
	m_pGridCtrl->SetItemText(nRow, 3, (LPCTSTR)strText);

	
	m_pGridCtrl->SetItemText(nRow + 1, 0, _T("ÕÇµø"));
	int nUpDown = nLastPrice - nOpenPrice;
	strText = FormatIntVal2String(nUpDown);
	m_pGridCtrl->SetItemText(nRow + 1, 1, (LPCTSTR)strText);
	m_pGridCtrl->SetItemFgColour(nRow + 1, 1,clText);
	m_pGridCtrl->SetItemText(nRow + 1, 2, _T("×î¸ß"));
	
	int nHigh = RoundOneRand(nOpenPrice, 1, 2);
	strText = FormatIntVal2String(nHigh);
	m_pGridCtrl->SetItemText(nRow + 1, 3, (LPCTSTR)strText);
	clText = GetCompareColor(nHigh, nOpenPrice);
	m_pGridCtrl->SetItemFgColour(nRow + 1, 3, clText);

}
void CGridDemoDlg::FillInOutSizeData()
{
	int nRow = 19;	
	m_pGridCtrl->SetItemText(nRow, 0, _T("ÍâÅÌ"));
	CString strText;
	strText.Format(_T("%u"), RoundOneRand(50000, 1, 4));
	m_pGridCtrl->SetItemText(nRow, 1, (LPCTSTR)strText);
	m_pGridCtrl->SetItemFgColour(nRow, 1, CL_RED);
	
	m_pGridCtrl->SetItemText(nRow, 2, _T("ÄÚÅÌ"));

	strText.Format(_T("%u"), RoundOneRand(50000, 2, 4));
	m_pGridCtrl->SetItemText(nRow, 3, (LPCTSTR)strText);
	m_pGridCtrl->SetItemFgColour(nRow, 3, CL_GREEN);

}

void CGridDemoDlg::AddOneTick()
{
	tagTickItem tTickItem;
	static DWORD dwItemID = 0;
	tTickItem.dwTime = ++dwItemID;
	tTickItem.fPrice = 9.0 + (rand()%100)/100.0;
	tTickItem.dwTradeSize = rand()%100;
	tTickItem.dwAppendVal = rand()%10;
	int nStartPos = m_nTickNum;
	if(nStartPos >= MAX_TICK_NUM)
	{
		nStartPos = MAX_TICK_NUM - 1;
	}
	for(int ii = nStartPos; ii > 0; ii--)
	{
		m_tTickArr[ii] = m_tTickArr[ii - 1];
	}
	m_tTickArr[0] = tTickItem;
	m_nTickNum ++;
	if(m_nTickNum >= MAX_TICK_NUM)
	{
		m_nTickNum = MAX_TICK_NUM;
	}

	for(ii = 0; ii < m_nTickNum; ii++)
	{
		CString strText;
		strText.Format(_T("%d"), m_tTickArr[ii].dwTime);
		m_pGridCtrlTick->SetItemText(ii, 0, (LPCTSTR)strText);

		strText.Format(_T("%.2f"), m_tTickArr[ii].fPrice);
		m_pGridCtrlTick->SetItemText(ii, 1, (LPCTSTR)strText);

		strText.Format(_T("%d"), m_tTickArr[ii].dwTradeSize);
		m_pGridCtrlTick->SetItemText(ii, 2, (LPCTSTR)strText);

		strText.Format(_T("%d"), m_tTickArr[ii].dwAppendVal);
		m_pGridCtrlTick->SetItemText(ii, 3, (LPCTSTR)strText);
	}
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

BOOL IsBig(DWORD dwVa1, DWORD dwVal2)
{
	if(dwVa1 > dwVal2)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL IsEqual(DWORD dwVal1, DWORD dwVal2)
{
	if(dwVal1 == dwVal2)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL IsBig(float fVa1, float fVal2)
{
	if(IsEqual(fVa1, fVal2))
	{
		return FALSE;
	}
	if(fVa1 > fVal2)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL IsEqual(float fVal1, float fVal2)
{	
	if(fabs(fVal2 - fVal1) < 0.00001)
	{
		return TRUE;
	}
	return FALSE;
}

COLORREF GetCompareColor(DWORD dwVal1, DWORD dwVal2)
{
	COLORREF clText = RGB(255, 0, 0);
	if(IsEqual(dwVal1, dwVal2))
	{
		clText = RGB(255, 255, 255);
	}
	if(IsBig(dwVal2, dwVal1))
	{
		clText = RGB(0, 255, 0);
	}
	return clText;
}

CString FormatIntVal2String(int dwVal, int byDotNum )
{
	CString strTxt;
	float fVal = dwVal/100.0;
	strTxt.Format(_T("%.2f"), fVal);
	return strTxt;
}

float Int2Float(int nVal, int byDotNum)
{
	int nRank = 1;
	for(int ii = 0; ii < nRank; ii++)
	{
		nRank *= 10;
	}
	float fVal = nVal * 1.0/nRank;
	return fVal;
}

DWORD RoundOneRand(DWORD dwBaseVal , BYTE byUpDown, int nDecimal)
{
	DWORD dwVal = dwBaseVal;
	int nRandVal = rand();
	int nMod = 1;
	for(int ii = 0; ii < nDecimal; ii++)
	{
		nMod *= 10;
		nRandVal %= nMod;
	}
	if(byUpDown == 1 || (0 == byUpDown &&rand()%2))
	{
		dwVal += nRandVal;
	}
	else
	{
		dwVal -= nRandVal;
	}
	return dwVal;
}
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CustomizeDrawQuote(CDC * pDC, DWORD dwContext)
{	
	CIDSGridCtrl *  pGridCtrl = (CIDSGridCtrl *)dwContext;
	if(NULL == pGridCtrl || NULL == pGridCtrl->GetSafeHwnd())
	{
		return;
	}
	CRect rcClient;
	pGridCtrl->GetClientRect(&rcClient);
	rcClient.right -= 1;
	rcClient.bottom -= 1;
	int nRowHight = pGridCtrl->GetRowHeight(0);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();
	
    CPen cRedPen(PS_SOLID, 1,  RGB(255, 0, 0));
	
    CPen *pOldPen = pDC->SelectObject(&cRedPen);
	pDC->MoveTo(0,0);
	pDC->LineTo(rcClient.right, rcClient.top);
	pDC->LineTo(rcClient.right, rcClient.bottom);
	pDC->LineTo(rcClient.left, rcClient.bottom);
	pDC->LineTo(0,0);

	
	int nStartPos = nRowHight;
	pDC->MoveTo(0, nStartPos);
	pDC->LineTo(nWidth, nStartPos);

	nStartPos = nRowHight * 6;
	pDC->MoveTo(0, nStartPos);
	pDC->LineTo(nWidth, nStartPos);

	nStartPos = nRowHight * 11;
	pDC->MoveTo(0, nStartPos);
	pDC->LineTo(nWidth, nStartPos);
	
	nStartPos = nRowHight * 19;
	pDC->MoveTo(0, nStartPos);
	pDC->LineTo(nWidth, nStartPos);

	pDC->SelectObject(pOldPen);

}



void CustomizeDrawTicks(CDC * pDC, DWORD dwContext)
{
	CIDSGridCtrl *  pGridCtrl = (CIDSGridCtrl *)dwContext;
	if(NULL == pGridCtrl || NULL == pGridCtrl->GetSafeHwnd())
	{
		return;
	}
	CRect rcClient;
	pGridCtrl->GetClientRect(&rcClient);
	rcClient.right -= 1;
	rcClient.bottom -= 1;
	int nRowHight = pGridCtrl->GetRowHeight(0);
	int nWidth = rcClient.Width();
	int nHeight = rcClient.Height();
	
    CPen cRedPen(PS_SOLID, 1,  RGB(255, 0, 0));
	
    CPen *pOldPen = pDC->SelectObject(&cRedPen);
	pDC->MoveTo(0,0);
	pDC->LineTo(rcClient.right, rcClient.top);
	pDC->LineTo(rcClient.right, rcClient.bottom);
	pDC->LineTo(rcClient.left, rcClient.bottom);
	pDC->LineTo(0,0);
	
	pDC->SelectObject(pOldPen);
}
// GridCellBase.cpp : implementation file
//
// MFC Grid Control - Main grid cell base class
//
// Provides the implementation for the base cell type of the
// grid control. No data is stored (except for state) but default
// implementations of drawing, printingetc provided. MUST be derived
// from to be used.
//
// Written by Chris Maunder <chris@codeproject.com>
// Copyright (c) 1998-2005. All Rights Reserved.
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name and all copyright 
// notices remains intact. 
//
// An email letting me know how you are using it would be nice as well. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage/loss of business that
// this product may cause.
//
// For use with CIDSGridCtrl v2.22+
//
// History:
// Ken Bertelson - 12 Apr 2000 - Split CXTPGridCell into CXTPGridCell and CIDSGridCellBase
// C Maunder     - 19 May 2000 - Fixed sort arrow drawing (Ivan Ilinov)
// C Maunder     - 29 Aug 2000 - operator= checks for NULL font before setting (Martin Richter)
// C Maunder     - 15 Oct 2000 - GetTextExtent fixed (Martin Richter)
// C Maunder     -  1 Jan 2001 - Added ValidateEdit
// Yogurt        - 13 Mar 2004 - GetCellExtent fixed
//
// NOTES: Each grid cell should take care of it's own drawing, though the Draw()
//        method takes an "erase background" paramter that is called if the grid
//        decides to draw the entire grid background in on hit. Certain ambient
//        properties such as the default font to use, and hints on how to draw
//        fixed cells should be fetched from the parent grid. The grid trusts the
//        cells will behave in a certain way, and the cells trust the grid will
//        supply accurate information.
//        
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GridCtrl.h"
#include "GridCellBase.h"

#include <math.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define UP_ARROW_CHAR			_T("↑")				// 上箭头
#define DOWN_ARROW_CHAR			_T("↓")				// 下箭头


IMPLEMENT_DYNAMIC(CIDSGridCellBase, CObject)

/////////////////////////////////////////////////////////////////////////////
// GridCellBase

CIDSGridCellBase::CIDSGridCellBase()
{
    Reset();
}

CIDSGridCellBase::~CIDSGridCellBase()
{
}

/////////////////////////////////////////////////////////////////////////////
// GridCellBase Operations

void CIDSGridCellBase::Reset()
{
    m_nState  = 0;
}

void CIDSGridCellBase::operator=(const CIDSGridCellBase& cell)
{
	if (this == &cell) return;

    SetGrid(cell.GetGrid());    // do first in case of dependencies

    SetText(cell.GetText());
    SetImage(cell.GetImage());
    SetData(cell.GetData());
    SetState(cell.GetState());
    SetFormat(cell.GetFormat());
    SetTextClr(cell.GetTextClr());
    SetBackClr(cell.GetBackClr());
    SetFont(cell.IsDefaultFont()? NULL : cell.GetFont());
    SetMargin(cell.GetMargin());
}



// ArrowTo()
//
void ArrowTo(HDC hDC, int x, int y, ARROWSTRUCT *pA) {
	
	POINT ptTo = {x, y};
	
	ArrowTo(hDC, &ptTo, pA);
}

// ArrowTo()
//
void ArrowTo(HDC hDC, const POINT *lpTo, ARROWSTRUCT *pA) {
	
	POINT pFrom;
	POINT pBase;
	POINT aptPoly[3];
	float vecLine[2];
	float vecLeft[2];
	float fLength;
	float th;
	float ta;
	
	// get from point
	MoveToEx(hDC, 0, 0, &pFrom);
	
	// set to point
	aptPoly[0].x = lpTo->x;
	aptPoly[0].y = lpTo->y;
	
	// build the line vector
	vecLine[0] = (float) aptPoly[0].x - pFrom.x;
	vecLine[1] = (float) aptPoly[0].y - pFrom.y;
	
	// build the arrow base vector - normal to the line
	vecLeft[0] = -vecLine[1];
	vecLeft[1] = vecLine[0];
	
	// setup length parameters
	fLength = (float) sqrt(vecLine[0] * vecLine[0] + vecLine[1] * vecLine[1]);
	th = pA->nWidth / (2.0f * fLength);
	ta = pA->nWidth / (2.0f * (tanf(pA->fTheta) / 2.0f) * fLength);
	
	// find the base of the arrow
	pBase.x = (int) (aptPoly[0].x + -ta * vecLine[0]);
	pBase.y = (int) (aptPoly[0].y + -ta * vecLine[1]);
	
	// build the points on the sides of the arrow
	aptPoly[1].x = (int) (pBase.x + th * vecLeft[0]);
	aptPoly[1].y = (int) (pBase.y + th * vecLeft[1]);
	aptPoly[2].x = (int) (pBase.x + -th * vecLeft[0]);
	aptPoly[2].y = (int) (pBase.y + -th * vecLeft[1]);
	
	MoveToEx(hDC, pFrom.x, pFrom.y, NULL);
	
	// draw we're fillin'...
	if(pA->bFill) {
		LineTo(hDC, aptPoly[0].x, aptPoly[0].y);
		Polygon(hDC, aptPoly, 3);
	}
	
	// ... or even jes chillin'...
	else {
		LineTo(hDC, pBase.x, pBase.y);
		LineTo(hDC, aptPoly[1].x, aptPoly[1].y);
		LineTo(hDC, aptPoly[0].x, aptPoly[0].y);
		LineTo(hDC, aptPoly[2].x, aptPoly[2].y);
		LineTo(hDC, pBase.x, pBase.y);
		MoveToEx(hDC, aptPoly[0].x, aptPoly[0].y, NULL);
	}
}
/////////////////////////////////////////////////////////////////////////////
// CIDSGridCellBase Attributes

// Returns a pointer to a cell that holds default values for this particular type of cell
CIDSGridCellBase* CIDSGridCellBase::GetDefaultCell() const
{
    if (GetGrid())
	{
        return GetGrid()->GetDefaultCell(IsFixedRow(), IsFixedCol());
	}
    return NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CIDSGridCellBase Operations

// EFW - Various changes to make it draw cells better when using alternate
// color schemes.  Also removed printing references as that's now done
// by PrintCell() and fixed the sort marker so that it doesn't draw out
// of bounds.
BOOL CIDSGridCellBase::Draw(CDC* pDC, int nRow, int nCol, CRect rect,  BOOL bEraseBkgnd /*=TRUE*/)
{
    // Note - all through this function we totally brutalise 'rect'. Do not
    // depend on it's value being that which was passed in.

    CIDSGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    if (!pGrid || !pDC)
        return FALSE;

    if( rect.Width() <= 0 || rect.Height() <= 0)  // prevents imagelist item from drawing even
        return FALSE;                             //  though cell is hidden

    //TRACE3("Drawing %scell %d, %d\n", IsFixed()? _T("Fixed ") : _T(""), nRow, nCol);

    int nSavedDC = pDC->SaveDC();
    pDC->SetBkMode(TRANSPARENT);

    // Get the default cell implementation for this kind of cell. We use it if this cell
    // has anything marked as "default"
    CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
    if (!pDefaultCell)
        return FALSE;

    // Set up text and background colours
    COLORREF TextClr, TextBkClr;

    TextClr = (GetTextClr() == CLR_DEFAULT)? pDefaultCell->GetTextClr() : GetTextClr();
    if (GetBackClr() == CLR_DEFAULT)
        TextBkClr = pDefaultCell->GetBackClr();
    else
    {
        bEraseBkgnd = TRUE;
        TextBkClr = GetBackClr();
    }

    // Draw cell background and highlighting (if necessary)
    if ( (pGrid->IsDrawFocusCell() && IsFocused()) || IsDropHighlighted() )
    {

        // Always draw even in list mode so that we can tell where the
        // cursor is at.  Use the highlight colors though.
        if(GetState() & GVIS_SELECTED)
        {
            //TextBkClr = ::GetSysColor(COLOR_HIGHLIGHT);
            TextBkClr = pGrid->GetSelectItemBKColor();
            TextClr = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
            bEraseBkgnd = TRUE;
        }

        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
        if (bEraseBkgnd)
        {
            TRY 
            {
                CBrush brush(TextBkClr);
                pDC->FillRect(rect, &brush);
            } 
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }

        // Don't adjust frame rect if no grid lines so that the
        // whole cell is enclosed.
        if(pGrid->GetGridLines() != GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

        if (pGrid->GetFrameFocusCell())
        {
                // Use same color as text to outline the cell so that it shows
                // up if the background is black.
            TRY 
            {
                CBrush brush(TextClr);
                pDC->FrameRect(rect, &brush);
            }
            CATCH(CResourceException, e)
            {
                //e->ReportError();
            }
            END_CATCH
        }
        pDC->SetTextColor(TextClr);

        // Adjust rect after frame draw if no grid lines
        if(pGrid->GetGridLines() == GVL_NONE)
        {
            rect.right--;
            rect.bottom--;
        }

		//rect.DeflateRect(0,1,1,1);  - Removed by Yogurt
    }
    else if ((GetState() & GVIS_SELECTED))
    {
        rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
//        pDC->FillSolidRect(rect, pGrid->GetSelectItemBKColor());//RGB(215, 187, 90));
		pDC->FillSolidRect(rect, pGrid->GetBkColor());//RGB(215, 187, 90));
		CPen newPen(PS_SOLID, 1, RGB(0X30, 0XF0, 0XF0));
		CPen* pOldPen = pDC->SelectObject(&newPen);
		pDC->MoveTo(rect.left, rect.bottom - 1);
		pDC->LineTo(rect.right, rect.bottom - 1);
		pDC->SelectObject(pOldPen);
        rect.right--; rect.bottom--;
		pDC->SetTextColor(TextClr);
      //  pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
    }
    else
    {
        if (bEraseBkgnd)
        {
            rect.right++; rect.bottom++;    // FillRect doesn't draw RHS or bottom
            CBrush brush(TextBkClr);
            pDC->FillRect(rect, &brush);
            rect.right--; rect.bottom--;
        }
        pDC->SetTextColor(TextClr);
    }

    // Draw lines only when wanted
    if (IsFixed() && pGrid->GetGridLines() != GVL_NONE)
    {
        CIDSCellID FocusCell = pGrid->GetFocusCell();

        // As above, always show current location even in list mode so
        // that we know where the cursor is at.
        BOOL bHiliteFixed = pGrid->GetTrackFocusCell() && pGrid->IsValid(FocusCell) &&
                            (FocusCell.row == nRow || FocusCell.col == nCol);

        // If this fixed cell is on the same row/col as the focus cell,
        // highlight it.

        if (bHiliteFixed)
        {
            rect.right++; rect.bottom++;
            pDC->DrawEdge(rect, BDR_SUNKENINNER /*EDGE_RAISED*/, BF_RECT);
            rect.DeflateRect(1,1);
        }
        else
        {   

		    /*
            CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
                darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
                *pOldPen = pDC->GetCurrentPen();
            */
#if 0
#if 1	

			COLORREF clGridLine = 0;
			if(this->GetGrid())
			{
				clGridLine = this->GetGrid()->GetFixtedGridLineColor();
			}
            CPen lightpen(PS_SOLID, 1,  clGridLine);
            CPen *pOldPen = pDC->GetCurrentPen();
            pDC->SelectObject(&lightpen);
            pDC->MoveTo(rect.right, rect.top);
            pDC->LineTo(rect.left, rect.top);
            pDC->LineTo(rect.left, rect.bottom);
			
            pDC->MoveTo(rect.right, rect.top);
            pDC->LineTo(rect.right, rect.bottom);
            pDC->LineTo(rect.left, rect.bottom);
#else
			CPen lightpen(PS_SOLID, 1,  RGB(58,58,58)),
                darkpen(PS_SOLID,  1, RGB(33,33,33)),
                *pOldPen = pDC->GetCurrentPen();
            
            
            pDC->SelectObject(&lightpen);
            pDC->MoveTo(rect.right, rect.top);
            pDC->LineTo(rect.left, rect.top);
            pDC->LineTo(rect.left, rect.bottom);

            pDC->SelectObject(&darkpen);
            pDC->MoveTo(rect.right, rect.top);
            pDC->LineTo(rect.right, rect.bottom);
            pDC->LineTo(rect.left, rect.bottom);
#endif
            pDC->SelectObject(pOldPen);
            rect.DeflateRect(1,1);
#endif
        }
    }

    // Draw Text and image
#if !defined(_WIN32_WCE_NO_PRINTING) && !defined(GRIDCONTROL_NO_PRINTING)
    if (!pDC->m_bPrinting)
#endif
    {
        CFont *pFont = GetFontObject();
		ASSERT(pFont);
        if (pFont)
            pDC->SelectObject(pFont);
    }

    //rect.DeflateRect(GetMargin(), 0); - changed by Yogurt
    rect.DeflateRect(GetMargin(), GetVMargin());    
    rect.right++;    
    rect.bottom++;

    if (pGrid->GetImageList() && GetImage() >= 0)
    {
        IMAGEINFO Info;
        if (pGrid->GetImageList()->GetImageInfo(GetImage(), &Info))
        {
            //  would like to use a clipping region but seems to have issue
            //  working with CXTPMemDC directly.  Instead, don't display image
            //  if any part of it cut-off
            //
            // CRgn rgn;
            // rgn.CreateRectRgnIndirect(rect);
            // pDC->SelectClipRgn(&rgn);
            // rgn.DeleteObject();

            /*
            // removed by Yogurt
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            int nImageHeight = Info.rcImage.bottom-Info.rcImage.top+1;
            if( nImageWidth + rect.left <= rect.right + (int)(2*GetMargin())
                && nImageHeight + rect.top <= rect.bottom + (int)(2*GetMargin())  )
            {
                pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);
            }
            */
            // Added by Yogurt
            int nImageWidth = Info.rcImage.right-Info.rcImage.left;            
            int nImageHeight = Info.rcImage.bottom-Info.rcImage.top;            
            if ((nImageWidth + rect.left <= rect.right) && (nImageHeight + rect.top <= rect.bottom))                
                pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);

            //rect.left += nImageWidth+GetMargin();
        }
    }

	// Modify by Carrot 20150611
//    // Draw sort arrow
//	int nSortCol = pGrid->GetSortColumn();
//    if (nSortCol == nCol && nRow == 0)
//    {
//        CSize size = pDC->GetTextExtent(_T("M"));
//        int nOffset = 2;
//
//        // Base the size of the triangle on the smaller of the column
//        // height or text height with a slight offset top and bottom.
//        // Otherwise, it can get drawn outside the bounds of the cell.
//        size.cy -= (nOffset * 2);
//
//        if (size.cy >= rect.Height())
//            size.cy = rect.Height() - (nOffset * 2);
//
//        size.cx = size.cy;      // Make the dimensions square
//
//        // Kludge for vertical text
//        BOOL bVertical = (GetFont()->lfEscapement == 900);
//
//        // Only draw if it'll fit!
//        //if (size.cx + rect.left < rect.right + (int)(2*GetMargin())) - changed / Yogurt
//        if (size.cx + rect.left < rect.right)
//        {
//            int nTriangleBase = rect.bottom - nOffset - size.cy;    // Triangle bottom right
//            //int nTriangleBase = (rect.top + rect.bottom - size.cy)/2; // Triangle middle right
//            //int nTriangleBase = rect.top + nOffset;                 // Triangle top right
//
//            //int nTriangleLeft = rect.right - size.cx;                 // Triangle RHS
//            //int nTriangleLeft = (rect.right + rect.left - size.cx)/2; // Triangle middle
//            //int nTriangleLeft = rect.left;                            // Triangle LHS
//
//            int nTriangleLeft;
//            if (bVertical)
//                nTriangleLeft = (rect.right + rect.left - size.cx)/2; // Triangle middle
//            else
//                nTriangleLeft = rect.right - size.cx;               // Triangle RHS
//
//            CPen penShadow(PS_SOLID, 0, ::GetSysColor(COLOR_3DSHADOW));
//            CPen penLight(PS_SOLID, 0, ::GetSysColor(COLOR_3DHILIGHT));
//			int nArrowLef = rect.right - 8;
//			BOOL bAsc = pGrid->GetSortAscending();
//            if (bAsc)
//            {
//                // Draw triangle pointing upwards
//#if 1
//				ARROWSTRUCT tArrow;
//				CBrush br1, br2, *pbrOld;
//				
//				CPen cRedPen(PS_SOLID, 1,  RGB(255, 0 , 0));
//				CPen * pOldPen = pDC->SelectObject(&cRedPen);
//				
//				br1.CreateSolidBrush(RGB(255,0,0));
//				br2.CreateSolidBrush(RGB(255,255,0));
//				pbrOld = pDC->SelectObject(&br1);
//				
//				// setup arrows
//				tArrow.nWidth = 6;
//				tArrow.fTheta = 3.14159/4 ;
//				tArrow.bFill = true;
//				pDC->SetBkColor(RGB(255,0,0));
//				
//				pDC->MoveTo(nArrowLef, rect.top + 16);
//				ArrowTo(pDC->m_hDC, nArrowLef, rect.top, &tArrow);
//
//				pDC->SelectObject(pbrOld);
//				pDC->SelectObject(pOldPen);
//#else
//                CPen *pOldPen = (CPen*) pDC->SelectObject(&penLight);
//                pDC->MoveTo( nTriangleLeft + 1, nTriangleBase + size.cy + 1);
//                pDC->LineTo( nTriangleLeft + (size.cx / 2) + 1, nTriangleBase + 1 );
//                pDC->LineTo( nTriangleLeft + size.cx + 1, nTriangleBase + size.cy + 1);
//                pDC->LineTo( nTriangleLeft + 1, nTriangleBase + size.cy + 1);
//
//                pDC->SelectObject(&penShadow);
//                pDC->MoveTo( nTriangleLeft, nTriangleBase + size.cy );
//                pDC->LineTo( nTriangleLeft + (size.cx / 2), nTriangleBase );
//                pDC->LineTo( nTriangleLeft + size.cx, nTriangleBase + size.cy );
//                pDC->LineTo( nTriangleLeft, nTriangleBase + size.cy );
//                pDC->SelectObject(pOldPen);
//#endif
//            }
//            else
//            {
//                // Draw triangle pointing downwards
//#if 1				
//				ARROWSTRUCT tArrow;
//				CBrush br1, br2, *pbrOld;
//				
//				CPen cRedPen(PS_SOLID, 1,  RGB(255, 0 , 0));
//				CPen * pOldPen = pDC->SelectObject(&cRedPen);
//				
//				br1.CreateSolidBrush(RGB(255,0,0));
//				br2.CreateSolidBrush(RGB(255,255,0));
//				pbrOld = pDC->SelectObject(&br1);
//				
//				// setup arrows
//				tArrow.nWidth = 6;
//				tArrow.fTheta = 3.14159/4 ;
//				tArrow.bFill = true;
//				pDC->SetBkColor(RGB(255,0,0));
//				
//				pDC->MoveTo(nArrowLef, rect.top );
//				ArrowTo(pDC->m_hDC, nArrowLef, rect.top + 16, &tArrow);
//				
//				pDC->SelectObject(pbrOld);
//				pDC->SelectObject(pOldPen);
//#else
//                CPen *pOldPen = (CPen*) pDC->SelectObject(&penLight);
//                pDC->MoveTo( nTriangleLeft + 1, nTriangleBase + 1 );
//                pDC->LineTo( nTriangleLeft + (size.cx / 2) + 1, nTriangleBase + size.cy + 1 );
//                pDC->LineTo( nTriangleLeft + size.cx + 1, nTriangleBase + 1 );
//                pDC->LineTo( nTriangleLeft + 1, nTriangleBase + 1 );
//    
//                pDC->SelectObject(&penShadow);
//                pDC->MoveTo( nTriangleLeft, nTriangleBase );
//                pDC->LineTo( nTriangleLeft + (size.cx / 2), nTriangleBase + size.cy );
//                pDC->LineTo( nTriangleLeft + size.cx, nTriangleBase );
//                pDC->LineTo( nTriangleLeft, nTriangleBase );
//                pDC->SelectObject(pOldPen);
//#endif
//            }
//            
//            if (!bVertical)
//                rect.right -= size.cy;
//        }
//    }

	// 绘制箭头
	BOOL bShowArrow(FALSE);
	int nSortCol = pGrid->GetSortColumn();
	CString strArrow;
	CRect rcArrow(0, 0, 0, 0);
	COLORREF clrArrow = RGB(0XFF, 0X00, 0X00);
    if (nSortCol == nCol && nRow == 0)
	{
		bShowArrow = TRUE;
		BOOL bAsc = pGrid->GetSortAscending();
		if (bAsc)
			strArrow =  UP_ARROW_CHAR;
		else
		{
			strArrow = DOWN_ARROW_CHAR;
			clrArrow = RGB(0X00, 0XFF, 0X00);
		}
		pDC->DrawText(strArrow, rcArrow, DT_CALCRECT | DT_SINGLELINE);
	}

    // We want to see '&' characters so use DT_NOPREFIX
    GetTextRect(rect);
    rect.right++;    
    rect.bottom++;
    
	//使用DT_CALCRECT，DT_CENTER，DT_EDITCONTROL，DT_WORDBREAK参数计算需要的文本高度 
	//多行垂直居中输出
	CRect rcText = rect;
	CRect rcTemp = rect;
	LPCTSTR pstrText = GetText();
	int nTextHegiht = pDC->DrawText(pstrText, rcText, DT_CALCRECT|DT_CENTER|DT_EDITCONTROL|DT_WORDBREAK);
	rect = rcTemp;//计算完成后将原来的区域赋回rect

	if (bShowArrow)
	{
		if ((GetFormat() & DT_CENTER) != 0)				// 居中
		{
			if (rcArrow.Width() + rcText.Width() > rect.Width())
			{
				rcText.right = rect.right -  rcArrow.Width();

				rcArrow.left = rcText.right;
				rcArrow.right = rect.right;
			}
			else
			{
				CRect rcTemp;
				rcTemp.top = rect.top;
				rcTemp.bottom = rect.bottom;
				rcTemp.left = rect.left + (rect.Width() - (rcText.Width() + rcArrow.Width()))/2;
				rcTemp.right = rcTemp.left +  rcText.Width() + rcArrow.Width();

				int nArrowWidth = rcArrow.Width();
				rcArrow.right = rcTemp.right;
				rcArrow.left = rcArrow.right - nArrowWidth;

				rcText.right = rcArrow.left;
				rcText.left = rcTemp.left;
			}
			rcText.top = rect.top;
			rcText.bottom = rect.bottom;

			rcArrow.top = rect.top;
			rcArrow.bottom = rect.bottom;
		}
		else
		{
			if (rcArrow.Width() + rcText.Width() > rect.Width())
			{
				rcText.right = rect.right -  rcArrow.Width();

				rcArrow.left = rcText.right;
				rcArrow.right = rect.right;
			}
			else
			{
				int nArrowWidth = rcArrow.Width();
				rcArrow.left = rcText.right;
				rcArrow.right = rcArrow.left + nArrowWidth;
			}
			rcText.left = rect.left;
			rcText.top = rect.top;
			rcText.bottom = rect.bottom;

			rcArrow.top = rect.top;
			rcArrow.bottom = rect.bottom;
		}
	}
	else
	{
		rcText = rect;
	}

	if(rcTemp.Height() > nTextHegiht)//判断高度是否超出范围，以免出现负数情况
	{
		//rect.top += (rcTemp.Height() - nTextHegiht)/2;//计算空白高度的一半，这里要用+=
	}
//	pDC->FillSolidRect(rect, RGB(0X000, 0X00, 0XFF));
	pDC->DrawText(pstrText, rcText, this->GetFormat()|DT_WORDBREAK| DT_END_ELLIPSIS); //输出垂直居中+左右居中+自动
	if (bShowArrow)
	{
		COLORREF clrOldArrow = pDC->SetTextColor(clrArrow);
		pDC->DrawText(strArrow, rcArrow, DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_END_ELLIPSIS);
		pDC->SetTextColor(clrOldArrow);
	}
   // DrawText(pDC->m_hDC, GetText(), -1, rect, DT_LEFT | DT_EDITCONTROL | DT_WORDBREAK);
   // DrawText(pDC->m_hDC, GetText(), -1, rect, this->GetFormat()  );
	
    pDC->RestoreDC(nSavedDC);

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIDSGridCellBase Mouse and Cursor events

// Not yet implemented
void CIDSGridCellBase::OnMouseEnter()
{
    TRACE0("Mouse entered cell\n");
}

void CIDSGridCellBase::OnMouseOver()
{
    //TRACE0("Mouse over cell\n");
}

// Not Yet Implemented
void CIDSGridCellBase::OnMouseLeave()
{
    TRACE0("Mouse left cell\n");
}

void CIDSGridCellBase::OnClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
   // TRACE2("Mouse Left btn up in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CIDSGridCellBase::OnClickDown( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
   // TRACE2("Mouse Left btn down in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CIDSGridCellBase::OnRClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    //TRACE2("Mouse right-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

void CIDSGridCellBase::OnDblClick( CPoint PointCellRelative)
{
    UNUSED_ALWAYS(PointCellRelative);
    //TRACE2("Mouse double-clicked in cell at x=%i y=%i\n", PointCellRelative.x, PointCellRelative.y);
}

// Return TRUE if you set the cursor
BOOL CIDSGridCellBase::OnSetCursor()
{
#ifndef _WIN32_WCE_NO_CURSOR
    SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
#endif
    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIDSGridCellBase editing

void CIDSGridCellBase::OnEndEdit() 
{
	ASSERT( FALSE); 
}

BOOL CIDSGridCellBase::ValidateEdit(LPCTSTR str)
{
    UNUSED_ALWAYS(str);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CIDSGridCellBase Sizing

BOOL CIDSGridCellBase::GetTextRect( LPRECT pRect)  // i/o:  i=dims of cell rect; o=dims of text rect
{
    if (GetImage() >= 0)
    {
        IMAGEINFO Info;

        CIDSGridCtrl* pGrid = GetGrid();
        CImageList* pImageList = pGrid->GetImageList();
        
        if (pImageList && pImageList->GetImageInfo( GetImage(), &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left+1;
            pRect->left += nImageWidth + GetMargin();
        }
    }

    return TRUE;
}

// By default this uses the selected font (which is a bigger font)
CSize CIDSGridCellBase::GetTextExtent(LPCTSTR szText, CDC* pDC /*= NULL*/)
{
    CIDSGridCtrl* pGrid = GetGrid();
    ASSERT(pGrid);

    BOOL bReleaseDC = FALSE;
    if (pDC == NULL || szText == NULL)
    {
        if (szText)
			pDC = pGrid->GetDC();
        if (pDC == NULL || szText == NULL) 
        {
            CGridDefaultCell* pDefCell = (CGridDefaultCell*) GetDefaultCell();
            ASSERT(pDefCell);
            return CSize(pDefCell->GetWidth(), pDefCell->GetHeight());
        }
        bReleaseDC = TRUE;
    }

    CFont *pOldFont = NULL,
          *pFont = GetFontObject();
    if (pFont)
        pOldFont = pDC->SelectObject(pFont);

    CSize size;
    int nFormat = GetFormat();

    // If the cell is a multiline cell, then use the width of the cell
    // to get the height
    if ((nFormat & DT_WORDBREAK) && !(nFormat & DT_SINGLELINE))
    {
        CString str = szText;
        int nMaxWidth = 0;
        while (TRUE)
        {
            int nPos = str.Find(_T('\n'));
            CString TempStr = (nPos < 0)? str : str.Left(nPos);
            int nTempWidth = pDC->GetTextExtent(TempStr).cx;
            if (nTempWidth > nMaxWidth)
                nMaxWidth = nTempWidth;

            if (nPos < 0)
                break;
            str = str.Mid(nPos + 1);    // Bug fix by Thomas Steinborn
        }
        
        CRect rect;
        rect.SetRect(0,0, nMaxWidth+1, 0);
        pDC->DrawText(szText, -1, rect, nFormat | DT_CALCRECT);
        size = rect.Size();
    }
    else
        size = pDC->GetTextExtent(szText, (int)_tcslen(szText));

    // Removed by Yogurt
    //TEXTMETRIC tm;
    //pDC->GetTextMetrics(&tm);
    //size.cx += (tm.tmOverhang);

    if (pOldFont)
        pDC->SelectObject(pOldFont);
    
    size += CSize(2*GetMargin(), 2*GetVMargin());

    // Kludge for vertical text
    LOGFONT *pLF = GetFont();
    if (pLF->lfEscapement == 900 || pLF->lfEscapement == -900)
    {
        int nTemp = size.cx;
        size.cx = size.cy;
        size.cy = nTemp;
        size += CSize(0, 4*GetVMargin());
    }
    
    if (bReleaseDC)
        pGrid->ReleaseDC(pDC);

    return size;
}


CSize CIDSGridCellBase::GetCellExtent(CDC* pDC)
{    
    CSize size = GetTextExtent(GetText(), pDC);    
    CSize ImageSize(0,0);    
    
    int nImage = GetImage();    
    if (nImage >= 0)    
    {        
        CIDSGridCtrl* pGrid = GetGrid();        
        ASSERT(pGrid);        
        IMAGEINFO Info;        
        if (pGrid->GetImageList() && pGrid->GetImageList()->GetImageInfo(nImage, &Info))         
        {            
            ImageSize = CSize(Info.rcImage.right-Info.rcImage.left,                                 
                Info.rcImage.bottom-Info.rcImage.top);            
            if (size.cx > 2*(int)GetMargin ())                
                ImageSize.cx += GetMargin();            
            ImageSize.cy += 2*(int)GetVMargin ();        
        }    
    }    
    size.cx += ImageSize.cx + 1;    
    size.cy = max(size.cy, ImageSize.cy) + 1;    
    if (IsFixed())    
    {        
        size.cx++;        
        size.cy++;    
    }    
    return size;
}

// EFW - Added to print cells so that grids that use different colors are
// printed correctly.
BOOL CIDSGridCellBase::PrintCell(CDC* pDC, int /*nRow*/, int /*nCol*/, CRect rect)
{
#if defined(_WIN32_WCE_NO_PRINTING) || defined(GRIDCONTROL_NO_PRINTING)
    return FALSE;
#else
    COLORREF crFG, crBG;
    GV_ITEM Item;

    CIDSGridCtrl* pGrid = GetGrid();
    if (!pGrid || !pDC)
        return FALSE;

    if( rect.Width() <= 0
        || rect.Height() <= 0)  // prevents imagelist item from drawing even
        return FALSE;           //  though cell is hidden

    int nSavedDC = pDC->SaveDC();

    pDC->SetBkMode(TRANSPARENT);

    if (pGrid->GetShadedPrintOut())
    {
        // Get the default cell implementation for this kind of cell. We use it if this cell
        // has anything marked as "default"
        CGridDefaultCell *pDefaultCell = (CGridDefaultCell*) GetDefaultCell();
        if (!pDefaultCell)
            return FALSE;

        // Use custom color if it doesn't match the default color and the
        // default grid background color.  If not, leave it alone.
        if(IsFixed())
            crBG = (GetBackClr() != CLR_DEFAULT) ? GetBackClr() : pDefaultCell->GetBackClr();
        else
            crBG = (GetBackClr() != CLR_DEFAULT && GetBackClr() != pDefaultCell->GetBackClr()) ?
                GetBackClr() : CLR_DEFAULT;

        // Use custom color if the background is different or if it doesn't
        // match the default color and the default grid text color.  
        if(IsFixed())
            crFG = (GetBackClr() != CLR_DEFAULT) ? GetTextClr() : pDefaultCell->GetTextClr();
        else
            crFG = (GetBackClr() != CLR_DEFAULT) ? GetTextClr() : pDefaultCell->GetTextClr();

        // If not printing on a color printer, adjust the foreground color
        // to a gray scale if the background color isn't used so that all
        // colors will be visible.  If not, some colors turn to solid black
        // or white when printed and may not show up.  This may be caused by
        // coarse dithering by the printer driver too (see image note below).
        if(pDC->GetDeviceCaps(NUMCOLORS) == 2 && crBG == CLR_DEFAULT)
            crFG = RGB(GetRValue(crFG) * 0.30, GetGValue(crFG) * 0.59,
                GetBValue(crFG) * 0.11);

        // Only erase the background if the color is not the default
        // grid background color.
        if(crBG != CLR_DEFAULT)
        {
            CBrush brush(crBG);
            rect.right++; rect.bottom++;
            pDC->FillRect(rect, &brush);
            rect.right--; rect.bottom--;
        }
    }
    else
    {
        crBG = CLR_DEFAULT;
        crFG = RGB(0, 0, 0);
    }

    pDC->SetTextColor(crFG);

    CFont *pFont = GetFontObject();
    if (pFont)
        pDC->SelectObject(pFont);

    /*
    // ***************************************************
    // Disabled - if you need this functionality then you'll need to rewrite.
    // Create the appropriate font and select into DC.
    CFont Font;
    // Bold the fixed cells if not shading the print out.  Use italic
    // font it it is enabled.
    const LOGFONT* plfFont = GetFont();
    if(IsFixed() && !pGrid->GetShadedPrintOut())
    {
        Font.CreateFont(plfFont->lfHeight, 0, 0, 0, FW_BOLD, plfFont->lfItalic, 0, 0,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
#ifndef _WIN32_WCE
            PROOF_QUALITY,
#else
            DEFAULT_QUALITY,
#endif
            VARIABLE_PITCH | FF_SWISS, plfFont->lfFaceName);
    }
    else
        Font.CreateFontIndirect(plfFont);

    pDC->SelectObject(&Font);
    // ***************************************************
    */

    // Draw lines only when wanted on fixed cells.  Normal cell grid lines
    // are handled in OnPrint.
    if(pGrid->GetGridLines() != GVL_NONE && IsFixed())
    {
        CPen lightpen(PS_SOLID, 1,  ::GetSysColor(COLOR_3DHIGHLIGHT)),
             darkpen(PS_SOLID,  1, ::GetSysColor(COLOR_3DDKSHADOW)),
            *pOldPen = pDC->GetCurrentPen();

        pDC->SelectObject(&lightpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.left, rect.top);
        pDC->LineTo(rect.left, rect.bottom);

        pDC->SelectObject(&darkpen);
        pDC->MoveTo(rect.right, rect.top);
        pDC->LineTo(rect.right, rect.bottom);
        pDC->LineTo(rect.left, rect.bottom);

        rect.DeflateRect(1,1);
        pDC->SelectObject(pOldPen);
    }

    rect.DeflateRect(GetMargin(), 0);

    if(pGrid->GetImageList() && GetImage() >= 0)
    {
        // NOTE: If your printed images look like fuzzy garbage, check the
        //       settings on your printer driver.  If it's using coarse
        //       dithering and/or vector graphics, they may print wrong.
        //       Changing to fine dithering and raster graphics makes them
        //       print properly.  My HP 4L had that problem.

        IMAGEINFO Info;
        if(pGrid->GetImageList()->GetImageInfo(GetImage(), &Info))
        {
            int nImageWidth = Info.rcImage.right-Info.rcImage.left;
            pGrid->GetImageList()->Draw(pDC, GetImage(), rect.TopLeft(), ILD_NORMAL);
            rect.left += nImageWidth+GetMargin();
        }
    }

    // Draw without clipping so as not to lose text when printed for real
	// DT_NOCLIP removed 01.01.01. Slower, but who cares - we are printing!
    DrawText(pDC->m_hDC, GetText(), -1, rect,
        GetFormat() | /*DT_NOCLIP | */ DT_NOPREFIX);

    pDC->RestoreDC(nSavedDC);

    return TRUE;
#endif
}

/*****************************************************************************
Callable by derived classes, only
*****************************************************************************/
LRESULT CIDSGridCellBase::SendMessageToParent(int nRow, int nCol, int nMessage)
{
    CIDSGridCtrl* pGrid = GetGrid();
    if( pGrid)
        return pGrid->SendMessageToParent(nRow, nCol, nMessage);
    else
        return 0;
}


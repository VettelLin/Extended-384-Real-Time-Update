// RoundButton.cpp : implementation file

#include "stdafx.h"
#include "MyCheckButton.h"
#include <GdiPlus.h>
using namespace Gdiplus;

IMPLEMENT_DYNAMIC(CMyCheckButton, CButton)

CMyCheckButton::CMyCheckButton():CFontSet(TRUE)
{
	m_bkClrUnchked = RGB(0XFF,0XFF,0XF0);
	m_bkClrChked = RGB(0,170,222);
	m_bRoundStyle = FALSE;

	m_isBtChecked = FALSE;
	m_bHasBkPic = FALSE;
	m_bHasPngBkPic = FALSE;

	m_picPath = _T("");
	m_sonPicNums = 2;
	m_iconW = 0;
	m_iLeftIndent = 2;
	m_rightEnd = 0;
	m_txClrUnchecked = RGB(0,0,0);
	m_txClrChecked = RGB(0,0,0);
	m_pngBkClr = RGB(255,255,255);
	m_cheBtHoriFontRatio = 0;
	m_cheBtVerFontRatio = 0;
	m_bShieldClickMsg = FALSE;
	m_bNeedRedrawBk = FALSE;
	m_bParentProcMouseClick = FALSE;

	m_roundStyleBkClr = RGB(255,255,255);
	m_bTickStyle = FALSE;
	m_dwWordStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;

	m_clrFrame = RGB(0,0,0);
	m_selStateSerail = 2;
}

CMyCheckButton::~CMyCheckButton()
{
	
}

BEGIN_MESSAGE_MAP(CMyCheckButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMyCheckButton message handlers

void CMyCheckButton::PreSubclassWindow()
{
	ModifyStyle(0,BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CMyCheckButton::SetBkPicture(UINT idChecked,UINT idUnChecked,BOOL bRedraw /*= FALSE*/)
{
	m_idChecked = idChecked;
	m_idUnChecked = idUnChecked;
	m_sonPicNums = 1;
	m_bHasBkPic = TRUE;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
}

void CMyCheckButton::SetBkPicture(UINT idPicture,int sonPicNums /*= 2*/,int iconW /*= 0*/)
{
	m_idChecked = idPicture;
	m_sonPicNums = sonPicNums;
	m_bHasBkPic = TRUE;
	m_iconW = iconW;
}

void CMyCheckButton::SetPngBkPic(wstring picPath,int sonPicNums /*= 2*/,int iconW /*= 0*/,COLORREF pngBkClr /*= RGB(255,255,255)*/,BOOL bInvalidate /*= FALSE*/)
{
	m_picPath = picPath;
	m_sonPicNums = sonPicNums;
	m_iconW = iconW;
	m_bHasPngBkPic = TRUE;
	if(bInvalidate && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return;
}

void CMyCheckButton::SetBtChecked(BOOL btState,BOOL bRedraw /*= FALSE*/)
{
	if(btState)
	{
		m_isBtChecked = TRUE;
	}
	else
	{
		m_isBtChecked = FALSE;
	}
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyCheckButton::GetCheckState()
{
	return m_isBtChecked;
}

void CMyCheckButton::SetShieldClickMsgState(BOOL bShieldClickMsg)
{
	m_bShieldClickMsg = bShieldClickMsg;
	return;
}

BOOL CMyCheckButton::GetShieldClickMsgState()
{
	return m_bShieldClickMsg;
}

void CMyCheckButton::SetBkColor(COLORREF bkClrUnchked,COLORREF bkClrChked,BOOL bRedraw /*= FALSE*/)
{
	m_bkClrUnchked = bkClrUnchked;
	m_bkClrChked = bkClrChked;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyCheckButton::GetBkColor(COLORREF& bkClrUnchked,COLORREF& bkClrChked)
{
	bkClrUnchked = m_bkClrUnchked;
	bkClrChked = m_bkClrChked;
	return;
}

void CMyCheckButton::SetRoundStyle(BOOL bRoundSytle,COLORREF roundStyleBkClr,BOOL bRedraw /*= FALSE*/)
{
	m_bRoundStyle = bRoundSytle;
	m_roundStyleBkClr = roundStyleBkClr;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyCheckButton::GetRoundStyle(COLORREF* pRoundStyleBkClr /*= NULL*/)
{
	if(NULL != pRoundStyleBkClr)
	{
		*pRoundStyleBkClr = m_roundStyleBkClr;
	}

	return m_bRoundStyle;
}

void CMyCheckButton::SetTickStyle(BOOL bTickSytle,BOOL bRedraw /*= FALSE*/)
{
	m_bTickStyle = bTickSytle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return;
}

BOOL CMyCheckButton::GetTickStyle()
{
	return m_bTickStyle;
}

void CMyCheckButton::SetWordStyle(DWORD dwWordStyle,BOOL bRedraw /*= FALSE*/)
{
	m_dwWordStyle = dwWordStyle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return;
}

DWORD CMyCheckButton::GetWordStyle()
{
	return m_dwWordStyle;
}

void CMyCheckButton::SetFrameColor(COLORREF clrFrame,BOOL bRedraw /*= FALSE*/)
{
	m_clrFrame = clrFrame;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

BOOL CMyCheckButton::GetFrameColor()
{
	return m_clrFrame;
}

void CMyCheckButton::SetSelectStateSerail(int selStateSerial,BOOL bRedraw /*= FALSE*/)
{
	m_selStateSerail = selStateSerial;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

int CMyCheckButton::GetSelectStateSerail()
{
	return m_selStateSerail;
}

void CMyCheckButton::SetTextColor(COLORREF txClrUnchecked,COLORREF txClrChecked /*= RGB(0,0,0)*/,BOOL bRedraw /*= FALSE*/)
{
	m_txClrUnchecked = txClrUnchecked;
	m_txClrChecked = txClrChecked;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

COLORREF CMyCheckButton::GetTextColor()
{
	return m_txClrUnchecked;
}

void CMyCheckButton::SetBeNeedRedrawBk(BOOL bNeedRedrawBk,BOOL bRedraw /*= FALSE*/)
{
	m_bNeedRedrawBk = bNeedRedrawBk;

	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
	return;
}

BOOL CMyCheckButton::GetBeNeedRedrawBk()
{
	return m_bNeedRedrawBk;
}

void CMyCheckButton::SetPngBkColor(COLORREF pngBkClr,BOOL bRedraw /*= FALSE*/)
{
	m_pngBkClr = pngBkClr;

	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
	return;
}

COLORREF CMyCheckButton::GetPngBkColor()
{
	return m_pngBkClr;
}

void CMyCheckButton::SetBeParentProcessMouseClick(BOOL bParentProcMouseClick,BOOL bRedraw /*= FALSE*/)
{
	m_bParentProcMouseClick = bParentProcMouseClick;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMyCheckButton::GetBeParentProcessMouseClick()
{
	return m_bParentProcMouseClick;
}

void CMyCheckButton::SetIconTextInterval(int iLeftIndent,BOOL bRedraw /*= FALSE*/)
{
	m_iLeftIndent = iLeftIndent;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMyCheckButton::GetIconTextInterval()
{
	return m_iLeftIndent;
}

void CMyCheckButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if(m_sonPicNums < 1)
	{
		return;
	}

	CRect rect = lpDrawItemStruct->rcItem;
	CDC cDC;
	cDC.Attach(lpDrawItemStruct->hDC);

	CDC memDC;
	memDC.CreateCompatibleDC(&cDC);
	CBitmap bitMapTemp;
	bitMapTemp.CreateCompatibleBitmap(&cDC,rect.Width(),rect.Height());
	CBitmap* pOldTempBitmap = memDC.SelectObject(&bitMapTemp);

	memDC.BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

	if(m_bHasBkPic)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(&cDC);
		CBitmap bitMap;

		if(1 == m_sonPicNums)
		{
			if(!m_isBtChecked)
			{
				bitMap.LoadBitmap(m_idUnChecked);
			}
			else
			{
				bitMap.LoadBitmap(m_idChecked);
			}
		}
		else
		{
			bitMap.LoadBitmap(m_idChecked);
		}
		
		BITMAP bit;
		bitMap.GetBitmap(&bit);
		CBitmap* pOldBitmap = dcMem.SelectObject(&bitMap);

		int startX = 0;
		int bkW = 0;
		if(1 == m_sonPicNums)
		{
			bkW = bit.bmWidth;
		}
		else 
		{
			if(!m_isBtChecked)
			{
				startX = 0;
			}
			else
			{
				startX = bit.bmWidth / m_sonPicNums;
			}
			bkW = bit.bmWidth / m_sonPicNums;
		}

		memDC.SetStretchBltMode(HALFTONE);
		memDC.StretchBlt(0,0,(double(m_iconW * GeneScreenXWidth)) / m_uiGeneDesWidth,rect.Height(),&dcMem,startX,0,bkW,bit.bmHeight,SRCCOPY);
		dcMem.SelectObject(pOldBitmap);
		bitMap.DeleteObject();
		dcMem.DeleteDC();
	}
	else if(m_bHasPngBkPic)
	{
		Graphics graphics(memDC.m_hDC);
		Image image(m_picPath.c_str());

		if(m_bNeedRedrawBk)
		{
			memDC.FillSolidRect(rect,m_pngBkClr);
		}

		ImageAttributes ImgAtt;
		ImgAtt.SetWrapMode(WrapModeTileFlipXY);
		if(0 != m_sonPicNums)
		{
			if(0 == m_iconW)
			{
				if(!m_isBtChecked)
				{
					graphics.DrawImage(&image,Rect(0,0,rect.Width(),rect.Height()),0,0,image.GetWidth() / m_sonPicNums,image.GetHeight(),UnitPixel,&ImgAtt);
				}
				else           
				{
					graphics.DrawImage(&image,Rect(0,0,rect.Width(),rect.Height()),image.GetWidth() / m_sonPicNums * (m_selStateSerail - 1),0,image.GetWidth() / m_sonPicNums,image.GetHeight(),UnitPixel,&ImgAtt);
				}
			}
			else
			{
				Rect rcImg;
				rcImg.X = 0;
				int iTemp = image.GetHeight();
				iTemp = (rect.Height() - iTemp) / 2;
				if(iTemp < 0)
				{
					iTemp = 0;
				}
				rcImg.Y = iTemp;
				rcImg.Width = m_iconW;
				rcImg.Height = image.GetHeight();
				if(rcImg.Height > rect.Height())
				{
					rcImg.Height = rect.Height();
				}

				int iStartX = 0;
				if(2 == m_sonPicNums)
				{
					if(m_isBtChecked)
					{
						iStartX = image.GetWidth() / m_sonPicNums;
					}
				}
				else if(3 == m_sonPicNums)
				{
					if(IsWindowEnabled())
					{
						if(m_isBtChecked)
						{
							iStartX = image.GetWidth() / m_sonPicNums;
						}
					}
					else
					{
						iStartX = image.GetWidth() / m_sonPicNums * 2;
					}
				}
				else if(4 == m_sonPicNums)
				{
					if(IsWindowEnabled())
					{
						if(m_isBtChecked)
						{
							iStartX = image.GetWidth() / m_sonPicNums;
						}
					}
					else
					{
						if(m_isBtChecked)
						{
							iStartX = image.GetWidth() / m_sonPicNums * 3;
						}
						else
						{
							iStartX = image.GetWidth() / m_sonPicNums * 2;
						}
					}
				}

				graphics.DrawImage(&image,rcImg,iStartX,0,image.GetWidth() / m_sonPicNums,image.GetHeight(),UnitPixel,&ImgAtt);
			}
		}

		graphics.ReleaseHDC(memDC.m_hDC);
	} 
	else
	{
		if(m_bTickStyle)
		{
			CRect tempRc(rect.left + (rect.Width() - 14) / 2,rect.top + (rect.Height() - 14) / 2,rect.left + (rect.Width() + 14) / 2,rect.top + (rect.Height() + 14) / 2);
			CBrush brush;
			brush.CreateSolidBrush(RGB(0,0,0));
			memDC.FrameRect(tempRc,&brush);
			tempRc.DeflateRect(1,1,1,1);
			if(m_isBtChecked)
			{
				memDC.FillSolidRect(tempRc,RGB(1,93,197));
			}
			else
			{
				memDC.FillSolidRect(tempRc,RGB(252,255,254));
			}
			brush.DeleteObject();
		}
		else if(m_bRoundStyle)
		{
			memDC.FillSolidRect(rect,m_roundStyleBkClr);
			Color bkClr;
			if(m_isBtChecked)
			{
				bkClr.SetFromCOLORREF(m_bkClrChked);
				DrawRound(&memDC,rect,TRUE,bkClr,1,TRUE,bkClr);
			}
			else
			{
				bkClr.SetFromCOLORREF(m_bkClrUnchked);
				DrawRound(&memDC,rect,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,bkClr);
			}
		}
		else
		{
			CPen pen;
			pen.CreatePen(PS_SOLID,1,m_clrFrame);
			CPen* pOldPen = memDC.SelectObject(&pen);
			
			if(m_isBtChecked)  
			{
				CBrush activeBrush;
				activeBrush.CreateSolidBrush(m_bkClrChked);

				memDC.FillRect(&rect,&activeBrush);
				activeBrush.DeleteObject();
			}
			else
			{
				CBrush normalBrush;
				normalBrush.CreateSolidBrush(m_bkClrUnchked);
				memDC.FillRect(&rect,&normalBrush);
				normalBrush.DeleteObject();
			}

			memDC.SelectObject(&pOldPen);
			pen.DeleteObject();
		}
	}

	CString strText;
	GetWindowText(strText);
	if(0 != strText.GetLength())
	{	
		CRect rcText = rect;
		rcText.left = m_iconW + m_iLeftIndent;
		rcText.right -= m_rightEnd;

		CFont *pOldFont = NULL;
		if(m_isBtChecked)
		{
			pOldFont = memDC.SelectObject(&m_fontSet);
			memDC.SetTextColor(m_txClrChecked);
		}
		else
		{
			pOldFont = memDC.SelectObject(&m_fontSet);
			memDC.SetTextColor(m_txClrUnchecked);
		}

		memDC.SetBkMode(TRANSPARENT);
		memDC.DrawText(strText,rcText,m_dwWordStyle);
		memDC.SelectObject(pOldFont);
	}

	cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);
	memDC.SelectObject(pOldTempBitmap);
	memDC.DeleteDC();

	cDC.Detach();
	return;
}

void CMyCheckButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bShieldClickMsg)
	{
		return;
	}
	CButton::OnLButtonDown(nFlags, point);
}


void CMyCheckButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bShieldClickMsg)
	{
		return;
	}

	if(!m_bParentProcMouseClick)
	{
		m_isBtChecked = !m_isBtChecked;
		Invalidate();
	}
	
	CButton::OnLButtonUp(nFlags, point);
}

void CMyCheckButton::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(m_bShieldClickMsg)
	{
		return;
	}

	if(!m_bParentProcMouseClick)
	{
		m_isBtChecked = !m_isBtChecked;
		Invalidate();
	}

	__super::OnLButtonDblClk(nFlags, point);
}

void CMyCheckButton::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
}

HBRUSH CMyCheckButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = __super::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

BOOL CMyCheckButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

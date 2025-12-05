// MyStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MyProgressBar.h"

IMPLEMENT_DYNAMIC(CMyProgressBar, CStatic)

CMyProgressBar::CMyProgressBar(BOOL bAutoDynamic /*= FALSE*/)
{
	m_bkColor = CLR_SOFT_THEME;
	m_iconWidth = 0;

	m_bSingleLine = TRUE;
	m_nFormat = DT_CENTER | DT_VCENTER | DT_WORDBREAK;

	m_bFstPaint = TRUE;
	m_pMemDc = NULL;
	m_bAutoDynamic = bAutoDynamic;
	m_fstPaintWidth = 0;
	m_fstPaintHeight = 0;
	m_hBitmap = NULL;

	m_dProgress = 0;
	m_pngPicPath = _T("");

	m_clrRopBk = Color(0XFF,0XFF,0XFF);
	m_clrRopFrame = Color(0XFF,0XFF,0XFF);

	m_clrBk = COLOR_2LEVEL_THEME;
	m_clrFrame = COLOR_2LEVEL_THEME;
#ifdef XINYI_THEME
	m_clrBk = Color(0,37,94);
	m_clrFrame = Color(0,37,94);
#endif
}

CMyProgressBar::~CMyProgressBar()
{
	if(NULL != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}

	MyDeletePtr(&m_pMemDc);
}

BEGIN_MESSAGE_MAP(CMyProgressBar, CStatic)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CMyProgressBar::SetSingleLineState(BOOL bSingleLine/* = TRUE*/)
{
	m_bSingleLine = bSingleLine;
}

BOOL CMyProgressBar::GetSingleLineState()
{
	return m_bSingleLine;
}

void CMyProgressBar::SetWordStyle(UINT nFormat,BOOL bInvalidate /*= FALSE*/)
{
	m_nFormat = nFormat;
	if(bInvalidate)
	{
		Invalidate();
	}
}

void CMyProgressBar::GetWordStyle(UINT& nFormat)
{
	nFormat = m_nFormat;
}

void CMyProgressBar::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	CRect rect = lpDrawItemStruct->rcItem;
	CRect rcClient(rect);
	CDC cDC;
	cDC.Attach(lpDrawItemStruct->hDC);
	ASSERT(NULL != cDC.GetSafeHdc());

	CDC memDC;
	memDC.CreateCompatibleDC(&cDC);
	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(&cDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDC.SelectObject(&bitMapCB);

	if(m_bAutoDynamic)
	{
		if(m_bFstPaint)
		{
			memDC.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&cDC,0,0,SRCCOPY);

			m_pMemDc = new CDC;
			BOOL ret = m_pMemDc->CreateCompatibleDC(&cDC);
			m_hBitmap = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //´´½¨¼æÈÝÎ»Í¼
			m_pMemDc->SelectObject(m_hBitmap);	
			m_pMemDc->BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

			m_fstPaintWidth = rect.Width();
			m_fstPaintHeight = rect.Height();
			m_bFstPaint = FALSE;
		}
		else
		{
			memDC.StretchBlt(0,0,rect.Width(),rect.Height(),m_pMemDc,0,0,m_fstPaintWidth,m_fstPaintHeight,SRCCOPY);
		}
	}
	else
	{
		memDC.FillSolidRect(&rect,m_bkColor); 
	}

	int iArcAngle = 4 * m_dVertRatio;
	
	DrawRoundRectangleNew(&memDC,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,iArcAngle,iArcAngle,iArcAngle,iArcAngle,FALSE,m_clrFrame,1,TRUE,m_clrBk,1);
	
	int iColorW = rect.Width() * m_dProgress;
	if(iColorW > 0)
	{
		if(iColorW <= iArcAngle * 2)
		{
			CDC memDcTemp;
			memDcTemp.CreateCompatibleDC(&cDC);
			CBitmap bitMapCBTemp;
			bitMapCBTemp.CreateCompatibleBitmap(&cDC,rect.Width(),rect.Height());
			CBitmap* pOldBitmapCbTemp = memDcTemp.SelectObject(&bitMapCBTemp);

			memDcTemp.BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&memDC,rect.left,rect.top,SRCCOPY);
			DrawRoundRectangleNew(&memDcTemp,rect.left + 1,rect.top + 1,rect.Width() - 3,rect.Height() - 3,iArcAngle,iArcAngle,iArcAngle,iArcAngle,FALSE,m_clrRopFrame,1,TRUE,m_clrRopBk,1);
			memDC.BitBlt(rect.left,rect.top,iColorW,rect.Height(),&memDcTemp,rect.left,rect.top,SRCCOPY);

			memDcTemp.SelectObject(pOldBitmapCbTemp);
			bitMapCBTemp.DeleteObject();
			memDcTemp.DeleteDC();
		}
		else
		{
			DrawRoundRectangleNew(&memDC,rect.left + 1,rect.top + 1,(rect.Width() - 2) * m_dProgress - 1,rect.Height() - 3,iArcAngle,iArcAngle,iArcAngle,iArcAngle,FALSE,m_clrRopFrame,1,TRUE,m_clrRopBk,1);
		}
	}
	
	CString strText;
	GetWindowText(strText);
	if(0 != strText.GetLength())
	{
		int nOldBkMode = memDC.SetBkMode(TRANSPARENT);
		CFont *pOldFont = memDC.SelectObject(&m_fontSet);
		CSize szExtent = memDC.GetTextExtent(strText,_tcslen(strText));
		memDC.SetTextColor(m_textColor);
	
		rect.left = m_iconWidth;

		int strHeight = rect.Height();
		if((!(m_nFormat & DT_SINGLELINE)) && (m_nFormat & DT_VCENTER))
		{
			CRect rectNew(rect);
			strHeight = memDC.DrawText(strText,&rectNew,m_nFormat | DT_CALCRECT);
		}
		rect.top += (rect.Height() - strHeight) / 2;
		memDC.DrawText(strText,&rect,m_nFormat);

		memDC.SelectObject(pOldFont);
		memDC.SetBkMode(nOldBkMode);
	}
	
	cDC.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDC.DeleteDC();

	cDC.Detach();

#ifdef _DEBUG
	clock_t endMs = clock();
#endif

	return;
}

void CMyProgressBar::PreSubclassWindow()
{
	ModifyStyle(0,SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}

void CMyProgressBar::SetBkColor(COLORREF bkColor)
{
	m_bkColor = bkColor;
}

COLORREF CMyProgressBar::GetBkColor()
{
	return m_bkColor;
}

void CMyProgressBar::SetTextColor(COLORREF textColor,BOOL bInvalidate /*= FALSE*/)
{
	m_textColor = textColor;
	if(bInvalidate)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
}

COLORREF CMyProgressBar::GetTextColor()
{
	return m_textColor;
}

void CMyProgressBar::SetBmpBkPic(int bkPicId,int iconWidth /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_bkPicID = bkPicId;
	m_iconWidth = iconWidth;

	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
}

void CMyProgressBar::SetPngBkPic(wstring picPath,BOOL bInvalidate /*= FALSE*/)
{
	m_pngPicPath = picPath;
	if(bInvalidate)
	{
		Invalidate(FALSE);
	}
}

void CMyProgressBar::SetCurProgress(double dProgress,BOOL bRedraw /*= FALSE*/)
{
	if(dProgress < 0)
	{
		dProgress = 0;
	}

	if(dProgress > 1)  
	{
		dProgress = 1;
	}

	m_dProgress = dProgress;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}	

	return;
}

void CMyProgressBar::SetZero(BOOL bRedraw /*= FALSE*/)
{
	m_dProgress = 0;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}	

	return;
}

void CMyProgressBar::SetOneHundred(BOOL bRedraw /*= FALSE*/)
{
	m_dProgress = 1;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}	

	return;
}

double CMyProgressBar::GetProgress()
{
	return m_dProgress;
}

void CMyProgressBar::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
}

LRESULT CMyProgressBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_NCHITTEST == message || WM_NCLBUTTONDOWN == message || WM_NCLBUTTONDBLCLK == message)
	{
		return ::DefWindowProc(m_hWnd,message,wParam,lParam);
	}

	return __super::WindowProc(message, wParam, lParam);
}

void CMyProgressBar::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CMyProgressBar::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

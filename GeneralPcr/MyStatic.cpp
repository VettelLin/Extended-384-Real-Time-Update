// MyStatic.cpp : implementation file
//

#include "stdafx.h"
#include "MyStatic.h"

IMPLEMENT_DYNAMIC(CMyStatic, CStatic)

CMyStatic::CMyStatic(BOOL bAutoDynamic /*= FALSE*/)
{
	m_bkColor = RGB(0XFF,0XFA,0XF0);

	m_iconWidth = 0;

	m_bSingleLine = TRUE;
	m_bHasBkPic = FALSE;
	m_bHasPngBkPic = FALSE;
	m_pngPicPath = _T("");
	m_nFormat = DT_LEFT | DT_VCENTER | DT_WORDBREAK;

	m_bDynamic = FALSE;

	m_bFstPaint = TRUE;
	m_pMemDc = NULL;
	m_bAutoDynamic = bAutoDynamic;
	m_fstPaintWidth = 0;
	m_fstPaintHeight = 0;
	m_sonPicNums = 1;
	m_curState = 1;
	m_hBitmap = NULL;

	m_iconLeftIndent = 0;
	m_iconRightIndent = 0;

	m_bTwinkle = FALSE;
	m_bTwinkleShow = TRUE;

	m_bHaveRoundRectBk = FALSE;
	m_clrFourAngleBk = CLR_SOFT_THEME;
	m_clrRoundRectBk = COLOR_2LEVEL_THEME;
	m_iArcRadius = 4;
}

CMyStatic::~CMyStatic()
{
	MyDeletePtr(&m_pMemDc);

	if(NULL != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
}

BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CMyStatic::SetSingleLineState(BOOL bSingleLine/* = TRUE*/)
{
	m_bSingleLine = bSingleLine;
}

BOOL CMyStatic::GetSingleLineState()
{
	return m_bSingleLine;
}

void CMyStatic::SetWordStyle(UINT nFormat,BOOL bInvalidate /*= FALSE*/)
{
	m_nFormat = nFormat;
	if(bInvalidate)
	{
		Invalidate();
	}
}

void CMyStatic::GetWordStyle(UINT& nFormat)
{
	nFormat = m_nFormat;
}

void CMyStatic::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	CRect rect = lpDrawItemStruct->rcItem;
	CRect rcClient(rect);

	CDC cDC;
	cDC.Attach(lpDrawItemStruct->hDC);
	ASSERT(NULL != cDC.GetSafeHdc());

	CDC memDCTemp;
	memDCTemp.CreateCompatibleDC(&cDC);
	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(&cDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDCTemp.SelectObject(&bitMapCB);

	memDCTemp.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&cDC,0,0,SRCCOPY);

	if(m_bHaveRoundRectBk)
	{
		memDCTemp.FillSolidRect(&rect,m_clrFourAngleBk);
		DrawRoundRectangleNew(&memDCTemp,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,m_iArcRadius,m_iArcRadius,m_iArcRadius,m_iArcRadius,FALSE,m_clrRoundRectBk,1,TRUE,m_clrRoundRectBk,1);
	}

	if(m_bAutoDynamic)
	{
		if(m_bFstPaint)
		{
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
			memDCTemp.StretchBlt(0,0,rect.Width(),rect.Height(),m_pMemDc,0,0,m_fstPaintWidth,m_fstPaintHeight,SRCCOPY);
		}
	}

	if(m_bHasBkPic)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(&cDC);
		CBitmap bitMap;
		bitMap.LoadBitmap(m_bkPicID);
		CBitmap* pOldBitmap = dcMem.SelectObject(&bitMap);
		BITMAP bit;
		bitMap.GetBitmap(&bit);
		memDCTemp.SetStretchBltMode(HALFTONE);
		memDCTemp.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,0,0,bit.bmWidth,bit.bmHeight,SRCCOPY);
		dcMem.SelectObject(pOldBitmap);
		bitMap.DeleteObject();
		dcMem.DeleteDC();
	}
	else if(m_bHasPngBkPic)
	{
		if(m_bTwinkle)
		{
			if(!m_bTwinkleShow)
			{
				if(-1 == m_curState || m_curState > m_sonPicNums)
				{
					memDCTemp.FillSolidRect(rcClient,RGB(3,91,199));
				}
				else
				{
					memDCTemp.FillSolidRect(rcClient,RGB(3,91,199));
					int curState = 2;
					if(3 == m_curState || 5 == m_curState)
					{
						curState = 4;
					}
					Graphics graphics(memDCTemp.m_hDC);
					Image image(m_pngPicPath);

					int bkW = image.GetWidth() / m_sonPicNums;
					int startX = (curState - 1) * bkW;

					graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
					ImageAttributes ImgAtt;
					ImgAtt.SetWrapMode(WrapModeTileFlipXY);
					Rect destRect;
					destRect.X = m_iconLeftIndent;
					if(rect.Height() > image.GetHeight())
					{
						destRect.Y = (rect.Height() - image.GetHeight()) / 2;
					}

					if(bkW > rect.Width() - m_iconLeftIndent)
					{
						destRect.Width = rect.Width() - m_iconLeftIndent;
					}
					else
					{
						destRect.Width = bkW;
					}

					destRect.Height = rect.Height() - destRect.Y * 2;

					if(0 == m_iconWidth)
					{
						destRect = Rect(rect.left,rect.top,rect.Width(),rect.Height());
					}

					graphics.DrawImage(&image,destRect,startX,0,bkW,image.GetHeight(),UnitPixel,&ImgAtt);
					graphics.ReleaseHDC(memDCTemp.m_hDC);
				}
			}
			else
			{
				Graphics graphics(memDCTemp.m_hDC);
				Image image(m_pngPicPath);

				int bkW = image.GetWidth() / m_sonPicNums;
				int startX = (m_curState - 1) * bkW;

				graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
				ImageAttributes ImgAtt;
				ImgAtt.SetWrapMode(WrapModeTileFlipXY);
				Rect destRect;
				destRect.X = m_iconLeftIndent;
				if(rect.Height() > image.GetHeight())
				{
					destRect.Y = (rect.Height() - image.GetHeight()) / 2;
				}

				if(bkW > rect.Width() - m_iconLeftIndent)
				{
					destRect.Width = rect.Width() - m_iconLeftIndent;
				}
				else
				{
					destRect.Width = bkW;
				}

				destRect.Height = rect.Height() - destRect.Y * 2;

				if(0 == m_iconWidth)
				{
					destRect = Rect(rect.left,rect.top,rect.Width(),rect.Height());
				}

				graphics.DrawImage(&image,destRect,startX,0,bkW,image.GetHeight(),UnitPixel,&ImgAtt);
				graphics.ReleaseHDC(memDCTemp.m_hDC);
			}
		}
		else
		{
			if(-1 == m_curState || m_curState > m_sonPicNums)
			{
				memDCTemp.FillSolidRect(rcClient,RGB(38,38,38));
			}
			else
			{
				Graphics graphics(memDCTemp.m_hDC);
				Image image(m_pngPicPath);

				int bkW = image.GetWidth() / m_sonPicNums;
				int startX = (m_curState - 1) * bkW;

				graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
				ImageAttributes ImgAtt;
				ImgAtt.SetWrapMode(WrapModeTileFlipXY);
				Rect destRect;
				destRect.X = m_iconLeftIndent;
				if(rect.Height() > image.GetHeight())
				{
					destRect.Y = (rect.Height() - image.GetHeight()) / 2;
				}

				if(bkW > rect.Width() - m_iconLeftIndent)
				{
					destRect.Width = rect.Width() - m_iconLeftIndent;
				}
				else
				{
					destRect.Width = bkW;
				}

				destRect.Height = rect.Height() - destRect.Y * 2;

				if(0 == m_iconWidth)
				{
					destRect = Rect(rect.left,rect.top,rect.Width(),rect.Height());
				}

				graphics.DrawImage(&image,destRect,startX,0,bkW,image.GetHeight(),UnitPixel,&ImgAtt);
				graphics.ReleaseHDC(memDCTemp.m_hDC);
			}
		}
	}
	else
	{
		if(m_bDynamic)
		{
			memDCTemp.FillSolidRect(&rect,m_bkColor);
		}
	}

	CString strText;
	GetWindowText(strText);
	if(0 != strText.GetLength())
	{
		int nOldBkMode = memDCTemp.SetBkMode(TRANSPARENT);
		CFont *pOldFont = memDCTemp.SelectObject(&m_fontSet);
		memDCTemp.SetTextColor(m_textColor);
	
		rect.left =  m_iconLeftIndent + m_iconRightIndent + m_iconWidth;

		if(m_bSingleLine)
		{
			m_nFormat |= DT_SINGLELINE;
		}	
		
		int strHeight = rect.Height();
		if((!(m_nFormat & DT_SINGLELINE)) && (m_nFormat & DT_VCENTER))
		{
			CRect rectNew(rect);
			memDCTemp.DrawText(strText,&rectNew,m_nFormat | DT_CALCRECT);
			strHeight = rectNew.Height();
		}

		int iTemp1 = (rect.Height() - strHeight) / 2;
		rect.top += iTemp1;
		rect.bottom -= iTemp1;
		memDCTemp.DrawText(strText,&rect,m_nFormat);
		memDCTemp.SelectObject(pOldFont);
		memDCTemp.SetBkMode(nOldBkMode);
	}
	
	cDC.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDCTemp,0,0,SRCCOPY);

	memDCTemp.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDCTemp.DeleteDC();

	cDC.Detach();

#ifdef _DEBUG
	clock_t endMs = clock();
#endif

	return;
}

void CMyStatic::PreSubclassWindow()
{
	ModifyStyle(0,SS_OWNERDRAW);
	CStatic::PreSubclassWindow();
}

void CMyStatic::SetCurState(int curState,BOOL bRedraw /*= FALSE*/)
{
	m_curState = curState;

	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
}

void CMyStatic::SetBkColor(COLORREF bkColor)
{
	m_bkColor = bkColor;
}

COLORREF CMyStatic::GetBkColor()
{
	return m_bkColor;
}

void CMyStatic::SetTextColor(COLORREF textColor,BOOL bRedraw /*= FALSE*/)
{
	m_textColor = textColor;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
}

COLORREF CMyStatic::GetTextColor()
{
	return m_textColor;
}

void CMyStatic::SetBmpBkPic(int bkPicId,int iconWidth /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_bkPicID = bkPicId;
	m_iconWidth = iconWidth;
	m_bHasBkPic = TRUE;

	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
			UpdateWindow();
		}
	}
}

void CMyStatic::SetPngBkPic(CString picPath,int sonPicNums/* = 1*/,int iconWidth /*= 0*/,int iconLeftIndent /*= 0*/,int iconRightIndent /*= 0*/,BOOL bInvalidate /*= FALSE*/)
{
	m_pngPicPath = picPath;
	m_iconWidth = iconWidth;
	m_iconLeftIndent = iconLeftIndent;
	m_iconRightIndent = iconRightIndent;
	m_bHasPngBkPic = TRUE;
	if(sonPicNums > 0)
	{
		m_sonPicNums = sonPicNums;
	}
	
	if(bInvalidate)
	{
		Invalidate(FALSE);
	}
}

void CMyStatic::SetRoundRectBkParam(BOOL bHaveRoundRectBk,COLORREF clrFourAngle,Color clrRoundRectBk,int iArcRadius,BOOL bRedraw /*= FALSE*/)
{
	m_bHaveRoundRectBk = bHaveRoundRectBk;
	m_clrFourAngleBk = clrFourAngle;
	m_clrRoundRectBk = clrRoundRectBk;
	m_iArcRadius = iArcRadius;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

Color CMyStatic::GetRoundRectBkColor()
{
	return m_clrRoundRectBk;
}

int CMyStatic::GetRoundRectArcSize()
{
	return m_iArcRadius;
}

void CMyStatic::SetBDynamic(BOOL bDyNaminc)
{
	m_bDynamic = bDyNaminc;
}

BOOL CMyStatic::GetBDynamic()
{
	return m_bDynamic;
}

void CMyStatic::StartTwinkle()
{
	SetTimer(INSTRUSTATELAMPTWINKLE,1000,NULL);
	m_bTwinkle = TRUE;
}

void CMyStatic::StopTwinkle()
{
	KillTimer(INSTRUSTATELAMPTWINKLE);
	m_bTwinkle = FALSE;
	Invalidate(FALSE);
}

void CMyStatic::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
}

LRESULT CMyStatic::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(WM_NCHITTEST == message || WM_NCLBUTTONDOWN == message || WM_NCLBUTTONDBLCLK == message)
	{
		return ::DefWindowProc(m_hWnd,message,wParam,lParam);
	}

	return __super::WindowProc(message, wParam, lParam);
}

void CMyStatic::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	__super::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CMyStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyStatic::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)      
	{      
	case INSTRUSTATELAMPTWINKLE:       
		m_bTwinkleShow = !m_bTwinkleShow;
		Invalidate(FALSE);
		break;      
	default:   
		break;     

	} 
	CStatic::OnTimer(nIDEvent);
}
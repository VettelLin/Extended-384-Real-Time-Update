// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyButton.h"
// CMyButton

IMPLEMENT_DYNAMIC(CMyButton, CButton)

CMyButton::CMyButton(BOOL bAutoDynamic /*= FALSE*/)
{
	m_bTracking = FALSE;
	m_ctrlState = CTRL_BUTTON_NOFOCUS;
	m_bHasBkPic = FALSE;
	m_bHasPathBkPic = FALSE; 
	m_bHasPngBkPic = FALSE;
	m_textColor = RGB(0,0,0);
	m_sonPicNums = 1;
	m_iconW = 0;
	m_iLeftIndent = 0;
	m_iRightIndent = 0;

	m_rightEnd = 0;

	m_txClrNormal = RGB(0X39,0X45,0X65);
	m_txClrFocused = RGB(0X39,0X45,0X65);
	m_txClrClicked = RGB(0X39,0X45,0X65);
	m_txClrDisabled = RGB(0XC6,0XC4,0XC4);

	m_edgeClrNormal = Color(0XDD,0XDD,0XDD);
	m_edgeClrFocused = Color(0XDD,0XDD,0XDD);
	m_edgeClrClicked = Color(0XDD,0XDD,0XDD);
	m_edgeClrDisabled = Color(0XDD,0XDD,0XDD);

	m_bkClrNormal = Color(0XF1,0XF1,0XF1);
	m_bkClrFocused = Color(0XFF,0XFF,0XFF);
	m_bkClrClicked = Color(0XD8,0XD8,0XD8);
	m_bkClrDisabled = Color(0XF1,0XF1,0XF1);

	m_bkClrNormalEnd = Color(0XD8,0XD8,0XD8);
	m_bkClrFocusedEnd = Color(0XF1,0XF1,0XF1);
	m_bkClrClickedEnd = Color(0XCA,0XCA,0XCA);
	m_bkClrDisabledEnd = Color(0XD8,0XD8,0XD8);

	m_bDrawFrame = FALSE;
	m_strPngPicPath = L"";

	m_bRoundRectBk = FALSE;
	m_clrRoundRectBk = RGB(3,91,199);
	m_bGradientBk = FALSE;

	m_bInMiddle = FALSE;
	m_bDlgSizeBt = FALSE;
	m_eDlgSizeBtType = EDSBT_CLOSE;

	m_bTwinkle = FALSE;
	m_bTwinkleShow = TRUE;
	m_curState = 1;
	m_curState2 = 1;

	m_bTextDown = FALSE;
	m_bIconBt = FALSE;
	m_bHaveTxPushedFormat = TRUE;

	m_dWordStyle = DT_CENTER | DT_SINGLELINE | DT_VCENTER;

	m_bFstPaint = TRUE;
	m_pMemDc = NULL;
	m_bAutoDynamic = bAutoDynamic;
	m_fstPaintWidth = 0;
	m_fstPaintHeight = 0;
	m_hBitmap = NULL;
}

CMyButton::~CMyButton()
{
	m_bTracking = FALSE;

	if(NULL != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}

	MyDeletePtr(&m_pMemDc);
}

BEGIN_MESSAGE_MAP(CMyButton, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,&CMyButton::OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER,&CMyButton::OnMouseHover)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMyButton message handlers
void CMyButton::PreSubclassWindow()
{
	ModifyStyle(0,BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CMyButton::SetBmpBkPic(UINT idPic,int sonPicNums /*= 2*/,int btIconW /*= 0*/,int leftStart /*= 0*/,int rightEnd /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_idNormal = idPic;
	m_sonPicNums = sonPicNums;
	m_iconW = btIconW;
	m_iLeftIndent = leftStart;
	m_rightEnd = rightEnd;
	m_bHasBkPic = TRUE;

	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
}

void CMyButton::SetPngBkPic(CString strPicPath,int sonPicNums,int iIconWidth /*= 0*/,int iLeftIndent /*= 0*/,int iRightIndent /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_sonPicNums = sonPicNums;
	m_bHasPngBkPic = TRUE;
	m_strPngPicPath = strPicPath;
	m_iconW = iIconWidth;
	m_iLeftIndent = iLeftIndent;
	m_iRightIndent = iRightIndent;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
	return;
}

void CMyButton::SetNewBmpBkPic(UINT idPic,int sonPicNums,BOOL bInvalidate)
{
	m_idNormal = idPic;
	m_sonPicNums = sonPicNums;
	m_bHasBkPic = TRUE;
	if(bInvalidate)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
}

void CMyButton::SetBmpBkPic(UINT idNormal,UINT idFocus,UINT idSelected,UINT idDisabled,int btIconW /*= 0*/,int leftStart /*= 0*/,int rightEnd /*= 0*/)
{
	m_idNormal = idNormal;
	m_idFocus = idFocus;
	m_idSelected = idSelected;
	m_idDisabled = idDisabled;
	m_sonPicNums = 1;
	m_iconW = btIconW;
	m_iLeftIndent = leftStart;
	m_rightEnd = rightEnd;
	m_bHasBkPic = TRUE;
}

void CMyButton::SetTextColor(COLORREF textColor)
{
	m_textColor = textColor;
}

COLORREF CMyButton::GetTextColor()
{
	return m_textColor;
}

void CMyButton::SetBkColor(Color bkClrNormal,Color bkClrFocused,Color bkClrClicked,Color bkClrDisabled,BOOL bRedraw/* = FALSE*/)
{
	m_bkClrNormal = bkClrNormal;
	m_bkClrFocused = bkClrFocused;
	m_bkClrClicked = bkClrClicked;
	m_bkClrDisabled = bkClrDisabled;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
	return;
}

void CMyButton::GetBkColor(Color& bkClrNormal,Color& bkClrFocused,Color& bkClrClicked,Color& bkClrDisabled)
{
	bkClrNormal = m_bkClrNormal;
	bkClrFocused = m_bkClrFocused;
	bkClrClicked = m_bkClrClicked;
	bkClrDisabled = m_bkClrDisabled;
	return;
}

void CMyButton::SetBkColorEnd(Color bkClrNormal,Color bkClrFocused,Color bkClrClicked,Color bkClrDisabled,BOOL bRedraw/* = FALSE*/)
{
	m_bkClrNormalEnd = bkClrNormal;
	m_bkClrFocusedEnd = bkClrFocused;
	m_bkClrClickedEnd = bkClrClicked;
	m_bkClrDisabledEnd = bkClrDisabled;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
	return;
}

void CMyButton::GetBkColorEnd(Color& bkClrNormal,Color& bkClrFocused,Color& bkClrClicked,Color& bkClrDisabled)
{
	bkClrNormal = m_bkClrNormalEnd;
	bkClrFocused = m_bkClrFocusedEnd;
	bkClrClicked = m_bkClrClickedEnd;
	bkClrDisabled = m_bkClrDisabledEnd;
	return;
}

void CMyButton::SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw/* = FALSE*/)
{
	m_txClrNormal = txClrNormal;
	m_txClrFocused = txClrFocused;
	m_txClrClicked = txClrClicked;
	m_txClrDisabled = txClrDisabled;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
	return;
}

void CMyButton::GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled)
{
	txClrNormal = m_txClrNormal;
	txClrFocused = m_txClrFocused;
	txClrClicked = m_txClrClicked;
	txClrDisabled = m_txClrDisabled;
	return;
}

void CMyButton::SetEdgeColor(Color edgeClrNormal,Color edgeClrFocused,Color edgeClrClicked,Color edgeClrDisabled,BOOL bRedraw/* = FALSE*/)
{
	m_edgeClrNormal = edgeClrNormal;
	m_edgeClrFocused = edgeClrFocused;
	m_edgeClrClicked = edgeClrClicked;
	m_edgeClrDisabled = edgeClrDisabled;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
	return;
}

void CMyButton::GetEdgeColor(Color& edgeClrNormal,Color& edgeClrFocused,Color& edgeClrClicked,Color& edgeClrDisabled)
{
	edgeClrNormal = m_edgeClrNormal;
	edgeClrFocused = m_edgeClrFocused;
	edgeClrClicked = m_edgeClrClicked;
	edgeClrDisabled = m_edgeClrDisabled;
}

void CMyButton::SetRoundRectParam(BOOL bRoundRect,COLORREF clrRoundRectBk,BOOL bGradientBk,BOOL bRedraw /*= FALSE*/)
{
	m_bRoundRectBk = bRoundRect;
	m_clrRoundRectBk = clrRoundRectBk;
	m_bGradientBk = bGradientBk;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyButton::GetRoundRectParam(BOOL& bRoundRect,COLORREF& clrRoundRectBk,BOOL& bGradientBk)
{
	bRoundRect = m_bRoundRectBk;
	clrRoundRectBk = m_clrRoundRectBk;
	bGradientBk = m_bGradientBk;
}

void CMyButton::SetDrawFrame(BOOL bDrawFrame,BOOL bRedraw /*= FALSE*/)
{
	m_bDrawFrame = bDrawFrame;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyButton::GetDrawFrame()
{
	return m_bDrawFrame;
}

void CMyButton::SetBeInMiddle(BOOL bInMiddle,BOOL bRedraw /*= FALSE*/)
{
	m_bInMiddle = bInMiddle;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyButton::GetBeInMiddle()
{
	return m_bInMiddle;
}

void CMyButton::SetDlgSizeBtParam(BOOL bDlgSizeBt,EDLGSIZEBTTYPE eDlgSizeBtType,BOOL bRedraw /*= FALSE*/)
{
	m_bDlgSizeBt = bDlgSizeBt;
	m_eDlgSizeBtType = eDlgSizeBtType;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyButton::GetBeDlgSizeBt()
{
	return m_bDlgSizeBt;
}

EDLGSIZEBTTYPE CMyButton::GetDlgSizeBtType()
{
	return m_eDlgSizeBtType;
}

void CMyButton::SetBeTextDown(BOOL bTextDown,BOOL bRedraw /*= FALSE*/)
{
	m_bTextDown = bTextDown;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyButton::GetBeTextDown()
{
	return m_bTextDown;
}

void CMyButton::SetWordStyle(DWORD dwWordStyle,BOOL bRedraw /*= FALSE*/)
{
	m_dWordStyle = dwWordStyle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

DWORD CMyButton::GetWordStyle()
{
	return m_dWordStyle;
}

void CMyButton::SetCurState(int curState,int curState2,BOOL bRedraw /*= FALSE*/)
{
	m_curState = curState;
	m_curState2 = curState2;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

int CMyButton::GetCurState()
{
	return m_curState;
}

int CMyButton::GetCurState2()
{
	return m_curState2;
}

BOOL CMyButton::GetBeTwinkle()
{
	return m_bTwinkle;
}

void CMyButton::EnableWindow(BOOL bEnabled)
{
	if(bEnabled)
	{
		m_ctrlState = CTRL_BUTTON_NOFOCUS;
	}
	else
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_CANCEL | TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);
		m_bTracking = FALSE;
	
		m_ctrlState = CTRL_BUTTON_DISABLE;
	}
	CButton::EnableWindow(bEnabled);
}

void CMyButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	CRect rect = lpDrawItemStruct->rcItem;
	CDC cDC;
	cDC.Attach(lpDrawItemStruct->hDC);
	ASSERT(NULL != cDC.GetSafeHdc());

	if(m_bHasBkPic)
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(&cDC);
		CBitmap bitMap;
		if(1 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				bitMap.LoadBitmap(m_idNormal);
				break;
			case CTRL_BUTTON_FOCUS:
				bitMap.LoadBitmap(m_idFocus); 
				break;
			case CTRL_BUTTON_SELECTED:
				bitMap.LoadBitmap(m_idSelected);
				break;
			case CTRL_BUTTON_DISABLE:
				bitMap.LoadBitmap(m_idDisabled);
				break;
			}
		}
		else if(m_sonPicNums > 1)
		{
			bitMap.LoadBitmap(m_idNormal);
		}
		
		CBitmap *pOldBit = dcMem.SelectObject(&bitMap);
		BITMAP bit;
		bitMap.GetBitmap(&bit);

		int startX = 0;
		int bkW = 0;
		if(1 == m_sonPicNums)
		{
			bkW = bit.bmWidth;
		}
		else if(2 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_SELECTED:
				startX = bit.bmWidth / 2;
				break;
			case CTRL_BUTTON_DISABLE:
				startX = bit.bmWidth / 2;
				break;
			}
			bkW = bit.bmWidth / 2;
		}
		else if(3 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				startX = bit.bmWidth / 3;
				break;
			case CTRL_BUTTON_SELECTED:
				startX = bit.bmWidth / 3 * 2;
				break;
			case CTRL_BUTTON_DISABLE:
				startX = bit.bmWidth / 3 * 2;
				break;
			}
			bkW = bit.bmWidth / 3;
		}
		else if(4 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				startX = bit.bmWidth / 4;
				break;
			case CTRL_BUTTON_SELECTED:
				startX = bit.bmWidth / 2;
				break;
			case CTRL_BUTTON_DISABLE:
				startX = bit.bmWidth / 4 * 3;
				break;
			}
			bkW = bit.bmWidth / 4;
		}
		cDC.SetStretchBltMode(HALFTONE);
		cDC.StretchBlt(0,0,rect.Width(),rect.Height(),&dcMem,startX,0,bkW,bit.bmHeight,SRCCOPY);
		dcMem.SelectObject(pOldBit);
		bitMap.DeleteObject();
		dcMem.DeleteDC();

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				m_textColor = m_txClrNormal;
				break;
			case CTRL_BUTTON_FOCUS:
				m_textColor = m_txClrFocused; 
				break;
			case CTRL_BUTTON_SELECTED:
				m_textColor = m_txClrClicked;
				break;
			case CTRL_BUTTON_DISABLE:
				m_textColor = m_txClrDisabled;
				break;
			}
	
			CFont *pOldFont = cDC.SelectObject(&m_fontSet);
			CSize szExtent = cDC.GetTextExtent(strText,_tcslen(strText));
			rect.left = (m_iconW * GeneScreenXWidth) / m_uiGeneDesWidth + m_iLeftIndent;
			
			UINT nFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
			
			rect.right -= m_rightEnd;
			if(lpDrawItemStruct->itemState & ODS_SELECTED)
			{
				rect.OffsetRect(1,1);
			}

			int nOldBkMode = cDC.SetBkMode(TRANSPARENT);
			cDC.SetTextColor(m_textColor);
			cDC.DrawText(strText,&rect,nFormat);
			cDC.SelectObject(pOldFont);
			cDC.SetBkMode(nOldBkMode);
		}
	}
	else if(m_bHasPngBkPic && m_bTwinkle)
	{
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //创建兼容位图
		HGDIOBJ hOldGdiObject = memDc.SelectObject(hBmp);

		memDc.BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

		int curState = m_curState;

		if(CTRL_BUTTON_SELECTED == m_ctrlState || CTRL_BUTTON_DISABLE == m_ctrlState)
		{
			curState = 5;
		}
		else
		{
			if(m_bTwinkleShow)
			{
				curState = m_curState2;
			}
		}

		if(-1 == curState || curState > m_sonPicNums)
		{
			memDc.FillSolidRect(rect,RGB(3,91,199));
		}
		else
		{
			Graphics graphics(memDc.m_hDC);
			Image image(m_strPngPicPath);

			int bkW = image.GetWidth() / m_sonPicNums;
			int startX = (curState - 1) * bkW;

			graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
			ImageAttributes ImgAtt;
			ImgAtt.SetWrapMode(WrapModeTileFlipXY);
			Rect destRect;
			destRect.X = 0;
			if(rect.Height() > image.GetHeight())
			{
				destRect.Y = (rect.Height() - image.GetHeight()) / 2;
			}

			if(bkW > rect.Width())
			{
				destRect.Width = rect.Width();
			}
			else
			{
				destRect.Width = bkW;
			}

			destRect.Height = rect.Height() - destRect.Y * 2;

			graphics.DrawImage(&image,destRect,startX,0,bkW,image.GetHeight(),UnitPixel,&ImgAtt);
			graphics.ReleaseHDC(memDc.m_hDC);
		}

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(hOldGdiObject);
		::DeleteObject(hBmp);
		memDc.DeleteDC();
	}
	else if(m_bHasPngBkPic)
	{
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //创建兼容位图
		HGDIOBJ hOldGdiObject = memDc.SelectObject(hBmp);

		COLORREF clrText = m_textColor;
		if(m_bRoundRectBk)
		{
			memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),m_clrRoundRectBk);

			Color clrFrame,clrBkStart,clrBkEnd;
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				clrText = m_txClrNormal;
				clrFrame = m_edgeClrNormal;
				clrBkStart = m_bkClrNormal;
				clrBkEnd = m_bkClrNormalEnd;
				break;
			case CTRL_BUTTON_FOCUS: 
				clrText = m_txClrFocused; 
				clrFrame = m_edgeClrFocused;
				clrBkStart = m_bkClrFocused;
				clrBkEnd = m_bkClrFocusedEnd;
				break;
			case CTRL_BUTTON_SELECTED:
				clrText = m_txClrClicked;
				clrFrame = m_edgeClrClicked;
				clrBkStart = m_bkClrClicked;
				clrBkEnd = m_bkClrClickedEnd;
				break;
			case CTRL_BUTTON_DISABLE:
				clrText = m_txClrDisabled;
				clrFrame = m_edgeClrDisabled;
				clrBkStart = m_bkClrDisabled;
				clrBkEnd = m_bkClrDisabledEnd;
				break;
			}

			if(m_bGradientBk)
			{
				DrawGradientRoundRectangle(&memDc,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,4,4,4,4,TRUE,clrFrame,1,TRUE,clrBkStart,clrBkEnd,1);
			}
			else
			{
				DrawRoundRectangleNew(&memDc,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,4,4,4,4,TRUE,clrFrame,1,TRUE,clrBkStart,1);
			}
		}
		else
		{
			memDc.FillSolidRect(&rect,RGB(0XFF,0XFF,0XFF)); 
		}

		Graphics graphics(memDc.m_hDC);
		Image image(m_strPngPicPath);

		int startX = 0;
		int bkW = 0;
		if(1 == m_sonPicNums)
		{
			bkW = image.GetWidth();
		}
		else if(2 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_SELECTED:
				startX = image.GetWidth() / 2;
				break;
			case CTRL_BUTTON_DISABLE:
				startX = image.GetWidth() / 2;
				break;
			}
			bkW = image.GetWidth() / 2;
		}
		else if(3 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				startX = image.GetWidth() / 3;
				break;
			case CTRL_BUTTON_SELECTED:
				startX = image.GetWidth() / 3 * 2;
				break;
			case CTRL_BUTTON_DISABLE:
				startX = image.GetWidth() / 3 * 2;
				break;
			}
			bkW = image.GetWidth() / 3;
		}
		else if(4 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				startX = image.GetWidth() / 4;
				break;
			case CTRL_BUTTON_SELECTED:
				startX = image.GetWidth() / 2;
				break;
			case CTRL_BUTTON_DISABLE:
				startX = image.GetWidth() / 4 * 3;
				break;
			}
			bkW = image.GetWidth() / 4;
		}
		else if(5 == m_sonPicNums)	//报警按钮使用
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				startX = 0;
				break;
			case CTRL_BUTTON_SELECTED:
				startX = image.GetWidth() / 5 * 3;
				break;
			case CTRL_BUTTON_DISABLE:
				startX = image.GetWidth() / 5 * 3;
				break;
			}
			bkW = image.GetWidth() / 5;
		}

		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
		ImageAttributes ImgAtt;
		ImgAtt.SetWrapMode(WrapModeTileFlipXY);

		Rect destRect;
		if(0 != m_iconW)
		{
			destRect.X = m_iLeftIndent;
			if(rect.Height() > image.GetHeight())
			{
				destRect.Y = (rect.Height() - image.GetHeight()) / 2;
			}

			if(bkW > rect.Width() - m_iLeftIndent)
			{
				destRect.Width = rect.Width() - m_iLeftIndent;
			}
			else
			{
				destRect.Width = bkW;
			}
			destRect.Height = rect.Height() - destRect.Y * 2;
		}
		else
		{
			if(m_bInMiddle)
			{
				if(rect.Width() > bkW)
				{
					destRect.X = (rect.Width() - bkW) / 2;
				}

				if(rect.Height() > image.GetHeight())
				{
					destRect.Y = (rect.Height() - image.GetHeight()) / 2;
				}
				destRect.Width = bkW;
				destRect.Height = image.GetHeight();
			}
			else
			{
				destRect.X = 0;
				destRect.Y = 0;
				destRect.Width = rect.Width();
				if(m_bTextDown)
				{
					memDc.FillSolidRect(&rect,RGB(246,247,251));
					destRect.Height = image.GetHeight();
				}
				else
				{
					destRect.Height = rect.Height();
				}
			}
		}

		graphics.DrawImage(&image,destRect,startX,0,bkW,image.GetHeight(),UnitPixel,&ImgAtt);
		graphics.ReleaseHDC(memDc.m_hDC);

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				clrText = m_txClrNormal;
				break;
			case CTRL_BUTTON_FOCUS:
				clrText = m_txClrFocused; 
				break;
			case CTRL_BUTTON_SELECTED:
				clrText = m_txClrClicked;
				break;
			case CTRL_BUTTON_DISABLE:
				clrText = m_txClrDisabled;
				break;
			}

			CFont *pOldFont = memDc.SelectObject(&m_fontSet);
			CRect rectText = lpDrawItemStruct->rcItem;

			if(m_bHaveTxPushedFormat)
			{
				UINT state = lpDrawItemStruct->itemState;
				if(state & ODS_SELECTED)
				{
					rectText.OffsetRect(1,1);
				}
			}

			int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
			memDc.SetTextColor(clrText);
			rectText.left += (m_iconW + m_iLeftIndent + m_iRightIndent);
			memDc.DrawText(strText,&rectText,m_dWordStyle);
			
			memDc.SelectObject(pOldFont);
			memDc.SetBkMode(nOldBkMode);
		}

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(hOldGdiObject);
		::DeleteObject(hBmp);
		memDc.DeleteDC();
	}
	else if(m_bIconBt)
	{
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //创建兼容位图
		HGDIOBJ hOldGdiObject = memDc.SelectObject(hBmp);

		Color clrBk,clrFrame;
		BOOL bFill = TRUE;
		switch(m_ctrlState)
		{
		case CTRL_BUTTON_NOFOCUS:
			clrBk = m_iConBtInfo.m_clrNormal;
			clrFrame = m_iConBtInfo.m_clrFrameNormal;
			m_textColor = m_txClrNormal;
			if(m_iConBtInfo.m_bNormalNotFill)
			{
				bFill = FALSE;
			}
			break;
		case CTRL_BUTTON_FOCUS:
			clrBk = m_iConBtInfo.m_clrHover;
			clrFrame = m_iConBtInfo.m_clrFrameHover;
			m_textColor = m_txClrFocused;
			if(m_iConBtInfo.m_bHoverNotFill)
			{
				bFill = FALSE;
			}
			break;
		case CTRL_BUTTON_SELECTED:
			clrBk = m_iConBtInfo.m_clrSelected;
			clrFrame = m_iConBtInfo.m_clrFrameSelected;
			m_textColor = m_txClrClicked;
			break;
		case CTRL_BUTTON_DISABLE:
			clrBk = m_iConBtInfo.m_clrDisabled;
			clrFrame = m_iConBtInfo.m_clrFrameDisabled;
			m_textColor = m_txClrDisabled;
			break;
		}
		
		if(m_bAutoDynamic)
		{
			if(m_bFstPaint)
			{
				memDc.BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

				m_pMemDc = new CDC;
				BOOL ret = m_pMemDc->CreateCompatibleDC(&cDC);
				m_hBitmap = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //创建兼容位图
				m_pMemDc->SelectObject(m_hBitmap);	
				m_pMemDc->BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

				m_fstPaintWidth = rect.Width();
				m_fstPaintHeight = rect.Height();
				m_bFstPaint = FALSE;
			}
			else
			{
				memDc.StretchBlt(0,0,rect.Width(),rect.Height(),m_pMemDc,0,0,m_fstPaintWidth,m_fstPaintHeight,SRCCOPY);
			}
		}
		else
		{
			memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),RGB(m_iConBtInfo.m_clrNormal.GetR(),m_iConBtInfo.m_clrNormal.GetG(),m_iConBtInfo.m_clrNormal.GetB()));
		}

		if(m_iConBtInfo.m_bRoundRectBk)
		{
			DrawRoundRectangleNew(&memDc,rect.left,rect.top,rect.Width() - m_iConBtInfo.m_iBoderLineW,
				rect.Height() - m_iConBtInfo.m_iBoderLineW,m_iConBtInfo.m_iRoundRadius,m_iConBtInfo.m_iRoundRadius,m_iConBtInfo.m_iRoundRadius,m_iConBtInfo.m_iRoundRadius,TRUE,clrFrame,m_iConBtInfo.m_iBoderLineW,bFill,clrBk,1);
		}
		else
		{
			memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),RGB(clrBk.GetR(),clrBk.GetG(),clrBk.GetB()));
		}

		if(m_iConBtInfo.m_bUseIcon)
		{
			Graphics graphics(memDc.m_hDC);
			Image image(m_iConBtInfo.m_strIconPath);

			graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
			ImageAttributes ImgAtt;
			ImgAtt.SetWrapMode(WrapModeTileFlipXY);

			int iStartX = 0;
			int iSonIconW = image.GetWidth() / m_iConBtInfo.m_iSonIconNum;
			if(2 == m_iConBtInfo.m_iSonIconNum)
			{
				switch(m_ctrlState)
				{
				case CTRL_BUTTON_NOFOCUS:
					if(m_iConBtInfo.m_bSonIconPosOrder)
					{
						iStartX = 0;
					}
					else
					{
						iStartX = iSonIconW;
					}
					break;
				case CTRL_BUTTON_FOCUS:
					if(m_iConBtInfo.m_bSonIconPosOrder)
					{
						iStartX = 0;
					}
					else
					{
						iStartX = iSonIconW;
					}
					break;
				case CTRL_BUTTON_SELECTED:
					if(m_iConBtInfo.m_bSonIconPosOrder)
					{
						iStartX = iSonIconW;
					}
					else
					{
						iStartX = 0;
					}
					break;
				case CTRL_BUTTON_DISABLE:
					if(m_iConBtInfo.m_bSonIconPosOrder)
					{
						iStartX = iSonIconW;
					}
					else
					{
						iStartX = 0;
					}
					break;
				}
			}

			graphics.DrawImage(&image,m_iConBtInfo.m_rcIcon,iStartX,0,iSonIconW,image.GetHeight(),UnitPixel,&ImgAtt);
			graphics.ReleaseHDC(memDc.m_hDC);
		}

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			CFont *pOldFont = memDc.SelectObject(&m_fontSet);
			int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
			memDc.SetTextColor(m_textColor);
			memDc.DrawText(strText,&m_iConBtInfo.m_rcText,DT_CENTER | DT_SINGLELINE | DT_VCENTER);

			memDc.SelectObject(pOldFont);
			memDc.SetBkMode(nOldBkMode);
		}

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(hOldGdiObject);
		::DeleteObject(hBmp);
		memDc.DeleteDC();
	}
	else if(m_bRoundRectBk)
	{
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);

		CBitmap bitMapCB;
		bitMapCB.CreateCompatibleBitmap(&cDC,rect.Width(),rect.Height());
		CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

		memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),m_clrRoundRectBk);

		Color clrFrame,clrBkStart,clrBkEnd;
		COLORREF clrText;
		switch(m_ctrlState)
		{
		case CTRL_BUTTON_NOFOCUS:
			clrText = m_txClrNormal;
			clrFrame = m_edgeClrNormal;
			clrBkStart = m_bkClrNormal;
			clrBkEnd = m_bkClrNormalEnd;
			break;
		case CTRL_BUTTON_FOCUS: 
			clrText = m_txClrFocused; 
			clrFrame = m_edgeClrFocused;
			clrBkStart = m_bkClrFocused;
			clrBkEnd = m_bkClrFocusedEnd;
			break;
		case CTRL_BUTTON_SELECTED:
			clrText = m_txClrClicked;
			clrFrame = m_edgeClrClicked;
			clrBkStart = m_bkClrClicked;
			clrBkEnd = m_bkClrClickedEnd;
			break;
		case CTRL_BUTTON_DISABLE:
			clrText = m_txClrDisabled;
			clrFrame = m_edgeClrDisabled;
			clrBkStart = m_bkClrDisabled;
			clrBkEnd = m_bkClrDisabledEnd;
			break;
		}

		if(m_bGradientBk)
		{
			DrawGradientRoundRectangle(&memDc,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,4,4,4,4,TRUE,clrFrame,1,TRUE,clrBkStart,clrBkEnd,1);
		}
		else
		{
			DrawRoundRectangleNew(&memDc,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,4,4,4,4,TRUE,clrFrame,1,TRUE,clrBkStart,1);
		}

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			CFont *pOldFont = memDc.SelectObject(&m_fontSet);
			CRect rectText = lpDrawItemStruct->rcItem;

			int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
			memDc.SetTextColor(clrText);
			memDc.DrawText(strText,&rectText,DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			memDc.SelectObject(pOldFont);
			memDc.SetBkMode(nOldBkMode);
		}

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(pOldBitmapCB);
		bitMapCB.DeleteObject();
		memDc.DeleteDC();
	}
	else if(m_bDlgSizeBt)
	{ 
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);

		CBitmap bitMapCB;
		bitMapCB.CreateCompatibleBitmap(&cDC,rect.Width(),rect.Height());
		CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

		Color clrRbBk = Color(0,0,0);
		switch(m_ctrlState)
		{
		case CTRL_BUTTON_NOFOCUS:
			clrRbBk = m_bkClrNormal;
			break;
		case CTRL_BUTTON_FOCUS: 
			clrRbBk = m_bkClrFocused; 
			break;
		case CTRL_BUTTON_SELECTED:
			clrRbBk = m_bkClrClicked;
			break;
		case CTRL_BUTTON_DISABLE:
			clrRbBk = m_bkClrDisabled;
			break;
		}

		if(m_bAutoDynamic)
		{
			if(m_bFstPaint)
			{
				memDc.BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

				m_pMemDc = new CDC;
				BOOL ret = m_pMemDc->CreateCompatibleDC(&cDC);
				m_hBitmap = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //创建兼容位图
				m_pMemDc->SelectObject(m_hBitmap);	
				m_pMemDc->BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

				m_fstPaintWidth = rect.Width();
				m_fstPaintHeight = rect.Height();
				m_bFstPaint = FALSE;
			}
			else
			{
				memDc.StretchBlt(0,0,rect.Width(),rect.Height(),m_pMemDc,0,0,m_fstPaintWidth,m_fstPaintHeight,SRCCOPY);
			}
		}
		else
		{
			memDc.BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);
		}

		Graphics graphics(memDc.m_hDC);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		//绘制背景
		if(CTRL_BUTTON_NOFOCUS == m_ctrlState)
		{
			//memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),RGB(clrRbBk.GetR(),clrRbBk.GetG(),clrRbBk.GetB()));
		}
		else
		{
			DrawRoundRectangleNew(&memDc,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,4,4,4,4,FALSE,clrRbBk,1,TRUE,clrRbBk);
		}

		//绘制“x”标识
		if(EDSBT_MINIMUM == m_eDlgSizeBtType)
		{
			int iWidth = 12 * m_dHoriRatio,iHeight = 2;
			Pen gdiplusPen(Color(0XFF,0XFF,0XFF),2);
			int iXStart = (rect.Width() - iWidth) / 2;
			int iYStart = (rect.Height() - iHeight) / 2;
			graphics.DrawLine(&gdiplusPen,Point(iXStart,iYStart),Point(iXStart + iWidth,iYStart));
		}
		else if(EDSBT_ZOOM == m_eDlgSizeBtType)
		{
			int iWidth = 12 * m_dHoriRatio,iHeight = 12 * m_dVertRatio;
			Pen gdiplusPen(Color(0XFF,0XFF,0XFF),2);
			int iTempInt1 = (rect.Width() - iWidth) / 2;
			int iTempInt2 = (rect.Height() - iHeight) / 2;

			CBrush brush;
			brush.CreateSolidBrush(RGB(0XFF,0XFF,0XFF));
			CRect rcTemp(iTempInt1,iTempInt2,iTempInt1 + iWidth,iTempInt2 + iHeight);
			memDc.FrameRect(rcTemp,&brush);
			rcTemp.DeflateRect(1,1,1,1);
			memDc.FrameRect(rcTemp,&brush);

			/*graphics.DrawRectangle(&gdiplusPen,iTempInt1,iTempInt2,iWidth,iHeight);
			iTempInt1 += 2;
			iTempInt2 += 2;
			iWidth -= 4;
			iHeight -= 4;
			graphics.DrawRectangle(&gdiplusPen,iTempInt1,iTempInt2,iWidth,iHeight);*/
			//memDc.FillSolidRect(iTempInt1 + 2,iTempInt2 + 2,iWidth - 4,iHeight - 4,CLR_SOFT_THEME);
		}
		else if(EDSBT_CLOSE == m_eDlgSizeBtType)
		{
			int iBtWH = 12 * m_dHoriRatio,iBtHeight = 12 * m_dVertRatio;
			CRect rcCloseBt = CRect((rect.Width() - iBtHeight) / 2,(rect.Height() - iBtWH) / 2,rect.Width() / 2 + iBtWH / 2,rect.Height() / 2 + iBtHeight / 2);
			Pen gdiplusPen(Color(0XFF,0XFF,0XFF),2);
			Point ptStart,ptEnd;
			ptStart.X = rcCloseBt.left;
			ptStart.Y = rcCloseBt.top;
			ptEnd.X = rcCloseBt.right;
			ptEnd.Y = rcCloseBt.bottom;
			graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);
			ptStart.X = rcCloseBt.right;
			ptStart.Y = rcCloseBt.top;
			ptEnd.X = rcCloseBt.left;
			ptEnd.Y = rcCloseBt.bottom;
			graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);
		}
		
		graphics.ReleaseHDC(memDc.m_hDC);

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			CFont *pOldFont = memDc.SelectObject(&m_fontSet);
			CRect rectText = lpDrawItemStruct->rcItem;

			int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
			memDc.SetTextColor(m_textColor);
			memDc.DrawText(strText,&rectText,DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			memDc.SelectObject(pOldFont);
			memDc.SetBkMode(nOldBkMode);
		}

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(pOldBitmapCB);
		bitMapCB.DeleteObject();
		memDc.DeleteDC();
	}
	else
	{
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);

		CBitmap bitMapCB;
		bitMapCB.CreateCompatibleBitmap(&cDC,rect.Width(),rect.Height());
		CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

		//memDc.BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

		CPen borderPen;
		CBrush bkBrush;

		switch(m_ctrlState)
		{
		case CTRL_BUTTON_NOFOCUS:
			borderPen.CreatePen(PS_SOLID,1,RGB(m_edgeClrNormal.GetR(),m_edgeClrNormal.GetG(),m_edgeClrNormal.GetB()));
			bkBrush.CreateSolidBrush(RGB(m_bkClrNormal.GetR(),m_bkClrNormal.GetG(),m_bkClrNormal.GetB())); 
			m_textColor = m_txClrNormal;
			break;
		case CTRL_BUTTON_FOCUS:
			borderPen.CreatePen(PS_SOLID,2,RGB(m_edgeClrFocused.GetR(),m_edgeClrFocused.GetG(),m_edgeClrFocused.GetB()));
			bkBrush.CreateSolidBrush(RGB(m_bkClrFocused.GetR(),m_bkClrFocused.GetG(),m_bkClrFocused.GetB())); 
			m_textColor = m_txClrFocused; 
			break;
		case CTRL_BUTTON_SELECTED:
			borderPen.CreatePen(PS_SOLID,2,RGB(m_edgeClrClicked.GetR(),m_edgeClrClicked.GetG(),m_edgeClrClicked.GetB()));
			bkBrush.CreateSolidBrush(RGB(m_bkClrClicked.GetR(),m_bkClrClicked.GetG(),m_bkClrClicked.GetB())); 
			m_textColor = m_txClrClicked;
			break;
		case CTRL_BUTTON_DISABLE:
			borderPen.CreatePen(PS_SOLID,2,RGB(m_edgeClrDisabled.GetR(),m_edgeClrDisabled.GetG(),m_edgeClrDisabled.GetB()));
			bkBrush.CreateSolidBrush(RGB(m_bkClrDisabled.GetR(),m_bkClrDisabled.GetG(),m_bkClrDisabled.GetB())); 
			m_textColor = m_txClrDisabled;
			break;
		}

		if(!m_bDrawFrame)
		{
			memDc.FillRect(&rect,&bkBrush);			
		}
		else
		{
			//绘制边框
			POINT pt;
			pt.x = 5;
			pt.y = 5;
			CPen *pOldPen = memDc.SelectObject(&borderPen);
			memDc.RoundRect(&rect,pt);
			memDc.SelectObject(pOldPen);

			//绘制背景
			rect.DeflateRect(2,2,2,2);
			CBrush *pOldBrush = memDc.SelectObject(&bkBrush);
			memDc.Rectangle(rect);
			memDc.SelectObject(pOldBrush);
		}

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			CFont *pOldFont = memDc.SelectObject(&m_fontSet);
			CSize szExtent = memDc.GetTextExtent(strText,_tcslen(strText));
			CRect rectText = lpDrawItemStruct->rcItem;
			
			int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
			memDc.SetTextColor(m_textColor);
			memDc.DrawText(strText,&rectText,DT_CENTER | DT_SINGLELINE | DT_VCENTER);
			memDc.SelectObject(pOldFont);
			memDc.SetBkMode(nOldBkMode);
		}
		 
		borderPen.DeleteObject();
		bkBrush.DeleteObject();

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(pOldBitmapCB);
		bitMapCB.DeleteObject();
		memDc.DeleteDC();
	}
	
	cDC.Detach();

#ifdef _DEBUG
	clock_t endMs = clock() - startMs;
	++endMs;
#endif
}

void CMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CButton::OnMouseMove(nFlags, point);
}

LRESULT CMyButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_BUTTON_NOFOCUS != m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_NOFOCUS;
		Invalidate();
	}
	m_bTracking = FALSE;
	return 0;
}

LRESULT CMyButton::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_BUTTON_NOFOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_FOCUS;
		Invalidate();
	}
	m_bTracking = FALSE;
	return 0;
}

void CMyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(CTRL_BUTTON_FOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_SELECTED;
		Invalidate();
	}
	__super::OnLButtonDown(nFlags, point);
}

void CMyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CTRL_BUTTON_SELECTED == m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_FOCUS;
		Invalidate();
	}
	__super::OnLButtonUp(nFlags, point);
}

void CMyButton::OnSize(UINT nType, int cx, int cy)
{
 	__super::OnSize(nType, cx, cy);
}

BOOL CMyButton::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		//case VK_RETURN:	return TRUE;
		case VK_ESCAPE:	return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

BOOL CMyButton::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyButton::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)      
	{      
	case TIMER_ALARMTWINKLE:       
		m_bTwinkleShow = !m_bTwinkleShow;
		Invalidate(FALSE);
		break;      
	default:   
		break;     

	} 
	CButton::OnTimer(nIDEvent);
}

void CMyButton::StartTwinkle()
{
	SetTimer(TIMER_ALARMTWINKLE,500,NULL);
	m_bTwinkle = TRUE;
}

void CMyButton::StopTwinkle()
{
	KillTimer(TIMER_ALARMTWINKLE);
	m_bTwinkle = FALSE;
	Invalidate(FALSE);
}

void CMyButton::SetIconBtInfo(CIconBtInfo info,BOOL bRedraw /*= FALSE*/)
{
	m_iConBtInfo = info;
	m_bIconBt = TRUE;
	if(bRedraw && m_bIconBt && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyButton::SetIconBtPath(CString strPath,BOOL bRedraw /*= FALSE*/)
{
	m_iConBtInfo.m_strIconPath = strPath;
	if(bRedraw && m_bIconBt && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

CIconBtInfo CMyButton::GetIconBtInfo()
{
	return m_iConBtInfo;
}

void CMyButton::SetIconBtTxRectEndX(int iEndX,BOOL bRedraw /*= FALSE*/)
{
	m_iConBtInfo.m_rcText.right = iEndX;
	if(bRedraw && m_bIconBt && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyButton::SetIconBtIconRect(Rect rc,BOOL bRedraw /*= FALSE*/)
{
	m_iConBtInfo.m_rcIcon = rc;
	if(bRedraw && m_bIconBt && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyButton::SetBeHaveTxPushedFormat(BOOL bHaveTxPushedFormat)
{
	m_bHaveTxPushedFormat = bHaveTxPushedFormat;
}

BOOL CMyButton::GetBeHaveTxPushedFormat()
{
	return m_bHaveTxPushedFormat;
}


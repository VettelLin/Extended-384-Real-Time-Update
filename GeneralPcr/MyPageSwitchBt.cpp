// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyPageSwitchBt.h"
// CMyPageSwitchBt


int CMyPageSwitchBt::PageCount = 0;

IMPLEMENT_DYNAMIC(CMyPageSwitchBt, CButton)

CMyPageSwitchBt::CMyPageSwitchBt(BOOL bAutoDynamic /*= FALSE*/,BOOL bUseFourthState /*= TRUE*/)
{
	m_bTracking = FALSE;
	m_ctrlState = CTRL_BUTTON_NOFOCUS;
	m_bHasBkPic = FALSE;
	m_bHasPathBkPic = FALSE; 
	m_bHasPngBkPic = FALSE;
	m_sonPicNums = 1;
	m_iconW = 0;
	m_iconLeftIndent = 0;
	m_iconRightIndent = 0;
	m_rightEnd = 0;

	m_txClrNormal = RGB(0X39,0X45,0X65);
	m_txClrFocused = CLR_2LEVEL_THEME;
	m_txClrClicked = CLR_2LEVEL_THEME;
	m_txClrDisabled = CLR_2LEVEL_THEME;

	m_edgeClrNormal = RGB(0XDC,0XDC,0XDC);
	m_edgeClrFocused = RGB(0XDC,0XDC,0XDC);
	m_edgeClrClicked = RGB(0XDC,0XDC,0XDC);
	m_edgeClrDisabled = RGB(0XDC,0XDC,0XDC);

	m_bkClrNormal = RGB(0XFF,0XFF,0XFF);
	m_bkClrFocused = RGB(0XFF,0XFF,0XFF);
	m_bkClrClicked = CLR_SOFT_LIGHT;
	m_bkClrDisabled = CLR_SOFT_LIGHT;

	m_bDrawFrame = TRUE;
	m_strPngPicPath = _T("");

	m_bInMiddle = FALSE;

	m_bGradientBk = FALSE;
	m_glrStart = Color(0,0X65,0X3D);
	m_glrEnd = Color(0,0X9E,0X70);
	m_gradientMode = LinearGradientModeHorizontal;

	m_bFstPaint = TRUE;
	m_pMemDc = NULL;
	m_bAutoDynamic = bAutoDynamic;
	m_fstPaintWidth = 0;
	m_fstPaintHeight = 0;
	m_hBitmap = NULL;

	m_txFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_bAmoySwtStyle = FALSE;
	m_AmoySwtBkClr = RGB(251,251,251);
	m_bUseFourthState = bUseFourthState;

	m_bCkbtFunc = FALSE;
	m_curStateId = 0;
	m_pageId = ++PageCount;

	m_bAddPageIcon = FALSE;
	m_strFilePath = _T("");
	m_bBtPageRunning = FALSE;

	m_iCloseIconW = 10;
	m_iCloseIconH = 10;
	m_iCloseIconRightIndent = 11;

	m_bMouseInColseBtRect = FALSE;
}

CMyPageSwitchBt::~CMyPageSwitchBt()
{
	m_bTracking = FALSE;

	MyDeletePtr(&m_pMemDc);

	if(NULL != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
}

BEGIN_MESSAGE_MAP(CMyPageSwitchBt, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,&CMyPageSwitchBt::OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER,&CMyPageSwitchBt::OnMouseHover)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_CAPTURECHANGED()
END_MESSAGE_MAP()

// CMyPageSwitchBt message handlers
void CMyPageSwitchBt::PreSubclassWindow()
{
	ModifyStyle(0,BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CMyPageSwitchBt::SetPngBkPic(CString strPicPath,int sonPicNums,int iconWidth /*= 0*/,int iconLeftIndent /*= 0*/,int iconRightIndent /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_sonPicNums = sonPicNums;
	m_bHasPngBkPic = TRUE;
	m_strPngPicPath = strPicPath;
	m_iconW = iconWidth;
	m_iconLeftIndent = iconLeftIndent;
	m_iconRightIndent = iconRightIndent;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyPageSwitchBt::SetBkColor(COLORREF bkClrNormal,COLORREF bkClrFocused,COLORREF bkClrClicked,COLORREF bkClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyPageSwitchBt::GetBkColor(COLORREF& bkClrNormal,COLORREF& bkClrFocused,COLORREF& bkClrClicked,COLORREF& bkClrDisabled)
{
	bkClrNormal = m_bkClrNormal;
	bkClrFocused = m_bkClrFocused;
	bkClrClicked = m_bkClrClicked;
	bkClrDisabled = m_bkClrDisabled;
	return;
}

void CMyPageSwitchBt::SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyPageSwitchBt::GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled)
{
	txClrNormal = m_txClrNormal;
	txClrFocused = m_txClrFocused;
	txClrClicked = m_txClrClicked;
	txClrDisabled = m_txClrDisabled;
	return;
}

void CMyPageSwitchBt::SetEdgeColor(COLORREF edgeClrNormal,COLORREF edgeClrFocused,COLORREF edgeClrClicked,COLORREF edgeClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyPageSwitchBt::GetEdgeColor(COLORREF& edgeClrNormal,COLORREF& edgeClrFocused,COLORREF& edgeClrClicked,COLORREF& edgeClrDisabled)
{
	edgeClrNormal = m_edgeClrNormal;
	edgeClrFocused = m_edgeClrFocused;
	edgeClrClicked = m_edgeClrClicked;
	edgeClrDisabled = m_edgeClrDisabled;
}

void CMyPageSwitchBt::SetDrawFrame(BOOL bDrawFrame,BOOL bRedraw /*= FALSE*/)
{
	m_bDrawFrame = bDrawFrame;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyPageSwitchBt::GetDrawFrame()
{
	return m_bDrawFrame;
}

void CMyPageSwitchBt::SetBeInMiddle(BOOL bInMiddle,BOOL bRedraw /*= FALSE*/)
{
	m_bInMiddle = bInMiddle;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyPageSwitchBt::GetBeInMiddle()
{
	return m_bInMiddle;
}

void CMyPageSwitchBt::SetTextFormat(UINT txFormat,BOOL bRedraw /*= FALSE*/)
{
	m_txFormat = txFormat;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

UINT CMyPageSwitchBt::GetTextFormat()
{
	return m_txFormat;
}

void CMyPageSwitchBt::SetAmoySwtParam(BOOL bAmoySwtStyle,COLORREF bkClr,BOOL bRedraw /*= FALSE*/)
{
	m_bAmoySwtStyle = bAmoySwtStyle;
	m_AmoySwtBkClr = bkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyPageSwitchBt::GetAmoySwtParam(BOOL& bAmoySwtStyle,COLORREF& bkClr)
{
	bAmoySwtStyle = m_bAmoySwtStyle;
	bkClr = m_AmoySwtBkClr;
	return;
}

void CMyPageSwitchBt::SetBeAmoySwtStyle(BOOL bAmoySwtStyle,BOOL bRedraw /*= FALSE*/)
{
	m_bAmoySwtStyle = bAmoySwtStyle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyPageSwitchBt::GetBeAmoySwtStyle()
{
	return m_bAmoySwtStyle;
}

void CMyPageSwitchBt::SetAmoySwtBkClr(COLORREF bkClr,BOOL bRedraw /*= FALSE*/)
{
	m_AmoySwtBkClr = bkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMyPageSwitchBt::GetAmoySwtBkClr()
{
	return m_AmoySwtBkClr;
}

void CMyPageSwitchBt::SetGradientParam(BOOL bGradientBk,Color glrStart,Color glrEnd,LinearGradientMode gradientMode,BOOL bRedraw /*= FALSE*/)
{
	m_bGradientBk = bGradientBk;
	m_glrStart = glrStart;
	m_glrEnd = glrEnd;
	m_gradientMode = gradientMode;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyPageSwitchBt::GetGradientParam(BOOL& bGradientBk,Color& glrStart,Color& glrEnd,LinearGradientMode& gradientMode)
{
	 bGradientBk = m_bGradientBk;
	 glrStart = m_glrStart;
	 glrEnd = m_glrEnd;
	 gradientMode = m_gradientMode;
}

void CMyPageSwitchBt::SetCkbtFuncParam(BOOL bCkbtFunc,int curState,BOOL bRedraw /*= FALSE*/)
{
	m_bCkbtFunc = bCkbtFunc;
	m_curStateId = curState;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyPageSwitchBt::GetCkbtFuncParam(BOOL& bCkbtFunc,int& curState)
{
	bCkbtFunc = m_bCkbtFunc;
	curState = m_curStateId;
	return;
}

void CMyPageSwitchBt::SetBeCkbtFunc(BOOL bCkbtFunc,BOOL bRedraw /*= FALSE*/)
{
	m_bCkbtFunc = bCkbtFunc;
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyPageSwitchBt::GetBeCkbtFunc()
{
	return m_bCkbtFunc;
}

void CMyPageSwitchBt::SetCkbtFuncCurState(int curState,BOOL bRedraw /*= FALSE*/)
{
	m_curStateId = curState;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyPageSwitchBt::GetCkbtFuncCurState()
{
	return m_curStateId;
}

void CMyPageSwitchBt::SetBeAddPageIcon(BOOL bAddPageIcon,BOOL bRedraw /*= FALSE*/)
{
	m_bAddPageIcon = bAddPageIcon;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyPageSwitchBt::GetBeAddPageIcon()
{
	return m_bAddPageIcon;
}

void CMyPageSwitchBt::SetBeBtPageRunning(BOOL bBtPageRunning,BOOL bRedraw /*= FALSE*/)
{
	m_bBtPageRunning = bBtPageRunning;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyPageSwitchBt::GetBeBtPageRunning()
{
	return m_bBtPageRunning;
}

void CMyPageSwitchBt::SetPageId(int pageId)
{
	m_pageId = pageId;
}

int CMyPageSwitchBt::GetPageId()
{
	return m_pageId;
}

void CMyPageSwitchBt::EnableWindow(BOOL bEnabled)
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
	Invalidate(FALSE);
	//CButton::EnableWindow(bEnabled);
}

void CMyPageSwitchBt::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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

	if(m_bAutoDynamic)
	{
		if(m_bFstPaint)
		{
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
			memDCTemp.StretchBlt(0,0,rect.Width(),rect.Height(),m_pMemDc,0,0,m_fstPaintWidth,m_fstPaintHeight,SRCCOPY);
		}
	}


	{
		Graphics graphics(memDCTemp.m_hDC);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);
		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);

		COLORREF clrBk = RGB(0XFF,0XFF,0XFF);
		Color clrCloseBt = Color(0X8B,0X8E,0X93);

		if(m_bBtPageRunning && CTRL_BUTTON_DISABLE != m_ctrlState)
		{
			if(m_bMouseInColseBtRect)
			{
				clrCloseBt = Color(0X20,0X44,0X7C);
			}
			else
			{
				clrCloseBt = COLOR_2LEVEL_THEME;
			}
		}
		else
		{
			if(m_bMouseInColseBtRect)
			{
				clrCloseBt = Color(0X20,0X44,0X7C);
			}
			else
			{
				clrCloseBt = Color(0X8B,0X8E,0X93);
			}
		}

		//绘制背景
		if(CTRL_BUTTON_DISABLE == m_ctrlState)
		{
			clrBk = m_bkClrDisabled;
		}
		else
		{
			if(!m_bBtPageRunning)
			{
				clrBk = m_bkClrNormal;
			}
		}
		memDCTemp.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),clrBk);

		if(m_bBtPageRunning && CTRL_BUTTON_DISABLE != m_ctrlState)
		{
			memDCTemp.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),RGB(0XFF,0XFF,0XFF));
			int iTempInt1 = 100 * m_dHoriRatio;
			if(rect.Width() > iTempInt1)
			{
				LinearGradientBrush linGrBrush(RectF(0,0,iTempInt1,rect.Height()),COLOR_PAGEBT_RUNNING_UNSHOW_START,COLOR_PAGEBT_RUNNING_UNSHOW_MIDDLE,LinearGradientModeHorizontal);
				Status sRet = graphics.FillRectangle(&linGrBrush,0,0,iTempInt1,rect.Height());

				LinearGradientBrush linGrBrush2(RectF(iTempInt1,0,rect.Width() - iTempInt1,rect.Height()),COLOR_PAGEBT_RUNNING_UNSHOW_MIDDLE,COLOR_PAGEBT_RUNNING_UNSHOW_END,LinearGradientModeHorizontal);
				sRet = graphics.FillRectangle(&linGrBrush2,iTempInt1,0,rect.Width() - iTempInt1,rect.Height());
			}
			else
			{
				LinearGradientBrush linGrBrush(RectF(0,0,rect.Width(),rect.Height()),Color(139,0X06,0X91,0X9D),Color(0,0X06,0X91,0X9D),LinearGradientModeHorizontal);
				Status sRet = graphics.FillRectangle(&linGrBrush,0,0,rect.Width() - 6,rect.Height());
			}
		}

		//绘制实验图标
		Image image(m_strPngPicPath);
		int iStartX = 0;
		int iBkW = 0;
		if(1 == m_sonPicNums)
		{
			iBkW = image.GetWidth();
		}
		else if(2 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				iStartX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				iStartX = 0;
				break;
			case CTRL_BUTTON_SELECTED:
				iStartX = image.GetWidth() / 2;
				break;
			case CTRL_BUTTON_DISABLE:
				iStartX = image.GetWidth() / 2;
				break;
			}
			iBkW = image.GetWidth() / 2;
		}
		else if(3 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				iStartX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				iStartX = 0;
				break;
			case CTRL_BUTTON_SELECTED:
				iStartX = image.GetWidth() / 3;
				break;
			case CTRL_BUTTON_DISABLE:
				iStartX = image.GetWidth() / 3;
				break;
			}
			iBkW = image.GetWidth() / 3;
		}
		else if(4 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_BUTTON_NOFOCUS:
				iStartX = 0;
				break;
			case CTRL_BUTTON_FOCUS:
				iStartX = image.GetWidth() / 4;
				break;
			case CTRL_BUTTON_SELECTED:
				iStartX = image.GetWidth() / 2;
				break;
			case CTRL_BUTTON_DISABLE:
				iStartX = image.GetWidth() / 4 * 3;
				break;
			}
			iBkW = image.GetWidth() / 4;
		}

		ImageAttributes ImgAtt;
		ImgAtt.SetWrapMode(WrapModeTileFlipXY);

		Rect destRect;
		if(0 != m_iconW)
		{
			destRect.X = m_iconLeftIndent;
			if(rect.Height() > image.GetHeight())
			{
				destRect.Y = (rect.Height() - image.GetHeight()) / 2;
			}

			if(iBkW > rect.Width() - m_iconLeftIndent)
			{
				destRect.Width = rect.Width() - m_iconLeftIndent;
			}
			else
			{
				destRect.Width = iBkW;
			}
			destRect.Height = rect.Height() - destRect.Y * 2;
		}
		else
		{
			if(m_bInMiddle)
			{
				if(rect.Width() > iBkW)
				{
					destRect.X = (rect.Width() - iBkW) / 2;
				}

				if(rect.Height() > image.GetHeight())
				{
					destRect.Y = (rect.Height() - image.GetHeight()) / 2;
				}
				destRect.Width = iBkW;
				destRect.Height = image.GetHeight();
			}
			else
			{
				destRect.X = 0;
				destRect.Y = 0;
				destRect.Width = rect.Width();
				destRect.Height = rect.Height();
			}
		}

		graphics.DrawImage(&image,destRect,0,0,iBkW,image.GetHeight(),UnitPixel,&ImgAtt);

		//绘制“x”标识
		m_rcCloseBt = CRect(rcClient.Width() - m_iCloseIconRightIndent - m_iCloseIconW,(rcClient.Height() - m_iCloseIconH) / 2,rcClient.Width() - m_iCloseIconRightIndent,rcClient.Height() / 2 + m_iCloseIconH / 2);
		Pen gdiplusPen(clrCloseBt,2 * m_dTxRatio);
		Point ptStart,ptEnd;
		ptStart.X = m_rcCloseBt.left;
		ptStart.Y = m_rcCloseBt.top;
		ptEnd.X = m_rcCloseBt.right;
		ptEnd.Y = m_rcCloseBt.bottom;
		graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);
		ptStart.X = m_rcCloseBt.right;
		ptStart.Y = m_rcCloseBt.top;
		ptEnd.X = m_rcCloseBt.left;
		ptEnd.Y = m_rcCloseBt.bottom;
		graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);

		graphics.ReleaseHDC(memDCTemp.m_hDC);
	}
	
	//如果设置了文本，则绘制文本
	CString strText;
	GetWindowText(strText);
	if(0 != strText.GetLength())
	{
		COLORREF clrTx = 0;

		switch(m_ctrlState)
		{
		case CTRL_BUTTON_NOFOCUS:
			clrTx = m_txClrNormal;
			break;
		case CTRL_BUTTON_FOCUS:
			clrTx = m_txClrFocused; 
			break;
		case CTRL_BUTTON_SELECTED:
			clrTx = m_txClrClicked;
			break;
		case CTRL_BUTTON_DISABLE:
			clrTx = m_txClrDisabled;
			break;
		}

		CFont *pOldFont = memDCTemp.SelectObject(&m_fontSet);
		CRect rectText = lpDrawItemStruct->rcItem;

		int nOldBkMode = memDCTemp.SetBkMode(TRANSPARENT);
		memDCTemp.SetTextColor(clrTx);
		rectText.left = (m_iconLeftIndent + m_iconW + m_iconRightIndent);

		rectText.right -= (m_iCloseIconW + m_iCloseIconRightIndent);
		m_txFormat |= DT_END_ELLIPSIS;
		memDCTemp.DrawText(strText,&rectText,m_txFormat);
		memDCTemp.SelectObject(pOldFont);
		memDCTemp.SetBkMode(nOldBkMode);
	}

	cDC.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDCTemp,0,0,SRCCOPY);
	memDCTemp.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDCTemp.DeleteDC();

	cDC.Detach();

#ifdef _DEBUG
	clock_t endMs = clock() - startMs;
	++endMs;
#endif
}

void CMyPageSwitchBt::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bPreState = m_bMouseInColseBtRect;
	if(m_rcCloseBt.PtInRect(point))
	{
		m_bMouseInColseBtRect = TRUE;
		SetCapture();
	}
	else
	{
		m_bMouseInColseBtRect = FALSE;
		ReleaseCapture();
	}
	if(bPreState != m_bMouseInColseBtRect)
	{
		Invalidate(FALSE);
	}

	if(CTRL_BUTTON_DISABLE == m_ctrlState)
	{
		return;
	}

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

LRESULT CMyPageSwitchBt::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_BUTTON_DISABLE == m_ctrlState)
	{
		return 1;
	}

	if(CTRL_BUTTON_NOFOCUS != m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_NOFOCUS;
		Invalidate(FALSE);
	}
	m_bTracking = FALSE;

	return 0;
}

LRESULT CMyPageSwitchBt::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_BUTTON_DISABLE == m_ctrlState)
	{
		return 1;
	}

	if(CTRL_BUTTON_NOFOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_FOCUS;
		Invalidate(FALSE);
	}
	m_bTracking = FALSE;
	return 0;
}

void CMyPageSwitchBt::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_rcCloseBt.PtInRect(point) && !m_bBtPageRunning)
	{
		CWnd* pParent = GetParent();
		if(NULL != pParent->GetSafeHwnd())
		{
			pParent->PostMessage(WM_CLOSEPAGE,m_pageId,0);
		}
		return;
	}
	

	if(CTRL_BUTTON_DISABLE == m_ctrlState)
	{
		return;
	}
	
	if(CTRL_BUTTON_FOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_SELECTED;
		Invalidate(FALSE);
	}
	__super::OnLButtonDown(nFlags, point);
}

void CMyPageSwitchBt::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CTRL_BUTTON_DISABLE == m_ctrlState)
	{
		return;
	}

	if(CTRL_BUTTON_SELECTED == m_ctrlState)
	{
		m_ctrlState = CTRL_BUTTON_FOCUS;
		Invalidate();
	}

	__super::OnLButtonUp(nFlags, point);
}

void CMyPageSwitchBt::OnSize(UINT nType, int cx, int cy)
{
 	__super::OnSize(nType, cx, cy);
}

BOOL CMyPageSwitchBt::PreTranslateMessage(MSG* pMsg)
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

BOOL CMyPageSwitchBt::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

int CMyPageSwitchBt::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iCloseIconW *= m_dHoriRatio;
	m_iCloseIconH *= m_dVertRatio;
	m_iCloseIconRightIndent *= m_dHoriRatio;

	return CButton::OnCreate(lpCreateStruct);
}

void CMyPageSwitchBt::OnCaptureChanged(CWnd* pNewWnd)
{
	CWnd* pTemp = this;
	if(pNewWnd != pTemp)
	{
		Invalidate(FALSE);
	}
	return;
}

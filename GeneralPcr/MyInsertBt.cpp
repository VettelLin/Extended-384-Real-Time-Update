// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyInsertBt.h"
// CMyInsertBt


IMPLEMENT_DYNAMIC(CMyInsertBt, CButton)

CMyInsertBt::CMyInsertBt()
{
	m_bTracking = FALSE;
	m_ctrlState = CTRL_INSERTBT_NOFOCUS;
	m_bHasBkPic = FALSE;
	m_bHasPathBkPic = FALSE; 
	m_bHasPngBkPic = FALSE;
	m_sonPicNums = 1;
	m_iconW = 0;
	m_leftIndent = 0;
	m_rightEnd = 0;

	m_txClrNormal = RGB(0XFF,0XFF,0XFF);
	m_txClrFocused = RGB(0XFF,0XFF,0XFF);
	m_txClrClicked = RGB(0XFF,0XFF,0XFF);
	m_txClrDisabled = CLR_INSERTBT_TX_DISABLED;

	m_edgeClrNormal = Color(0XD8,0XD8,0XD8);
	m_edgeClrFocused = Color(0XD8,0XD8,0XD8);
	m_edgeClrClicked = Color(0XD8,0XD8,0XD8);
	m_edgeClrDisabled = Color(0XD8,0XD8,0XD8);

	m_bkClrNormal = COLOR_BTBK_NORMAL;
	m_bkClrFocused = COLOR_BTBK_HOVER;
	m_bkClrClicked = COLOR_BTBK_CLICKED;
	m_bkClrDisabled = COLOR_BTBK_DISABLED;

	m_clrArrowNormal = Color(0XFF,0XFF,0XFF);
	m_clrArrowFocused = Color(0XFF,0XFF,0XFF);
	m_clrArrowClicked = Color(0XFF,0XFF,0XFF);
	m_clrArrowDisabled = Color(0XC6,0XC4,0XC4);

	m_clrSplitLineNormal = Color(0XFF,0XFF,0XFF);
	m_clrSplitLineFocused = Color(0XFF,0XFF,0XFF);
	m_clrSplitLineClicked = Color(0XFF,0XFF,0XFF);
	m_clrSplitLineDisabled = Color(0XC6,0XC4,0XC4);

	m_bDrawFrame = TRUE;
	m_pngPicPath = _T("");

	m_bRoundRectBk = TRUE;
	m_clrRoundRectBk = Color(0XFF,0XFF,0XFF);
	m_bInMiddle = FALSE;
	m_bCloseBt = FALSE;

	m_bTwinkle = FALSE;
	m_bTwinkleShow = TRUE;
	m_curState = 1;
	m_curState2 = 1;

	m_bTextDown = FALSE;
	m_iRoundAngle = 4;

	m_iSplitLineXPos = 69;
}

CMyInsertBt::~CMyInsertBt()
{
	m_bTracking = FALSE;
}

BEGIN_MESSAGE_MAP(CMyInsertBt, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,&CMyInsertBt::OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER,&CMyInsertBt::OnMouseHover)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CMyInsertBt message handlers
void CMyInsertBt::PreSubclassWindow()
{
	ModifyStyle(0,BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CMyInsertBt::SetBmpBkPic(UINT idPic,int sonPicNums /*= 2*/,int btIconW /*= 0*/,int leftStart /*= 0*/,int rightEnd /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_idNormal = idPic;
	m_sonPicNums = sonPicNums;
	m_iconW = btIconW;
	m_leftIndent = leftStart;
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

void CMyInsertBt::SetPngBkPic(wstring picPath,int sonPicNums,int iconWidth /*= 0*/,int leftIndent/* = 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_sonPicNums = sonPicNums;
	m_bHasPngBkPic = TRUE;
	m_pngPicPath = picPath;
	m_iconW = iconWidth;
	m_leftIndent = leftIndent;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
}

void CMyInsertBt::SetNewBmpBkPic(UINT idPic,int sonPicNums,BOOL bInvalidate)
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

void CMyInsertBt::SetBmpBkPic(UINT idNormal,UINT idFocus,UINT idSelected,UINT idDisabled,int btIconW /*= 0*/,int leftStart /*= 0*/,int rightEnd /*= 0*/)
{
	m_idNormal = idNormal;
	m_idFocus = idFocus;
	m_idSelected = idSelected;
	m_idDisabled = idDisabled;
	m_sonPicNums = 1;
	m_iconW = btIconW;
	m_leftIndent = leftStart;
	m_rightEnd = rightEnd;
	m_bHasBkPic = TRUE;
}

void CMyInsertBt::SetBkColor(Color bkClrNormal,Color bkClrFocused,Color bkClrClicked,Color bkClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyInsertBt::GetBkColor(Color& bkClrNormal,Color& bkClrFocused,Color& bkClrClicked,Color& bkClrDisabled)
{
	bkClrNormal = m_bkClrNormal;
	bkClrFocused = m_bkClrFocused;
	bkClrClicked = m_bkClrClicked;
	bkClrDisabled = m_bkClrDisabled;
	return;
}

void CMyInsertBt::SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyInsertBt::GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled)
{
	txClrNormal = m_txClrNormal;
	txClrFocused = m_txClrFocused;
	txClrClicked = m_txClrClicked;
	txClrDisabled = m_txClrDisabled;
	return;
}

void CMyInsertBt::SetEdgeColor(Color edgeClrNormal,Color edgeClrFocused,Color edgeClrClicked,Color edgeClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyInsertBt::GetEdgeColor(Color& edgeClrNormal,Color& edgeClrFocused,Color& edgeClrClicked,Color& edgeClrDisabled)
{
	edgeClrNormal = m_edgeClrNormal;
	edgeClrFocused = m_edgeClrFocused;
	edgeClrClicked = m_edgeClrClicked;
	edgeClrDisabled = m_edgeClrDisabled;
}

void CMyInsertBt::SetRoundRectParam(BOOL bRoundRect,Color clrRoundRectBk,BOOL bRedraw /*= FALSE*/)
{
	m_bRoundRectBk = bRoundRect;
	m_clrRoundRectBk = clrRoundRectBk;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyInsertBt::GetRoundRectParam(BOOL& bRoundRect,Color& clrRoundRectBk)
{
	bRoundRect = m_bRoundRectBk;
	clrRoundRectBk = m_clrRoundRectBk;
}

void CMyInsertBt::SetDrawFrame(BOOL bDrawFrame,BOOL bRedraw /*= FALSE*/)
{
	m_bDrawFrame = bDrawFrame;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyInsertBt::GetDrawFrame()
{
	return m_bDrawFrame;
}

void CMyInsertBt::SetBeInMiddle(BOOL bInMiddle,BOOL bRedraw /*= FALSE*/)
{
	m_bInMiddle = bInMiddle;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyInsertBt::GetBeInMiddle()
{
	return m_bInMiddle;
}

void CMyInsertBt::SetBeCloseBt(BOOL bCloseBt,BOOL bRedraw /*= FALSE*/)
{
	m_bCloseBt = bCloseBt;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyInsertBt::GetBeCloseBt()
{
	return m_bCloseBt;
}

void CMyInsertBt::SetBeTextDown(BOOL bTextDown,BOOL bRedraw /*= FALSE*/)
{
	m_bTextDown = bTextDown;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyInsertBt::GetBeTextDown()
{
	return m_bTextDown;
}

void CMyInsertBt::SetCurState(int curState,int curState2,BOOL bRedraw /*= FALSE*/)
{
	m_curState = curState;
	m_curState2 = curState2;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

int CMyInsertBt::GetCurState()
{
	return m_curState;
}

int CMyInsertBt::GetCurState2()
{
	return m_curState2;
}

BOOL CMyInsertBt::GetBeTwinkle()
{
	return m_bTwinkle;
}

void CMyInsertBt::EnableWindow(BOOL bEnabled)
{
	if(bEnabled)
	{
		m_ctrlState = CTRL_INSERTBT_NOFOCUS;
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
	
		m_ctrlState = CTRL_INSERTBT_DISABLE;
	}
	CButton::EnableWindow(bEnabled);
}

void CMyInsertBt::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	CRect rect = lpDrawItemStruct->rcItem;
	CDC cDC;
	cDC.Attach(lpDrawItemStruct->hDC);
	ASSERT(NULL != cDC.GetSafeHdc());

	if(m_bHasPngBkPic)
	{
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //创建兼容位图
		HGDIOBJ hOldGdiObject = memDc.SelectObject(hBmp);

		memDc.BitBlt(0,0,rect.Width(),rect.Height(),&cDC,0,0,SRCCOPY);

		if(m_bRoundRectBk)
		{
			CRect rcTemp(rect);
			rcTemp.DeflateRect(1,1);
			DrawRoundRectangle(&memDc,rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),10,m_clrRoundRectBk,1,TRUE,m_clrRoundRectBk);
		}

		Graphics graphics(memDc.m_hDC);
		Image image(m_pngPicPath.c_str());

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
			case CTRL_INSERTBT_NOFOCUS:
				startX = 0;
				break;
			case CTRL_INSERTBT_FOCUS:
				startX = 0;
				break;
			case CTRL_INSERTBT_SELECTED:
				startX = image.GetWidth() / 2;
				break;
			case CTRL_INSERTBT_DISABLE:
				startX = image.GetWidth() / 2;
				break;
			}
			bkW = image.GetWidth() / 2;
		}
		else if(3 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_INSERTBT_NOFOCUS:
				startX = 0;
				break;
			case CTRL_INSERTBT_FOCUS:
				startX = image.GetWidth() / 3;
				break;
			case CTRL_INSERTBT_SELECTED:
				startX = image.GetWidth() / 3 * 2;
				break;
			case CTRL_INSERTBT_DISABLE:
				startX = image.GetWidth() / 3 * 2;
				break;
			}
			bkW = image.GetWidth() / 3;
		}
		else if(4 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_INSERTBT_NOFOCUS:
				startX = 0;
				break;
			case CTRL_INSERTBT_FOCUS:
				startX = image.GetWidth() / 4;
				break;
			case CTRL_INSERTBT_SELECTED:
				startX = image.GetWidth() / 2;
				break;
			case CTRL_INSERTBT_DISABLE:
				startX = image.GetWidth() / 4 * 3;
				break;
			}
			bkW = image.GetWidth() / 4;
		}
		else if(5 == m_sonPicNums)
		{
			switch(m_ctrlState)
			{
			case CTRL_INSERTBT_NOFOCUS:
				startX = 0;
				break;
			case CTRL_INSERTBT_FOCUS:
				startX = image.GetWidth() / 5;
				break;
			case CTRL_INSERTBT_SELECTED:
				startX = image.GetWidth() / 5 * 2;
				break;
			case CTRL_INSERTBT_DISABLE:
				startX = image.GetWidth() / 5 * 3;
				break;
			}
			startX = 0;	//只使用第一种状态
			bkW = image.GetWidth() / 5;
		}

		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
		ImageAttributes ImgAtt;
		ImgAtt.SetWrapMode(WrapModeTileFlipXY);

		Rect destRect;
		if(0 != m_iconW)
		{
			memDc.FillSolidRect(&rect,RGB(255,255,255));

			destRect.X = m_leftIndent;
			if(rect.Height() > image.GetHeight())
			{
				destRect.Y = (rect.Height() - image.GetHeight()) / 2;
			}

			if(bkW > rect.Width() - m_leftIndent)
			{
				destRect.Width = rect.Width() - m_leftIndent;
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
				destRect.Height = rect.Height();
			}
		}

		graphics.DrawImage(&image,destRect,startX,0,bkW,image.GetHeight(),UnitPixel,&ImgAtt);

		//绘制三角形
		SolidBrush brush(Color(128,128,128));

		Point pt1(rect.right - 22,rect.top + (rect.Height() - 8) / 2);
		Point pt2(rect.right - 12,rect.top + (rect.Height() - 8) / 2);
		Point pt3(rect.right - 17,rect.top + (rect.Height() - 8) / 2 + 8);

		Point crvPts[3] = {pt1,pt2,pt3};
		graphics.FillPolygon(&brush,crvPts,3,FillModeAlternate);

		Pen gdiplusPen(Color(164,164,164),2);
		Point ptStart,ptEnd;
		ptStart.X = rect.right - RIFHTWIDTH;
		ptStart.Y = rect.top + 5;
		ptEnd.X = rect.right - RIFHTWIDTH;
		ptEnd.Y = rect.bottom - 5;
		graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);
		
		graphics.ReleaseHDC(memDc.m_hDC);

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			COLORREF clrText = 0;
			switch(m_ctrlState)
			{
			case CTRL_INSERTBT_NOFOCUS:
				clrText = m_txClrNormal;
				break;
			case CTRL_INSERTBT_FOCUS:
				clrText = m_txClrFocused; 
				break;
			case CTRL_INSERTBT_SELECTED:
				clrText = m_txClrClicked;
				break;
			case CTRL_INSERTBT_DISABLE:
				clrText = m_txClrDisabled;
				break;
			}

			CFont *pOldFont = memDc.SelectObject(&m_fontSet);
			CRect rectText = lpDrawItemStruct->rcItem;

			UINT state = lpDrawItemStruct->itemState;
			if(state & ODS_SELECTED)
			{
				rectText.OffsetRect(1,1);
			}

			//右侧显示三角形
			rectText.right -= RIFHTWIDTH;

			int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
			memDc.SetTextColor(clrText);
			rectText.left += (m_iconW + m_leftIndent);
			if(m_bTextDown)
			{
				rectText.top = rectText.bottom - 18;
				memDc.DrawText(strText,&rectText,DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS);
			}
			else
			{
				memDc.DrawText(strText,&rectText,DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS);
			}
			
			memDc.SelectObject(pOldFont);
			memDc.SetBkMode(nOldBkMode);
		}

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(hOldGdiObject);
		::DeleteObject(hBmp);
		memDc.DeleteDC();
	}
	else
	{
		CDC memDc;
		memDc.CreateCompatibleDC(&cDC);
		HBITMAP hBmp = ::CreateCompatibleBitmap(cDC.m_hDC,rect.Width(),rect.Height()); //创建兼容位图
		HGDIOBJ hOldGdiObject = memDc.SelectObject(hBmp);

		memDc.FillSolidRect(&rect,RGB(m_clrRoundRectBk.GetR(),m_clrRoundRectBk.GetG(),m_clrRoundRectBk.GetB()));

		BOOL bShowFrame = FALSE;
		Color clrFrame = 0;
		Color clrFill = 0;
		Color clrArrow = 0;
		Color clrSplitLine = 0;
		COLORREF clrText = 0;
		switch(m_ctrlState)
		{
		case CTRL_INSERTBT_NOFOCUS:
			clrFrame = m_edgeClrNormal;
			clrFill = m_bkClrNormal;
			clrArrow = m_clrArrowNormal;
			clrSplitLine = m_clrSplitLineNormal;
			clrText = m_txClrNormal;
			break;
		case CTRL_INSERTBT_FOCUS:
			clrFrame = m_edgeClrFocused;
			clrFill = m_bkClrFocused;
			clrArrow = m_clrArrowFocused;
			clrSplitLine = m_clrSplitLineFocused;
			clrText = m_txClrFocused;
			break;
		case CTRL_INSERTBT_SELECTED:
			clrFrame = m_edgeClrClicked;
			clrFill = m_bkClrClicked;
			clrArrow = m_clrArrowClicked;
			clrSplitLine = m_clrSplitLineClicked;
			clrText = m_txClrClicked;
			break;
		case CTRL_INSERTBT_DISABLE:
			clrFrame = m_edgeClrDisabled;
			clrFill = m_bkClrDisabled;
			clrArrow = m_clrArrowDisabled;
			clrSplitLine = m_clrSplitLineDisabled;
			clrText = m_txClrDisabled;
			bShowFrame = TRUE;
			break;
		default:	break;
		}

		if(m_bRoundRectBk)
		{
			DrawRoundRectangleNew(&memDc,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,m_iRoundAngle,m_iRoundAngle,m_iRoundAngle,m_iRoundAngle,bShowFrame,clrFrame,1,TRUE,clrFill,1);
		}

		//绘制三角形
		Graphics graphics(memDc.m_hDC);
		ImageAttributes ImgAtt;
		ImgAtt.SetWrapMode(WrapModeTileFlipXY);
		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);

		CRect rcArrow = rect;
		rcArrow.left = m_iSplitLineXPos;
		m_arrowInfo.CalcPtData(rcArrow);

		if(EAS_FILL == m_arrowInfo.m_eArrowStyle)
		{
			SolidBrush brush(clrArrow);
			graphics.FillPolygon(&brush,m_arrowInfo.m_aryPtTriangle,3,FillModeAlternate);
		}
		else
		{
			Pen pen(clrArrow,m_arrowInfo.m_iLineW);
			graphics.DrawLines(&pen,m_arrowInfo.m_aryPtTriangle,3);
		}

		//绘制分割线
		Pen gdiplusPen(Color(0XFF,0XFF,0XFF),1);
		Point ptStart,ptEnd;
		ptStart.X = m_iSplitLineXPos;
		ptStart.Y = rect.top + 3;
		ptEnd.X = m_iSplitLineXPos + 1;
		ptEnd.Y = rect.bottom - 3;
		graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);

		graphics.ReleaseHDC(memDc.m_hDC);

		//如果设置了文本，则绘制文本
		CString strText;
		GetWindowText(strText);
		if(0 != strText.GetLength())
		{
			CFont *pOldFont = memDc.SelectObject(&m_fontSet);
			CRect rectText = rect;

			int nOldBkMode = memDc.SetBkMode(TRANSPARENT);
			memDc.SetTextColor(clrText);
			rectText.right = m_iSplitLineXPos;
			memDc.DrawText(strText,&rectText,DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_WORD_ELLIPSIS);

			memDc.SelectObject(pOldFont);
			memDc.SetBkMode(nOldBkMode);
		}

		cDC.BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

		memDc.SelectObject(hOldGdiObject);
		::DeleteObject(hBmp);
		memDc.DeleteDC();
	}
	
	cDC.Detach();

#ifdef _DEBUG
	clock_t endMs = clock() - startMs;
	++endMs;
#endif
}

void CMyInsertBt::OnMouseMove(UINT nFlags, CPoint point)
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

LRESULT CMyInsertBt::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_INSERTBT_NOFOCUS != m_ctrlState)
	{
		m_ctrlState = CTRL_INSERTBT_NOFOCUS;
		Invalidate();
	}
	m_bTracking = FALSE;
	return 0;
}

LRESULT CMyInsertBt::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_INSERTBT_NOFOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_INSERTBT_FOCUS;
		Invalidate();
	}
	m_bTracking = FALSE;
	return 0;
}

void CMyInsertBt::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(CTRL_INSERTBT_FOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_INSERTBT_SELECTED;
		Invalidate();
	}
	__super::OnLButtonDown(nFlags, point);
}

void CMyInsertBt::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CTRL_INSERTBT_SELECTED == m_ctrlState)
	{
		m_ctrlState = CTRL_INSERTBT_FOCUS;
		Invalidate();
	}
	__super::OnLButtonUp(nFlags, point);
}

void CMyInsertBt::OnSize(UINT nType, int cx, int cy)
{
 	__super::OnSize(nType, cx, cy);
}

BOOL CMyInsertBt::PreTranslateMessage(MSG* pMsg)
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

BOOL CMyInsertBt::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyInsertBt::OnTimer(UINT_PTR nIDEvent)
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

void CMyInsertBt::StartTwinkle()
{
	SetTimer(TIMER_ALARMTWINKLE,300,NULL);
	m_bTwinkle = TRUE;
}

void CMyInsertBt::StopTwinkle()
{
	KillTimer(TIMER_ALARMTWINKLE);
	m_bTwinkle = FALSE;
	Invalidate(FALSE);
}

void CMyInsertBt::SetSplitLineXPos(int iSplitLineXPos,BOOL bRedraw /*= FALSE*/)
{
	m_iSplitLineXPos = iSplitLineXPos;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMyInsertBt::GetSplitLineXPos()
{
	return m_iSplitLineXPos;
}








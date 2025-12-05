// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "MyUrlBt.h"
// CMyUrlBt

IMPLEMENT_DYNAMIC(CMyUrlBt,CButton)

CMyUrlBt::CMyUrlBt(BOOL bAutoDynamic /*= FALSE*/,BOOL bUseFourthState /*= TRUE*/)
{
	m_bTracking = FALSE;
	m_ctrlState = CTRL_URLBT_NOFOCUS;
	m_bHasBkPic = FALSE;
	m_bHasPathBkPic = FALSE; 
	m_bHasPngBkPic = FALSE;
	m_textColor = RGB(0,0,0);
	m_bStaticSize = FALSE;
	m_bOnPaneDlg = FALSE;
	m_paneStandardWidth = 1920;
	m_paneStandardHeight = 1080;
	m_btHoriFontRatio = 0;
	m_btVerFontRatio = 0;
	m_sonPicNums = 1;
	m_iconW = 0;
	m_iconLeftIndent = 0;
	m_iconRightIndent = 0;
	m_rightEnd = 0;

	m_txClrNormal = RGB(0,0,0);
	m_txClrFocused = RGB(0,0,0);
	m_txClrClicked = RGB(0,0,0);
	m_txClrDisabled = RGB(0,0,0);

	m_edgeClrNormal = RGB(0XDC,0XDC,0XDC);
	m_edgeClrFocused = RGB(0XDC,0XDC,0XDC);
	m_edgeClrClicked = RGB(0XDC,0XDC,0XDC);
	m_edgeClrDisabled = RGB(0XDC,0XDC,0XDC);

	m_bkClrNormal = RGB(0XFF,0XFF,0XF0);
	m_bkClrFocused = RGB(0XFF,0XFA,0XCD);
	m_bkClrClicked = RGB(0XE6,0XE6,0XFA);
	m_bkClrDisabled = RGB(0XCF,0XCF,0XCF);

	m_bDrawFrame = TRUE;
	m_pngPicPath = L"";

	m_bRoundRectBk = FALSE;
	m_clrRoundRectBk = Color(3,91,199);
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

	m_txFormat = DT_CENTER | DT_SINGLELINE | DT_VCENTER;
	m_bAmoySwtStyle = FALSE;
	m_AmoySwtBkClr = RGB(251,251,251);
	m_bUseFourthState = bUseFourthState;

	m_bCkbtFunc = FALSE;
	m_curStateId = 0;
	m_bMinBt = FALSE;
}

CMyUrlBt::~CMyUrlBt()
{
	m_bTracking = FALSE;

	MyDeletePtr(&m_pMemDc);

	if(NULL != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
}

BEGIN_MESSAGE_MAP(CMyUrlBt, CButton)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,&CMyUrlBt::OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER,&CMyUrlBt::OnMouseHover)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CMyUrlBtmessage handlers
void CMyUrlBt::PreSubclassWindow()
{
	ModifyStyle(0,BS_OWNERDRAW);
	CButton::PreSubclassWindow();
}

void CMyUrlBt::SetBmpBkPic(UINT idPic,int sonPicNums /*= 2*/,int btIconW /*= 0*/,int iconLeftIndent /*= 0*/,int iconRightIndent /*= 0*/,int rightEnd /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_idNormal = idPic;
	m_sonPicNums = sonPicNums;
	m_iconW = btIconW;
	m_iconLeftIndent = iconLeftIndent;
	m_iconRightIndent = iconRightIndent;
	m_rightEnd = rightEnd;
	m_bHasBkPic = TRUE;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyUrlBt::SetPngBkPic(wstring picPath,int sonPicNums,int iconWidth /*= 0*/,int iconLeftIndent /*= 0*/,int iconRightIndent /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	m_sonPicNums = sonPicNums;
	m_bHasPngBkPic = TRUE;
	m_pngPicPath = picPath;
	m_iconW = iconWidth;
	m_iconLeftIndent = iconLeftIndent;
	m_iconRightIndent = iconRightIndent;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyUrlBt::SetNewBmpBkPic(UINT idPic,int sonPicNums,BOOL bInvalidate)
{
	m_idNormal = idPic;
	m_sonPicNums = sonPicNums;
	m_bHasBkPic = TRUE;
	if(bInvalidate && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyUrlBt::SetBmpBkPic(UINT idNormal,UINT idFocus,UINT idSelected,UINT idDisabled,int btIconW /*= 0*/,int iconLeftIndent /*= 0*/,int iconRightIndent /*= 0*/,int rightEnd /*= 0*/)
{
	m_idNormal = idNormal;
	m_idFocus = idFocus;
	m_idSelected = idSelected;
	m_idDisabled = idDisabled;
	m_sonPicNums = 1;
	m_iconW = btIconW;
	m_iconLeftIndent = iconLeftIndent;
	m_iconRightIndent = iconRightIndent;
	m_rightEnd = rightEnd;
	m_bHasBkPic = TRUE;
}

void CMyUrlBt::SetTextColor(COLORREF textColor)
{
	m_textColor = textColor;
}

COLORREF CMyUrlBt::GetTextColor()
{
	return m_textColor;
}

void CMyUrlBt::SetBkColor(COLORREF bkClrNormal,COLORREF bkClrFocused,COLORREF bkClrClicked,COLORREF bkClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyUrlBt::GetBkColor(COLORREF& bkClrNormal,COLORREF& bkClrFocused,COLORREF& bkClrClicked,COLORREF& bkClrDisabled)
{
	bkClrNormal = m_bkClrNormal;
	bkClrFocused = m_bkClrFocused;
	bkClrClicked = m_bkClrClicked;
	bkClrDisabled = m_bkClrDisabled;
	return;
}

void CMyUrlBt::SetTextColor(COLORREF txClrNormal,COLORREF txClrFocused,COLORREF txClrClicked,COLORREF txClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyUrlBt::GetTextColor(COLORREF& txClrNormal,COLORREF& txClrFocused,COLORREF& txClrClicked,COLORREF& txClrDisabled)
{
	txClrNormal = m_txClrNormal;
	txClrFocused = m_txClrFocused;
	txClrClicked = m_txClrClicked;
	txClrDisabled = m_txClrDisabled;
	return;
}

void CMyUrlBt::SetEdgeColor(COLORREF edgeClrNormal,COLORREF edgeClrFocused,COLORREF edgeClrClicked,COLORREF edgeClrDisabled,BOOL bRedraw/* = FALSE*/)
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

void CMyUrlBt::GetEdgeColor(COLORREF& edgeClrNormal,COLORREF& edgeClrFocused,COLORREF& edgeClrClicked,COLORREF& edgeClrDisabled)
{
	edgeClrNormal = m_edgeClrNormal;
	edgeClrFocused = m_edgeClrFocused;
	edgeClrClicked = m_edgeClrClicked;
	edgeClrDisabled = m_edgeClrDisabled;
}

void CMyUrlBt::SetRoundRectParam(BOOL bRoundRect,Color clrRoundRectBk,BOOL bRedraw /*= FALSE*/)
{
	m_bRoundRectBk = bRoundRect;
	m_clrRoundRectBk = clrRoundRectBk;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyUrlBt::GetRoundRectParam(BOOL& bRoundRect,Color& clrRoundRectBk)
{
	bRoundRect = m_bRoundRectBk;
	clrRoundRectBk = m_clrRoundRectBk;
}

void CMyUrlBt::SetDrawFrame(BOOL bDrawFrame,BOOL bRedraw /*= FALSE*/)
{
	m_bDrawFrame = bDrawFrame;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyUrlBt::GetDrawFrame()
{
	return m_bDrawFrame;
}

void CMyUrlBt::SetBeInMiddle(BOOL bInMiddle,BOOL bRedraw /*= FALSE*/)
{
	m_bInMiddle = bInMiddle;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyUrlBt::GetBeInMiddle()
{
	return m_bInMiddle;
}

void CMyUrlBt::SetTextFormat(UINT txFormat,BOOL bRedraw /*= FALSE*/)
{
	m_txFormat = txFormat;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

UINT CMyUrlBt::GetTextFormat()
{
	return m_txFormat;
}

void CMyUrlBt::SetAmoySwtParam(BOOL bAmoySwtStyle,COLORREF bkClr,BOOL bRedraw /*= FALSE*/)
{
	m_bAmoySwtStyle = bAmoySwtStyle;
	m_AmoySwtBkClr = bkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyUrlBt::GetAmoySwtParam(BOOL& bAmoySwtStyle,COLORREF& bkClr)
{
	bAmoySwtStyle = m_bAmoySwtStyle;
	bkClr = m_AmoySwtBkClr;
	return;
}

void CMyUrlBt::SetBeAmoySwtStyle(BOOL bAmoySwtStyle,BOOL bRedraw /*= FALSE*/)
{
	m_bAmoySwtStyle = bAmoySwtStyle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyUrlBt::GetBeAmoySwtStyle()
{
	return m_bAmoySwtStyle;
}

void CMyUrlBt::SetAmoySwtBkClr(COLORREF bkClr,BOOL bRedraw /*= FALSE*/)
{
	m_AmoySwtBkClr = bkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMyUrlBt::GetAmoySwtBkClr()
{
	return m_AmoySwtBkClr;
}

void CMyUrlBt::SetGradientParam(BOOL bGradientBk,Color glrStart,Color glrEnd,LinearGradientMode gradientMode,BOOL bRedraw /*= FALSE*/)
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

void CMyUrlBt::GetGradientParam(BOOL& bGradientBk,Color& glrStart,Color& glrEnd,LinearGradientMode& gradientMode)
{
	 bGradientBk = m_bGradientBk;
	 glrStart = m_glrStart;
	 glrEnd = m_glrEnd;
	 gradientMode = m_gradientMode;
}

void CMyUrlBt::SetCkbtFuncParam(BOOL bCkbtFunc,int curState,BOOL bRedraw /*= FALSE*/)
{
	m_bCkbtFunc = bCkbtFunc;
	m_curStateId = curState;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

void CMyUrlBt::GetCkbtFuncParam(BOOL& bCkbtFunc,int& curState)
{
	bCkbtFunc = m_bCkbtFunc;
	curState = m_curStateId;
	return;
}

void CMyUrlBt::SetBeCkbtFunc(BOOL bCkbtFunc,BOOL bRedraw /*= FALSE*/)
{
	m_bCkbtFunc = bCkbtFunc;
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyUrlBt::GetBeCkbtFunc()
{
	return m_bCkbtFunc;
}

void CMyUrlBt::SetCkbtFuncCurState(int curState,BOOL bRedraw /*= FALSE*/)
{
	m_curStateId = curState;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

BOOL CMyUrlBt::GetCkbtFuncCurState()
{
	return m_curStateId;
}

void CMyUrlBt::SetBeMinimizeBt(BOOL bMinBt,BOOL bRedraw /*= FALSE*/)
{
	m_bMinBt = bMinBt;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyUrlBt::GetBeMinimizeBt()
{
	return m_bMinBt;
}

void CMyUrlBt::EnableWindow(BOOL bEnabled)
{
	if(bEnabled)
	{
		m_ctrlState = CTRL_URLBT_NOFOCUS;
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
	
		m_ctrlState = CTRL_URLBT_DISABLE;
	}
	CButton::EnableWindow(bEnabled);
}

void CMyUrlBt::SetBStaticSize(BOOL bStaticSize)
{
	m_bStaticSize = bStaticSize;
}

void CMyUrlBt::SetRatioMode(BOOL bIsStaticRatio/* = FALSE*/,BOOL bOnPaneDlg/* = FALSE*/,BOOL bInvalidate /*= FALSE*/,int paneStandardWidth /*= 1920*/,int paneStandardHeight/* = 1080*/)
{
	if((paneStandardWidth <= 0) || (paneStandardHeight <= 0))
	{
		AfxMessageBox(_T("CMyUrlBt::SetRatioMode失败，paneStandardWidth和paneStandardHeight必须都大于零!"));
		return;
	}
	m_bStaticSize = bIsStaticRatio;
	m_bOnPaneDlg = bOnPaneDlg;
	m_paneStandardWidth = paneStandardWidth;
	m_paneStandardHeight = paneStandardHeight;

	if(!m_bStaticSize)
	{
		if(!m_bOnPaneDlg)
		{
			m_btHoriFontRatio = (double(m_fontOriginalHeight * GeneDpi)) / m_uiGeneDesDpi / GeneScreenXWidth;
			m_btVerFontRatio = (double(m_fontOriginalHeight * GeneDpi)) / m_uiGeneDesDpi / GeneScreenYHeight;
			GetLogFont().lfHeight = GeneCRDlg.Width() * m_btHoriFontRatio * 0.5 + GeneCRDlg.Height() * m_btVerFontRatio * 0.5;  
		}
		else
		{	//父窗口是对话框
			CRect rect;
			GetParent()->GetClientRect(&rect);
			m_btHoriFontRatio = (double(m_fontOriginalHeight * GeneDpi)) / m_uiGeneDesDpi / m_paneStandardWidth;
			m_btVerFontRatio = (double(m_fontOriginalHeight * GeneDpi)) / m_uiGeneDesDpi / m_paneStandardHeight;
			GetLogFont().lfHeight = rect.Width() * m_btHoriFontRatio * 0.5 + rect.Height() * m_btVerFontRatio * 0.5;
		}
	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		GetLogFont().lfHeight =  rcClient.Height() * 0.7;  
	}

	SetMyFont();

	if(bInvalidate)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}
}

void CMyUrlBt::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
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

	//如果设置了文本，则绘制文本
	CString strText;
	GetWindowText(strText);
	if(0 != strText.GetLength())
	{
		switch(m_ctrlState)
		{
		case CTRL_URLBT_NOFOCUS:
			m_textColor = m_txClrNormal;
			break;
		case CTRL_URLBT_FOCUS:
			m_textColor = m_txClrFocused; 
			break;
		case CTRL_URLBT_SELECTED:
			m_textColor = m_txClrClicked;
			break;
		case CTRL_URLBT_DISABLE:
			if(m_bUseFourthState)
			{
				m_textColor = m_txClrDisabled;
			}
			else
			{
				m_textColor = m_txClrClicked;
			}
			break;
		}

		CFont *pOldFont = memDCTemp.SelectObject(&m_fontSet);
		CRect rectText = lpDrawItemStruct->rcItem;

		UINT state = lpDrawItemStruct->itemState;
		if(state & ODS_SELECTED)
		{
			rectText.OffsetRect(1,1);
		}

		int nOldBkMode = memDCTemp.SetBkMode(TRANSPARENT);
		memDCTemp.SetTextColor(m_textColor);
		rectText.left = (m_iconLeftIndent + m_iconW + m_iconRightIndent);
		if(!m_bStaticSize)
		{
			rectText.left *= GeneScreenXWidth / m_uiGeneDesWidth;
		}

		memDCTemp.DrawText(strText,&rectText,m_txFormat);

		CRect rcTemp;
		memDCTemp.DrawText(strText,&rcTemp,DT_CALCRECT);
		POINT ptStart,ptEnd;
		if(m_txFormat & DT_CENTER)
		{
			ptStart.x = (rectText.Width() - rcTemp.Width()) / 2;
			ptEnd.x = ptStart.x + rcTemp.Width();
		}
		else
		{
			ptStart.x = 0;
			ptEnd.x = rcTemp.Width();
		}

		if(m_txFormat & DT_VCENTER)
		{
			ptStart.y = ptEnd.y = (rectText.Height() - rcTemp.Height()) / 2 + rcTemp.Height() + 1;
		}
		else
		{
			ptStart.x = ptEnd.x = rcTemp.Height() + 1;
		}
		
		CPen pen;
		pen.CreatePen(PS_SOLID,1,m_textColor);
		CPen* pOldPen = memDCTemp.SelectObject(&pen);
		memDCTemp.MoveTo(ptStart);
		memDCTemp.LineTo(ptEnd);
		memDCTemp.SelectObject(pOldPen);
		pen.DeleteObject();

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

void CMyUrlBt::OnMouseMove(UINT nFlags, CPoint point)
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

LRESULT CMyUrlBt::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_URLBT_NOFOCUS != m_ctrlState)
	{
		m_ctrlState = CTRL_URLBT_NOFOCUS;
		Invalidate();
	}
	m_bTracking = FALSE;
	return 0;
}

LRESULT CMyUrlBt::OnMouseHover(WPARAM wParam,LPARAM lParam)
{
	if(CTRL_URLBT_NOFOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_URLBT_FOCUS;
		Invalidate();
	}
	m_bTracking = FALSE;
	return 0;
}

void CMyUrlBt::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(CTRL_URLBT_FOCUS == m_ctrlState)
	{
		m_ctrlState = CTRL_URLBT_SELECTED;
		Invalidate();
	}
	__super::OnLButtonDown(nFlags, point);
}

void CMyUrlBt::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(CTRL_URLBT_SELECTED == m_ctrlState)
	{
		m_ctrlState = CTRL_URLBT_FOCUS;
		Invalidate();
	}
	__super::OnLButtonUp(nFlags, point);
}

void CMyUrlBt::OnSize(UINT nType, int cx, int cy)
{
	if(IsWindow(m_hWnd))
	{
		if(!m_bStaticSize)
		{
			if(!m_bOnPaneDlg)
			{
				GetLogFont().lfHeight = GeneCRDlg.Width() * m_btHoriFontRatio * 0.6 + GeneCRDlg.Height() * m_btVerFontRatio * 0.4; 
			}
			else
			{	//父窗口是对话框
				CRect rect;
				GetParent()->GetClientRect(&rect);
				GetLogFont().lfHeight = rect.Width() * m_btHoriFontRatio * 0.5 + rect.Height() * m_btVerFontRatio * 0.5;
			}
		}
		else
		{
			CRect rcClient;
			GetClientRect(&rcClient);
			GetLogFont().lfHeight =  rcClient.Height() * 0.7;  
		}

		SetMyFont();
		Invalidate();
	}

 	__super::OnSize(nType, cx, cy);
}

BOOL CMyUrlBt::PreTranslateMessage(MSG* pMsg)
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

BOOL CMyUrlBt::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
#include "StdAfx.h"
#include "BioButton.h"
#include "BioMemDC.h"
#include <math.h>
//#include <afximpl.h>



/////////////////////////////////////////////////////////////////////////////
// CBioButton
/////////////////////////////////////////////////////////////////////////////
CBioButton::CBioButton()
{
	m_pToolTip = NULL;

	m_nStyle = BTN_STYLE_FLAT;
	m_nAlign = BTN_ALIGN_HORZ;

	m_sizeImage	= CSize(0,0);
	m_sizeBigImage = CSize(0, 0);
	m_nBorderGap = 4;
	m_nImageGap = 8;
	m_pFontText = NULL;
	m_nCunstomNumber = -1;

	m_bHasFlatBorder = TRUE;
	m_bLBtnDown = FALSE;
	m_bPainted = FALSE;
    m_bIsHilited = FALSE;
	m_bTransparent = TRUE;
	m_bAutoDisabled = TRUE;
	m_bDefault = FALSE;

	m_bAutoBigger = TRUE;
	m_fBigPercent = 1.2f;
	m_bIsChecked = FALSE;

	m_bGradualColor = FALSE;
	m_bUser3DColor = FALSE;

	m_clrThinEDGE = RGB(0x88, 0x9e, 0xc1);
	m_bOnlyIcon = FALSE;
	m_nCornerRadius = 6;
	m_clrParentBgk = ::GetSysColor(COLOR_3DFACE);

	Initialize();
}

CBioButton::~CBioButton()
{
	if (m_pFontText != NULL)
	{
		delete m_pFontText;
		m_pFontText = NULL;
	}

	if (m_pToolTip != NULL)
	{
		delete m_pToolTip;
		m_pToolTip = NULL;
	}

	CleanUpGDI();
}

void CBioButton::Initialize(void)
{
	// 初始化图标，为空
	for (int i=0; i<BTN_MAX_TYPES; i++)
	{
		m_hIcons[i] = NULL;
	}
	
	// 初始化颜色
	m_clrColors[BTN_COLOR_BKGIN] = ::GetSysColor(COLOR_BTNFACE);
	m_clrColors[BTN_COLOR_TXTIN] = ::GetSysColor(COLOR_BTNTEXT);
	m_clrColors[BTN_COLOR_BKGOUT] = ::GetSysColor(COLOR_BTNFACE);
	m_clrColors[BTN_COLOR_TXTOUT] = ::GetSysColor(COLOR_BTNTEXT);
	m_clrColors[BTN_COLOR_BKGFOCUS] = ::GetSysColor(COLOR_BTNFACE);
	m_clrColors[BTN_COLOR_TXTFOCUS] = ::GetSysColor(COLOR_BTNTEXT);

	// 初始化字体
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = -14;
	_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("微软雅黑"), 22);
	
	m_fontNormal.CreateFontIndirect(&lf);
}

IMPLEMENT_DYNAMIC(CBioButton, CButton)

BEGIN_MESSAGE_MAP(CBioButton, CButton)
	//{{AFX_MSG_MAP(CBioButton)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBioButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
    NoteButtonDepressed(TRUE);
	CButton::OnLButtonDown(nFlags, point);
}

void CBioButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
    NoteButtonDepressed(FALSE);
	CButton::OnLButtonUp(nFlags, point);
}

void CBioButton::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1)
	{
		CRect rc;
		GetWindowRect(&rc);

		CPoint pt;
		GetCursorPos(&pt);

		if (m_bLBtnDown == TRUE || (!rc.PtInRect(pt)))
		{
			KillTimer(1);

            m_bIsHilited = FALSE;

            if (m_bPainted == TRUE) 
			{
				InvalidateRect (NULL);
			}

			m_bPainted = FALSE;
			return;
		}

		// On mouse over, show raised button.
		else if (!m_bPainted)
		{
            m_bIsHilited = TRUE;
			InvalidateRect(NULL);
			m_bPainted = true;
		}
	}
	
	CButton::OnTimer(nIDEvent);
}

void CBioButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS != NULL);

	// define some temporary variables.
	CDC*  pDC	    = CDC::FromHandle( lpDIS->hDC );
	CRect rcItem    = lpDIS->rcItem;
	int   nState    = lpDIS->itemState;
	int   nSavedDC  = pDC->SaveDC();
	BOOL  bSelected = ((nState & ODS_SELECTED) == ODS_SELECTED);
	BOOL  bFocused = ((nState & ODS_FOCUS) == ODS_FOCUS);
	
	if (pDC == NULL)
		return;

	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	// Paint the background.
	COLORREF clrBackground;
	COLORREF clrText;
	if (m_bIsHilited)
	{
		clrBackground = m_clrColors[BTN_COLOR_BKGIN];
		clrText = m_clrColors[BTN_COLOR_TXTIN];
	}
	else if (bFocused)
	{
		clrBackground = m_clrColors[BTN_COLOR_BKGFOCUS];
		clrText = m_clrColors[BTN_COLOR_TXTFOCUS];
	}
	else
	{
		clrBackground = m_clrColors[BTN_COLOR_BKGOUT];
		clrText = m_clrColors[BTN_COLOR_TXTOUT];
	}

	CRgn rgn;
	if(m_nStyle == BTN_STYLE_THIN_EDGE && m_nCornerRadius > 0)
	{
		rgn.CreateRoundRectRgn(rcItem.left, rcItem.top, rcItem.right, rcItem.bottom, m_nCornerRadius, m_nCornerRadius);
		//SetWindowRgn(rgn, TRUE);

		/*HDC hdc; 
		hdc = ::GetDC(NULL); 
		CRect rect = rcItem;
		ClientToScreen(&rect);
		POINT lpPoint = rect.TopLeft();
		lpPoint.x = lpPoint.x - 2;
		COLORREF color = ::GetPixel(hdc, lpPoint.x, lpPoint.y); 
		pDC->FillSolidRect(rcItem, color);*/
	}

	// 绘制背景
	if(!m_bGradualColor)	// 渐变色
	{
		if (m_bTransparent)
		{
			DrawParentBackground(this, pDC);
		}
		else
		{
			pDC->FillSolidRect(rcItem, clrBackground);
		}
	}
	else
	{
		//DrawGradualBackground
		DrawGradualBackground(pDC, rcItem);
	}


	COLORREF clr3DHilight = ::GetSysColor(COLOR_3DHILIGHT);
	COLORREF clr3DShadow = ::GetSysColor(COLOR_3DSHADOW);
	
	switch (m_nStyle)
	{
	case BTN_STYLE_FLAT:
        if ((m_bIsHilited || bSelected) && m_bHasFlatBorder && !m_bIsChecked)
        {
        	pDC->Draw3dRect( rcItem, 
                bSelected ? clr3DShadow  : clr3DHilight,
    			bSelected ? clr3DHilight : clr3DShadow );
        } // if ((m_bIsHilited || bSelected) && m_bHasFlatBorder && !m_bIsChecked)
		else if (m_bIsChecked)
		{
        	pDC->Draw3dRect(rcItem, clr3DShadow, clr3DHilight);
		}
		break;
	case BTN_STYLE_SEMIFLAT:
		if (!m_bIsChecked)
		{
			CRect rcTemp = rcItem;
			if (m_bDefault)
			{
				rcTemp.DeflateRect(1,1);
				pDC->Draw3dRect(rcItem, RGB(0,0,0), RGB(0,0,0));
			}
			pDC->Draw3dRect( rcTemp , 
				bSelected ? clr3DShadow : clr3DHilight,
				bSelected ? clr3DHilight : clr3DShadow);
		} // if (!m_bIsChecked)
		else
		{
			CRect rcTemp = rcItem;
			if (m_bDefault)
			{
				rcTemp.DeflateRect(1,1);
				pDC->Draw3dRect(rcItem, RGB(0,0,0), RGB(0,0,0));
			}
			pDC->Draw3dRect( rcTemp, clr3DShadow ,clr3DHilight);
		}
		break;
	case BTN_STYLE_NORMAL:
		if (!m_bIsChecked)
		{
			if(m_bUser3DColor)
			{
				CRect rcTemp = rcItem;
				if (m_bDefault)
				{
					pDC->Draw3dRect(rcItem, RGB(0,0,0), RGB(0,0,0));
					rcTemp.DeflateRect(1, 1);
				}
				pDC->Draw3dRect(rcTemp, bSelected ? m_clrUser3DRightBottom : m_clrUser3DTopLeft,
					bSelected ? m_clrUser3DTopLeft : m_clrUser3DRightBottom);
				rcTemp.DeflateRect(1,1);
				pDC->Draw3dRect(rcTemp, bSelected ? m_clrUser3DRightBottom : clrBackground,
					bSelected ? clrBackground : m_clrUser3DRightBottom);
			}
			else
			{
				CRect rcTemp = rcItem;
				if (m_bDefault)
				{
					pDC->Draw3dRect(rcItem, RGB(0,0,0), RGB(0,0,0));
					rcTemp.DeflateRect(1, 1);
				}
				pDC->Draw3dRect(rcTemp, bSelected ? ::GetSysColor(COLOR_3DDKSHADOW) : clr3DHilight,
					bSelected ? clr3DHilight : ::GetSysColor(COLOR_3DDKSHADOW));
				rcTemp.DeflateRect(1,1);
				pDC->Draw3dRect(rcTemp, bSelected ? clr3DShadow : clrBackground,
					bSelected ? clrBackground : clr3DShadow);
			}
		} // if (!m_bIsChecked)
		else
		{
			CRect rcTemp = rcItem;		
			if (m_bDefault)
			{
				pDC->Draw3dRect(rcItem, RGB(0,0,0), RGB(0,0,0));
				rcTemp.DeflateRect(1,1);
			}
			pDC->Draw3dRect(rcTemp, GetSysColor(COLOR_3DDKSHADOW) ,clr3DHilight);
			rcTemp.DeflateRect(1,1);
			pDC->Draw3dRect(rcTemp, clr3DShadow ,clrBackground);
		}
		break;
	case BTN_STYLE_CIRCLE:
		{
			//pDC->Ellipse(rcItem);
			//pDC->DrawEdge(rcItem, EDGE_RAISED, BF_RECT);
			CRgn m_rgn;
			m_rgn.CreateEllipticRgn(rcItem.left, rcItem.top, rcItem.right, rcItem.bottom);
			SetWindowRgn(m_rgn, TRUE);
			//pDC->Ellipse(rcItem);
		}
		break;
	case BTN_STYLE_THIN_EDGE:
		{
			CBrush brush(m_clrThinEDGE);
			pDC->FrameRgn(&rgn, &brush, 1, 1);
			//pDC->FrameRect(rcItem, &brush);
			if(bSelected || bFocused)
			{
				CRect rcItemFocus = rcItem;
				rcItemFocus.DeflateRect(1,1);
				rcItemFocus.right = rcItemFocus.right - 1;
				rcItemFocus.bottom = rcItemFocus.bottom - 1;
				pDC->DrawFocusRect(rcItemFocus);
			}
		}
		break;
	}

	// Set the background mode to transparent.
	int nOldBkMode = pDC->SetBkMode(TRANSPARENT);
	COLORREF clrOldBk = pDC->SetBkColor(clrBackground);
	
	// draw the button text, icon or bitmap.
	if(m_nCunstomNumber < 0)
	{
		DrawButtonText(pDC, nState, rcItem, clrText);
		DrawButtonIcon(pDC, nState, rcItem);
	}
	else
	{
		CRect rcBtnText, rcNumber;
		rcBtnText = rcItem;
		rcBtnText.top = rcItem.top + rcItem.Height() / 2;
		DrawCustomText(pDC, nState, rcBtnText, clrText, GetButtonText(TRUE));

		rcNumber = rcItem;
		rcNumber.bottom = rcItem.top + rcItem.Height() / 2;
		DrawCustomNumber(pDC, nState, rcNumber, clrText);
	}

	if (m_bIsChecked)
	{
		rcItem.DeflateRect(1,1);
		pDC->DrawFocusRect(rcItem);
	}


	pDC->SetBkMode(nOldBkMode);
	pDC->SetBkColor(clrOldBk);

	pDC->RestoreDC(nSavedDC);
}

CString CBioButton::GetButtonText(BOOL bRemoveAmpersand)
{
	CString strText;
	GetWindowText(strText);
	
	if (bRemoveAmpersand) {
		StripMnemonics(strText);
	}

	return strText;
}

void CBioButton::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem, COLORREF clrText)
{
	// Get the window text, return if empty.
	CString strText = GetButtonText(TRUE);

	if( !strText.IsEmpty() )
	{
		CFont *oldFont;
		if (m_pFontText == NULL)
			oldFont = pDC->SelectObject(GetFont());
		else
			oldFont = pDC->SelectObject(m_pFontText);

		CSize sizeText = pDC->GetTextExtent(strText);
		CRect rcText = rcItem;
		rcText.DeflateRect(2,0);
		CPoint point = rcText.TopLeft();

		OffsetPoint(point, sizeText);
		point.x = m_nBorderGap;
		
		DWORD dwStyle = GetStyle();
		if (m_nAlign == BTN_ALIGN_VERT) 
		{
			point.y += (m_sizeImage.cy/2)+(m_nImageGap/2);
		} 

		// horz. alignment
		if ((dwStyle & BS_CENTER) == BS_CENTER	|| (dwStyle & BS_CENTER) == 0)
		{
			if (m_nAlign == BTN_ALIGN_HORZ) // 图标水平在左
			{
				point.x += m_sizeImage.cx;
				if(m_sizeImage.cx != 0 )
				{
					point.x += m_nImageGap;
				}
				//point.x += m_sizeImage.cx +  m_sizeImage.cx == 0 ? 0 : m_nImageGap;
				//point.x = rcItem.CenterPoint().x - (m_sizeImage.cx + m_nImageGap + sizeText.cx) / 2 + m_sizeImage.cx + m_nImageGap;
			}
			point.x += (rcItem.right - point.x - sizeText.cx - m_nBorderGap) / 2;
		}
		else if (dwStyle & BS_LEFT)
		{
			if (m_nAlign == BTN_ALIGN_HORZ)
			{
				point.x += m_sizeImage.cx + m_nImageGap;
			}
		}
		else // right
		{
			point.x = rcItem.right - m_nBorderGap - sizeText.cx;
		}

		// Set the draw state flags.
		DWORD dwFlags = DST_PREFIXTEXT;
		if( nState & ODS_DISABLED ) 
		{
			pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		}
		else
		{
			dwFlags |= DSS_NORMAL;
			pDC->SetTextColor(clrText);
		}

		if( (nState & ODS_SELECTED) && (m_nStyle == BTN_STYLE_THIN_EDGE))
		//if(m_bLBtnDown && (m_nStyle == BTN_STYLE_THIN_EDGE))
		{
			point.x = point.x + 1;
			point.y = point.y + 1;
		}
		// draw the text, and select the original font.
		strText = GetButtonText(FALSE);
        pDC->SetTextAlign(TA_LEFT);
		pDC->DrawState( point, sizeText, strText, dwFlags, TRUE, strText.GetLength(), (HBRUSH)NULL );
		pDC->SelectObject( oldFont );
	}
}

void CBioButton::DrawCustomText( CDC* pDC, UINT nState, CRect& rcItem, COLORREF clrText, CString& strText )
{
	// 仿照上面DrawButtonText
	if( !strText.IsEmpty() )
	{
		CFont *oldFont;
		if (m_pFontText == NULL)
			oldFont = pDC->SelectObject(GetFont());
		else
			oldFont = pDC->SelectObject(m_pFontText);

		CSize sizeText = pDC->GetTextExtent(strText);
		CRect rcText = rcItem;
		rcText.DeflateRect(2,0);

		// Set the draw state flags.
		
		
		DWORD dwStyle = GetStyle();

		DWORD dwFlags = DST_PREFIXTEXT;
		if( nState & ODS_DISABLED ) 
		{
			pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
		}
		else
		{
			dwFlags |= DSS_NORMAL;
			pDC->SetTextColor(clrText);
		}

		if( (nState & ODS_SELECTED) && (m_nStyle == BTN_STYLE_THIN_EDGE))
			//if(m_bLBtnDown && (m_nStyle == BTN_STYLE_THIN_EDGE))
		{
			rcItem.OffsetRect(1, 1);
		}
		// draw the text, and select the original font.
		pDC->SetTextAlign(TA_LEFT);
		//pDC->DrawState( point, sizeText, strText, dwFlags, TRUE, strText.GetLength(), (HBRUSH)NULL );
		pDC->DrawText(strText, rcItem, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		pDC->SelectObject( oldFont );
	}
}

void CBioButton::DrawCustomNumber( CDC* pDC, UINT nState, CRect& rcItem, COLORREF clrText )
{
	if(m_nCunstomNumber < 0)
		return;

	CString strNumber;
	strNumber.Format(_T("%d"), m_nCunstomNumber);
	DrawCustomText(pDC, nState, rcItem, clrText, strNumber);
}

CPoint CBioButton::CalculateImagePosition(CDC* pDC, UINT nState, CRect& rcItem, BOOL bHasPushedImage)
{
	// Get the window text, return if empty.
	CString strText  = GetButtonText(TRUE);
	CSize   sizeText = pDC->GetTextExtent(strText);
	CPoint  point    = rcItem.TopLeft();
	DWORD dwStyle = GetStyle();
	CSize sizeImage = m_sizeImage;
	
	if (m_bIsHilited && m_bAutoBigger)
	{
		sizeImage = m_sizeBigImage;
	}

	OffsetPoint(point, sizeImage);
    point.x = m_nBorderGap;

    if (m_nAlign == BTN_ALIGN_VERT) 
    {
        if (!strText.IsEmpty())
        {
			point.y -= ((sizeText.cy/2)+(m_nImageGap/2));
        }
		
        // horz. alignment - only for two-row mode, otherwise always
        // on left edge
        if ((dwStyle & BS_CENTER) == BS_CENTER  ||
            (dwStyle & BS_CENTER) == 0)
        {
            point.x = rcItem.left +
                __max(0,((rcItem.Width()-sizeImage.cx)/2));
        }
        else if (dwStyle & BS_RIGHT)
        {
			point.x = rcItem.right - m_nBorderGap - sizeImage.cx;
        }
	}
	else
	{
		if (strText.IsEmpty())
		{
			if ((dwStyle & BS_CENTER) == BS_CENTER)
	        {
				int cx = rcItem.Width();
				if (cx >= m_sizeImage.cx)
				{
					point.x = (cx - sizeImage.cx)/2;
				}
			}
			if  ((dwStyle & BS_VCENTER) == BS_VCENTER)
			{
				int cy = rcItem.Height();
				if (cy >= m_sizeImage.cy)
				{
					point.y = (cy - sizeImage.cy)/2;
				}
			}
		}
		else
		{
			int nGap = m_nImageGap;
			if(rcItem.Width() >= (sizeImage.cx + nGap + sizeText.cx))
			{
				point.x = rcItem.CenterPoint().x - (sizeImage.cx + nGap + sizeText.cx) / 2;
			}
			else if(rcItem.Width() >= (sizeImage.cx + sizeText.cx))
			{
				nGap = (rcItem.Width() - sizeImage.cx - sizeText.cx) / 3;
				point.x = rcItem.CenterPoint().x - (sizeImage.cx + nGap + sizeText.cx) / 2;
			}
			else
			{
				point.x = 0;
			}

			int cy = rcItem.Height();
			if (cy >= m_sizeImage.cy)
			{
				point.y = rcItem.top + (cy - sizeImage.cy) / 2;
			}
			else
			{
				point.y = 0;
			}
		}
	}

	if(m_bOnlyIcon)
	{
		point.x = rcItem.CenterPoint().x - m_sizeImage.cx / 2;
		point.y = rcItem.CenterPoint().y - m_sizeImage.cy / 2;
	}

	// if the button is selected.
	if( nState & ODS_SELECTED )
	{
		if( !bHasPushedImage ) 
        {
			point.Offset(1,1);
		}
	}

    return point;
}

void CBioButton::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem)
{
	HICON hIconNormal = m_hIcons[BTN_TYPE_NORMAL];
	HICON hIconPressed = m_hIcons[BTN_TYPE_PRESSED];
	HICON hIconDisabled = m_hIcons[BTN_TYPE_DISABLED];
	if (hIconNormal != NULL)
	{
        CPoint point = CalculateImagePosition(pDC, nState, rcItem, (hIconPressed != NULL));

		if( hIconPressed && (nState & ODS_SELECTED) ) 
        {
			hIconNormal = hIconPressed;
		}
		
		if (hIconDisabled && (nState & ODS_DISABLED))
		{
			hIconNormal = hIconDisabled;
		}
		

		// Set the draw state flags.
		DWORD dwFlags = DST_ICON;
		if( nState & ODS_DISABLED && hIconDisabled == NULL) 
		{
			dwFlags |= DSS_DISABLED;
		}
		else 
		{
			dwFlags |= DSS_NORMAL;
		}

		// draw the icon associated with this button.
		if (m_bIsHilited && m_hIcons[BTN_TYPE_HILITED])
		{
			pDC->DrawState( point, m_sizeBigImage, m_hIcons[BTN_TYPE_HILITED], dwFlags, (HBRUSH)NULL );
		}
		else
			pDC->DrawState( point, m_sizeImage, hIconNormal, dwFlags, (HBRUSH)NULL );
	}
}


void CBioButton::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	POINT pt;

	::GetCursorPos(&pt);
	::MapWindowPoints(NULL,(HWND)(m_hWnd),&pt,1);
	::SendMessage((HWND)(m_hWnd),WM_LBUTTONDOWN,0,MAKELPARAM(pt.x,pt.y)); 
	
	CButton::OnLButtonDblClk(nFlags, point);
}


void CBioButton::PreSubclassWindow() 
{
	CButton::PreSubclassWindow();
	
	// Initialize the control.
	DeferInitialUpdate();
}

void CBioButton::DeferInitialUpdate()
{
	ModifyStyle(0, BS_OWNERDRAW);
	SetFont(&m_fontNormal);
}

int CBioButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Initialize the control.
	DeferInitialUpdate();
	
	return 0;
}

void CBioButton::CleanUpGDI()
{
	HICON hIcon = NULL;
	for (int i=0; i<BTN_MAX_TYPES; i++)
	{
		hIcon = m_hIcons[i];
		if (hIcon != NULL)
		{
			::DestroyIcon(hIcon);
			hIcon = NULL;
		}
	}
}

BOOL CBioButton::SetIcon(CSize size, HICON hIcon, HICON hIconPushed, HICON hIconHilited, BOOL bRedraw)
{
	// Save the image size and set the icon
	// handles to NULL.
	m_hIcons[BTN_TYPE_NORMAL] = hIcon;
	m_hIcons[BTN_TYPE_PRESSED] = hIconPushed;
	m_hIcons[BTN_TYPE_HILITED] = hIconHilited;
	m_sizeImage = size;

	if (hIconHilited)
		m_bAutoBigger = FALSE;

	// 创建灰度图标作为非激活图标
	if (m_bAutoDisabled)
	{
		HICON hIconDisabled = CreateGrayscaleIcon(hIcon);
		m_hIcons[BTN_TYPE_DISABLED] = hIconDisabled;
	}

	// Redraw the button.
	if (GetSafeHwnd() && bRedraw) 
	{
		Invalidate();
	}

	return TRUE;
}

BOOL CBioButton::SetIcon(CSize size, UINT nID, UINT nPushedID, UINT nHilitedID, BOOL bRedraw)
{
	UNUSED_ALWAYS(bRedraw);
	return SetIcon(size, MAKEINTRESOURCE(nID), MAKEINTRESOURCE(nPushedID)
		, MAKEINTRESOURCE(nHilitedID), bRedraw);
}

BOOL CBioButton::SetIcon(CSize size, LPCTSTR lpszID, LPCTSTR lpszPushedID, LPCTSTR lpszHilitedID, BOOL bRedraw)
{
	// Free previous resources (if any).
	CleanUpGDI();

	// Find the resource for the normal icon and load the image.
	//HINSTANCE hInst = AfxFindResourceHandle(lpszID, RT_GROUP_ICON);
	//HINSTANCE hInst = AfxFindResourceHandle(lpszID, RT_ICON);
	HICON hIcon = (HICON)::LoadImage(NULL, lpszID, 
					IMAGE_ICON, size.cx, size.cy, LR_LOADFROMFILE);

	HICON hIconPressed = NULL;
	if (lpszPushedID) 
	{
		// Find the resource for the pushed icon and load the image.
		//hInst = AfxFindResourceHandle(lpszPushedID, RT_GROUP_ICON);
		//hInst = AfxFindResourceHandle(lpszPushedID, RT_ICON);
		hIconPressed = (HICON)::LoadImage(NULL, lpszPushedID, 
						IMAGE_ICON, size.cx, size.cy, LR_LOADFROMFILE);
	}
	
	HICON hIconHilited = NULL;
	if (lpszHilitedID) 
	{
		// Find the resource for the pushed icon and load the image.
		//hInst = AfxFindResourceHandle(lpszHilitedID, RT_GROUP_ICON);
		//hInst = AfxFindResourceHandle(lpszHilitedID, RT_ICON);
		hIconHilited = (HICON)::LoadImage(NULL, lpszHilitedID, 
						IMAGE_ICON, size.cx, size.cy, LR_LOADFROMFILE);

		m_bAutoBigger = FALSE;
	} // if (lpszHilitedID)
	else
	{
		if (m_bAutoBigger)
		{
			m_sizeBigImage.cx = (long)(double(size.cx) * m_fBigPercent);
			m_sizeBigImage.cy = (long)(double(size.cy) * m_fBigPercent);

			HICON hIconHilited = (HICON)::LoadImage(NULL, lpszID, 
						IMAGE_ICON, m_sizeBigImage.cx, m_sizeBigImage.cy, LR_DEFAULTCOLOR);
		}
	}

	return SetIcon(size, hIcon, hIconPressed, hIconHilited, bRedraw);
}

/**
 * 设置按钮的非激活图标
 * \param size 
 * \param hIcon 
 * \param bRedraw 
 * \return 
 */
BOOL CBioButton::SetDisabledIcon(HICON hIcon, BOOL bRedraw)
{
	HICON hOldIcon = m_hIcons[BTN_TYPE_DISABLED];
	if (hOldIcon != NULL)
	{
		::DestroyIcon(hOldIcon);
	}
	m_hIcons[BTN_TYPE_DISABLED] = hIcon;

	// Redraw the button.
	if (GetSafeHwnd() && bRedraw) 
	{
		Invalidate();
	}

	return TRUE;
}

/**
 * 设置按钮的非激活图标
 * \param size 
 * \param nID 
 * \param bRedraw 
 * \return 
 */
BOOL CBioButton::SetDisabledIcon(UINT nID, BOOL bRedraw)
{
	return SetDisabledIcon(MAKEINTRESOURCE(nID), bRedraw);
}

/**
 * 设置按钮的非激活图标
 * \param size 
 * \param lpszID 
 * \param bRedraw 
 * \return 
 */
BOOL CBioButton::SetDisabledIcon(LPCTSTR lpszID, BOOL bRedraw)
{
	// Find the resource for the normal icon and load the image.
	HINSTANCE hInst = AfxFindResourceHandle(lpszID, RT_GROUP_ICON);
	HICON hIcon = (HICON)::LoadImage(hInst, lpszID, 
					IMAGE_ICON, m_sizeImage.cx, m_sizeImage.cy, LR_DEFAULTCOLOR);

	return SetDisabledIcon(hIcon, bRedraw);
}


void CBioButton::SetButtonFlat(BOOL bRedraw)
{
	m_nStyle = BTN_STYLE_FLAT;

	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

void CBioButton::SetButtonSemiflat(BOOL bRedraw)
{
	m_nStyle = BTN_STYLE_SEMIFLAT;

	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

void CBioButton::SetButtonNormal(BOOL bRedraw)
{
	m_nStyle = BTN_STYLE_NORMAL;

	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

void CBioButton::SetButtonNoBorder(BOOL bRedraw)
{
	m_nStyle = BTN_STYLE_NOBORDER;

	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

void CBioButton::SetButtonDefault(BOOL bRedraw)
{
	m_bDefault = TRUE;
	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

void CBioButton::SetButtonCircle( BOOL bRedraw/*=TRUE*/ )
{
	m_nStyle = BTN_STYLE_CIRCLE;

	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

/**
 * 按钮是否被选中
 * \param bChecked 
 * \param bRedra 
 */
void CBioButton::SetButtonChecked(BOOL bChecked, BOOL bRedraw)
{
	m_bIsChecked = bChecked;
	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}


void CBioButton::SetIconRight(BOOL bRedraw)
{
	m_nAlign = BTN_ALIGN_HORZ_RIGHT;
	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

void CBioButton::SetIconVert(BOOL bRedraw)
{
	m_nAlign = BTN_ALIGN_VERT;

	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}


void CBioButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//if (m_nStyle == BTN_STYLE_FLAT) 
	//{
		SetTimer(1, 10, NULL);
		OnTimer(1);
	//}

	CButton::OnMouseMove(nFlags, point);
}

void CBioButton::OffsetPoint(CPoint& point, CSize size)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	if (m_nStyle == BTN_ALIGN_VERT) 
	{
        point.x = __max(0,((rcClient.Width()-size.cx)/2));
		point.y = __max(0,((rcClient.Height()-size.cy)/2 -1));
	}
	else 
	{
		point.x = m_nBorderGap;
		point.y = __max(0,((rcClient.Height()-size.cy)/2 -1));
	}
}

void CBioButton::OnPaint()
{
	// background is already filled in gray
	CPaintDC dc(this);

	// Get the client rect.
	CRect rectClient;
	GetClientRect(&rectClient);

	// Paint to a memory device context to help
	// eliminate screen flicker.
	if(m_nStyle == BTN_STYLE_THIN_EDGE)
	{
		CBioMemDC memDC(&dc, rectClient, m_clrParentBgk);

		// Now let the window do its default painting...
		CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	}
	else
	{
		CBioMemDC memDC(&dc, rectClient);

		// Now let the window do its default painting...
		CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );
	}
}

BOOL CBioButton::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return FALSE;
}

void CBioButton::NoteButtonDepressed(BOOL bDepressed)
{
    if (m_bLBtnDown != bDepressed)
    {
        m_bLBtnDown = bDepressed;
        Invalidate();
    }
}

void CBioButton::StripMnemonics(CString& strIn)
{
	CString  strText(strIn);

	// Check to see if '&&' was used for format an ampersand.
	int nIndex = strText.Find(_T("&&"), 0);

	// If not then remove mnemonic formatting.
	if (nIndex == -1) {
		strText.Remove(_T('&'));
	}
	else
	{
		strText.Replace(_T("&&"), _T("^^"));
		strText.Remove(_T('&'));
		strText.Replace(_T("^^"), _T("&&"));
	}

	strIn = strText;
}

void CBioButton::SetToolTip(CString strTip)
{
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl();
		m_pToolTip->Create(this);
	}   // if (m_pToolTip == NULL)
	m_pToolTip->AddTool(this, strTip);
}

BOOL CBioButton::PreTranslateMessage(MSG* pMsg)
{
	if(m_pToolTip != NULL)
	{
		m_pToolTip->RelayEvent(pMsg);
	}

	return CButton::PreTranslateMessage(pMsg);
}

BOOL CBioButton::DrawParentBackground (CWnd* pWnd, CDC* pDC)
{
	BOOL bReturn = FALSE;

	CWnd* pParent = pWnd->GetParent();
	ASSERT_VALID(pParent);

	CPoint pt (0, 0);
	pWnd->MapWindowPoints (pParent, &pt, 1);
	pt = pDC->OffsetWindowOrg (pt.x, pt.y);
	bReturn = (BOOL) pParent->SendMessage(WM_ERASEBKGND, (WPARAM)pDC->m_hDC);

	pDC->SetWindowOrg(pt.x, pt.y);

	return bReturn;
}  


HICON CBioButton::CreateGrayscaleIcon(HICON hIcon)
{
	HICON		hGrayIcon = NULL;
	HDC			hMainDC = NULL, hMemDC1 = NULL, hMemDC2 = NULL;
	BITMAP		bmp;
	HBITMAP		hOldBmp1 = NULL, hOldBmp2 = NULL;
	ICONINFO	csII, csGrayII;
	BOOL		bRetValue = FALSE;

	bRetValue = ::GetIconInfo(hIcon, &csII);
	if (bRetValue == FALSE)	return NULL;

	hMainDC = ::GetDC(m_hWnd);
	hMemDC1 = ::CreateCompatibleDC(hMainDC);
	hMemDC2 = ::CreateCompatibleDC(hMainDC);
	if (hMainDC == NULL || hMemDC1 == NULL || hMemDC2 == NULL)	return NULL;
  
	if (::GetObject(csII.hbmColor, sizeof(BITMAP), &bmp))
	{
		csGrayII.hbmColor = ::CreateBitmap(csII.xHotspot*2, csII.yHotspot*2, bmp.bmPlanes, bmp.bmBitsPixel, NULL);
		if (csGrayII.hbmColor)
		{
			hOldBmp1 = (HBITMAP)::SelectObject(hMemDC1, csII.hbmColor);
			hOldBmp2 = (HBITMAP)::SelectObject(hMemDC2, csGrayII.hbmColor);

			::BitBlt(hMemDC2, 0, 0, csII.xHotspot*2, csII.yHotspot*2, hMemDC1, 0, 0, SRCCOPY);

			DWORD		dwLoopY = 0, dwLoopX = 0;
			COLORREF	crPixel = 0;
			BYTE		byNewPixel = 0;

			for (dwLoopY = 0; dwLoopY < csII.yHotspot*2; dwLoopY++)
			{
				for (dwLoopX = 0; dwLoopX < csII.xHotspot*2; dwLoopX++)
				{
					crPixel = ::GetPixel(hMemDC2, dwLoopX, dwLoopY);

					byNewPixel = (BYTE)((GetRValue(crPixel) * 0.299) + (GetGValue(crPixel) * 0.587) + (GetBValue(crPixel) * 0.114));
					if (crPixel)	::SetPixel(hMemDC2, dwLoopX, dwLoopY, RGB(byNewPixel, byNewPixel, byNewPixel));
				} // for
			} // for

			::SelectObject(hMemDC1, hOldBmp1);
			::SelectObject(hMemDC2, hOldBmp2);

			csGrayII.hbmMask = csII.hbmMask;

			csGrayII.fIcon = TRUE;
			hGrayIcon = ::CreateIconIndirect(&csGrayII);
		} // if

		::DeleteObject(csGrayII.hbmColor);
		//::DeleteObject(csGrayII.hbmMask);
	} // if

	::DeleteObject(csII.hbmColor);
	::DeleteObject(csII.hbmMask);
	::DeleteDC(hMemDC1);
	::DeleteDC(hMemDC2);
	::ReleaseDC(m_hWnd, hMainDC);

	return hGrayIcon;
} // End of CreateGrayscaleIcon

void CBioButton::SetTextColor(COLORREF clrIn, COLORREF clrOut, COLORREF clrFocu)
{
	m_clrColors[BTN_COLOR_TXTIN] = clrIn;
	if (clrOut == NULL)
		m_clrColors[BTN_COLOR_TXTOUT] = clrIn;
	else
		m_clrColors[BTN_COLOR_TXTOUT] = clrOut;

	if (clrFocu == NULL)
		m_clrColors[BTN_COLOR_TXTFOCUS] = clrIn;
	else
		m_clrColors[BTN_COLOR_TXTFOCUS] = clrFocu;
}

/**
 * 设置字体的大小
 * \param lHeight 
 * \param lWeight 
 */
void CBioButton::SetFontSize(long lHeight, long lWeight)
{
	LOGFONT lf;
	CFont* pFont = GetFont();
	if (pFont != NULL)
	{
		pFont->GetLogFont(&lf);
	}
	else
	{
		memset(&lf, 0, sizeof(LOGFONT));
		_tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("微软雅黑"), 22);
	}

	lf.lfHeight = lHeight;
	lf.lfWeight = lWeight;
	if (m_pFontText == NULL)
		m_pFontText = new CFont;

	m_pFontText->CreateFontIndirect(&lf);
}

void CBioButton::SetFontPoint(int nPoint, CString strFont)
{
	if (m_pFontText == NULL)
	{
		m_pFontText = new CFont;
	}
	else
	{
		m_pFontText->DeleteObject();
	}

	m_pFontText->CreatePointFont(nPoint, strFont);
}

void CBioButton::SetGradualColor( COLORREF clrTop, COLORREF clrBottom )
{
	if(clrTop == clrBottom) return;

	m_clrTop = clrTop;
	m_clrBottom = clrBottom;
	m_bGradualColor = TRUE;

	if (GetSafeHwnd()) {
		Invalidate();
	}
}

void CBioButton::DrawGradualBackground( CDC* pDC, CRect& rcItem )
{
	if(!m_bGradualColor) return;
	rcItem.DeflateRect(1, 1);

	BYTE topR = GetRValue(m_clrTop);
	BYTE topG = GetGValue(m_clrTop);
	BYTE topB = GetBValue(m_clrTop);

	BYTE bottomR = GetRValue(m_clrBottom);
	BYTE bottomG = GetGValue(m_clrBottom);
	BYTE bottomB = GetBValue(m_clrBottom);

	double dRadius = (double)rcItem.Height() / 2.0;

	CRect rectangle;
	for(int i = 0; i < rcItem.Height(); i++)
	{
		if(m_nStyle != BTN_STYLE_CIRCLE)
		{
			if(m_nCornerRadius > 0 && m_nStyle == BTN_STYLE_THIN_EDGE)
			{
				if(i < m_nCornerRadius || (rcItem.Height() - i) <= m_nCornerRadius)
				{
					// 圆角部分
					int nOffsetY = 0;
					if(i < m_nCornerRadius)
					{
						nOffsetY = m_nCornerRadius - i - 1;
					}
					else
					{
						nOffsetY = m_nCornerRadius - (rcItem.Height() - i);
					}
					double dOffsetX = sqrt(pow((float)m_nCornerRadius, 2) - pow((float)nOffsetY, 2));
					rectangle.SetRect(int(rcItem.left + m_nCornerRadius - dOffsetX + 0.5), rcItem.top + i, int(rcItem.right - m_nCornerRadius + dOffsetX - 0.5), rcItem.top + i + 1);
				}
				else
				{
					rectangle.SetRect(rcItem.left, rcItem.top + i, rcItem.right, rcItem.top + i + 1);
				}
			}
			else
			{
				rectangle.SetRect(rcItem.left, rcItem.top + i, rcItem.right, rcItem.top + i + 1);
			}
		}
		else
		{
			CPoint centerPoint = rcItem.CenterPoint();
			double offsetY = 0;
			if(i < dRadius)
			{
				offsetY = dRadius - 1 - i;
			}
			else
			{
				offsetY = i - dRadius;
			}
			double offsetX = sqrt(pow(dRadius, 2) - offsetY * offsetY);

			rectangle.SetRect(int(centerPoint.x - offsetX + 0.5), rcItem.top + i, int(centerPoint.x + offsetX + 0.5), rcItem.top + i + 1);
		}
		pDC->FillSolidRect(rectangle, 
					RGB(topR + (double)(bottomR - topR) / (double)(rcItem.Height() - 1) * i, 
					topG + (double)(bottomG - topG) / (double)(rcItem.Height() - 1) * i, 
					topB + (double)(bottomB - topB) / (double)(rcItem.Height() - 1) * i ));
	}
}

void CBioButton::SetUser3DColor( COLORREF clrTopLeft, COLORREF clrRightBottom )
{
	m_bUser3DColor = TRUE;
	m_clrUser3DTopLeft = clrTopLeft;
	m_clrUser3DRightBottom = clrRightBottom;

	if (GetSafeHwnd()) {
		Invalidate();
	}
}

void CBioButton::SetButtonThinEDGE( BOOL bRedraw/*=TRUE*/ )
{
	m_nStyle = BTN_STYLE_THIN_EDGE;

	if (GetSafeHwnd() && bRedraw) {
		Invalidate();
	}
}

void CBioButton::SetCustomNumber( int nNumber )
{
	m_nCunstomNumber = nNumber;
	if (GetSafeHwnd()) {
		Invalidate();
	}
}

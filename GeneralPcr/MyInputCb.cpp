#include "StdAfx.h"
#include "MyInputCb.h"

IMPLEMENT_DYNAMIC(CMyInputCb,CMyNewEdit)

CMyInputCb::CMyInputCb(CWnd* pParent /*= NULL*/) : CMyNewEdit(pParent)
{
	m_eCbSelWndState = EICSWS_HIDE;
	m_iSelWndH = 100;
	m_bFstDown = TRUE;
}

CMyInputCb::~CMyInputCb(void)
{
	
}

BEGIN_MESSAGE_MAP(CMyInputCb, CMyNewEdit)
	 ON_MESSAGE(WM_ROWSELECTCHANGE,&CMyInputCb::OnMsgRowSelectedChange) 
	 ON_MESSAGE(WM_HIDESONCTRL,&CMyInputCb::OnMsgHideSonCtrl) 
	 ON_WM_KILLFOCUS()
	 ON_WM_SIZE()
	 ON_MESSAGE(WM_DELETE_COMBO_ROW_STRING,&CMyInputCb::OnMsgDeleteRowString)
	 ON_MESSAGE(WM_RESET_WINDOW_SIZE,&CMyInputCb::OnMsgResetWindowSize)
END_MESSAGE_MAP()

void CMyInputCb::OnPaint()
{
	CPaintDC dc(this); 

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());

	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	Color clrFrame,clrMainWndBk;
	COLORREF clrTx,clrTxSel,clrBkSel;
	switch(m_eCtrlState)
	{
	case ECS_NORMAL: clrFrame = m_clrFrameNormal;
		clrTx = m_clrTxNormal;
		clrTxSel = m_clrTxSelNormal;
		clrBkSel = m_clrBkSelNormal;
		clrMainWndBk = m_clrMainWndNormal;	break;
	case ECS_HOVER: clrFrame = m_clrFrameHover;
		clrTx = m_clrTxHover;
		clrTxSel = m_clrTxSelHover;
		clrBkSel = m_clrBkSelHover;
		clrMainWndBk = m_clrMainWndHover;	break;
	case ECS_SELECTED: clrFrame = m_clrFrameSelected;
		clrTx = m_clrTxSelected;
		clrTxSel = m_clrTxSelSelected;
		clrBkSel = m_clrBkSelSelected;
		clrMainWndBk = m_clrMainWndSelected;	break;
	case ECS_DISABLE: clrFrame = m_clrFrameDisabled;
		clrTx = m_clrTxDisabled;
		clrTxSel = m_clrTxSelDisabled;
		clrBkSel = m_clrBkSelDisabled;
		clrMainWndBk = m_clrMainWndDisabled;	break;
	}

	if(EFS_RECTANGLE == m_eFrameStyle)
	{
		CPen pen;
		pen.CreatePen(PS_SOLID,m_pxFrameW,RGB(clrFrame.GetR(),clrFrame.GetG(),clrFrame.GetB()));
		CPen* pOldPen = memDC.SelectObject(&pen);
		memDC.Rectangle(&rcClient);
		memDC.SelectObject(&pOldPen);
		pen.DeleteObject();

		CRect rect = rcClient;
		rect.DeflateRect(m_pxFrameW,m_pxFrameW,m_pxFrameW,m_pxFrameW);
		memDC.FillSolidRect(&rect,RGB(clrMainWndBk.GetR(),clrMainWndBk.GetG(),clrMainWndBk.GetB()));
		memDC.SetBkMode(TRANSPARENT);
	}
	else
	{
		memDC.FillSolidRect(rcClient.left,rcClient.top,rcClient.Width(),rcClient.Height(),RGB(m_clrMainWndNormal.GetR(),m_clrMainWndNormal.GetG(),m_clrMainWndNormal.GetB()));
		DrawRoundRectangleNew(&memDC,rcClient.left,rcClient.top,rcClient.Width() - 1,rcClient.Height() - 1,m_iFrameRoundAngle,m_iFrameRoundAngle,m_iFrameRoundAngle,m_iFrameRoundAngle,TRUE,clrFrame,1,TRUE,clrMainWndBk,1);
	}

	if(_T("") != m_strText)
	{
		CheckLnkCharWidth();

		memDC.SetTextColor(clrTx);
		if(-1 == m_iCharStartShowIndex || -1 == m_iCharEndShowIndex)
		{
			m_iCharStartShowIndex = 0;
			m_iCharEndShowIndex = m_strText.GetLength() - 1;
			m_iCharShowLI = 0;
			m_iCharShowRI = 0;
		}
		m_iCdcW = GetWidth(m_iCharStartShowIndex,m_iCharEndShowIndex);

		if(m_iCdcW > 0)
		{
			CDC memDCText;
			memDCText.CreateCompatibleDC(&dc);
			CBitmap bitMapText;
			bitMapText.CreateCompatibleBitmap(&dc,m_iCdcW,rcClient.Height() - m_pxFrameW * 2);
			CBitmap* pOldBitmapText = memDCText.SelectObject(&bitMapText);

			CRect rcText(0,0,m_iCdcW,rcClient.Height() - m_pxFrameW * 2);
			memDCText.FillSolidRect(&rcText,RGB(clrMainWndBk.GetR(),clrMainWndBk.GetG(),clrMainWndBk.GetB()));
			memDCText.SetBkMode(TRANSPARENT);
			memDCText.SetTextColor(clrTx);
			CFont* pOldFont = memDCText.SelectObject(&m_fontSet);

			CSize size = memDCText.GetTextExtent(_T("A"));

			CString strToDraw = m_strText.Mid(m_iCharStartShowIndex,m_iCharEndShowIndex - m_iCharStartShowIndex + 1);
			memDCText.DrawText(strToDraw,strToDraw.GetLength(),&rcText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			memDCText.SelectObject(pOldFont);

			if(m_iSelCharNum > 0)
			{
				int iXBkStart = 0,iXBkEnd = m_iCdcW;
				int iSelStart = m_iSelStart,iSelEnd = m_iSelEnd,iTemp = m_iSelStart;
				int iSelStartDraw = m_iCharStartShowIndex,iSelEndDraw = m_iCharEndShowIndex;
				if(iSelStart > iSelEnd)
				{
					iSelStart = iSelEnd;
					iSelEnd = iTemp;
				}
				if(iSelStart >= m_iCharStartShowIndex)
				{
					iXBkStart = GetWidth(m_iCharStartShowIndex,iSelStart);
					iSelStartDraw = iSelStart + 1;
				}
				if(iSelEnd <= m_iCharEndShowIndex)
				{
					iXBkEnd = GetWidth(m_iCharStartShowIndex,iSelEnd);
					iSelEndDraw = iSelEnd;
				}

				if(iXBkEnd > iXBkStart)
				{
					m_iSelDrawCDcW = GetWidth(iSelStartDraw,iSelEndDraw);

					CDC memDCSel;
					memDCSel.CreateCompatibleDC(&dc);

					CRect rcSelDraw(0,0,m_iSelDrawCDcW,size.cy);

					CBitmap bitMapSel;
					bitMapSel.CreateCompatibleBitmap(&dc,rcSelDraw.Width(),rcSelDraw.Height());
					CBitmap* pOldBitmapSelDraw = memDCSel.SelectObject(&bitMapSel);

					memDCSel.FillSolidRect(&rcSelDraw,clrBkSel);

					CFont* pOldFontSel = memDCSel.SelectObject(&m_fontSet);
					memDCSel.SetTextColor(clrTxSel);
					memDCSel.SetBkMode(TRANSPARENT);

					CString strSelDraw(_T(""));
					strSelDraw = m_strText.Mid(iSelStartDraw,iSelEndDraw - iSelStartDraw + 1);
					memDCSel.DrawText(strSelDraw,strSelDraw.GetLength(),&rcSelDraw,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
					memDCSel.SelectObject(pOldFontSel);

					memDCText.BitBlt(iXBkStart,(rcText.Height() - size.cy) / 2,rcSelDraw.Width(),rcSelDraw.Height(),&memDCSel,0,0,SRCCOPY);

					memDCSel.SelectObject(pOldBitmapSelDraw);
					bitMapSel.DeleteObject();
					memDCSel.DeleteDC();
				}

			}

			memDC.BitBlt(m_pxFrameW + m_iLeftIndent,m_pxFrameW,m_iCharShowW,rcClient.Height() - m_pxFrameW * 2,&memDCText,m_iCharShowLI,0,SRCCOPY); 

			memDCText.SelectObject(pOldBitmapText);
			bitMapText.DeleteObject();
			memDCText.DeleteDC();
		}
	}

	//»æÖÆ¼ýÍ·
	Graphics graphics(memDC.m_hDC);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	m_arrowInfo.CalcPtData(m_rcRightIndent);

	if(EAS_FILL == m_arrowInfo.m_eArrowStyle)
	{
		SolidBrush brush(m_arrowInfo.m_clrArrow);
		graphics.FillPolygon(&brush,m_arrowInfo.m_aryPtTriangle,3,FillModeAlternate);
	}
	else
	{
		if(EICSWS_SHOW == m_eCbSelWndState)
		{
			Pen pen(m_arrowInfo.m_clrLineDropDown,m_arrowInfo.m_iLineW);
			graphics.DrawLines(&pen,m_arrowInfo.m_aryPtTriangleDropDown,3);
		}
		else
		{
			Pen pen(m_arrowInfo.m_clrLineNormal,m_arrowInfo.m_iLineW);
			graphics.DrawLines(&pen,m_arrowInfo.m_aryPtTriangle,3);
		}
	}

	graphics.ReleaseHDC(memDC.m_hDC);

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);   

	memDC.SelectObject(pOldBitmap);
	bitMap.DeleteObject();
	memDC.DeleteDC();

	return;
}

int CMyInputCb::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CMyNewEdit::OnCreate(lpCreateStruct);
	m_selWnd.m_bTellParentRowSelChange = TRUE;
	m_selWnd.m_pParent = this;
	m_selWnd.Create(IDD_POPUPDLG,this);	
	return 1;
}

BOOL CMyInputCb::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:	return TRUE;
		case VK_ESCAPE:	return TRUE;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

int CMyInputCb::LBtDownSpecialProc()
{
	if(EICSWS_SHOW == m_eCbSelWndState)
	{
		m_selWnd.ShowWindow(SW_HIDE);
		m_eCbSelWndState = EICSWS_HIDE;
	}
	else
	{ 
		CRect rcTemp = m_rcCtrl;
		if(m_bFstDown)
		{
			m_selWnd.SetSelectedRow(m_strText);
			m_bFstDown = FALSE;
		}
		ClientToScreen(&rcTemp);
		m_selWnd.MoveWindow(rcTemp.left,rcTemp.bottom,m_rcCtrl.Width(),CalcSelWndHeight());
		m_selWnd.ShowWindow(SW_SHOW);
		m_eCbSelWndState = EICSWS_SHOW;
	}
	
	return 0;
}

int CMyInputCb::GetString(int iIndex,CString& strText)
{
	if(iIndex < 0)
	{
		return -1;
	}
	if(iIndex >= m_selWnd.m_rowNum)
	{
		return -2;
	}
	return m_selWnd.GetRowString(iIndex,strText);
}

int CMyInputCb::InsertString(int iIndex,CString strText,BOOL bRedraw /*= FALSE*/)
{
	return m_selWnd.InsertRow(iIndex,strText,bRedraw);
}

int CMyInputCb::DeleteString(int iIndex,BOOL bRedraw /*= FALSE*/)
{
	return m_selWnd.DeleteRow(iIndex,bRedraw);
}

int CMyInputCb::Clear(BOOL bRedraw /*= FALSE*/)
{
	DeleteAll(bRedraw);
	m_bFstDown = TRUE;
	return m_selWnd.DeleteAll();
}

LRESULT CMyInputCb::OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam)
{
	CString strText(_T(""));
	if(0 != m_selWnd.GetRowString(wParam,strText))
	{
		return 1;
	}

	SetText(strText);

	LBtDownSpecialProc();

	return 0;
}

LRESULT CMyInputCb::OnMsgHideSonCtrl(WPARAM wParam,LPARAM lParam)
{
	m_selWnd.ShowWindow(SW_HIDE);
	m_eCbSelWndState = EICSWS_HIDE;

	if(!wParam)	
	{
		m_ePreCtrlState = m_eCtrlState;
		if(m_bEidtable)
		{
			m_eCtrlState = ECS_NORMAL;

			if(m_bFuncEdit && IsWindowVisible())
			{
				CWnd* pParent = GetParent();
				if(NULL != pParent->GetSafeHwnd())
				{
					pParent->PostMessage(WM_FUNCEDITKILLFOCUS,m_iCtrlTag,0);
				}
				ShowWindow(SW_HIDE);
			}
		}
	}

	return 0;
}

void CMyInputCb::OnKillFocus(CWnd* pNewWnd)
{
	CWnd* pWnd = &m_selWnd;
	if(pNewWnd == pWnd)
	{
		SetFocus();
		return;
	}

	SetDropUp();
	CMyNewEdit::OnKillFocus(pNewWnd);
	
	return;
}

int CMyInputCb::CalcSelWndHeight()
{
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);

	if(m_selWnd.m_totalRowH < GeneCRDlg.Height() - rect.bottom)
	{
		m_iSelWndH = m_selWnd.m_totalRowH + m_selWnd.m_pxFrameW * 2;
	}
	else
	{
		m_iSelWndH = GeneCRDlg.Height() - rect.bottom - 5;
	}
	return m_iSelWndH;
}

int CMyInputCb::SetText(CString strText,BOOL bRedraw /*= TRUE*/,BOOL bForbidTellParent /*= FALSE*/)
{
	BOOL bDifferentText = (m_strText != strText);

	m_strText = strText;
	m_lnkCharW.Clear();
	int iCharNum = m_strText.GetLength();
	if(iCharNum > 0 && NULL != GetSafeHwnd())
	{
		CDC* pDc = GetDC();
		if(NULL != pDc)
		{
			CFont* pOldFont = pDc->SelectObject(&m_fontSet);
			CSize size;
			for(int i = 0;i < iCharNum;++i)
			{
				size = pDc->GetTextExtent(m_strText.Mid(i,1));
				int iCharW = size.cx;
				m_lnkCharW.AddTail(iCharW);
			}

			pDc->SelectObject(pOldFont);
			ReleaseDC(pDc);	
		}
	}

	if(0 == iCharNum)	
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}
	else if(m_lnkCharW.GetSize() == iCharNum)
	{
		m_iCharEndShowIndex = iCharNum - 1;
		m_iCharShowRI = 0;
		CalcCharEndStartShowIndex();
	}
	else
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;

		return -1;
	}

	m_caretInfo.m_iCurCharIndex = iCharNum - 1;
	if(-1 == m_caretInfo.m_iCurCharIndex)
	{
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
	}
	else
	{
		if(-1 != m_iCharStartShowIndex && -1 != m_iCharEndShowIndex)
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent + *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + GetWidth(m_iCharStartShowIndex + 1,m_caretInfo.m_iCurCharIndex);
		}
		else
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		}
	}

	if(EWS_READONLY != m_eWorkState)
	{
		CPoint pt;
		pt.x = m_caretInfo.m_pxXStart;
		pt.y = m_caretInfo.m_pxYStart;

		CWnd* pFocusWnd = GetFocus();
		if(NULL != pFocusWnd && m_hWnd == pFocusWnd->m_hWnd)
		{
			SetCaretPos(pt);
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	if(!bForbidTellParent && bDifferentText)
	{
		TellParentSetNewTextProcess();
	}

	return 0;
}

int CMyInputCb::SetCurSel(int iIndex,BOOL bRedraw /*= TRUE*/,BOOL bForbidTellParent /*= FALSE*/)
{
	if(iIndex < -1)
	{
		return -1;
	}

	if(iIndex >= m_selWnd.m_lnkItemForSelInfo.GetSize())
	{
		return -2;
	}

	CString strTemp(_T(""));
	if(-1 != iIndex)
	{
		strTemp = m_selWnd.m_lnkItemForSelInfo[iIndex]->m_strText;
	}

	if(0 == SetText(strTemp,bRedraw,bForbidTellParent))
	{
		return 0;
	}
	else
	{
		return -3;
	}
}

int CMyInputCb::GetCurSel()
{
	return m_selWnd.GetFirstSelectedRow(m_strText);
}

int CMyInputCb::SetDropDown()
{
	if(EICSWS_SHOW == m_eCbSelWndState)
	{
		return 1;
	}
	else
	{ 
		CRect rcTemp = m_rcCtrl;
		if(m_bFstDown)
		{
			m_selWnd.SetSelectedRow(m_strText);
			m_bFstDown = FALSE;
		}
		ClientToScreen(&rcTemp);
		m_selWnd.MoveWindow(rcTemp.left,rcTemp.bottom,m_rcCtrl.Width(),CalcSelWndHeight());
		m_selWnd.ShowWindow(SW_SHOW);
		m_eCbSelWndState = EICSWS_SHOW;
	}

	return 0;
}

int CMyInputCb::SetDropUp()
{
	if(EICSWS_SHOW == m_eCbSelWndState)
	{
		m_selWnd.ShowWindow(SW_HIDE);
		m_eCbSelWndState = EICSWS_HIDE;
	}
	else
	{ 
		return 1;
	}

	return 0;
}

LRESULT CMyInputCb::OnMsgDeleteRowString(WPARAM wParam,LPARAM lParam)
{
	CString strText = GetText();
	CString strDelete;
	m_selWnd.GetRowString(wParam,strDelete);
	if (strText == strDelete)
	{
		SetText(_T(""));
	}
	GetParent()->SendMessage(WM_DELETE_COMBO_ROW_STRING, wParam, lParam);
	return 0;
}

void CMyInputCb::EnableRowDelete(BOOL bEnable)
{
	m_selWnd.SetEnableDeleteString(bEnable);
}

LRESULT CMyInputCb::OnMsgResetWindowSize(WPARAM wParam,LPARAM lParam)
{
	SetDropUp();
	SetDropDown();
	return 0;
}

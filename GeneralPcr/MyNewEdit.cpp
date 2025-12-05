
#include "stdafx.h"
#include "MyNewEdit.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CMyNewEdit, CEdit)

CMyNewEdit::CMyNewEdit(CWnd* pParent /*=NULL*/)
{
	m_strText = _T("");
	m_iLeftIndent = 16;
	m_iRightIndent = 16;
	m_iTopIndent = 6;
	m_iBottomIndent = 7;
	m_iCharShowLI = 0;
	m_iCharShowRI = 0;
	m_iCharShowW = 0;
	m_pxFrameW = 1;
	m_iCharStartShowIndex = -1;
	m_iCharEndShowIndex = -1;
	m_iCdcW = 0;
	m_iSelDrawCDcW = 0;
	m_iSelStart = -1;
	m_iSelEnd = -1;
	m_iSelCharNum = 0;
	m_bLBtPushedMove = FALSE;
	m_eCurSMT = ESMT_NONE;
	m_ePreSMT = ESMT_NONE;
	m_bSelMoveCaretHid = FALSE;
	m_bEidtable = TRUE;
	m_bMouseFstInCtrl = FALSE;
	m_bEnterTellParent = FALSE;
	m_eCtrlState = ECS_NORMAL;
	m_ePreCtrlState = ECS_NORMAL;

	m_clrFrameNormal = RGB(217,217,217);
	m_clrFrameHover = RGB(96,161,188);
	m_clrFrameSelected = RGB(2,91,199);
	m_clrFrameDisabled = RGB(209,209,209);

	m_clrMainWndNormal = RGB(255,255,255);
	m_clrMainWndHover = RGB(255,255,255); 
	m_clrMainWndSelected = RGB(255,255,255);
	m_clrMainWndDisabled = RGB(246,246,246);

	m_clrTxNormal = RGB(0X56,0X56,0X56);
	m_clrTxHover = RGB(0X56,0X56,0X56);
	m_clrTxSelected = RGB(0X56,0X56,0X56);
	m_clrTxDisabled = RGB(0X54,0X54,0X54);

	m_clrTxSelNormal = RGB(0X39,0X45,0X65);
	m_clrTxSelHover = RGB(0X39,0X45,0X65);
	m_clrTxSelSelected = RGB(0X39,0X45,0X65);
	m_clrTxSelDisabled = RGB(0X97,0X97,0X97);

	m_clrBkSelNormal = RGB(0XFF,0XFF,0XFF);
	m_clrBkSelHover = RGB(0XFF,0XFF,0XFF);
	m_clrBkSelSelected = RGB(0X7E,0XD3,0XDB /*210,251,255*/);
	m_clrBkSelDisabled = RGB(0XF7,0XF7,0XF7);
	m_eWorkState = EWS_NORMAL;
	m_eRgnType = ERT_EMPTY;

	m_bTellParentTxChanged = FALSE;
	m_iTextTypeTag = 1;
	m_bTellParentSetNewText = FALSE;
	m_bFuncEdit = FALSE;

	m_eFrameStyle = EFS_ROUNDRECT;
	m_iFrameRoundAngle = 4;
	m_iCtrlTag = 0;

	m_bShowLeftImg = FALSE;
	m_iLeftImgSonPicNum = 1;
	m_strLeftImgPath.Empty();

	m_iPWDotRadius = 2;
	m_iPWDotDiam = 4;
	m_iPWDotInterval = 4;
	m_iPWCharW = m_iPWDotDiam + m_iPWDotInterval + 1;	//1是预留插入符宽度
	m_eTxMode = ETM_NORMAL;

	m_iTxH = 14;
}

CMyNewEdit::~CMyNewEdit()
{
	
}

void CMyNewEdit::DoDataExchange(CDataExchange* pDX)
{
	CEdit::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyNewEdit, CEdit)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()
  
void CMyNewEdit::OnPaint()
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
	
	//绘制边框
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
	
	//绘制左侧图标
	if(m_bShowLeftImg && !m_strLeftImgPath.IsEmpty() && m_iLeftImgSonPicNum > 0)
	{
		CRect rcImg(m_pxFrameW,m_pxFrameW,m_iLeftIndent + m_pxFrameW,m_rcCtrl.bottom - m_pxFrameW);

		//绘制背景图片
		Graphics graphics(memDC.m_hDC);
		ImageAttributes ImgAtt;
		ImgAtt.SetWrapMode(WrapModeTileFlipXY);
		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
		graphics.SetSmoothingMode(SmoothingModeAntiAlias);

		Image img(m_strLeftImgPath);
		int iTempInt1 = 0;
		int iIconW = img.GetWidth() / m_iLeftImgSonPicNum;

		Rect rcPic;
		if(rcImg.Width() <= iIconW)
		{
			rcPic.X = 0;
			rcPic.Width = rcImg.Width();
		}
		else
		{
			rcPic.X = rcImg.left + (rcImg.Width() - iIconW) / 2;
			rcPic.Width = iIconW;
		}
		if(rcImg.Height() <= img.GetHeight())
		{
			rcPic.Y = 0;
			rcPic.Height = rcImg.Height();
		}
		else
		{
			rcPic.Y = rcImg.top + (rcImg.Height() - img.GetHeight()) / 2;
			rcPic.Height = img.GetHeight();
		}

		int iStartX = 0;
		if(m_iLeftImgSonPicNum > 1)
		{
			if(ECS_HOVER == m_eCtrlState || ECS_SELECTED == m_eCtrlState)
			{
				iStartX = iIconW;
			}
		}
		
		graphics.DrawImage(&img,rcPic,iStartX,0,iIconW,img.GetHeight(),UnitPixel,&ImgAtt);

		graphics.ReleaseHDC(memDC.m_hDC);
	}

	//绘制文字
	if(!m_strText.IsEmpty())
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

			CString strToDraw = m_strText.Mid(m_iCharStartShowIndex,m_iCharEndShowIndex - m_iCharStartShowIndex + 1);
		
			if(ETM_PASSWORDDOT == m_eTxMode)
			{
				Graphics graphics(memDCText.GetSafeHdc());
				graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
				SolidBrush brush(Color(0X39,0X45,0X65));
				Pen pen(Color(0X39,0X45,0X65),1);

				int iDotStartX = 1,iDotStartY = (rcClient.Height() - m_pxFrameW * 2 - m_iPWDotDiam) / 2;
				if(iDotStartY < 0)
				{
					iDotStartY = 0;
				}
				int iDrawTxNum = strToDraw.GetLength();
				for(int i = 0;i < iDrawTxNum;++i)
				{
					GraphicsPath roundRectPath;
					roundRectPath.AddEllipse(iDotStartX,iDotStartY,m_iPWDotDiam,m_iPWDotDiam);
					graphics.FillPath(&brush,&roundRectPath);
					graphics.DrawPath(&pen,&roundRectPath);

					iDotStartX += m_iPWCharW;
				}

				graphics.ReleaseHDC(memDCText.m_hDC);
			}
			else
			{
				memDCText.SetBkMode(TRANSPARENT);
				memDCText.SetTextColor(clrTx);
				CFont* pOldFont = memDCText.SelectObject(&m_fontSet);
				memDCText.DrawText(strToDraw,strToDraw.GetLength(),&rcText,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
				memDCText.SelectObject(pOldFont);
			}

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

					CRect rcSelDraw(0,0,m_iSelDrawCDcW,m_logFont.lfHeight);

					CBitmap bitMapSel;
					bitMapSel.CreateCompatibleBitmap(&dc,rcSelDraw.Width(),rcSelDraw.Height());
					CBitmap* pOldBitmapSelDraw = memDCSel.SelectObject(&bitMapSel);

					memDCSel.FillSolidRect(&rcSelDraw,clrBkSel);

					CString strSelDraw(_T(""));
					strSelDraw = m_strText.Mid(iSelStartDraw,iSelEndDraw - iSelStartDraw + 1);

					if(ETM_PASSWORDDOT == m_eTxMode)
					{
						Graphics graphics(memDCSel.GetSafeHdc());
						graphics.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
						SolidBrush brush(Color(0X39,0X45,0X65));
						Pen pen(Color(0X39,0X45,0X65),1);

						int iDotStartX = 1,iDotStartY = (rcSelDraw.Height() - m_iPWDotDiam) / 2;
						if(iDotStartY < 0)
						{
							iDotStartY = 0;
						}
						int iDrawTxNum = strSelDraw.GetLength();
						for(int i = 0;i < iDrawTxNum;++i)
						{
							GraphicsPath roundRectPath;
							roundRectPath.AddEllipse(iDotStartX,iDotStartY,m_iPWDotDiam,m_iPWDotDiam);
							graphics.FillPath(&brush,&roundRectPath);
							graphics.DrawPath(&pen,&roundRectPath);

							iDotStartX += m_iPWCharW;
						}

						graphics.ReleaseHDC(memDCSel.m_hDC);
					}
					else
					{
						CFont* pOldFontSel = memDCSel.SelectObject(&m_fontSet);
						memDCSel.SetTextColor(clrTxSel);
						memDCSel.SetBkMode(TRANSPARENT);
						memDCSel.DrawText(strSelDraw,strSelDraw.GetLength(),&rcSelDraw,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
						memDCSel.SelectObject(pOldFontSel);
					}

					memDCText.BitBlt(iXBkStart,(rcText.Height() - m_logFont.lfHeight) / 2,rcSelDraw.Width(),rcSelDraw.Height(),&memDCSel,0,0,SRCCOPY);

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
	
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);   

	memDC.SelectObject(pOldBitmap);
	bitMap.DeleteObject();
	memDC.DeleteDC();

	return;
}

BOOL CMyNewEdit::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyNewEdit::OnSize(UINT nType, int cx, int cy)
{
	CEdit::OnSize(nType, cx, cy);
	m_caretInfo.m_pxYStart = (cy - m_caretInfo.m_pxHeight) / 2;
	m_iCharShowW = cx - m_iLeftIndent - m_iRightIndent - 2 * m_pxFrameW;
	if(m_iCharShowW < 0)
	{
		m_iCharShowW = 0;
	}
	CalcCharStartEndShowIndex();

	m_rcCtrl = CRect(0,0,cx,cy);
	m_rcText = CRect(m_pxFrameW + m_iLeftIndent,m_pxFrameW + m_iTopIndent,cx - m_pxFrameW - m_iRightIndent,cy - m_pxFrameW - m_iBottomIndent);
	m_rcLeftIndent = CRect(m_pxFrameW,m_pxFrameW,m_pxFrameW + m_iLeftIndent,m_pxFrameW + m_iTopIndent);
	m_rcRightIndent = CRect(cx - m_pxFrameW - m_iRightIndent,m_pxFrameW,cx - m_pxFrameW,cy - m_pxFrameW);
	m_rcTopIndent = CRect(m_pxFrameW + m_iLeftIndent,m_pxFrameW,cx - m_pxFrameW - m_iRightIndent,m_pxFrameW + m_iTopIndent);
	m_rcBottomIndent = CRect(m_pxFrameW + m_iLeftIndent,cx - m_pxFrameW - m_iRightIndent,cx - m_pxFrameW - m_iRightIndent,cy - m_pxFrameW);
}

void CMyNewEdit::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bEidtable)
	{
		return;
	}

	if(EWS_READONLY == m_eWorkState)
	{
		return;
	}

	return;
}

void CMyNewEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bEidtable)
	{
		return;
	}

	if(EWS_READONLY == m_eWorkState || (EWS_NORMAL == m_eWorkState && ERT_RIGHTINDENT == m_eRgnType))
	{
		LBtDownSpecialProc();
		return;
	}

	CRect rect;
	GetClientRect(&rect);
	if(!rect.PtInRect(point))
	{
		return;
	}
	
	//刷新插入符行高
	m_caretInfo.m_pxHeight = /*m_iTxH*/m_logFont.lfHeight;
	m_caretInfo.m_pxYStart = (m_rcCtrl.Height() - m_caretInfo.m_pxHeight) / 2;

	CreateSolidCaret(m_caretInfo.m_pxWidth,m_caretInfo.m_pxHeight);
	TransformXPosToCharInex(point.x);
	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);
	ShowCaret();
	SetFocus();

	if(m_iSelCharNum > 0)
	{
		if(m_iSelStart > m_iSelEnd)
		{
			int iTemp = m_iSelStart;
			m_iSelStart = m_iSelEnd;
			m_iSelEnd = iTemp;
		}

		if(!(m_iSelStart >= m_iCharEndShowIndex || m_iSelEnd < m_iCharStartShowIndex))
		{
			Invalidate(FALSE);
		}
		m_iSelStart = -1;
		m_iSelEnd = -1;
		m_iSelCharNum = 0;
	}

	/*PVOID pOldValue = NULL;
	BOOL bRet = Wow64DisableWow64FsRedirection(&pOldValue);
	HINSTANCE hInstance = ShellExecuteW(NULL,_T("open"),_T("osk.exe"),NULL,NULL,SW_SHOWNORMAL);
	if(bRet)
	{
	Wow64RevertWow64FsRedirection(pOldValue);
	}
	pOldValue = NULL;
	DWORD dwRet = GetLastError();*/

	return;
}

void CMyNewEdit::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_bEidtable)
	{
		return;
	}

	if(EWS_READONLY == m_eWorkState)
	{
		return;
	}

	if(m_bLBtPushedMove)
	{
		m_bLBtPushedMove = FALSE;
		m_eCurSMT = ESMT_NONE;
		m_ePreSMT = ESMT_NONE;
		//ReleaseCapture();
	}
}

void CMyNewEdit::OnLButtonDblClk(UINT nFlags,CPoint point)
{
	if(!m_bEidtable)
	{
		return;
	}

	if(EWS_READONLY == m_eWorkState || (EWS_NORMAL == m_eWorkState && ERT_RIGHTINDENT == m_eRgnType))
	{
		LBtDownSpecialProc();
		return;
	}

	return;
}

void CMyNewEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bEidtable)
	{
		return;
	}

	GetRegionType(point);

	//处理鼠标形状
	HCURSOR hCursor;
	if((EWS_READONLY != m_eWorkState) && (ERT_EDIT == m_eRgnType || m_bLBtPushedMove))
	{
		hCursor = LoadCursorW(NULL,IDC_IBEAM);
	}
	else
	{
		hCursor = LoadCursorW(NULL,IDC_ARROW);
	}
	SetCursor(hCursor);

	if((EWS_READONLY != m_eWorkState) && (MK_LBUTTON & nFlags))
	{
		if(!m_bLBtPushedMove && m_rcText.PtInRect(point))
		{
			m_bLBtPushedMove = TRUE;
			m_iSelStart = m_caretInfo.m_iCurCharIndex;
			m_eCurSMT = ESMT_NORMAL;
			m_bSelMoveCaretHid = FALSE;
			SetCapture();
		}

		if(m_iSelCharNum > 0 && !m_bSelMoveCaretHid)
		{
			HideCaret();
			m_bSelMoveCaretHid = TRUE;
		}
		else if(m_iSelCharNum <= 0 && m_bSelMoveCaretHid)
		{
			ShowCaret();
			m_bSelMoveCaretHid = FALSE;
		}
		MouseLeftBtPushedMoveProcess(point);
		m_ptPreSelMove = point;
		m_ePreSMT = m_eCurSMT;
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		BOOL bInCtrl = rect.PtInRect(point);

		if(!m_bMouseFstInCtrl)
		{
			m_bMouseFstInCtrl = TRUE;
			SetCapture();
		}
		else
		{
			if(!bInCtrl)
			{
				m_bMouseFstInCtrl = FALSE;
 				ReleaseCapture();
			}
		}
		
		if(m_bMouseFstInCtrl)
		{
			if(ECS_SELECTED != m_eCtrlState)
			{
				m_eCtrlState = ECS_HOVER;
			}
		}
		else
		{
			if(ECS_SELECTED != m_eCtrlState)
			{
				m_eCtrlState = ECS_NORMAL;
			}
		}
		if(m_ePreCtrlState != m_eCtrlState)
		{
			Invalidate(FALSE);
			m_ePreCtrlState = m_eCtrlState;
		}
	}

	return;
}

BOOL CMyNewEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_bEidtable)
	{
		return __super::OnMouseWheel(nFlags, zDelta, pt);
	}

	BOOL bSetCaret = TRUE;
	HWND hWndHaveFocus = ::GetFocus();
	if(NULL != hWndHaveFocus && hWndHaveFocus != m_hWnd)
	{
		bSetCaret = FALSE;;
	}

	if(EWS_READONLY == m_eWorkState)
	{
		bSetCaret = FALSE;;
	}	

	if(zDelta > 0)
	{
		HoriScrollPixel(-20,TRUE,bSetCaret);
	}
	else
	{
		HoriScrollPixel(20,TRUE,bSetCaret);
	}

	return TRUE;
}

int CMyNewEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_caretInfo.m_pxWidth *= m_dHoriRatio;
	CEdit::OnCreate(lpCreateStruct);
	return 1;
}

void CMyNewEdit::OnSetFocus(CWnd* pOldWnd)
{
	m_ePreCtrlState = m_eCtrlState;
	if(m_bEidtable)
	{
		m_eCtrlState = ECS_SELECTED;
	}
	
	if(m_ePreCtrlState != m_eCtrlState)
	{
		Invalidate(FALSE);
	}

	return;
}

void CMyNewEdit::OnKillFocus(CWnd* pNewWnd)
{
	m_ePreCtrlState = m_eCtrlState;
	if(m_bEidtable)
	{
		m_eCtrlState = ECS_NORMAL;

		if(m_bFuncEdit)
		{
			CWnd* pParent = GetParent();
			if(NULL != pParent->GetSafeHwnd() && IsWindowVisible())
			{
				pParent->PostMessage(WM_FUNCEDITKILLFOCUS,m_iCtrlTag,0);
			}

			if(IsWindowVisible())
			{
				ShowWindow(SW_HIDE);
			}
		}
	}

	CEdit::OnKillFocus(pNewWnd);
	HideCaret();

	return;
}

void CMyNewEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(EWS_READONLY == m_eWorkState)
	{
		return;
	}
	
	if(!m_bEidtable)
	{
		return;
	}

	if(m_bLBtPushedMove)	//选中滑动时不处理字符消息
	{
		return;
	}
	if(0 == CharFilter(nChar,nFlags))	
	{
		return;
	}

	if(m_iSelCharNum > 0)
	{
		DeleteSelectedChar(FALSE);
	}

	CDC* pDc = GetDC();
	if(NULL == pDc)
	{
		return;
	}

	if(m_caretInfo.m_iCurCharIndex < -1)
	{
		AfxMessageBox(_T("CMyNewEdit错误1"));
	}
	else
	{ 
		m_strText.Insert(++m_caretInfo.m_iCurCharIndex,(TCHAR)nChar);
	}

	CFont* pOldFont = pDc->SelectObject(&m_fontSet);

	CString strTemp(_T(""));
	strTemp = (TCHAR)nChar;
	CSize size = pDc->GetTextExtent(strTemp);
	int iCharW = size.cx;
	if(ETM_PASSWORDDOT == m_eTxMode)
	{
		iCharW = m_iPWCharW;
	}
	m_lnkCharW.InsertAtPre(m_caretInfo.m_iCurCharIndex,iCharW);

	CalcCharStartEndShowIndex();

	SetCharFullShow(m_caretInfo.m_iCurCharIndex);
	CalcCaretPos(m_caretInfo.m_iCurCharIndex,m_caretInfo.m_pxXStart);
	
	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	pDc->SelectObject(pOldFont);
	ReleaseDC(pDc);

	Invalidate(FALSE);
	
	TellParentTxChangedProcess();

	return;
}

void CMyNewEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(EWS_READONLY == m_eWorkState)
	{
		return;
	}

	if(!m_bEidtable)
	{
		return;
	}

	if(0 == ArrowKeyProcess(nChar))
	{
		return;
	}

	if(0 == DeleteKeyProcess(nChar))
	{
		TellParentTxChangedProcess();
		return;
	}

	return;
}

void CMyNewEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(EWS_READONLY == m_eWorkState)
	{
		return;
	}

	if(!m_bEidtable)
	{
		return;
	}

	return;
}

BOOL CMyNewEdit::PreTranslateMessage(MSG* pMsg)
{
	if(0 == MsgFilter(pMsg))
	{
		return TRUE;
	}

	if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			if(m_bEnterTellParent && NULL != GetParent()->GetSafeHwnd())
			{
				//ShowWindow(SW_HIDE);
				GetParent()->PostMessage(WM_EDITENTERPUSHED,0,0);
			}
			return TRUE;
		case VK_ESCAPE:	return TRUE;
		}
	}

	if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
			if(m_bEnterTellParent && NULL != GetParent()->GetSafeHwnd())
			{
				GetParent()->PostMessage(WM_EDITENTERPUSHED,0,0);
			}
			return TRUE;
		case VK_ESCAPE:	return TRUE;
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

int CMyNewEdit::SetCharFullShow(int iIndex)
{
	if(-1 == iIndex)
	{
		iIndex = 0;
	}
	if(iIndex < 0)
	{
		return -1;
	}
	
	if(iIndex >= m_strText.GetLength())
	{
		return -2;
	}

	if(iIndex > m_iCharStartShowIndex && iIndex < m_iCharEndShowIndex)
	{
		return 1;
	}

	if(iIndex <= m_iCharStartShowIndex)
	{
		m_iCharShowLI = 0;
		m_iCharStartShowIndex = iIndex;
		CalcCharStartEndShowIndex();
	}
	else if(iIndex == m_iCharEndShowIndex)
	{
		if(m_iCharEndShowIndex < m_strText.GetLength() - 1)
		{
			m_iCharShowRI = 0;
			CalcCharEndStartShowIndex();
		}
		else
		{
			if(0 != m_iCharShowRI)
			{
				m_iCharShowRI = 0;
				CalcCharEndStartShowIndex();
			}
		}
	}
	else
	{
		m_iCharShowRI = 0;
		m_iCharEndShowIndex = iIndex;
		CalcCharEndStartShowIndex();
	}

	return 0;
}

int CMyNewEdit::CalcCaretPos(int iIndex,int& iCaretXPos)
{
	iCaretXPos = m_pxFrameW + m_iLeftIndent;
	if(iIndex < 0)
	{
		return -1;
	}
	if(iIndex >= m_strText.GetLength())
	{
		return -2;
	}

	if(-1 != m_iCharStartShowIndex && -1 != m_iCharEndShowIndex)
	{
		iCaretXPos = *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + m_iLeftIndent + m_pxFrameW;
	}
	
	for(int i = m_iCharStartShowIndex + 1;i <= iIndex;++i)
	{
		iCaretXPos += *m_lnkCharW[i];
	}
	return 0;
}

void CMyNewEdit::ClearSelectInfo()
{
	m_iSelStart = -1;
	m_iSelEnd = -1;
	m_iSelCharNum = 0;
	m_bLBtPushedMove = FALSE;
	m_bSelMoveCaretHid = FALSE;
}

void CMyNewEdit::SetBeEditable(BOOL bEditable,BOOL bRedraw /*= FALSE*/)
{
	if(bEditable == m_bEidtable)
	{
		return;
	}

	m_bEidtable = bEditable;
	m_ePreCtrlState = m_eCtrlState;
	if(bEditable)
	{
		m_eCtrlState = ECS_NORMAL;
	}
	else
	{
		m_eCtrlState = ECS_DISABLE;
	}
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyNewEdit::GetBeEditable()
{
	return m_bEidtable;
}

int CMyNewEdit::SetAllSelected()
{
	int iCharNum = m_strText.GetLength();
	if(iCharNum < 1)
	{
		return 1;
	}

	m_iSelCharNum = iCharNum;
	m_iSelStart = -1;
	m_iSelEnd = iCharNum - 1;

	Invalidate(FALSE);
	return 0;
}

int CMyNewEdit::DeleteAll(BOOL bRedraw /*= TRUE*/)
{
	if(_T("") == m_strText)
	{
		return 1;
	}

	m_strText = _T("");
	ClearSelectInfo();

	m_caretInfo.m_iCurCharIndex = -1;
	m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
	m_iCharStartShowIndex = -1;
	m_iCharEndShowIndex = -1;
	m_iCharShowLI = 0;
	m_iCharShowRI = 0;

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	
	TellParentTxChangedProcess();

	return 0;
}

int CMyNewEdit::SetFrameColor(Color clrNormal,Color clrHover,Color clrSelected,Color clrDisabled,BOOL bRedraw /*= FALSE*/)
{
	m_clrFrameNormal = clrNormal;
	m_clrFrameHover = clrHover;
	m_clrFrameSelected = clrSelected;
	m_clrFrameDisabled = clrDisabled;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyNewEdit::SetMainWndBkColor(Color clrNormal,Color clrHover,Color clrSelected,Color clrDisabled,BOOL bRedraw /*= FALSE*/)
{
	m_clrMainWndNormal = clrNormal;
	m_clrMainWndHover = clrHover;
	m_clrMainWndSelected = clrSelected;
	m_clrMainWndDisabled = clrDisabled;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyNewEdit::SetTextColor(COLORREF clrNormal,COLORREF clrHover,COLORREF clrSelected,COLORREF clrDisabled,BOOL bRedraw /*= FALSE*/)
{
	m_clrTxNormal = clrNormal;
	m_clrTxHover = clrHover;
	m_clrTxSelected = clrSelected;
	m_clrTxDisabled = clrDisabled;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyNewEdit::SetSelectedTextColor(COLORREF clrNormal,COLORREF clrHover,COLORREF clrSelected,COLORREF clrDisabled,BOOL bRedraw /*= FALSE*/)
{
	m_clrTxSelNormal = clrNormal;
	m_clrTxSelHover = clrHover;
	m_clrTxSelSelected = clrSelected;
	m_clrTxSelDisabled = clrDisabled;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyNewEdit::SetSelectedBkColor(COLORREF clrNormal,COLORREF clrHover,COLORREF clrSelected,COLORREF clrDisabled,BOOL bRedraw /*= FALSE*/)
{
#ifdef CAN_YOUNG
	m_clrBkSelNormal = ED_SELECT_BK;
	m_clrBkSelHover = ED_SELECT_BK;
	m_clrBkSelSelected = ED_SELECT_BK;
	m_clrBkSelDisabled = ED_SELECT_BK;
#else
	m_clrBkSelNormal = clrNormal;
	m_clrBkSelHover = clrHover;
	m_clrBkSelSelected = clrSelected;
	m_clrBkSelDisabled = clrDisabled;
#endif

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyNewEdit::SetText(CString strText,BOOL bRedraw /*= TRUE*/,BOOL bForbidTellParent /*= FALSE*/)
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
				if(ETM_PASSWORDDOT == m_eTxMode)
				{
					iCharW = m_iPWCharW;
				}
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

	if(bDifferentText && !bForbidTellParent)
	{
		TellParentSetNewTextProcess();
	}

	return 0;
}

int CMyNewEdit::SetText(double dData,BOOL bRedraw /*= TRUE*/,BOOL bForbidTellParent /*= FALSE*/)
{
	CString strText(_T(""));
	strText.Format(_T("%.2lf"),dData);

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
				if(ETM_PASSWORDDOT == m_eTxMode)
				{
					iCharW = m_iPWCharW;
				}
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

	if(bDifferentText && !bForbidTellParent)
	{
		TellParentSetNewTextProcess();
	}

	return 0;
}

CString CMyNewEdit::GetText()
{
	return m_strText;
}

void CMyNewEdit::ClearMaxMinLimit()
{
	m_maxMinInfo.ClearMaxMinLimit();
}

void CMyNewEdit::SetMaxMinInfo(CMaxMinInfo maxMinInfo,BOOL bRedraw /*= FALSE*/)
{
	m_maxMinInfo = maxMinInfo;
	return;
}

CMaxMinInfo CMyNewEdit::GetMaxMinInfo()
{
	return m_maxMinInfo;
}

void CMyNewEdit::SetBeEnterTellParent(BOOL bEnterTellParent)
{
	m_bEnterTellParent = bEnterTellParent;
}

BOOL CMyNewEdit::GetBeEnterTellParent()
{
	return m_bEnterTellParent;
}

void CMyNewEdit::SetTellParentTxChangedParam(BOOL bTellParentTxChanged,int iTextTypeTag)
{
	m_bTellParentTxChanged = bTellParentTxChanged;
	m_iTextTypeTag = iTextTypeTag;
}

void CMyNewEdit::GetTellParentTxChangedParam(BOOL& bTellParentTxChanged,int& iTextTypeTag)
{
	bTellParentTxChanged = m_bTellParentTxChanged;
	iTextTypeTag = m_iTextTypeTag;
}

void CMyNewEdit::SetTellParentSetNewText(BOOL bTellParentSetNewText,int iTextTypeTag)
{
	m_bTellParentSetNewText = bTellParentSetNewText;
	m_iTextTypeTag = iTextTypeTag;
}

void CMyNewEdit::GetTellParentSetNewText(BOOL& bTellParentSetNewText,int& iTextTypeTag)
{
	bTellParentSetNewText = m_bTellParentSetNewText;
	iTextTypeTag = m_iTextTypeTag;
}

void CMyNewEdit::SetWorkState(EWORKSTATE eWorkState)
{
	m_eWorkState = eWorkState;
	ClearSelectInfo();

	if(NULL != GetSafeHwnd())
	{
		HideCaret();
	}
	return;
}

EWORKSTATE CMyNewEdit::GetWorkState()
{
	return m_eWorkState;
}

void CMyNewEdit::SetFrameStyle(EFRAMESTYLE eFrameStyle,BOOL bRedraw /*= FALSE*/)
{
	m_eFrameStyle = eFrameStyle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

EFRAMESTYLE CMyNewEdit::GetFrameStyle()
{
	return m_eFrameStyle;
}

void CMyNewEdit::SetFrameRoundAngle(int iFrameRoundAngle,BOOL bRedraw /*= FALSE*/)
{
	m_iFrameRoundAngle = iFrameRoundAngle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMyNewEdit::GetFrameRoundAngle()
{
	return m_iFrameRoundAngle;
}

void CMyNewEdit::SetLeftImageParam(BOOL bShowImg,int iSonPicNum,CString strFilePath,BOOL bRedraw /*= FALSE*/)
{
	m_bShowLeftImg = bShowImg;
	m_iLeftImgSonPicNum = iSonPicNum;
	m_strLeftImgPath = strFilePath;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyNewEdit::GetBeShowLeftImage()
{
	return m_bShowLeftImg;
}

int CMyNewEdit::GetLeftImageSonPicNum()
{
	return m_iLeftImgSonPicNum;
}

CString CMyNewEdit::GetLeftImagePath()
{
	return m_strLeftImgPath;
}

void CMyNewEdit::SetPasswordParam(ETEXTMODE eData,int iDotRadius,int iDotInterval)
{
	if(iDotRadius < 1 || iDotInterval < 0)
	{
		return;
	}

	m_eTxMode = eData;
	m_iPWDotRadius = iDotRadius;
	m_iPWDotDiam = iDotRadius * 2;
	m_iPWDotInterval = iDotInterval;
	m_iPWCharW = m_iPWDotDiam + m_iPWDotInterval + 1;
	return;
}

int CMyNewEdit::TextParamChanged(BOOL bRedraw /*= FALSE*/)
{
	CDC* pDc = GetDC();
	if(NULL == pDc)
	{
		return -1;
	}

	CFont* pOldFont = pDc->SelectObject(&m_fontSet);

	CString strTemp(_T(""));
	m_lnkCharW.Clear();

	int iCharNum = m_strText.GetLength();
	for(int i = 0;i < iCharNum;++i)
	{
		m_strText.GetAt(i);
		strTemp = (TCHAR)m_strText.GetAt(i);
		CSize size = pDc->GetTextExtent(strTemp);
		int iCharW = size.cx;
		m_lnkCharW.AddTail(iCharW);
	}

	m_caretInfo.m_pxHeight = m_logFont.lfHeight;
	m_caretInfo.m_pxYStart = (m_rcCtrl.Height() - m_caretInfo.m_pxHeight) / 2;
	CalcCaretPos(m_caretInfo.m_iCurCharIndex,m_caretInfo.m_pxXStart);

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	pDc->SelectObject(pOldFont);
	ReleaseDC(pDc);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::LBtDownSpecialProc()
{
	return 0;
}

int CMyNewEdit::CalcCharStartEndShowIndex()
{
	if(_T("") == m_strText || m_iCharShowW <= 0)
	{
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
		return 1;
	}

	int iCharNum = m_strText.GetLength();
	if(-1 == m_iCharStartShowIndex)
	{
		m_iCharStartShowIndex = 0;
		m_iCharShowLI = 0;
	}

	int iSumW = 0;
	int i = m_iCharStartShowIndex;
	for(;i < iCharNum;++i)
	{
		iSumW += *m_lnkCharW[i];
		if(i == m_iCharStartShowIndex)
		{
			iSumW -= m_iCharShowLI;
		}

		if(m_iCharShowW == iSumW)
		{
			m_iCharEndShowIndex = i;
			m_iCharShowRI = 0;
			break; 
		}
		else if(iSumW > m_iCharShowW)
		{
			m_iCharEndShowIndex = i;
			m_iCharShowRI = iSumW - m_iCharShowW;
			break;
		}
	}

	if(i >= iCharNum)
	{
		m_iCharEndShowIndex = iCharNum - 1;
		m_iCharShowRI = 0;
	}

	if(iSumW < m_iCharShowW)
	{
		int iEmptyW = m_iCharShowW - iSumW;
		int iLeftUnShow = GetWidth(0,m_iCharStartShowIndex - 1) + m_iCharShowLI;
		if(iLeftUnShow >= iEmptyW)
		{
			HoriScrollPixel(-iEmptyW);
		}
		else
		{
			HoriScrollPixel(-iLeftUnShow);
		}
	}

	return 0;
}

int CMyNewEdit::CalcCharStartEndShowIndex(int iStartIndex)
{
	if(_T("") == m_strText || m_iCharShowW <= 0)
	{
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
		return 1;
	}

	if(iStartIndex < 0)
	{
		return -1;
	}

	int iCharNum = m_strText.GetLength();
	if(iStartIndex >= iCharNum)
	{
		return -2;
	}

	m_iCharStartShowIndex = iStartIndex;
	m_iCharShowLI = 0;

	int iSumW = 0;
	int i = m_iCharStartShowIndex;
	for(;i < iCharNum;++i)
	{
		iSumW += *m_lnkCharW[i];
		if(i == m_iCharStartShowIndex)
		{
			iSumW -= m_iCharShowLI;
		}

		if(m_iCharShowW == iSumW)
		{
			m_iCharEndShowIndex = i;
			m_iCharShowRI = 0;
			break; 
		}
		else if(iSumW > m_iCharShowW)
		{
			m_iCharEndShowIndex = i;
			m_iCharShowRI = iSumW - m_iCharShowW;
			break;
		}
	}

	if(i >= iCharNum)
	{
		m_iCharEndShowIndex = iCharNum - 1;
		m_iCharShowRI = 0;
	}

	return 0;
}

int CMyNewEdit::CalcCharEndStartShowIndex()
{
	if(_T("") == m_strText || m_iCharShowW <= 0)
	{
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
		return 1;
	}

	int iCharNum = m_strText.GetLength();
	if(-1 == m_iCharEndShowIndex)
	{
		m_iCharEndShowIndex = iCharNum - 1;
		m_iCharShowRI = 0;
	}

	int iSumW = 0;
	int i = m_iCharEndShowIndex;
	for(;i >= 0;--i)
	{
		iSumW += *m_lnkCharW[i];
		if(i == m_iCharEndShowIndex)
		{
			iSumW -= m_iCharShowRI;
		}

		if(m_iCharShowW == iSumW)
		{
			m_iCharStartShowIndex = i;
			m_iCharShowLI = 0;
			break; 
		}
		else if(iSumW > m_iCharShowW)
		{
			m_iCharStartShowIndex = i;
			m_iCharShowLI = iSumW - m_iCharShowW;
			break;
		}
	}

	if(i < 0)
	{
		m_iCharStartShowIndex = 0;
		m_iCharShowLI = 0;
	}

	if(iSumW < m_iCharShowW)
	{
		int iEmptyW = m_iCharShowW - iSumW;
		int iRightUnShow = GetWidth(m_iCharEndShowIndex + 1,iCharNum - 1) + m_iCharShowRI;
		if(iRightUnShow >= iEmptyW)
		{
			HoriScrollPixel(iEmptyW);
		}
		else
		{
			HoriScrollPixel(iRightUnShow);
		}
	}

	return 0;
}

int CMyNewEdit::CalcCharEndStartShowIndex(int iEndIndex)
{
	if(_T("") == m_strText || m_iCharShowW <= 0)
	{
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
		return 1;
	}

	if(iEndIndex < 0)
	{
		return -1;
	}

	int iCharNum = m_strText.GetLength();
	if(iEndIndex >= iCharNum)
	{
		return -2;
	}

	m_iCharEndShowIndex = iEndIndex;
	m_iCharShowRI = 0;

	int iSumW = 0;
	int i = m_iCharEndShowIndex;
	for(;i > 0;--i)
	{
		iSumW += *m_lnkCharW[i];
		if(i == m_iCharEndShowIndex)
		{
			iSumW -= m_iCharShowRI;
		}

		if(m_iCharShowW == iSumW)
		{
			m_iCharStartShowIndex = i;
			m_iCharShowLI = 0;
			break; 
		}
		else if(iSumW > m_iCharShowW)
		{
			m_iCharStartShowIndex = i;
			m_iCharShowLI = iSumW - m_iCharShowW;
			break;
		}
	}

	if(i < 0)
	{
		m_iCharStartShowIndex = 0;
		m_iCharShowLI = 0;
	}

	return 0;
}

int CMyNewEdit::HoriScrollPixel(int iPxShift,BOOL bRedraw /*= FALSE*/,BOOL bSetCaretPos /*= FALSE*/)
{
	if(_T("") == m_strText)
	{
		return 1;
	}

	if(iPxShift > 0)
	{
		int iPxRightUnshow = GetRightUnShowCharPixelLength();
		if(iPxShift > iPxRightUnshow)
		{
			iPxShift = iPxRightUnshow;
		}
	}
	else if(iPxShift < 0)
	{
		int iPxLeftUnshow = GetLeftUnShowCharPixelLength();
		if(abs(iPxShift) > iPxLeftUnshow)
		{
			iPxShift = -iPxLeftUnshow;
		}
	}

	if(0 == iPxShift)
	{
		return -1;
	}

	if(-1 == m_iCharStartShowIndex || -1 == m_iCharEndShowIndex)
	{
		m_iCharStartShowIndex = 0;
		m_iCharEndShowIndex = 0;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}

	int iCharNum = m_strText.GetLength();

	if(iPxShift > 0)
	{
		int iWidth = *m_lnkCharW[m_iCharStartShowIndex];
		if(iWidth - m_iCharShowLI > iPxShift)
		{
			m_iCharShowLI += iPxShift;
		}
		else
		{
			int remainder = iPxShift - (iWidth - m_iCharShowLI);
			for(int i = m_iCharStartShowIndex + 1;i < iCharNum;remainder -= *m_lnkCharW[i],++i)
			{
				if(remainder < *m_lnkCharW[i])
				{
					m_iCharStartShowIndex = i;
					m_iCharShowLI = remainder;
					break;
				}
			}
		}
	}
	else
	{
		iPxShift = abs(iPxShift);
		if(m_iCharShowLI >= iPxShift)
		{
			m_iCharShowLI -= iPxShift;
		}
		else
		{
			int remainder = iPxShift - m_iCharShowLI;
			for(int i = m_iCharStartShowIndex - 1;i >= 0;remainder -= *m_lnkCharW[i],--i)
			{
				//=号在这里至关重要,20211122,孙文郎
				if(remainder <= *m_lnkCharW[i])
				{
					m_iCharStartShowIndex = i;
					m_iCharShowLI = *m_lnkCharW[i];
					break;
				}
			}
		}
	}

	CalcCharStartEndShowIndex();

	if(m_caretInfo.m_iCurCharIndex < m_iCharStartShowIndex)
	{
		m_caretInfo.m_iCurCharIndex = m_iCharStartShowIndex;
	}
	else if(m_caretInfo.m_iCurCharIndex >= m_iCharEndShowIndex)
	{
		m_caretInfo.m_iCurCharIndex = m_iCharEndShowIndex - 1;
	}

	CalcCaretPos(m_caretInfo.m_iCurCharIndex,m_caretInfo.m_pxXStart);

	if(bSetCaretPos)
	{
		CPoint pt;
		pt.x = m_caretInfo.m_pxXStart;
		pt.y = m_caretInfo.m_pxYStart;
		SetCaretPos(pt);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::HoriScrollChar(int iCharShift,BOOL bRedraw /*= FALSE*/)
{
	if(_T("") == m_strText)
	{
		return 1;
	}

	if(-1 == m_iCharStartShowIndex || -1 == m_iCharEndShowIndex)
	{
		m_iCharStartShowIndex = 0;
		m_iCharEndShowIndex = 0;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}

	int iCharNum = m_strText.GetLength();
	if(iCharShift > 0)
	{
		if(iCharShift > iCharNum - 1 - m_caretInfo.m_iCurCharIndex)
		{
			iCharShift = iCharNum - 1 - m_caretInfo.m_iCurCharIndex;
		}
	}
	else if(iCharShift < 0)
	{
		if(abs(iCharShift) > m_caretInfo.m_iCurCharIndex + 1)
		{
			iCharShift = -(m_caretInfo.m_iCurCharIndex + 1);
		}
	}

	if(0 == iCharShift)
	{
		return -1;
	}

	m_caretInfo.m_iCurCharIndex += iCharShift;
	SetCharFullShow(m_caretInfo.m_iCurCharIndex);
	CalcCaretPos(m_caretInfo.m_iCurCharIndex,m_caretInfo.m_pxXStart);
	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::TransformXPosToCharInex(int pxX)
{
	BOOL bNeedRedraw = FALSE;

	if(-1 == m_iCharStartShowIndex || -1 == m_iCharEndShowIndex)
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_iLeftIndent + m_pxFrameW;
		return 1;
	}
	
	int iCharNum = m_strText.GetLength();
	int iSumW = m_iLeftIndent + m_pxFrameW;
	int iPreSumW = m_iLeftIndent + m_pxFrameW;
	int i = m_iCharStartShowIndex;
	int iCharW = 0;
	for(;i < iCharNum;++i)
	{
		iCharW = *m_lnkCharW[i];
		iSumW += iCharW;
		if(i == m_iCharStartShowIndex)
		{
			iSumW -= m_iCharShowLI;
		}

		if(pxX <= iSumW)
		{
			if(pxX - iPreSumW < iCharW / 2)
			{
				m_caretInfo.m_iCurCharIndex = i - 1;
				if(i == m_iCharStartShowIndex && 0 != m_iCharShowLI)
				{
					m_iCharShowLI = 0;
					CalcCharStartEndShowIndex();
					m_caretInfo.m_pxXStart = m_iLeftIndent + m_pxFrameW;
					bNeedRedraw = TRUE;
				}
				else
				{
					m_caretInfo.m_pxXStart = iPreSumW;
				}
			}
			else
			{
				m_caretInfo.m_iCurCharIndex = i;
				if(i == m_iCharEndShowIndex && 0 != m_iCharShowRI)
				{
					m_iCharShowRI = 0;
					CalcCharEndStartShowIndex();
					m_caretInfo.m_pxXStart = m_iLeftIndent + m_pxFrameW + m_iCharShowW;
					bNeedRedraw = TRUE;
				}
				else
				{
					m_caretInfo.m_pxXStart = iSumW;
				}
			}
			break;
		}

		iPreSumW = iSumW;
	}
	if(i >= iCharNum)
	{
		m_caretInfo.m_iCurCharIndex = iCharNum - 1;
		m_caretInfo.m_pxXStart = iSumW;
	}

	if(bNeedRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::TransformXPosToCharInex(int pxX,int& iIndex)
{
	if(_T("") == m_strText)
	{
		return -1;
	}

	if(-1 == m_iCharStartShowIndex || -1 == m_iCharEndShowIndex)
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_iLeftIndent + m_pxFrameW;
		return 1;
	}

#ifdef _DEBUG
	CRect rcClient;
	GetClientRect(&rcClient);
	if(pxX > rcClient.right)
	{
		int tt = 0;
		++tt;
	}
#endif

	BOOL bNeedRedraw = FALSE;

	int iCharNum = m_strText.GetLength();
	int iSumW = m_iLeftIndent + m_pxFrameW;
	int iPreSumW = m_iLeftIndent + m_pxFrameW;
	int i = m_iCharStartShowIndex;
	int iCharW = 0;
	for(;i < iCharNum;++i)
	{
		iCharW = *m_lnkCharW[i];
		iSumW += iCharW;
		if(i == m_iCharStartShowIndex)
		{
			iSumW -= m_iCharShowLI;
		}

		if(pxX <= iSumW)
		{
			if(pxX - iPreSumW < iCharW / 2)
			{
				iIndex = i - 1;
				m_caretInfo.m_iCurCharIndex = i - 1;
				if(i == m_iCharStartShowIndex && 0 != m_iCharShowLI)
				{
					m_iCharShowLI = 0;
					CalcCharStartEndShowIndex();
					m_caretInfo.m_pxXStart = m_iLeftIndent + m_pxFrameW;
					bNeedRedraw = TRUE;
				}
				else
				{
					m_caretInfo.m_pxXStart = iPreSumW;
				}
			}
			else
			{
				iIndex = i;
				m_caretInfo.m_iCurCharIndex = i;
				if(i == m_iCharEndShowIndex && 0 != m_iCharShowRI)
				{
					m_iCharShowRI = 0;
					CalcCharEndStartShowIndex();
					m_caretInfo.m_pxXStart = m_iLeftIndent + m_pxFrameW + m_iCharShowW;
					bNeedRedraw = TRUE;
				}
				else
				{
					m_caretInfo.m_pxXStart = iSumW;
				}
			}
			break;
		}

		iPreSumW = iSumW;
	}
	if(i >= iCharNum)
	{
		iIndex = iCharNum - 1;
		m_caretInfo.m_iCurCharIndex = iCharNum - 1;
		m_caretInfo.m_pxXStart = iSumW;
	}

	if(bNeedRedraw && NULL != GetSafeHwnd())
	{
		//Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::GetLeftUnShowCharPixelLength()
{
	int iSum = 0;
	for(int i = 0;i < m_iCharStartShowIndex;++i)
	{
		iSum += *m_lnkCharW[i];
	}
	iSum += m_iCharShowLI;
	return iSum;
}

int CMyNewEdit::GetRightUnShowCharPixelLength()
{
	int iSum = m_iCharShowRI;
	for(int i = m_iCharEndShowIndex + 1;i < m_strText.GetLength();++i)
	{
		iSum += *m_lnkCharW[i];
	}
	return iSum;
}

int CMyNewEdit::GetWidth(int iStart,int iEnd)
{
	int iSum = 0;
	if(iStart < 0 || iEnd < 0)
	{
		return iSum;
	}

	int iCharNum = m_strText.GetLength();
	if(iStart >= iCharNum || iEnd >= iCharNum)
	{
		return iSum;
	}

	if(iStart > iEnd)
	{
		return iSum;
	}
	for(int i = iStart;i <= iEnd;++i)
	{
		iSum += *m_lnkCharW[i];
	}
	return iSum;
}

int CMyNewEdit::ArrowKeyProcess(UINT nChar)
{
	if(VK_LEFT != nChar && VK_UP != nChar && VK_RIGHT != nChar && VK_DOWN != nChar)
	{
		return -1;
	}

	if(VK_LEFT == nChar)
	{
		HoriScrollChar(-1,TRUE);
	}
	else if(VK_RIGHT == nChar)
	{
		HoriScrollChar(1,TRUE);
	}

	return 0;
}

//未选中字符时，VK_DELETE删除后面1个字符，VK_BACK删除前面一个字符，选中字符时，都删除选中的字符
int CMyNewEdit::DeleteKeyProcess(UINT nChar)
{
	if(VK_DELETE != nChar && VK_BACK != nChar)
	{
		return -1;
	}

	if(m_iSelCharNum > 0)
	{
		DeleteSelectedChar();
	}
	else
	{
		if(VK_DELETE == nChar)
		{
			DeleteCharAfter();
		}
		else if(VK_BACK == nChar)
		{
			DeleteChar();
		}
	}

	return 0;
}

int CMyNewEdit::MouseLeftBtPushedMoveProcess(CPoint pt)
{
	if(!m_bLBtPushedMove)
	{
		return -1;
	}

	if(_T("") == m_strText)
	{
		return 1;
	}

	if(-1 == m_iCharStartShowIndex || -1 == m_iCharEndShowIndex)
	{
		return -2;
	}

	CRect rcClient;
	GetClientRect(&rcClient);
	if(pt.x > rcClient.right - m_pxFrameW - m_iRightIndent)
	{
		m_eCurSMT = ESMT_OUTRIGHT;
		if(m_ptPreSelMove.x >= pt.x)
		{
			return 0;
		}
		else
		{
			if(0 != HoriScrollChar(1))
			{
				return -3;
			}
			else
			{
				m_iSelEnd = m_caretInfo.m_iCurCharIndex;
			}
		}
	}
	else if(pt.x < rcClient.left + m_pxFrameW + m_iLeftIndent)
	{
		m_eCurSMT = ESMT_OUTLEFT;
		if(m_ptPreSelMove.x <= pt.x)
		{
			return 0;
		}
		else
		{
			if(0 != HoriScrollChar(-1))
			{
				return -3;
			}
			else
			{
				m_iSelEnd = m_caretInfo.m_iCurCharIndex;
			}
		}
	}
	else
	{
		if(0 != TransformXPosToCharInex(pt.x,m_iSelEnd))
		{
			return -4;
		}
	}

	m_iSelCharNum = abs(m_iSelEnd - m_iSelStart);

	Invalidate(FALSE);

	return 0;
}

int CMyNewEdit::DeleteSelectedChar(BOOL bRedraw /*= TRUE*/)
{
	if(m_iSelCharNum < 1)
	{
		return 1;
	}

	if(_T("") == m_strText)
	{
		return 2;
	}

	if(m_bLBtPushedMove)	//选中滑动期间禁止删除操作
	{
		return 3;
	}

	if(m_iSelStart == m_iSelEnd)
	{
		return -1;
	}

	if(m_iSelStart > m_iSelEnd)
	{
		int iTemp = m_iSelStart;
		m_iSelStart = m_iSelEnd;
		m_iSelEnd = iTemp;
	}

	m_strText.Delete(m_iSelStart + 1,m_iSelCharNum);
	for(int i = m_iSelEnd;i > m_iSelStart;--i)
	{
		m_lnkCharW.DeleteAt(i);
	}

	if(_T("") == m_strText)	//全部删除
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}
	else
	{
		if(m_iSelStart < m_iCharStartShowIndex || m_iSelStart > m_iCharEndShowIndex)
		{
			m_iCharStartShowIndex = m_iSelStart;
			m_iCharShowLI = 0;
		}

		CalcCharStartEndShowIndex();

		m_caretInfo.m_iCurCharIndex = m_iSelStart;
		if(m_caretInfo.m_iCurCharIndex < m_iCharStartShowIndex)
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		}
		else
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent + *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + GetWidth(m_iCharStartShowIndex + 1,m_iSelStart);
		}
	}

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);
	ShowCaret();

	ClearSelectInfo();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	
	return 0;
}

int CMyNewEdit::DeleteChar()
{
	int iCharNum = m_strText.GetLength();
	if(iCharNum < 1)
	{
		return 1;
	}

	if(-1 == m_caretInfo.m_iCurCharIndex)
	{
		return 2;
	}

	if(m_bLBtPushedMove)	//选中滑动期间禁止删除操作
	{
		return 3;
	}

	m_strText.Delete(m_caretInfo.m_iCurCharIndex,1);
	m_lnkCharW.DeleteAt(m_caretInfo.m_iCurCharIndex);

	if(_T("") == m_strText)	//全部删除
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}
	else
	{
		if(m_caretInfo.m_iCurCharIndex > m_iCharEndShowIndex)
		{
			m_iCharEndShowIndex = m_caretInfo.m_iCurCharIndex - 1;
			m_iCharShowRI = 0;
		}
		else
		{
			m_iCharEndShowIndex -= 1;
		}

		CalcCharEndStartShowIndex();

		--m_caretInfo.m_iCurCharIndex;
		if(-1 == m_caretInfo.m_iCurCharIndex)
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		}
		else
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent + *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + GetWidth(m_iCharStartShowIndex + 1,m_caretInfo.m_iCurCharIndex);
		}
	}

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	if(NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::DeleteCharAfter()
{
	int iCharNum = m_strText.GetLength();
	if(iCharNum < 1)
	{
		return 1;
	}

	if(iCharNum - 1 == m_caretInfo.m_iCurCharIndex)
	{
		return 2;
	}

	if(m_bLBtPushedMove)	//选中滑动期间禁止删除操作
	{
		return 3;
	}

	m_strText.Delete(m_caretInfo.m_iCurCharIndex + 1,1);
	m_lnkCharW.DeleteAt(m_caretInfo.m_iCurCharIndex + 1);

	if(_T("") == m_strText)	//全部删除
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}
	else
	{
		CalcCharStartEndShowIndex();
		if(-1 == m_caretInfo.m_iCurCharIndex)
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		}
		else
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent + *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + GetWidth(m_iCharStartShowIndex + 1,m_caretInfo.m_iCurCharIndex);
		}
	}

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	if(NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::DeleteChar(int iIndex)
{
	int iCharNum = m_strText.GetLength();
	if(iCharNum < 1)
	{
		return 1;
	}

	if(iIndex < 0)
	{
		return -1;
	}

	if(iIndex >= iCharNum)
	{
		return -2;
	}

	m_strText.Delete(m_caretInfo.m_iCurCharIndex,1);
	m_lnkCharW.DeleteAt(m_caretInfo.m_iCurCharIndex);

	if(_T("") == m_strText)	//全部删除
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}
	else
	{
		if(iIndex <= m_iCharEndShowIndex)
		{
			m_iCharEndShowIndex -= 1;
		}

		CalcCharEndStartShowIndex();

		if(iIndex <= m_caretInfo.m_iCurCharIndex)
		{
			--m_caretInfo.m_iCurCharIndex;
		}
		
		if(-1 == m_caretInfo.m_iCurCharIndex)
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		}
		else
		{
			m_caretInfo.m_pxXStart = *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + GetWidth(m_iCharStartShowIndex + 1,m_caretInfo.m_iCurCharIndex);
		}
	}

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	if(NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::DeleteCharAfter(int iIndex)
{
	int iCharNum = m_strText.GetLength();
	if(iCharNum < 1)
	{
		return 1;
	}

	if(iIndex < -1)
	{
		return -1;
	}

	if(iIndex >= iCharNum - 1)
	{
		return -2;
	}

	m_strText.Delete(iIndex + 1,1);
	m_lnkCharW.DeleteAt(iIndex + 1);

	if(_T("") == m_strText)	//全部删除
	{
		m_caretInfo.m_iCurCharIndex = -1;
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		m_iCharStartShowIndex = -1;
		m_iCharEndShowIndex = -1;
		m_iCharShowLI = 0;
		m_iCharShowRI = 0;
	}
	else
	{
		if(iIndex + 1 <= m_iCharEndShowIndex)
		{
			m_iCharEndShowIndex -= 1;
		}

		CalcCharEndStartShowIndex();

		if(iIndex + 1 <= m_caretInfo.m_iCurCharIndex)
		{
			--m_caretInfo.m_iCurCharIndex;
		}

		if(-1 == m_caretInfo.m_iCurCharIndex)
		{
			m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
		}
		else
		{
			m_caretInfo.m_pxXStart = *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + GetWidth(m_iCharStartShowIndex + 1,m_caretInfo.m_iCurCharIndex);
		}
	}

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	if(NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyNewEdit::CharFilter(UINT nChar,UINT nFlags)
{
	if(VK_BACK == nChar)
	{
		return 0;
	}

	return -1;
}

int CMyNewEdit::MsgFilter(MSG* pMsg)
{
	if(NULL == pMsg)
	{
		return -1;
	}

	if(WM_KEYDOWN == pMsg->message)
	{  
		BOOL bCtrlPushed = ::GetKeyState(VK_CONTROL) & 0x8000;
		UINT  nKeyCode = pMsg->wParam; 
		if(((_T('c') == nKeyCode) || (_T('C') == nKeyCode)) && bCtrlPushed)	//Ctrl+C消息
		{
			CopySelectedTextToClipBoard();
			return 0;
		}
		if(((_T('v') == nKeyCode) || (_T('V') == nKeyCode)) && bCtrlPushed)	//Ctrl+C消息
		{
			CtrlAndVProcess();
			return 0;
		}
		else if(((_T('a') == nKeyCode) || (_T('A') == nKeyCode)) && bCtrlPushed)	//Ctrl+A消息
		{
			SetAllSelected();
			return 0;
		}
		else if(((_T('x') == nKeyCode) || (_T('X') == nKeyCode)) && bCtrlPushed)	//Ctrl+A消息
		{
			CopySelectedTextToClipBoard();
			DeleteSelectedChar();
			return 0;
		}
	}

	return -2;
}

int CMyNewEdit::CopySelectedTextToClipBoard()
{
	CString strText(_T(""));

	if(m_iSelCharNum > 0)
	{
		if(m_iSelStart > m_iSelEnd)
		{
			int iTemp = m_iSelStart;
			m_iSelStart = m_iSelEnd;
			m_iSelEnd = iTemp;
		}
		strText = m_strText.Mid(m_iSelStart + 1,m_iSelCharNum);
	}

	if(OpenClipboard())
	{
		HGLOBAL hClipBuf;
		TCHAR* buf;
		EmptyClipboard();
		hClipBuf = GlobalAlloc(GMEM_DDESHARE,strText.GetLength() * sizeof(TCHAR) + 2);
		buf = (TCHAR*)GlobalLock(hClipBuf);
		lstrcpy(buf,strText);
		GlobalUnlock(hClipBuf);
		SetClipboardData(CF_UNICODETEXT,hClipBuf);
		CloseClipboard();
	}

	return 0;
}

int CMyNewEdit::CtrlAndVProcess()
{
	if(!OpenClipboard())
	{
		return -1;
	}

	HANDLE hClip = NULL;
	hClip = GetClipboardData(CF_UNICODETEXT);
	if(NULL == hClip)
	{
		CloseClipboard();
		return -2;
	}

	CString strClipText(_T(""));
	char* pBuf = NULL;
	pBuf = (char*)GlobalLock(hClip);
	GlobalUnlock(hClip);

	strClipText = (TCHAR*)pBuf;
	if(m_iSelCharNum > 0)
	{
		DeleteSelectedChar(FALSE);
	}

	if(_T("") == strClipText)
	{
		Invalidate( FALSE);
		return 0;
	}

	CDC* pDc = GetDC();
	if(NULL == pDc)
	{
		return -3;
	}
	CFont* pOldFont = pDc->SelectObject(&m_fontSet);

	int iClipTextNum = strClipText.GetLength();
	int iCaretPosBeforeInsert = m_caretInfo.m_iCurCharIndex;
	m_strText.Insert(++iCaretPosBeforeInsert,strClipText);

	CString strTemp(_T(""));
	CSize size;
	iCaretPosBeforeInsert = m_caretInfo.m_iCurCharIndex;
	for(int i = 0;i < iClipTextNum;++i)
	{
		strTemp = strClipText.GetAt(i);
		size = pDc->GetTextExtent(strTemp);
		int iCharW = size.cx;
		if(ETM_PASSWORDDOT == m_eTxMode)
		{
			iCharW = m_iPWCharW;
		}
		m_lnkCharW.InsertAtPre(++iCaretPosBeforeInsert,iCharW);
	}
	pDc->SelectObject(pOldFont);
	ReleaseDC(pDc);

	m_caretInfo.m_iCurCharIndex = iCaretPosBeforeInsert;

	CloseClipboard();

	CalcCharStartEndShowIndex();
	SetCharFullShow(m_caretInfo.m_iCurCharIndex);
	CalcCaretPos(m_caretInfo.m_iCurCharIndex,m_caretInfo.m_pxXStart);
	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	Invalidate(FALSE);

	TellParentTxChangedProcess();

	return 0;
}

int CMyNewEdit::CheckLnkCharWidth()
{
	int iCharNum = m_strText.GetLength();
	if(iCharNum < 1)
	{
		return 1;
	}

	if(m_lnkCharW.GetSize() == iCharNum)
	{
		return 2;
	}
	m_lnkCharW.Clear();

	if(NULL == GetSafeHwnd())
	{
		return -1;
	}

	CDC* pDc = GetDC();
	if(NULL == pDc)
	{
		return -2;
	}
	
	CFont* pOldFont = pDc->SelectObject(&m_fontSet);
	CSize size;
	for(int i = 0;i < iCharNum;++i)
	{
		size = pDc->GetTextExtent(m_strText.Mid(i,1));
		int iCharW = size.cx;
		if(ETM_PASSWORDDOT == m_eTxMode)
		{
			iCharW = m_iPWCharW;
		}
		m_lnkCharW.AddTail(iCharW);
	}

	pDc->SelectObject(pOldFont);
	ReleaseDC(pDc);	

	m_iCharEndShowIndex = iCharNum - 1;
	m_iCharShowRI = 0;
	CalcCharEndStartShowIndex();

	m_caretInfo.m_iCurCharIndex = iCharNum - 1;
	if(-1 == m_caretInfo.m_iCurCharIndex)
	{
		m_caretInfo.m_pxXStart = m_pxFrameW + m_iLeftIndent;
	}
	else
	{
		m_caretInfo.m_pxXStart = *m_lnkCharW[m_iCharStartShowIndex] - m_iCharShowLI + GetWidth(m_iCharStartShowIndex + 1,m_caretInfo.m_iCurCharIndex);
	}

	CPoint pt;
	pt.x = m_caretInfo.m_pxXStart;
	pt.y = m_caretInfo.m_pxYStart;
	SetCaretPos(pt);

	return 0;
}

EREGIONTYPE CMyNewEdit::GetRegionType(CPoint pt)
{
	if(!m_rcCtrl.PtInRect(pt))
	{
		m_eRgnType = ERT_OUTSIDE;
		return ERT_OUTSIDE;
	}

	if(m_rcText.PtInRect(pt))
	{
		m_eRgnType = ERT_EDIT;
		return ERT_EDIT;
	}

	if(m_rcLeftIndent.PtInRect(pt))
	{
		m_eRgnType = ERT_LEFTINDENT;
		return ERT_LEFTINDENT;
	}

	if(m_rcRightIndent.PtInRect(pt))
	{
		m_eRgnType = ERT_RIGHTINDENT;
		return ERT_RIGHTINDENT;
	}

	if(m_rcRightIndent.PtInRect(pt))
	{
		m_eRgnType = ERT_TOPINDENT;
		return ERT_TOPINDENT;
	}

	if(m_rcRightIndent.PtInRect(pt))
	{
		m_eRgnType = ERT_BOTTOMINDENT;
		return ERT_BOTTOMINDENT;
	}

	m_eRgnType = ERT_EMPTY;
	return ERT_EMPTY;
}

int CMyNewEdit::TellParentTxChangedProcess()
{
	if(!m_bTellParentTxChanged)
	{
		return 1;
	}

	CWnd* pParent = GetParent();
	if(NULL != pParent->GetSafeHwnd())
	{
		pParent->PostMessage(WM_TELLPARENTTEXTCHANGED,m_iTextTypeTag,0);
		return 0;
	}

	return -1;
}

int CMyNewEdit::TellParentSetNewTextProcess()
{
	if(!m_bTellParentSetNewText)
	{
		return 1;
	}

	CWnd* pParent = GetParent();
	if(NULL != pParent->GetSafeHwnd())
	{
		pParent->PostMessage(WM_TELLPARENTSETNEWTEXT,m_iTextTypeTag,0);
		return 0;
	}

	return -1;
}

void CMyNewEdit::SetBeFuncEdit(BOOL bFuncEdit)
{
	m_bFuncEdit = bFuncEdit;
}

BOOL CMyNewEdit::GetBeFuncEdit()
{
	return m_bFuncEdit;
}

void CMyNewEdit::SetDefaultIndent()
{
	m_iLeftIndent = 2;
	m_iRightIndent = 2;
	m_iTopIndent = 2;
	m_iBottomIndent = 2;
}

void CMyNewEdit::SetCtrlTag(int iCtrlTag)
{
	m_iCtrlTag = iCtrlTag;
}

int CMyNewEdit::GetCtrlTag()
{
	return m_iCtrlTag;
}


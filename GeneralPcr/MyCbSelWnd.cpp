// SonWnd.cpp : implementation file
//

#include "stdafx.h"
#include "MyCbSelWnd.h"
#include "MyNewCb.h"

// CMyCbSelWnd

IMPLEMENT_DYNAMIC(CMyCbSelWnd, CDialogEx)

CMyCbSelWnd::CMyCbSelWnd()
{
	m_pxFrameW = 1;

	m_rowNum = 0;
	m_cdcH = 0;	
	m_totalRowH = 0;	
	m_rowIdStartShow = -1;
	m_rowIdEndShow = -1;
	m_iRowTopIndent = 0;	
	m_iRowBottomIndent = 0;
	m_lnkHistoryDotPosInfo.SetLimitMaxParam(TRUE,100);
	m_bTellParentRowSelChange = FALSE;
	m_bMouseFstInCtrl = FALSE;
	m_pParent = NULL;
	m_bAllowSameItem = FALSE;
	InitVertScroll();
	m_iLeftIndent = 16;
	m_iRightIndent = 16;
	m_iDefRowH = 40;

	m_bEnableDelete = TRUE;
}

CMyCbSelWnd::~CMyCbSelWnd()
{

}

BEGIN_MESSAGE_MAP(CMyCbSelWnd, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_SHOWWINDOW()
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DELETE_COMBO_ROW_STRING,OnDeleteRowString)
END_MESSAGE_MAP()

void CMyCbSelWnd::OnPaint()
{
	CPaintDC dc(this); 

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(&dc,m_rcWnd.Width(),m_rcWnd.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	Graphics graphics(memDC.m_hDC);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	Pen pen(Color(2,91,199));
	graphics.DrawRectangle(&pen,m_rcWnd.left,m_rcWnd.top,m_rcWnd.Width(),m_rcWnd.Height());

	graphics.ReleaseHDC(memDC.m_hDC);

	DrawBody(&memDC);
	DrawVertScroll(&memDC);

	dc.BitBlt(0,0,m_rcWnd.Width(),m_rcWnd.Height(),&memDC,0,0,SRCCOPY);   

	memDC.SelectObject(pOldBitmap);
	bitMap.DeleteObject();
	memDC.DeleteDC();

	return;
}

BOOL CMyCbSelWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyCbSelWnd::OnSize(UINT nType, int cx, int cy)
{
	m_rcWnd = CRect(0,0,cx,cy);
	m_rcBody = CRect(m_pxFrameW,m_pxFrameW,cx - m_pxFrameW,cy - m_pxFrameW);

	CalcScrollPos();
	CalcRowToShow();
	
	CDialogEx::OnSize(nType, cx, cy);
}

void CMyCbSelWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if(!rect.PtInRect(point))
	{
		ShowWindow(SW_HIDE);
		ReleaseCapture();
	}
	return;
}

void CMyCbSelWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(&rect);
	if(rect.PtInRect(point))
	{
		if(m_vertScroll.m_bInSlider)
		{
			m_vertScroll.m_bLeftBtDown = TRUE;
			m_vertScroll.m_prePt = point;
			SetCapture();
		}
		else
		{
			//计算鼠标点击时所在的行和列
			CMySelWndDotPosInfo dotPosInfo = CalcPtPos(point);
			if(EMSWDPT_BODY == dotPosInfo.m_eDotPosType && !m_lnkItemForSelInfo[dotPosInfo.m_rowId]->GetBeDisabled())
			{
				
			}
		}
		
DEFPROCESS:
		CDialogEx::OnLButtonDown(nFlags,point);
	}
	else
	{
		ShowWindow(SW_HIDE);
		ReleaseCapture();
	}
	return;
}

void CMyCbSelWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_vertScroll.m_bLeftBtDown = FALSE;
	m_vertScroll.m_bStartDragSlider = FALSE;
	m_vertScroll.m_bSliderDragMove = FALSE;

	ReleaseCapture();

	m_preDotPosInfo = m_curDotPosInfo;
	m_curDotPosInfo = CalcPtPos(point);
	m_lnkHistoryDotPosInfo.AddTail(m_curDotPosInfo);

	BOOL bRedraw = FALSE;

	//更新行列选择状态
	if(EMSWDPT_BODY == m_curDotPosInfo.m_eDotPosType && -1 != m_curDotPosInfo.m_rowId)
	{
		CItemForSelInfo rowInfo = *m_lnkItemForSelInfo[m_curDotPosInfo.m_rowId];

		SetRowSingleSelected(m_curDotPosInfo.m_rowId);

		BOOL bSelected = rowInfo.GetBeSelected();
		if(m_bTellParentRowSelChange && NULL != m_pParent->GetSafeHwnd())	//选中行改变，通知父窗口
		{
			m_pParent->PostMessage(WM_ROWSELECTCHANGE,m_curDotPosInfo.m_rowId,0);
		}

		bRedraw = TRUE;
	}

	if(bRedraw)
	{
		Invalidate(FALSE);
	}

	CDialogEx::OnLButtonUp(nFlags,point);
	return;
}

void CMyCbSelWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	{
		CRect rect;
		GetClientRect(&rect);
		BOOL bInCtrl = rect.PtInRect(point);

		if(!m_bMouseFstInCtrl)
		{
			m_bMouseFstInCtrl = TRUE;
			if(m_bMouseFstInCtrl)
			{
				SetCapture();
			}
		}
		else
		{
			if(!bInCtrl)
			{
				m_bMouseFstInCtrl = FALSE;
			}
		}
	}


	m_preDotPosInfo = m_curDotPosInfo;
	m_curDotPosInfo = CalcPtPos(point);

	BOOL bNeedRedraw = FALSE;
	
	if(m_vertScroll.m_rcSlider.PtInRect(point))
	{
		m_vertScroll.m_bInSlider = TRUE;
		if(m_vertScroll.m_bLeftBtDown)
		{
			m_vertScroll.m_bStartDragSlider = TRUE;
		}

		if(m_vertScroll.m_bStartDragSlider && (MK_LBUTTON & nFlags))
		{
			m_vertScroll.m_bSliderDragMove = TRUE;
		}
	}
	else
	{
		m_vertScroll.m_bInSlider = FALSE;
	}

	if(m_vertScroll.m_bSliderDragMove)
	{
		ProcessVertSrcoll(nFlags,point);
	}
	else
	{
		if(m_rowNum < 0)
		{
			goto END;
		}

		if(m_preDotPosInfo.m_eDotPosType == m_curDotPosInfo.m_eDotPosType)
		{
			if(EMSWDPT_BODY == m_curDotPosInfo.m_eDotPosType)
			{
				if(m_preDotPosInfo.m_rowId != m_curDotPosInfo.m_rowId)
				{
					if(-1 != m_preDotPosInfo.m_rowId)
					{
						m_lnkItemForSelInfo[m_preDotPosInfo.m_rowId]->MouseOut();  
						bNeedRedraw = TRUE;
					}

					if(-1 != m_curDotPosInfo.m_rowId)
					{
						m_lnkItemForSelInfo[m_curDotPosInfo.m_rowId]->MouseIn();  
						bNeedRedraw = TRUE;
					}
				}
			}
		}
		else
		{
			if(EMSWDPT_BODY == m_curDotPosInfo.m_eDotPosType)
			{
				if(-1 != m_curDotPosInfo.m_rowId)
				{
					m_lnkItemForSelInfo[m_curDotPosInfo.m_rowId]->MouseIn();  
					bNeedRedraw = TRUE;
				}
			}
			else if(EMSWDPT_BODY == m_preDotPosInfo.m_eDotPosType)
			{
				if(-1 != m_preDotPosInfo.m_rowId)
				{
					m_lnkItemForSelInfo[m_preDotPosInfo.m_rowId]->MouseOut();  
					bNeedRedraw = TRUE;
				}
			}
		}
	}

	if(bNeedRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
END:
	return;
}

BOOL CMyCbSelWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(m_vertScroll.m_bScrollNeedShow)
	{
		if(zDelta > 0)
		{
			ScrollRow(-28 * m_dVertRatio * 2);
		}
		else
		{
			ScrollRow(28 * m_dVertRatio * 2);
		}
	}
	
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyCbSelWnd::OnSetFocus(CWnd* pOldWnd)
{
	/*CWnd* pParent = GetParent();
	if(NULL != pParent->GetSafeHwnd())
	{
	pParent->SetFocus();
	}*/
	//CDialogEx::OnSetFocus(pOldWnd);
	return;
}

void CMyCbSelWnd::OnKillFocus(CWnd* pNewWnd)
{
	//CDialogEx::OnKillFocus(pNewWnd);
	return;
}

void CMyCbSelWnd::OnShowWindow(BOOL bShow,UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow,nStatus);
	return;
}

int CMyCbSelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iLeftIndent *= m_dHoriRatio;
	m_iRightIndent *= m_dHoriRatio;
	m_iDefRowH *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

int CMyCbSelWnd::DrawBody(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	if(m_rowNum < 1)
	{
		return -2;
	}

	if(m_lnkItemForSelInfo.GetSize() != m_rowNum)
	{
		return -3;
	}

	CDC memDcBody;
	memDcBody.CreateCompatibleDC(pDC);

	CBitmap bitMap;
	int tempCdcH = m_cdcH;
	if(-1 == m_rowIdStartShow || m_cdcH < m_rcBody.Height())
	{
		tempCdcH = m_rcBody.Height();
	}
	bitMap.CreateCompatibleBitmap(pDC,m_rcBody.Width(),tempCdcH);
	CBitmap* pOldBitmap = memDcBody.SelectObject(&bitMap);

	memDcBody.FillSolidRect(0,0,m_rcBody.Width(),tempCdcH,RGB(255,255,255));

	CRect tempRc;
	
	CFont* pOldFont = memDcBody.SelectObject(&m_fontSet);
	memDcBody.SetTextColor(m_txClr1);
	memDcBody.SetBkMode(TRANSPARENT);

	Graphics graphics(memDcBody.m_hDC);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	if(-1 != m_rowIdStartShow)
	{
		BOOL bBkDDrawed = FALSE;

		COLORREF clrText = 0;
		for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
		{
			CItemForSelInfo& rowInfo = *m_lnkItemForSelInfo[i];

			tempRc.top = GetRowStartDrawInMemoryVertPixel(i);
			tempRc.left = 0;
			tempRc.right = m_rcBody.Width();
			tempRc.bottom = tempRc.top + rowInfo.m_iItemH;

			if(ESS_NORMAL == rowInfo.m_eSelState)
			{
				memDcBody.FillSolidRect(&tempRc,RGB(255,255,255));
				clrText = RGB(0X39,0X45,0X65);
			}
			else if(ESS_HOVER == rowInfo.m_eSelState)
			{
				memDcBody.FillSolidRect(&tempRc,RGB(0XF5,0XF7,0XFA));
				clrText = RGB(0X39,0X45,0X65);
				bBkDDrawed = TRUE;
			}
			else if(ESS_SELECTED == rowInfo.m_eSelState)
			{
				if(m_curDotPosInfo.m_rowId != i)
				{
					memDcBody.FillSolidRect(&tempRc,RGB(255,255,255));
				}
				else
				{
					memDcBody.FillSolidRect(&tempRc,RGB(0XF5,0XF7,0XFA));
				}
				clrText = CLR_SOFT_THEME;
				bBkDDrawed = TRUE;
			}
			else if(ESS_DISABLE == rowInfo.m_eSelState)
			{
				memDcBody.FillSolidRect(&tempRc,RGB(0XF7,0XF7,0XF7));
				clrText = RGB(0X39,0X45,0X65);
			}

			tempRc.left = m_iLeftIndent;
			tempRc.right = m_rcBody.Width() - m_iRightIndent;

			memDcBody.SetTextColor(clrText);
			memDcBody.DrawText(rowInfo.m_strText,&tempRc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		}
	}

	graphics.ReleaseHDC(memDcBody.m_hDC);
	BOOL bRet = pDC->BitBlt(m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),&memDcBody,0,m_iRowTopIndent,SRCCOPY);   

	memDcBody.SelectObject(pOldFont);
	memDcBody.SelectObject(pOldBitmap);

	memDcBody.DeleteDC();
	bitMap.DeleteObject();

	return 0;
}
 
int CMyCbSelWnd::DrawVertScroll(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	if(!m_vertScroll.m_bScrollNeedShow)
	{
		return 1;
	}

#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	pDC->FillSolidRect(&m_vertScroll.m_rcScroll,m_vertScroll.m_clrBk);

	CBrush lineBrush;
	lineBrush.CreateSolidBrush(RGB(197,197,197));
	pDC->FrameRect(&m_vertScroll.m_rcScroll,&lineBrush);

	CRect rcTemp;
	rcTemp = m_vertScroll.m_rcStartArrow;
	rcTemp.InflateRect(0,m_pxFrameW,m_pxFrameW,0);
	pDC->FrameRect(&rcTemp,&lineBrush);

	//绘制顶向箭头
	Graphics graphics(pDC->m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	SolidBrush brush(Color(128,128,128));
	int triangleW = 9,triangleH = 7;
	int tempInt1 = rcTemp.left + (rcTemp.Width() - triangleW) / 2;
	int tempInt2 = rcTemp.top + (rcTemp.Height() - triangleH) / 2;
	Point pt1(tempInt1,tempInt2 + triangleH);
	Point pt2(tempInt1 + triangleW,tempInt2 + triangleH);
	Point pt3(tempInt1 + triangleW / 2,tempInt2);

	Point crvPts[3] = {pt1,pt2,pt3};
	graphics.FillPolygon(&brush,crvPts,3,FillModeAlternate);

	rcTemp = m_vertScroll.m_rcEndArrow;
	rcTemp.InflateRect(0,0,m_pxFrameW,m_pxFrameW);
	pDC->FrameRect(&rcTemp,&lineBrush);
	//绘制底向箭头
	tempInt1 = rcTemp.left + (rcTemp.Width() - triangleW) / 2;
	tempInt2 = rcTemp.bottom - (rcTemp.Height() - triangleH) / 2;
	Point pt12(tempInt1,tempInt2 - triangleH);
	Point pt22(tempInt1 + triangleW,tempInt2 - triangleH);
	Point pt32(tempInt1 + triangleW / 2,tempInt2);

	Point crvPts2[3] = {pt12,pt22,pt32};
	graphics.FillPolygon(&brush,crvPts2,3,FillModeAlternate);
	graphics.ReleaseHDC(pDC->m_hDC);

	CPen penSliderFrame;
	penSliderFrame.CreatePen(PS_SOLID,1,RGB(164,164,164));
	CPen* pOldPen = pDC->SelectObject(&penSliderFrame);
	rcTemp = m_vertScroll.m_rcSlider;
	rcTemp.DeflateRect(1,0,-1,0);
	pDC->Rectangle(&rcTemp);
	pDC->SelectObject(pOldPen);

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return 0;
}

void CMyCbSelWnd::CalcScrollPos()
{
	if(m_totalRowH > m_rcBody.Height())
	{
		m_vertScroll.m_bScrollNeedShow = TRUE;		
	}
	else
	{
		m_vertScroll.m_bScrollNeedShow = FALSE;	
	}
	
	if(m_vertScroll.m_bScrollNeedShow)
	{
		m_rcBody.right = m_rcWnd.right - m_pxFrameW - m_vertScroll.m_scrollW;
		m_vertScroll.m_rcScroll = CRect(m_rcWnd.Width() - m_pxFrameW - m_vertScroll.m_scrollW,m_pxFrameW,m_rcWnd.Width() - m_pxFrameW,m_rcWnd.Height() - m_pxFrameW);
	}
	else
	{
		m_rcBody.right = m_rcWnd.right - m_pxFrameW;
	}

	if(m_vertScroll.m_bScrollNeedShow)
	{
		m_vertScroll.m_pixelTotal = m_totalRowH;
		m_vertScroll.m_pixelForShow = m_rcBody.Height();
		m_vertScroll.CalcSliderPos();
	}

	return;
}

int CMyCbSelWnd::CalcRowToShow()
{
	if(m_rowNum > 0)
	{
		if(m_totalRowH > m_rcBody.Height())
		{
			if(-1 == m_rowIdStartShow)
			{
				m_rowIdStartShow = 0;
				m_iRowTopIndent = 0;
			}

			int sum = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH - m_iRowTopIndent;
			int i = m_rowIdStartShow + 1;
			for(;i < m_rowNum;++i)
			{
				int rowH = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH;
				sum += rowH;
				if(sum == m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_iRowBottomIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_iRowBottomIndent = sum - m_rcBody.Height();
					break;
				}
			}
		}
		else
		{
			m_rowIdStartShow = 0;
			m_rowIdEndShow = m_rowNum - 1;
			m_iRowTopIndent = 0;
			m_iRowBottomIndent = 0;
		}
	}
	else
	{
		m_rowIdStartShow = -1;
		m_rowIdEndShow = -1;
		m_iRowTopIndent = 0;
		m_iRowBottomIndent = 0;
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 

	return 0;
}

int CMyCbSelWnd::CalcRowToShow(int newStartRow)
{
	if(m_rowNum > 0 && newStartRow < m_rowNum)
	{
		if(m_totalRowH > m_rcBody.Height())
		{
			int errNum = 0;
			int sum = 0;
			int i = 0;

REFIND:
			if(newStartRow < 0)
			{
				m_rowIdStartShow = 0;

				//防止陷入死循环,20211122,孙文郎
				if(++errNum > 10)
				{
					m_rowIdStartShow = 0;
					m_rowIdEndShow = m_rowNum - 1;
					m_iRowTopIndent = 0;
					m_iRowBottomIndent = 0;
					m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 
					return -1;
				}
			}

			sum = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH - m_iRowTopIndent;
			i = m_rowIdStartShow + 1;
			for(;i < m_rowNum;++i)
			{
				int rowH = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH;
				sum += rowH;
				if(sum == m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_iRowBottomIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_iRowBottomIndent = sum - m_rcBody.Height();
					break;
				}
				else
				{
					--newStartRow;
					goto REFIND;
				}
			}
		}
		else
		{
			m_rowIdStartShow = 0;
			m_rowIdEndShow = m_rowNum - 1;
			m_iRowTopIndent = 0;
			m_iRowBottomIndent = 0;
		}
	}
	else
	{
		m_rowIdStartShow = -1;
		m_rowIdEndShow = -1;
		m_iRowTopIndent = 0;
		m_iRowBottomIndent = 0;
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 

	return 0;
}

int CMyCbSelWnd::CalcTotalRowHeight()
{
	m_totalRowH = 0;
	Node<CItemForSelInfo>* pNode = m_lnkItemForSelInfo.GetHead();
	while(NULL != pNode)
	{
		m_totalRowH += pNode->data.m_iItemH;
		pNode = pNode->pNext;
	}

	return m_totalRowH;
}

int CMyCbSelWnd::GetHeight(int rowStart,int rowEnd)
{
	if(rowStart < 0 || rowStart >= m_rowNum)
	{
		return -1;
	}
	if(rowEnd < 0 || rowEnd >= m_rowNum)
	{
		return -1;
	}

	int width = 0;
	for(int i = rowStart;i <= rowEnd;++i)
	{
		width += m_lnkItemForSelInfo[i]->m_iItemH;
	}
	return width;
}

int CMyCbSelWnd::GetRowStartDrawInMemoryVertPixel(int rowId)
{
	if(rowId < m_rowIdStartShow)
	{
		return -1;
	}

	int vertPixel = 0;
	for(int i = m_rowIdStartShow;i < rowId;++i)
	{
		vertPixel += m_lnkItemForSelInfo[i]->m_iItemH;
	}
	return vertPixel;
}

void CMyCbSelWnd::ProcessVertSrcoll(UINT nFlags, CPoint point)
{
	if(m_vertScroll.m_bSliderDragMove)
	{
		int vertShift = point.y - m_vertScroll.m_prePt.y;
		if(m_vertScroll.m_rcSlider.top + vertShift < m_vertScroll.m_rcStartArrow.bottom)
		{
			int sliderHeight = m_vertScroll.m_rcSlider.Height();
			m_vertScroll.m_rcSlider.top = m_vertScroll.m_rcStartArrow.bottom;
			m_vertScroll.m_rcSlider.bottom = m_vertScroll.m_rcSlider.top + sliderHeight;
			if(0 != m_vertScroll.m_curSliderPos)
			{
				m_vertScroll.m_curSliderPos = 0;
				m_vertScroll.CalcCurPixelPos();
				ScrollRowToFixPos(m_vertScroll.m_curPixelPos);
				Invalidate(FALSE);
			}
		}
		else if(m_vertScroll.m_rcSlider.bottom + vertShift > m_vertScroll.m_rcEndArrow.top)
		{
			int sliderHeight = m_vertScroll.m_rcSlider.Height();
			m_vertScroll.m_rcSlider.bottom = m_vertScroll.m_rcEndArrow.top;
			m_vertScroll.m_rcSlider.top = m_vertScroll.m_rcSlider.bottom - sliderHeight;

			if(m_vertScroll.m_validScope != m_vertScroll.m_curSliderPos)
			{
				m_vertScroll.m_curSliderPos = m_vertScroll.m_validScope;
				m_vertScroll.CalcCurPixelPos();
				ScrollRowToFixPos(m_vertScroll.m_curPixelPos);
				Invalidate(FALSE);
			}
		}
		else
		{
			m_vertScroll.m_rcSlider.DeflateRect(0,vertShift,0,-vertShift);
			m_vertScroll.m_curSliderPos += vertShift;
			m_vertScroll.CalcCurPixelPos();
			ScrollRowToFixPos(m_vertScroll.m_curPixelPos);
			Invalidate(FALSE);
		}
	}
	m_vertScroll.m_prePt = point;

	return;
}

int CMyCbSelWnd::ScrollRow(int pxVertShift)
{
	if(m_rowNum <= 0)
	{
		return -1;
	}

	if((pxVertShift > 0) && (m_vertScroll.m_curPixelPos + pxVertShift > m_totalRowH - m_rcBody.Height()))
	{
		pxVertShift -= (m_vertScroll.m_curPixelPos + pxVertShift - m_totalRowH + m_rcBody.Height());
	}

	if((pxVertShift < 0) && (m_vertScroll.m_curPixelPos + pxVertShift < 0))
	{
		pxVertShift -= (m_vertScroll.m_curPixelPos + pxVertShift);
	}

	if(0 == pxVertShift)
	{
		return 1;
	}

	int tempVertShift = pxVertShift;

	if(m_totalRowH > m_rcBody.Height())
	{
		if(-1 == m_rowIdStartShow)
		{
			m_rowIdStartShow = 0;
			m_iRowTopIndent = 0;
		}
		else
		{
			if(pxVertShift > 0)
			{
				int rowH = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH;
				if(rowH - m_iRowTopIndent > pxVertShift)
				{
					m_iRowTopIndent += pxVertShift;
				}
				else
				{
					int remainder = pxVertShift - (rowH - m_iRowTopIndent);
					for(int i = m_rowIdStartShow + 1;i < m_rowNum;remainder -= m_lnkItemForSelInfo[i]->m_iItemH,++i)
					{
						if(remainder < m_lnkItemForSelInfo[i]->m_iItemH)
						{
							m_rowIdStartShow = i;
							m_iRowTopIndent = remainder;
							break;
						}
					}
				}
			}
			else
			{
				pxVertShift = abs(pxVertShift);
				if(m_iRowTopIndent >= pxVertShift)
				{
					m_iRowTopIndent -= pxVertShift;
				}
				else
				{
					int remainder = pxVertShift - m_iRowTopIndent;
					for(int i = m_rowIdStartShow - 1;i >= 0;remainder -= m_lnkItemForSelInfo[i]->m_iItemH,--i)
					{
						//=号在这里至关重要,20211122,孙文郎
						if(remainder <= m_lnkItemForSelInfo[i]->m_iItemH)
						{
							m_rowIdStartShow = i;
							m_iRowTopIndent = m_lnkItemForSelInfo[i]->m_iItemH - remainder;
							break;
						}
					}
				}
			}
		}

		int sum = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH - m_iRowTopIndent;
		int i = m_rowIdStartShow + 1;
		for(;i < m_rowNum;++i)
		{
			sum += m_lnkItemForSelInfo[i]->m_iItemH;
			if(sum == m_rcBody.Height())
			{
				m_rowIdEndShow = i;
				m_iRowBottomIndent = 0;
				break;
			}
			else if(sum > m_rcBody.Height())
			{
				m_rowIdEndShow = i;
				m_iRowBottomIndent = sum - m_rcBody.Height();
				break;
			}
			else if(i + 1 == m_rowNum)
			{
				if(abs(sum - m_rcBody.Height()) < 2)
				{
					m_rowIdEndShow = i;
					m_iRowBottomIndent = 0;
				}
			}
		}
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 

	m_vertScroll.m_curPixelPos += tempVertShift;

	m_vertScroll.CalcSliderPos();
	Invalidate(FALSE);

	return 0;
}

int CMyCbSelWnd::ScrollRowToFixPos(int pxFixPos)
{
	if(m_rowNum <= 0)
	{
		return -1;
	}
	if(pxFixPos < 0)
	{
		return -2;
	}

	if(m_rowNum > 0)
	{
		int sum = 0;
		for(int i = 0;i < m_rowNum;++i)
		{
			sum += m_lnkItemForSelInfo[i]->m_iItemH;
			if(sum == pxFixPos)
			{
				m_rowIdStartShow = i;
				m_iRowTopIndent = 0;
				break;
			}
			else if(sum > pxFixPos)
			{
				m_rowIdStartShow = i;
				m_iRowTopIndent = m_lnkItemForSelInfo[i]->m_iItemH - (sum - pxFixPos);
				break;
			}
		}

		if(m_totalRowH > m_rcBody.Height())
		{
			if(-1 == m_rowIdStartShow)
			{
				m_rowIdStartShow = 0;
				m_iRowTopIndent = 0;
			}

			int newSum = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH - m_iRowTopIndent;
			int i = m_rowIdStartShow + 1;
			for(;i < m_rowNum;++i)
			{
				newSum += m_lnkItemForSelInfo[i]->m_iItemH;
				if(newSum == m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_iRowBottomIndent = 0;
					break;
				}
				else if(newSum > m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_iRowBottomIndent = newSum - m_rcBody.Height();
					break;
				}
				else if(i + 1 == m_rowNum)
				{
					if(abs(newSum - m_rcBody.Height()) < 2)
					{
						m_rowIdEndShow = i;
						m_iRowBottomIndent = 0;
					}
				}
			}
		}
		else
		{
			m_rowIdEndShow = m_rowNum - 1;
			m_iRowBottomIndent = 0;
		}
	}
	else
	{
		m_rowIdStartShow = -1;
		m_rowIdEndShow = -1;
		m_iRowTopIndent = 0;
		m_iRowBottomIndent = 0;
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow);

#ifdef _DEBUG
	if(m_rowIdEndShow < m_rowIdStartShow)
	{
		int tt = 0;
		++tt;
	}
#endif

	return 0;
}

CMySelWndDotPosInfo CMyCbSelWnd::CalcPtPos(CPoint pt)
{
	CMySelWndDotPosInfo dotPosInfo;
	dotPosInfo.m_pt = pt;
	if(!m_rcWnd.PtInRect(pt))
	{
		return dotPosInfo;
	}

	if(m_rowNum < 1)
	{
		return dotPosInfo;
	}

	if(m_rcBody.PtInRect(pt))	//pt在body内
	{
		dotPosInfo.m_eDotPosType = EMSWDPT_BODY;

		int sum = m_lnkItemForSelInfo[m_rowIdStartShow]->m_iItemH - m_iRowTopIndent + m_pxFrameW;
		if(pt.y <= sum)
		{
			dotPosInfo.m_rowId = m_rowIdStartShow;
			return dotPosInfo;
		}
		else
		{
			for(int i = m_rowIdStartShow + 1;i <= m_rowIdEndShow;++i)
			{
				sum += m_lnkItemForSelInfo[i]->m_iItemH;
				if(pt.y <= sum)
				{
					dotPosInfo.m_rowId = i;
					return dotPosInfo;
				}
			}
		}
	}
	else if(m_vertScroll.m_bScrollNeedShow && m_vertScroll.m_rcScroll.PtInRect(pt))
	{
		dotPosInfo.m_eDotPosType = EMSWDPT_VERTSCROLL;
		dotPosInfo.m_rowId = -1;
		return dotPosInfo;
	}
	else if(0 == pt.x || 0 == pt.y || m_rcWnd.right == pt.x || m_rcWnd.bottom == pt.y)
	{
		dotPosInfo.m_eDotPosType = EMSWDPT_FRAME;
		dotPosInfo.m_rowId = -1;
		return dotPosInfo;
	}

	return dotPosInfo;
}

int CMyCbSelWnd::SetRowSingleSelected(int rowId,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(rowId >= m_lnkItemForSelInfo.GetSize())
	{
		return -3;
	}

	int iIndex = 0;
	Node<CItemForSelInfo>* pNode = m_lnkItemForSelInfo.GetHead();
	while(NULL != pNode)
	{
		if(iIndex == rowId)
		{
			pNode->data.SetSelectState(ESS_SELECTED);
		}
		else
		{
			if(ESS_SELECTED == pNode->data.m_eSelState)
			{
				pNode->data.SetSelectState(ESS_NORMAL);
			}
		}
		pNode = pNode->pNext;
		++iIndex;
	}
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyCbSelWnd::GetRowString(int rowId,CString& strText)
{
	strText = _T("");
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	strText = m_lnkItemForSelInfo[rowId]->m_strText;
	return 0;
}

int CMyCbSelWnd::InsertRow(int rowId,CString strText,BOOL bRedraw /*= TRUE*/)
{
	if(rowId < 0)
	{
		return -1;
	}
	
	if(rowId > m_rowNum)
	{
		rowId = m_rowNum;
	}

	if(!m_bAllowSameItem)
	{
		for(int i = 0;i < m_rowNum;++i)
		{
			if(strText == m_lnkItemForSelInfo[i]->m_strText)
			{
				return -2;
			}
		}
	}

	CItemForSelInfo itemForSelInfo;
	itemForSelInfo.m_strText = strText;
	itemForSelInfo.m_iItemH = m_iDefRowH;
	m_lnkItemForSelInfo.InsertAtPre(rowId,itemForSelInfo);
	++m_rowNum;
	CalcTotalRowHeight();
	CalcRowToShow();

	//更新垂直滚动条当前位置
	if(m_totalRowH > m_rcBody.Height())
	{
		m_vertScroll.m_curPixelPos = 0;
		for(int i = 0;i < m_rowIdStartShow;++i)
		{
			m_vertScroll.m_curPixelPos += m_lnkItemForSelInfo[i]->m_iItemH;
		}
		if(-1 != m_rowIdStartShow)
		{
			m_vertScroll.m_curPixelPos += m_iRowTopIndent;
		}
	}
	else
	{
		m_vertScroll.m_curPixelPos = 0;
	}
	CalcScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyCbSelWnd::DeleteRow(int rowId,BOOL bRedraw /*= TRUE*/)
{
	if(rowId < 0)
	{
		return -1;
	}
	if(rowId >= m_rowNum)
	{
		return -2;
	}

	int rowH = m_lnkItemForSelInfo[rowId]->m_iItemH;

	m_lnkItemForSelInfo.DeleteAt(rowId);
	--m_rowNum;

	CalcTotalRowHeight();

	if(rowId < m_rowIdStartShow)
	{
		CalcRowToShow(m_rowIdStartShow - 1);
	}
	else
	{
		CalcRowToShow(m_rowIdStartShow);
	}

	//更新垂直滚动条当前位置
	if(m_totalRowH > m_rcBody.Height())
	{
		m_vertScroll.m_curPixelPos = 0;
		for(int i = 0;i < m_rowIdStartShow;++i)
		{
			m_vertScroll.m_curPixelPos += m_lnkItemForSelInfo[i]->m_iItemH;
		}
		if(-1 != m_rowIdStartShow)
		{
			m_vertScroll.m_curPixelPos += m_iRowTopIndent;
		}
	}
	else
	{
		m_vertScroll.m_curPixelPos = 0;
	}
	CalcScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyCbSelWnd::DeleteAll(BOOL bRedraw /*= TRUE*/)
{
	m_lnkItemForSelInfo.Clear();
	m_rowNum = 0;

	m_totalRowH = 0;

	//更新垂直滚动条当前位置
	m_vertScroll.m_curPixelPos = 0;
	CalcScrollPos();

	m_rowIdStartShow = -1;
	m_rowIdEndShow = -1;
	m_iRowTopIndent = 0;
	m_iRowBottomIndent = 0;

	m_preDotPosInfo.Clear();
	m_curDotPosInfo.Clear();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyCbSelWnd::SetSelectedRow(CString strSelectedRow,BOOL bRedraw /*= FALSE*/)
{
	for(int i = 0;i < m_rowNum;++i)
	{
		if(strSelectedRow == m_lnkItemForSelInfo[i]->m_strText)
		{
			m_lnkItemForSelInfo[i]->m_eSelState = ESS_SELECTED;
		}
		else if(ESS_DISABLE != m_lnkItemForSelInfo[i]->m_eSelState)
		{
			m_lnkItemForSelInfo[i]->m_eSelState = ESS_NORMAL;
		}
	}

	if(bRedraw && NULL != GetSafeHwnd()) 
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyCbSelWnd::SetRowSelected(CString strSelectedRow,BOOL bRedraw /*= FALSE*/)
{
	for(int i = 0;i < m_rowNum;++i)
	{
		if(strSelectedRow == m_lnkItemForSelInfo[i]->m_strText)
		{
			m_lnkItemForSelInfo[i]->m_eSelState = ESS_SELECTED;
			break;
		}
	}

	if(bRedraw && NULL != GetSafeHwnd()) 
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyCbSelWnd::GetFirstSelectedRow(CString strSelectedRow)
{
	for(int i = 0;i < m_rowNum;++i)
	{
		if(strSelectedRow == m_lnkItemForSelInfo[i]->m_strText)
		{
			return i;
		}
	}

	return -1;
}

void CMyCbSelWnd::SetBeAllowSameItem(BOOL bAllowSamItem)
{
	m_bAllowSameItem = bAllowSamItem;
}

BOOL CMyCbSelWnd::GetBeAllowSameItem()
{
	return m_bAllowSameItem;
}

int CMyCbSelWnd::InitVertScroll()
{
	m_vertScroll.m_scrollW = 30;
	m_vertScroll.m_eScrollType = EST_VERT;
	m_vertScroll.m_arrowWH = 20;
	m_vertScroll.m_scrollW = 20;
	return 0;
}

void CMyCbSelWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bEnableDelete && m_curDotPosInfo.m_rowId != -1)
	{
		CLanguageInfo* pLanguage = CLanguageInfo::GetInstance();
		CString strText = pLanguage->GetText(45);

		CMenu menu1;
		menu1.CreatePopupMenu();
		menu1.AppendMenu(MF_STRING,ID_DELETE_COMBO_ROW_STRING, strText);

		CPoint pt;
		GetCursorPos(&pt);
		menu1.TrackPopupMenu(TPM_RIGHTBUTTON, pt.x,pt.y,this);
		menu1.DestroyMenu();
	}

	__super::OnRButtonUp(nFlags, point);
}

void CMyCbSelWnd::OnDeleteRowString()
{
	// AfxMessageBox(_T("Test1"));
	m_pParent->SendMessage(WM_DELETE_COMBO_ROW_STRING,m_curDotPosInfo.m_rowId);
	DeleteRow(m_curDotPosInfo.m_rowId);
	m_curDotPosInfo.m_rowId = -1;
	m_pParent->SendMessage(WM_RESET_WINDOW_SIZE);

	return;
}

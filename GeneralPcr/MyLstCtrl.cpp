// PcrSegmentExpWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MyLstCtrl.h"
#include "ColorSelectDlg.h"

// CMyLstCtrl

#define MYLST_CB_TYPETAG	10

IMPLEMENT_DYNAMIC(CMyLstCtrl, CDialogEx)
CMyLstCtrl::CMyLstCtrl(): m_image(_T(".\\BkPic\\CheckBox.png")),m_img2(_T(".\\BkPic\\CheckBox2.png")),m_clrSelDlg(this)
{
	m_rowNum = 0;
	m_colNum = 0;
	m_colIdStartShow = -1;
	m_colIdEndShow = -1;
	m_rowIdStartShow = -1;
	m_rowIdEndShow = -1;
	m_pxFrame = 1;
	m_pxRowColLine = 1;
	m_ssColLeftIndent = 0;
	m_esColRightIndent = 0;
	m_ssRowTopIndent = 0;
	m_esRowBottomIndent = 0;

	m_pxColW = 60;	
	m_pxRowH = 28;	
	m_pxHeaderH = 88;
	m_bUseFixedHW = TRUE;
	m_clrBk = Color(0XFF,0XFF,0XFF);
	m_clrHeaderBk = Color(0XF1,0XF4,0XF9);
	m_clrBodyBk = Color(0XFF,0XFF,0XFE);
	m_clrFrame = Color(0XD8,0XD8,0XD8);
	m_clrRowColLine = RGB(0XD8,0XD8,0XD8);
	m_clrHeaderText = RGB(0X39,0X45,0X65);
	m_bGradientHeader = FALSE;
	m_clrGraHeadStart = Color(0XAA,0XAB,0XAC);
	m_clrGraHeadEnd = Color(0XBA,0XBB,0XBC);
	m_bEditable = TRUE;

	m_bShowHoriScroll = FALSE;
	m_bShowVertScroll = FALSE;

	m_totalColW = 60;
	m_totalRowH = 30;
	m_cdcW = 60;
	m_cdcH = 160;

	m_horiScroll.m_scrollH = 30;
	m_vertScroll.m_scrollW = 30;
	m_horiScroll.m_eScrollType = EST_HORI;
	m_vertScroll.m_eScrollType = EST_VERT;

	m_eScrollHVCM = ESHVCM_HORI;
	m_bShowFrameLine = TRUE;
	m_bShowRowColLine = TRUE;
	m_lnkHistoryDotPosInfo.SetLimitMaxParam(TRUE,100);
	
	m_rowIdSeparatorStart = -1;
	m_rowIdSeparatorEnd = -1;
	m_esSeparatorBottomIndent = 0;
	m_bSingleSelect = FALSE;
	m_bAllowCancelRowSelect = FALSE; 
	m_strLstTitle = _T("");

	m_clrHeadBodySepa = RGB(197,197,197);
	m_bTellParentRowSelChange = FALSE;

	m_clrFourAngleBk = RGB(0XFF,0XFF,0XFF);
	m_bRoundRectStyle = TRUE;
	m_iRoundAngleSize = 8;
	m_clrIndepent = Color(0XF1,0XF4,0XF9);

	m_iInputEdTag = 1;
	m_iInputCbTag = 2;
	m_bShowRowSelState = TRUE;
	m_bScrollInsideFrame = FALSE;

	m_clrRowBkSel = RGB(203,233,235);
	m_clrRowBkUnSel = RGB(255,255,254);

	InitFontHeader(16);
	InitFontTitle(16);

	SetScrollArrowWH(4,8);

	m_bLeftBtDown = FALSE;

	m_bUseLargeChkIcon = FALSE;

	m_bUseFixedIdpd = FALSE;

	m_bCbEndEdit = FALSE;
}

CMyLstCtrl::~CMyLstCtrl()
{
	
}

BEGIN_MESSAGE_MAP(CMyLstCtrl, CDialogEx)
   ON_WM_ERASEBKGND()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_RBUTTONDOWN()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_MOUSEWHEEL()
   ON_WM_PAINT()
   ON_WM_CREATE()
   ON_WM_SIZE()
   ON_MESSAGE(WM_FUNCEDITKILLFOCUS,&CMyLstCtrl::OnMsgFuncEditKillFocus)
   ON_MESSAGE(WM_EDITENTERPUSHED,&CMyLstCtrl::OnMsgEditEnterPushed)
   ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,&CMyLstCtrl::OnMsgTellParentTextChanged)
   ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,&CMyLstCtrl::OnMsgTellParentSetNewText)
   //ON_MESSAGE(WM_TOUCH,&CMyLstCtrl::OnMsgTouch)
  // ON_MESSAGE(WM_GESTURE,&CMyLstCtrl::OnMsgGesture)
   //ON_MESSAGE(WM_HSCROLL,&CMyLstCtrl::OnMsgHorizonScroll)
   //ON_MESSAGE(WM_VSCROLL,&CMyLstCtrl::OnMsgVerticalScroll)
   ON_MESSAGE(WM_SELECTNEWCOLOR,&CMyLstCtrl::OnMsgSelectNewColor)
   ON_MESSAGE(WM_DELETE_COMBO_ROW_STRING,&CMyLstCtrl::OnMsgDeleteComboRowString)
END_MESSAGE_MAP()

void CMyLstCtrl::OnPaint()
{
#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	CPaintDC dc(this); 

	CRect rcClient; 
	GetClientRect(&rcClient);

	CDC memDC,memDCInside;
	memDC.CreateCompatibleDC(&dc);
	memDCInside.CreateCompatibleDC(&dc);

	CBitmap bitMap,bitmapInside;
	bitMap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);
	bitmapInside.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmapInside = memDCInside.SelectObject(&bitmapInside);

	if(m_bScrollInsideFrame)
	{
		memDCInside.FillSolidRect(m_rcLst.left,m_rcLst.top,m_rcLst.Width(),m_rcLst.Height(),m_clrFourAngleBk);
		DrawRoundRectangleNew(&memDCInside,m_rcLst.left,m_rcLst.top,m_rcLst.Width() - 1,m_rcLst.Height() - 1,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,TRUE,m_clrFrame,m_pxFrame,TRUE,m_clrIndepent,1);
		memDC.FillSolidRect(m_rcLst.left,m_rcLst.top,m_rcLst.Width(),m_rcLst.Height(),m_clrFourAngleBk);
	}

	CFont* pOldFont = memDC.SelectObject(&m_fontSet);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(m_txClr1);

	//绘制边框
	if(m_bRoundRectStyle && !m_bScrollInsideFrame)
	{
		CRect rcTemp = CRect(0,0,m_rcBody.right + m_pxFrame,m_rcBody.bottom + m_pxFrame);
		memDC.FillSolidRect(rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),m_clrFourAngleBk);
		DrawRoundRectangleNew(&memDC,rcTemp.left,rcTemp.top,rcTemp.Width() - 1,rcTemp.Height() - 1,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,TRUE,m_clrFrame,m_pxFrame,TRUE,m_clrIndepent,1);
	}
	else if(!m_bScrollInsideFrame)
	{
		if(m_bShowFrameLine)
		{
			CPen penFrame;
			penFrame.CreatePen(PS_SOLID,m_pxFrame,RGB(m_clrFrame.GetR(),m_clrFrame.GetG(),m_clrFrame.GetB()));
			CPen* pOldPen = memDC.SelectObject(&penFrame);
			memDC.Rectangle(&rcClient);
			memDC.SelectObject(pOldPen);
		}
		memDC.FillSolidRect(rcClient.left + m_pxFrame,rcClient.top + m_pxFrame,rcClient.Width() - m_pxFrame * 2,rcClient.Height() - m_pxFrame * 2,RGB(m_clrBk.GetR(),m_clrBk.GetG(),m_clrBk.GetB()));
	}
	
	DrawHeader(&memDC);
	DrawBody(&memDC);

	if(m_bRoundRectStyle)
	{
		DrawHoriScrollStyle2(&memDC);
		DrawVertScrollStyle2(&memDC);
	}
	else
	{
		DrawHoriScroll(&memDC);
		DrawVertScroll(&memDC);
	}
	
	if(m_bScrollInsideFrame)
	{
		int bRet1 = memDC.TransparentBlt(0,0,m_rcLst.Width(),m_rcLst.Height(),&memDCInside,0,0,m_rcLst.Width(),m_rcLst.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
		int bRet2 = memDCInside.TransparentBlt(0,0,m_rcLst.Width(),m_rcLst.Height(),&memDC,0,0,m_rcLst.Width(),m_rcLst.Height(),m_clrFourAngleBk);
		dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDCInside,0,0,SRCCOPY); 
	}
	else
	{
		dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY); 
	}  

	memDCInside.SelectObject(pOldBitmapInside);
	bitmapInside.DeleteObject();
	memDCInside.DeleteDC();

	memDC.SelectObject(pOldBitmap);
	memDC.SelectObject(pOldFont);
	bitMap.DeleteObject();
	memDC.DeleteDC();

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return;
}

BOOL CMyLstCtrl::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	//RegisterTouchWindow(TRUE,TWF_WANTPALM);

	return TRUE;
}

int CMyLstCtrl::DrawHeader(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	if(m_colNum < 1)
	{
		return -2;
	}

	if(m_lnkTitleInfo.GetSize() != m_colNum)
	{
		return -3;
	}

	CDC memDcHeader;
	BOOL bRet3 = memDcHeader.CreateCompatibleDC(pDC);

	CRect tempRcHeader(0,0,m_cdcW,m_rcHeader.Height());

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC,m_cdcW,m_rcHeader.Height());
	CBitmap* pOldBitmap = memDcHeader.SelectObject(&bitMap);
	memDcHeader.SetBkMode(TRANSPARENT);

	if(m_bGradientHeader)
	{
		Graphics graphics(memDcHeader.m_hDC);
		LinearGradientBrush linGrBrush(RectF(0,0,m_cdcW,m_pxHeaderH),m_clrGraHeadStart,m_clrGraHeadEnd,LinearGradientModeVertical);
		Status sRet = graphics.FillRectangle(&linGrBrush,0,0,m_cdcW,m_pxHeaderH);
		graphics.ReleaseHDC(memDcHeader.m_hDC);
	}
	else
	{
		memDcHeader.FillSolidRect(&tempRcHeader,RGB(m_clrHeaderBk.GetR(),m_clrHeaderBk.GetG(),m_clrHeaderBk.GetB()));
	}

	CFont* pOldFont = NULL;
	memDcHeader.SetTextColor(m_clrHeaderText);
	memDcHeader.SetBkMode(TRANSPARENT);

	//绘制标题
	int titleH = 0;
	if(_T("") != m_strLstTitle)
	{
		titleH = m_pxHeaderH - 28;
		pOldFont = memDcHeader.SelectObject(&m_fontSetTitle);
		memDcHeader.DrawText(m_strLstTitle,CRect(6,5,140,22),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
		memDcHeader.SelectObject(pOldFont);
	}

	CRect tempRc;
	tempRc.top = titleH;
	tempRc.bottom = m_pxHeaderH;
	pOldFont = memDcHeader.SelectObject(&m_fontSetHeader);

	for(int i = m_colIdStartShow;i <= m_colIdEndShow;++i)
	{
		tempRc.left = GetColStartDrawInMemoryHoriPixel(i);
		tempRc.right = tempRc.left + m_lnkTitleInfo[i]->m_colWidth;
		CTitleInfo& titleInfo = m_lnkTitleInfo.GetAt(i)->data;
		COLORREF clrTemp = 0;
		if(titleInfo.m_bSelected)
		{
			clrTemp = titleInfo.m_clrBkSlected;
		}
		else
		{
			clrTemp = titleInfo.m_clrBkUnselected;
		}
		memDcHeader.FillSolidRect(&tempRc,clrTemp);
		memDcHeader.DrawText(titleInfo.m_strTitle,&tempRc,titleInfo.m_dwTextAlign);
	}

	//绘制Header和Body分隔线
	CPen penLine;
	penLine.CreatePen(PS_SOLID,2,m_clrHeadBodySepa);
	CPen* pOldPen = memDcHeader.SelectObject(&penLine);
	
	POINT pt;
	pt.x = 1;
	pt.y = m_pxHeaderH - 1;
	memDcHeader.MoveTo(pt);
	pt.x = m_cdcW - 1;
	memDcHeader.LineTo(pt);

	memDcHeader.SelectObject(pOldPen);

	BOOL bRet1 = FALSE,bRet2 = FALSE;
	DWORD dwLastError = 0;

	int iRealW = m_rcHeader.Width();
	if(m_cdcW < iRealW)
	{
		iRealW = m_cdcW;
	}
	if(m_bRoundRectStyle && !m_bScrollInsideFrame)
	{
		//检查使用稳定性
		bRet1 = memDcHeader.TransparentBlt(m_ssColLeftIndent,0,iRealW,m_rcHeader.Height(),pDC,m_rcHeader.left,m_rcHeader.top,iRealW,m_rcHeader.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
		bRet2 = pDC->TransparentBlt(m_rcHeader.left,m_rcHeader.top,iRealW,m_rcHeader.Height(),&memDcHeader,m_ssColLeftIndent,0,iRealW,m_rcHeader.Height(),m_clrFourAngleBk);
		dwLastError = GetLastError();
	}
	else
	{
		pDC->BitBlt(m_rcHeader.left,m_rcHeader.top,iRealW,m_rcHeader.Height(),&memDcHeader,m_ssColLeftIndent,0,SRCCOPY);
	}
	
	memDcHeader.SelectObject(pOldFont);
	memDcHeader.SelectObject(pOldBitmap);

	memDcHeader.DeleteDC();
	bitMap.DeleteObject();

	return 0;
}

int CMyLstCtrl::DrawBody(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	if(m_colNum < 1)
	{
		return -2;
	}

	if(m_lnkTitleInfo.GetSize() != m_colNum)
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
	bitMap.CreateCompatibleBitmap(pDC,m_cdcW,tempCdcH);
	CBitmap* pOldBitmap = memDcBody.SelectObject(&bitMap);

	memDcBody.FillSolidRect(0,0,m_cdcW,tempCdcH,RGB(m_clrBodyBk.GetR(),m_clrBodyBk.GetG(),m_clrBodyBk.GetB()));

	CRect tempRc;
	tempRc.top = m_pxHeaderH;
	CFont* pOldFont = memDcBody.SelectObject(&m_fontSet);
	memDcBody.SetTextColor(m_txClr1);
	memDcBody.SetBkMode(TRANSPARENT);

	CPen penLine;
	penLine.CreatePen(PS_SOLID,m_pxRowColLine,m_clrRowColLine);
	CPen* pOldPen = memDcBody.SelectObject(&penLine);

	Graphics graphics(memDcBody.m_hDC);
	ImageAttributes ImgAtt;
	ImgAtt.SetWrapMode(WrapModeTileFlipXY);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	if(-1 == m_rowIdStartShow || m_cdcH < m_rcBody.Height())
	{
		if(m_bShowRowColLine)
		{
			//绘制水平分割线
			for(int i = m_rowIdSeparatorStart;i <= m_rowIdSeparatorEnd;++i)
			{
				POINT pt;
				pt.x = 0;
				pt.y = (i + 1) * m_pxRowH - m_pxRowColLine;

				//如果底部分隔线离列表框底部太近，则不绘制该直线
				if(abs(pt.y - m_rcBody.Height()) > 8)
				{
					memDcBody.MoveTo(pt);
					pt.x = m_cdcW;
					memDcBody.LineTo(pt);
				}
			}

			//绘制垂直分割线
			for(int j = m_colIdStartShow;j <= m_colIdEndShow;++j)
			{
				if(j != m_colIdEndShow)
				{
					POINT pt;
					pt.x = GetColStartDrawInMemoryHoriPixel(j + 1) - m_pxRowColLine;
					pt.y = 0;
					memDcBody.MoveTo(pt);
					pt.y = tempCdcH;
					memDcBody.LineTo(pt);
				}
			}
		}
	}

	if(-1 != m_rowIdStartShow)
	{
		for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
		{
			CRowInfo& rowInfo = *m_lnkRowInfo[i];

			tempRc.top = GetRowStartDrawInMemoryVertPixel(i);
			tempRc.left = 0;
			tempRc.right = m_cdcW;
			tempRc.bottom = tempRc.top + rowInfo.m_rowH;
			if(rowInfo.m_bSelected && m_bShowRowSelState)
			{
				memDcBody.FillSolidRect(&tempRc,m_clrRowBkSel);
			}
			else
			{
				memDcBody.FillSolidRect(&tempRc,m_clrRowBkUnSel);
			}

			for(int j = m_colIdStartShow;j <= m_colIdEndShow;++j)
			{
				CTitleInfo& titleInfo = m_lnkTitleInfo.GetAt(j)->data;
				CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(i)->data.GetAt(j)->data;
				tempRc.left = GetColStartDrawInMemoryHoriPixel(j);
				tempRc.right = tempRc.left + m_lnkTitleInfo[j]->m_colWidth;
				if(j == m_colIdEndShow && m_vertScroll.m_bScrollNeedShow)
				{
					tempRc.right -= m_vertScroll.m_scrollW;
					if(tempRc.right <= tempRc.left)
					{
						tempRc.right = tempRc.left + m_lnkTitleInfo[j]->m_colWidth;
					}
				}
				
				cellInfo.m_rcInCDc = tempRc;
				cellInfo.m_rcInCDc.top += (m_rcHeader.Height() + m_pxFrame);
				if(i != m_rowIdStartShow)
				{
					cellInfo.m_rcInCDc.top += 1; //“+1”是因为每一行底部负责画分割线,第一显示行除外
				}

				cellInfo.m_rcInCDc.bottom += (m_rcHeader.Height() + m_pxFrame - 1);
				cellInfo.m_rcInCDc.left += 1;	//“+1”是因为每一列右侧负责画分割线
				if(cellInfo.m_rcInCDc.right > m_rcBody.right)	//防止“垂直滚动条存在时，最右侧一列双击鼠标左键编辑内容时，编辑框越界”
				{
					cellInfo.m_rcInCDc.right = m_rcBody.right;
				}

				cellInfo.m_rcInCDc.InflateRect(m_ssColLeftIndent,m_ssRowTopIndent,-m_ssColLeftIndent,-m_ssRowTopIndent);

				if(!m_bEditable || rowInfo.m_bDisabled || titleInfo.GetBeDisabled() || cellInfo.m_bDisabled)
				{
					CRect rcDisabledBk = tempRc;
					if(i != m_rowIdStartShow)
					{
						rcDisabledBk.DeflateRect(1,1,1,1);
					}
					else
					{
						rcDisabledBk.DeflateRect(1,0,1,1);
					}
					memDcBody.FillSolidRect(&rcDisabledBk,cellInfo.GetColorDisabled());
				}
				
				if(cellInfo.GetCheckable())
				{
					Image* pImg = NULL;
					if(m_bUseLargeChkIcon)
					{
						pImg = &m_img2;
					}
					else
					{
						pImg = &m_image;
					}

					int sonPicNums = 4,tempInt = 0;
					int iconW = pImg->GetWidth() / sonPicNums;

					Rect rect;
					tempInt = tempRc.left + (tempRc.Width() - iconW) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.X = tempInt;

					tempInt = tempRc.top + (tempRc.Height() - pImg->GetHeight()) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.Y = tempInt;

					rect.Width = iconW;
					rect.Height = pImg->GetHeight();

					int iStartX = 0;
					if(!m_bEditable || rowInfo.m_bDisabled || cellInfo.m_bDisabled)
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW * 3;
						}
						else
						{
							iStartX = iconW * 2;
						}
					}
					else
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW;
						}
					}
					graphics.DrawImage(pImg,rect,iStartX,0,iconW,pImg->GetHeight(),UnitPixel,&ImgAtt);
				}
				else if(titleInfo.m_bCheckFunc)
				{
					Image* pImg = NULL;
					if(m_bUseLargeChkIcon)
					{
						pImg = &m_img2;
					}
					else
					{
						pImg = &m_image;
					}

					int sonPicNums = 4,tempInt = 0;
					int iconW = pImg->GetWidth() / sonPicNums;
					
					Rect rect;
					tempInt = tempRc.left + (tempRc.Width() - iconW) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.X = tempInt;

					tempInt = tempRc.top + (tempRc.Height() - pImg->GetHeight()) / 2;
					if(tempInt < 0)
					{
						tempInt = 0;
					}
					rect.Y = tempInt;

					rect.Width = iconW;
					rect.Height = pImg->GetHeight();

					int iStartX = 0;
					if(!m_bEditable || rowInfo.m_bDisabled || cellInfo.m_bDisabled)
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW * 3;
						}
						else
						{
							iStartX = iconW * 2;
						}	
					}
					else
					{
						if(cellInfo.GetItemCheckState())
						{
							iStartX = iconW;
						}
					}
					graphics.DrawImage(pImg,rect,iStartX,0,iconW,pImg->GetHeight(),UnitPixel,&ImgAtt);
				}
				else if(titleInfo.m_bColorFunc)
				{
					COLORREF clr = 0;
					int clrW = 0,clrH = 0,iRoundAngle = 2;;
					cellInfo.GetItemColorParam(clr,clrW,clrH,iRoundAngle);
					int iTempIntLeft1 = tempRc.left + (tempRc.Width() - clrW) / 2, iTempIntTop1 = tempRc.top + (tempRc.Height() - clrH) / 2;
					cellInfo.m_rcClrBlock = CRect(iTempIntLeft1,iTempIntTop1,iTempIntLeft1 + clrW,iTempIntTop1 + clrH);
					Color clrTemp;
					clrTemp.SetFromCOLORREF(cellInfo.GetItemColor());
					DrawRoundRectangleNew(&memDcBody,cellInfo.m_rcClrBlock.left,cellInfo.m_rcClrBlock.top,cellInfo.m_rcClrBlock.Width() - 1,cellInfo.m_rcClrBlock.Height() - 1,iRoundAngle,iRoundAngle,iRoundAngle,iRoundAngle,TRUE,clrTemp,1,TRUE,clrTemp);
					cellInfo.m_rcClrBlock.DeflateRect(0,m_pxHeaderH,0,-m_pxHeaderH);
				}
				else
				{
					CRect rcText = tempRc;
					rcText.left += 2;
					CRect rcRightRgn = tempRc;
					rcRightRgn.right -= 2;
					if(cellInfo.m_bCbAble)
					{
						SolidBrush brush(titleInfo.m_arrowInfo.m_clrArrow);
						rcText.left += titleInfo.m_iCbTxLeftIndent;
						rcText.right -= titleInfo.m_iCbTxRightIndent;
						rcRightRgn.left = rcRightRgn.right - titleInfo.m_iCbTxRightIndent;
						titleInfo.m_arrowInfo.CalcPtData(rcRightRgn);
						graphics.FillPolygon(&brush,titleInfo.m_arrowInfo.m_aryPtTriangle,3,FillModeAlternate);
						memDcBody.DrawText(cellInfo.GetItemText(),&rcText,titleInfo.m_dwTextAlign);
					}
					else if(titleInfo.m_bCbtable)
					{
						SolidBrush brush(titleInfo.m_arrowInfo.m_clrArrow);
						rcText.left += titleInfo.m_iCbTxLeftIndent;
						rcText.right -= titleInfo.m_iCbTxRightIndent;
						rcRightRgn.left = rcRightRgn.right - titleInfo.m_iCbTxRightIndent;
						titleInfo.m_arrowInfo.CalcPtData(rcRightRgn);
						graphics.FillPolygon(&brush,titleInfo.m_arrowInfo.m_aryPtTriangle,3,FillModeAlternate);
						memDcBody.DrawText(cellInfo.GetItemText(),&rcText,titleInfo.m_dwTextAlign);
					}
					else
					{
						rcText.left += cellInfo.m_iTxLeftIndent;
						rcText.right -= cellInfo.m_iTxRightIndent;
						memDcBody.DrawText(cellInfo.GetItemText(),&rcText,cellInfo.m_dwTextAlign);
					}
				} 
			}
		}

		if(m_bShowRowColLine)
		{
			//绘制水平分割线
			for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
			{
				if(i != m_rowIdEndShow)
				{
					POINT pt;
					pt.x = 0;
					pt.y = GetRowStartDrawInMemoryVertPixel(i + 1) - m_pxRowColLine;
					memDcBody.MoveTo(pt);
					pt.x = m_cdcW;
					memDcBody.LineTo(pt);
				}
				else if(m_cdcH < m_rcBody.Height() && abs(m_cdcH - m_rcBody.Height()) > 8)
				{
					POINT pt;
					pt.x = 0;
					pt.y = GetRowStartDrawInMemoryVertPixel(i + 1) - m_pxRowColLine;
					memDcBody.MoveTo(pt);
					pt.x = m_cdcW;
					memDcBody.LineTo(pt);
				}
			}

			//绘制垂直分割线
			for(int j = m_colIdStartShow;j <= m_colIdEndShow;++j)
			{
				if(j != m_colIdEndShow)
				{
					POINT pt;
					pt.x = GetColStartDrawInMemoryHoriPixel(j + 1) - m_pxRowColLine;
					pt.y = 0;
					memDcBody.MoveTo(pt);
					pt.y = m_cdcH;
					memDcBody.LineTo(pt);
				}
			}
		}
	}

	graphics.ReleaseHDC(memDcBody.m_hDC);

	if(m_bRoundRectStyle && !m_bScrollInsideFrame)
	{
		//检查使用稳定性
		BOOL bRet1 = memDcBody.TransparentBlt(m_ssColLeftIndent,m_ssRowTopIndent,m_rcBody.Width(),m_rcBody.Height(),pDC,m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
		BOOL bRet2 = pDC->TransparentBlt(m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),&memDcBody,m_ssColLeftIndent,m_ssRowTopIndent, m_rcBody.Width(),m_rcBody.Height(),m_clrFourAngleBk);
		if(!bRet1 || !bRet2)
		{
			int tt = 0;
			++tt;
		}
	}
	else
	{
		pDC->BitBlt(m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),&memDcBody,m_ssColLeftIndent,m_ssRowTopIndent,SRCCOPY);   
	}
	
	memDcBody.SelectObject(&pOldPen);
	memDcBody.SelectObject(pOldFont);
	memDcBody.SelectObject(pOldBitmap);

	memDcBody.DeleteDC();
	bitMap.DeleteObject();

	return 0;
}

int CMyLstCtrl::DrawHoriScroll(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}
	if(!m_horiScroll.m_bScrollNeedShow)
	{
		return 1;
	}

#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	pDC->FillSolidRect(&m_horiScroll.m_rcScroll,m_horiScroll.m_clrBk);

	CBrush lineBrush;
	lineBrush.CreateSolidBrush(RGB(197,197,197));
	pDC->FrameRect(&m_horiScroll.m_rcScroll,&lineBrush);

	CRect rcTemp;
	rcTemp = m_horiScroll.m_rcStartArrow;
	rcTemp.InflateRect(m_pxFrame,0,0,m_pxFrame);
	pDC->FrameRect(&rcTemp,&lineBrush);

	//绘制左向箭头
	Graphics graphics(pDC->m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	SolidBrush brush(Color(128,128,128));
	int triangleW = 7,triangleH = 9;
	int tempInt1 = rcTemp.right - (rcTemp.Width() - triangleW) / 2;
	int tempInt2 = rcTemp.top + (rcTemp.Height() - triangleH) / 2;
	Point pt1(tempInt1,tempInt2);
	Point pt2(tempInt1,tempInt2 + triangleH);
	Point pt3(tempInt1 - triangleW,tempInt2 + triangleH / 2);

	Point crvPts[3] = {pt1,pt2,pt3};
	graphics.FillPolygon(&brush,crvPts,3,FillModeAlternate);

	rcTemp = m_horiScroll.m_rcEndArrow;
	if(ESHVCM_HORI == m_eScrollHVCM)
	{
		rcTemp.InflateRect(0,0,m_pxFrame,m_pxFrame);
	}
	else
	{
		rcTemp.InflateRect(0,0,0,m_pxFrame);
	}
	pDC->FrameRect(&rcTemp,&lineBrush);

	//绘制右向箭头
	tempInt1 = rcTemp.left + (rcTemp.Width() - triangleW) / 2;
	tempInt2 = rcTemp.top + (rcTemp.Height() - triangleH) / 2;
	Point pt12(tempInt1,tempInt2);
	Point pt22(tempInt1,tempInt2 + triangleH);
	Point pt32(tempInt1 + triangleW,tempInt2 + triangleH / 2);

	Point crvPts2[3] = {pt12,pt22,pt32};
	graphics.FillPolygon(&brush,crvPts2,3,FillModeAlternate);
	graphics.ReleaseHDC(pDC->m_hDC);

	CPen penSliderFrame;
	penSliderFrame.CreatePen(PS_SOLID,1,RGB(164,164,164));
	CPen* pOldPen = pDC->SelectObject(&penSliderFrame);
	rcTemp = m_horiScroll.m_rcSlider;
	rcTemp.DeflateRect(0,1,0,-1);
	pDC->Rectangle(&rcTemp);
	pDC->SelectObject(pOldPen);
	
#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return 0;
}

int CMyLstCtrl::DrawVertScroll(CDC* pDC)
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
	rcTemp.InflateRect(0,m_pxFrame,m_pxFrame,0);
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
	if(ESHVCM_HORI == m_eScrollHVCM)
	{
		rcTemp.InflateRect(0,0,m_pxFrame,0);
	}
	else
	{
		rcTemp.InflateRect(0,0,m_pxFrame,m_pxFrame);
	}
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

int CMyLstCtrl::DrawHoriScrollStyle2(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}
	if(!m_horiScroll.m_bScrollNeedShow)
	{
		return 1;
	}

#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	pDC->FillSolidRect(&m_horiScroll.m_rcScroll,m_horiScroll.m_clrBk);

	Graphics graphics(pDC->m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	m_horiScroll.m_arrowInfo1.CalcPtData(m_horiScroll.m_rcStartArrow);

	if(EAS_FILL == m_horiScroll.m_arrowInfo1.m_eArrowStyle)
	{
		SolidBrush brush(m_horiScroll.m_arrowInfo1.m_clrArrow);
		graphics.FillPolygon(&brush,m_horiScroll.m_arrowInfo1.m_aryPtTriangleLeft,3,FillModeAlternate);
	}
	else
	{
		Pen pen(m_horiScroll.m_arrowInfo1.m_clrLineNormal,m_horiScroll.m_arrowInfo1.m_iLineW);
		graphics.DrawLines(&pen,m_horiScroll.m_arrowInfo1.m_aryPtTriangleLeft,3);
	}

	m_horiScroll.m_arrowInfo1.CalcPtData(m_horiScroll.m_rcEndArrow);
	if(EAS_FILL == m_horiScroll.m_arrowInfo1.m_eArrowStyle)
	{
		SolidBrush brush(m_horiScroll.m_arrowInfo1.m_clrArrow);
		graphics.FillPolygon(&brush,m_horiScroll.m_arrowInfo1.m_aryPtTriangleRight,3,FillModeAlternate);
	}
	else
	{
		Pen pen(m_horiScroll.m_arrowInfo1.m_clrLineNormal,m_horiScroll.m_arrowInfo1.m_iLineW);
		graphics.DrawLines(&pen,m_horiScroll.m_arrowInfo1.m_aryPtTriangleRight,3);
	}

	graphics.ReleaseHDC(pDC->m_hDC);

	//绘制滑块
	pDC->FillSolidRect(&m_horiScroll.m_rcSlider,RGB(m_horiScroll.m_clrSliderBk.GetR(),m_horiScroll.m_clrSliderBk.GetG(),m_horiScroll.m_clrSliderBk.GetB()));

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return 0;
}

int CMyLstCtrl::DrawVertScrollStyle2(CDC* pDC)
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

	Graphics graphics(pDC->m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	m_vertScroll.m_arrowInfo1.CalcPtData(m_vertScroll.m_rcEndArrow);
	if(EAS_FILL == m_vertScroll.m_arrowInfo1.m_eArrowStyle)
	{
		SolidBrush brush(m_vertScroll.m_arrowInfo1.m_clrArrow);
		graphics.FillPolygon(&brush,m_vertScroll.m_arrowInfo1.m_aryPtTriangle,3,FillModeAlternate);
	}
	else
	{
		Pen pen(m_vertScroll.m_arrowInfo1.m_clrLineNormal,m_vertScroll.m_arrowInfo1.m_iLineW);
		graphics.DrawLines(&pen,m_vertScroll.m_arrowInfo1.m_aryPtTriangle,3);
	}

	m_vertScroll.m_arrowInfo2.CalcPtData(m_vertScroll.m_rcStartArrow);
	if(EAS_FILL == m_vertScroll.m_arrowInfo2.m_eArrowStyle)
	{
		SolidBrush brush(m_vertScroll.m_arrowInfo2.m_clrArrow);
		graphics.FillPolygon(&brush,m_vertScroll.m_arrowInfo2.m_aryPtTriangleDropDown,3,FillModeAlternate);
	}
	else
	{
		Pen pen(m_vertScroll.m_arrowInfo2.m_clrLineNormal,m_vertScroll.m_arrowInfo1.m_iLineW);
		graphics.DrawLines(&pen,m_vertScroll.m_arrowInfo2.m_aryPtTriangleDropDown,3);
	}

	graphics.ReleaseHDC(pDC->m_hDC);

	//绘制滑块
	pDC->FillSolidRect(&m_vertScroll.m_rcSlider,RGB(m_vertScroll.m_clrSliderBk.GetR(),m_vertScroll.m_clrSliderBk.GetG(),m_vertScroll.m_clrSliderBk.GetB()));

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return 0;
}

//BOOL CMyLstCtrl::PreTranslateMessage(MSG* pMsg)
//{
//	if(WM_GESTURE == pMsg->message)
//	{
//		OnMsgGesture(pMsg->wParam,pMsg->lParam);
//	}
//
//	return __super::PreTranslateMessage(pMsg);
//}

BOOL CMyLstCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyLstCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}
}

void CMyLstCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	EndEDit();
	CDotPosInfo dotPosInfo = CalcPtRowCol(point);	
	if(dotPosInfo.BeValidCellPos() && !m_lnkRowInfo[dotPosInfo.m_rowId]->m_bDisabled && !m_lnkTitleInfo[dotPosInfo.m_colId]->GetBeDisabled())
	{
		CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(dotPosInfo.m_rowId)->data.GetAt(dotPosInfo.m_colId)->data;

		if(cellInfo.m_bEditable && !cellInfo.m_bDisabled)
		{
			m_preRowColEditInfo = m_rowColEditInfo;

			m_rowColEditInfo.m_rowId = dotPosInfo.m_rowId;
			m_rowColEditInfo.m_colId = dotPosInfo.m_colId;
			m_rowColEditInfo.m_strText = cellInfo.GetStringFormatData();
			m_edInput.SetMaxMinInfo(cellInfo.m_maxMinInfo);
			m_edInput.MoveWindow(&cellInfo.m_rcInCDc);
			m_edInput.SetText(cellInfo.GetStringFormatData());
			m_edInput.ShowWindow(SW_SHOW);
			::SetFocus(m_edInput.GetSafeHwnd());
		}
	}

	return;
}

void CMyLstCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLeftBtDown = TRUE;

	if(!m_bEditable)
	{
		return CDialogEx::OnLButtonDown(nFlags,point);
	}

	EndEDit();

	if(m_horiScroll.IsInScrollPane())
	{
		m_horiScroll.m_bLeftBtDown = TRUE;
		m_horiScroll.m_prePt = point;
		SetCapture();
	}
	else if(m_vertScroll.IsInScrollPane())
	{
		m_vertScroll.m_bLeftBtDown = TRUE;
		m_vertScroll.m_prePt = point;
		SetCapture();
	}
	else
	{
		//计算鼠标点击时所在的行和列
		CDotPosInfo dotPosInfo = CalcPtRowCol(point);
		if(dotPosInfo.BeValidCellPos() && !m_lnkRowInfo[dotPosInfo.m_rowId]->m_bDisabled && !m_lnkTitleInfo[dotPosInfo.m_colId]->GetBeDisabled())
		{
			CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(dotPosInfo.m_rowId)->data.GetAt(dotPosInfo.m_colId)->data;
			CTitleInfo& titleInfo = m_lnkTitleInfo.GetAt(dotPosInfo.m_colId)->data;

			if(cellInfo.m_bDisabled)
			{
				goto DEFPROCESS;
			}

			//初始化下拉框内容
			if(cellInfo.m_bCbAble)
			{
				m_cbCtrl.Clear();

				Node<CString>* pNodeCbStr = NULL;
				if(cellInfo.m_bUseCellLnkCbStr)
				{
					pNodeCbStr = cellInfo.m_lnkCbStr.GetHead();
				}
				else
				{
					pNodeCbStr = titleInfo.m_lnkCbStr.GetHead();
				}
				 
				int cbIndex = -1;
				while(NULL != pNodeCbStr)
				{
					m_cbCtrl.InsertString(++cbIndex,pNodeCbStr->data);
					pNodeCbStr = pNodeCbStr->pNext;
				}
				m_cbCtrl.SetText(cellInfo.GetStringFormatData(),FALSE);

				m_preRowColEditInfo = m_rowColEditInfo;

				m_rowColEditInfo.m_rowId = dotPosInfo.m_rowId;
				m_rowColEditInfo.m_colId = dotPosInfo.m_colId;
				m_rowColEditInfo.m_strText = cellInfo.GetStringFormatData();

				m_cbCtrl.m_iLeftIndent = titleInfo.m_iCbTxLeftIndent;
				m_cbCtrl.m_iRightIndent = titleInfo.m_iCbTxRightIndent;
				if(cellInfo.m_bCbEditable || (titleInfo.m_bCbtable && titleInfo.m_bCbEditable))
				{
					m_cbCtrl.SetWorkState(EWS_NORMAL);
				}
				else
				{
					m_cbCtrl.SetWorkState(EWS_READONLY);
				}
				m_cbCtrl.EnableRowDelete(cellInfo.m_bCbDeleteRow);

				CRect tempRc = cellInfo.m_rcInCDc;
				if(tempRc.Height() > m_cbCtrl.m_selWnd.m_iDefRowH)
				{
					tempRc.top += (tempRc.Height() - m_cbCtrl.m_selWnd.m_iDefRowH) / 2;
					tempRc.bottom = tempRc.top + m_cbCtrl.m_selWnd.m_iDefRowH;
				}

				m_cbCtrl.MoveWindow(&tempRc);
				m_cbCtrl.ShowWindow(SW_SHOW);
				m_cbCtrl.SetDropDown();
				m_cbCtrl.SetFocus();
			}
			else if(titleInfo.m_bCbtable)
			{
				if(!(m_preRowColEditInfo == m_rowColEditInfo) || (!m_preRowColEditInfo.BeValidRowCol() && !m_rowColEditInfo.BeValidRowCol()))
				{
					m_cbCtrl.Clear();
					Node<CString>* pNodeCbStr = titleInfo.m_lnkCbStr.GetHead();
					int cbIndex = -1;
					while(NULL != pNodeCbStr)
					{
						m_cbCtrl.InsertString(++cbIndex,pNodeCbStr->data);
						pNodeCbStr = pNodeCbStr->pNext;
					}
					m_cbCtrl.SetText(cellInfo.GetStringFormatData(),FALSE);
				}

				m_preRowColEditInfo = m_rowColEditInfo;

				m_rowColEditInfo.m_rowId = dotPosInfo.m_rowId;
				m_rowColEditInfo.m_colId = dotPosInfo.m_colId;
				m_rowColEditInfo.m_strText = cellInfo.GetStringFormatData();

				m_cbCtrl.m_iLeftIndent = titleInfo.m_iCbTxLeftIndent;
				m_cbCtrl.m_iRightIndent = titleInfo.m_iCbTxRightIndent;
				if(titleInfo.m_bCbEditable)
				{
					m_cbCtrl.SetWorkState(EWS_NORMAL);
				}
				else
				{
					m_cbCtrl.SetWorkState(EWS_READONLY);
				}

				CRect tempRc = cellInfo.m_rcInCDc;
				if(tempRc.Height() > m_cbCtrl.m_selWnd.m_iDefRowH)
				{
					tempRc.top += (tempRc.Height() - m_cbCtrl.m_selWnd.m_iDefRowH) / 2;
					tempRc.bottom = tempRc.top + m_cbCtrl.m_selWnd.m_iDefRowH;
				}

				m_cbCtrl.MoveWindow(&tempRc);
				m_cbCtrl.ShowWindow(SW_SHOW);
				m_cbCtrl.SetDropDown();
				m_cbCtrl.SetFocus();
			}
		}
	}

DEFPROCESS:
	CDialogEx::OnLButtonDown(nFlags,point);

	return;
}

void CMyLstCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bLeftBtDown)
	{
		if (m_horiScroll.m_bInStartArrow)
		{
			ScrollCol(-m_pxRowH);
		}
		else if (m_horiScroll.m_bInEndArrow)
		{
			ScrollCol(m_pxRowH);
		}
		else if (m_horiScroll.m_bInStartPane)
		{
			ScrollCol(-m_horiScroll.m_pixelForShow);
		}
		else if (m_horiScroll.m_bInEndPane)
		{
			ScrollCol(m_horiScroll.m_pixelForShow);
		}

		if (m_vertScroll.m_bInStartArrow)
		{
			ScrollRow(-m_pxRowH);
		}
		else if (m_vertScroll.m_bInEndArrow)
		{
			ScrollRow(m_pxRowH);
		}
		else if (m_vertScroll.m_bInStartPane)
		{
			ScrollRow(-m_vertScroll.m_pixelForShow);
		}
		else if (m_vertScroll.m_bInEndPane)
		{
			ScrollRow(m_vertScroll.m_pixelForShow);
		}

		m_bLeftBtDown = FALSE;
	}
	else
	{
		return CDialogEx::OnLButtonUp(nFlags,point);
	}

	if(!m_bEditable)
	{
		return CDialogEx::OnLButtonUp(nFlags,point);;
	}

	m_horiScroll.m_bLeftBtDown = FALSE;
	m_horiScroll.m_bStartDragSlider = FALSE;
	m_horiScroll.m_bSliderDragMove = FALSE;

	m_vertScroll.m_bLeftBtDown = FALSE;
	m_vertScroll.m_bStartDragSlider = FALSE;
	m_vertScroll.m_bSliderDragMove = FALSE;

	ReleaseCapture();
	
	m_preDotPosInfo = m_curDotPosInfo;
	m_curDotPosInfo = CalcPtRowCol(point);
	m_lnkHistoryDotPosInfo.AddTail(m_curDotPosInfo);

	//更新行列选择状态
	if(m_curDotPosInfo.BeValidCellPos() && !m_lnkRowInfo[m_curDotPosInfo.m_rowId]->m_bDisabled && !m_lnkTitleInfo[m_curDotPosInfo.m_colId]->GetBeDisabled())
	{
		CWnd* pParent = GetParent();
		CRowInfo& rowInfo = *m_lnkRowInfo[m_curDotPosInfo.m_rowId];
		CTitleInfo& titleInfo = *m_lnkTitleInfo[m_curDotPosInfo.m_colId];
		CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(m_curDotPosInfo.m_rowId)->data.GetAt(m_curDotPosInfo.m_colId)->data;
		CTitleInfo& titleInfoFst = *m_lnkTitleInfo[0];
		CCellInfo& cellInfoFst = m_2LnkCellInfo.GetAt(m_curDotPosInfo.m_rowId)->data.GetAt(0)->data;

		BOOL bSelected = rowInfo.m_bSelected;
		if((nFlags & MK_CONTROL) && !m_bSingleSelect)
		{
			rowInfo.m_bSelected = !bSelected;
			if(cellInfo.GetCheckable())
			{
				cellInfo.SetItemCheckState(!cellInfo.GetItemCheckState());
				CString strTemp(_T("FALSE"));
				if(cellInfo.GetItemCheckState())
				{
					strTemp = _T("TRUE");
				}
				UpdateRowColumnDisableState(m_curDotPosInfo.m_rowId,m_curDotPosInfo.m_colId,strTemp);
			}
			else if(titleInfo.m_bCheckFunc)
			{
				cellInfo.SetItemCheckState(!cellInfo.GetItemCheckState());
				CString strTemp(_T("FALSE"));
				if(cellInfo.GetItemCheckState())
				{
					strTemp = _T("TRUE");
				}
				UpdateRowColumnDisableState(m_curDotPosInfo.m_rowId,m_curDotPosInfo.m_colId,strTemp);
			}
			
			Invalidate(FALSE);

			if(m_bTellParentRowSelChange && NULL != pParent->GetSafeHwnd())	//选中行改变，通知父窗口
			{
				pParent->PostMessage(WM_ROWSELECTCHANGE,0,0);
			}
		}
		else
		{
			BOOL bNeedRedraw = FALSE;
			if(titleInfo.m_bColorFunc && titleInfo.m_bClrEditale && ((0 != m_curDotPosInfo.m_colId) || (0 == m_curDotPosInfo.m_colId && !m_bSingleSelect)))
			{
#ifdef USE_SELFDEFINED_COLORSELECTDLG
				CRect rcClrBlock = GetColorSelectDlgRect(cellInfo.m_rcClrBlock,m_clrSelDlg.m_iWndWidth,m_clrSelDlg.m_iWndHeight);

				m_clrSelDlg.MoveWindow(&rcClrBlock,FALSE);
				m_clrSelDlg.m_iCellRowId = m_curDotPosInfo.m_rowId;
				m_clrSelDlg.m_iCellColId = m_curDotPosInfo.m_colId;
				m_clrSelDlg.ShowWindow(SW_SHOW);

				goto PROCESSEND;
#else
				CColorDialog clrSelectDlg;
				if(IDOK == clrSelectDlg.DoModal())
				{
					COLORREF clrNew = clrSelectDlg.GetColor();
					PostMessage(WM_SELECTNEWCOLOR,clrNew,MAKELPARAM(m_curDotPosInfo.m_rowId,m_curDotPosInfo.m_colId));
				}
				goto PROCESSEND;
#endif
			}
			else if(cellInfo.GetCheckable() && ((0 != m_curDotPosInfo.m_colId) || (0 == m_curDotPosInfo.m_colId && !m_bSingleSelect)))
			{
				cellInfo.SetItemCheckState(!cellInfo.GetItemCheckState());
				TellParentCellChanged(m_curDotPosInfo.m_rowId,m_curDotPosInfo.m_colId);

				CString strTemp(_T("FALSE"));
				if(cellInfo.GetItemCheckState())
				{
					strTemp = _T("TRUE");
				}
				UpdateRowColumnDisableState(m_curDotPosInfo.m_rowId,m_curDotPosInfo.m_colId,strTemp);

				bNeedRedraw = TRUE;
				goto REDRAWLST;
			}
			else if(titleInfo.m_bCheckFunc && ((0 != m_curDotPosInfo.m_colId) || (0 == m_curDotPosInfo.m_colId && !m_bSingleSelect)))
			{
				cellInfo.SetItemCheckState(!cellInfo.GetItemCheckState());
				TellParentCellChanged(m_curDotPosInfo.m_rowId,m_curDotPosInfo.m_colId);

				CString tempStr(_T("FALSE"));
				if(cellInfo.GetItemCheckState())
				{
					tempStr = _T("TRUE");
				}
				UpdateRowColumnDisableState(m_curDotPosInfo.m_rowId,m_curDotPosInfo.m_colId,tempStr);

				bNeedRedraw = TRUE;
				goto REDRAWLST;
			}

			if(bSelected && m_bAllowCancelRowSelect)
			{
				bNeedRedraw = TRUE;
				rowInfo.m_bSelected = FALSE;
			}
			else
			{
				bNeedRedraw = !bSelected;
				rowInfo.m_bSelected = TRUE;
			}

			if(titleInfoFst.m_bCheckFunc && m_bSingleSelect)	//如果是行单选列表框，则第一列选择状态跟随行选择状态
			{
				if(bSelected && m_bAllowCancelRowSelect)
				{
					cellInfoFst.SetItemCheckState(FALSE);
				}
				else
				{
					cellInfoFst.SetItemCheckState(TRUE);
				}
			}

			for(int i = 0;i < m_rowNum;++i)
			{
				if(i != m_curDotPosInfo.m_rowId)
				{
					if(m_lnkRowInfo[i]->m_bSelected && i >= m_rowIdStartShow && i <= m_rowIdEndShow)
					{
						bNeedRedraw = TRUE;
					}
					m_lnkRowInfo[i]->m_bSelected = FALSE;
					if(m_bSingleSelect && titleInfoFst.m_bCheckFunc)	//如果是行单选列表框，则第一列选择状态跟随行选择状态
					{
						CCellInfo& cellInfoOtherFst = m_2LnkCellInfo.GetAt(i)->data.GetAt(0)->data;
						cellInfoOtherFst.SetItemCheckState(FALSE);
					}
				}
			}

REDRAWLST:
			if(bNeedRedraw)
			{
				Invalidate(FALSE);
			}

			//if(!(m_preDotPosInfo == m_curDotPosInfo))	//单行取消选中时不会通知父窗口，20220711，孙文郎
			{
				if(m_bTellParentRowSelChange && NULL != pParent->GetSafeHwnd())	//选中行改变，通知父窗口
				{
					pParent->PostMessage(WM_ROWSELECTCHANGE,0,0);
				}
			}
		}
	}

PROCESSEND:
	CDialogEx::OnLButtonUp(nFlags,point);
	return;
}

void CMyLstCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	m_horiScroll.CalcCurPointInPane(point);
	if(m_horiScroll.m_bScrollNeedShow && m_horiScroll.m_rcSlider.PtInRect(point))
	{
		if(m_horiScroll.m_bLeftBtDown)
		{
			m_horiScroll.m_bStartDragSlider = TRUE;
		}

		if(m_horiScroll.m_bStartDragSlider && (MK_LBUTTON & nFlags))
		{
			m_horiScroll.m_bSliderDragMove = TRUE;
		}
	}

	m_vertScroll.CalcCurPointInPane(point);
	if(m_vertScroll.m_bScrollNeedShow && m_vertScroll.m_rcSlider.PtInRect(point))
	{
		if(m_vertScroll.m_bLeftBtDown)
		{
			m_vertScroll.m_bStartDragSlider = TRUE;
		}

		if(m_vertScroll.m_bStartDragSlider && (MK_LBUTTON & nFlags))
		{
			m_vertScroll.m_bSliderDragMove = TRUE;
		}
	}

	if(m_horiScroll.m_bScrollNeedShow && m_horiScroll.m_bSliderDragMove)
	{
		ProcessHoriSrcoll(nFlags,point);
	}
	else if(m_vertScroll.m_bScrollNeedShow && m_vertScroll.m_bSliderDragMove)
	{
		ProcessVertSrcoll(nFlags,point);
	}

	return;
}

BOOL CMyLstCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_bEditable)
	{
		return __super::OnMouseWheel(nFlags, zDelta, pt);
	}

	EndEDit();

	if(m_vertScroll.m_bScrollNeedShow)
	{
		if(zDelta > 0)
		{
			ScrollRow(-m_pxRowH * 2);
		}
		else
		{
			ScrollRow(m_pxRowH * 2);
		}
	}
	else if(m_horiScroll.m_bScrollNeedShow)
	{
		if(zDelta > 0)
		{
			ScrollCol(-m_pxRowH * 2);
		}
		else
		{
			ScrollCol(m_pxRowH * 2);
		}
	}
	
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

int CMyLstCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDialogEx::OnCreate(lpCreateStruct);

	CRect rcCreate(0,0,0,0);

	m_cbCtrl.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_MYLSTCTRL_CB_FORINPUT);

	int iFtH = 15 * m_dTxRatio;
	m_cbCtrl.SetMyFont(m_stTxFont,iFtH);
	m_cbCtrl.m_selWnd.SetMyFont(m_stTxFont,iFtH);
	m_cbCtrl.SetWorkState(EWS_READONLY);
	m_cbCtrl.SetBeEnterTellParent(TRUE);
	m_cbCtrl.m_iLeftIndent = 8 * m_dHoriRatio;
	m_cbCtrl.m_iRightIndent = 8 * m_dHoriRatio;
	m_cbCtrl.SetBeFuncEdit(TRUE);
	m_cbCtrl.SetCtrlTag(m_iInputCbTag);
	m_cbCtrl.SetTellParentTxChangedParam(TRUE,MYLST_CB_TYPETAG);
	m_cbCtrl.SetTellParentSetNewText(TRUE,MYLST_CB_TYPETAG);
	
	m_edInput.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_MYLSTCTRL_ED_FORINPUT);
	m_edInput.SetBeEnterTellParent(TRUE);
	m_edInput.SetMyFont(m_stTxFont,iFtH);
	m_edInput.SetTextClr1(RGB(0X39,0X45,0X65));
	m_edInput.m_iLeftIndent = 8 * m_dHoriRatio;
	m_edInput.m_iRightIndent = 8 * m_dHoriRatio;
	m_edInput.SetBeFuncEdit(TRUE);
	m_edInput.SetCtrlTag(m_iInputEdTag);

	m_clrSelDlg.Create(IDD_POPUPDLG,this);	

	return 1;
}

void CMyLstCtrl::OnSize(UINT nType, int cx, int cy)
{
	m_rcLst = CRect(0,0,cx,cy);
	m_rcHeader = CRect(m_pxFrame,m_pxFrame,cx - m_pxFrame,m_pxHeaderH); 
	m_rcBody = CRect(m_pxFrame,m_pxHeaderH,cx - m_pxFrame,cy - m_pxFrame);

	CalcScrollPos();
	CalcRowColToShow();
	CalcRowColToShowOnNoRowData();
	RenewHoriScrollPos();
	RenewVertScrollPos();

	CDialogEx::OnSize(nType, cx, cy);
}

LRESULT CMyLstCtrl::OnMsgFuncEditKillFocus(WPARAM wParam,LPARAM lParam)
{
	EndEDitForFuncCtrlKillFocus(wParam);
	
	return 1;
}

LRESULT CMyLstCtrl::OnMsgEditEnterPushed(WPARAM wParam,LPARAM lParam)
{
	EndEDit();
	return 1;
}

LRESULT CMyLstCtrl::OnMsgTellParentTextChanged(WPARAM wParam,LPARAM lParam)
{
	if(MYLST_CB_TYPETAG == wParam)
	{
		CbTextChanged();
	}

	return 0;
}

LRESULT CMyLstCtrl::OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam)
{
	if(MYLST_CB_TYPETAG == wParam)
	{
		CbTextChanged();
	}

	return 0;
}

LRESULT CMyLstCtrl::OnMsgTouch(WPARAM wParam,LPARAM lParam)
{
	UINT uiNumInputs = LOWORD(wParam);
	if(uiNumInputs < 1)
	{
		return 1;
	}

	TOUCHINPUT* pTouchInput = new TOUCHINPUT[uiNumInputs];
	if(NULL == pTouchInput)
	{
		return 2;
	}

	CString strTemp(_T(""));

	static int a = 1;

	if(GetTouchInputInfo((HTOUCHINPUT)lParam,uiNumInputs,pTouchInput,sizeof(TOUCHINPUT)))
	{
		for(UINT i = 0;i < uiNumInputs;++i)
		{
			TOUCHINPUT ptTouch = pTouchInput[i];

			int iX = TOUCH_COORD_TO_PIXEL(ptTouch.x);
			int iY = TOUCH_COORD_TO_PIXEL(ptTouch.y);

			strTemp.Format(_T("%d,  %d,  %d"),uiNumInputs,iX,iY);
			if(++a < 3)
			{
				//AfxMessageBox(strTemp);
			}

			if(ptTouch.dwFlags & TOUCHEVENTF_MOVE)
			{
				ScrollRow(20);
			}
		}

		if(!CloseTouchInputHandle((HTOUCHINPUT)lParam))
		{
			//error handling
		}
	}
	else
	{
		DWORD dwRet = GetLastError();
	}

	delete [] pTouchInput;

	return 0;
}

LRESULT CMyLstCtrl::OnMsgGesture(WPARAM wParam,LPARAM lParam)
{
	AfxMessageBox(_T("GID_PAN"));

	GESTUREINFO gi;
	memset(&gi,0,sizeof(GESTUREINFO));
	gi.cbSize = sizeof(GESTUREINFO);

	BOOL bHandled = FALSE;
	BOOL bRet = GetGestureInfo((HGESTUREINFO)lParam,&gi);

	if(bRet)
	{
		switch (gi.dwID)
		{
		case GID_ZOOM: break;
		case GID_PAN: AfxMessageBox(_T("GID_PAN"));bHandled = TRUE; break;
		case GID_ROTATE: break;
		case GID_TWOFINGERTAP: break;
		case GID_PRESSANDTAP: break;
		default: break;
		}
	}
	else
	{
		DWORD dwErr = GetLastError();
	}

	if(bHandled)
	{
		return 0;
	}
	else
	{
		return CDialogEx::OnGesture(wParam,lParam);
	}
}

LRESULT CMyLstCtrl::OnMsgHorizonScroll(WPARAM wParam,LPARAM lParam)
{
	if(SB_LEFT == LOWORD(wParam))
	{
		ScrollCol(-m_pxRowH * 2);
	}
	else if(SB_RIGHT == LOWORD(wParam))
	{
		ScrollCol(m_pxRowH * 2);
	}

	return 0;
}

LRESULT CMyLstCtrl::OnMsgVerticalScroll(WPARAM wParam,LPARAM lParam)
{
	if(SB_TOP == LOWORD(wParam))
	{
		ScrollRow(-m_pxRowH * 2);
	}
	else if(SB_BOTTOM == LOWORD(wParam))
	{
		ScrollRow(m_pxRowH * 2);
	}

	return 0;
}

LRESULT CMyLstCtrl::OnMsgSelectNewColor(WPARAM wParam,LPARAM lParam)
{
	COLORREF clrSelected = wParam;

	int iRowId = LOWORD(lParam);
	int iColId = HIWORD(lParam);

	if(iRowId < 0 || iRowId >= m_rowNum)
	{
		return 1;
	}

	if(iColId < 0 || iColId >= m_colNum)
	{
		return 2;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(m_curDotPosInfo.m_rowId)->data.GetAt(m_curDotPosInfo.m_colId)->data;
	if(cellInfo.GetItemColor() == clrSelected)
	{
		return 3;
	}

	cellInfo.SetItemColor(clrSelected);
	Invalidate(FALSE);

	CWnd* pParent = GetParent();
	if(NULL != pParent->GetSafeHwnd())
	{
		pParent->PostMessage(WM_TELLPARENTCELLCHANGED,iRowId,iColId);
	}

	return 0;
}

void CMyLstCtrl::ProcessHoriSrcoll(UINT nFlags, CPoint point)
{
	EndEDit();

	if(m_horiScroll.m_bSliderDragMove)
	{
		int horiShift = point.x - m_horiScroll.m_prePt.x;
		if(m_horiScroll.m_rcSlider.left + horiShift < m_horiScroll.m_rcStartArrow.right)
		{
			int sliderWidth = m_horiScroll.m_rcSlider.Width();
			m_horiScroll.m_rcSlider.left = m_horiScroll.m_rcStartArrow.right;
			m_horiScroll.m_rcSlider.right = m_horiScroll.m_rcSlider.left + sliderWidth;
			if(0 != m_horiScroll.m_curSliderPos)
			{
				m_horiScroll.m_curSliderPos = 0;
				m_horiScroll.CalcCurPixelPos();
				ScrollColToFixPos(m_horiScroll.m_curPixelPos);
				Invalidate(FALSE);
			}
		}
		else if(m_horiScroll.m_rcSlider.right + horiShift > m_horiScroll.m_rcEndArrow.left)
		{
			int sliderWidth = m_horiScroll.m_rcSlider.Width();
			m_horiScroll.m_rcSlider.right = m_horiScroll.m_rcEndArrow.left;
			m_horiScroll.m_rcSlider.left = m_horiScroll.m_rcSlider.right - sliderWidth;

			if(m_horiScroll.m_validScope != m_horiScroll.m_curSliderPos)
			{
				m_horiScroll.m_curSliderPos = m_horiScroll.m_validScope;
				m_horiScroll.CalcCurPixelPos();
				ScrollColToFixPos(m_horiScroll.m_curPixelPos);
				Invalidate(FALSE);
			}
		}
		else
		{
			m_horiScroll.m_rcSlider.DeflateRect(horiShift,0,-horiShift,0);
			m_horiScroll.m_curSliderPos += horiShift;
			m_horiScroll.CalcCurPixelPos();
			ScrollColToFixPos(m_horiScroll.m_curPixelPos);
			Invalidate(FALSE);
		}
	}
	m_horiScroll.m_prePt = point;

	return;
}

void CMyLstCtrl::ProcessVertSrcoll(UINT nFlags, CPoint point)
{
	EndEDit();

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

int CMyLstCtrl::GetColStartDrawInMemoryHoriPixel(int colId)
{
	if(colId < m_colIdStartShow)
	{
		return -1;
	}

	int horiPixel = 0;
	for(int i = m_colIdStartShow;i < colId;++i)
	{
		horiPixel += m_lnkTitleInfo[i]->m_colWidth;
	}
	return horiPixel;
}

int CMyLstCtrl::GetRowStartDrawInMemoryVertPixel(int rowId)
{
	if(rowId < m_rowIdStartShow)
	{
		return -1;
	}

	int vertPixel = 0;
	for(int i = m_rowIdStartShow;i < rowId;++i)
	{
		vertPixel += m_lnkRowInfo[i]->m_rowH;
	}
	return vertPixel;
}

int CMyLstCtrl::GetWidth(int colStart,int colEnd)
{
	if(colStart < 0 || colStart >= m_colNum)
	{
		return -1;
	}
	if(colEnd < 0 || colEnd >= m_colNum)
	{
		return -1;
	}

	int width = 0;
	for(int i = colStart;i <= colEnd;++i)
	{
		width += m_lnkTitleInfo[i]->m_colWidth;
	}
	return width;
}

int CMyLstCtrl::GetHeight(int rowStart,int rowEnd)
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
		width += m_lnkRowInfo[i]->m_rowH;
	}
	return width;
}

void CMyLstCtrl::SetScrollArrowWH(int iWidth,int iHeight,BOOL bRedraw /*= FALSE*/)
{
	m_horiScroll.m_arrowInfo1.m_arrowW = 4;
	m_horiScroll.m_arrowInfo1.m_arrowH = 8;
	m_horiScroll.m_arrowInfo2.m_arrowW = 4;
	m_horiScroll.m_arrowInfo2.m_arrowH = 8;

	m_vertScroll.m_arrowInfo1.m_arrowW = 8;
	m_vertScroll.m_arrowInfo1.m_arrowH = 4;
	m_vertScroll.m_arrowInfo2.m_arrowW = 8;
	m_vertScroll.m_arrowInfo2.m_arrowH = 4;

	if(bRedraw && (m_bShowHoriScroll || m_bShowVertScroll))
	{
		Invalidate(FALSE);
	}

	return;
}

CDotPosInfo CMyLstCtrl::CalcPtRowCol(CPoint pt)
{
	CDotPosInfo dotPosInfo;
	dotPosInfo.m_pt = pt;
	if(!m_rcLst.PtInRect(pt))
	{
		return dotPosInfo;
	}

	if(0 == m_colNum)
	{
		return dotPosInfo;
	}

	//pt在header（标题栏）内
	if(m_rcHeader.PtInRect(pt))
	{
		dotPosInfo.m_eDotPosType = EDT_HEADER;
		int sum = m_lnkTitleInfo[m_colIdStartShow]->m_colWidth - m_ssColLeftIndent;
		if(pt.x <= sum)
		{
			dotPosInfo.m_colId = m_colIdStartShow;
			return dotPosInfo;
		}
		else
		{
			for(int i = m_colIdStartShow + 1;i <= m_colIdEndShow;++i)
			{
				sum += m_lnkTitleInfo[i]->m_colWidth;
				if(pt.x <= sum)
				{
					dotPosInfo.m_colId = i;
					return dotPosInfo;
				}
			}
		}
	}
	else if(m_rcBody.PtInRect(pt) && 0 != m_rowNum)	//pt在body内
	{
		dotPosInfo.m_eDotPosType = EDT_BODY;

		int sum = m_lnkTitleInfo[m_colIdStartShow]->m_colWidth - m_ssColLeftIndent;
		if(pt.x <= sum)
		{
			dotPosInfo.m_colId = m_colIdStartShow;
		}
		else
		{
			for(int i = m_colIdStartShow + 1;i <= m_colIdEndShow;++i)
			{
				sum += m_lnkTitleInfo[i]->m_colWidth;
				if(pt.x <= sum)
				{
					dotPosInfo.m_colId = i;
					break;
				}
			}
		}

		sum = m_lnkRowInfo[m_rowIdStartShow]->m_rowH - m_ssRowTopIndent + m_pxFrame + m_rcHeader.Height();
		if(pt.y <= sum)
		{
			dotPosInfo.m_rowId = m_rowIdStartShow;
			return dotPosInfo;
		}
		else
		{
			for(int i = m_rowIdStartShow + 1;i <= m_rowIdEndShow;++i)
			{
				sum += m_lnkRowInfo[i]->m_rowH;
				if(pt.y <= sum)
				{
					dotPosInfo.m_rowId = i;
					return dotPosInfo;
				}
			}
		}
	}
	else if(m_horiScroll.m_bScrollNeedShow && m_horiScroll.m_rcScroll.PtInRect(pt))
	{
		dotPosInfo.m_eDotPosType = EDT_HORISCROLL;
		return dotPosInfo;
	}
	else if(m_vertScroll.m_bScrollNeedShow && m_vertScroll.m_rcScroll.PtInRect(pt))
	{
		dotPosInfo.m_eDotPosType = EDT_VERTSCROLL;
		return dotPosInfo;
	}

	return dotPosInfo;
}

int CMyLstCtrl::InsertColumn(int colId,CString strColTitle,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}

	if(colId > m_colNum)
	{
		colId = m_colNum;
	}

	CTitleInfo titleInfo;
	titleInfo.m_strTitle = strColTitle;
	int ret = m_lnkTitleInfo.InsertAtPre(colId,titleInfo);
	if(0 == ret)
	{
		++m_colNum;
		if(m_rowNum > 0)
		{
			for(int i = 0;i < m_rowNum;++i)
			{
				m_2LnkCellInfo[i]->InsertAtPre(colId);
			}		
		}

		CalcTotalColumnWidth();
		CalcScrollPos();
		CalcRowColToShow();
		CalcRowColToShowOnNoRowData();
		RenewHoriScrollPos();
		RenewVertScrollPos();

		if(bRedraw && NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
		return 0;
	}
	else
	{
		return -2;
	}
}

int CMyLstCtrl::InsertRow(int rowId,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId > m_rowNum)
	{
		rowId = m_rowNum;
	}

	m_2LnkCellInfo.InsertAtPre(rowId);
	m_lnkRowInfo.InsertAtPre(rowId);
	++m_rowNum;

	for(int i = 0;i < m_colNum;++i)
	{
		CCellInfo cellInfo;
		CTitleInfo* pTitleInfo = m_lnkTitleInfo[i];
		if(pTitleInfo->GetBeEditable())
		{
			cellInfo.m_bEditable = TRUE;
			cellInfo.m_maxMinInfo = pTitleInfo->m_maxMinInfo;
		}
		else if(pTitleInfo->m_bCbtable)
		{
			cellInfo.m_bCbAble = TRUE;
			cellInfo.m_bCbEditable = pTitleInfo->m_bCbEditable;
			cellInfo.m_dwTextAlign = pTitleInfo->m_dwTextAlign;
			cellInfo.m_arrowInfo = pTitleInfo->m_arrowInfo;
			cellInfo.m_lnkCbStr = pTitleInfo->m_lnkCbStr;
			cellInfo.m_iTxLeftIndent = pTitleInfo->m_iCbTxLeftIndent;
			cellInfo.m_iTxRightIndent = pTitleInfo->m_iCbTxRightIndent;
		}
		m_2LnkCellInfo[rowId]->AddTail(cellInfo);
	}

	CalcTotalRowHeight();
	CalcScrollPos();
	CalcRowColToShow();
	CalcRowColToShowOnNoRowData();
	RenewVertScrollPos();
	RenewHoriScrollPos();

	//设置最大最小值限制信息
	for(int i = 0;i < m_colNum;++i)
	{
		m_2LnkCellInfo[rowId]->GetAt(i)->data.m_maxMinInfo = m_lnkTitleInfo[i]->m_maxMinInfo;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyLstCtrl::InsertRowSetHeight(int rowId,int iRowH,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId > m_rowNum)
	{
		rowId = m_rowNum;
	}

	m_2LnkCellInfo.InsertAtPre(rowId);

	CRowInfo rowInfo;
	rowInfo.m_rowH = iRowH;
	m_lnkRowInfo.InsertAtPre(rowId,rowInfo);
	++m_rowNum;

	for(int i = 0;i < m_colNum;++i)
	{
		CCellInfo cellInfo;
		CTitleInfo* pTitleInfo = m_lnkTitleInfo[i];
		if(pTitleInfo->GetBeEditable())
		{
			cellInfo.m_bEditable = TRUE;
			cellInfo.m_maxMinInfo = pTitleInfo->m_maxMinInfo;
		}
		else if(pTitleInfo->m_bCbtable)
		{
			cellInfo.m_bCbAble = TRUE;
			cellInfo.m_bCbEditable = pTitleInfo->m_bCbEditable;
			cellInfo.m_dwTextAlign = pTitleInfo->m_dwTextAlign;
			cellInfo.m_arrowInfo = pTitleInfo->m_arrowInfo;
			cellInfo.m_lnkCbStr = pTitleInfo->m_lnkCbStr;
			cellInfo.m_iTxLeftIndent = pTitleInfo->m_iCbTxLeftIndent;
			cellInfo.m_iTxRightIndent = pTitleInfo->m_iCbTxRightIndent;
		}
		m_2LnkCellInfo[rowId]->AddTail(cellInfo);
	}

	CalcTotalRowHeight();
	CalcScrollPos();
	CalcRowColToShow();
	CalcRowColToShowOnNoRowData();
	RenewVertScrollPos();
	RenewHoriScrollPos();

	//设置最大最小值限制信息
	for(int i = 0;i < m_colNum;++i)
	{
		m_2LnkCellInfo[rowId]->GetAt(i)->data.m_maxMinInfo = m_lnkTitleInfo[i]->m_maxMinInfo;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyLstCtrl::DeleteColumn(int colId,BOOL bRedraw /*= TRUE*/)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}

	int colW = m_lnkTitleInfo[colId]->m_colWidth;

	Node<CLinkList<CCellInfo>>* pNode = m_2LnkCellInfo.GetHead();
	while(NULL != pNode)
	{
		pNode->data.DeleteAt(colId);
		pNode = pNode->pNext;
	}
	m_lnkTitleInfo.DeleteAt(colId);
	--m_colNum;

	CalcTotalColumnWidth();
	CalcScrollPos();
	if(colId < m_colIdStartShow)
	{
		CalcColToShow(m_colIdStartShow - 1);
	}
	else
	{
		CalcColToShow(m_colIdStartShow);
	}
	CalcRowColToShowOnNoRowData();
	RenewHoriScrollPos();
	RenewVertScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::DeleteAllColumn(BOOL bRedraw /*= TRUE*/)
{
	if(m_colNum < 1)
	{
		return -1;
	}

	m_2LnkCellInfo.Clear();
	m_lnkTitleInfo.Clear();
	m_lnkRowInfo.Clear();
	m_colNum = 0;
	m_rowNum = 0;

	m_colIdStartShow = -1;
	m_colIdEndShow = -1;
	m_rowIdStartShow = -1;
	m_rowIdEndShow = -1;
	m_ssColLeftIndent = 0;
	m_esColRightIndent = 0;
	m_ssRowTopIndent = 0;
	m_esRowBottomIndent = 0;

	CalcTotalColumnWidth();

	CalcScrollPos();
	RenewHoriScrollPos();
	RenewVertScrollPos();
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::DeleteRow(int rowId,BOOL bRedraw /*= TRUE*/)
{
	if(rowId < 0)
	{
		return -1;
	}
	if(rowId >= m_rowNum)
	{
		return -2;
	}

	int rowH = m_lnkRowInfo[rowId]->m_rowH;

	m_2LnkCellInfo.DeleteAt(rowId);
	m_lnkRowInfo.DeleteAt(rowId);
	--m_rowNum;

	CalcTotalRowHeight();
	CalcScrollPos();
	if(rowId < m_rowIdStartShow)
	{
		CalcRowToShow(m_rowIdStartShow - 1);
	}
	else
	{
		CalcRowToShow(m_rowIdStartShow);
	}
	RenewVertScrollPos();
	RenewHoriScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::DeleteMultiRow(vector<int> vecSelectedRow,BOOL bRedraw /*= TRUE*/)
{
	if(vecSelectedRow.size() < 1)
	{
		return -1;
	}

	int rowH = 0;
	int fstDeleteRowId = vecSelectedRow[0];

	for(int i = vecSelectedRow.size() - 1;i >= 0;--i)
	{
		if(vecSelectedRow[i] >= 0 && vecSelectedRow[i] < m_rowNum)
		{
			rowH += m_lnkRowInfo[vecSelectedRow[i]]->m_rowH;
			m_2LnkCellInfo.DeleteAt(vecSelectedRow[i]);
			m_lnkRowInfo.DeleteAt(vecSelectedRow[i]);
			--m_rowNum;
		}
	}
	
	CalcTotalRowHeight();
	CalcScrollPos();
	if(fstDeleteRowId < m_rowIdStartShow)
	{
		CalcRowToShow(m_rowIdStartShow - 1);
	}
	else
	{
		CalcRowToShow(m_rowIdStartShow);
	}
	RenewVertScrollPos();
	RenewHoriScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

void CMyLstCtrl::DeleteData(BOOL bRedraw /*= FALSE*/)
{
	Node<CLinkList<CCellInfo>>* pNode = m_2LnkCellInfo.GetHead();
	while(NULL != pNode)
	{
		Node<CCellInfo>* pNodeCellInfo = pNode->data.GetHead();
		while(NULL != pNodeCellInfo)
		{
			pNodeCellInfo->data.DeleteData();
			pNodeCellInfo = pNodeCellInfo->pNext;
		}
		pNode = pNode->pNext;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyLstCtrl::Clear(BOOL bRedraw /*= FALSE*/)
{
	m_2LnkCellInfo.Clear();
	m_lnkRowInfo.Clear();
	m_rowNum = 0;

	CalcTotalRowHeight();

	//更新垂直滚动条当前位置
	CalcScrollPos();
	CalcRowColToShow();
	CalcRowColToShowOnNoRowData();
	RenewVertScrollPos();
	RenewHoriScrollPos();

	m_rowIdStartShow = -1;
	m_rowIdEndShow = -1;
	m_ssRowTopIndent = 0;
	m_esRowBottomIndent = 0;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMyLstCtrl::SetSelectedRowId(vector<int>& vecSelRowId,BOOL bRedraw /*= FALSE*/)
{
	for(int i = 0;i < m_rowNum;++i)
	{
		m_lnkRowInfo[i]->m_bSelected = FALSE;
	}

	for(int i = 0;i < vecSelRowId.size();++i)
	{
		if(vecSelRowId[i] >= 0 && vecSelRowId[i] < m_rowNum)
		{
			m_lnkRowInfo[vecSelRowId[i]]->m_bSelected = TRUE;
		}
	}
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return 0;
}

vector<int> CMyLstCtrl::GetSelectedRowId()
{
	vector<int> vecSeletedRow;
	vecSeletedRow.clear();
	if(m_rowNum <= 0)
	{
		return vecSeletedRow;
	}

	for(int i = 0;i < m_rowNum;++i)
	{
		if(m_lnkRowInfo[i]->m_bSelected)
		{
			vecSeletedRow.push_back(i);
		}
	}

	return vecSeletedRow;
}

int CMyLstCtrl::SetColumnTitleInfo(int colId,CTitleInfo titleInfo,BOOL bRedraw /*= TRUE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	*(m_lnkTitleInfo[colId]) = titleInfo;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnTitleInfo(int colId,CTitleInfo& titleInfo)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	titleInfo = *(m_lnkTitleInfo[colId]);
	return 0;
}

int CMyLstCtrl::SetColumnCheckFunc(int colId,BOOL bHaveCheckFunc,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_bCheckFunc = bHaveCheckFunc;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnCheckFunc(int colId,BOOL& bHaveCheckFunc)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bHaveCheckFunc = m_lnkTitleInfo[colId]->m_bCheckFunc;
	return 0;
}

int CMyLstCtrl::SetCellCheckFunc(int rowId,int colId,BOOL bCheckable,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}
	if(colId < 0)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	(*m_2LnkCellInfo[rowId])[colId]->SetCheckable(bCheckable);
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return 0;
}

int CMyLstCtrl::GetCellCheckFunc(int rowId,int colId,BOOL& bCheckable)
{
	if(colId >= m_colNum)
	{
		return -1;
	}
	if(colId < 0)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	bCheckable = (*m_2LnkCellInfo[rowId])[colId]->GetCheckable();
	return 0;
}

int CMyLstCtrl::SetColumnColorFunc(int colId,BOOL bHaveColorFunc,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_bColorFunc = bHaveColorFunc;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnColorFunc(int colId,BOOL& bHaveColorFunc)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bHaveColorFunc = m_lnkTitleInfo[colId]->m_bColorFunc;
	return 0;
}

int CMyLstCtrl::SetColumnColorEditable(int colId,BOOL bClrEditable,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_bClrEditale = bClrEditable;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnColorEditable(int colId,BOOL& bClrEditable)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bClrEditable = m_lnkTitleInfo[colId]->m_bClrEditale;
	return 0;
}

int CMyLstCtrl::SetColumnEditable(int colId,BOOL bEditable,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->SetBeEditable(bEditable);
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnEditable(int colId,BOOL& bEditable)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bEditable = m_lnkTitleInfo[colId]->GetBeEditable();
	return 0;
}

int CMyLstCtrl::SetColumnDisableState(int colId,BOOL bDisabled,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->SetBeDisabled(bDisabled);
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnDisableState(int colId,BOOL& bDisabled)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bDisabled = m_lnkTitleInfo[colId]->GetBeDisabled();
	return 0;
}

int CMyLstCtrl::SetColumnTextAlignFormat(int colId,DWORD dwTxAlign,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_dwTextAlign = dwTxAlign;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnTextAlignFormat(int colId,DWORD& dwTxAlign)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	dwTxAlign = m_lnkTitleInfo[colId]->m_dwTextAlign;
	return 0;
}

int CMyLstCtrl::SetColumnMultiDensityState(int colId,BOOL bData,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->SetMultiDensityState(bData);
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnMultiDensityState(int colId,BOOL& bData)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bData = m_lnkTitleInfo[colId]->GetMultiDensityState();
	return 0;
}

int CMyLstCtrl::SetCellEditable(int rowId,int colId,BOOL bEditable,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}
	if(colId < 0)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	(*m_2LnkCellInfo[rowId])[colId]->m_bEditable = bEditable;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetCellEditable(int rowId,int colId,BOOL& bEditable)
{
	if(colId >= m_colNum)
	{
		return -1;
	}
	if(colId < 0)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	bEditable = (*m_2LnkCellInfo[rowId])[colId]->m_bEditable;
	return 0;
}

int CMyLstCtrl::SetColumnCbtable(int colId,BOOL bCbtable,BOOL bCbEditable /*= FALSE*/,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_bCbtable = bCbtable;
	m_lnkTitleInfo[colId]->m_bCbEditable = bCbEditable;
	m_lnkTitleInfo[colId]->m_dwTextAlign = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnCbtable(int colId,BOOL& bCbtable)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bCbtable = m_lnkTitleInfo[colId]->m_bCbtable;
	return 0;
}

int CMyLstCtrl::SetColumnBeTellParentCellChanged(int colId,BOOL bTellParentCellChanged)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_bTellParentCellChanged = bTellParentCellChanged;
	return 0;
}

int CMyLstCtrl::GetColumnBeTellParentCellChanged(int colId,BOOL& bTellParentCellChanged)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	bTellParentCellChanged = m_lnkTitleInfo[colId]->m_bTellParentCellChanged;
	return 0;
}

int CMyLstCtrl::SetColumnCbString(int colId,CLinkList<CString>& lnkCbStr,BOOL bRedraw /*= FALSE*/)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_lnkCbStr = lnkCbStr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetColumnCbString(int colId,CLinkList<CString>& lnkCbStr)
{
	if(colId >= m_colNum)
	{
		return -1;
	}

	if(colId < 0)
	{
		return -2;
	}

	lnkCbStr = m_lnkTitleInfo[colId]->m_lnkCbStr;
	return 0;
}

int CMyLstCtrl::SetColumnWidth(int colId,int colWidth,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}

	if(colId >= m_colNum)
	{
		return -2;
	}

	if(colWidth < 0)
	{
		return -3;
	}

	CTitleInfo* pTitleInfo = m_lnkTitleInfo[colId];
	if(NULL == pTitleInfo)
	{
		return -4;
	}
	pTitleInfo->m_colWidth = colWidth;

	CalcTotalColumnWidth();
	CalcScrollPos();
	CalcRowColToShow();
	RenewHoriScrollPos();
	RenewVertScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	
	return 0;
}

int CMyLstCtrl::GetColumnWidth(int colId)
{
	if(colId < 0)
	{
		return -1;
	}

	if(colId >= m_colNum)
	{
		return -1;
	}

	CTitleInfo* pTitleInfo = m_lnkTitleInfo[colId];
	if(NULL == pTitleInfo)
	{
		return -3;
	}
	return pTitleInfo->m_colWidth;
}

int CMyLstCtrl::SetColumnDataParam(int iColId,CDataParam& dtParam,BOOL bRedraw /*= FALSE*/)
{
	if(iColId < 0)
	{
		return -1;
	}

	if(iColId >= m_colNum)
	{
		return -1;
	}

	CTitleInfo* pTitleInfo = m_lnkTitleInfo[iColId];
	if(NULL == pTitleInfo)
	{
		return -3;
	}

	 pTitleInfo->SetDataParam(dtParam);

	 if(bRedraw && NULL != GetSafeHwnd())
	 {
		 Invalidate(FALSE);
	 }

	 return 0;
}

int CMyLstCtrl::GetColumnDataParam(int iColId,CDataParam& dtParam)
{
	if(iColId < 0)
	{
		return -1;
	}

	if(iColId >= m_colNum)
	{
		return -1;
	}

	CTitleInfo* pTitleInfo = m_lnkTitleInfo[iColId];
	if(NULL == pTitleInfo)
	{
		return -3;
	}

	dtParam = pTitleInfo->GetDataParam();

	return 0;
}

int CMyLstCtrl::SetItemHeight(int itemH,BOOL bRedraw /*= FALSE*/)
{
	m_pxRowH = itemH;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetItemHeight()
{
	return m_pxRowH;
}

int CMyLstCtrl::SetColumnText(int colId,CString strColTitle,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}
	(*(m_lnkTitleInfo[colId])).m_strTitle = strColTitle;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

CString CMyLstCtrl::GetColumnText(int colId)
{
	if(colId < 0)
	{
		return _T("");
	}
	if(colId >= m_colNum)
	{
		return _T("");
	}
	return (*(m_lnkTitleInfo[colId])).m_strTitle;
}

int CMyLstCtrl::SetColumnMaxMinInfo(int colId,CMaxMinInfo maxMinInfo)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}

	m_lnkTitleInfo[colId]->m_maxMinInfo = maxMinInfo;

	for(int i = 0;i < m_rowNum;++i)
	{
		m_2LnkCellInfo[i]->GetAt(colId)->data.m_maxMinInfo = maxMinInfo;
	}

	return 0;
}

int CMyLstCtrl::GetColumnMaxMinInfo(int colId,CMaxMinInfo& maxMinInfo)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}

	maxMinInfo = m_lnkTitleInfo[colId]->m_maxMinInfo;
	return 0;
}

int CMyLstCtrl::SetItemText(int rowId,int colId,CString strText,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	(*m_2LnkCellInfo[rowId])[colId]->SetItemText(strText);

	UpdateRowColumnDisableState(rowId,colId,strText);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

CString CMyLstCtrl::GetItemText(int rowId,int colId)
{
	if(colId < 0)
	{
		return _T("");
	}
	if(colId >= m_colNum)
	{
		return _T("");
	}
	if(rowId < 0)
	{
		return _T("");
	}
	if(rowId >= m_rowNum)
	{
		return _T("");
	}

	return (*m_2LnkCellInfo[rowId])[colId]->GetItemText();
}

int CMyLstCtrl::SetItemColorParam(int rowId,int colId,COLORREF clr,int clrW,int clrH,int iRoundAngle,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	(*m_2LnkCellInfo[rowId])[colId]->SetItemColorParam(clr,clrW,clrH,iRoundAngle);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetItemColorParam(int rowId,int colId,COLORREF& clr,int& clrW,int& clrH,int& iRoundAngle)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	(*m_2LnkCellInfo[rowId])[colId]->GetItemColorParam(clr,clrW,clrH,iRoundAngle);
	return 0;
}

int CMyLstCtrl::SetItemColor(int rowId,int colId,COLORREF clr,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	(*m_2LnkCellInfo[rowId])[colId]->SetItemColor(clr);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetItemColor(int rowId,int colId,COLORREF& clr)
{
	if(colId < 0)
	{
		return -1;
	}
	if(colId >= m_colNum)
	{
		return -2;
	}
	if(rowId < 0)
	{
		return -3;
	}
	if(rowId >= m_rowNum)
	{
		return -4;
	}

	clr = (*m_2LnkCellInfo[rowId])[colId]->GetItemColor();
	return 0;
}

int CMyLstCtrl::SetRowSelectState(int rowId,BOOL bSelected,BOOL bSingleSel /*= FALSE*/,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(bSelected && bSingleSel)
	{
		Node<CRowInfo>* pNode = m_lnkRowInfo.GetHead();
		while(NULL != pNode)
		{
			pNode->data.m_bSelected = FALSE;
			pNode = pNode->pNext;
		}
	}

	m_lnkRowInfo[rowId]->m_bSelected = bSelected;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetRowSelectState(int rowId,BOOL& bSelected)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	bSelected = m_lnkRowInfo[rowId]->m_bSelected;
	return 0;
}

int CMyLstCtrl::SetRowDisableState(int rowId,BOOL bDisabled,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}
	if(rowId >= m_rowNum)
	{
		return -2;
	}
	m_lnkRowInfo[rowId]->m_bDisabled = bDisabled;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetRowDisableState(int rowId,BOOL& bDisabled)
{
	if(rowId < 0)
	{
		return -1;
	}
	if(rowId >= m_rowNum)
	{
		return -2;
	}
	bDisabled = m_lnkRowInfo[rowId]->m_bDisabled;
	return 0;
}

int CMyLstCtrl::SetCellCheckState(int rowId,int colId,BOOL bChecked,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	if(!m_2LnkCellInfo[rowId]->GetAt(colId)->data.GetCheckable() && !m_lnkTitleInfo[colId]->m_bCheckFunc)
	{
		return -5;
	}

	m_2LnkCellInfo[rowId]->GetAt(colId)->data.SetItemCheckState(bChecked);
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	CString strText(_T("FALSE"));
	if(bChecked)
	{
		strText = _T("TRUE");
	}
	UpdateRowColumnDisableState(rowId,colId,strText);

	return 0;
}

int CMyLstCtrl::GetCellCheckState(int rowId,int colId,BOOL& bChecked)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	if(!m_2LnkCellInfo[rowId]->GetAt(colId)->data.GetCheckable() && !m_lnkTitleInfo[colId]->m_bCheckFunc)
	{
		return -5;
	}

	bChecked = m_2LnkCellInfo[rowId]->GetAt(colId)->data.GetItemCheckState();
	return 0;
}

int CMyLstCtrl::SetCellDisableState(int rowId,int colId,BOOL bDisabled,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	m_2LnkCellInfo[rowId]->GetAt(colId)->data.m_bDisabled = bDisabled;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetCellDisableState(int rowId,int colId,BOOL& bDisabled)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	bDisabled = m_2LnkCellInfo[rowId]->GetAt(colId)->data.m_bDisabled;
	return 0;
}

int CMyLstCtrl::SetCellSelfCbInfo(int rowId,int colId,BOOL bCbable,BOOL bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL bEditableCb,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo[rowId]->GetAt(colId)->data;
	cellInfo.m_bCbAble = bCbable;
	cellInfo.m_bUseCellLnkCbStr = bUseCellLnkCbStr;
	cellInfo.m_lnkCbStr = lnkCbStr;
	cellInfo.m_bEditableCb = bEditableCb;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::SetCellSelfCbInfo(int rowId,int colId,BOOL bCbable,BOOL bUseCellLnkCbStr,BOOL bEditableCb,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo[rowId]->GetAt(colId)->data;
	cellInfo.m_bCbAble = bCbable;
	cellInfo.m_bUseCellLnkCbStr = bUseCellLnkCbStr;
	cellInfo.m_bEditableCb = bEditableCb;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::SetCellSelfCbInfo(int rowId,int colId,CLinkList<CString>& lnkCbStr,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo[rowId]->GetAt(colId)->data;
	cellInfo.m_lnkCbStr = lnkCbStr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetCellSelfCbInfo(int rowId,int colId,BOOL& bCbable,BOOL& bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL& bEditableCb)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo[rowId]->GetAt(colId)->data;
	bCbable = cellInfo.m_bCbAble;
	bUseCellLnkCbStr = cellInfo.m_bUseCellLnkCbStr;
	lnkCbStr = cellInfo.m_lnkCbStr;
	bEditableCb = cellInfo.m_bEditableCb;
	return 0;
}

int CMyLstCtrl::GetCellSelfCbInfo(int rowId,int colId,CLinkList<CString>& lnkCbStr)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo[rowId]->GetAt(colId)->data;
	lnkCbStr = cellInfo.m_lnkCbStr;
	return 0;
}

int CMyLstCtrl::GetRowNum()
{
	return m_rowNum;
}

int CMyLstCtrl::GetColNum()
{
	return m_colNum;
}

void CMyLstCtrl::SetBeGradientHeader(BOOL bGradientHeader,BOOL bRedraw /*= FALSE*/)
{
	m_bGradientHeader = bGradientHeader;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeGradientHeader()
{
	return m_bGradientHeader;
}

void CMyLstCtrl::SetBkColor(Color bkClr,BOOL bRedraw /*= FALSE*/)
{
	m_clrBk = bkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

Color CMyLstCtrl::GetBkColor()
{
	return m_clrBk;
}

void CMyLstCtrl::SetHeaderBkColor(Color headerBkClr,BOOL bRedraw /*= FALSE*/)
{
	m_clrHeaderBk = headerBkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

Color CMyLstCtrl::GetHeaderBkColor()
{
	return m_clrHeaderBk;
}

void CMyLstCtrl::SetBodyBkColor(Color bodyBkClr,BOOL bRedraw /*= FALSE*/)
{
	m_clrBodyBk = bodyBkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

Color CMyLstCtrl::GetBodyBkColor()
{
	return m_clrBodyBk;
}

void CMyLstCtrl::SetHeaderHeight(COLORREF headerHeight,BOOL bRedraw /*= FALSE*/)
{
	m_pxHeaderH = headerHeight;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMyLstCtrl::GetHeaderHeight()
{
	return m_pxHeaderH;
}

void CMyLstCtrl::SetFrameLineWidth(int frameLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_pxFrame = frameLineWidth;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMyLstCtrl::GetFrameLineWidth()
{
	return m_pxFrame;
}

void CMyLstCtrl::SetFrameLineColor(Color frameLineColor,BOOL bRedraw /*= FALSE*/)
{
	m_clrFrame = frameLineColor;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

Color CMyLstCtrl::GetFrameLineColor()
{
	return m_clrFrame;
}

void CMyLstCtrl::SetRowColLineWidth(int rowColLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_pxRowColLine = rowColLineWidth;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMyLstCtrl::GetRowColLineWidth()
{
	return m_pxRowColLine;
}


void CMyLstCtrl::SetRowColLineColor(COLORREF rowColLineColor,BOOL bRedraw /*= FALSE*/)
{
	m_clrRowColLine = rowColLineColor;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMyLstCtrl::GetRowColLineColor()
{
	return m_clrRowColLine;
}

void CMyLstCtrl::SetBeShowFrameLine(int bShowFrameLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowFrameLine = bShowFrameLine;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeShowFrameLine()
{
	return m_bShowFrameLine;
}

void CMyLstCtrl::SetBeShowRowColLine(int bShowRowColLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowRowColLine = bShowRowColLine;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeShowRowColLine()
{
	return m_bShowRowColLine;
}

void CMyLstCtrl::SetHeaderTextColor(COLORREF headTextColor,BOOL bRedraw /*= FALSE*/)
{
	m_clrHeaderText = headTextColor;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMyLstCtrl::GetHeaderTextColor()
{
	return m_clrHeaderText;
}

void CMyLstCtrl::SetBeSingleSelect(BOOL bSingleSelect,BOOL bRedraw /*= FALSE*/)
{
	m_bSingleSelect = bSingleSelect;

	vector<int> vecSelectedRow = GetSelectedRowId();	//如果有多行被选中，设置第一行选中
	for(int i = 1;i < vecSelectedRow.size();++i)
	{
		SetRowSelectState(vecSelectedRow[i],FALSE);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeSingleSelect()
{
	return m_bSingleSelect;
}

void CMyLstCtrl::SetBeAllowCancelRowSelect(BOOL bValue,BOOL bRedraw /*= FALSE*/)
{
	m_bAllowCancelRowSelect = bValue;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeAllowCancelRowSelect()
{
	return m_bAllowCancelRowSelect;
}

void CMyLstCtrl::SetTitle(CString strTitle,BOOL bRedraw /*= FALSE*/)
{
	m_strLstTitle = strTitle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

CString CMyLstCtrl::GetTitle()
{
	return m_strLstTitle;
}

int CMyLstCtrl::SetMapColumnDisableControl(int colId,CLinkList<int>& lnkDisabledCol)
{
	if(colId < 0)
	{
		return -1;
	}

	if(colId > m_colNum)
	{
		return -2;
	}

	m_mapColDisableCtrl[colId] = lnkDisabledCol;
	return 0;
}

int CMyLstCtrl::GetMapColumnDisableControl(int colId,CLinkList<int>& lnkDisabledCol)
{
	lnkDisabledCol.Clear();

	if(colId < 0)
	{
		return -1;
	}

	if(colId > m_colNum)
	{
		return -2;
	}

	map<int,CLinkList<int>>::iterator iter = m_mapColDisableCtrl.find(colId);
	if(m_mapColDisableCtrl.end() == iter)
	{
		return -3;
	}

	lnkDisabledCol = iter->second;

	return 0;
}

int CMyLstCtrl::DeleteColumnDisableControl(int colId,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}

	if(colId > m_colNum)
	{
		return -2;
	}

	map<int,CLinkList<int>>::iterator iter = m_mapColDisableCtrl.find(colId);
	if(m_mapColDisableCtrl.end() == iter)
	{
		return -3;
	}

	iter->second.Clear();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

void CMyLstCtrl::ClearMapColumnDisableControl()
{
	m_mapColDisableCtrl.clear();
	return;
}

int CMyLstCtrl::SetMapRowOutsideControl(int rowId,CLinkList<int>& lnkDisabledRow)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId > m_rowNum)
	{
		return -2;
	}

	m_mapRowOutsideCtrl[rowId] = lnkDisabledRow;
	return 0;
}

int CMyLstCtrl::GetMapRowOutsideControl(int rowId,CLinkList<int>& lnkDisabledRow)
{
	lnkDisabledRow.Clear();

	if(rowId < 0)
	{
		return -1;
	}

	if(rowId > m_rowNum)
	{
		return -2;
	}

	map<int,CLinkList<int>>::iterator iter = m_mapRowOutsideCtrl.find(rowId);
	if(m_mapRowOutsideCtrl.end() == iter)
	{
		return -3;
	}

	lnkDisabledRow = iter->second;

	return 0;
}

int CMyLstCtrl::DeleteRowOutsideControl(int rowId,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId > m_rowNum)
	{
		return -2;
	}

	map<int,CLinkList<int>>::iterator iter = m_mapRowOutsideCtrl.find(rowId);
	if(m_mapRowOutsideCtrl.end() == iter)
	{
		return -3;
	}

	iter->second.Clear();

	return 0;
}

void CMyLstCtrl::ClearMapRowOutsideControl()
{
	m_mapRowOutsideCtrl.clear();
	return;
}

int CMyLstCtrl::UpdateRowColumnDisableState(int rowId,int colId,CString strText,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}
	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}
	if(colId >= m_colNum)
	{
		return -4;
	}

	map<int,CLinkList<int>>::iterator iter = m_mapColDisableCtrl.find(colId);
	if(m_mapColDisableCtrl.end() == iter)
	{
		return 1;
	}

	CLinkList<int>& lnkColControled = iter->second;
	if(lnkColControled.GetSize() < 1)
	{
		return 2;
	}

	map<CString,BOOL>::iterator iterDisabledState = m_mapDisabledStateKeyString.find(strText);
	if(m_mapDisabledStateKeyString.end() == iterDisabledState)
	{
		return -5;
	}

	Node<int>* pNode = lnkColControled.GetHead();
	while(NULL != pNode)
	{
		CCellInfo& cellInfo = m_2LnkCellInfo[rowId]->GetAt(pNode->data)->data;
		cellInfo.m_bDisabled = iterDisabledState->second;
		pNode = pNode->pNext;
	}
	
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyLstCtrl::TellParentSomething(int rowId,int colId,CString strText,BOOL bRedraw /*= FALSE*/)
{
	if(rowId < 0)
	{
		return -1;
	}
	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}
	if(colId >= m_colNum)
	{
		return -4;
	}

	map<int,CLinkList<int>>::iterator iter = m_mapRowOutsideCtrl.find(rowId);
	if(m_mapRowOutsideCtrl.end() == iter)
	{
		return 1;
	}

	CLinkList<int>& lnkRowOutside = iter->second;
	if(lnkRowOutside.GetSize() < 1)
	{
		return 2;
	}

	map<CString,BOOL>::iterator iterDisabledState = m_mapDisabledStateKeyString.find(strText);
	if(m_mapDisabledStateKeyString.end() == iterDisabledState)
	{
		return -5;
	}

	CWnd* pParent = GetParent();
	if(NULL != pParent->GetSafeHwnd())
	{
		Node<int>* pNode = lnkRowOutside.GetHead();
		while(NULL != pNode)
		{
			pParent->PostMessage(WM_TELLPARENTROWCHANGED,pNode->data,iterDisabledState->second);
			pNode = pNode->pNext;
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyLstCtrl::TellParentCellChanged(int rowId,int colId)
{
	if(rowId < 0)
	{
		return -1;
	}
	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}
	if(colId >= m_colNum)
	{
		return -4;
	}

	if(!m_lnkTitleInfo[colId]->m_bTellParentCellChanged)
	{
		return 1;
	}

	CWnd* pParent = GetParent();
	if(NULL == pParent->GetSafeHwnd())
	{
		return 2;
	}
	pParent->PostMessage(WM_TELLPARENTCELLCHANGED,rowId,colId);

	return 0;
}

int CMyLstCtrl::AddPairToMapDisabledStateKeyString(CString strKey,BOOL bDisabled,BOOL bRedraw /*= FALSE*/)
{
	m_mapDisabledStateKeyString[strKey] = bDisabled;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyLstCtrl::GetKeyStringDisabledState(CString strKey,BOOL& bDisabled)
{
	map<CString,BOOL>::iterator iter = m_mapDisabledStateKeyString.find(strKey);
	if(m_mapDisabledStateKeyString.end() == iter)
	{
		return -1;
	}

	bDisabled = iter->second;
	return 0;
}

int CMyLstCtrl::DeleteKeyStringDisabledState(CString strKey,BOOL bRedraw /*= FALSE*/)
{
	map<CString,BOOL>::iterator iter = m_mapDisabledStateKeyString.find(strKey);
	if(m_mapDisabledStateKeyString.end() == iter)
	{
		return -1;
	}

	m_mapDisabledStateKeyString.erase(iter);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

void CMyLstCtrl::ClearMapDisabledStateKeyString()
{
	m_mapDisabledStateKeyString.clear();
	return;
}

void CMyLstCtrl::SetBeEditable(BOOL bEditable,BOOL bRedraw /*= FALSE*/)
{
	if(bEditable == m_bEditable)
	{
		return;
	}

	m_bEditable = bEditable;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeEditable()
{
	return m_bEditable;
}

int CMyLstCtrl::CalcTotalColumnWidth()
{
	m_totalColW = 0;

	Node<CTitleInfo>* pNode = m_lnkTitleInfo.GetHead();
	while(NULL != pNode)
	{
		m_totalColW += pNode->data.m_colWidth;
		pNode = pNode->pNext;
	}

	return m_totalColW;
}

int CMyLstCtrl::CalcTotalRowHeight()
{
	m_totalRowH = 0;
	Node<CRowInfo>* pNode = m_lnkRowInfo.GetHead();
	while(NULL != pNode)
	{
		m_totalRowH += pNode->data.m_rowH;
		pNode = pNode->pNext;
	}

	return m_totalRowH;
}

int CMyLstCtrl::CalcRowColToShow()
{
	if(m_colNum > 0)
	{
		if(m_totalColW > m_rcBody.Width())
		{
			if(-1 == m_colIdStartShow)
			{
				m_colIdStartShow = 0;
				m_ssColLeftIndent = 0;
			}

			int sum = m_lnkTitleInfo[m_colIdStartShow]->m_colWidth - m_ssColLeftIndent;
			int i = m_colIdStartShow + 1;
			for(;i < m_colNum;++i)
			{
				int colWidth = m_lnkTitleInfo[i]->m_colWidth;
				sum += colWidth;
				if(sum == m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = sum - m_rcBody.Width();
					break;
				}
			}
		}
		else
		{
			m_colIdStartShow = 0;
			m_colIdEndShow = m_colNum - 1;
			m_ssColLeftIndent = 0;
			m_esColRightIndent = 0;
		}
	}
	else
	{
		m_colIdStartShow = -1;
		m_colIdEndShow = -1;
		m_ssColLeftIndent = 0;
		m_esColRightIndent = 0;
	}
	m_cdcW = GetWidth(m_colIdStartShow,m_colIdEndShow); 

	if(m_rowNum > 0)
	{
		if(m_totalRowH > m_rcBody.Height())
		{
			if(-1 == m_rowIdStartShow)
			{
				m_rowIdStartShow = 0;
				m_ssRowTopIndent = 0;
			}

			int sum = m_lnkRowInfo[m_rowIdStartShow]->m_rowH - m_ssRowTopIndent;
			int i = m_rowIdStartShow + 1;
			for(;i < m_rowNum;++i)
			{
				int rowH = m_lnkRowInfo[m_rowIdStartShow]->m_rowH;
				sum += rowH;
				if(sum == m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = sum - m_rcBody.Height();
					break;
				}
			}
		}
		else
		{
			m_rowIdStartShow = 0;
			m_rowIdEndShow = m_rowNum - 1;
			m_ssRowTopIndent = 0;
			m_esRowBottomIndent = 0;
		}
	}
	else
	{
		m_rowIdStartShow = -1;
		m_rowIdEndShow = -1;
		m_ssRowTopIndent = 0;
		m_esRowBottomIndent = 0;
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 

	return 0;
}

int CMyLstCtrl::CalcRowColToShowOnNoRowData()
{
	if(m_rcBody.Height() < 1)
	{
		return -1;
	}

	int sum = m_pxRowH;
	m_rowIdSeparatorStart = 0;
	m_rowIdSeparatorEnd = 0;
	while(sum < m_rcBody.Height())
	{
		sum += m_pxRowH;
		++m_rowIdSeparatorEnd;
	}
	m_esSeparatorBottomIndent = sum - m_rcBody.Height();

	return 0;
}

int CMyLstCtrl::CalcColToShow(int newStartCol)
{
	if(m_colNum > 0 && newStartCol < m_colNum)
	{
		if(m_totalColW > m_rcBody.Width())
		{
			int errNum = 0;
			int sum = 0;
			int i = 0;

REFIND:
			if(newStartCol < 0)
			{
				m_colIdStartShow = 0;

				//防止陷入死循环,20211122,孙文郎
				if(++errNum > 10)
				{
					m_colIdStartShow = 0;
					m_colIdEndShow = m_colNum - 1;
					m_ssColLeftIndent = 0;
					m_esColRightIndent = 0;
					m_cdcW = GetWidth(m_colIdStartShow,m_colIdEndShow); 
					return -1;
				}
			}

			m_colIdStartShow = newStartCol;
			m_ssColLeftIndent = 0;
			m_esColRightIndent = 0;  

			sum = m_lnkTitleInfo[m_colIdStartShow]->m_colWidth;
			i = m_colIdStartShow + 1;
			for(;i < m_colNum;++i)
			{
				int colWidth = m_lnkTitleInfo[i]->m_colWidth;
				sum += colWidth;
				if(sum == m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = sum - m_rcBody.Width();
					break;
				}
				else if(m_colNum - 1 == i)
				{
					--newStartCol;
					goto REFIND;
				}
			}
		}
		else
		{
			m_colIdStartShow = 0;
			m_colIdEndShow = m_colNum - 1;
			m_ssColLeftIndent = 0;
			m_esColRightIndent = 0;
		}
	}
	else
	{
		m_colIdStartShow = -1;
		m_colIdEndShow = -1;
		m_ssColLeftIndent = 0;
		m_esColRightIndent = 0;
	}
	m_cdcW = GetWidth(m_colIdStartShow,m_colIdEndShow); 

	return 0;
}

int CMyLstCtrl::CalcRowToShow(int newStartRow)
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
					m_ssRowTopIndent = 0;
					m_esRowBottomIndent = 0;
					m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 
					return -1;
				}
			}

			sum = m_lnkRowInfo[m_rowIdStartShow]->m_rowH - m_ssRowTopIndent;
			i = m_rowIdStartShow + 1;
			for(;i < m_rowNum;++i)
			{
				int rowH = m_lnkRowInfo[m_rowIdStartShow]->m_rowH;
				sum += rowH;
				if(sum == m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = sum - m_rcBody.Height();
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
			m_ssRowTopIndent = 0;
			m_esRowBottomIndent = 0;
		}
	}
	else
	{
		m_rowIdStartShow = -1;
		m_rowIdEndShow = -1;
		m_ssRowTopIndent = 0;
		m_esRowBottomIndent = 0;
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 

	return 0;
}

int CMyLstCtrl::ScrollCol(int pxHoriShift)
{
	if(m_colNum <= 0)
	{
		return -1;
	}

	if(pxHoriShift > 0 && m_horiScroll.m_curPixelPos + pxHoriShift > m_totalColW - m_rcBody.Width())
	{
		pxHoriShift -= (m_horiScroll.m_curPixelPos + pxHoriShift - m_totalColW + m_rcBody.Width());
	}

	if(pxHoriShift < 0 && m_horiScroll.m_curPixelPos + pxHoriShift < 0)
	{
		pxHoriShift -= (m_horiScroll.m_curPixelPos + pxHoriShift);
	}

	if(0 == pxHoriShift)
	{
		return 1;
	}

	int tempHoriShift = pxHoriShift;

	if(m_totalColW > m_rcBody.Width())
	{
		if(-1 == m_colIdStartShow)
		{
			m_colIdStartShow = 0;
			m_ssColLeftIndent = 0;
		}
		else
		{
			if(pxHoriShift > 0)
			{
				int colWidth = m_lnkTitleInfo[m_colIdStartShow]->m_colWidth;
				if(colWidth - m_ssColLeftIndent > pxHoriShift)
				{
					m_ssColLeftIndent += pxHoriShift;
				}
				else
				{
					int remainder = pxHoriShift - (colWidth - m_ssColLeftIndent);
					for(int i = m_colIdStartShow + 1;i < m_colNum;remainder -= m_lnkTitleInfo[i]->m_colWidth,++i)
					{
						if(remainder < m_lnkTitleInfo[i]->m_colWidth)
						{
							m_colIdStartShow = i;
							m_ssColLeftIndent = remainder;
							break;
						}
					}
				}
			}
			else
			{
				pxHoriShift = abs(pxHoriShift);
				if(m_ssColLeftIndent >= pxHoriShift)
				{
					m_ssColLeftIndent -= pxHoriShift;
				}
				else
				{
					int remainder = pxHoriShift - m_ssColLeftIndent;
					for(int i = m_colIdStartShow - 1;i >= 0;remainder -= m_lnkTitleInfo[i]->m_colWidth,--i)
					{
						//=号在这里至关重要,20211122,孙文郎
						if(remainder <= m_lnkTitleInfo[i]->m_colWidth)
						{
							m_colIdStartShow = i;
							m_ssColLeftIndent = m_lnkTitleInfo[i]->m_colWidth - remainder;
							break;
						}
					}
				}
			}
		}

		int sum = m_lnkTitleInfo[m_colIdStartShow]->m_colWidth - m_ssColLeftIndent;
		int i = m_colIdStartShow + 1;
		for(;i < m_colNum;++i)
		{
			sum += m_lnkTitleInfo[i]->m_colWidth;
			if(sum == m_rcBody.Width())
			{
				m_colIdEndShow = i;
				m_esColRightIndent = 0;
				break;
			}
			else if(sum > m_rcBody.Width())
			{
				m_colIdEndShow = i;
				m_esColRightIndent = sum - m_rcBody.Width();
				break;
			}
			else if(i + 1 == m_colNum)
			{
				if(abs(sum - m_rcBody.Width()) < 2)
				{
					m_colIdEndShow = i;
					m_esColRightIndent = 0;
				}
			}
		}
	}
	m_cdcW = GetWidth(m_colIdStartShow,m_colIdEndShow); 

	m_horiScroll.m_curPixelPos += tempHoriShift;
	m_horiScroll.CalcSliderPos();
	Invalidate(FALSE);

	return 0;
}

int CMyLstCtrl::ScrollRow(int pxVertShift)
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

#ifdef _DEBUG
	int temp1 = m_rowIdStartShow;
	int temp2 = m_ssRowTopIndent;

	if(2 == temp1 && 4 == temp2)
	{
		int tt = 0;
		++tt;
	}
#endif

	int tempVertShift = pxVertShift;

	if(m_totalRowH > m_rcBody.Height())
	{
		if(-1 == m_rowIdStartShow)
		{
			m_rowIdStartShow = 0;
			m_ssRowTopIndent = 0;
		}
		else
		{
			if(pxVertShift > 0)
			{
				int rowH = m_lnkRowInfo[m_rowIdStartShow]->m_rowH;
				if(rowH - m_ssRowTopIndent > pxVertShift)
				{
					m_ssRowTopIndent += pxVertShift;
				}
				else
				{
					int remainder = pxVertShift - (rowH - m_ssRowTopIndent);
					for(int i = m_rowIdStartShow + 1;i < m_rowNum;remainder -= m_lnkRowInfo[i]->m_rowH,++i)
					{
						if(remainder < m_lnkRowInfo[i]->m_rowH)
						{
							m_rowIdStartShow = i;
							m_ssRowTopIndent = remainder;
							break;
						}
					}
				}
			}
			else
			{
				pxVertShift = abs(pxVertShift);
				if(m_ssRowTopIndent >= pxVertShift)
				{
					m_ssRowTopIndent -= pxVertShift;
				}
				else
				{
					int remainder = pxVertShift - m_ssRowTopIndent;
					for(int i = m_rowIdStartShow - 1;i >= 0;remainder -= m_lnkRowInfo[i]->m_rowH,--i)
					{
						//=号在这里至关重要,20211122,孙文郎
						if(remainder <= m_lnkRowInfo[i]->m_rowH)
						{
							m_rowIdStartShow = i;
							m_ssRowTopIndent = m_lnkRowInfo[i]->m_rowH - remainder;
							break;
						}
					}
				}
			}
		}

		int sum = m_lnkRowInfo[m_rowIdStartShow]->m_rowH - m_ssRowTopIndent;
		int i = m_rowIdStartShow + 1;
		for(;i < m_rowNum;++i)
		{
			sum += m_lnkRowInfo[i]->m_rowH;
			if(sum == m_rcBody.Height())
			{
				m_rowIdEndShow = i;
				m_esRowBottomIndent = 0;
				break;
			}
			else if(sum > m_rcBody.Height())
			{
				m_rowIdEndShow = i;
				m_esRowBottomIndent = sum - m_rcBody.Height();
				break;
			}
			else if(i + 1 == m_rowNum)
			{
				if(abs(sum - m_rcBody.Height()) < 2)
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = 0;
				}
#ifdef _DEBUG
				if(sum < m_rcBody.Height())
				{
					int tt = 0;
					++tt;
				}

				if(tempVertShift != pxVertShift)
				{  
					int kk = 0;
					++kk;
				}
#endif
			}
		}
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 

	m_vertScroll.m_curPixelPos += tempVertShift;

#ifdef _DEBUG
	if(m_rowIdStartShow * 28 + m_ssRowTopIndent > m_vertScroll.m_curPixelPos)
	{
		int kk = 0;
		++kk;
	}

	if(240 == m_vertScroll.m_curPixelPos && 10 == m_rowIdStartShow)
	{
		int kk = 0;
		++kk;
	}
#endif

	m_vertScroll.CalcSliderPos();
	Invalidate(FALSE);

	return 0;
}

int CMyLstCtrl::ScrollColToFixPos(int pxFixPos)
{
	if(m_colNum <= 0)
	{
		return -1;
	}
	if(pxFixPos < 0)
	{
		return -2;
	}

	int newSum = 0;
	if(m_colNum > 0)
	{
		int sum = 0;
		for(int i = 0;i < m_colNum;++i)
		{
			sum += m_lnkTitleInfo[i]->m_colWidth;
			if(sum == pxFixPos)
			{
				m_colIdStartShow = i;
				m_ssColLeftIndent = 0;
				break;
			}
			else if(sum > pxFixPos)
			{
				m_colIdStartShow = i;
				m_ssColLeftIndent = m_lnkTitleInfo[i]->m_colWidth - (sum - pxFixPos);
				break;
			}
		}

		if(m_totalColW > m_rcBody.Width())
		{
			if(-1 == m_colIdStartShow)
			{
				m_colIdStartShow = 0;
				m_ssColLeftIndent = 0;
			}

			newSum = m_lnkTitleInfo[m_colIdStartShow]->m_colWidth - m_ssColLeftIndent;
			int i = m_colIdStartShow + 1;
			for(;i < m_colNum;++i)
			{
				newSum += m_lnkTitleInfo[i]->m_colWidth;
				if(newSum == m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = 0;
					break;
				}
				else if(newSum > m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = newSum - m_rcBody.Width();
					break;
				}
				else if(i + 1 == m_colNum)
				{
					if(abs(newSum - m_rcBody.Width()) < 2)
					{
						m_colIdEndShow = i;
						m_esColRightIndent = 0;
					}
				}
			}
		}
		else
		{
			m_colIdEndShow = m_colNum - 1;
			m_esColRightIndent = 0;
		}
	}
	else
	{
		m_colIdStartShow = -1;
		m_colIdEndShow = -1;
		m_ssColLeftIndent = 0;
		m_esColRightIndent = 0;
	}
	m_cdcW = GetWidth(m_colIdStartShow,m_colIdEndShow);

	return 0;
}

int CMyLstCtrl::ScrollRowToFixPos(int pxFixPos)
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
			sum += m_lnkRowInfo[i]->m_rowH;
			if(sum == pxFixPos)
			{
				m_rowIdStartShow = i;
				m_ssRowTopIndent = 0;
				break;
			}
			else if(sum > pxFixPos)
			{
				m_rowIdStartShow = i;
				m_ssRowTopIndent = m_lnkRowInfo[i]->m_rowH - (sum - pxFixPos);
				break;
			}
		}

		if(m_totalRowH > m_rcBody.Height())
		{
			if(-1 == m_rowIdStartShow)
			{
				m_rowIdStartShow = 0;
				m_ssRowTopIndent = 0;
			}

			int newSum = m_lnkRowInfo[m_rowIdStartShow]->m_rowH - m_ssRowTopIndent;
			int i = m_rowIdStartShow + 1;
			for(;i < m_rowNum;++i)
			{
				newSum += m_lnkRowInfo[i]->m_rowH;
				if(newSum == m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = 0;
					break;
				}
				else if(newSum > m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = newSum - m_rcBody.Height();
					break;
				}
				else if(i + 1 == m_rowNum)
				{
					if(abs(newSum - m_rcBody.Height()) < 2)
					{
						m_rowIdEndShow = i;
						m_esRowBottomIndent = 0;
					}
				}
			}
		}
		else
		{
			m_rowIdEndShow = m_rowNum - 1;
			m_esRowBottomIndent = 0;
		}
	}
	else
	{
		m_rowIdStartShow = -1;
		m_rowIdEndShow = -1;
		m_ssRowTopIndent = 0;
		m_esRowBottomIndent = 0;
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

void CMyLstCtrl::CalcScrollPos()
{
	CalcScrollPosStyle2();
	return;

	if(m_totalColW > m_rcBody.Width())
	{
		m_horiScroll.m_bScrollNeedShow = TRUE;	
	}
	else
	{
		m_horiScroll.m_bScrollNeedShow = FALSE;	
	}

	if(m_totalRowH > m_rcBody.Height())
	{
		m_vertScroll.m_bScrollNeedShow = TRUE;		
	}
	else
	{
		m_vertScroll.m_bScrollNeedShow = FALSE;	
	}

	if(m_horiScroll.m_bScrollNeedShow)
	{
		m_rcBody.bottom = m_rcLst.bottom - m_pxFrame - m_horiScroll.m_scrollH;
		if(m_vertScroll.m_bScrollNeedShow)
		{
			if(ESHVCM_HORI == m_eScrollHVCM)
			{
				m_horiScroll.m_rcScroll = CRect(m_pxFrame,m_rcLst.Height() - m_pxFrame - m_horiScroll.m_scrollH,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
			}
			else
			{
				m_horiScroll.m_rcScroll = CRect(m_pxFrame,m_rcLst.Height()- m_pxFrame - m_horiScroll.m_scrollH,m_rcLst.Width() - m_pxFrame - m_vertScroll.m_scrollW,m_rcLst.Height() - m_pxFrame);
			}
		}
		else
		{
			m_horiScroll.m_rcScroll = CRect(m_pxFrame,m_rcLst.Height() - m_pxFrame - m_horiScroll.m_scrollH,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
		}
	}
	else
	{
		m_rcBody.bottom = m_rcLst.bottom - m_pxFrame;
	}
	
	if(m_vertScroll.m_bScrollNeedShow)
	{
		m_rcBody.right = m_rcLst.right - m_pxFrame - m_vertScroll.m_scrollW;
		m_rcHeader.right = m_rcLst.right - m_pxFrame - m_vertScroll.m_scrollW;
		if(m_horiScroll.m_bScrollNeedShow)
		{
			if(ESHVCM_VERT == m_eScrollHVCM)
			{
				m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_pxFrame - m_vertScroll.m_scrollW,m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
			}
			else
			{
				m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_pxFrame - m_vertScroll.m_scrollW,m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame - m_horiScroll.m_scrollH);
			}
		}
		else
		{
			m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_pxFrame - m_vertScroll.m_scrollW,m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
		}
	}
	else
	{
		m_rcBody.right = m_rcLst.right - m_pxFrame;
		m_rcHeader.right = m_rcLst.right - m_pxFrame;
	}

	//防止水平滚动条信息设置错误，20211120，孙文郎
	if(m_horiScroll.m_bScrollNeedShow)
	{
		m_horiScroll.m_pixelTotal = m_totalColW;
		m_horiScroll.m_pixelForShow = m_rcBody.Width();
		m_horiScroll.CalcSliderPos();
	}
	
	if(m_vertScroll.m_bScrollNeedShow)
	{
		m_vertScroll.m_pixelTotal = m_totalRowH;
		m_vertScroll.m_pixelForShow = m_rcBody.Height();
		m_vertScroll.CalcSliderPos();
	}

	return;
}

void CMyLstCtrl::CalcScrollPosStyle2()
{
	m_horiScroll.m_bScrollNeedShow = (m_totalColW > m_rcBody.Width());	
	m_vertScroll.m_bScrollNeedShow = (m_totalRowH > m_rcBody.Height());

	if(m_horiScroll.m_bScrollNeedShow && !m_vertScroll.m_bScrollNeedShow)
	{
		if(m_totalRowH > m_rcBody.Height() - m_horiScroll.m_scrollH)
		{
			m_vertScroll.m_bScrollNeedShow = TRUE;
		}
	}

	if(m_vertScroll.m_bScrollNeedShow && !m_horiScroll.m_bScrollNeedShow)
	{
		if(m_totalColW > m_rcBody.Width() - m_vertScroll.m_scrollW)
		{
			m_horiScroll.m_bScrollNeedShow = TRUE;
		}
	}

	if(m_bScrollInsideFrame)
	{
		if(m_horiScroll.m_bScrollNeedShow)
		{
			m_rcBody.bottom = m_rcLst.bottom - m_horiScroll.m_scrollH - m_pxFrame;
			if(m_vertScroll.m_bScrollNeedShow)
			{
				if(ESHVCM_HORI == m_eScrollHVCM)
				{
					m_horiScroll.m_rcScroll = CRect(m_pxFrame,m_rcLst.Height() - m_horiScroll.m_scrollH - m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
				}
				else
				{
					m_horiScroll.m_rcScroll = CRect(m_pxFrame,m_rcLst.Height() - m_horiScroll.m_scrollH - m_pxFrame,m_rcLst.Width() - m_vertScroll.m_scrollW - m_pxFrame,m_rcLst.Height() - m_pxFrame);
				}
			}
			else
			{
				m_horiScroll.m_rcScroll = CRect(m_pxFrame,m_rcLst.Height() - m_horiScroll.m_scrollH - m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
			}
		}
		else
		{
			m_rcBody.bottom = m_rcLst.bottom - m_pxFrame;
		}

		if(m_vertScroll.m_bScrollNeedShow)
		{
			m_rcBody.right = m_rcLst.right - m_pxFrame - m_vertScroll.m_scrollW;
			m_rcHeader.right = m_rcLst.right - m_pxFrame - m_vertScroll.m_scrollW;

			if(m_horiScroll.m_bScrollNeedShow)
			{
				if(ESHVCM_VERT == m_eScrollHVCM)
				{
					m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_vertScroll.m_scrollW - m_pxFrame,m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
				}
				else
				{
					m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_vertScroll.m_scrollW - m_pxFrame,m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_horiScroll.m_scrollH - m_pxFrame);
				}
			}
			else
			{
				m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_vertScroll.m_scrollW - m_pxFrame,m_pxFrame,m_rcLst.Width() - m_pxFrame,m_rcLst.Height() - m_pxFrame);
			}
		}
		else
		{
			m_rcBody.right = m_rcLst.right - m_pxFrame;
			m_rcHeader.right = m_rcLst.right - m_pxFrame;
		}
	}
	else
	{
		if(m_horiScroll.m_bScrollNeedShow)
		{
			m_rcBody.bottom = m_rcLst.bottom - m_horiScroll.m_scrollH - m_pxFrame;

			if(m_vertScroll.m_bScrollNeedShow)
			{
				if(ESHVCM_HORI == m_eScrollHVCM)
				{
					m_horiScroll.m_rcScroll = CRect(0,m_rcLst.Height() - m_horiScroll.m_scrollH,m_rcLst.Width(),m_rcLst.Height());
				}
				else
				{
					m_horiScroll.m_rcScroll = CRect(0,m_rcLst.Height() - m_horiScroll.m_scrollH,m_rcLst.Width() - m_vertScroll.m_scrollW,m_rcLst.Height());
				}
			}
			else
			{
				m_horiScroll.m_rcScroll = CRect(0,m_rcLst.Height() - m_horiScroll.m_scrollH,m_rcLst.Width(),m_rcLst.Height());
			}
		}
		else
		{
			m_rcBody.bottom = m_rcLst.bottom - m_pxFrame;
		}

		if(m_vertScroll.m_bScrollNeedShow)
		{
			m_rcBody.right = m_rcLst.right - m_pxFrame - m_vertScroll.m_scrollW;
			m_rcHeader.right = m_rcLst.right - m_pxFrame - m_vertScroll.m_scrollW;

			if(m_horiScroll.m_bScrollNeedShow)
			{
				if(ESHVCM_VERT == m_eScrollHVCM)
				{
					m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_vertScroll.m_scrollW,0,m_rcLst.Width(),m_rcLst.Height());
				}
				else
				{
					m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_vertScroll.m_scrollW,0,m_rcLst.Width(),m_rcLst.Height() - m_horiScroll.m_scrollH);
				}
			}
			else
			{
				m_vertScroll.m_rcScroll = CRect(m_rcLst.Width() - m_vertScroll.m_scrollW,0,m_rcLst.Width(),m_rcLst.Height());
			}
		}
		else
		{
			m_rcBody.right = m_rcLst.right - m_pxFrame;
			m_rcHeader.right = m_rcLst.right - m_pxFrame;
		}
	}

	return;
}

void CMyLstCtrl::EndEDit()
{
	if(!m_rowColEditInfo.BeValidRowCol())
	{
		return;
	}

	if(NULL != m_edInput.GetSafeHwnd() && m_edInput.IsWindowVisible())
	{
		CString strTemp(_T(""));
		strTemp = m_edInput.GetText();
		m_rowColEditInfo.m_strNewText = strTemp;
		m_edInput.ShowWindow(SW_HIDE);
		if(m_rowColEditInfo.m_strText != strTemp)
		{
			CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(m_rowColEditInfo.m_rowId)->data.GetAt(m_rowColEditInfo.m_colId)->data;
			CTitleInfo& titleInfo = *m_lnkTitleInfo[m_rowColEditInfo.m_colId];
			CheckMaxMinSet(strTemp,cellInfo.m_maxMinInfo);

			if(m_bUseFixedIdpd)
			{
				titleInfo.DataFilter(strTemp);
			}

			cellInfo.SetStringFormatData(strTemp);
			UpdateRowColumnDisableState(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp,TRUE);
			TellParentSomething(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp);
			TellParentCellChanged(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId);
		}
	}

	if(NULL != m_cbCtrl.GetSafeHwnd() && m_cbCtrl.IsWindowVisible())
	{
		CString tempStr(_T(""));
		tempStr = m_cbCtrl.GetText();
		m_rowColEditInfo.m_strNewText = tempStr;
		m_cbCtrl.ShowWindow(SW_HIDE);
		if(m_rowColEditInfo.m_strText != tempStr)
		{
			m_bCbEndEdit = TRUE;
			m_2LnkCellInfo.GetAt(m_rowColEditInfo.m_rowId)->data.GetAt(m_rowColEditInfo.m_colId)->data.SetStringFormatData(tempStr);
			UpdateRowColumnDisableState(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,tempStr,TRUE);
			TellParentSomething(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,tempStr);
			TellParentCellChanged(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId);
		}
	}

	return;
}

void CMyLstCtrl::EndEDitForFuncCtrlKillFocus(int iCtrlTag)
{
	if(!m_rowColEditInfo.BeValidRowCol())
	{
		return;
	}

	if(iCtrlTag == m_iInputEdTag && NULL != m_edInput.GetSafeHwnd())
	{
		CString strTemp(_T(""));
		strTemp = m_edInput.GetText();
		m_rowColEditInfo.m_strNewText = strTemp;
		if(m_rowColEditInfo.m_strText != strTemp)
		{
			CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(m_rowColEditInfo.m_rowId)->data.GetAt(m_rowColEditInfo.m_colId)->data;
			CTitleInfo& titleInfo = *m_lnkTitleInfo[m_rowColEditInfo.m_colId];
			CheckMaxMinSet(strTemp,cellInfo.m_maxMinInfo);

			if(m_bUseFixedIdpd)
			{
				titleInfo.DataFilter(strTemp);
			}

			cellInfo.SetStringFormatData(strTemp);
			UpdateRowColumnDisableState(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp,TRUE);
			TellParentSomething(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp);
			TellParentCellChanged(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId);
		}
	}
	else if(iCtrlTag == m_iInputCbTag && NULL != m_cbCtrl.GetSafeHwnd())
	{
		CString strTemp(_T(""));
		strTemp = m_cbCtrl.GetText();
		m_rowColEditInfo.m_strNewText = strTemp;
		if(m_rowColEditInfo.m_strText != strTemp)
		{
			m_bCbEndEdit = TRUE;
			m_2LnkCellInfo.GetAt(m_rowColEditInfo.m_rowId)->data.GetAt(m_rowColEditInfo.m_colId)->data.SetStringFormatData(strTemp);
			UpdateRowColumnDisableState(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp,TRUE);
			TellParentSomething(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp);
			TellParentCellChanged(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId);
		}
	}

	Invalidate(FALSE);

	return;
}

void CMyLstCtrl::CbTextChanged()
{
	if(!m_rowColEditInfo.BeValidRowCol())
	{
		return;
	}

	if(NULL != m_cbCtrl.GetSafeHwnd() && m_cbCtrl.IsWindowVisible())
	{
		CString strTemp(_T(""));
		strTemp = m_cbCtrl.GetText();
		m_rowColEditInfo.m_strNewText = strTemp;
		if(m_rowColEditInfo.m_strText != strTemp)
		{
			m_2LnkCellInfo.GetAt(m_rowColEditInfo.m_rowId)->data.GetAt(m_rowColEditInfo.m_colId)->data.SetStringFormatData(strTemp);
			UpdateRowColumnDisableState(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp,TRUE);
			TellParentSomething(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId,strTemp);
			TellParentCellChanged(m_rowColEditInfo.m_rowId,m_rowColEditInfo.m_colId);
		}
	}

	return;
}

void CMyLstCtrl::SetBeShowRowSelectState(BOOL bShowRowSelState,BOOL bRedraw /*= FALSE*/)
{
	m_bShowRowSelState = bShowRowSelState;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeShowRowSelectState()
{
	return m_bShowRowSelState;
}

void CMyLstCtrl::SetBeScrollInsideFrame(BOOL bScrollInsideFrame,BOOL bRedraw /*= FALSE*/)
{
	m_bScrollInsideFrame = bScrollInsideFrame;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetBeScrollInsideFrame()
{
	return m_bScrollInsideFrame;
}

CRect CMyLstCtrl::GetColorSelectDlgRect(CRect rcBase,int iWndW,int iWndH)
{
	ClientToScreen(rcBase);
	CRect rcClrDlg,rcDefault;
	rcClrDlg.left = rcBase.left;
	rcClrDlg.right = rcClrDlg.left + iWndW;
	rcClrDlg.top = rcBase.bottom;
	rcClrDlg.bottom = rcClrDlg.top + iWndH;

	rcDefault = rcClrDlg;
	if(rcClrDlg.bottom > GeneCRDlg.bottom)
	{
		rcClrDlg.top = rcBase.top - iWndH;
		rcClrDlg.bottom = rcClrDlg.top + iWndH;
	}

	if(rcClrDlg.right > GeneCRDlg.right)
	{
		rcClrDlg.left = rcBase.right - iWndW + 1;
		rcClrDlg.right = rcClrDlg.left + iWndW;

		if(rcBase.top - iWndH >= GeneCRDlg.top)	//让窗口始终保持在左上角
		{
			rcClrDlg.top = rcBase.top - iWndH;
			rcClrDlg.bottom = rcClrDlg.top + iWndH;
		}
	}

	return rcClrDlg;
}

void CMyLstCtrl::SetRowBkSelectedColor(COLORREF clrRowBkSel)
{
	m_clrRowBkSel = clrRowBkSel;
}

COLORREF CMyLstCtrl::GetRowBkSelectedColor()
{
	return m_clrRowBkSel;
}

void CMyLstCtrl::SetRowBkUnSelectedColor(COLORREF clrRowBkUnSel)
{
	m_clrRowBkUnSel = clrRowBkUnSel;
}

COLORREF CMyLstCtrl::GetRowBkUnSelectedColor()
{
	return m_clrRowBkUnSel;
}

int CMyLstCtrl::CalculateFitRowHeight()
{
	if(m_colNum < 1)
	{
		return m_pxRowH;
	}
	if(m_rowNum < 1)
	{
		return m_pxRowH;
	}

	if(m_totalRowH > m_rcBody.Height())
	{
		if(m_lnkRowInfo.GetSize() > 0)
		{
			return m_lnkRowInfo[0]->m_rowH;
		}
		else
		{
			return m_pxRowH;
		}
	}
	else
	{
		return m_rcBody.Height() / m_rowNum;
	}
}

int CMyLstCtrl::SetAllRowHeight(int iRowH,BOOL bRedraw /*= FALSE*/)
{
	if(iRowH < 1)
	{
		return -1;
	}

	m_totalRowH = 0;
	for(int i = 0;i < m_rowNum;++i)
	{
		m_lnkRowInfo[i]->m_rowH = iRowH;
		m_totalRowH += iRowH;
	}

	CalcScrollPos();
	CalcRowColToShow();
	CalcRowColToShowOnNoRowData();
	RenewVertScrollPos();
	RenewHoriScrollPos();

	m_pxRowH = iRowH;

	return 0;
}

int CMyLstCtrl::BottomBlankSetAutoFitHeight(BOOL bLastRowUseRemainder /*= TRUE*/,BOOL bRedraw /*= FALSE*/)
{
	if(m_colNum < 1)
	{
		return -1;
	}
	if(m_rowNum < 1)
	{
		return -2;
	}

	if(m_totalRowH > m_rcBody.Height())
	{
		return -3;
	}

	int iDivide = m_rcBody.Height() / m_rowNum;
	int iRemainder = m_rcBody.Height() % m_rowNum;
	
	for(int i = 0;i < m_rowNum;++i)
	{
		m_lnkRowInfo[i]->m_rowH = iDivide;
	}

	if(bLastRowUseRemainder)
	{
		m_lnkRowInfo[m_rowNum - 1]->m_rowH += iRemainder;
	}

	CalcScrollPos();
	CalcRowColToShow();
	CalcRowColToShowOnNoRowData();
	RenewVertScrollPos();
	RenewHoriScrollPos();

	m_pxRowH = iDivide;

	return 0;
}

int CMyLstCtrl::SetFuncEditCtrlFont(CString strFtName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/)
{
	int iRet = -1;
	if(NULL != m_cbCtrl.GetSafeHwnd())
	{
		m_cbCtrl.SetMyFont(strFtName,nHeight,nWeight,fontUnitType);
		m_cbCtrl.m_selWnd.SetMyFont(strFtName);
		m_cbCtrl.TextParamChanged();
		iRet = 1;
	}

	if(NULL != m_edInput.GetSafeHwnd())
	{
		m_edInput.SetMyFont(strFtName,nHeight,nWeight,fontUnitType);
		m_edInput.TextParamChanged();
		iRet = 2;
	}

	return iRet;
}

int CMyLstCtrl::RefreshSerialNumber(int iColId,int iIndex,BOOL bRedraw /*= FALSE*/)
{
	if(iColId < 0)
	{
		return -1;
	}

	if(iColId >= m_colNum)
	{
		return -2;
	}

	if(iIndex < 0)
	{
		return -3;
	}

	CString strTemp(_T(""));
	for(int i = iIndex;i < m_rowNum;++i)
	{
		strTemp.Format(_T("%d"),i + 1);
		SetItemText(i,iColId,strTemp);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

BOOL CMyLstCtrl::IsCbEndEdit()
{
	return m_bCbEndEdit;
}

void CMyLstCtrl::SetCbEndEditState(BOOL bData)
{
	m_bCbEndEdit = bData;
}

int CMyLstCtrl::SetCbSelectItem(int iRowId,int iColId,CLinkList<CString>& lnkStr)
{
	if(iRowId < 0)
	{
		return -1;
	}
	if(iRowId >= m_rowNum)
	{
		return -2;
	}
	if(iColId < 0)
	{
		return -3;
	}
	if(iColId >= m_colNum)
	{
		return -4;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo.GetAt(iRowId)->data.GetAt(iColId)->data;
	cellInfo.m_lnkCbStr = lnkStr;
	if(m_cbCtrl.IsWindowVisible())
	{
		//m_cbCtrl.
	}
	
}

int CMyLstCtrl::RenewHoriScrollPos()
{
	//更新水平滚动条当前位置
	if(m_horiScroll.m_bScrollNeedShow)
	{
		m_horiScroll.m_curPixelPos = 0;
		for(int i = 0;i < m_colIdStartShow;++i)
		{
			m_horiScroll.m_curPixelPos += m_lnkTitleInfo[i]->m_colWidth;
		}
		if(-1 != m_colIdStartShow)
		{
			m_horiScroll.m_curPixelPos += m_ssColLeftIndent;
		}

		m_horiScroll.m_pixelTotal = m_totalColW;
		m_horiScroll.m_pixelForShow = m_rcBody.Width();
		m_horiScroll.CalcSliderPos();
	}
	else
	{
		m_horiScroll.m_curPixelPos = 0;
	}

	return 0;
}

int CMyLstCtrl::RenewVertScrollPos()
{
	//更新垂直滚动条当前位置
	if(m_vertScroll.m_bScrollNeedShow)
	{
		m_vertScroll.m_curPixelPos = 0;
		for(int i = 0;i < m_rowIdStartShow;++i)
		{
			m_vertScroll.m_curPixelPos += m_lnkRowInfo[i]->m_rowH;
		}
		if(-1 != m_rowIdStartShow)
		{
			m_vertScroll.m_curPixelPos += m_ssRowTopIndent;
		}

		m_vertScroll.m_pixelTotal = m_totalRowH;
		m_vertScroll.m_pixelForShow = m_rcBody.Height();
		m_vertScroll.CalcSliderPos();
	}
	else
	{
		m_vertScroll.m_curPixelPos = 0;
	}

	return 0;
}

void CMyLstCtrl::CheckMaxMinSet(CString& strTemp,CMaxMinInfo& maxMinInfo)
{
	double dData = 0;

	if(maxMinInfo.GetBeSetMinValue())
	{
		if(maxMinInfo.GetBeOnlyFloatNumber())
		{
			dData = _ttof(strTemp);
		}
		else if(maxMinInfo.GetBeOnlyIntergerNumber())
		{
			dData = _ttoi(strTemp);
		}
		else
		{
			dData = _ttof(strTemp);
		}

		if(dData < maxMinInfo.GetMinValue())
		{
			dData = maxMinInfo.GetMinValue();
			if(maxMinInfo.GetBeOnlyFloatNumber())
			{
				strTemp.Format(_T("%.2lf"),dData);
			}
			else if(maxMinInfo.GetBeOnlyIntergerNumber())
			{
				int iData = dData;
				strTemp.Format(_T("%d"),iData);
			}
			else
			{
				strTemp.Format(_T("%.2lf"),dData);
			}
		}
	}

	return;
}

void CMyLstCtrl::SetBeTellParentRowSelChange(BOOL beTellParentRowSelChange)
{
	m_bTellParentRowSelChange = beTellParentRowSelChange;
	return;
}

BOOL CMyLstCtrl::GetBeTellParentRowSelChange()
{
	return m_bTellParentRowSelChange;
}

void CMyLstCtrl::SetFourAngleBkColor(COLORREF clr,BOOL bRedraw /*= FALSE*/)
{
	m_clrFourAngleBk = clr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMyLstCtrl::GetFourAngleBkColor()
{
	return m_clrFourAngleBk;
}

void CMyLstCtrl::SetLargeCheckIconUseState(BOOL bData,BOOL bRedraw /*= FALSE*/)
{
	m_bUseLargeChkIcon = bData;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMyLstCtrl::GetLargeCheckIconUseState()
{
	return m_bUseLargeChkIcon;
}

void CMyLstCtrl::InitFontHeader(int lfHeight)
{
	m_logFontHeader.lfHeight = lfHeight;
	m_logFontHeader.lfWidth = 0;
	m_logFontHeader.lfEscapement = 0;
	m_logFontHeader.lfOrientation = 400;
	m_logFontHeader.lfWeight = FW_NORMAL;
	m_logFontHeader.lfItalic = FALSE;
	m_logFontHeader.lfUnderline = FALSE;
	m_logFontHeader.lfStrikeOut = 0;
	m_logFontHeader.lfCharSet = ANSI_CHARSET;
	m_logFontHeader.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontHeader.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logFontHeader.lfQuality = PROOF_QUALITY;
	m_logFontHeader.lfPitchAndFamily = VARIABLE_PITCH | FF_SCRIPT;
	_tcscpy(m_logFontHeader.lfFaceName,  _T("微软雅黑"));

	m_fontSetHeader.CreateFontIndirect(&m_logFontHeader);  
}

void CMyLstCtrl::SetMyHeaderFont(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/) //nHeight单位为 磅，即1/72英寸
{
	if(TYPE_POUND == fontUnitType)
	{
		m_logFontHeader.lfHeight = (double(nHeight * fontSetDesDpi))/ 72;
	}
	else
	{
		m_logFontHeader.lfHeight = nHeight;
	}

	_tcscpy(m_logFontHeader.lfFaceName, fontName);
	m_logFontHeader.lfWeight = nWeight;
	m_fontSetHeader.DeleteObject();
	m_fontSetHeader.CreateFontIndirect(&m_logFontHeader);
}

void CMyLstCtrl::SetMyHeaderFont() 
{
	m_fontSetHeader.DeleteObject();
	m_fontSetHeader.CreateFontIndirect(&m_logFontHeader);
}

void CMyLstCtrl::InitFontTitle(int lfHeight)
{
	m_logFontTitle.lfHeight = lfHeight;
	m_logFontTitle.lfWidth = 0;
	m_logFontTitle.lfEscapement = 0;
	m_logFontTitle.lfOrientation = 400;
	m_logFontTitle.lfWeight = FW_NORMAL;
	m_logFontTitle.lfItalic = FALSE;
	m_logFontTitle.lfUnderline = FALSE;
	m_logFontTitle.lfStrikeOut = 0;
	m_logFontTitle.lfCharSet = ANSI_CHARSET;
	m_logFontTitle.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontTitle.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logFontTitle.lfQuality = PROOF_QUALITY;
	m_logFontTitle.lfPitchAndFamily = VARIABLE_PITCH | FF_SCRIPT;
	_tcscpy(m_logFontTitle.lfFaceName, _T("微软雅黑"));

	m_fontSetTitle.CreateFontIndirect(&m_logFont);  
}

void CMyLstCtrl::SetMyTitleFont(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/)
{
	if(TYPE_POUND == fontUnitType)
	{
		m_logFontTitle.lfHeight = (double(nHeight * fontSetDesDpi))/ 72;
	}
	else
	{
		m_logFontTitle.lfHeight = nHeight;
	}

	_tcscpy(m_logFontTitle.lfFaceName,fontName);
	m_logFontTitle.lfWeight = nWeight;
	m_fontSetTitle.DeleteObject();
	m_fontSetTitle.CreateFontIndirect(&m_logFontTitle);
}

void CMyLstCtrl::SetMyTitleFont()
{
	m_fontSetTitle.DeleteObject();
	m_fontSetTitle.CreateFontIndirect(&m_logFontTitle);
}


int CMyLstCtrl::SetCellSelCbRowDelete(int rowId, int colId, BOOL bEnableDelete)
{
	if(rowId < 0)
	{
		return -1;
	}

	if(rowId >= m_rowNum)
	{
		return -2;
	}

	if(colId < 0)
	{
		return -3;
	}

	if(colId >= m_colNum)
	{
		return -4;
	}

	CCellInfo& cellInfo = m_2LnkCellInfo[rowId]->GetAt(colId)->data;
	cellInfo.m_bCbDeleteRow = bEnableDelete;
	return 0;
}



LRESULT CMyLstCtrl::OnMsgDeleteComboRowString(WPARAM wParam,LPARAM lParam)
{
	GetParent()->SendMessage(WM_DELETE_COMBO_ROW_STRING, m_rowColEditInfo.m_colId, wParam);
	return 0;
}


// PcrSegmentExpWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MySamPlateDlg.h"
#include "LanguageInfo.h"
#include "Resource.h"

// CMySamPalteDlg

map<eSampleType,CString> CMySamPalteDlg::m_mapSamTypeToTx;
map<CString,eSampleType> CMySamPalteDlg::m_mapTxToSamType;
map<eSampleType,CHoleColorInfo> CMySamPalteDlg::m_mapSamTypeToHoleClrInfo;
map<eSampleType,CString> CMySamPalteDlg::m_mapSamTypeToAbbr;

IMPLEMENT_DYNAMIC(CMySamPalteDlg, CDialogEx)
CMySamPalteDlg::CMySamPalteDlg()
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

	m_pxColW = 110;	
	m_pxRowH = 80;	
	m_pxRowHeaderW = 50;
	m_pxColHeaderH = 46;
	m_bUseFixedHW = TRUE;
	m_clrBk = RGB(0XFF,0XFF,0XFF);
	m_clrHeaderBk = Color(0XF1,0XF4,0XF9);
	m_clrHeaderSelectedBk = Color(0XBB,0XCA,0XE3);
	
	m_clrBodyBk = RGB(0XFF,0XFF,0XFF);
	m_clrFrame = RGB(0XD8,0XD8,0XD8);
	m_clrRowColLine = RGB(197,197,197);
	m_clrHeaderText = RGB(0X39,0X45,0X65);
	m_bGradientHeader = FALSE;
	m_clrGraHeadStart = Color(0XAA,0XAB,0XAC);
	m_clrGraHeadEnd = Color(0XBA,0XBB,0XBC);
	m_bEditable = TRUE;

	m_totalColW = 60;
	m_totalRowH = 30;
	m_cdcW = 60;
	m_cdcH = 160;

	m_horiScroll.m_scrollH = 30;
	m_vertScroll.m_scrollW = 30;

	m_horiScroll.m_eScrollType = EST_HORI;
	m_vertScroll.m_eScrollType = EST_VERT;

	m_eScrollHVCM = ESHVCM_HORI;
	m_bShowFrameLine = FALSE;
	m_bShowRowColLine = FALSE;
	m_lnkHistoryDotPosInfo.SetLimitMaxParam(TRUE,100);
	
	m_rowIdSeparatorStart = -1;
	m_rowIdSeparatorEnd = -1;
	m_esSeparatorBottomIndent = 0;
	m_bSingleSelect = FALSE;

	m_clrHeadBodySepa = RGB(197,197,197);
	m_bTellParentSiteSelChange = FALSE;

	InitFontHeader(16);
	InitFontTitle(16);
	InitFontAbbr(12);

	m_pTubeInfo = NULL;
	m_pPlateInfo = NULL;
	m_strTotalTitle = _T("All");
	m_iTotalSiteNum = 0;
	m_bAllSelected = FALSE;
	m_bDraggableStart = FALSE;
	m_bDragMove = FALSE;
	m_iRectW = 50;
	m_iRectH = 40;
	m_bShowSplitLine = FALSE;

	m_clrFourAngleBk = CLR_SOFT_LIGHT;
	m_bRoundRectStyle = TRUE;
	m_iRoundAngleSize = 8;
	m_clrIndepent = Color(0XF2,0XF5,0XFA);

	m_bShowTopTx = FALSE;

#ifdef SAMPLATE_16SITE
	m_bShowBottomTx = TRUE;
#else
	m_bShowBottomTx = FALSE;
#endif

	m_iLeftIndent = 20;
	m_iRightIndent = 24;
	m_iTopIndent = 15;
	m_iBottomIndent = 17;
	m_iHoriInterval = 8;
	m_iVertInterval = 18;

	m_bShieldDeleteKey = FALSE;
	m_bParentProcessDeleteKey = FALSE;
	m_bLeftBtDown = FALSE;

	m_iRoundAngleLT = 8;
	m_iRoundAngleRT = 8;
	m_iRoundAngleRB = 8;
	m_iRoundAngleLB = 8;

	m_bShowSiteId = TRUE;
	m_bShowLastChanBlock = TRUE;
}

CMySamPalteDlg::~CMySamPalteDlg()
{
	MyMulitDeletePtr(&m_pTubeInfo);
	MyMulitDeletePtr(&m_pPlateInfo);

	m_fontSetHeader.DeleteObject(); 
	m_fontSetTitle.DeleteObject();
	m_fontAbbr.DeleteObject();
}

BEGIN_MESSAGE_MAP(CMySamPalteDlg, CDialogEx)
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
   ON_MESSAGE(WM_EDITENTERPUSHED,&CMySamPalteDlg::OnMsgEditEnterPushed)
END_MESSAGE_MAP()

void CMySamPalteDlg::OnPaint()
{
#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	CPaintDC dc(this); 

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	CFont* pOldFont = memDC.SelectObject(&m_fontSet);
	memDC.SetBkMode(TRANSPARENT);
	memDC.SetTextColor(m_txClr1);

	//���Ʊ߿�
	if(m_bRoundRectStyle)
	{
		Color clrFrame = 0;
		clrFrame.SetFromCOLORREF(m_clrFrame);
		CRect rcTemp(0,0,m_rcBody.right + m_pxFrame,m_rcBody.bottom + m_pxFrame);
		memDC.FillSolidRect(rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),m_clrFourAngleBk);
		DrawRoundRectangleNew(&memDC,rcTemp.left,rcTemp.top,rcTemp.Width() - m_pxFrame,rcTemp.Height() - m_pxFrame,m_iRoundAngleSize,m_iRoundAngleSize,0,0,TRUE, clrFrame,m_pxFrame,TRUE,m_clrIndepent,1);
	}
	else
	{
		if(m_bShowFrameLine)
		{
			CPen penFrame;
			penFrame.CreatePen(PS_SOLID,m_pxFrame,m_clrFrame);
			CPen* pOldPen = memDC.SelectObject(&penFrame);
			memDC.Rectangle(&rcClient);
			memDC.SelectObject(pOldPen);
		}
	}

	DrawTotalHeader(&memDC);
	DrawRowHeader(&memDC);
	DrawColHeader(&memDC);
	DrawBody(&memDC);
	DrawHoriScroll(&memDC);
	DrawVertScroll(&memDC);

	//ȥ���ײ��߿�����Ч��
	if(m_bRoundRectStyle)
	{
		CPoint pt;
		CPen penBottomFrame;
		
		//�������һ���б���ѡ�����ȷ��������ɫ
		penBottomFrame.CreatePen(PS_SOLID,m_pxFrame,RGB(m_clrHeaderBk.GetR(),m_clrHeaderBk.GetG(),m_clrHeaderBk.GetB()));
		CPen* pOldPen = memDC.SelectObject(&penBottomFrame);

		pt.x = m_pxFrame;
		pt.y = m_rcLst.bottom - m_pxFrame;
		memDC.MoveTo(pt);
		pt.x = m_rcRowHeader.right;
		memDC.LineTo(pt);
		penBottomFrame.DeleteObject();

		penBottomFrame.CreatePen(PS_SOLID,m_pxFrame,m_clrBodyBk);

		pt.x = m_rcRowHeader.right + 1;
		pt.y = m_rcLst.bottom - m_pxFrame;
		memDC.MoveTo(pt);
		pt.x = m_rcBody.right;
		memDC.LineTo(pt);

		memDC.SelectObject(pOldPen);
		penBottomFrame.DeleteObject();
	}

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);   

	memDC.SelectObject(pOldBitmap);
	memDC.SelectObject(pOldFont);
	bitMap.DeleteObject();
	memDC.DeleteDC();

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return;
}

int CMySamPalteDlg::DrawTotalHeader(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	CDC memDcHeader;
	memDcHeader.CreateCompatibleDC(pDC);

	CRect tempRcHeader(0,0,m_rcRowHeader.Width(),m_rcColHeader.Height());

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC,m_rcRowHeader.Width(),m_rcColHeader.Height());
	CBitmap* pOldBitmap = memDcHeader.SelectObject(&bitMap);
	memDcHeader.SetBkMode(TRANSPARENT);

	if(m_bGradientHeader)
	{
		Graphics graphics(memDcHeader.m_hDC);
		LinearGradientBrush linGrBrush(RectF(0,0,m_rcRowHeader.Width(),m_rcColHeader.Height()),m_clrGraHeadStart,m_clrGraHeadEnd,LinearGradientModeVertical);
		graphics.FillRectangle(&linGrBrush,0,0,m_rcRowHeader.Width(),m_rcColHeader.Height());
		graphics.ReleaseHDC(memDcHeader.m_hDC);
	}
	else
	{
		Color clrBk = m_clrHeaderBk;
		if(m_bAllSelected)
		{
			clrBk = m_clrHeaderSelectedBk;
		}
		memDcHeader.FillSolidRect(&tempRcHeader,RGB(clrBk.GetR(),clrBk.GetG(),clrBk.GetB()));
	}

	memDcHeader.SetTextColor(m_clrHeaderText);
	CFont* pOldFont = memDcHeader.SelectObject(&m_fontSetHeader);
	memDcHeader.DrawText(m_strTotalTitle,&tempRcHeader,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	if(m_bShowSplitLine)
	{
		//���Ʒָ���
		CPen penLine;
		penLine.CreatePen(PS_SOLID,2,m_clrHeadBodySepa);
		CPen* pOldPen = memDcHeader.SelectObject(&penLine);

		POINT pt;
		pt.x = 1;
		pt.y = m_pxColHeaderH - 1;
		memDcHeader.MoveTo(pt);
		pt.x = m_pxRowHeaderW - 1;
		memDcHeader.LineTo(pt);

		pt.x = m_pxRowHeaderW - 1;
		pt.y = 1;
		memDcHeader.MoveTo(pt);
		pt.y = m_pxColHeaderH - 1;
		memDcHeader.LineTo(pt);

		memDcHeader.SelectObject(pOldPen);
	}

	if(m_bRoundRectStyle)
	{
		//���ʹ���ȶ���
		memDcHeader.TransparentBlt(0,0,tempRcHeader.Width(),tempRcHeader.Height(),pDC,m_rcTotalHeader.left,m_rcTotalHeader.top,m_rcTotalHeader.Width(),m_rcTotalHeader.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
		pDC->TransparentBlt(m_rcTotalHeader.left,m_rcTotalHeader.top,m_rcTotalHeader.Width(),m_rcTotalHeader.Height(),&memDcHeader,0,0,tempRcHeader.Width(),tempRcHeader.Height(),m_clrFourAngleBk);
	}
	else
	{
		pDC->BitBlt(m_rcTotalHeader.left,m_rcTotalHeader.top,m_rcTotalHeader.Width(),m_rcTotalHeader.Height(),&memDcHeader,0,0,SRCCOPY); 
	} 

	memDcHeader.SelectObject(pOldFont);
	memDcHeader.SelectObject(pOldBitmap);

	memDcHeader.DeleteDC();
	bitMap.DeleteObject();

	return 0;
}

int CMySamPalteDlg::DrawRowHeader(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	if(m_rowNum < 1)
	{
		return -2;
	}

	if(m_lnkRowInfo.GetSize() != m_rowNum)
	{
		return -3;
	}

	CDC memDcHeader;
	memDcHeader.CreateCompatibleDC(pDC);

	CRect tempRcHeader(0,0,m_rcRowHeader.Width(),m_cdcH);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC,m_rcRowHeader.Width(),m_cdcH);
	CBitmap* pOldBitmap = memDcHeader.SelectObject(&bitMap);
	memDcHeader.SetBkMode(TRANSPARENT);

	if(m_bGradientHeader)
	{
		Graphics graphics(memDcHeader.m_hDC);
		LinearGradientBrush linGrBrush(RectF(0,0,m_rcRowHeader.Width(),m_cdcH),m_clrGraHeadStart,m_clrGraHeadEnd,LinearGradientModeVertical);
		graphics.FillRectangle(&linGrBrush,0,0,m_rcRowHeader.Width(),m_cdcH);
		graphics.ReleaseHDC(memDcHeader.m_hDC);
	}
	else
	{
		memDcHeader.FillSolidRect(&tempRcHeader,RGB(m_clrHeaderBk.GetR(),m_clrHeaderBk.GetG(),m_clrHeaderBk.GetB()));
	}

	memDcHeader.SetTextColor(m_clrHeaderText);

	CRect tempRc;
	tempRc.left = 0;
	tempRc.right = m_pxRowHeaderW;
	CFont* pOldFont = memDcHeader.SelectObject(&m_fontSetHeader);
	CString strTag(_T(""));
	for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
	{
		tempRc.top = GetRowStartDrawInMemoryVertPixel(i);
		tempRc.bottom = tempRc.top + m_lnkRowInfo[i]->m_rowH;
		CRowInfo& rowInfo = m_lnkRowInfo.GetAt(i)->data;
		if(rowInfo.m_bSelected)
		{
			memDcHeader.FillSolidRect(&tempRc,RGB(m_clrHeaderSelectedBk.GetR(),m_clrHeaderSelectedBk.GetG(),m_clrHeaderSelectedBk.GetB()));
		}
		else
		{
			memDcHeader.FillSolidRect(&tempRc,RGB(m_clrHeaderBk.GetR(),m_clrHeaderBk.GetG(),m_clrHeaderBk.GetB()));
		}
		GetRowTag(i,strTag);
		memDcHeader.DrawText(strTag,&tempRc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	if(m_bShowSplitLine)
	{
		//����Header��Body�ָ���
		CPen penLine;
		penLine.CreatePen(PS_SOLID,2,m_clrHeadBodySepa);
		CPen* pOldPen = memDcHeader.SelectObject(&penLine);

		POINT pt;
		pt.y = 1;
		pt.x = m_pxRowHeaderW - 1;
		memDcHeader.MoveTo(pt);
		pt.y = m_cdcH - 1;
		memDcHeader.LineTo(pt);

		memDcHeader.SelectObject(pOldPen);
	}

	pDC->BitBlt(m_rcRowHeader.left,m_rcRowHeader.top,m_rcRowHeader.Width(),m_rcRowHeader.Height(),&memDcHeader,0,m_ssRowTopIndent,SRCCOPY);  

	memDcHeader.SelectObject(pOldFont);
	memDcHeader.SelectObject(pOldBitmap);

	memDcHeader.DeleteDC();
	bitMap.DeleteObject();

	return 0;
}

int CMySamPalteDlg::DrawColHeader(CDC* pDC)
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
	memDcHeader.CreateCompatibleDC(pDC);

	CRect tempRcHeader(0,0,m_cdcW,m_rcColHeader.Height());

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC,m_cdcW,m_rcColHeader.Height());
	CBitmap* pOldBitmap = memDcHeader.SelectObject(&bitMap);
	memDcHeader.SetBkMode(TRANSPARENT);

	if(m_bGradientHeader)
	{
		Graphics graphics(memDcHeader.m_hDC);
		LinearGradientBrush linGrBrush(RectF(0,0,m_cdcW,m_pxColHeaderH),m_clrGraHeadStart,m_clrGraHeadEnd,LinearGradientModeVertical);
		graphics.FillRectangle(&linGrBrush,0,0,m_cdcW,m_pxColHeaderH);
		graphics.ReleaseHDC(memDcHeader.m_hDC);
	}
	else
	{
		memDcHeader.FillSolidRect(&tempRcHeader,RGB(m_clrHeaderBk.GetR(),m_clrHeaderBk.GetG(),m_clrHeaderBk.GetB()));
	}

	memDcHeader.SetTextColor(m_clrHeaderText);

	CRect tempRc;
	tempRc.top = 0;
	tempRc.bottom = m_pxColHeaderH;
	CFont* pOldFont = memDcHeader.SelectObject(&m_fontSetHeader);
	CString strTag(_T(""));
	for(int i = m_colIdStartShow;i <= m_colIdEndShow;++i)
	{
		tempRc.left = GetColStartDrawInMemoryHoriPixel(i);
		tempRc.right = tempRc.left + m_lnkTitleInfo[i]->m_colWidth;
		CTitleInfo& titleInfo = m_lnkTitleInfo.GetAt(i)->data;
		if(titleInfo.m_bSelected)
		{
			memDcHeader.FillSolidRect(&tempRc,RGB(m_clrHeaderSelectedBk.GetR(),m_clrHeaderSelectedBk.GetG(),m_clrHeaderSelectedBk.GetB()));
		}
		else
		{
			memDcHeader.FillSolidRect(&tempRc,RGB(m_clrHeaderBk.GetR(),m_clrHeaderBk.GetG(),m_clrHeaderBk.GetB()));
		}

		GetColTag(i,strTag);
		memDcHeader.DrawText(strTag,&tempRc,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}

	if(m_bShowSplitLine)
	{
		//����Header��Body�ָ���
		CPen penLine;
		penLine.CreatePen(PS_SOLID,2,m_clrHeadBodySepa);
		CPen* pOldPen = memDcHeader.SelectObject(&penLine);

		POINT pt;
		pt.x = 1;
		pt.y = m_pxColHeaderH - 1;
		memDcHeader.MoveTo(pt);
		pt.x = m_cdcW - 1;
		memDcHeader.LineTo(pt);

		memDcHeader.SelectObject(pOldPen);
	}

	int iRealW = m_rcColHeader.Width();
	if(m_cdcW < iRealW)
	{
		iRealW = m_cdcW;
	}
	memDcHeader.TransparentBlt(m_ssColLeftIndent,0,iRealW,m_rcColHeader.Height(),pDC,m_rcColHeader.left,m_rcColHeader.top,iRealW,m_rcColHeader.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
	pDC->TransparentBlt(m_rcColHeader.left,m_rcColHeader.top,iRealW,m_rcColHeader.Height(),&memDcHeader,m_ssColLeftIndent,0,iRealW,m_rcColHeader.Height(),m_clrFourAngleBk);

	//pDC->BitBlt(m_rcColHeader.left,m_rcColHeader.top,m_rcColHeader.Width(),m_rcColHeader.Height(),&memDcHeader,m_ssColLeftIndent,0,SRCCOPY);  

	memDcHeader.SelectObject(pOldFont);
	memDcHeader.SelectObject(pOldBitmap);

	memDcHeader.DeleteDC();
	bitMap.DeleteObject();
	
	return 0;
}

int CMySamPalteDlg::DrawBody(CDC* pDC)
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
	int ltempCdcW = m_cdcW,tempCdcH = m_cdcH;
	if(-1 == m_rowIdStartShow || m_cdcW < m_rcBody.Width())
	{
		ltempCdcW = m_rcBody.Width();
	}
	if(-1 == m_rowIdStartShow || m_cdcH < m_rcBody.Height())
	{
		tempCdcH = m_rcBody.Height();
	}
	bitMap.CreateCompatibleBitmap(pDC,ltempCdcW,tempCdcH);
	CBitmap* pOldBitmap = memDcBody.SelectObject(&bitMap);

	memDcBody.FillSolidRect(0,0,ltempCdcW,tempCdcH,m_clrBodyBk);

	CRect tempRc;
	tempRc.top = m_pxColHeaderH;
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

	CString strTemp(_T(""));

	if(-1 == m_rowIdStartShow || m_cdcH < m_rcBody.Height())
	{
		if(m_bShowRowColLine)
		{
			//����ˮƽ�ָ���
			for(int i = m_rowIdSeparatorStart;i <= m_rowIdSeparatorEnd;++i)
			{
				POINT pt;
				pt.x = 0;
				pt.y = (i + 1) * m_pxRowH - m_pxRowColLine;
				memDcBody.MoveTo(pt);
				pt.x = ltempCdcW;
				memDcBody.LineTo(pt);
			}

			//���ƴ�ֱ�ָ���
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
		int iFrameLineW = 2,iBottomTxRgnH = 30 * m_dVertRatio,iAbbrH = 30 * m_dVertRatio;
		int iChanCount = 0; 
		int iClrWH = 12 * m_dHoriRatio,iClrHoriInterval = 4 * m_dHoriRatio;
		COLORREF clrChan = 0;
		CString strChanName(_T(""));

		if(m_bShowBottomTx)
		{
			iClrHoriInterval = 12 * m_dHoriRatio;
		}

		for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
		{
			CRowInfo& rowInfo = *m_lnkRowInfo[i];

			tempRc.top = GetRowStartDrawInMemoryVertPixel(i);
			tempRc.left = 0;
			tempRc.right = ltempCdcW;
			tempRc.bottom = tempRc.top + rowInfo.m_rowH;
			tempRc.DeflateRect(0,1,0,1);
			for(int j = m_colIdStartShow;j <= m_colIdEndShow;++j)
			{
				CTitleInfo& titleInfo = m_lnkTitleInfo.GetAt(j)->data;
				CCellInfo& cellInfo = m_2LnkCellInfo[i]->GetAt(j)->data;
				tempRc.left = GetColStartDrawInMemoryHoriPixel(j);
				tempRc.right = tempRc.left + m_lnkTitleInfo[j]->m_colWidth;

#ifdef SAMPLATE_16SITE
				tempRc.top = GetRowStartDrawInMemoryVertPixel(i);
				tempRc.bottom = tempRc.top + rowInfo.m_rowH;
				tempRc.left += m_iHoriInterval;
				tempRc.right -= m_iHoriInterval;
				tempRc.top += m_iVertInterval;
				tempRc.bottom -= m_iVertInterval;
#else
#ifdef PS384INSTRUMENT
				// For 12x32 display style, draw slimmer wells with proportional inset
				tempRc.top = GetRowStartDrawInMemoryVertPixel(i);
				tempRc.bottom = tempRc.top + rowInfo.m_rowH;
				CRect rcShrink = tempRc;
				// Quarter current size: make final rect ~ 1/12 of original cell (1/4 of current 1/3)
				int defX = (rcShrink.Width() * 11) / 24;   // newW = W - 2*defX ≈ W/12
				int defY = (rcShrink.Height() * 11) / 24;  // newH = H - 2*defY ≈ H/12
				if (defX < 1) defX = 1;
				if (defY < 1) defY = 1;
				rcShrink.DeflateRect(defX, defY);
				tempRc = rcShrink;
#else
				tempRc.left += (tempRc.Width() - m_iRectW) / 2;
				tempRc.right = tempRc.left + m_iRectW;
				tempRc.top += (tempRc.Height() - m_iRectH) / 2;
				tempRc.bottom = tempRc.top + m_iRectH;
#endif
#endif

				CRect rcTop,rcBottom,rcChanClr,rcChanText,rcTypeAbbr;
				int iIndex = i * m_colNum + j;
				iChanCount = m_pTubeInfo[iIndex].GetChannelNum();
				if(!m_bShowLastChanBlock)
				{
					iChanCount -= 1;
				}
				
				if(m_bShowTopTx)
				{
					rcTop.left = tempRc.left;
					rcTop.right = tempRc.right;
					rcTop.top = tempRc.top;
					rcTop.bottom = rcTop.top + m_iRectH / 3;
				}
				
				if(m_bShowBottomTx)
				{
					rcBottom.left = tempRc.left;
					rcBottom.right = tempRc.right;
					rcBottom.top = tempRc.bottom - iBottomTxRgnH;
					rcBottom.bottom = rcBottom.top + iBottomTxRgnH;
				}

#ifdef SAMPLATE_16SITE
				rcChanClr.left = tempRc.left + 6 * m_dHoriRatio;
				rcChanClr.right = rcChanClr.left + iClrWH;
				rcChanClr.top = tempRc.top + 10 * m_dVertRatio;
				rcChanClr.bottom = rcChanClr.top + iClrWH;

				rcTypeAbbr.left = tempRc.left + 6 * m_dHoriRatio;
				rcTypeAbbr.right = tempRc.right;
				rcTypeAbbr.top = tempRc.bottom - iBottomTxRgnH - iAbbrH;
				rcTypeAbbr.bottom = rcTypeAbbr.top + iAbbrH;
#else
#ifdef PS384INSTRUMENT
				// Adjust channel color and type abbr regions for slimmer shape
				if(iChanCount > 0)
				{
					iClrWH = (tempRc.Width() - iClrHoriInterval * (iChanCount + 1)) / iChanCount;
					if (iClrWH < 1) iClrWH = 1;
				}
				rcChanClr.left = tempRc.left + int(2 * m_dHoriRatio);
				rcChanClr.right = rcChanClr.left + iClrWH;
				rcChanClr.top = tempRc.top + int(2 * m_dVertRatio);
				rcChanClr.bottom = rcChanClr.top + iClrWH;

				iAbbrH = 16 * m_dVertRatio;
				rcTypeAbbr = tempRc;
				rcTypeAbbr.DeflateRect(rcTypeAbbr.Width()/10, rcTypeAbbr.Height()/6, 0, 0);
#else
				if(iChanCount > 0)
				{
					iClrWH = (tempRc.Width() - iClrHoriInterval * (iChanCount + 1)) / iChanCount;
				}
				rcChanClr.left = tempRc.left + iClrHoriInterval;
				rcChanClr.right = rcChanClr.left + iClrWH;
				rcChanClr.top = tempRc.top + 4 * m_dVertRatio;
				rcChanClr.bottom = rcChanClr.top + iClrWH;

				iAbbrH = 16 * m_dVertRatio;
				rcTypeAbbr.left = tempRc.left + 5 * m_dHoriRatio;
				rcTypeAbbr.right = tempRc.right;
				rcTypeAbbr.top = tempRc.bottom - iAbbrH;
				if(m_bShowBottomTx)
				{
					rcTypeAbbr.top -= iBottomTxRgnH;
				}
				rcTypeAbbr.bottom = rcTypeAbbr.top + iAbbrH;
#endif
#endif

				CHoleColorInfo holeClrInfo;
				GetHoleColorInfo(i,j,holeClrInfo);
				
				if(m_pTubeInfo[iIndex].GetBeNoneSampleType() && !m_pPlateInfo[iIndex].m_bSelected)
				{
					iFrameLineW = 2;
				}
				else
				{
					iFrameLineW = 2;
				}
				DrawRoundRectangleNew(&memDcBody,tempRc.left,tempRc.top,tempRc.Width(),tempRc.Height(),m_iRoundAngleLT,m_iRoundAngleRT,m_iRoundAngleRB,m_iRoundAngleLB,TRUE,holeClrInfo.m_clrFrame,iFrameLineW,m_pPlateInfo[i * m_colNum + j].m_bSelected,holeClrInfo.m_clrFill);

				COLORREF clrTemp = holeClrInfo.m_clrTxUnSelected;
				if(m_pPlateInfo[iIndex].m_bSelected)
				{
					clrTemp = holeClrInfo.m_clrTxSelected;
				}

				if(m_bShowSiteId)
				{
					memDcBody.SetTextColor(clrTemp);
					strTemp.Format(_T("%d"),iIndex + 1);
					memDcBody.DrawText(strTemp,&tempRc,DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				}

				if(m_bShowTopTx)
				{
					memDcBody.DrawText(m_pTubeInfo[iIndex].GetSampleName(),&rcTop,DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				}
				if(m_bShowBottomTx)
				{
					memDcBody.DrawText(m_pTubeInfo[iIndex].GetSampleID(),&rcBottom,DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				}

#ifdef SAMPLATE_16SITE
				COLORREF clrOldText2 = memDcBody.SetTextColor(RGB(0X39,0X45,0X65));
				memDcBody.DrawText(GetSampleTypeAbbr(m_pTubeInfo[iIndex].GetSampleType()),&rcTypeAbbr,DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				memDcBody.SetTextColor(clrOldText2);

				//����ͨ����ɫ��
				for(int k = 0;k < iChanCount;++k)
				{
					if(m_pTubeInfo[iIndex].GetSampleChannelSelectState(k))
					{
						COLORREF clrOldText = memDcBody.SetTextColor(RGB(0X39,0X45,0X65));
						strChanName = m_pTubeInfo[iIndex].GetDyeName(k);
						clrChan = m_pTubeInfo[iIndex].GetSampleDyeTargetColor(k);
						//memDcBody.FillSolidRect(&rcChanClr,clrChan);

						Color clrFill = 0;
						clrFill.SetFromCOLORREF(clrChan);
						DrawRoundRectangleNew(&memDcBody,rcChanClr.left,rcChanClr.top,rcChanClr.Width(),rcChanClr.Height(),2,2,2,2,FALSE,Color(0,0,0),1,TRUE,clrFill);

						rcChanText = rcChanClr;
						rcChanText.left = rcChanText.right + 2;
						rcChanText.right = tempRc.right;
						rcChanText.top -= 5 * m_dVertRatio;
						rcChanText.bottom += 5 * m_dVertRatio;
						memDcBody.DrawText(strChanName,&rcChanText,DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
						memDcBody.SetTextColor(clrOldText);
					}
					rcChanClr.top = rcChanClr.bottom + m_iRectH / 6;
					rcChanClr.bottom = rcChanClr.top + iClrWH;
				}
#else
				CFont* pOldFontTemp = memDcBody.SelectObject(&m_fontAbbr);

				COLORREF clrOldText2 = memDcBody.SetTextColor(RGB(0X39,0X45,0X65));
				memDcBody.DrawText(GetSampleTypeAbbr(m_pTubeInfo[iIndex].GetSampleType()),&rcTypeAbbr,DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
				memDcBody.SetTextColor(clrOldText2);

				memDcBody.SelectObject(pOldFontTemp);

#ifndef SHIELD_COLOR_BLOCK
				//����ͨ����ɫ��
				for(int k = 0;k < iChanCount;++k)
				{
					if(m_pTubeInfo[iIndex].GetSampleChannelSelectState(k))
					{
						COLORREF clrOldText = memDcBody.SetTextColor(RGB(0X39,0X45,0X65));
						strChanName = m_pTubeInfo[iIndex].GetDyeName(k);
						clrChan = m_pTubeInfo[iIndex].GetSampleDyeTargetColor(k);
						memDcBody.FillSolidRect(&rcChanClr,clrChan);

						/*Color clrFill = 0;
						clrFill.SetFromCOLORREF(clrChan);
						DrawRoundRectangleNew(&memDcBody,rcChanClr.left,rcChanClr.top,rcChanClr.Width(),rcChanClr.Height(),2,2,2,2,FALSE,Color(0,0,0),1,TRUE,clrFill);*/

						/*rcChanText = rcChanClr;
						rcChanText.left = rcChanText.right + 2;
						rcChanText.right = tempRc.right;
						rcChanText.top -= 5 * m_dVertRatio;
						rcChanText.bottom += 5 * m_dVertRatio;
						memDcBody.DrawText(strChanName,&rcChanText,DT_LEFT | DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS);
						memDcBody.SetTextColor(clrOldText);*/
					}
					rcChanClr.left = rcChanClr.right + iClrHoriInterval;
					rcChanClr.right = rcChanClr.left + iClrWH;
				}
#endif

#endif
			}
		}
		 
		m_bShowRowColLine = FALSE;
		if(m_bShowRowColLine)
		{
			//����ˮƽ�ָ���
			for(int i = m_rowIdStartShow;i <= m_rowIdEndShow;++i)
			{
				if(i != m_rowIdEndShow)
				{
					POINT pt;
					pt.x = 0;
					pt.y = GetRowStartDrawInMemoryVertPixel(i + 1) - m_pxRowColLine;
					memDcBody.MoveTo(pt);
					pt.x = ltempCdcW;
					memDcBody.LineTo(pt);
				}
				else if(m_cdcH < m_rcBody.Height())
				{
					POINT pt;
					pt.x = 0;
					pt.y = GetRowStartDrawInMemoryVertPixel(i + 1) - m_pxRowColLine;
					memDcBody.MoveTo(pt);
					pt.x = ltempCdcW;
					memDcBody.LineTo(pt);
				}
			}

			//���ƴ�ֱ�ָ���
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
	pDC->BitBlt(m_rcBody.left,m_rcBody.top,m_rcBody.Width(),m_rcBody.Height(),&memDcBody,m_ssColLeftIndent,m_ssRowTopIndent,SRCCOPY);   

	memDcBody.SelectObject(&pOldPen);
	memDcBody.SelectObject(pOldFont);
	memDcBody.SelectObject(pOldBitmap);

	memDcBody.DeleteDC();
	bitMap.DeleteObject();

	return 0;
}

int CMySamPalteDlg::DrawHoriScroll(CDC* pDC)
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

	//���������ͷ
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

	//���������ͷ
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

int CMySamPalteDlg::DrawVertScroll(CDC* pDC)
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

	//���ƶ����ͷ
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
	//���Ƶ����ͷ
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

BOOL CMySamPalteDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMySamPalteDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	return;

	if(m_sonWnd.IsWindowVisible())
	{
		m_sonWnd.ShowWindow(SW_HIDE);
	}
	else 
	{
		CRect rcTemp = CRect(point.x,point.y,point.x + 100,point.y + 60);
		ClientToScreen(&rcTemp);
		m_sonWnd.MoveWindow(&rcTemp,FALSE);
		m_sonWnd.ShowWindow(SW_SHOW);
		m_sonWnd.SetCapture();
	}
	
	if(!m_bEditable)
	{
		return;
	}
}

void CMySamPalteDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	CDotPosInfo dotPosInfo = CalcPtRowCol(point);	
	if(EDT_BODY == dotPosInfo.m_eDotPosType && !m_lnkRowInfo[dotPosInfo.m_rowId]->m_bDisabled)
	{
		
	}

	return;
}

void CMySamPalteDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLeftBtDown = TRUE;

	if(!m_bEditable)
	{
		return CDialogEx::OnLButtonDown(nFlags,point);
	}

	SetFocus();

	m_dotPosLeftBtDown = CalcPtRowCol(point);

	if(m_horiScroll.m_bInSlider)
	{
		m_horiScroll.m_bLeftBtDown = TRUE;
		m_horiScroll.m_prePt = point;
		SetCapture();
	}
	else if(m_vertScroll.m_bInSlider)
	{
		m_vertScroll.m_bLeftBtDown = TRUE;
		m_vertScroll.m_prePt = point;
		SetCapture();
	}
	else
	{
		//���������ʱ���ڵ��к���
		CDotPosInfo dotPosInfo = CalcPtRowCol(point);
		if(EDT_BODY == dotPosInfo.m_eDotPosType && -1 != dotPosInfo.m_rowId && !m_lnkRowInfo[dotPosInfo.m_rowId]->m_bDisabled)
		{
			m_bDraggableStart = TRUE;
			m_dragStartDotPosInfo = dotPosInfo;
			SetCapture();
		}
	}

DEFPROCESS:
	CDialogEx::OnLButtonDown(nFlags,point);

	return;
}

void CMySamPalteDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_bLeftBtDown)
	{
		return CDialogEx::OnLButtonUp(nFlags,point);;
	}
	m_bLeftBtDown = FALSE;

	if(!m_bEditable)
	{
		return CDialogEx::OnLButtonUp(nFlags,point);
	}

	if(!GetBeValidPlate())
	{
		return;
	}

	if(m_horiScroll.m_bLeftBtDown || m_vertScroll.m_bLeftBtDown)
	{
		ReleaseCapture();
	}

	if(m_bDraggableStart)
	{
		m_bDraggableStart = FALSE;
		ReleaseCapture();
	}

	m_preDotPosInfo = m_curDotPosInfo;  
	m_curDotPosInfo = CalcPtRowCol(point);
	m_lnkHistoryDotPosInfo.AddTail(m_curDotPosInfo);
	m_dotPosLeftBtUp = m_curDotPosInfo;

	m_horiScroll.m_bLeftBtDown = FALSE;
	m_horiScroll.m_bStartDragSlider = FALSE;
	m_horiScroll.m_bSliderDragMove = FALSE;

	m_vertScroll.m_bLeftBtDown = FALSE;
	m_vertScroll.m_bStartDragSlider = FALSE;
	m_vertScroll.m_bSliderDragMove = FALSE;

	BOOL bCtrlPushed = MK_CONTROL & nFlags;
	BOOL bNeedRedraw = FALSE,bSelChange = FALSE;

	vector<BOOL> vecOriginSel;
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		vecOriginSel.push_back(m_pPlateInfo[i].m_bSelected);
	}

	if(m_bDragMove)	//�϶����ѡ��������
	{
		m_bDragMove = FALSE;
		CPoint ptNew = point;
		if(point.y > m_rcBody.bottom)
		{
			ptNew.y = m_rcBody.bottom - 1;
		}
		if(point.y < m_rcBody.top)
		{
			ptNew.y = m_rcBody.top + 1;
		}
		if(point.x > m_rcBody.right)
		{
			ptNew.x = m_rcBody.right - 1;
		}
		if(point.x < m_rcBody.left)
		{
			ptNew.x = m_rcBody.left + 1;
		}
		CDotPosInfo tempDotPosInfo = CalcPtRowCol(ptNew);

		int startX = 0,endX = 0,startY,endY = 0;
		if(m_dragStartDotPosInfo.m_rowId <= tempDotPosInfo.m_rowId)
		{
			startX = m_dragStartDotPosInfo.m_rowId;
			endX = tempDotPosInfo.m_rowId;
		}
		else
		{
			endX = m_dragStartDotPosInfo.m_rowId;
			startX = tempDotPosInfo.m_rowId;
		}
		
		if(m_dragStartDotPosInfo.m_colId <= tempDotPosInfo.m_colId)
		{
			startY = m_dragStartDotPosInfo.m_colId;
			endY = tempDotPosInfo.m_colId;
		}
		else
		{
			endY = m_dragStartDotPosInfo.m_colId;
			startY = tempDotPosInfo.m_colId;
		}

		if(-1 == startX)
		{
			startX = 0;
		}
		if(-1 == endX)
		{
			endX = 0;
		}
		if(-1 == startY)
		{
			startY = 0;
		}
		if(-1 == endY)
		{
			endY = 0;
		}

		if(!bCtrlPushed)
		{
			for(int i = 0;i < m_iTotalSiteNum;++i)
			{
				m_pPlateInfo[i].m_bSelected = FALSE;
			}
		}

		int index = 0;
		for(int i = startY;i <= endY;++i)
		{
			for(int j = startX;j <= endX;++j)
			{
				index = j * m_colNum + i;
				m_pPlateInfo[index].m_bSelected = TRUE;
			}
		}

		UpdateRowSelectState(FALSE);
		UpdateColSelectState(FALSE);
		GetBeAllSelected();

		bNeedRedraw = TRUE;
		//�ж�ѡ�п�λ�Ƿ�ı�
		for(int i = 0;i < m_iTotalSiteNum;++i)
		{
			if(vecOriginSel[i] != m_pPlateInfo[i].m_bSelected)
			{
				bSelChange = TRUE;
				break;
			}
		}	
	}
	else	
	{
		//���¿װ�ѡ��״̬
		if(EDT_TOTALHEADER == m_dotPosLeftBtUp.m_eDotPosType)
		{
			SetAllSelectState(!m_bAllSelected);
			bNeedRedraw = TRUE;
			bSelChange = TRUE;
		}
		else if(EDT_ROWHEADER == m_dotPosLeftBtUp.m_eDotPosType && -1 != m_dotPosLeftBtUp.m_rowId)
		{
			BOOL bSelected = m_lnkRowInfo[m_dotPosLeftBtUp.m_rowId]->m_bSelected;

			for(int j = 0;j < m_rowNum;++j)
			{
				if(j == m_dotPosLeftBtUp.m_rowId)
				{
					for(int i = 0;i < m_colNum;++i)
					{
						m_pPlateInfo[j * m_colNum + i].m_bSelected = TRUE;
					}
				}
				else if(!bCtrlPushed)
				{
					for(int i = 0;i < m_colNum;++i)
					{
						m_pPlateInfo[j * m_colNum + i].m_bSelected = FALSE;
					}
				}
			}

			UpdateRowSelectState(FALSE);
			UpdateColSelectState(FALSE);
			GetBeAllSelected();
			bNeedRedraw = TRUE;
			bSelChange = TRUE;
		}
		else if(EDT_COLHEADER == m_dotPosLeftBtUp.m_eDotPosType && -1 != m_dotPosLeftBtUp.m_colId)
		{
			BOOL bSelected = m_lnkTitleInfo[m_dotPosLeftBtUp.m_colId]->m_bSelected;
			for(int j = 0;j < m_colNum;++j)
			{
				if(j == m_dotPosLeftBtUp.m_colId)
				{
					for(int i = 0;i < m_rowNum;++i)
					{
						m_pPlateInfo[i * m_colNum + m_dotPosLeftBtUp.m_colId].m_bSelected = TRUE;
					}
				}
				else if(!bCtrlPushed)
				{
					for(int i = 0;i < m_rowNum;++i)
					{
						m_pPlateInfo[i * m_colNum + j].m_bSelected = FALSE;
					}
				}
			}

			UpdateColSelectState(FALSE);
			UpdateRowSelectState(FALSE);
			GetBeAllSelected();
			bNeedRedraw = TRUE;
			bSelChange = TRUE;
		}
		else if(EDT_BODY == m_dotPosLeftBtUp.m_eDotPosType && -1 != m_dotPosLeftBtUp.m_rowId && -1 != m_dotPosLeftBtUp.m_colId)
		{
			int iSiteIndex = m_dotPosLeftBtUp.m_rowId * m_colNum + m_dotPosLeftBtUp.m_colId;

			CRowInfo& rowInfo = *m_lnkRowInfo[m_dotPosLeftBtUp.m_rowId];
			CTitleInfo& titleInfo = *m_lnkTitleInfo[m_dotPosLeftBtUp.m_colId];
			CPlateInfo& plateInfo = m_pPlateInfo[iSiteIndex];
			
			BOOL bSelected = plateInfo.m_bSelected;
			if(bCtrlPushed)
			{
				plateInfo.m_bSelected = !bSelected;
				bNeedRedraw = TRUE;
				bSelChange = TRUE;
			}
			else
			{
				bNeedRedraw = TRUE;
				bSelChange = TRUE;
				for(int i = 0;i < m_iTotalSiteNum;++i)
				{
					if(i == iSiteIndex)
					{
						m_pPlateInfo[i].m_bSelected = m_bSingleSelect || bSelected;	//���Ż���20220301
						if(m_bSingleSelect && m_preDotPosInfo == m_dotPosLeftBtUp && 1 == vecOriginSel.size())
						{
							bNeedRedraw = FALSE;
							bSelChange = FALSE;
						}
					}
					else
					{
						if(m_bSingleSelect)
						{
							m_pPlateInfo[i].m_bSelected = FALSE;
						}
					}
				}
			}
			
			//��������ѡ��״̬
			UpdateRowSelectState(FALSE);
			UpdateColSelectState(FALSE);
			GetBeAllSelected();
		}
	}

REDRAWLST:
	if(bNeedRedraw)
	{
		Invalidate(FALSE);
	}
	
	if(m_bTellParentSiteSelChange)
	{
		CWnd* pParent = GetParent();
		if(bSelChange && NULL != pParent->GetSafeHwnd())	//ѡ���иı䣬֪ͨ������
		{
			pParent->PostMessage(WM_TELLPARENTSITESELECTCHANGED,0,0);
		}   
	}
	
	CDialogEx::OnLButtonUp(nFlags,point);
	return;
}

void CMySamPalteDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	if(m_horiScroll.m_bScrollNeedShow && m_horiScroll.m_rcSlider.PtInRect(point))
	{
		m_horiScroll.m_bInSlider = TRUE;
		if(m_horiScroll.m_bLeftBtDown)
		{
			m_horiScroll.m_bStartDragSlider = TRUE;
		}

		if(m_horiScroll.m_bStartDragSlider && (MK_LBUTTON & nFlags))
		{
			m_horiScroll.m_bSliderDragMove = TRUE;
		}
	}
	else
	{
		m_horiScroll.m_bInSlider = FALSE;
	}

	if(m_vertScroll.m_bScrollNeedShow && m_vertScroll.m_rcSlider.PtInRect(point))
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

	if(m_horiScroll.m_bScrollNeedShow && m_horiScroll.m_bSliderDragMove)
	{
		ProcessHoriSrcoll(nFlags,point);
	}
	else if(m_vertScroll.m_bScrollNeedShow && m_vertScroll.m_bSliderDragMove)
	{
		ProcessVertSrcoll(nFlags,point);
	}

	//�����϶�ѡ��������λ
	if(m_bDraggableStart && (nFlags & MK_LBUTTON))
	{	
		m_bDragMove = TRUE;

		CPoint ptNew = point;
		if(!m_rcBody.PtInRect(point))
		{
			if(point.y > m_rcBody.bottom)
			{
				ptNew.y = m_rcBody.bottom;
			}
			if(point.y < m_rcBody.top)
			{
				ptNew.y = m_rcBody.top;
			}
			if(point.x > m_rcBody.right)
			{
				ptNew.x = m_rcBody.right;
			}
			if(point.x < m_rcBody.left)
			{
				ptNew.x = m_rcBody.left;
			}
		}

		CClientDC dc(this);
		CPen penStroke;
		if(penStroke.CreatePen(PS_DASH,1,RGB(0X31,0X24,0X1B)))
		{
			//ѡ����
			CPen* pOldPen = dc.SelectObject(&penStroke);
			dc.SelectStockObject(NULL_BRUSH);
			int n = dc.GetROP2();
			dc.SetROP2(R2_NOTXORPEN);		
			dc.Rectangle(m_dragStartDotPosInfo.m_pt.x,m_dragStartDotPosInfo.m_pt.y,m_ptPreOnMove.x,m_ptPreOnMove.y);
			dc.Rectangle(m_dragStartDotPosInfo.m_pt.x,m_dragStartDotPosInfo.m_pt.y,ptNew.x,ptNew.y);

			//�ָ�����
			dc.SelectObject(pOldPen);
			dc.SetROP2(n);
		}
		m_ptPreOnMove = ptNew;
	}
	else
	{
		m_ptPreOnMove = point;
	}
	
	return;
}

BOOL CMySamPalteDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_bEditable)
	{
		return __super::OnMouseWheel(nFlags, zDelta, pt);
	}

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

int CMySamPalteDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CDialogEx::OnCreate(lpCreateStruct);

	m_iRectW = 50 * m_dHoriRatio;
	m_iRectH = 42 * m_dVertRatio;

	m_iRoundAngleLT *= m_dTxRatio;
	m_iRoundAngleRT *= m_dTxRatio;
	m_iRoundAngleRB *= m_dTxRatio;
	m_iRoundAngleLB *= m_dTxRatio;

	m_sonWnd.Create(IDD_POPUPDLG,this);
	m_sonWnd.InsertRow(0,_T("1"));
	m_sonWnd.InsertRow(0,_T("2"));
	m_sonWnd.InsertRow(0,_T("3"));
	m_sonWnd.InsertRow(0,_T("4"));
	m_sonWnd.InsertRow(0,_T("�л����񹲺͹�"));

	return 1;
}

void CMySamPalteDlg::OnSize(UINT nType, int cx, int cy)
{
	m_rcLst = CRect(0,0,cx,cy);
	m_rcTotalHeader = CRect(m_pxFrame,m_pxFrame,m_pxRowHeaderW,m_pxColHeaderH);
	m_rcRowHeader = CRect(m_pxFrame,m_pxColHeaderH,m_pxRowHeaderW,cy - m_pxFrame);
	m_rcColHeader = CRect(m_pxRowHeaderW,m_pxFrame,cx - m_pxFrame,m_pxColHeaderH); 
	m_rcBody = CRect(m_pxRowHeaderW,m_pxColHeaderH,cx - m_pxFrame,cy - m_pxFrame);

	if(0 != m_colNum)
	{
		m_pxColW = (cx - m_pxFrame * 2 - m_pxRowHeaderW) / m_colNum;
	}

	if(0 != m_rowNum)
	{
		m_pxRowH = (cy - m_pxFrame * 2 - m_pxColHeaderH) / m_rowNum;
	}

	for(int i = 0;i < m_rowNum;++i)
	{
		m_lnkRowInfo[i]->m_rowH = m_pxRowH;
	}

	for(int i = 0;i < m_colNum;++i)
	{
		m_lnkTitleInfo[i]->m_colWidth = m_pxColW;
	}

#ifdef SAMPLATE_16SITE
	int iColRemainder = 0,iRowRemainder = 0;

	//���¼����иߺ��п�
	if(m_colNum > 0)
	{
		m_pxColW = m_rcBody.Width() / m_colNum;
		iColRemainder = m_rcBody.Width() % m_colNum;
	}
	if(m_rowNum > 0)
	{
		m_pxRowH = m_rcBody.Height() / m_rowNum;
		iRowRemainder = m_rcBody.Height() % m_rowNum;
	}

	for(int i = 0;i < m_colNum;++i)
	{
		if(i == m_colNum - 1)
		{
			m_lnkTitleInfo[i]->m_colWidth = m_pxColW + iColRemainder;
		}
		else
		{
			m_lnkTitleInfo[i]->m_colWidth = m_pxColW;
		}
	}
	for(int i = 0;i < m_rowNum;++i)
	{
		if(i == m_rowNum - 1)
		{
			m_lnkRowInfo[i]->m_rowH = m_pxRowH +iRowRemainder;
		}
		else
		{
			m_lnkRowInfo[i]->m_rowH = m_pxRowH;
		}
	}
#endif

	CalcTotalColumnWidth();
	CalcTotalRowHeight();

	CalcScrollPos();
	CalcRowColToShow();
	CalcRowColToShowOnNoRowData();

	CDialogEx::OnSize(nType, cx, cy);
} 

BOOL CMySamPalteDlg::PreTranslateMessage(MSG* pMsg)
{
	if(WM_KEYDOWN == pMsg->message)
	{
		int tt = 0;
		++tt;

		if(!m_bShieldDeleteKey && VK_DELETE == pMsg->wParam)
		{
			if(m_bParentProcessDeleteKey)
			{
				CWnd* pParent = GetParent();
				if(NULL != pParent->GetSafeHwnd())
				{
					pParent->PostMessage(SAMPLATE_DELETEKEY_PRESSED,0,0);
				}
			}
			else
			{
				DeleteSelectedTubeInfo(TRUE);
			}
		}
	}

	if(WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		switch(pMsg->wParam)
		{
		case VK_DELETE:
			if(1)
			{
				int tt = 0;
				++tt;
			}
			return TRUE;
		case VK_ESCAPE:	return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CMySamPalteDlg::OnMsgEditEnterPushed(WPARAM wParam,LPARAM lParam)
{
	return 1;
}

void CMySamPalteDlg::ProcessHoriSrcoll(UINT nFlags, CPoint point)
{
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

void CMySamPalteDlg::ProcessVertSrcoll(UINT nFlags, CPoint point)
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

int CMySamPalteDlg::GetColStartDrawInMemoryHoriPixel(int colId)
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

int CMySamPalteDlg::GetRowStartDrawInMemoryVertPixel(int rowId)
{
	if(rowId < m_rowIdStartShow)
	{
		return -1;
	}

	int horiPixel = 0;
	for(int i = m_rowIdStartShow;i < rowId;++i)
	{
		horiPixel += m_lnkRowInfo[i]->m_rowH;
	}
	return horiPixel;
}

int CMySamPalteDlg::GetWidth(int colStart,int colEnd)
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

int CMySamPalteDlg::GetHeight(int rowStart,int rowEnd)
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

CDotPosInfo CMySamPalteDlg::CalcPtRowCol(CPoint pt)
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

	//pt��header������������
	if(m_rcTotalHeader.PtInRect(pt))
	{
		dotPosInfo.m_eDotPosType = EDT_TOTALHEADER;
	}
	else if(m_rcRowHeader.PtInRect(pt))
	{
		dotPosInfo.m_eDotPosType = EDT_ROWHEADER;
		int sum = m_pxFrame + m_pxColHeaderH + m_lnkRowInfo[m_rowIdStartShow]->m_rowH - m_ssRowTopIndent;
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
	else if(m_rcColHeader.PtInRect(pt))
	{
		dotPosInfo.m_eDotPosType = EDT_COLHEADER;
		int sum = m_pxFrame + m_pxRowHeaderW + m_lnkTitleInfo[m_colIdStartShow]->m_colWidth - m_ssColLeftIndent;
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
	else if(m_rcBody.PtInRect(pt) && 0 != m_rowNum)	//pt��body��
	{
		dotPosInfo.m_eDotPosType = EDT_BODY;

		int sum = m_pxFrame + m_pxRowHeaderW + m_lnkTitleInfo[m_colIdStartShow]->m_colWidth - m_ssColLeftIndent;
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

		sum = m_pxFrame + m_pxColHeaderH + m_lnkRowInfo[m_rowIdStartShow]->m_rowH - m_ssRowTopIndent;
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

int CMySamPalteDlg::InsertColumn(int colId,CString strColTitle,BOOL bRedraw /*= TRUE*/)
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
		CalcRowColToShow();

		//����ˮƽ��������ǰλ��
		if(m_totalColW > m_rcBody.Width())
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
		}
		else
		{
			m_horiScroll.m_curPixelPos = 0;
		}
		CalcScrollPos();

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

int CMySamPalteDlg::InsertRow(int rowId,BOOL bRedraw /*= TRUE*/)
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
		}
		m_2LnkCellInfo[rowId]->AddTail(cellInfo);
	}

	CalcTotalRowHeight();
	CalcRowColToShow();

	//���´�ֱ��������ǰλ��
	if(m_totalRowH > m_rcBody.Height())
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
	}
	else
	{
		m_vertScroll.m_curPixelPos = 0;
	}
	CalcScrollPos();

	//���������Сֵ������Ϣ
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

int CMySamPalteDlg::DeleteColumn(int colId,BOOL bRedraw /*= TRUE*/)
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

	if(colId < m_colIdStartShow)
	{
		CalcColToShow(m_colIdStartShow - 1);
	}
	else
	{
		CalcColToShow(m_colIdStartShow);
	}

	//����ˮƽ��������ǰλ��
	if(m_totalColW > m_rcBody.Width())
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
	}
	else
	{
		m_horiScroll.m_curPixelPos = 0;
	}
	CalcScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySamPalteDlg::DeleteRow(int rowId,BOOL bRedraw /*= TRUE*/)
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

	if(rowId < m_rowIdStartShow)
	{
		CalcRowToShow(m_rowIdStartShow - 1);
	}
	else
	{
		CalcRowToShow(m_rowIdStartShow);
	}
	
	//���´�ֱ��������ǰλ��
	if(m_totalRowH > m_rcBody.Height())
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

int CMySamPalteDlg::DeleteMultiRow(vector<int> vecSelectedRow,BOOL bRedraw /*= TRUE*/)
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

	//���´�ֱ��������ǰλ��
	if(m_totalRowH > m_rcBody.Height())
	{
		m_vertScroll.m_curPixelPos -= rowH;
		if(m_vertScroll.m_curPixelPos < 0)
		{
			m_vertScroll.m_curPixelPos = 0;
		}
	}
	else
	{
		m_vertScroll.m_curPixelPos = 0;
	}

	CalcScrollPos();

	if(fstDeleteRowId < m_rowIdStartShow)
	{
		CalcRowToShow(m_rowIdStartShow - 1);
	}
	else
	{
		CalcRowToShow(m_rowIdStartShow);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

void CMySamPalteDlg::DeleteData(BOOL bRedraw /*= FALSE*/)
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

void CMySamPalteDlg::Clear(BOOL bRedraw /*= FALSE*/)
{
	m_2LnkCellInfo.Clear();
	m_lnkRowInfo.Clear();
	m_rowNum = 0;

	CalcTotalRowHeight();

	//���´�ֱ��������ǰλ��
	m_vertScroll.m_curPixelPos = 0;
	CalcScrollPos();

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

vector<int> CMySamPalteDlg::GetSelectedRowId()
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

int CMySamPalteDlg::SetColumnTitleInfo(int colId,CTitleInfo titleInfo,BOOL bRedraw /*= TRUE*/)
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

int CMySamPalteDlg::GetColumnTitleInfo(int colId,CTitleInfo& titleInfo)
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

int CMySamPalteDlg::SetColumnCheckFunc(int colId,BOOL bHaveCheckFunc,BOOL bRedraw /*= FALSE*/)
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

int CMySamPalteDlg::GetColumnCheckFunc(int colId,BOOL& bHaveCheckFunc)
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

int CMySamPalteDlg::SetColumnColorFunc(int colId,BOOL bHaveColorFunc,BOOL bRedraw /*= FALSE*/)
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

int CMySamPalteDlg::GetColumnColorFunc(int colId,BOOL& bHaveColorFunc)
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

int CMySamPalteDlg::SetColumnWidth(int colId,int colWidth,BOOL bRedraw /*= FALSE*/)
{
	if(colId < 0)
	{
		return -1;
	}

	if(colId >= m_colNum)
	{
		return -2;
	}

	CTitleInfo* pTitleInfo = m_lnkTitleInfo[colId];
	if(NULL == pTitleInfo)
	{
		return -3;
	}
	pTitleInfo->m_colWidth = colWidth;

	CalcTotalColumnWidth();
	CalcScrollPos();
	CalcRowColToShow();
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	
	return 0;
}

int CMySamPalteDlg::GetColumnWidth(int colId)
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

int CMySamPalteDlg::SetItemHeight(int itemH,BOOL bRedraw /*= FALSE*/)
{
	m_pxRowH = itemH;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySamPalteDlg::GetItemHeight()
{
	return m_pxRowH;
}

int CMySamPalteDlg::SetFixedRowHeightColumnWidth(int iRowH,int iColW,BOOL bRedraw /*= FALSE*/)
{
	m_pxRowH = iRowH;
	m_pxColW = iColW;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySamPalteDlg::GetFixedRowHeightColumnWidth(int& iRowH,int& iColW)
{
	iRowH = m_pxRowH;
	iColW = m_pxColW;
	return 0;
}

int CMySamPalteDlg::SetColumnText(int colId,CString strColTitle)
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
	return 0;
}

CString CMySamPalteDlg::GetColumnText(int colId)
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

int CMySamPalteDlg::SetColumnMaxMinInfo(int colId,CMaxMinInfo maxMinInfo)
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

int CMySamPalteDlg::GetColumnMaxMinInfo(int colId,CMaxMinInfo& maxMinInfo)
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

int CMySamPalteDlg::SetItemText(int rowId,int colId,CString strText,BOOL bRedraw /*= FALSE*/)
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

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

CString CMySamPalteDlg::GetItemText(int rowId,int colId)
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

int CMySamPalteDlg::SetRowSelectState(int rowId,BOOL bSelected,BOOL bSingleSel /*= FALSE*/,BOOL bRedraw /*= FALSE*/)
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

int CMySamPalteDlg::GetRowSelectState(int rowId,BOOL& bSelected)
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

int CMySamPalteDlg::SetRowDisableState(int rowId,BOOL bDisabled,BOOL bRedraw /*= FALSE*/)
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

int CMySamPalteDlg::GetRowDisableState(int rowId,BOOL& bDisabled)
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

int CMySamPalteDlg::SetCellCheckState(int rowId,int colId,BOOL bChecked,BOOL bRedraw /*= FALSE*/)
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

	if(!m_lnkTitleInfo[colId]->m_bCheckFunc)
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
	
	return 0;
}

int CMySamPalteDlg::GetCellCheckState(int rowId,int colId,BOOL& bChecked)
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

	if(!m_lnkTitleInfo[colId]->m_bCheckFunc)
	{
		return -5;
	}

	bChecked = m_2LnkCellInfo[rowId]->GetAt(colId)->data.GetItemCheckState();
	return 0;
}

int CMySamPalteDlg::SetCellDisableState(int rowId,int colId,BOOL bDisabled,BOOL bRedraw /*= FALSE*/)
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

int CMySamPalteDlg::GetCellDisableState(int rowId,int colId,BOOL& bDisabled)
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

int CMySamPalteDlg::SetCellSelfCbInfo(int rowId,int colId,BOOL bCbable,BOOL bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL bEditableCb,BOOL bRedraw /*= FALSE*/)
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

int CMySamPalteDlg::GetCellSelfCbInfo(int rowId,int colId,BOOL& bCbable,BOOL& bUseCellLnkCbStr,CLinkList<CString>& lnkCbStr,BOOL& bEditableCb)
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

int CMySamPalteDlg::GetRowNum()
{
	return m_rowNum;
}

int CMySamPalteDlg::GetColNum()
{
	return m_colNum;
}

void CMySamPalteDlg::SetBeGradientHeader(BOOL bGradientHeader,BOOL bRedraw /*= FALSE*/)
{
	m_bGradientHeader = bGradientHeader;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetBeGradientHeader()
{
	return m_bGradientHeader;
}

void CMySamPalteDlg::SetBkColor(COLORREF bkClr,BOOL bRedraw /*= FALSE*/)
{
	m_clrBk = bkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMySamPalteDlg::GetBkColor()
{
	return m_clrBk;
}

void CMySamPalteDlg::SetHeaderBkColor(COLORREF headerBkClr,BOOL bRedraw /*= FALSE*/)
{
	m_clrHeaderBk = headerBkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

Color CMySamPalteDlg::GetHeaderBkColor()
{
	return m_clrHeaderBk;
}

void CMySamPalteDlg::SetBodyBkColor(COLORREF bodyBkClr,BOOL bRedraw /*= FALSE*/)
{
	m_clrBodyBk = bodyBkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMySamPalteDlg::GetBodyBkColor()
{
	return m_clrBodyBk;
}

void CMySamPalteDlg::SetRowHeaderWidth(int iRowW,BOOL bRedraw /*= FALSE*/)
{
	m_pxRowHeaderW = iRowW;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMySamPalteDlg::GetRowHeaderWidth()
{
	return m_pxRowHeaderW;
}

void CMySamPalteDlg::SetColumnHeaderHeight(int iColH,BOOL bRedraw /*= FALSE*/)
{
	m_pxColHeaderH = iColH;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMySamPalteDlg::GetColumnHeaderHeight()
{
	return m_pxColHeaderH;
}

void CMySamPalteDlg::SetFrameLineWidth(int frameLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_pxFrame = frameLineWidth;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMySamPalteDlg::GetFrameLineWidth()
{
	return m_pxFrame;
}

void CMySamPalteDlg::SetFrameLineColor(COLORREF frameLineColor,BOOL bRedraw /*= FALSE*/)
{
	m_clrFrame = frameLineColor;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMySamPalteDlg::GetFrameLineColor()
{
	return m_clrFrame;
}

void CMySamPalteDlg::SetRowColLineWidth(int rowColLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_pxRowColLine = rowColLineWidth;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMySamPalteDlg::GetRowColLineWidth()
{
	return m_pxRowColLine;
}


void CMySamPalteDlg::SetRowColLineColor(COLORREF rowColLineColor,BOOL bRedraw /*= FALSE*/)
{
	m_clrRowColLine = rowColLineColor;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMySamPalteDlg::GetRowColLineColor()
{
	return m_clrRowColLine;
}

void CMySamPalteDlg::SetBeShowFrameLine(int bShowFrameLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowFrameLine = bShowFrameLine;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetBeShowFrameLine()
{
	return m_bShowFrameLine;
}

void CMySamPalteDlg::SetBeShowRowColLine(int bShowRowColLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowRowColLine = bShowRowColLine;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetBeShowRowColLine()
{
	return m_bShowRowColLine;
}

void CMySamPalteDlg::SetHeaderTextColor(COLORREF headTextColor,BOOL bRedraw /*= FALSE*/)
{
	m_clrHeaderText = headTextColor;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMySamPalteDlg::GetHeaderTextColor()
{
	return m_clrHeaderText;
}

void CMySamPalteDlg::SetBeSingleSelect(BOOL bSingleSelect,BOOL bRedraw /*= FALSE*/)
{
	m_bSingleSelect = bSingleSelect;

	vector<int> vecSelectedRow = GetSelectedRowId();	//����ж��б�ѡ�У����õ�һ��ѡ��
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

BOOL CMySamPalteDlg::GetBeSingleSelect()
{
	return m_bSingleSelect;
}

void CMySamPalteDlg::SetTitle(CString strTitle,BOOL bRedraw /*= FALSE*/)
{
	m_strTotalTitle = strTitle;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

CString CMySamPalteDlg::GetTitle()
{
	return m_strTotalTitle;
}

void CMySamPalteDlg::SetBeEditable(BOOL bEditable,BOOL bRedraw /*= FALSE*/)
{
	m_bEditable = bEditable;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetBeEditable()
{
	return m_bEditable;
}

int CMySamPalteDlg::GetRowTag(int iRowId,CString& strTag)
{
	if(iRowId < 0)
	{
		return -1;
	}
	if(iRowId >= m_rowNum)
	{
		return -2;
	}
	// Multi-letter row tag: A..Z, AA, AB, ...
	CString rowLabel;
	int idx = iRowId;
	while (idx >= 0)
	{
		int rem = idx % 26;
		rowLabel.Insert(0, TCHAR('A' + rem));
		idx = idx / 26 - 1;
	}
	strTag = rowLabel;
	return 0;
}

int CMySamPalteDlg::GetColTag(int iColId,CString& strTag)
{
	if(iColId < 0)
	{
		return -1;
	}
	if(iColId >= m_colNum)
	{
		return -2;
	}
	strTag.Format(_T("%d"),iColId + 1);
	return 0;
}

int CMySamPalteDlg::GetSiteTag(int iSiteIndex,CString& strTag)
{
	if(iSiteIndex < 0)
	{
		return -1;
	}
	if(iSiteIndex >= m_iTotalSiteNum)
	{
		return -2;
	}
	if(m_rowNum < 1)
	{
		return -3;
	}
	if(m_colNum < 1)
	{
		return -4;
	}

	int rowIndex = iSiteIndex / m_colNum;
	int colIndex = iSiteIndex % m_colNum;
	// Build multi-letter row label (A..Z, AA..)
	CString rowLabel;
	int idx = rowIndex;
	while (idx >= 0)
	{
		int rem = idx % 26;
		rowLabel.Insert(0, TCHAR('A' + rem));
		idx = idx / 26 - 1;
	}
	strTag.Format(_T("%s%d"), rowLabel, colIndex + 1);
	return 0;
}

int CMySamPalteDlg::GetSiteTag(int iRowId,int iColId,CString& strTag)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(iRowId < 0)
	{
		return -3;
	}
	if(iRowId >= m_rowNum)
	{
		return -4;
	}
	if(iColId < 0)
	{
		return -5;
	}
	if(iColId >= m_colNum)
	{
		return -6;
	}
	// Convert row index to letters (A..Z, AA..)
	CString rowLabel;
	int idx = iRowId;
	while (idx >= 0)
	{
		int rem = idx % 26;
		rowLabel.Insert(0, TCHAR('A' + rem));
		idx = idx / 26 - 1;
	}
	strTag.Format(_T("%s%d"), rowLabel, iColId + 1);
	return 0;
}

int CMySamPalteDlg::SetRowColNum(int iRowNum,int iColNum,BOOL bRedraw /*= FALSE*/)
{
	if(iRowNum < 1)
	{
		return -1;
	}
	if(iColNum < 1)
	{
		return -2;
	}

	if(m_rowNum == iRowNum && m_colNum == iColNum)
	{
		return 1;
	}

	//���¼����иߺ��п�
	m_pxColW = m_rcBody.Width() / iColNum;
	int iColRemainder = m_rcBody.Width() % iColNum;

	m_pxRowH = m_rcBody.Height() / iRowNum;
	int iRowRemainder = m_rcBody.Height() % iRowNum;

	MyMulitDeletePtr(&m_pTubeInfo);
	MyMulitDeletePtr(&m_pPlateInfo);

	m_lnkTitleInfo.Clear();
	m_lnkRowInfo.Clear();
	m_2LnkCellInfo.Clear();

	CTitleInfo titleInfo;
	titleInfo.m_colWidth = m_pxColW;
	for(int i = 0;i < iColNum;++i)
	{
		if(i == iColNum - 1)
		{
			titleInfo.m_colWidth += iColRemainder;
		}
		m_lnkTitleInfo.AddTail(titleInfo);
	}
	CRowInfo rowInfo;
	rowInfo.m_rowH = m_pxRowH;
	for(int i = 0;i < iRowNum;++i)
	{
		if(i == iRowNum - 1)
		{
			rowInfo.m_rowH += iRowRemainder;
		}
		m_lnkRowInfo.AddTail(rowInfo);
	}

	m_iTotalSiteNum = iRowNum * iColNum;
	m_pTubeInfo = new CTubeInfo[m_iTotalSiteNum];
	if(NULL == m_pTubeInfo)
	{
		return -3;
	}

	m_pPlateInfo = new CPlateInfo[m_iTotalSiteNum];
	if(NULL == m_pPlateInfo)
	{
		MyMulitDeletePtr(&m_pTubeInfo);
		return -4;
	}

	m_rowNum = iRowNum;
	m_colNum = iColNum;

	CalcTotalColumnWidth();
	CalcRowColToShow();

	//����ˮƽ��������ǰλ��
	if(m_totalColW > m_rcBody.Width())
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
	}
	else
	{
		m_horiScroll.m_curPixelPos = 0;
	}
	CalcScrollPos();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

BOOL CMySamPalteDlg::GetBeValidPlate()
{
	if(m_rowNum < 1 || m_colNum < 1 || m_iTotalSiteNum < 1 || NULL == m_pTubeInfo || NULL == m_pPlateInfo || m_lnkRowInfo.GetSize() != m_rowNum || m_lnkTitleInfo.GetSize() != m_colNum)
	{
		return FALSE;
	}
	return TRUE;
}

int CMySamPalteDlg::UpdateRowSelectState(int iRowId,BOOL bRedraw)
{
	if(iRowId < 0)
	{
		return -1;
	}
	if(iRowId >= m_rowNum)
	{
		return -2;
	}
	if(NULL == m_pPlateInfo)
	{
		return -3;
	}
	if(iRowId >= m_lnkRowInfo.GetSize())
	{
		return -4;
	}
	
	BOOL bSelected = TRUE;
	int iStart = iRowId * m_colNum,iEnd = iStart + m_colNum;
	for(int i = iStart;i < iEnd;++i)
	{
		if(!m_pPlateInfo[i].m_bSelected)
		{
			bSelected = FALSE;
			break;
		}
	}

	m_lnkRowInfo[iRowId]->m_bSelected = bSelected;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySamPalteDlg::UpdateColSelectState(int iColId,BOOL bRedraw)
{
	if(iColId < 0)
	{
		return -1;
	}
	if(iColId >= m_colNum)
	{
		return -2;
	}
	if(NULL == m_pPlateInfo)
	{
		return -3;
	}
	if(iColId >= m_lnkTitleInfo.GetSize())
	{
		return -4;
	}

	BOOL bSelected = TRUE;
	for(int i = 0;i < m_rowNum;++i)
	{
		if(!m_pPlateInfo[i * m_colNum + iColId].m_bSelected)
		{
			bSelected = FALSE;
			break;
		}
	}

	m_lnkTitleInfo[iColId]->m_bSelected = bSelected;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySamPalteDlg::UpdateRowSelectState(BOOL bRedraw)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(NULL == m_pPlateInfo)
	{
		return -3;
	}
	if(m_rowNum > m_lnkRowInfo.GetSize())
	{
		return -4;
	}
	if(m_colNum > m_lnkTitleInfo.GetSize())
	{
		return -5;
	}

	for(int i = 0;i < m_rowNum;++i)
	{
		BOOL bSelected = FALSE;
		for(int j = 0;j < m_colNum;++j)
		{
			if(m_pPlateInfo[i * m_colNum + j].m_bSelected)
			{
				bSelected = TRUE;
				break;
			}
		}
		m_lnkRowInfo[i]->m_bSelected = bSelected;
	}
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySamPalteDlg::UpdateColSelectState(BOOL bRedraw)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(NULL == m_pPlateInfo)
	{
		return -3;
	}
	if(m_rowNum > m_lnkRowInfo.GetSize())
	{
		return -4;
	}
	if(m_colNum > m_lnkTitleInfo.GetSize())
	{
		return -5;
	}

	for(int i = 0;i < m_colNum;++i)
	{
		BOOL bSelected = FALSE;
		for(int j = 0;j < m_rowNum;++j)
		{
			if(m_pPlateInfo[j * m_colNum + i].m_bSelected)
			{
				bSelected = TRUE;
				break;
			}
		}
		m_lnkTitleInfo[i]->m_bSelected = bSelected;
	}
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

void CMySamPalteDlg::SetAllSelectState(BOOL bSelected,BOOL bRedraw /*= FALSE*/)
{
	BOOL bPreAllSelected = m_bAllSelected;

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		m_pPlateInfo[i].m_bSelected = bSelected;
	}
	for(int i = 0;i < m_rowNum;++i)
	{
		m_lnkRowInfo[i]->m_bSelected = bSelected;
	}
	for(int i = 0;i < m_colNum;++i)
	{
		m_lnkTitleInfo[i]->m_bSelected = bSelected;
	}
	m_bAllSelected = bSelected;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	if(m_bTellParentSiteSelChange && (bPreAllSelected != bSelected))
	{
		CWnd* pParent = GetParent();
		if(NULL != pParent->GetSafeHwnd())	
		{
			pParent->PostMessage(WM_TELLPARENTSITESELECTCHANGED,0,0);
		}   
	}

	return ;
}

void CMySamPalteDlg::SetAllSelectStateImmediately(BOOL bSelected,BOOL bRedraw /*= FALSE*/)
{
	BOOL bPreAllSelected = m_bAllSelected;

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		m_pPlateInfo[i].m_bSelected = bSelected;
	}
	for(int i = 0;i < m_rowNum;++i)
	{
		m_lnkRowInfo[i]->m_bSelected = bSelected;
	}
	for(int i = 0;i < m_colNum;++i)
	{
		m_lnkTitleInfo[i]->m_bSelected = bSelected;
	}
	m_bAllSelected = bSelected;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	if(m_bTellParentSiteSelChange && (bPreAllSelected != bSelected))
	{
		CWnd* pParent = GetParent();
		if(NULL != pParent->GetSafeHwnd())	
		{
			pParent->SendMessage(WM_TELLPARENTSITESELECTCHANGED,0,0);
		}   
	}

	return ;
}

BOOL CMySamPalteDlg::GetBeAllSelected()
{
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		if(!m_pPlateInfo[i].m_bSelected)
		{
			m_bAllSelected = FALSE;
			return m_bAllSelected;
		}
	}
	m_bAllSelected = TRUE;
	return m_bAllSelected;
}

int CMySamPalteDlg::SetCurSelectSite(vector<int>& vecSelSite,BOOL bRedraw /*= FALSE*/)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(NULL == m_pPlateInfo)
	{
		return -3;
	}
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		m_pPlateInfo[i].m_bSelected = FALSE;
	}
	for(int i = 0;i < vecSelSite.size();++i)
	{
		m_pPlateInfo[vecSelSite[i]].m_bSelected = TRUE;
	}
	return 0;
}

int CMySamPalteDlg::GetCurSelectSite(vector<int>& vecSelSite)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(NULL == m_pPlateInfo)
	{
		return -3;
	}

	vecSelSite.clear();
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		if(m_pPlateInfo[i].m_bSelected)
		{
			vecSelSite.push_back(i);
		}
	}

	return 0;
}

int CMySamPalteDlg::GetSamInfo(int iSiteIndex,CTubeInfo& samInfo)
{
	if(iSiteIndex < 0)
	{
		return -1;
	}
	if(iSiteIndex >= m_iTotalSiteNum)
	{
		return -2;
	}
	if(m_rowNum < 1)
	{
		return -3;
	}
	if(m_colNum < 1)
	{
		return -4;
	}
	if(NULL == m_pTubeInfo)
	{
		return -5;
	}
	samInfo = m_pTubeInfo[iSiteIndex];
	return 0;
}

int CMySamPalteDlg::GetSamInfo(int iRowId,int iColId,CTubeInfo& samInfo)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(iRowId < 0)
	{
		return -3;
	}
	if(iRowId >= m_rowNum)
	{
		return -4;
	}
	if(iColId < 0)
	{
		return -5;
	}
	if(iColId >= m_colNum)
	{
		return -6;
	}
	if(NULL == m_pTubeInfo)
	{
		return -7;
	}
	samInfo = m_pTubeInfo[iRowId * m_colNum + iColId];
	return 0;
}

int CMySamPalteDlg::GetTubeInfoPtr(int iSiteIndex,CTubeInfo** ppTubeInfo)
{
	if(iSiteIndex < 0)
	{
		return -1;
	}
	if(iSiteIndex >= m_iTotalSiteNum)
	{
		return -2;
	}
	if(m_rowNum < 1)
	{
		return -3;
	}
	if(m_colNum < 1)
	{
		return -4;
	}
	if(NULL == m_pTubeInfo)
	{
		return -5;
	}

	if(NULL == ppTubeInfo)
	{
		return -6;
	}

	*ppTubeInfo = &(m_pTubeInfo[iSiteIndex]);
	return 0;
}

CTubeInfo* CMySamPalteDlg::GetTubeInfoPtr()
{
	return m_pTubeInfo;
}

int CMySamPalteDlg::GetSamInfo(vector<CTubeInfo>& vecSamInfo)
{
	vecSamInfo.clear();
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(NULL == m_pTubeInfo)
	{
		return -3;
	}
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		vecSamInfo.push_back(m_pTubeInfo[i]);
	}
	return 0;
}

int CMySamPalteDlg::SetSamInfo(vector<CTubeInfo>& vecSamInfo,BOOL bRedraw /*= FALSE*/)
{
	if(m_iTotalSiteNum != vecSamInfo.size())
	{
		return -1;
	}

	if(m_rowNum < 1)
	{
		return -2;
	}
	if(m_colNum < 1)
	{
		return -3;
	}
	if(NULL == m_pTubeInfo)
	{
		return -4;
	}

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		m_pTubeInfo[i] = vecSamInfo[i];
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMySamPalteDlg::SetTubeInfo(CPtrArray* pTubeAryPtr,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pTubeAryPtr)
	{
		return -1;
	}
	if(m_iTotalSiteNum != pTubeAryPtr->GetSize())
	{
		return -2;
	}

	if(m_rowNum < 1)
	{
		return -3;
	}
	if(m_colNum < 1)
	{
		return -4;
	}
	if(NULL == m_pTubeInfo)
	{
		return -5;
	}

	CTubeInfo* pTubeInfo = NULL;
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		pTubeInfo = (CTubeInfo*)pTubeAryPtr->GetAt(i);
		if(NULL == pTubeInfo)
		{
			continue;
		}
		m_pTubeInfo[i] = *pTubeInfo;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMySamPalteDlg::SetTubeInfo(CTubeInfo* pTubeInfo,int iSiteNum,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pTubeInfo)
	{
		return -1;
	}
	if(iSiteNum < 1)
	{
		return -2;
	}
	if(NULL == m_pTubeInfo)
	{
		return -3;
	}
	if(m_iTotalSiteNum < 1)
	{
		return -4;
	}

	int iTempNum = iSiteNum;
	if(iSiteNum > m_iTotalSiteNum)
	{
		iTempNum = m_iTotalSiteNum;
	}

	for(int i = 0;i < iTempNum;++i)
	{
		m_pTubeInfo[i] = pTubeInfo[i];
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMySamPalteDlg::SetTubeInfo(vector<CTubeInfo*>& vecPTubeInfo,BOOL bRedraw /*= FALSE*/)
{
	int iTempNum = vecPTubeInfo.size();
	if(iTempNum < 1)
	{
		return -1;
	}

	if(NULL == m_pTubeInfo)
	{
		return -2;
	}
	if(m_iTotalSiteNum < 1)
	{
		return -3;
	}

	if(iTempNum > m_iTotalSiteNum)
	{
		iTempNum = m_iTotalSiteNum;
	}

	for(int i = 0;i < iTempNum;++i)
	{
		if(NULL != vecPTubeInfo[i])
		{
			m_pTubeInfo[i] = *(vecPTubeInfo[i]);
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMySamPalteDlg::SetSamInfo(int iSiteIndex,CTubeInfo& samInfo,BOOL bRedraw /*= FALSE*/)
{
	if(iSiteIndex < 0)
	{
		return -1;
	}
	if(iSiteIndex >= m_iTotalSiteNum)
	{
		return -2;
	}
	if(m_rowNum < 1)
	{
		return -3;
	}
	if(m_colNum < 1)
	{
		return -4;
	}
	if(NULL == m_pTubeInfo)
	{
		return -5;
	}
	m_pTubeInfo[iSiteIndex] = samInfo;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMySamPalteDlg::SetSamInfo(int iRowId,int iColId,CTubeInfo& samInfo,BOOL bRedraw /*= FALSE*/)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(iRowId < 0)
	{
		return -3;
	}
	if(iRowId >= m_rowNum)
	{
		return -4;
	}
	if(iColId < 0)
	{
		return -5;
	}
	if(iColId >= m_colNum)
	{
		return -6;
	}
	if(NULL == m_pTubeInfo)
	{
		return -7;
	}
	m_pTubeInfo[iRowId * m_colNum + iColId] = samInfo;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMySamPalteDlg::SetSamInfo(int iSiteIndex,EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	if(iSiteIndex < 0)
	{
		return -1;
	}
	if(iSiteIndex >= m_iTotalSiteNum)
	{
		return -2;
	}
	if(m_rowNum < 1)
	{
		return -3;
	}
	if(m_colNum < 1)
	{
		return -4;
	}
	if(NULL == m_pTubeInfo)
	{
		return -5;
	}

	CTubeInfo& samInfo = m_pTubeInfo[iSiteIndex];

	switch(eItemInfoType)
	{
	case ITEMHOLEINFO: samInfo.SetTubeLabel(strData); break;
	case ITEMSAMNAME: samInfo.SetSampleName(strData); break;
	case ITEMSAMTYPE: samInfo.SetSampleType(TextToSampleType(strData)); break;
	//case ITEMSTDDENSITY: samInfo.m_dDensity = dData; break;
	case ITEMREPETITIVETUBE: samInfo.SetIsRepeatTube(intData); break;
	case ITEMSAMID: samInfo.SetSampleID(strData); break;
	case ITEMPATIENTNAME: samInfo.m_strPatientName = strData; break;
	case ITEMSEXUALITY: samInfo.m_nPatientSex = (ePatientSexuality)intData; break;
	case ITEMAGE: samInfo.m_nPatientAge = intData; break;
	case ITEMSAMCOLLECTDATE: samInfo.m_strCollectDate = strData; break;
	case ITEMSENDINGDOCTOR: samInfo.m_strProposer = strData; break;
	case ITEMSENDINGSECTION: samInfo.m_strDepartment = strData; break;
	case ITEMANAMNESISNO: samInfo.m_strCaseID = strData; break;
	case ITEMBEDNO: samInfo.m_strBedNo = strData; break;
	case ITEMHOSPITALID: samInfo.m_strHospital = strData; break;
	case ITEMOUTPATIENTNO: samInfo.m_strClinicID = strData; break;
	case ITEMREMARKS: samInfo.SetComments(strData); break;
	case ITEM_REPETITIVETUBETAG:	break;

	default:	break;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return 0;
}

int CMySamPalteDlg::SetSamInfo(int iRowId,int iColId,EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/,BOOL bRedraw /*= FALSE*/)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(iRowId < 0)
	{
		return -3;
	}
	if(iRowId >= m_rowNum)
	{
		return -4;
	}
	if(iColId < 0)
	{
		return -5;
	}
	if(iColId >= m_colNum)
	{
		return -6;
	}
	if(NULL == m_pTubeInfo)
	{
		return -7;
	}

	CTubeInfo& tubeInfo = m_pTubeInfo[iRowId * m_colNum + iColId];

	switch(eItemInfoType)
	{
	case ITEMHOLEINFO: tubeInfo.SetTubeLabel(strData); break;
	case ITEMSAMNAME: tubeInfo.SetSampleName(strData); break;
	case ITEMSAMTYPE: tubeInfo.SetSampleType((eSampleType)intData); break;
		//case ITEMSTDDENSITY: tubeInfo.m_dDensity = dData; break;
	case ITEMREPETITIVETUBE: tubeInfo.SetIsRepeatTube(intData); break;
	case ITEMSAMID: tubeInfo.SetSampleID(strData); break;
	case ITEMPATIENTNAME: tubeInfo.m_strPatientName = strData; break;
	case ITEMSEXUALITY: tubeInfo.m_nPatientSex = (ePatientSexuality)intData; break;
	case ITEMAGE: tubeInfo.m_nPatientAge = intData; break;
	case ITEMSAMCOLLECTDATE: tubeInfo.m_strCollectDate = strData; break;
	case ITEMSENDINGDOCTOR: tubeInfo.m_strProposer = strData; break;
	case ITEMSENDINGSECTION: tubeInfo.m_strDepartment = strData; break;
	case ITEMANAMNESISNO: tubeInfo.m_strCaseID = strData; break;
	case ITEMBEDNO: tubeInfo.m_strBedNo = strData; break;
	case ITEMHOSPITALID: tubeInfo.m_strHospital = strData; break;
	case ITEMOUTPATIENTNO: tubeInfo.m_strClinicID = strData; break;
	case ITEMREMARKS: tubeInfo.SetComments(strData); break;

	default:	break;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return 0;
}

int CMySamPalteDlg::SetRectWidth(int iRectW,BOOL bRedraw /*= FALSE*/)
{
	m_iRectW = iRectW;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMySamPalteDlg::SetRectHeight(int iRectH,BOOL bRedraw /*= FALSE*/)
{
	m_iRectH = iRectH;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

void CMySamPalteDlg::SetBeShowTopText(BOOL bShowTopTx,BOOL bRedraw /*= FALSE*/)
{
	m_bShowTopTx = bShowTopTx;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetBeShowTopText()
{
	return m_bShowTopTx;
}

void CMySamPalteDlg::SetBeShowBottomText(BOOL bShowBottomTx,BOOL bRedraw /*= FALSE*/)
{
	m_bShowBottomTx = bShowBottomTx;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetBeShowBottomText()
{
	return m_bShowBottomTx;
}

int CMySamPalteDlg::GetPlateSelectState(vector<BOOL>& vecSelState)
{
	vecSelState.clear();

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		vecSelState.push_back(m_pPlateInfo[i].m_bSelected);
	}
	return 0;
}

int CMySamPalteDlg::GetPlateSelectedSite(vector<BOOL>& vecSelectedSite)
{
	vecSelectedSite.clear();

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		if(m_pPlateInfo[i].m_bSelected)
		{
			vecSelectedSite.push_back(i);
		}
	}
	return 0;
}

void CMySamPalteDlg::SetFourAngleBkColor(COLORREF clrFourAngleBk,BOOL bRedraw /*= FALSE*/)
{
	m_clrFourAngleBk = clrFourAngleBk;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

COLORREF CMySamPalteDlg::GetFourAngleBkColor()
{
	return m_clrFourAngleBk;
}

void CMySamPalteDlg::SetInterval(int iHori,int iVert,BOOL bRedraw /*= FALSE*/)
{
	m_iHoriInterval = iHori;
	m_iVertInterval = iVert;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMySamPalteDlg::GetHoriInterval()
{
	return m_iHoriInterval;
}

int CMySamPalteDlg::GetVertInterval()
{
	return m_iVertInterval;
}

void CMySamPalteDlg::SetBeShieldDeleteKey(BOOL bShieldDeleteKey)
{
	m_bShieldDeleteKey = bShieldDeleteKey;
}

BOOL CMySamPalteDlg::GetBeShieldDeleteKey()
{
	return m_bShieldDeleteKey;
}

void CMySamPalteDlg::SetBeParentProcessDeleteKey(BOOL bParentProcessDeleteKey)
{
	m_bParentProcessDeleteKey = bParentProcessDeleteKey;
}

BOOL CMySamPalteDlg::GetBeParentProcessDeleteKey()
{
	return m_bParentProcessDeleteKey;
}

void CMySamPalteDlg::SetSiteIdShowState(BOOL bShow,BOOL bRedraw /*= FALSE*/)
{
	m_bShowSiteId = bShow;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetSiteIdShowState()
{
	return m_bShowSiteId;
}

void CMySamPalteDlg::SetLastChanBlockShowState(BOOL bShow,BOOL bRedraw /*= FALSE*/)
{
	m_bShowLastChanBlock = bShow;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySamPalteDlg::GetLastChanBlockShowState()
{
	return m_bShowLastChanBlock;
}

int CMySamPalteDlg::DeleteSelectedTubeInfo(BOOL bTemplateMode)
{
	if(m_iTotalSiteNum < 1)
	{
		return -1;
	}

	if(NULL == m_pPlateInfo)
	{
		return -2;
	}

	if(NULL == m_pTubeInfo)
	{
		return -3;
	}

	BOOL bExistSelectedSite = FALSE;
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		if(m_pPlateInfo[i].m_bSelected)
		{
			bExistSelectedSite = TRUE;
			m_pTubeInfo[i].Reset(bTemplateMode);
		}
	}

	if(bExistSelectedSite)
	{
		Invalidate(FALSE);
		if(m_bTellParentSiteSelChange)
		{
			CWnd* pParent = GetParent();
			if(NULL != pParent->GetSafeHwnd())	//������Ϣ�ı䣬֪ͨ������
			{
				pParent->PostMessage(WM_TELLPARENTSITESELECTCHANGED,0,0);
			}   
		}
	}

	return 0;
}

int CMySamPalteDlg::DeleteTubeInfo(vector<int> vecId,BOOL bTemplateMode)
{
	if(m_iTotalSiteNum < 1)
	{
		return -1;
	}

	if(NULL == m_pPlateInfo)
	{
		return -2;
	}

	if(NULL == m_pTubeInfo)
	{
		return -3;
	}

	if(vecId.size() < 1)
	{
		return -4;
	}

	for(int i = 0;i < vecId.size();++i)
	{
		if(vecId[i] < m_iTotalSiteNum)
		{
			m_pTubeInfo[vecId[i]].Reset(bTemplateMode);
		}
	}

	Invalidate(FALSE);
	if(m_bTellParentSiteSelChange)
	{
		CWnd* pParent = GetParent();
		if(NULL != pParent->GetSafeHwnd())	//������Ϣ�ı䣬֪ͨ������
		{
			pParent->PostMessage(WM_TELLPARENTSITESELECTCHANGED,0,0);
		}   
	}

	return 0;
}

int CMySamPalteDlg::DeleteTubeInfo(vector<int> vecId,EDELETETYPE eDeletType,BOOL bTemplateMode)
{
	if(m_iTotalSiteNum < 1)
	{
		return -1;
	}

	if(NULL == m_pPlateInfo)
	{
		return -2;
	}

	if(NULL == m_pTubeInfo)
	{
		return -3;
	}

	if(vecId.size() < 1)
	{
		return -4;
	}

	for(int i = 0;i < vecId.size();++i)
	{
		if(vecId[i] < m_iTotalSiteNum)
		{
			if(EDT_DELETEALL == eDeletType)
			{
				m_pTubeInfo[vecId[i]].Reset(bTemplateMode);
			}
			else if(EDT_DELETE1 == eDeletType)
			{
				m_pTubeInfo[vecId[i]].ResetInfo1(bTemplateMode);
			}
			else if(EDT_DELETE2 == eDeletType)
			{
				m_pTubeInfo[vecId[i]].ResetInfo2(bTemplateMode);
			}
		}
	}

	Invalidate(FALSE);
	if(m_bTellParentSiteSelChange)
	{
		CWnd* pParent = GetParent();
		if(NULL != pParent->GetSafeHwnd())	//������Ϣ�ı䣬֪ͨ������
		{
			pParent->PostMessage(WM_TELLPARENTSITESELECTCHANGED,0,0);
		}   
	}

	return 0;
}

int CMySamPalteDlg::DeleteAllTubeInfo()
{
	if(m_iTotalSiteNum < 1)
	{
		return -1;
	}

	if(NULL == m_pPlateInfo)
	{
		return -2;
	}

	if(NULL == m_pTubeInfo)
	{
		return -3;
	}

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		m_pTubeInfo[i].Reset(FALSE);
	}

	SetAllSelectState(FALSE,TRUE);

	return 0;
}

int CMySamPalteDlg::GetSameTubeInfo(CTubeInfo& tubeInfo)
{
	if(NULL == m_pTubeInfo)
	{
		return -1;
	}

	vector<int> vecSelectedSite;
	GetPlateSelectedSite(vecSelectedSite);
	if(vecSelectedSite.size() < 1)
	{
		return 1;
	}

	CString strTemp(_T(""));
	COLORREF clrTemp = 0;
	int iTemp = 0;
	eSampleType eTemp = SAMPLE_TYPE_NONE;
	BOOL bFind = FALSE;

	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetTubeFlag();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetTubeFlag())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetTubeFlag(strTemp);
	}
	
	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetSampleID();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetSampleID())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetSampleID(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetSampleName();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetSampleName())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetSampleName(strTemp);
	}

	bFind = FALSE;
	clrTemp = m_pTubeInfo[vecSelectedSite[0]].GetSampleColor();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(clrTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetSampleColor())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetSampleColor(clrTemp);
	}

	eTemp = m_pTubeInfo[vecSelectedSite[0]].GetSampleType();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(eTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetSampleType())
		{
			eTemp = SAMPLE_TYPE_MULLTI_DIFFERENT;
			break;
		}
	}
	tubeInfo.SetSampleType(eTemp);

	bFind = FALSE;
	iTemp = m_pTubeInfo[vecSelectedSite[0]].GetIsRepeatTube();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(iTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetIsRepeatTube())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetIsRepeatTube(iTemp);
	}

	bFind = FALSE;
	iTemp = m_pTubeInfo[vecSelectedSite[0]].GetRepeatNo();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(iTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetRepeatNo())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetRepeatNo(iTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetComments();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetComments())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetComments(strTemp);
	}

	bFind = FALSE;
	iTemp = m_pTubeInfo[vecSelectedSite[0]].GetCalibrateChannel();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(iTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetCalibrateChannel())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetCalibrateChannel(iTemp);
	}

	bFind = FALSE;
	int iDyeTargetNum = m_pTubeInfo[vecSelectedSite[0]].GetTargetCount();
	if(iDyeTargetNum > 0)
	{
		tubeInfo.InitDyeChannel(iDyeTargetNum);
		for(int i = 1;i < vecSelectedSite.size();++i)
		{
			if(iDyeTargetNum != (m_pTubeInfo[vecSelectedSite[i]]).GetTargetCount())
			{
				bFind = TRUE;
				break;
			}
		}
	}
	else
	{
		bFind = TRUE;
	}

	if(!bFind)
	{
		BOOL* pBFind = new BOOL[iDyeTargetNum];
		if(NULL != pBFind)
		{
			for(int j = 0;j < iDyeTargetNum;++j)
			{
				pBFind[j] = FALSE;
				iTemp = m_pTubeInfo[vecSelectedSite[0]].GetSampleChannelSelectState(j);
				for(int i = 1;i < vecSelectedSite.size();++i)
				{
					if(iTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetSampleChannelSelectState(j))
					{
						pBFind[j] = TRUE;
						break;
					}
				}
				if(!pBFind[j])
				{
					tubeInfo.SetSampleChannelSelectState(j,iTemp);
				}

				pBFind[j] = FALSE;
				strTemp = m_pTubeInfo[vecSelectedSite[0]].GetDyeName(j);
				for(int i = 1;i < vecSelectedSite.size();++i)
				{
					if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetDyeName(j))
					{
						pBFind[j] = TRUE;
						break;
					}
				}
				if(!pBFind[j])
				{
					tubeInfo.SetDyeName(j,strTemp);
				}

				pBFind[j] = FALSE;
				strTemp = m_pTubeInfo[vecSelectedSite[0]].GetTargetName(j);
				for(int i = 1;i < vecSelectedSite.size();++i)
				{
					if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetTargetName(j))
					{
						pBFind[j] = TRUE;
						break;
					}
				}
				if(!pBFind[j])
				{
					tubeInfo.SetTargetName(j,strTemp);
				}

				pBFind[j] = FALSE;
				clrTemp = m_pTubeInfo[vecSelectedSite[0]].GetSampleDyeTargetColor(j);
				for(int i = 1;i < vecSelectedSite.size();++i)
				{
					if(clrTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetSampleDyeTargetColor(j))
					{
						pBFind[j] = TRUE;
						break;
					}
				}
				if(!pBFind[j])
				{
					tubeInfo.SetSampleDyeTargetColor(j,clrTemp);
				}
			}
			delete [] pBFind;
		}
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetPatientID();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetPatientID())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetPaitentID(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetClinicID();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetClinicID())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetClinicID(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetCaseID();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetCaseID())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetCaseID(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetPatientName();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetPatientName())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetPatientName(strTemp);
	}

	bFind = FALSE;
	ePatientSexuality eSex = SEXUALITY_NONE;
	eSex = m_pTubeInfo[vecSelectedSite[0]].GetSexuality();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(eSex != (m_pTubeInfo[vecSelectedSite[i]]).GetSexuality())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetSexuality(eSex);
	}

	bFind = FALSE;
	iTemp = m_pTubeInfo[vecSelectedSite[0]].GetPaitentAge();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(iTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetPaitentAge())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetPaitentAge(iTemp);
	}

	bFind = FALSE;
	ePatientAgeUnit eAgeUnit = AGE_UNIT_YEAR;
	eAgeUnit = m_pTubeInfo[vecSelectedSite[0]].GetAgeUnit();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(eAgeUnit != (m_pTubeInfo[vecSelectedSite[i]]).GetAgeUnit())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetAgeUnit(eAgeUnit);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetAreaNo();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetAreaNo())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetAreaNo(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetRoomNo();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetRoomNo())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetRoomNo(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetBedNo();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetBedNo())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetBedNo(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetProposer();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetProposer())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetProposer(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetDepartment();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetDepartment())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetDepartment(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetHospital();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetHospital())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetHospital(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetCollectDate();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetCollectDate())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetCollectDate(strTemp);
	}

	bFind = FALSE;
	strTemp = m_pTubeInfo[vecSelectedSite[0]].GetApplyDate();
	for(int i = 1;i < vecSelectedSite.size();++i)
	{
		if(strTemp != (m_pTubeInfo[vecSelectedSite[i]]).GetApplyDate())
		{
			bFind = TRUE;
			break;
		}
	}
	if(!bFind)
	{
		tubeInfo.SetApplyDate(strTemp);
	}

	return 0;
}

int CMySamPalteDlg::SetAllTubeChanInfo(int iChanId,BOOL bSelected,CString strDyeName,CString strTargetName)
{
	if(iChanId < 0)
	{
		return -1;
	}

	if(NULL == m_pTubeInfo)
	{
		return -2;
	}

	int iChanNum = 0;
	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		pTubeInfo = &m_pTubeInfo[i];
		if(NULL == pTubeInfo)
		{
			continue;
		}

		iChanNum = pTubeInfo->GetTargetCount();
		if(iChanId >= iChanNum)
		{
			continue;
		}

		pDyeTargetInfo = pTubeInfo->GetTargetInfo(iChanId);
		if(NULL == pDyeTargetInfo)
		{
			continue;
		}

		pDyeTargetInfo->SetBeSelected(bSelected);
		pDyeTargetInfo->SetDyeName(strDyeName);
		pDyeTargetInfo->SetTargetName(strTargetName);
	}

	return 0;
}

CString CMySamPalteDlg::GetOneChannelDyeNameFromTubeInfo(int iChanId)
{
	CString strDyeName(_T(""));

	if(m_iTotalSiteNum < 1 || NULL == m_pTubeInfo)
	{
		return strDyeName;
	}

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		strDyeName = m_pTubeInfo[i].GetDyeName(iChanId);
		if(!strDyeName.IsEmpty())
		{
			break;
		}
	}

	return strDyeName;
}

int CMySamPalteDlg::GetAllSampleType(vector<eSampleType>& vecSamType)
{
	vecSamType.clear();

	if(m_iTotalSiteNum < 1)
	{
		return -1;
	}

	if(NULL == m_pTubeInfo)
	{
		return -2;
	}

	for(int i = 0;i < m_iTotalSiteNum;++i)
	{
		vecSamType.push_back(m_pTubeInfo[i].GetSampleType());
	}

	return 0;
}

CString CMySamPalteDlg::SampleTypeToText(eSampleType eType)
{
	map<eSampleType,CString>::iterator iter = m_mapSamTypeToTx.find(eType);
	if(m_mapSamTypeToTx.end() == iter)
	{
		return _T("");
	}

	return iter->second;
}

eSampleType CMySamPalteDlg::TextToSampleType(CString strText)
{
	map<CString,eSampleType>::iterator iter = m_mapTxToSamType.find(strText);
	if(m_mapTxToSamType.end() == iter)
	{
		return SAMPLE_TYPE_NONE;
	}

	return iter->second;
}

int CMySamPalteDlg::InitMapSampleTypeToText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_mapSamTypeToTx[SAMPLE_TYPE_NONE] = pLangInfo->GetText(292);
	m_mapSamTypeToTx[SAMPLE_TYPE_UNKNOWN] = pLangInfo->GetText(211);
	m_mapSamTypeToTx[SAMPLE_TYPE_STANDARD] = pLangInfo->GetText(212);
	m_mapSamTypeToTx[SAMPLE_TYPE_NTC] = pLangInfo->GetText(213);
	m_mapSamTypeToTx[SAMPLE_TYPE_NEGATIVE] = pLangInfo->GetText(214);
	m_mapSamTypeToTx[SAMPLE_TYPE_POSITIVE] = pLangInfo->GetText(215);
	m_mapSamTypeToTx[SAMPLE_TYPE_POS_CRITICAL] = pLangInfo->GetText(216);
	m_mapSamTypeToTx[SAMPLE_TYPE_CALIBRATOR] = pLangInfo->GetText(217);
	m_mapSamTypeToTx[SAMPLE_TYPE_BUFFER] = pLangInfo->GetText(218);
	m_mapSamTypeToTx[SAMPLE_TYPE_IC] = pLangInfo->GetText(457);

	return 0;
}

int CMySamPalteDlg::InitMapTextToSampleType() 
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_mapTxToSamType[pLangInfo->GetText(292,LANGUAGE_CHS)] = SAMPLE_TYPE_NONE;
	m_mapTxToSamType[pLangInfo->GetText(211,LANGUAGE_CHS)] = SAMPLE_TYPE_UNKNOWN;
	m_mapTxToSamType[pLangInfo->GetText(212,LANGUAGE_CHS)] = SAMPLE_TYPE_STANDARD;
	m_mapTxToSamType[pLangInfo->GetText(213,LANGUAGE_CHS)] = SAMPLE_TYPE_NTC;
	m_mapTxToSamType[pLangInfo->GetText(214,LANGUAGE_CHS)] = SAMPLE_TYPE_NEGATIVE;
	m_mapTxToSamType[pLangInfo->GetText(215,LANGUAGE_CHS)] = SAMPLE_TYPE_POSITIVE;
	m_mapTxToSamType[pLangInfo->GetText(216,LANGUAGE_CHS)] = SAMPLE_TYPE_POS_CRITICAL;
	m_mapTxToSamType[pLangInfo->GetText(217,LANGUAGE_CHS)] = SAMPLE_TYPE_CALIBRATOR;
	m_mapTxToSamType[pLangInfo->GetText(218,LANGUAGE_CHS)] = SAMPLE_TYPE_BUFFER;
	m_mapTxToSamType[pLangInfo->GetText(457,LANGUAGE_CHS)] = SAMPLE_TYPE_IC;	

	m_mapTxToSamType[pLangInfo->GetText(292,LANGUAGE_ENG)] = SAMPLE_TYPE_NONE;
	m_mapTxToSamType[pLangInfo->GetText(211,LANGUAGE_ENG)] = SAMPLE_TYPE_UNKNOWN;
	m_mapTxToSamType[pLangInfo->GetText(212,LANGUAGE_ENG)] = SAMPLE_TYPE_STANDARD;
	m_mapTxToSamType[pLangInfo->GetText(213,LANGUAGE_ENG)] = SAMPLE_TYPE_NTC;
	m_mapTxToSamType[pLangInfo->GetText(214,LANGUAGE_ENG)] = SAMPLE_TYPE_NEGATIVE;
	m_mapTxToSamType[pLangInfo->GetText(215,LANGUAGE_ENG)] = SAMPLE_TYPE_POSITIVE;
	m_mapTxToSamType[pLangInfo->GetText(216,LANGUAGE_ENG)] = SAMPLE_TYPE_POS_CRITICAL;
	m_mapTxToSamType[pLangInfo->GetText(217,LANGUAGE_ENG)] = SAMPLE_TYPE_CALIBRATOR;
	m_mapTxToSamType[pLangInfo->GetText(218,LANGUAGE_ENG)] = SAMPLE_TYPE_BUFFER;	
	m_mapTxToSamType[pLangInfo->GetText(457,LANGUAGE_ENG)] = SAMPLE_TYPE_IC;	

	return 0;
}

int CMySamPalteDlg::InitMapSamTypeToHoleClrInfo()
{
	int iAlpha = 255 * 0.2;

	CHoleColorInfo holeClrInfo;

#ifdef N16USENEWSAMPLETYPE
	holeClrInfo.m_clrFrame = Color(0XC6,0XC6,0XC6);
	holeClrInfo.m_clrFill = Color(255 * 0.3,0XC6,0XC6,0XC6);
	holeClrInfo.m_clrTxUnSelected = RGB(0X66,0X66,0X66);
	holeClrInfo.m_clrTxSelected = RGB(0X66,0X66,0X66);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_NONE] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0X9B,0X51,0XE0);
	holeClrInfo.m_clrFill = Color(iAlpha,0X9B,0X51,0XE0);
	holeClrInfo.m_clrTxUnSelected = RGB(0X9B,0X51,0XE0);
	holeClrInfo.m_clrTxSelected = RGB(0X9B,0X51,0XE0);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_UNKNOWN] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0X21,0X96,0X53);
	holeClrInfo.m_clrFill = Color(iAlpha,0X21,0X96,0X53);
	holeClrInfo.m_clrTxUnSelected = RGB(0X21,0X96,0X53);
	holeClrInfo.m_clrTxSelected = RGB(0X21,0X96,0X53);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_STANDARD] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0XEB,0X57,0X57);
	holeClrInfo.m_clrFill = Color(iAlpha,0XEB,0X57,0X57);
	holeClrInfo.m_clrTxUnSelected = RGB(0XEB,0X57,0X57);
	holeClrInfo.m_clrTxSelected = RGB(0XEB,0X57,0X57);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_POSITIVE] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0X2F,0X80,0XED);
	holeClrInfo.m_clrFill = Color(iAlpha,0X2F,0X80,0XED);
	holeClrInfo.m_clrTxUnSelected = RGB(0X2F,0X80,0XED);
	holeClrInfo.m_clrTxSelected = RGB(0X2F,0X80,0XED);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_NEGATIVE] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0XF2,0X99,0X4A);
	holeClrInfo.m_clrFill = Color(iAlpha,0XF2,0X99,0X4A);
	holeClrInfo.m_clrTxUnSelected = RGB(0XF2,0X99,0X4A);
	holeClrInfo.m_clrTxSelected = RGB(0XF2,0X99,0X4A);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_NTC] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0XF2,0X31,0X31);
	holeClrInfo.m_clrFill = Color(iAlpha,0XF2,0X31,0X31);
	holeClrInfo.m_clrTxUnSelected = RGB(0XF2,0X31,0X31);
	holeClrInfo.m_clrTxSelected = RGB(0XF2,0X31,0X31);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_POS_CRITICAL] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0XF0,0X31,0XF2);
	holeClrInfo.m_clrFill = Color(iAlpha,0XF0,0X31,0XF2);
	holeClrInfo.m_clrTxUnSelected = RGB(0XF0,0X31,0XF2);
	holeClrInfo.m_clrTxSelected = RGB(0XF0,0X31,0XF2);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_CALIBRATOR] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0,0X70,0XC0);
	holeClrInfo.m_clrFill = Color(iAlpha,0,0X70,0XC0);
	holeClrInfo.m_clrTxUnSelected = RGB(0,0X70,0XC0);
	holeClrInfo.m_clrTxSelected = RGB(0,0X70,0XC0);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_BUFFER] = holeClrInfo;

	// �ڲ��ʿ���ʱʹ�û���Һ����ɫ
	holeClrInfo.m_clrFrame = Color(0,0X70,0XC0);
	holeClrInfo.m_clrFill = Color(iAlpha,0,0X70,0XC0);
	holeClrInfo.m_clrTxUnSelected = RGB(0,0X70,0XC0);
	holeClrInfo.m_clrTxSelected = RGB(0,0X70,0XC0);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_IC] = holeClrInfo;

#else
	holeClrInfo.m_clrFrame = Color(0XC6,0XC6,0XC6);
	holeClrInfo.m_clrFill = Color(255 * 0.3,0XC6,0XC6,0XC6);
	holeClrInfo.m_clrTxUnSelected = RGB(0X66,0X66,0X66);
	holeClrInfo.m_clrTxSelected = RGB(0X66,0X66,0X66);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_NONE] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0X20,0X44,0X7C);
	holeClrInfo.m_clrFill = Color(iAlpha,0X39,0X45,0X65);
	holeClrInfo.m_clrTxUnSelected = RGB(0X20,0X44,0X7C);
	holeClrInfo.m_clrTxSelected = RGB(iAlpha,0X39,0X45,0X65);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_UNKNOWN] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0X20,0X9F,0X85);
	holeClrInfo.m_clrFill = Color(iAlpha,0X20,0X9F,0X85);
	holeClrInfo.m_clrTxUnSelected = RGB(0X20,0X9F,0X85);
	holeClrInfo.m_clrTxSelected = RGB(0X20,0X9F,0X85);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_STANDARD] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0X33,0X83,0XFD);
	holeClrInfo.m_clrFill = Color(iAlpha,0X33,0X83,0XFD);
	holeClrInfo.m_clrTxUnSelected = RGB(0X33,0X83,0XFD);
	holeClrInfo.m_clrTxSelected = RGB(0X33,0X83,0XFD);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_NTC] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0X24,0XB7,0XEA);
	holeClrInfo.m_clrFill = Color(iAlpha,0X24,0XB7,0XEA);
	holeClrInfo.m_clrTxUnSelected = RGB(0X24,0XB7,0XEA);
	holeClrInfo.m_clrTxSelected = RGB(0X24,0XB7,0XEA);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_NEGATIVE] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0XC5,0X64,0X62);
	holeClrInfo.m_clrFill = Color(iAlpha,0XC5,0X64,0X62);
	holeClrInfo.m_clrTxUnSelected = RGB(0XC5,0X64,0X62);
	holeClrInfo.m_clrTxSelected = RGB(0XC5,0X64,0X62);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_POSITIVE] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0XF2,0X31,0X31);
	holeClrInfo.m_clrFill = Color(iAlpha,0XF2,0X31,0X31);
	holeClrInfo.m_clrTxUnSelected = RGB(0XF2,0X31,0X31);
	holeClrInfo.m_clrTxSelected = RGB(0XF2,0X31,0X31);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_POS_CRITICAL] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0XF0,0X31,0XF2);
	holeClrInfo.m_clrFill = Color(iAlpha,0XF0,0X31,0XF2);
	holeClrInfo.m_clrTxUnSelected = RGB(0XF0,0X31,0XF2);
	holeClrInfo.m_clrTxSelected = RGB(0XF0,0X31,0XF2);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_CALIBRATOR] = holeClrInfo;

	holeClrInfo.m_clrFrame = Color(0,0X70,0XC0);
	holeClrInfo.m_clrFill = Color(iAlpha,0,0X70,0XC0);
	holeClrInfo.m_clrTxUnSelected = RGB(0,0X70,0XC0);
	holeClrInfo.m_clrTxSelected = RGB(0,0X70,0XC0);
	m_mapSamTypeToHoleClrInfo[SAMPLE_TYPE_BUFFER] = holeClrInfo;
#endif

	return 0;
}

int CMySamPalteDlg::InitMapSamTypeToAbbr()
{
	m_mapSamTypeToAbbr[SAMPLE_TYPE_UNKNOWN] = _T("UN");
	m_mapSamTypeToAbbr[SAMPLE_TYPE_STANDARD] = _T("ST");
	m_mapSamTypeToAbbr[SAMPLE_TYPE_POSITIVE] = _T("PC");
	m_mapSamTypeToAbbr[SAMPLE_TYPE_NEGATIVE] = _T("NC");
	m_mapSamTypeToAbbr[SAMPLE_TYPE_NTC] = _T("NT");
	m_mapSamTypeToAbbr[SAMPLE_TYPE_IC] = _T("IC");

	return 0;
}

CString CMySamPalteDlg::GetSampleTypeAbbr(eSampleType eType)
{
	map<eSampleType,CString>::iterator iter = m_mapSamTypeToAbbr.find(eType);
	if(m_mapSamTypeToAbbr.end() == iter)
	{
		return _T("");
	}

	return iter->second;
}

int CMySamPalteDlg::CalcTotalColumnWidth()
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

int CMySamPalteDlg::CalcTotalRowHeight()
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

int CMySamPalteDlg::CalcRowColToShow()
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

int CMySamPalteDlg::CalcRowColToShowOnNoRowData()
{
	if(m_rcBody.Height() < 1)
	{
		return -1;
	}

	if(m_pxRowH <= 0)
	{
		return -2;
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

int CMySamPalteDlg::CalcColToShow(int newStartCol)
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

				//��ֹ������ѭ��,20211122,������
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

int CMySamPalteDlg::CalcRowToShow(int newStartRow)
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

				//��ֹ������ѭ��,20211122,������
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

int CMySamPalteDlg::ScrollCol(int pxHoriShift)
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
						//=��������������Ҫ,20211122,������
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

int CMySamPalteDlg::ScrollRow(int pxVertShift)
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
						//=��������������Ҫ,20211122,������
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

int CMySamPalteDlg::ScrollColToFixPos(int pxFixPos)
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

int CMySamPalteDlg::ScrollRowToFixPos(int pxFixPos)
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

void CMySamPalteDlg::CalcScrollPos()
{
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
		m_rcColHeader.right = m_rcLst.right - m_pxFrame - m_vertScroll.m_scrollW;
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
		m_rcColHeader.right = m_rcLst.right - m_pxFrame;
	}

	//��ֹˮƽ��������Ϣ���ô���20211120��������
	if(m_horiScroll.m_bScrollNeedShow)
	{
		m_horiScroll.m_pixelTotal = m_totalColW;
		m_horiScroll.m_pixelForShow = m_rcBody.Width();
		m_horiScroll.CalcSliderPos();
	}
	else
	{
		m_horiScroll.m_rcScroll = CRect(0,0,0,0);
	}
	
	if(m_vertScroll.m_bScrollNeedShow)
	{
		m_vertScroll.m_pixelTotal = m_totalRowH;
		m_vertScroll.m_pixelForShow = m_rcBody.Height();
		m_vertScroll.CalcSliderPos();
	}
	else
	{
		m_vertScroll.m_rcScroll = CRect(0,0,0,0);
	}

	return;
}

void CMySamPalteDlg::CheckMaxMinSet(CString& strTemp,CMaxMinInfo& maxMinInfo)
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

int CMySamPalteDlg::GetHoleColorInfo(int iRowId,int iColId,CHoleColorInfo& holeClrInfo)
{
	if(m_rowNum < 1)
	{
		return -1;
	}
	if(m_colNum < 1)
	{
		return -2;
	}
	if(iRowId < 0)
	{
		return -3;
	}
	if(iRowId >= m_rowNum)
	{
		return -4;
	}
	if(iColId < 0)
	{
		return -5;
	}
	if(iColId >= m_colNum)
	{
		return -6;
	}
	if(NULL == m_pTubeInfo)
	{
		return -7;
	}

	eSampleType stTemp = m_pTubeInfo[iRowId * m_colNum + iColId].GetSampleType();
	map<eSampleType,CHoleColorInfo>::iterator iter = m_mapSamTypeToHoleClrInfo.find(stTemp);
	if(m_mapSamTypeToHoleClrInfo.end() == iter)
	{
		return -8;
	}

	holeClrInfo = iter->second;
	return 0;
}

void CMySamPalteDlg::SetBeTellParentRowSelChange(BOOL beTellParentSiteSelChange)
{
	m_bTellParentSiteSelChange = beTellParentSiteSelChange;
	return;
}

BOOL CMySamPalteDlg::GetBeTellParentRowSelChange()
{
	return m_bTellParentSiteSelChange;
}

void CMySamPalteDlg::InitFontHeader(int lfHeight)
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
	_tcscpy(m_logFontHeader.lfFaceName,  _T("΢���ź�"));

	m_fontSetHeader.CreateFontIndirect(&m_logFontHeader);  
}

void CMySamPalteDlg::SetMyHeaderFont(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/) //nHeight��λΪ ������1/72Ӣ��
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

void CMySamPalteDlg::SetMyHeaderFont() 
{
	m_fontSetHeader.DeleteObject();
	m_fontSetHeader.CreateFontIndirect(&m_logFontHeader);
}

void CMySamPalteDlg::InitFontTitle(int lfHeight)
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
	_tcscpy(m_logFontTitle.lfFaceName, _T("΢���ź�"));

	m_fontSetTitle.CreateFontIndirect(&m_logFontTitle);  
}

void CMySamPalteDlg::SetMyTitleFont(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/)
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

void CMySamPalteDlg::SetMyTitleFont()
{
	m_fontSetTitle.DeleteObject();
	m_fontSetTitle.CreateFontIndirect(&m_logFontTitle);
}

void CMySamPalteDlg::InitFontAbbr(int lfHeight)
{
	m_logFontAbbr.lfHeight = lfHeight;
	m_logFontAbbr.lfWidth = 0;
	m_logFontAbbr.lfEscapement = 0;
	m_logFontAbbr.lfOrientation = 400;
	m_logFontAbbr.lfWeight = FW_NORMAL;
	m_logFontAbbr.lfItalic = FALSE;
	m_logFontAbbr.lfUnderline = FALSE;
	m_logFontAbbr.lfStrikeOut = 0;
	m_logFontAbbr.lfCharSet = ANSI_CHARSET;
	m_logFontAbbr.lfOutPrecision = OUT_DEFAULT_PRECIS;
	m_logFontAbbr.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	m_logFontAbbr.lfQuality = PROOF_QUALITY;
	m_logFontAbbr.lfPitchAndFamily = VARIABLE_PITCH | FF_SCRIPT;
	_tcscpy(m_logFontAbbr.lfFaceName, _T("΢���ź�"));

	m_fontAbbr.CreateFontIndirect(&m_logFontAbbr);  
}

void CMySamPalteDlg::SetMyFontAbbr(CString fontName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/)
{
	if(TYPE_POUND == fontUnitType)
	{
		m_logFontAbbr.lfHeight = (double(nHeight * fontSetDesDpi))/ 72;
	}
	else
	{
		m_logFontAbbr.lfHeight = nHeight;
	}

	_tcscpy(m_logFontAbbr.lfFaceName,fontName);
	m_logFontAbbr.lfWeight = nWeight;
	m_fontAbbr.DeleteObject();
	m_fontAbbr.CreateFontIndirect(&m_logFontAbbr);
}

void CMySamPalteDlg::SetMyFontAbbr()
{
	m_fontAbbr.DeleteObject();
	m_fontAbbr.CreateFontIndirect(&m_logFontAbbr);
}


// PcrSegmentExpWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "MySegDlg.h"
#include "MySegEditDlg.h"
#include "MyStepEditDlg.h"

// CMySegDlg

IMPLEMENT_DYNAMIC(CMySegDlg, CDialogEx)
CMySegDlg::CMySegDlg():m_image(_T(".\\BkPic\\stepicon.png")),m_imgTriangle(_T(".\\BkPic\\triangle.png")),m_imgCamera(_T(".\\BkPic\\camera.png")),m_imageMelt(_T(".\\BkPic\\MeltStepIcon.png")),m_imgAllPtIcon(_T(".\\BkPic\\MeltTriangle.png"))
{
	m_rowNum = 0;
	m_colNum = 0;
	m_colIdStartShow = -1;
	m_colIdEndShow = -1;
	m_rowIdStartShow = -1;
	m_rowIdEndShow = -1;
	m_ssColLeftIndent = 0;
	m_esColRightIndent = 0;
	m_ssRowTopIndent = 0;
	m_esRowBottomIndent = 0;

	m_pxColW = 180;	
	m_pxRowH = 40;	
	m_pxHeaderH = 60;
	m_bUseFixedHW = TRUE;
	
	m_clrGraHeadStart = Color(0XAA,0XAB,0XAC);
	m_clrGraHeadEnd = Color(0XBA,0XBB,0XBC);
	m_bEditable = TRUE;

	m_bShowHoriScroll = FALSE;
	m_bShowVertScroll = FALSE;

	m_totalColW = 0;
	m_totalRowH = 0;
	m_cdcW = 0;
	m_cdcH = 0;

	m_eScrollHVCM = ESHVCM_VERT;

	m_clrHeaderBk = RGB(0XF1,0XF4,0XF9);
	m_clrBodyBk = RGB(0XFF,0XFF,0XFE);
	m_stepWidth = 160;
	m_bMonitorFunc = FALSE;
	m_bEditDataProcessed = FALSE;
	m_visibleStepNum = 12;

	m_roundHeight = 16; 

	m_bSelMoveStart = FALSE;

	m_minTem = 4;
	m_maxTem = 100;
	m_oneTemH = 1;
	m_frameW = 1;

	InitHoriScrollParam();
	InitVertScrollParam();

	m_priorItem = -1;
	m_priorStep = -1;
	m_curRunCycle = -1;
	m_curRunItem = -1;
	m_curRunStep = -1;
	
	m_clrFourAngleBk = RGB(0XFF,0XFF,0XFF);
	m_iRoundAngleSize = 8;
	m_clrIndepent = Color(0XF1,0XF4,0XF6);
	SetScrollArrowWH(4,8);

	m_eCollectFluTagType = ECFTT_CAMERA;

	m_dMaxTem = 100;
	m_dMinTem = 4;
	m_iMaxCycleNum = 999;
	m_iMinCycleNum = 1;
	m_iMaxDelaySec = 3600;
	m_iMinDelaySec = 1;

	m_strSegDefName = _T("Segment");
	m_strStepDefName = _T("Step");

	m_bShieldGradientSet = FALSE;
	m_bShowTemRate = FALSE;

	m_iGradInfoW = 100;
	m_bHaveGrad = FALSE;
	m_bUseGradInfo = TRUE;

	m_strGrad.Empty();
	m_strCol.Empty();
	m_strSeg.Empty();
	m_strStep.Empty();
	m_strRange.Empty();
}

CMySegDlg::~CMySegDlg()
{
	
}

BEGIN_MESSAGE_MAP(CMySegDlg, CDialogEx)
   ON_WM_CREATE()
   ON_WM_ERASEBKGND()
   ON_WM_LBUTTONDBLCLK()
   ON_WM_RBUTTONDOWN()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_MOUSEMOVE()
   ON_WM_MOUSEWHEEL()
   ON_WM_SETCURSOR()
   ON_WM_HSCROLL()
   ON_WM_PAINT()
   ON_WM_SIZE()
   ON_WM_KEYDOWN()
   ON_WM_GETDLGCODE()   
   ON_MESSAGE(WM_FUNCEDITKILLFOCUS,&CMySegDlg::OnMsgFuncEditKillFocus)
   ON_MESSAGE(WM_EDITENTERPUSHED,&CMySegDlg::OnMsgEditEnterPushed)
END_MESSAGE_MAP()

void CMySegDlg::OnPaint()
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
	
	CRect rcTemp(0,0,m_rcLst.Width(),m_rcLst.Height());
	memDC.FillSolidRect(rcTemp.left,rcTemp.top,rcTemp.Width(),rcTemp.Height(),m_clrFourAngleBk);

	DrawRoundRectangleNew(&memDC,rcTemp.left,rcTemp.top,rcTemp.Width() - 1,rcTemp.Height() - 1,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,TRUE,Color(0XD8,0XD8,0XD8),m_frameW,TRUE,m_clrIndepent,1);
	if(IsShowGradientData() && !m_rcGrad.IsRectEmpty())
	{
		DrawRoundRectangleNew(&memDC,m_rcGrad.left,rcTemp.top,rcTemp.Width() - m_rcGrad.left - 1,rcTemp.Height() - 1,0,m_iRoundAngleSize,m_iRoundAngleSize,0,TRUE,Color(0XD8,0XD8,0XD8),m_frameW,TRUE,m_clrIndepent,1);
	}

	memDC.SetBkMode(TRANSPARENT);

	DrawSegment(&memDC);
	DrawHoriScrollStyle2(&memDC);
	DrawVertScrollStyle2(&memDC);
	DrawGradientData(&memDC);

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);   
	
	memDC.SelectObject(pOldBitmap);
	bitMap.DeleteObject();
	memDC.DeleteDC();
	
#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return;
}

int CMySegDlg::DrawSegment(CDC* pDC)
{
#ifdef _DEBUG
	clock_t startMs = clock();
#endif

	if(NULL == pDC)
	{
		return -1;
	}

	CString tempStr(_T(""));
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	int iRealCdcW = m_cdcW;
	if(iRealCdcW < m_rcBody.Width())
	{
		iRealCdcW = m_rcBody.Width();
	}

	int iTempCdcH = m_cdcH;
	if(m_cdcH < m_rcBody.Height())
	{
		iTempCdcH = m_rcBody.Height();
	}

	CDC memDcBody;
	memDcBody.CreateCompatibleDC(pDC);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC,iRealCdcW,iTempCdcH);
	CBitmap* pOldBitmap = memDcBody.SelectObject(&bitMap);
	memDcBody.FillSolidRect(0,0,iRealCdcW,iTempCdcH,RGB(0XFF,0XFF,0XFE));

	CRect rcTemp;
	CFont* pOldFont = memDcBody.SelectObject(&m_fontGeneral.m_fontSet);
	int oldBkMode = memDcBody.SetBkMode(TRANSPARENT);

	Image* pImgCollectTag = &m_imgTriangle;
	if(ECFTT_CAMERA == m_eCollectFluTagType)
	{
		pImgCollectTag = &m_imgCamera;
	}
	m_roundHeight = pImgCollectTag->GetHeight() * m_dVertRatio;

	m_oneTemH = (m_rcBody.Height() - m_image.GetHeight() * m_dVertRatio - pImgCollectTag->GetHeight() * m_dVertRatio) / (m_maxTem - m_minTem);

	memDcBody.FillSolidRect(0,0,iRealCdcW,m_pxHeaderH,m_clrHeaderBk);
	memDcBody.FillSolidRect(0,m_pxHeaderH,iRealCdcW,iTempCdcH - m_pxHeaderH, m_clrBodyBk);
	memDcBody.FillSolidRect(0,m_pxHeaderH - 1,iRealCdcW,1,RGB(0XD8,0XD8,0XD8));

	CSize size; 
	int stepNum = 0,startX = 0,sonInfoH = 25 * m_dVertRatio;
	
	int halfImgH = m_image.GetHeight() * m_dVertRatio / 2 + m_roundHeight;
	if(halfImgH < 0)
	{
		halfImgH = 0;
	}
	float leftTem = GetSegLastStepTem(m_segStepRange.GetSegmentStart(),m_segStepRange.GetStepStart());
	if(leftTem < m_minTem)
	{
		leftTem = m_minTem;
	}
	if(leftTem > m_maxTem)
	{
		leftTem = m_maxTem;
	}
	int lastY = m_cdcH - (leftTem - m_minTem) * m_oneTemH - halfImgH;	

	int leftMargin = 0;
	if(-1 != m_segStepRange.GetStepStart())
	{
		leftMargin = m_segStepRange.GetStepStart() * m_stepWidth + m_ssColLeftIndent;
	}

	Graphics graphics(memDcBody.m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	Pen gdiplusPen(COLOR_2LEVEL_THEME,2);

	BOOL bMeltSeg = FALSE;	

	for(int i = m_segStepRange.GetSegmentStart();i <= m_segStepRange.GetSegmentEnd();++i)
	{
		int stepStart = 0,stepEnd = 0;
		if(i == m_segStepRange.GetSegmentStart())
		{
			stepStart = m_segStepRange.GetStepStart();
		}
		if(i == m_segStepRange.GetSegmentEnd())
		{
			stepEnd = m_segStepRange.GetStepEnd();
		}
		else
		{
			stepEnd = m_lnkSegItemInfo[i]->m_lnkStepItemInfo.GetSize() - 1;
		}

		Node<CSegItemInfo>* pNodeSeg = m_lnkSegItemInfo.GetAt(i);
		if(NULL == pNodeSeg)
		{
			continue;
		}
		CSegItemInfo* pSegItemInfo = m_lnkSegItemInfo[i];
		if(NULL == pSegItemInfo)
		{
			continue;
		}

		bMeltSeg = pSegItemInfo->GetBeMeltSeg();

		stepNum += pSegItemInfo->m_lnkStepItemInfo.GetSize();
		int segWidth = pSegItemInfo->m_lnkStepItemInfo.GetSize() * m_stepWidth;

		//绘制段选中背景
		pSegItemInfo->m_rcSeg.SetRect(startX,0,startX + segWidth,m_pxHeaderH);
		if(pSegItemInfo->m_bSelected)
		{
			memDcBody.FillSolidRect(&pSegItemInfo->m_rcSeg,RGB(0XAE,0XBC,0XD3));
		}

		//绘制段名称
		tempStr.Format(_T("%s"),pSegItemInfo->m_strSegName);
		size = memDcBody.GetTextExtent(tempStr);
		size.cx += 20 * m_dHoriRatio;
		rcTemp.SetRect(startX + (segWidth - size.cx) / 2,1,startX + (segWidth - size.cx) / 2 + size.cx,sonInfoH + 1);
		memDcBody.SelectObject(&m_fontSegTag.m_fontSet);
		memDcBody.SetTextColor(m_segParam.m_clrSegTag);
		memDcBody.DrawText(tempStr,&rcTemp,DT_CENTER | DT_VCENTER | DT_SINGLELINE);																									
		
		pSegItemInfo->m_rcSegName = rcTemp;
		pSegItemInfo->m_rcSegName.InflateRect(leftMargin,0,-leftMargin,0);
		
		pSegItemInfo->m_rcSeg.InflateRect(leftMargin,0,-leftMargin,0);

		//绘制循环数、升降温速率
		if(m_bMonitorFunc)
		{
			if(i == m_curRunItem)
			{
				tempStr.Format(_T("%d/%d"),m_curRunCycle,pSegItemInfo->m_iCycleNum);
			}
			else
			{
				tempStr.Format(_T("%d"),pSegItemInfo->m_iCycleNum);
			}
		}
		else
		{
			tempStr = pLangInfo->GetText(238);
			tempStr.AppendFormat(_T(":%d"),pSegItemInfo->m_iCycleNum);
		}				
		size = memDcBody.GetTextExtent(tempStr);				
		rcTemp.SetRect(startX + (segWidth - size.cx) / 2,sonInfoH,startX + (segWidth - size.cx) / 2 + size.cx,sonInfoH * 2);
		memDcBody.SelectObject(&m_fontGeneral.m_fontSet);
		memDcBody.DrawText(tempStr,&rcTemp,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pSegItemInfo->m_rcCycleNum = CRect(rcTemp.left - leftMargin,rcTemp.top,rcTemp.right - leftMargin,rcTemp.bottom);

		//画垂直分割线
		{
			CPen pen;
			pen.CreatePen(PS_SOLID,1,RGB(0XD8,0XD8,0XD8));
			CPen* pOldPen = memDcBody.SelectObject(&pen);
			memDcBody.MoveTo(startX + segWidth,0);
			memDcBody.LineTo(startX + segWidth,m_cdcH);
			memDcBody.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		for(int j = stepStart;j <= stepEnd;++j)
		{	
			CStepItemInfo* pStepItemInfo = pSegItemInfo->m_lnkStepItemInfo[j];
			if(NULL == pStepItemInfo)
			{
				continue;
			}
			
			int stepLeft = startX + j * m_stepWidth;
			CRect rcStep;
			rcStep = CRect(stepLeft,m_pxHeaderH,stepLeft + m_stepWidth,m_cdcH);

			if(pStepItemInfo->m_bSel)  
			{
				CRect tempRc = rcStep;
				tempRc.DeflateRect(1,1,1,1);
				if(m_bMonitorFunc)
				{
					memDcBody.FillSolidRect(tempRc,RGB(0XE6,0XF4,0XF5));
				}
				else
				{
					memDcBody.FillSolidRect(tempRc,RGB(0XE8,0XF0,0XFF));
				}	
			}

			CPen pen;
			int lineWidth = 1;

			if(!bMeltSeg)
			{
				pen.CreatePen(PS_SOLID,lineWidth,RGB(0XD8,0XD8,0XD8));
				CPen* pOldPen = memDcBody.SelectObject(&pen);
				memDcBody.MoveTo(rcStep.right,rcStep.top);
				memDcBody.LineTo(rcStep.right,rcStep.bottom);
				memDcBody.SelectObject(pOldPen);
				pen.DeleteObject();
			}

			Point ptStart,ptEnd;

			ptStart.X = rcStep.left;
			ptEnd.X = rcStep.left + rcStep.Width() / 2;

			ptEnd.Y = m_cdcH - (pStepItemInfo->m_fAimTem - m_minTem) * m_oneTemH - halfImgH;	
			ptStart.Y = lastY;

			memDcBody.SelectObject(&m_fontGeneral.m_fontSet);
			memDcBody.SetTextColor(m_segParam.m_clrGeneral);

			ImageAttributes ImgAtt;
			ImgAtt.SetWrapMode(WrapModeTileFlipXY);
			int bkW = 0;
			int tempInt = 0;
			if(bMeltSeg && pStepItemInfo->GetBeCollectAll())
			{
				bkW = m_imageMelt.GetWidth();
				graphics.DrawImage(&m_imageMelt,Rect(ptEnd.X,ptEnd.Y - m_imageMelt.GetHeight() / 2,bkW,m_imageMelt.GetHeight()),tempInt,0,bkW,m_imageMelt.GetHeight(),UnitPixel,&ImgAtt);
				tempStr.Format(_T("%.1lf℃"),pStepItemInfo->m_fAimTem);
				memDcBody.DrawText(tempStr,CRect(ptEnd.X,ptEnd.Y - m_imageMelt.GetHeight() / 2,ptEnd.X + bkW,ptEnd.Y),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				pStepItemInfo->m_rcTem = CRect(ptEnd.X - leftMargin,ptEnd.Y - m_imageMelt.GetHeight() / 2,ptEnd.X - leftMargin + bkW,ptEnd.Y);
			}
			else
			{
				bkW = m_image.GetWidth();
				Rect rcIconRect(ptEnd.X,ptEnd.Y - m_image.GetHeight() * m_dVertRatio / 2,bkW * m_dHoriRatio,m_image.GetHeight() * m_dVertRatio); 
				graphics.DrawImage(&m_image,rcIconRect,0,0,bkW,m_image.GetHeight(),UnitPixel,&ImgAtt);

				tempStr.Format(_T("%.1lf℃"),pStepItemInfo->m_fAimTem);
				memDcBody.DrawText(tempStr,CRect(ptEnd.X,ptEnd.Y - m_image.GetHeight() * m_dVertRatio / 2,ptEnd.X + bkW * m_dHoriRatio,ptEnd.Y),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				pStepItemInfo->m_rcTem = CRect(ptEnd.X - leftMargin,ptEnd.Y - m_image.GetHeight() * m_dVertRatio / 2,ptEnd.X - leftMargin + bkW * m_dHoriRatio,ptEnd.Y);

				bkW = pImgCollectTag->GetWidth() / 2;
				if(2 == pStepItemInfo->m_eChkType)
				{
					tempInt = bkW;
				}

				//不采集数据时，相机图标第二种状态不显示
				if(!(ECFTT_CAMERA == m_eCollectFluTagType && pStepItemInfo->GetBeNoneCollect()))
				{
					Rect rcTriangle;
					rcTriangle.X = rcIconRect.X + (m_image.GetWidth() * m_dHoriRatio - bkW * m_dHoriRatio) / 2;
					rcTriangle.Y = ptEnd.Y + m_image.GetHeight() * m_dVertRatio / 2;
					rcTriangle.Width =  bkW * m_dHoriRatio;
					rcTriangle.Height = pImgCollectTag->GetHeight() * m_dVertRatio;
					graphics.DrawImage(pImgCollectTag,rcTriangle,tempInt,0,bkW,pImgCollectTag->GetHeight(),UnitPixel,&ImgAtt);
				}
			}
			pStepItemInfo->m_rcTem.DeflateRect(12 * m_dHoriRatio,2 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio);
			
			if(!pStepItemInfo->GetBeCollectAll())	//熔解步骤不显示延时时间
			{
				//绘制延时时间
				int minute = pStepItemInfo->m_iDelay / 60;
				int sec = pStepItemInfo->m_iDelay % 60;
				tempStr.Format(_T("%02d:%02d"),minute,sec);
				memDcBody.DrawText(tempStr,CRect(ptEnd.X,ptEnd.Y,ptEnd.X + m_image.GetWidth() * m_dHoriRatio,ptEnd.Y + m_image.GetHeight() * m_dVertRatio / 2),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			
			pStepItemInfo->m_rcMinSec = CRect(ptEnd.X - leftMargin,ptEnd.Y,ptEnd.X - leftMargin + m_image.GetWidth() * m_dHoriRatio,ptEnd.Y + m_image.GetHeight() * m_dVertRatio / 2);
			pStepItemInfo->m_rcMinSec.DeflateRect(4 * m_dHoriRatio,4 * m_dVertRatio,4 * m_dHoriRatio,2 * m_dVertRatio);

			int X = ptEnd.X + (m_image.GetWidth() * m_dHoriRatio - pImgCollectTag->GetWidth() * m_dHoriRatio / 2) / 2  - leftMargin;
			int Y = ptEnd.Y + m_image.GetHeight() * m_dVertRatio / 2;
			pStepItemInfo->m_rcCkbtCollectFlu = CRect(X,Y,X + pImgCollectTag->GetWidth() / 2,Y + pImgCollectTag->GetHeight());

			graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);

			//绘制熔解段AllPoint模式图标
			if(pStepItemInfo->GetBeCollectAll())
			{
				Point ptIcon;
				ptIcon.X = (ptEnd.X - ptStart.X) * 2 / 5 + ptStart.X;
				ptIcon.Y = (ptEnd.Y - ptStart.Y) * 2 / 5 + ptStart.Y;
				int iconStartX = m_imgAllPtIcon.GetWidth() / 2;
				int iconW = iconStartX;
				graphics.DrawImage(&m_imgAllPtIcon,Rect(ptIcon.X,ptIcon.Y,iconW,m_imgAllPtIcon.GetHeight()),iconStartX,0,iconW,m_imgAllPtIcon.GetHeight(),UnitPixel,&ImgAtt);	
			}

			ptStart.X = ptEnd.X;
			ptEnd.X = rcStep.right;
			ptStart.Y = ptEnd.Y;
			graphics.DrawLine(&gdiplusPen,ptEnd,ptStart);
			lastY = ptEnd.Y;
			pStepItemInfo->m_rcLine = CRect(ptStart.X - leftMargin,ptStart.Y - 2,ptEnd.X - leftMargin,ptEnd.Y + 2);

			//绘制步骤标识
			if(pStepItemInfo->m_strStepName.IsEmpty())
			{
				tempStr = pLangInfo->GetText(58);
				tempStr.AppendFormat(_T("%d"),j + 1);
				pStepItemInfo->m_strStepName = tempStr;
			}
			else
			{
				tempStr = pStepItemInfo->m_strStepName;
			}
			
			memDcBody.SelectObject(&m_fontStepTag.m_fontSet);
			memDcBody.SetTextColor(m_segParam.m_clrStepTag);
			memDcBody.DrawText(tempStr,CRect(rcStep.left,rcStep.bottom - 20 * m_dVertRatio,rcStep.right,rcStep.bottom - 40 * m_dVertRatio),DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);

			rcStep.left = rcStep.right;			
		}

		startX += segWidth;
	}

	//画水平分割线
	{
		CPen pen;
		pen.CreatePen(PS_SOLID,1,RGB(0XD8,0XD8,0XD8));
		CPen* pOldPen = memDcBody.SelectObject(&pen);
		memDcBody.MoveTo(0,m_pxHeaderH);
		memDcBody.LineTo(iRealCdcW,m_pxHeaderH);

		int iTempInt = iTempCdcH - 40 * m_dVertRatio;
		memDcBody.MoveTo(0,iTempInt);
		memDcBody.LineTo(iRealCdcW,iTempInt);

		memDcBody.SelectObject(pOldPen);
		pen.DeleteObject();
	}	

	BOOL bRet1 = memDcBody.TransparentBlt(leftMargin,0,m_rcBody.Width(),iTempCdcH,pDC,m_frameW,m_frameW,m_rcBody.Width(),iTempCdcH,RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
	BOOL bRet2 = pDC->TransparentBlt(m_frameW,m_frameW,m_rcBody.Width(),iTempCdcH,&memDcBody,leftMargin,0,m_rcBody.Width(),iTempCdcH,m_clrFourAngleBk);
	
	DWORD dwData = 0;
	if(!bRet1 || !bRet2)
	{
		dwData = GetLastError();
	}

	graphics.ReleaseHDC(memDcBody.m_hDC);

	memDcBody.SetBkMode(oldBkMode);
	memDcBody.SelectObject(pOldFont);
	memDcBody.SelectObject(pOldBitmap);

	bitMap.DeleteObject();
	memDcBody.DeleteDC();

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
	++existMs;
#endif

	return 0;
}

int CMySegDlg::DrawHoriScroll(CDC* pDC)
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
	pDC->FillSolidRect(m_horiScroll.m_rcScroll.left,m_horiScroll.m_rcScroll.top - 1,m_horiScroll.m_rcScroll.Width(),1,RGB(197,197,197));
	pDC->FillSolidRect(m_horiScroll.m_rcScroll.left,m_horiScroll.m_rcScroll.bottom,m_horiScroll.m_rcScroll.Width(),1,RGB(197,197,197));

	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(197,197,197));
	CPen* pOldPen = pDC->SelectObject(&pen);
	POINT pt;

	Graphics graphics(pDC->m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	SolidBrush brush(Color(128,128,128));
	int triangleW = 8,triangleH = 10;

	//绘制左箭头
	pt.x = m_horiScroll.m_rcStartArrow.right - 1;
	pt.y = m_horiScroll.m_rcStartArrow.top; 
	pDC->MoveTo(pt);
	pt.y = m_horiScroll.m_rcStartArrow.bottom; 
	pDC->LineTo(pt);

	CRect& rcLeftArrow = m_horiScroll.m_rcStartArrow;
	Point pt1(rcLeftArrow.left + (rcLeftArrow.Width() - triangleW) / 2,rcLeftArrow.top + rcLeftArrow.Height() / 2);
	Point pt2(rcLeftArrow.left + (rcLeftArrow.Width() - triangleW) / 2 + triangleW,rcLeftArrow.top + (rcLeftArrow.Height() - triangleH) / 2);
	Point pt3(rcLeftArrow.left + (rcLeftArrow.Width() - triangleW) / 2 + triangleW,rcLeftArrow.top + (rcLeftArrow.Height() - triangleH) / 2 + triangleH);

	Point crvPts[3] = {pt1,pt2,pt3};
	graphics.FillPolygon(&brush,crvPts,3,FillModeAlternate);

	//绘制右箭头
	pt.x = m_horiScroll.m_rcEndArrow.left + 1;
	pt.y = m_horiScroll.m_rcEndArrow.top; 
	pDC->MoveTo(pt);
	pt.y = m_horiScroll.m_rcEndArrow.bottom; 
	pDC->LineTo(pt);

	CRect& rcRightArrow = m_horiScroll.m_rcEndArrow;
	Point pt4(rcRightArrow.left + (rcRightArrow.Width() - triangleW) / 2 + triangleW,rcRightArrow.top + rcRightArrow.Height() / 2);
	Point pt5(rcRightArrow.left + (rcRightArrow.Width() - triangleW) / 2,rcRightArrow.top + (rcRightArrow.Height() - triangleH) / 2);
	Point pt6(rcRightArrow.left + (rcRightArrow.Width() - triangleW) / 2,rcRightArrow.top + (rcRightArrow.Height() - triangleH) / 2 + triangleH);

	Point crvPtsRight[3] = {pt4,pt5,pt6};
	graphics.FillPolygon(&brush,crvPtsRight,3,FillModeAlternate);

	graphics.ReleaseHDC(pDC->m_hDC);

	//绘制滑块
	CPen penSlider;
	penSlider.CreatePen(PS_SOLID,1,RGB(164,164,164));
	pDC->SelectObject(&penSlider);
	pDC->Rectangle(&m_horiScroll.m_rcSlider);
	
	pDC->SelectObject(pOldPen);

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return 0;
}

int CMySegDlg::DrawVertScroll(CDC* pDC)
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
	DrawRoundRectangleNew(pDC,m_vertScroll.m_rcSlider.left,m_vertScroll.m_rcSlider.top,m_vertScroll.m_rcSlider.Width(),m_vertScroll.m_rcSlider.Height(),3,3,3,3,FALSE,m_vertScroll.m_clrSliderBk,1,TRUE,m_horiScroll.m_clrSliderBk);

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return 0;
}

int CMySegDlg::DrawHoriScrollStyle2(CDC* pDC)
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

	m_horiScroll.m_arrowInfo2.CalcPtData(m_horiScroll.m_rcEndArrow);
	if(EAS_FILL == m_horiScroll.m_arrowInfo2.m_eArrowStyle)
	{
		SolidBrush brush(m_horiScroll.m_arrowInfo2.m_clrArrow);
		graphics.FillPolygon(&brush,m_horiScroll.m_arrowInfo2.m_aryPtTriangleRight,3,FillModeAlternate);
	}
	else
	{
		Pen pen(m_horiScroll.m_arrowInfo2.m_clrLineNormal,m_horiScroll.m_arrowInfo2.m_iLineW);
		graphics.DrawLines(&pen,m_horiScroll.m_arrowInfo2.m_aryPtTriangleRight,3);
	}

	graphics.ReleaseHDC(pDC->m_hDC);

	//绘制滑块
	pDC->FillSolidRect(&m_horiScroll.m_rcSlider,RGB(m_horiScroll.m_clrSliderBk.GetR(),m_horiScroll.m_clrSliderBk.GetG(),m_horiScroll.m_clrSliderBk.GetB()));

#ifdef _DEBUG
	clock_t existMs = clock() - startMs;
#endif

	return 0;
}

int CMySegDlg::DrawVertScrollStyle2(CDC* pDC)
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

int CMySegDlg::DrawGradientData(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	if(!IsShowGradientData())
	{
		return 1;
	}

	if(m_rcGrad.IsRectEmpty())
	{
		return 2;
	}

	//pDC->FillSolidRect(m_rcGrad,RGB(0XFF,0XFF,0XFF));

	CString strTemp(_T(""));
	strTemp += _T("列1:1.23456\n");
	strTemp += _T("列2:1.23456\n");
	strTemp += _T("列3:1.23456\n");
	strTemp += _T("列4:1.23456\n");
	strTemp += _T("列5:1.23456\n");
	strTemp += _T("列6:1.23456\n");

	strTemp += _T("列7:1.23456\n");
	strTemp += _T("列8:1.23456\n");
	strTemp += _T("列9:1.23456\n");
	strTemp += _T("列10:1.23456\n");
	strTemp += _T("列11:1.23456\n");
	strTemp += _T("列12:1.23456\n");

	strTemp = GetGradientInfoString();

	CFont* pOldFt = pDC->SelectObject(&m_fontSet);
	pDC->SetTextColor(m_txClr1);
	pDC->SetBkMode(TRANSPARENT);

	CRect rcTemp,rcTemp2;
	pDC->DrawText(strTemp,rcTemp,DT_CALCRECT);
	rcTemp2.left = m_rcGrad.left + (m_rcGrad.Width() - rcTemp.Width()) / 2;
	if(rcTemp2.left < m_rcGrad.left)
	{
		rcTemp2.left = m_rcGrad.left;
	}
	rcTemp2.top = m_rcGrad.top + (m_rcGrad.Height() - rcTemp.Height()) / 2;
	if(rcTemp2.top < m_rcGrad.top)
	{
		rcTemp2.top = m_rcGrad.top;
	}
	rcTemp2.right = rcTemp2.left + rcTemp.Width();
	rcTemp2.bottom = rcTemp2.top + rcTemp.Height();
	pDC->DrawText(strTemp,rcTemp2,DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);

	pDC->SelectObject(pOldFt);

	return 0;
}

int CMySegDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_edSegInfo.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_SEGED_SEGINFO);

	int iFtH = 14 * m_dTxRatio;

	m_edSegInfo.SetMyFont(m_stTxFont,iFtH);
	m_edSegInfo.SetTextClr1(RGB_SEGEDITSTEP);
	m_edSegInfo.SetBeFuncEdit(TRUE);
	m_edSegInfo.SetDefaultIndent();

	m_pxColW *= m_dHoriRatio;
	m_stepWidth *= m_dHoriRatio;
	m_iGradInfoW *= m_dHoriRatio;
	m_roundHeight *= m_dVertRatio;
	m_pxHeaderH *= m_dVertRatio;

	m_strSegDefName = pLangInfo->GetText(57);
	m_strStepDefName = pLangInfo->GetText(58);

	return CDialogEx::OnCreate(lpCreateStruct);
}

BOOL CMySegDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMySegDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMySegDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	
}

void CMySegDlg::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	if(m_lnkSegItemInfo.GetSize() < 1)
	{
		return;
	}

	m_segDotPosInfo = FindDotPosInfo(point);
	if(!m_segDotPosInfo.BeValidStepEmptyPos())
	{
		if(!m_segDotPosInfo.BeValidSegmentEmptyPos())
		{
			return;
		}

		EditSegment(m_segDotPosInfo.m_seg);

		return;
	}

	return EditStep(m_segDotPosInfo.m_seg,m_segDotPosInfo.m_step);
}

void CMySegDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	m_segDotPosInfo = FindDotPosInfo(point);
	if(EDP_LINE == m_segDotPosInfo.m_eDotPosType)
	{
		if(!m_bSelMoveStart)
		{
			m_bSelMoveStart = TRUE;
			SetCapture();
			CDialogEx::OnLButtonDown(nFlags,point);
			return;
		}
	}

	SetFocus();

	SetSegmentStepSelectState(MK_CONTROL & nFlags,TRUE);

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
		//计算鼠标点击时所在的行和列
	}

	m_ptPre = point;	

	return;
}

void CMySegDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	m_segDotPosInfo = FindDotPosInfo(point);

	BOOL bProcessEdit = TRUE;
	if(m_bSelMoveStart)
	{
		bProcessEdit = FALSE;
		m_bSelMoveStart = FALSE;
		ReleaseCapture();
	}

	m_horiScroll.m_bLeftBtDown = FALSE;
	m_horiScroll.m_bStartDragSlider = FALSE;
	m_horiScroll.m_bSliderDragMove = FALSE;

	m_vertScroll.m_bLeftBtDown = FALSE;
	m_vertScroll.m_bStartDragSlider = FALSE;
	m_vertScroll.m_bSliderDragMove = FALSE;

	ReleaseCapture();

	if(EDP_NONE == m_segDotPosInfo.m_eDotPosType || !bProcessEdit)
	{

	}
	else
	{
		CSegItemInfo* pSegItem = m_lnkSegItemInfo[m_segDotPosInfo.m_seg];
		if(NULL == pSegItem)
		{
			return;
		}
		CStepItemInfo* pStepItem = pSegItem->m_lnkStepItemInfo[m_segDotPosInfo.m_step];
		if(EDP_CYCLE != m_segDotPosInfo.m_eDotPosType && EDP_SEGNAME != m_segDotPosInfo.m_eDotPosType)
		{
			if(NULL == pStepItem)
			{
				return;
			}
		}

		CString tempStr(_T(""));
		switch(m_segDotPosInfo.m_eDotPosType)
		{
		case EDP_CYCLE:
			if(m_lnkSegItemInfo[m_segDotPosInfo.m_seg]->GetBeMeltSeg())
			{
				CDialogEx::OnLButtonUp(nFlags,point);
				return;
			}
			tempStr.Format(_T("%d"),pSegItem->m_iCycleNum);	
			break;
		case EDP_SEGNAME: return CDialogEx::OnLButtonUp(nFlags,point);	
		case EDP_TEM: tempStr.Format(_T("%.1lf"),pStepItem->m_fAimTem);	break;
		case EDP_TIME: 
			if(pStepItem->GetBeCollectAll())	//熔解步骤禁止编辑延时时间
			{
				CDialogEx::OnLButtonUp(nFlags,point);
				return;
			}
			tempStr.Format(_T("%d"),pStepItem->m_iDelay);	
			break;
		case EDP_COLLECTFLU:  break;
		case EDP_LINE:	if(m_bSelMoveStart){m_bSelMoveStart = FALSE;} break;
		default: break;
		}

		if(EDP_COLLECTFLU == m_segDotPosInfo.m_eDotPosType)
		{
			if(pStepItem->GetBeNoneCollect())
			{
#ifndef DISABLE_FIXED_METLSTEPNUM
				//熔解程序低温段不可设置荧光
				if(m_lnkSegItemInfo[m_segDotPosInfo.m_seg]->GetBeMeltSeg())
				{
					CDialogEx::OnLButtonUp(nFlags,point);
					return;
				}
#endif

#ifdef ONEPROC_ONEAMLSTEP
				if(GetBeHaveAmlCollectStepExcept(m_segDotPosInfo.m_seg,m_segDotPosInfo.m_step))
				{
					CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
					PopNormalTipInfo(pLangInfo->GetText(308));

					CDialogEx::OnLButtonUp(nFlags,point);
					return;
				}
				else
				{
					pStepItem->SetCollect();
				}
#else
				pStepItem->SetCollect();
#endif
			}
			else if(pStepItem->GetBeCollect())
			{
				pStepItem->SetNoneCollect();
			}
			
			Invalidate(FALSE);
		}
		else if(EDP_LINE != m_segDotPosInfo.m_eDotPosType)
		{
			if(EDP_TEM == m_segDotPosInfo.m_eDotPosType)
			{
				m_edSegInfo.SetBeEnterTellParent(TRUE);
				m_edSegInfo.SetTextClr1(RGB_SEGEDITSTEP);
			}
			else if(EDP_TIME == m_segDotPosInfo.m_eDotPosType)
			{
				m_edSegInfo.SetBeEnterTellParent(TRUE);
				m_edSegInfo.SetTextClr1(RGB_SEGEDITSTEP);
			}
			else if(EDP_CYCLE == m_segDotPosInfo.m_eDotPosType)
			{
				m_edSegInfo.SetBeEnterTellParent(TRUE);
				m_edSegInfo.SetCfsTextClr(RGB_SEGEDITSTEP);
			}
			else
			{
				m_edSegInfo.ClearMaxMinLimit();
				m_edSegInfo.SetBeEnterTellParent(TRUE);
				m_edSegInfo.SetTextClr1(RGB_SEGEDITSEG);
			}

			m_edSegInfo.MoveWindow(m_segDotPosInfo.m_rcItem);
			m_edSegInfo.SetText(tempStr);
			m_edSegInfo.ShowWindow(SW_SHOW);

			m_preEditDotInfo = m_segDotPosInfo;
		}
	}
	
	CDialogEx::OnLButtonUp(nFlags,point);
	return;
}

void CMySegDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(!m_bEditable)
	{
		return;
	}

	if(!m_bSelMoveStart)
	{
		m_segDotPosInfo = FindDotPosInfo(point);
	}
	
	if(EDP_LINE == m_segDotPosInfo.m_eDotPosType)
	{
		HCURSOR hCur = LoadCursor(NULL,IDC_SIZENS) ;
		::SetCursor(hCur);
	}
	else
	{
		HCURSOR hCur = LoadCursor(NULL,IDC_ARROW) ;
		::SetCursor(hCur); 
	}  

	if (MK_LBUTTON == nFlags && m_bSelMoveStart)
	{
		if(-1 != m_segDotPosInfo.m_seg && -1 != m_segDotPosInfo.m_step)
		{
			float fTempMaxTem = m_maxTem;
			float fTempMinTem =  m_minTem;
			float curTem = PixelHeightToTem(point.y);

			CSegItemInfo* pSegItem = m_lnkSegItemInfo[m_segDotPosInfo.m_seg];
			if(NULL == pSegItem)
			{
				return;
			}
			CStepItemInfo* pStepItem = pSegItem->m_lnkStepItemInfo[m_segDotPosInfo.m_step];
			if(NULL == pSegItem)
			{
				return;
			}

#ifndef DISABLE_FIXED_METLSTEPNUM
			if(pSegItem->GetBeMeltSeg())
			{
				if(1 == pStepItem->m_eChkType)
				{
					Node<CStepItemInfo>* pNextNode = pSegItem->m_lnkStepItemInfo.GetAt(m_segDotPosInfo.m_step)->pNext;
					if(NULL != pNextNode)
					{
						fTempMaxTem = pNextNode->data.m_fAimTem - 0.1;	//0.1防止低温度和高度段温度相同
					}
				}
				else if(3 == pStepItem->m_eChkType)
				{
					Node<CStepItemInfo>* pPreNode = pSegItem->m_lnkStepItemInfo.GetAt(m_segDotPosInfo.m_step)->pPrevious;
					if(NULL != pPreNode)
					{
						fTempMinTem = pPreNode->data.m_fAimTem + 0.1;
					}
				}
			}
#endif

			if(curTem > fTempMaxTem )
			{
				curTem = fTempMaxTem;
			}		
			else if(curTem < fTempMinTem)
			{
				curTem = fTempMinTem;
			}
			
			int tempInt = (pStepItem->m_fAimTem - curTem) * m_oneTemH;
			pStepItem->m_fAimTem = curTem;
			pStepItem->m_rcTem.DeflateRect(0,tempInt,0,-tempInt);
			pStepItem->m_rcLine.DeflateRect(0,tempInt,0,-tempInt);
			pStepItem->m_rcMinSec.DeflateRect(0,tempInt,0,-tempInt);
			Invalidate(FALSE);
		}
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

	m_ptPre = point;

	return;
}

BOOL CMySegDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_bEditable)
	{
		return __super::OnMouseWheel(nFlags, zDelta, pt);
	}

	EndEdit();
	
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

void CMySegDlg::OnSize(UINT nType, int cx, int cy)
{
	m_rcLst = CRect(0,0,cx,cy);
	CalcScrollPos();
	 
	CDialogEx::OnSize(nType, cx, cy);
}

UINT CMySegDlg::OnGetDlgCode()
{
	UINT nCode = CDialogEx::OnGetDlgCode();
	nCode |= DLGC_WANTARROWS;

	return nCode;
}

LRESULT CMySegDlg::OnMsgFuncEditKillFocus(WPARAM wParam,LPARAM lParam)
{
	if(!m_bEditDataProcessed)
	{
		EndEdit();
	}
	else
	{
		m_bEditDataProcessed = FALSE;
	}
	return 1;
}

LRESULT CMySegDlg::OnMsgEditEnterPushed(WPARAM wParam,LPARAM lParam)
{
	if(m_edSegInfo.GetBeEnterTellParent())
	{
		m_bEditDataProcessed = TRUE;
		EndEdit();
		m_edSegInfo.ShowWindow(SW_HIDE);
	}
	return 1;
}

BOOL CMySegDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialogEx::PreTranslateMessage(pMsg);
}

CSegDotPosInfo CMySegDlg::FindDotPosInfo(CPoint pt)
{
	CSegDotPosInfo segDotPosInfo;
	segDotPosInfo.m_pt = pt;

	if(!m_segStepRange.BeValidRange())
	{
		return segDotPosInfo;
	}

	if(m_horiScroll.m_bScrollNeedShow && m_horiScroll.m_rcScroll.PtInRect(pt))
	{
		segDotPosInfo.m_eDotPosType = EDP_HORISCROLL;
		return segDotPosInfo;
	}

	if(m_vertScroll.m_bScrollNeedShow && m_vertScroll.m_rcScroll.PtInRect(pt))
	{
		segDotPosInfo.m_eDotPosType = EDP_VERTSCROLL;
		return segDotPosInfo;
	}

	int count = 0;

	for(int i = m_segStepRange.GetSegmentStart();i <= m_segStepRange.GetSegmentEnd();++i)
	{
		CSegItemInfo* pSegItem = m_lnkSegItemInfo[i];
		if(NULL == pSegItem)
		{
			continue;
		}

		if(pSegItem->m_rcCycleNum.PtInRect(pt))
		{
			segDotPosInfo.m_eDotPosType = EDP_CYCLE;
			segDotPosInfo.m_rcItem = pSegItem->m_rcCycleNum;
			segDotPosInfo.m_seg = i;
			return segDotPosInfo;
		}
		else
		{
			if(pSegItem->m_rcSegName.PtInRect(pt))
			{
				segDotPosInfo.m_eDotPosType = EDP_SEGNAME;
				segDotPosInfo.m_rcItem = pSegItem->m_rcSegName;
				segDotPosInfo.m_seg = i;
				return segDotPosInfo;
			}
		}

		if(pSegItem->m_rcSeg.PtInRect(pt))
		{
			segDotPosInfo.m_eDotPosType = EDP_SEGEMPTY;
			segDotPosInfo.m_rcItem = pSegItem->m_rcSeg;
			segDotPosInfo.m_seg = i;
			return segDotPosInfo;
		}

		int stepStart = 0,stepEnd = 0;
		if(i == m_segStepRange.GetSegmentStart())
		{
			stepStart = m_segStepRange.GetStepStart();
		}

		if(i == m_segStepRange.GetSegmentEnd())
		{
			stepEnd = m_segStepRange.GetStepEnd();
		}
		else
		{
			stepEnd = pSegItem->m_lnkStepItemInfo.GetSize() - 1;
		}

		for(int j = stepStart;j <= stepEnd;++j)
		{
			++count;

			CStepItemInfo* pStepItem = pSegItem->m_lnkStepItemInfo[j];
			if(NULL == pSegItem)
			{
				continue;
			}

			if(pStepItem->m_rcTem.PtInRect(pt))
			{
				segDotPosInfo.m_eDotPosType = EDP_TEM;
				segDotPosInfo.m_rcItem = pStepItem->m_rcTem;
				segDotPosInfo.m_seg = i;
				segDotPosInfo.m_step = j;
				return segDotPosInfo;
			}
			else if(pStepItem->m_rcMinSec.PtInRect(pt))
			{
				segDotPosInfo.m_eDotPosType = EDP_TIME;
				segDotPosInfo.m_rcItem = pStepItem->m_rcMinSec;
				segDotPosInfo.m_seg = i;
				segDotPosInfo.m_step = j;
				return segDotPosInfo;
			}
			else if(pStepItem->m_rcLine.PtInRect(pt))
			{
				segDotPosInfo.m_eDotPosType = EDP_LINE;
				segDotPosInfo.m_rcItem = pStepItem->m_rcLine;
				segDotPosInfo.m_seg = i;
				segDotPosInfo.m_step = j;
				return segDotPosInfo;
			}
			else if(pStepItem->m_rcCkbtCollectFlu.PtInRect(pt))
			{
				segDotPosInfo.m_eDotPosType = EDP_COLLECTFLU;
				segDotPosInfo.m_rcItem = pStepItem->m_rcCkbtCollectFlu;
				segDotPosInfo.m_seg = i;
				segDotPosInfo.m_step = j;
				return segDotPosInfo;
			}
			else if(pt.x < count * m_stepWidth - m_ssColLeftIndent)
			{
				segDotPosInfo.m_eDotPosType = EDP_NONE;
				segDotPosInfo.m_seg = i;
				segDotPosInfo.m_step = j;
				return segDotPosInfo;
			}
		}
	}

	return segDotPosInfo;
}

float CMySegDlg::PixelHeightToTem(int pixelH)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int tempInt = m_image.GetHeight() * m_dVertRatio / 2 + m_roundHeight;
	float tem = m_minTem;
	if(m_horiScroll.m_bScrollNeedShow)
	{
		tem = (rcClient.Height() - m_horiScroll.m_scrollH - tempInt - pixelH) / m_oneTemH + m_minTem;
	}
	else
	{
		tem = (rcClient.Height() - tempInt - pixelH) / m_oneTemH + m_minTem;
	}

	return tem; 
}

int CMySegDlg::UpdateSegStepRange(int segId,int stepId)
{
	if(m_lnkSegItemInfo.GetSize() <= 0)
	{
		m_segStepRange.ResetSegStepRange();
		return -1;
	}

	if(-1 == segId)
	{
		return -2;
	}

	if(-1 == stepId)
	{
		return -3;
	}

	if(m_segStepRange.BeValidRange())
	{
		if(segId < m_segStepRange.GetSegmentStart())
		{
			m_segStepRange.SetSegmentStart(segId);
			m_segStepRange.SetStepStart(stepId);
			goto NODE1;
		}
		else if(segId > m_segStepRange.GetSegmentEnd())
		{
			m_segStepRange.SetSegmentEnd(segId);
			m_segStepRange.SetStepEnd(stepId);
			goto NODE2;
		}
		else
		{
			if(segId == m_segStepRange.GetSegmentStart())
			{
				if(stepId < m_segStepRange.GetStepStart())
				{
					m_segStepRange.SetStepStart(stepId);
					goto NODE1;
				}
			}

			if(segId == m_segStepRange.GetSegmentEnd())
			{
				if(stepId > m_segStepRange.GetStepEnd())
				{
					m_segStepRange.SetStepEnd(stepId);
					goto NODE2;
				}
			}
		}
	}
	else
	{
		m_segStepRange.SetSegmentStart(0);
		m_segStepRange.SetStepStart(0);
		goto NODE1;
	}

NODE1:
	int count = 0;
	int i = m_segStepRange.GetSegmentStart();
	int j = -1;
	for(;i < m_lnkSegItemInfo.GetSize();++i)
	{
		j = 0;
		if(i == m_segStepRange.GetSegmentStart())
		{
			j = m_segStepRange.GetStepStart();
		}
		for(;j < m_lnkSegItemInfo[i]->m_lnkStepItemInfo.GetSize();++j)
		{
			if(++count == m_visibleStepNum)
			{
				m_segStepRange.SetSegmentEnd(i);
				m_segStepRange.SetStepEnd(j);
				goto REDRAW;
			}
		}
	}

	m_segStepRange.SetSegmentEnd(m_lnkSegItemInfo.GetSize() - 1);
	m_segStepRange.SetStepEnd(m_lnkSegItemInfo[m_lnkSegItemInfo.GetSize() - 1]->m_lnkStepItemInfo.GetSize() - 1);

REDRAW:
	return 1;

NODE2:
	int secCount = 0;
	int t = m_segStepRange.GetSegmentEnd();
	int k = -1;
	for(;t >= 0;--t)
	{
		k = m_lnkSegItemInfo[t]->m_lnkStepItemInfo.GetSize() - 1;
		if(t == m_segStepRange.GetSegmentEnd())
		{
			k = m_segStepRange.GetStepEnd();
		}
		for(;k >= 0;--k)
		{
			if(++secCount == m_visibleStepNum)
			{
				m_segStepRange.SetSegmentStart(t);
				m_segStepRange.SetStepStart(k);
				goto SECONDREDRAW;
			}
		}
	}

	m_segStepRange.SetSegmentStart(0);
	m_segStepRange.SetStepStart(0);

SECONDREDRAW:
	return 2;
}

void CMySegDlg::ProcessHoriSrcoll(UINT nFlags, CPoint point)
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

void CMySegDlg::ProcessVertSrcoll(UINT nFlags, CPoint point)
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
				ScrollRowToFixPos(m_horiScroll.m_curPixelPos);
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

void CMySegDlg::AddSegment()
{
	CSegItemInfo segItem;
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	segItem.m_strSegName = pLangInfo->GetText(57);
	m_lnkSegItemInfo.AddTail(segItem);
	int iSegIndex = m_lnkSegItemInfo.GetSize() - 1;
	AddStep(iSegIndex);
	return;
}

void CMySegDlg::AddStep(int segId,BOOL bMelt /*= FALSE*/)
{
	if(segId < 0 || segId >= m_lnkSegItemInfo.GetSize())
	{
		return;
	}

	CSegItemInfo* pSegItemInfo = m_lnkSegItemInfo[segId];
	if(NULL != pSegItemInfo)
	{
		if(pSegItemInfo->GetBeMeltSeg())
		{
			CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
			PopNormalTipInfo(pLangInfo->GetText(269));
			return;
		}

		CStepItemInfo newStep;
#ifdef N96INSTRUMENT
		newStep.m_fRate = 6;
#else
	#ifdef SAMPLATE_16SITE
		newStep.m_fRate = 12;
	#endif
#endif

		pSegItemInfo->m_lnkStepItemInfo.AddTail(newStep);
		if(bMelt)
		{
			Node<CStepItemInfo>* pNode = pSegItemInfo->m_lnkStepItemInfo.GetTail();
			if(NULL != pNode)
			{
				pNode->data.m_eChkType = CHECK_ALL_POINT;
			}
		}
	}
	
	SetSegStepName(segId);

	CalcTotalColumnWidth();
	CalcScrollPos();
	CalcSegStepToShow();

	Invalidate(FALSE);
	return;
}

int CMySegDlg::SetSegmentStepSelectState(BOOL bCtrlClicked,BOOL bRedraw /*= FALSE*/)
{
	if(m_segDotPosInfo.BeValidStepEmptyPos())
	{
		SetStepSelectState(bCtrlClicked);
	}
	else if(m_segDotPosInfo.BeValidSegmentEmptyPos())
	{
		SetSegmentSelectState(bCtrlClicked);
	}
	else
	{
		return -1;
	}


	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySegDlg::SetSegmentSelectState(BOOL bCtrlClicked,BOOL bRedraw /*= FALSE*/)
{
	if(!m_segDotPosInfo.BeValidSegmentEmptyPos())
	{
		return -1;
	}

	Node<CSegItemInfo>* pSegNode = NULL;

	if(bCtrlClicked)
	{
		pSegNode = m_lnkSegItemInfo.GetAt(m_segDotPosInfo.m_seg);
		if(NULL == pSegNode)
		{
			return -2;
		}

		pSegNode->data.m_bSelected = !pSegNode->data.m_bSelected;

		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			pStepNode->data.m_bSel = pSegNode->data.m_bSelected;
			pStepNode = pStepNode->pNext;
		}
	}
	else
	{
		BOOL bSelected = FALSE;
		int iSegNum = m_lnkSegItemInfo.GetSize();
		for(int i = 0;i < iSegNum;++i)
		{
			pSegNode = m_lnkSegItemInfo.GetAt(i);
			if(NULL == pSegNode)
			{
				continue;
			}
			if(i == m_segDotPosInfo.m_seg)
			{
				pSegNode->data.m_bSelected = TRUE;
				bSelected = TRUE;
			}
			else
			{
				pSegNode->data.m_bSelected = FALSE; 
				bSelected = FALSE;
			}
			Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
			while(NULL != pStepNode)
			{
				if(pStepNode->data.m_bSel != bSelected)
				{
					pStepNode->data.m_bSel = bSelected;
				}
				pStepNode = pStepNode->pNext;
			}
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySegDlg::SetStepSelectState(BOOL bCtrlClicked,BOOL bRedraw /*= FALSE*/)
{
	if(!m_segDotPosInfo.BeValidStepEmptyPos())
	{
		return -1;
	}

	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetAt(m_segDotPosInfo.m_seg);
	if(NULL == pSegNode)
	{
		return -2;
	}

	BOOL bSelected = FALSE,bExistSelected = FALSE;

	if(bCtrlClicked)
	{
		bSelected = !(pSegNode->data.m_lnkStepItemInfo[m_segDotPosInfo.m_step]->m_bSel);

#ifndef DISABLE_FIXED_METLSTEPNUM	//熔解程序段同时选中，同时取消选中
		if(pSegNode->data.GetBeMeltSeg())	
		{
			Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
			while(NULL != pStepNode)
			{
				pStepNode->data.m_bSel = bSelected;
				pStepNode = pStepNode->pNext;
			}
		}
		else
#endif
		{
			m_lnkSegItemInfo[m_segDotPosInfo.m_seg]->m_lnkStepItemInfo[m_segDotPosInfo.m_step]->m_bSel = bSelected;
		}
		pSegNode->data.m_bSelected = pSegNode->data.GetBeAllStepSelected();
	}
	else
	{
		CSegItemInfo* pSegItem = NULL;
		CStepItemInfo* pStepItem = NULL;
		int segNum = m_lnkSegItemInfo.GetSize();
		for(int i = 0;i < segNum;++i)
		{
			bExistSelected = FALSE;
			bSelected = FALSE;

			pSegItem = m_lnkSegItemInfo[i];
			if(NULL == pSegItem)
			{
				continue;
			}

#ifndef DISABLE_FIXED_METLSTEPNUM
			if(pSegItem->GetBeMeltSeg())	//熔解程序段同时选中，同时取消选中
			{
				if(i == m_segDotPosInfo.m_seg)
				{
					bSelected = TRUE;
					bExistSelected = TRUE;
				}
				else
				{
					bSelected = FALSE;
				}
				Node<CStepItemInfo>* pStepNode = pSegItem->m_lnkStepItemInfo.GetHead();
				while(NULL != pStepNode)
				{
					pStepNode->data.m_bSel = bSelected;
					pStepNode = pStepNode->pNext;
				}
			}
			else
#endif
			{
				int stepNum = pSegItem->m_lnkStepItemInfo.GetSize();
				for(int j = 0;j < stepNum;++j)
				{
					pStepItem = pSegItem->m_lnkStepItemInfo[j];
					if(NULL == pStepItem)
					{
						continue;
					}

					if(i != m_segDotPosInfo.m_seg || j != m_segDotPosInfo.m_step)
					{
						pStepItem->m_bSel = FALSE;
					}
					else
					{
						pStepItem->m_bSel = TRUE;
						bExistSelected = TRUE;
					}
				}
			}

			pSegItem->m_bSelected = pSegItem->GetBeAllStepSelected();
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySegDlg::SetSegmentFullSelectState(BOOL bSel,BOOL bRedraw /*= TRUE*/)
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			pStepNode->data.m_bSel = bSel;
			pStepNode = pStepNode->pNext;
		}

		pSegNode->data.m_bSelected = bSel;
		pSegNode = pSegNode->pNext;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMySegDlg::SetDefaultSingleSelectStep(int iSegIndex,int iStepIndex,BOOL bRedraw /*= FALSE*/)	
{
	if(m_lnkSegItemInfo.GetSize() < 1)
	{
		return 1;
	}

	if(iSegIndex < 0)
	{
		return -1;
	}
	if(iStepIndex < 0)
	{
		return -2;
	}

	int iNewSegId = -1,iNewStepId = -1;
	CSegItemInfo* pSegItemInfo = NULL;
	CStepItemInfo* pStepItemInfo = NULL;
	if(iSegIndex < m_lnkSegItemInfo.GetSize())
	{
		iNewSegId = iSegIndex;
		pSegItemInfo = m_lnkSegItemInfo[iSegIndex];
		if(NULL != pSegItemInfo)
		{
			if(pSegItemInfo->m_lnkStepItemInfo.GetSize() > 0)
			{
				if(iStepIndex < pSegItemInfo->m_lnkStepItemInfo.GetSize())
				{
					iNewStepId = iStepIndex;
				}
				else
				{
					iNewStepId = pSegItemInfo->m_lnkStepItemInfo.GetSize() - 1;
				}
			}
		}
	}
	else
	{
		iNewSegId = m_lnkSegItemInfo.GetSize() - 1;
		pSegItemInfo = m_lnkSegItemInfo[iNewSegId];
		iNewStepId = pSegItemInfo->m_lnkStepItemInfo.GetSize() - 1;
	}

	if(iNewStepId >= 0 && iNewStepId < pSegItemInfo->m_lnkStepItemInfo.GetSize())
	{
		pStepItemInfo = pSegItemInfo->m_lnkStepItemInfo[iNewStepId];
		if(NULL != pSegItemInfo)
		{
			pStepItemInfo->m_bSel = TRUE;
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

void CMySegDlg::InsertSegment(EINSERTSELECT insetSel)
{
	if(m_lnkSegItemInfo.GetSize() >= 9)
	{
		CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
		CString str1 = pLangInfo->GetText(250);
		CString strTemp(_T(""));
		strTemp.AppendFormat(str1,9);
		PopNormalTipInfo(strTemp);

		return;
	}

	if(EIS_LAST == insetSel)
	{
		AddSegment();
		return;
	}

	int iSegIndex = m_lnkSegItemInfo.GetSize() - 1,iRealSegIndex = iSegIndex;;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetTail();
	while(NULL != pSegNode)
	{
		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetTail();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel)
			{
				CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
				CSegItemInfo segItem;
				segItem.m_strSegName = pLangInfo->GetText(57);

				CStepItemInfo newStep;
#ifdef N96INSTRUMENT
				newStep.m_fRate = 6;
#else
	#ifdef SAMPLATE_16SITE
				newStep.m_fRate = 12;
	#endif
#endif

				segItem.m_lnkStepItemInfo.AddTail(newStep);
				if(EIS_BEFORE == insetSel)
				{
					m_lnkSegItemInfo.InsertPreNode(pSegNode,segItem);
					iRealSegIndex -= 1;
				}
				else if(EIS_AFTER == insetSel)
				{
					m_lnkSegItemInfo.InsertAfterNode(pSegNode,segItem);
					iRealSegIndex += 1;
				}
				UpdateSegStepScrollPosInfo();
				SetSegStepName(iRealSegIndex);
				Invalidate(FALSE);
				return;
			}
			pStepNode = pStepNode->pPrevious;
		}
		pSegNode = pSegNode->pPrevious;
		iRealSegIndex = --iSegIndex;
	}

	AddSegment();
	return;
}

void CMySegDlg::InsertStep(EINSERTSELECT insetSel)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	if(m_lnkSegItemInfo.GetSize() < 1)
	{
		PopNormalTipInfo(pLangInfo->GetText(245));
		return;
	}

	int iSegIndex = m_lnkSegItemInfo.GetSize() - 1;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetTail();
	while(NULL != pSegNode)
	{
		if(pSegNode->data.m_lnkStepItemInfo.GetSize() >= 9)
		{
			CString str1 = pLangInfo->GetText(244);
			CString strTemp(_T(""));
			strTemp.AppendFormat(str1,9);
			PopNormalTipInfo(strTemp);
			return;
		}

		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetTail();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel)
			{
#ifndef DISABLE_FIXED_METLSTEPNUM
				if(pSegNode->data.GetBeMeltSeg())
				{
					PopNormalTipInfo(pLangInfo->GetText(243));
					return;
				}
				else
#endif
				{
					CStepItemInfo stepItem;

#ifdef N96INSTRUMENT
					stepItem.m_fRate = 6;
#else
	#ifdef SAMPLATE_16SITE
					stepItem.m_fRate = 12;
	#endif
#endif

					if(EIS_BEFORE == insetSel)
					{
						pSegNode->data.m_lnkStepItemInfo.InsertPreNode(pStepNode,stepItem);
					}
					else if(EIS_AFTER == insetSel)
					{
						pSegNode->data.m_lnkStepItemInfo.InsertAfterNode(pStepNode,stepItem);
					}
					else
					{
						pSegNode->data.m_lnkStepItemInfo.AddTail(stepItem);
					}
					UpdateSegStepScrollPosInfo();
					SetSegStepName(iSegIndex);
					Invalidate(FALSE);
					return;
				}
			}
			pStepNode = pStepNode->pPrevious;
		}
		pSegNode = pSegNode->pPrevious;
		--iSegIndex;
	}

	AddStep(m_lnkSegItemInfo.GetSize() - 1);

	return;
}

void CMySegDlg::DeleteSelectedItem()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(m_lnkSegItemInfo.GetSize() < 1)
	{
		PopNormalTipInfo(pLangInfo->GetText(239));
		return;
	}

	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	Node<CSegItemInfo>* pTempSeg = NULL;

	BOOL bDeleted = FALSE;
	BOOL bAsked = FALSE;

	int iStepDelete = 0,iSegDelete = 0;
	int iSegIndex = 0,iFstDeletedSeg = 0;	//iFstDeletedSeg记录第一个删除的步骤在流程中的下标
	BOOL bFstDeleteStep = FALSE;

	while(NULL != pSegNode)
	{
		int iStepIndex = 0;
		Node<CStepItemInfo>* pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		Node<CStepItemInfo>* pTempStep = NULL;
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel)
			{
				if(!bAsked)
				{
					bAsked = TRUE;
				}

				pTempStep = pStepNode->pNext;
				pSegNode->data.m_lnkStepItemInfo.DeleteNode(pStepNode);
				pStepNode = pTempStep;
				pTempStep = NULL;
				bDeleted = TRUE;
				iStepDelete = iStepIndex;
				iSegDelete = iSegIndex;

				iFstDeletedSeg = iSegIndex;

				continue;
			}
			pStepNode = pStepNode->pNext;
			++iStepIndex;
		}

		if(pSegNode->data.m_lnkStepItemInfo.GetSize() < 1)
		{
			pTempSeg = pSegNode->pNext;
			m_lnkSegItemInfo.DeleteNode(pSegNode);
			pSegNode = pTempSeg;
			pTempSeg = NULL;
			bDeleted = TRUE;
			iSegDelete = iSegIndex;
			continue;
		}

		pSegNode = pSegNode->pNext;
		++iSegIndex;
	}

	if(bDeleted)
	{
		UpdateSegStepScrollPosInfo();

		//设置选中的步骤
		SetDefaultSingleSelectStep(iSegDelete,iStepDelete);

		if(!bFstDeleteStep)
		{
			SetSegStepName(iFstDeletedSeg);
		}

		Invalidate(FALSE);
	}
	else
	{
		PopNormalTipInfo(pLangInfo->GetText(239));
	}

	return;
}

void CMySegDlg::Clear(BOOL bRedraw /*= TRUE*/)
{
	if(m_lnkSegItemInfo.GetSize() < 1)
	{
		return;
	}

	m_lnkSegItemInfo.Clear();
	UpdateSegStepScrollPosInfo();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMySegDlg::DeleteMeltSeg()
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	
	BOOL bDeleted = FALSE;
	while(NULL != pSegNode)
	{
		if(pSegNode->data.GetBeMeltSeg())
		{
			m_lnkSegItemInfo.DeleteNode(pSegNode);
			bDeleted = TRUE;
			break;
		}
		pSegNode = pSegNode->pNext;
	}

	if(bDeleted)
	{
		UpdateSegStepScrollPosInfo();
		Invalidate(FALSE);
	}

	return;
}

void CMySegDlg::AddMeltSegIfNoMeltSegExist()
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();

	BOOL bHaveMelt = FALSE;
	while(NULL != pSegNode)
	{
		if(pSegNode->data.GetBeMeltSeg())
		{
			bHaveMelt = TRUE;
			break;
		}
		pSegNode = pSegNode->pNext;
	}

	if(!bHaveMelt)
	{
		m_lnkSegItemInfo.AddTail();
		AddStep(m_lnkSegItemInfo.GetSize() - 1,TRUE);
		return;
	}

	return;
}

void CMySegDlg::SetDefaultAmlProgramForProject()
{
	Clear();

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString str1(_T(""));
	str1 = pLangInfo->GetText(57);

	CLinkList<CSegItemInfo> lnkSegItemInfo;
	CSegItemInfo segItem;
	segItem.m_iCycleNum = 1;
	segItem.m_strSegName = str1 + _T("1");
	CStepItemInfo stepItem;
	stepItem.m_fAimTem = 61;
	stepItem.m_iDelay = 180;
	stepItem.m_eChkType = CHECK_NONE;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	segItem.m_lnkStepItemInfo.Clear();
	segItem.m_iCycleNum = 60;
	segItem.m_strSegName = str1 + _T("2");
	stepItem.m_fAimTem = 61;
	stepItem.m_iDelay = 60;
	stepItem.m_eChkType = CHECK_END_POINT;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	SetSegmentInfo(lnkSegItemInfo,TRUE);
	
	return;
}

void CMySegDlg::SetDefaultAmlProgramForProject2()
{
	Clear();

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString str1(_T(""));
	str1 = pLangInfo->GetText(57);

	CLinkList<CSegItemInfo> lnkSegItemInfo;
	CSegItemInfo segItem;
	segItem.m_iCycleNum = 1;
	segItem.m_strSegName = str1 + _T("1");
	CStepItemInfo stepItem;
	stepItem.m_fAimTem = 30;
	stepItem.m_iDelay = 20;
	stepItem.m_eChkType = CHECK_END_POINT;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	SetSegmentInfo(lnkSegItemInfo,TRUE);

	return;
}

void CMySegDlg::SetDefaultMeltProgramForProject()
{
	Clear();

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString str1(_T(""));
	str1 = pLangInfo->GetText(57);

	CLinkList<CSegItemInfo> lnkSegItemInfo;
	CSegItemInfo segItem;
	segItem.m_strSegName = str1 + _T("1");
	CStepItemInfo stepItem;
	stepItem.m_fAimTem = 60;
	stepItem.m_iDelay = 120;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	segItem.m_strSegName = str1 + _T("2");

	segItem.m_lnkStepItemInfo.Clear();
	stepItem.m_fAimTem = 60;
	stepItem.m_iDelay = 5;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	stepItem.m_fAimTem = 90;
	stepItem.m_iDelay = 5;
	stepItem.m_eChkType = CHECK_ALL_POINT;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	SetSegmentInfo(lnkSegItemInfo,TRUE);

	return;
}

void CMySegDlg::SetDefaultAbsoluteQuantification()
{
	Clear();

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString str1(_T(""));
	str1 = pLangInfo->GetText(57);

	CLinkList<CSegItemInfo> lnkSegItemInfo;
	CSegItemInfo segItem;

	segItem.m_strSegName = str1 + _T("1");
	CStepItemInfo stepItem;
	
#ifdef N96INSTRUMENT
	stepItem.m_fRate = 6;
#else
	#ifdef SAMPLATE_16SITE
		stepItem.m_fRate = 12;
	#endif
#endif

	stepItem.m_fAimTem = 95;
	stepItem.m_iDelay = 120;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	segItem.m_strSegName = str1 + _T("2");
	segItem.m_iCycleNum = 45;
	segItem.m_lnkStepItemInfo.Clear();

	stepItem.m_fAimTem = 95;
	stepItem.m_iDelay = 10;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	stepItem.m_fAimTem = 60;
	stepItem.m_iDelay = 30;
	stepItem.m_eChkType = CHECK_END_POINT;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	segItem.m_strSegName = str1 + _T("3");
	segItem.m_iCycleNum = 1;
	segItem.m_lnkStepItemInfo.Clear();

	stepItem.m_fAimTem = 40;
	stepItem.m_iDelay = 15;
	stepItem.m_eChkType = CHECK_NONE;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	SetSegmentInfo(lnkSegItemInfo,TRUE);

	return;
}

void CMySegDlg::SetDefaultStandardMeltingCurve()
{
	Clear();

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString str1(_T(""));
	str1 = pLangInfo->GetText(57);

	CLinkList<CSegItemInfo> lnkSegItemInfo;
	CSegItemInfo segItem;

	segItem.m_strSegName = str1 + _T("1");
	CStepItemInfo stepItem;

#ifdef N96INSTRUMENT
	stepItem.m_fRate = 6;
#else
	#ifdef SAMPLATE_16SITE
		stepItem.m_fRate = 12;
	#endif
#endif

	stepItem.m_fAimTem = 95;
	stepItem.m_iDelay = 120;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	segItem.m_strSegName = str1 + _T("2");
	segItem.m_iCycleNum = 45;
	segItem.m_lnkStepItemInfo.Clear();

	stepItem.m_fAimTem = 95;
	stepItem.m_iDelay = 10;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	stepItem.m_fAimTem = 60;
	stepItem.m_iDelay = 30;
	stepItem.m_eChkType = CHECK_END_POINT;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	segItem.m_strSegName = str1 + _T("3");
	segItem.m_iCycleNum = 1;
	segItem.m_lnkStepItemInfo.Clear();

	stepItem.m_fAimTem = 95;
	stepItem.m_iDelay = 15;
	stepItem.m_eChkType = CHECK_NONE;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	stepItem.m_fAimTem = 60;
	stepItem.m_iDelay = 30;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	stepItem.m_fAimTem = 95;
	stepItem.m_iDelay = 10;
	stepItem.m_eChkType = CHECK_ALL_POINT;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	segItem.m_strSegName = str1 + _T("4");
	segItem.m_iCycleNum = 1;
	segItem.m_lnkStepItemInfo.Clear();

	stepItem.m_fAimTem = 40;
	stepItem.m_iDelay = 10;
	stepItem.m_eChkType = CHECK_NONE;
	segItem.m_lnkStepItemInfo.AddTail(stepItem);

	lnkSegItemInfo.AddTail(segItem);

	SetSegmentInfo(lnkSegItemInfo,TRUE);

	return;
}

void CMySegDlg::SetBeMonitorFunc(BOOL beMonitorFunc,BOOL bRedraw /*= FALSE*/)
{
	m_bMonitorFunc = beMonitorFunc;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySegDlg::GetBeMonitorFunc()
{
	return m_bMonitorFunc;
}

BOOL CMySegDlg::SetSegmentInfo(CLinkList<CSegItemInfo>& lnkSegItemInfo,BOOL bRedraw /*= FALSE*/)
{
	m_lnkSegItemInfo = lnkSegItemInfo;
	CorrectSegmengIndex();

	CalcTotalColumnWidth();
	CalcScrollPos();
	CalcSegStepToShow();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return TRUE;
}

BOOL CMySegDlg::GetSegmentInfo(CLinkList<CSegItemInfo>& lnkSegItemInfo)
{
	lnkSegItemInfo = m_lnkSegItemInfo;
	return TRUE;
}

void CMySegDlg::SetBeEditable(BOOL bEditable)
{
	m_bEditable = bEditable;
}

BOOL CMySegDlg::GetBeEditable()
{
	return m_bEditable;
}

int CMySegDlg::CalcTotalColumnWidth()
{
	m_totalColW = 0;
	Node<CSegItemInfo>* pNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pNode)
	{
		m_totalColW += pNode->data.m_lnkStepItemInfo.GetSize() * m_stepWidth;
		pNode = pNode->pNext;
	}
	
	return m_totalColW;
}

int CMySegDlg::CalcTotalRowHeight()
{
	m_totalRowH = m_rowNum * m_pxRowH;
	return m_totalRowH;
}

int CMySegDlg::CalcSegStepToShow()
{
	int stepNum = GetStepNum();

RECALCULATE:
	if(m_lnkSegItemInfo.GetSize() > 0)
	{
		if(m_totalColW > m_rcBody.Width())
		{
			if(-1 == m_colIdStartShow)
			{
				m_colIdStartShow = 0;
				m_ssColLeftIndent = 0;
			}

			int sum = m_pxColW - m_ssColLeftIndent;
			int i = m_colIdStartShow + 1;
			for(;i < stepNum;++i)
			{
				sum += m_pxColW;
				if(sum == m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = m_pxColW - (sum - m_rcBody.Width());
					break;
				}
			}

			if(i >= stepNum)
			{
				if(0 == m_colIdStartShow && 0 == m_ssColLeftIndent)
				{
					m_colIdEndShow = stepNum - 1;
					m_esColRightIndent = 0;
				}
				else
				{
					--m_colIdStartShow;
					goto RECALCULATE;
				}
			}
		}
		else
		{
			m_colIdStartShow = 0;
			m_colIdEndShow = stepNum - 1;
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

	StartEndShowColToSegStepRange();
	m_cdcW = GetStepNum(m_segStepRange.GetSegmentStart(),m_segStepRange.GetSegmentEnd()) * m_pxColW;

	return 0;
}

int CMySegDlg::GetStepNum()
{
	int stepNum = 0;
	Node<CSegItemInfo>* pNodeSegItemInfo = m_lnkSegItemInfo.GetHead();
	while(NULL != pNodeSegItemInfo)
	{
		stepNum += pNodeSegItemInfo->data.m_lnkStepItemInfo.GetSize();
		pNodeSegItemInfo = pNodeSegItemInfo->pNext;
	}

	return stepNum;
}

int CMySegDlg::GetStepNum(int segStart,int segEnd)
{
	if(segStart < 0 || segEnd < 0)
	{
		return 0;
	}

	int stepNum = 0,index = 0;
	Node<CSegItemInfo>* pNodeSegItemInfo = m_lnkSegItemInfo.GetAt(segStart);
	if(NULL == pNodeSegItemInfo)
	{
		return 0;
	}
	int calcSegNum = segEnd - segStart + 1;
	while(NULL != pNodeSegItemInfo && index++ < calcSegNum)
	{
		stepNum += pNodeSegItemInfo->data.m_lnkStepItemInfo.GetSize();
		pNodeSegItemInfo = pNodeSegItemInfo->pNext;
	}

	return stepNum;
}

int CMySegDlg::StartEndShowColToSegStepRange()
{
	m_segStepRange.ResetSegStepRange();

	BOOL bFindStart = FALSE,bFindEnd = FALSE;

	Node<CSegItemInfo>* pNodeSegItemInfo = m_lnkSegItemInfo.GetHead();
	int totalNum = 0,preNum = 0,segIndex = 0;
	while(NULL != pNodeSegItemInfo)
	{
		int stepNum = pNodeSegItemInfo->data.m_lnkStepItemInfo.GetSize();
		preNum = totalNum;
		totalNum += stepNum;
		if(totalNum > m_colIdStartShow && !bFindStart)
		{
			m_segStepRange.SetSegmentStart(segIndex);
			m_segStepRange.SetStepStart(m_colIdStartShow - preNum);
			bFindStart = TRUE;
		}

		if(totalNum > m_colIdEndShow && !bFindEnd)
		{
			m_segStepRange.SetSegmentEnd(segIndex);
			m_segStepRange.SetStepEnd(m_colIdEndShow - preNum);
			bFindEnd = TRUE;
		}

		++segIndex;
		pNodeSegItemInfo = pNodeSegItemInfo->pNext;
	}
																				
	return 0;
}

int CMySegDlg::ScrollCol(int pxHoriShift)
{
	m_colNum = GetStepNum();
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
				int colWidth = m_pxColW;
				if(colWidth - m_ssColLeftIndent > pxHoriShift)
				{
					m_ssColLeftIndent += pxHoriShift;
				}
				else
				{
					int remainder = pxHoriShift - (colWidth - m_ssColLeftIndent);
					for(int i = m_colIdStartShow + 1;i < m_colNum;remainder -= m_pxColW,++i)
					{
						if(remainder < m_pxColW)
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
					for(int i = m_colIdStartShow - 1;i >= 0;remainder -= m_pxColW,--i)
					{
						//=号在这里至关重要,20211122,孙文郎
						if(remainder <= m_pxColW)
						{
							m_colIdStartShow = i;
							m_ssColLeftIndent = m_pxColW - remainder;
							break;
						}
					}
				}
			}
		}

		int sum = m_pxColW - m_ssColLeftIndent;
		int i = m_colIdStartShow + 1;
		for(;i < m_colNum;++i)
		{
			sum += m_pxColW;
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
	
	StartEndShowColToSegStepRange();
	m_cdcW = GetStepNum(m_segStepRange.GetSegmentStart(),m_segStepRange.GetSegmentEnd()) * m_pxColW;

	m_horiScroll.m_curPixelPos += tempHoriShift;
	m_horiScroll.CalcSliderPos();
	Invalidate(FALSE);

	return 0;
}

int CMySegDlg::ScrollRow(int pxVertShift)
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
			m_ssRowTopIndent = 0;
		}
		else
		{
			if(pxVertShift > 0)
			{
				int rowH = m_pxRowH;
				if(rowH - m_ssRowTopIndent > pxVertShift)
				{
					m_ssRowTopIndent += pxVertShift;
				}
				else
				{
					int remainder = pxVertShift - (rowH - m_ssRowTopIndent);
					for(int i = m_rowIdStartShow + 1;i < m_rowNum;remainder -= m_pxRowH,++i)
					{
						if(remainder < m_pxRowH)
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
					for(int i = m_rowIdStartShow - 1;i >= 0;remainder -= m_pxRowH,--i)
					{
						//=号在这里至关重要,20211122,孙文郎
						if(remainder <= m_pxRowH)
						{
							m_rowIdStartShow = i;
							m_ssRowTopIndent = m_pxRowH - remainder;
							break;
						}
					}
				}
			}
		}

		int sum = m_pxRowH - m_ssRowTopIndent;
		int i = m_rowIdStartShow + 1;
		for(;i < m_rowNum;++i)
		{
			sum += m_pxRowH;
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
			}
		}
	}
	m_cdcH = GetHeight(m_rowIdStartShow,m_rowIdEndShow); 

	m_vertScroll.m_curPixelPos += tempVertShift;

	m_vertScroll.CalcSliderPos();
	Invalidate(FALSE);

	return 0;
}

int CMySegDlg::ScrollColToFixPos(int pxFixPos)
{
	int stepNum = GetStepNum();

	if(stepNum <= 0)
	{
		return -1;
	}
	if(pxFixPos < 0)
	{
		return -2;
	}

	if(stepNum > 0)
	{
		int sum = 0;
		for(int i = 0;i < stepNum;++i)
		{
			sum += m_pxColW;
			if(sum == pxFixPos)
			{
				m_colIdStartShow = i;
				m_ssColLeftIndent = 0;
				break;
			}
			else if(sum > pxFixPos)
			{
				m_colIdStartShow = i;
				m_ssColLeftIndent = m_pxColW - (sum - pxFixPos);
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

			int sum = m_pxColW - m_ssColLeftIndent;
			int i = m_colIdStartShow + 1;
			for(;i < stepNum;++i)
			{
				sum += m_pxColW;
				if(sum == m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Width())
				{
					m_colIdEndShow = i;
					m_esColRightIndent = m_pxColW - (sum - m_rcBody.Width());
					break;
				}
			}
		}
		else
		{
			m_colIdEndShow = stepNum - 1;
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
	
	StartEndShowColToSegStepRange();
	m_cdcW = GetStepNum(m_segStepRange.GetSegmentStart(),m_segStepRange.GetSegmentEnd()) * m_pxColW;

	return 0;
}

int CMySegDlg::ScrollRowToFixPos(int pxFixPos)
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
			sum += m_pxRowH;
			if(sum == pxFixPos)
			{
				m_rowIdStartShow = i;
				m_ssRowTopIndent = 0;
				break;
			}
			else if(sum > pxFixPos)
			{
				m_rowIdStartShow = i;
				m_ssRowTopIndent = m_pxRowH - (sum - pxFixPos);
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

			int sum = m_pxRowH - m_ssRowTopIndent;
			int i = m_rowIdStartShow + 1;
			for(;i < m_rowNum;++i)
			{
				sum += m_pxRowH;
				if(sum == m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = 0;
					break;
				}
				else if(sum > m_rcBody.Height())
				{
					m_rowIdEndShow = i;
					m_esRowBottomIndent = m_pxRowH - (sum - m_rcBody.Height());
					break;
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
	m_cdcH = (m_rowIdEndShow - m_rowIdStartShow + 1) * m_pxRowH;

	return 0;
}

void CMySegDlg::CalcScrollPos()
{
	m_bHaveGrad = IsExistGradientStep();

	int iRI = 0;
	if(IsShowGradientData())
	{
		iRI = m_iGradInfoW;
	}

	m_rcHeader = CRect(m_frameW,m_frameW,m_rcLst.Width() - iRI,m_pxHeaderH); 
	m_rcBody = CRect(m_frameW,m_pxHeaderH + m_frameW,m_rcLst.Width() - m_frameW - iRI,m_rcLst.Height() - m_frameW);
	m_rcGrad = CRect(m_rcLst.Width() - m_frameW - iRI,m_frameW,m_rcLst.Width() - m_frameW,m_rcLst.Height() - m_frameW);
	m_cdcH = m_rcLst.Height() - m_frameW * 2;
	
	if(m_totalColW > m_rcBody.Width())
	{
		m_horiScroll.m_bScrollNeedShow = TRUE;	

		m_horiScroll.m_curPixelPos = 0;
		for(int i = 0;i < m_colIdStartShow;++i)
		{
			m_horiScroll.m_curPixelPos += m_pxColW;
		}
		if(-1 != m_colIdStartShow)
		{
			m_horiScroll.m_curPixelPos += m_ssColLeftIndent;
		}
	}
	else
	{
		m_horiScroll.m_bScrollNeedShow = FALSE;	
		m_horiScroll.m_curPixelPos = 0;
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
		m_rcBody.bottom = m_rcLst.bottom - m_horiScroll.m_scrollH - m_frameW;
		if(m_vertScroll.m_bScrollNeedShow)
		{
			if(ESHVCM_HORI == m_eScrollHVCM)
			{
				m_horiScroll.m_rcScroll = CRect(0,m_rcLst.Height() - m_horiScroll.m_scrollH,m_rcLst.Width() - iRI,m_rcLst.Height());
			}
			else
			{
				m_horiScroll.m_rcScroll = CRect(0,m_rcLst.Height() - m_horiScroll.m_scrollH,m_rcLst.Width() - m_vertScroll.m_scrollW - iRI,m_rcLst.Height());
			}
		}
		else
		{
			m_horiScroll.m_rcScroll = CRect(0,m_rcLst.Height() - m_horiScroll.m_scrollH,m_rcLst.Width() - iRI,m_rcLst.Height());
		}

		m_horiScroll.m_pixelTotal = m_totalColW;
		m_horiScroll.m_pixelForShow = m_rcBody.Width();
		m_horiScroll.CalcSliderPos();

		m_cdcH = m_rcLst.Height() - m_frameW * 2 - m_horiScroll.m_scrollH;
	}
	
	if(m_vertScroll.m_bScrollNeedShow)
	{
		m_rcBody.right = m_rcLst.right - m_vertScroll.m_scrollW;
		m_rcHeader.right = m_rcLst.right - m_vertScroll.m_scrollW;
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
		                               
		m_vertScroll.m_pixelTotal = m_totalRowH;
		m_vertScroll.m_pixelForShow = m_rcBody.Height();
		m_vertScroll.CalcSliderPos();
	}
	
	return;
}

void CMySegDlg::InitHoriScrollParam()
{
	m_horiScroll.m_eScrollType = EST_HORI;
}

void CMySegDlg::InitVertScrollParam()
{
	m_vertScroll.m_eScrollType = EST_VERT;
}

float CMySegDlg::GetSegLastStepTem(int segId,int stepId)
{
	if(segId < 0 || segId >= m_lnkSegItemInfo.GetSize())
	{
		return 0;
	}
	CSegItemInfo* pSegItem = m_lnkSegItemInfo[segId];
	if(NULL == pSegItem)
	{
		return 0;
	}
	if(stepId < 0 || stepId >= pSegItem->m_lnkStepItemInfo.GetSize())
	{
		return 0;
	}
	if(0 == segId && 0 == stepId)
	{
		return 0;
	}

	float fTem = 0;
	CStepItemInfo* pStepItem = NULL;

	if(0 == segId)
	{
		pStepItem = pSegItem->m_lnkStepItemInfo[stepId - 1];
		if(NULL != pStepItem)
		{
			fTem = pStepItem->m_fAimTem;
		}
	}
	else
	{
		if(0 == stepId)
		{
			pSegItem = m_lnkSegItemInfo[segId - 1];
			if(NULL != pSegItem)
			{
				pStepItem = pSegItem->m_lnkStepItemInfo[pSegItem->m_lnkStepItemInfo.GetSize() - 1];
				if(NULL != pStepItem)
				{
					fTem = pStepItem->m_fAimTem;
				}
			}
		}
		else
		{
			pSegItem = m_lnkSegItemInfo[segId];
			if(NULL != pSegItem)
			{
				pStepItem = pSegItem->m_lnkStepItemInfo[stepId - 1];
				if(NULL != pStepItem)
				{
					fTem = pStepItem->m_fAimTem;
				}
			}
		}
	}

	return fTem;
}

BOOL CMySegDlg::GetMeltSegStartEndTemperature(double& dStartTem,double& dEndTem)
{
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	Node<CStepItemInfo>* pStepNode = NULL;
	while(NULL != pSegNode)
	{
		if(pSegNode->data.GetBeMeltSeg())
		{
			pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
			while(NULL != pStepNode)
			{
				if(3 == pStepNode->data.m_eChkType)
				{
					dEndTem = pStepNode->data.m_fAimTem;
					if(NULL != pStepNode->pPrevious)
					{
						dStartTem = pStepNode->pPrevious->data.m_fAimTem;
						return TRUE;
					}
					else
					{
						if(NULL != pSegNode->pPrevious)
						{
							Node<CStepItemInfo>* pTempStepNode = pSegNode->pPrevious->data.m_lnkStepItemInfo.GetTail();
							if(NULL != pTempStepNode)
							{
								dStartTem = pTempStepNode->data.m_fAimTem;
								return TRUE;
							}
							else
							{
								return FALSE;
							}
						}
						else
						{
							return FALSE;
						}
					}
				}

				pStepNode = pStepNode->pNext;
			}
		}

		pSegNode = pSegNode->pNext;
	}
	
	return FALSE;
}

void CMySegDlg::UpdateSegStepScrollPosInfo()
{
	CalcTotalColumnWidth();
	CalcScrollPos();
	CalcSegStepToShow();
	return;
}

BOOL CMySegDlg::UpdateCurSelectStepCycleInfo(int item,int sub,int cycle,BOOL bSelState,BOOL bRedraw /*= FALSE*/)
{
	if(item < 0 || sub < 0)
	{
		return FALSE;
	}

	if(m_lnkSegItemInfo.GetSize() > item)
	{
		CLinkList<CStepItemInfo>& lnkStepItem = m_lnkSegItemInfo.GetAt(item)->data.m_lnkStepItemInfo;
		if(lnkStepItem.GetSize() > sub)
		{
			lnkStepItem.GetAt(sub)->data.m_bSel = bSelState;
			if(bSelState)
			{
				m_curRunItem = item;
				m_curRunStep = sub;
				m_curRunCycle = cycle;
			}

			if(bRedraw && NULL != GetSafeHwnd())
			{
				Invalidate();
			}

			return TRUE;
		}
	}

	return FALSE;
}

void CMySegDlg::ResetCurItemSubCycle()
{
	m_priorItem = -1;
	m_priorStep = -1;
	m_curRunCycle = -1;
	m_curRunItem = -1;
	m_curRunStep = -1;
}

void CMySegDlg::EditStep(int iSegId,int iStepId)
{
	if(iSegId < 0 || iStepId < 0)
	{
		return;
	}

	if(m_lnkSegItemInfo.GetSize() < 1)
	{
		return;
	}

	CSegItemInfo* pSegItemInfo = m_lnkSegItemInfo[iSegId];
	if(NULL == pSegItemInfo)
	{
		return;
	}
	CStepItemInfo* pStepItemInfo = pSegItemInfo->m_lnkStepItemInfo[iStepId];
	if(NULL == pStepItemInfo)
	{
		return;
	}

	CMyStepEditInfo stepEditInfo;

	stepEditInfo.m_strSegName = pSegItemInfo->m_strSegName;
	stepEditInfo.m_strStepName = pStepItemInfo->m_strStepName;
	stepEditInfo.m_fAimTem = pStepItemInfo->m_fAimTem;
	stepEditInfo.m_bSelGradient = pStepItemInfo->m_bSelGrad;
	stepEditInfo.m_fGradientMaxTem = pStepItemInfo->m_fGradUpTem;
	stepEditInfo.m_holdSec = pStepItemInfo->m_iDelay;
	stepEditInfo.m_iCollect = pStepItemInfo->m_eChkType;
	stepEditInfo.m_fRate = pStepItemInfo->m_fRate;
	stepEditInfo.m_fIncrement = pStepItemInfo->m_fIncrement;
	stepEditInfo.m_bCollectExistExceptThisStep = GetBeHaveAmlCollectStepExcept(iSegId,iStepId);
	stepEditInfo.m_bCollectAllExistExceptThisStep = GetBeHaveMeltCollectStepExcept(iSegId,iStepId);
	stepEditInfo.m_bMeltSeg = pSegItemInfo->GetBeMeltSeg();

	CMyStepEditDlg stepEditDlg(stepEditInfo,this,m_bShieldGradientSet,m_bShowTemRate);

#ifndef PS96INSTRUMENT
	stepEditDlg.m_eInstruType = EIT_GENERAL;
#endif

	if(IDOK == stepEditDlg.DoModal())
	{
		pSegItemInfo->m_strSegName = stepEditDlg.m_stepEditInfo.m_strSegName;
		pStepItemInfo->m_strStepName = stepEditDlg.m_stepEditInfo.m_strStepName;
		pStepItemInfo->m_fAimTem = stepEditDlg.m_stepEditInfo.m_fAimTem;
		pStepItemInfo->m_bSelGrad = stepEditDlg.m_stepEditInfo.m_bSelGradient;
		pStepItemInfo->m_fGradUpTem = stepEditDlg.m_stepEditInfo.m_fGradientMaxTem;
		pStepItemInfo->m_iDelay = stepEditDlg.m_stepEditInfo.m_holdSec;
		pStepItemInfo->m_eChkType = (eStepCheckType)stepEditDlg.m_stepEditInfo.m_iCollect;
		pStepItemInfo->m_fRate = stepEditDlg.m_stepEditInfo.m_fRate;
		pStepItemInfo->m_fIncrement = stepEditDlg.m_stepEditInfo.m_fIncrement;

		if(pStepItemInfo->GetBeCollectAll() && 1 != pSegItemInfo->m_iCycleNum)	//熔曲程序段只能设置一个循环
		{
			pSegItemInfo->m_iCycleNum = 1;
		}

		//非熔曲步骤不处理梯度信息
		GradientSetChanged(FALSE);

		Invalidate(FALSE);
	}

	return;
}

void CMySegDlg::EditSegment(int segId)
{
	if(segId < 0 || segId >= m_lnkSegItemInfo.GetSize())
	{
		return;
	}

	if(m_lnkSegItemInfo.GetSize() < 1)
	{
		return;
	}

	CSegItemInfo* pSegItemInfo = m_lnkSegItemInfo[segId];
	if(NULL == pSegItemInfo)
	{
		return;
	}

	CMySegEditInfo segEditInfo;
	segEditInfo.m_strSegName = pSegItemInfo->m_strSegName;
	segEditInfo.m_cycleNum = pSegItemInfo->m_iCycleNum;
	segEditInfo.m_bMeltSeg = pSegItemInfo->GetBeMeltSeg();

	CMySegEditDlg segEditDlg(segEditInfo,this);
	if(IDOK == segEditDlg.DoModal())
	{
		pSegItemInfo->m_strSegName = segEditDlg.m_segEditInfo.m_strSegName;
		pSegItemInfo->m_iCycleNum = segEditDlg.m_segEditInfo.m_cycleNum;
		Invalidate(FALSE);
	}

	return;
}

BOOL CMySegDlg::GetBeHaveGetDataStep()
{
	Node<CSegItemInfo>* pNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pNode)
	{
		Node<CStepItemInfo>* pStepNode = pNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.GetBeCollect() || pStepNode->data.GetBeCollectAll())
			{
				return TRUE;
			}
			pStepNode = pStepNode->pNext;
		}
		pNode = pNode->pNext;
	}

	return FALSE;
}

BOOL CMySegDlg::GetSegmentBeHaveGetDataStep(int iSegId)
{
	Node<CSegItemInfo>* pNode = m_lnkSegItemInfo.GetAt(iSegId);
	if(NULL != pNode)
	{
		Node<CStepItemInfo>* pStepNode = pNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.GetBeCollect() || pStepNode->data.GetBeCollectAll())
			{
				return TRUE;
			}
			pStepNode = pStepNode->pNext;
		}
	}

	return FALSE;
}

BOOL CMySegDlg::GetBeHaveAmlCollectStepExcept(int iSegId,int iStepId)
{
	int iSegIndex = 0,iStepIndex = 0;

	Node<CSegItemInfo>* pNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pNode)
	{
		iStepIndex = 0;
		Node<CStepItemInfo>* pStepNode = pNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(!(iSegIndex == iSegId && iStepIndex == iStepId))
			{
				if(pStepNode->data.GetBeCollect())
				{
					return TRUE;
				}		
			}

			++iStepIndex;
			pStepNode = pStepNode->pNext;
		}

		++iSegIndex;
		pNode = pNode->pNext;
	}

	return FALSE;
}

BOOL CMySegDlg::GetBeHaveMeltCollectStepExcept(int iSegId,int iStepId)
{
	int iSegIndex = 0,iStepIndex = 0;

	Node<CSegItemInfo>* pNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pNode)
	{
		iStepIndex = 0;
		Node<CStepItemInfo>* pStepNode = pNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(!(iSegIndex == iSegId && iStepIndex == iStepId))
			{
				if(pStepNode->data.GetBeCollectAll())
				{
					return TRUE;
				}		
			}

			++iStepIndex;
			pStepNode = pStepNode->pNext;
		}

		++iSegIndex;
		pNode = pNode->pNext;
	}

	return FALSE;
}

void CMySegDlg::SetScrollArrowWH(int iWidth,int iHeight,BOOL bRedraw /*= FALSE*/)
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

void CMySegDlg::SetFourAngleBkColor(COLORREF clr,BOOL bRedraw /*= FALSE*/)
{
	m_clrFourAngleBk = clr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

COLORREF CMySegDlg::GetFourAngleBkColor()
{
	return m_clrFourAngleBk;
}

void CMySegDlg::SetSegStepName(int iStartSegIndex,BOOL bRedraw /*= FALSE*/)
{
	if(iStartSegIndex < 0 || iStartSegIndex >= m_lnkSegItemInfo.GetSize())
	{
		return;
	}

	int iIndex = iStartSegIndex;
	int iStepIndex = 0;

	Node<CStepItemInfo>* pStepNode = NULL;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetAt(iIndex);
	while(NULL != pSegNode)
	{
		pSegNode->data.m_strSegName = m_strSegDefName;
		pSegNode->data.m_strSegName.AppendFormat(_T("%d"),iIndex + 1);

		iStepIndex = 0;
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			pStepNode->data.m_strStepName = m_strStepDefName;
			pStepNode->data.m_strStepName.AppendFormat(_T("%d"),iStepIndex + 1);

			pStepNode = pStepNode->pNext;
			++iStepIndex;
		}

		pSegNode = pSegNode->pNext;
		++iIndex;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return;
}

void CMySegDlg::SetCollectFluTagType(ECOLLECTFLUTAGTYPE eType,BOOL bRedraw /*= FALSE*/)
{
	m_eCollectFluTagType = eType;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return;
}

ECOLLECTFLUTAGTYPE CMySegDlg::GetCollectFluTagType()
{
	return m_eCollectFluTagType;
}

void CMySegDlg::SetShieldGradientSetState(BOOL bData)
{
	m_bShieldGradientSet = bData;
}

BOOL CMySegDlg::GetShieldGradientSetState()
{
	return m_bShieldGradientSet;
}

void CMySegDlg::SetTemperatureRateShowState(BOOL bData)
{
	m_bShowTemRate = bData;
}

BOOL CMySegDlg::GetTemperatureRateShowState()
{
	return m_bShowTemRate;
}

int CMySegDlg::GetStepIndexAt(int iSegId,int iStepId)
{
	if(iSegId < 0)
	{
		return -1;
	}

	if(iSegId >= m_lnkSegItemInfo.GetSize())
	{
		return -1;
	}

	CSegItemInfo* pSegItemInfo = m_lnkSegItemInfo[iSegId];
	if(NULL == pSegItemInfo)
	{
		return -1;
	}

	if(iStepId < 0)
	{
		return -1;
	}

	if(iStepId >= pSegItemInfo->m_lnkStepItemInfo.GetSize())
	{
		return -1;
	}

	int iStepIndex = 0;
	for(int i = 0;i < iSegId;++i)
	{
		iStepIndex += m_lnkSegItemInfo[i]->GetStepNum();
	}
	iStepIndex += iStepId;

	return iStepIndex;
}

int CMySegDlg::SetSegmentSetpShow(int iSegId,int iStepId,BOOL bRedraw /*= FALSE*/)
{
	int iRet = 0;
	if(-1 == m_colIdStartShow)
	{
		iRet = -1;
		goto END;
	}

	if(-2 == m_colIdEndShow)
	{
		iRet = -2;
		goto END;
	}

	if(iSegId < 0)
	{
		iRet = -3;
		goto END;
	}

	if(iSegId >= m_lnkSegItemInfo.GetSize())
	{
		iRet = -4;
		goto END;
	}

	CSegItemInfo* pSegItemInfo = m_lnkSegItemInfo[iSegId];
	if(NULL == pSegItemInfo)
	{
		iRet = -5;
		goto END;
	}

	if(iStepId < 0)
	{
		iRet = -6;
		goto END;
	}

	if(iStepId >= pSegItemInfo->m_lnkStepItemInfo.GetSize())
	{
		iRet = -7;
		goto END;
	}

	int iStepIndex = GetStepIndexAt(iSegId,iStepId);
	if(iStepIndex < 0)
	{
		iRet = -8;
		goto END;
	}

	if(iStepIndex >= m_colIdStartShow && (iStepIndex < m_colIdEndShow || (iStepIndex == m_colIdEndShow && 0 == m_esColRightIndent)))
	{
		iRet = 1;
		goto END;
	}

	if(iStepIndex < m_colIdStartShow)
	{
		m_colIdStartShow = iStepIndex;
		m_ssColLeftIndent = 0;
		CalcSegStepToShow();
	}
	else if(iStepIndex >= m_colIdEndShow)
	{
		m_colIdStartShow += (iStepIndex - m_colIdEndShow);
		CalcSegStepToShow();
		if(-1 != m_colIdStartShow && -1 != m_colIdEndShow && m_esColRightIndent > 0)
		{
			m_ssColLeftIndent += (m_stepWidth - m_esColRightIndent);
			if(m_ssColLeftIndent >= m_stepWidth)
			{
				++m_colIdStartShow;
				m_ssColLeftIndent -= m_stepWidth;
			}
			CalcSegStepToShow();
		}
	}	

	CalcScrollPos();

END:
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return iRet;
}

int CMySegDlg::SetFuncEditCtrlFont(CString strFtName,int nHeight,int nWeight /*= FW_NORMAL*/,FONT_UNIT_TYPE fontUnitType /*= TYPE_PIXEL*/)
{
	int iRet = -1;
	if(NULL != m_edSegInfo.GetSafeHwnd())
	{
		m_edSegInfo.SetMyFont(strFtName,nHeight,nWeight,fontUnitType);
		m_edSegInfo.TextParamChanged();
		iRet = 2;
	}

	return iRet;
}

void CMySegDlg::SetGradientInfoUseState(BOOL bUse,BOOL bRedraw /*= FALSE*/)
{
	m_bUseGradInfo = bUse;
	GradientSetChanged(bRedraw);
	return;
}

BOOL CMySegDlg::GetGradientInfoUseState()
{
	return m_bUseGradInfo;
}

void CMySegDlg::SetGradientString(CString strData,BOOL bRedraw /*= FALSE*/)
{
	m_strGrad = strData;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMySegDlg::SetColumnString(CString strData,BOOL bRedraw /*= FALSE*/)
{
	m_strCol = strData;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMySegDlg::SetSegmentString(CString strData,BOOL bRedraw /*= FALSE*/)
{
	m_strSeg = strData;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMySegDlg::SetStepString(CString strData,BOOL bRedraw /*= FALSE*/)
{
	m_strStep = strData;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMySegDlg::SetRangeString(CString strData,BOOL bRedraw /*= FALSE*/)
{
	m_strRange = strData;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

BOOL CMySegDlg::IsSelectMultiSegment()
{
	int iNum = 0;

	Node<CStepItemInfo>* pStepNode = NULL;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel)
			{
				if(++iNum > 1)
				{
					return TRUE;
				}
				break;
			}

			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}
	
	return FALSE;
}

BOOL CMySegDlg::IsOnlyOneSegmentSelected()
{
	int iNum = 0;

	Node<CStepItemInfo>* pStepNode = NULL;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel)
			{
				if(++iNum > 1)
				{
					return FALSE;
				}
				break;
			}

			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}

	return 1 == iNum;
}

int CMySegDlg::GetSelectedSegmentCount()
{
	int iNum = 0;

	Node<CStepItemInfo>* pStepNode = NULL;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel)
			{
				++iNum;
				break;
			}

			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}

	return iNum;
}

int CMySegDlg::GetFirstSelectedSegmentId()
{
	int iIndex = -1;

	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	Node<CStepItemInfo>* pStepNode = NULL;
	while(NULL != pSegNode)
	{
		++iIndex;
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel)
			{
				return iIndex;
			}

			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}

	return iIndex;
}

CString CMySegDlg::GetGradientInfoString()
{
	if(!IsShowGradientData())
	{
		return _T("");
	}
	
	CString strTemp(_T(""));
	int iSegId = 0,iStepId = 0;
	CStepItemInfo* pStep = NULL;
	Node<CStepItemInfo>* pStepNode = NULL;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSel && pStepNode->data.m_bSelGrad)
			{
				pStep = &pStepNode->data;
				goto END;
			}
			pStepNode = pStepNode->pNext;
			++iStepId;
		}

		pSegNode = pSegNode->pNext;
		++iSegId;
	}

END:
	strTemp = m_strGrad + _T("\n");
	
	if(NULL == pStep)
	{
		strTemp.AppendFormat(_T("%s\n%s\n"),m_strSeg,m_strStep);
		strTemp += (m_strCol + _T("1\n") + m_strCol + _T("2\n") + m_strCol + _T("3\n") + m_strCol + _T("4\n") + m_strCol + _T("5\n") + m_strCol + _T("6\n"));
		strTemp += (m_strCol + _T("7\n") + m_strCol + _T("8\n") + m_strCol + _T("9\n") + m_strCol + _T("10\n") + m_strCol + _T("11\n") + m_strCol + _T("12\n"));
		strTemp += m_strRange;
	}
	else
	{
		strTemp.AppendFormat(_T("%s%d\n%s%d\n"),m_strSeg,iSegId + 1,m_strStep,iStepId + 1);
		float ColTemp[12] = {0};
		float DeltaT = 0; 
		float fCaliAimT = 0;
		float fCaliDeltaT = 0;
		pStep->GetStepGradientInfo(ColTemp,DeltaT,fCaliAimT,fCaliDeltaT);
		strTemp.AppendFormat(_T("%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n"),m_strCol,1,ColTemp[0],m_strCol,2,ColTemp[1],m_strCol,3,ColTemp[2],m_strCol,4,ColTemp[3],m_strCol,5,ColTemp[4],m_strCol,6,ColTemp[5]);
		strTemp.AppendFormat(_T("%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n%s%d %.2lf\n"),m_strCol,7,ColTemp[6],m_strCol,8,ColTemp[7],m_strCol,9,ColTemp[8],m_strCol,10,ColTemp[9],m_strCol,11,ColTemp[10],m_strCol,12,ColTemp[11]);
		strTemp.AppendFormat(_T("%s %.2lf"),m_strRange,pStep->m_fGradUpTem - pStep->m_fAimTem);
	}

	return strTemp;
}

int CMySegDlg::EndEdit()
{
	CString tempStr(_T(""));
	tempStr = m_edSegInfo.GetText();

	if(m_preEditDotInfo.m_seg >= m_lnkSegItemInfo.GetSize())
	{
		return 2;
	}

	CSegItemInfo* pSegItem = m_lnkSegItemInfo[m_preEditDotInfo.m_seg];
	if(NULL == pSegItem)
	{
		return 3;
	}
	
	if(EDP_CYCLE != m_preEditDotInfo.m_eDotPosType && EDP_SEGNAME != m_preEditDotInfo.m_eDotPosType && (m_preEditDotInfo.m_step >= pSegItem->m_lnkStepItemInfo.GetSize() || m_preEditDotInfo.m_step < 0))
	{
		return 4;
	}

	CStepItemInfo* pStepItem = pSegItem->m_lnkStepItemInfo[m_preEditDotInfo.m_step];
	if(EDP_CYCLE != m_preEditDotInfo.m_eDotPosType && EDP_SEGNAME != m_preEditDotInfo.m_eDotPosType)
	{
		if(NULL == pStepItem)
		{
			return 5;
		}
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	int tempInt = 0;
	float tempFloat = 0;
	switch(m_preEditDotInfo.m_eDotPosType)
	{
	case EDP_CYCLE: 
		tempInt = _ttoi(tempStr); 
		if(tempInt < m_iMinCycleNum || tempInt > m_iMaxCycleNum)
		{ 
			CString str1(_T(""));
			str1.Format(pLangInfo->GetText(236),m_iMinCycleNum,m_iMaxCycleNum);
			PopNormalTipInfo(str1);
			return 4;
		}	
		break;
	case EDP_SEGNAME: 
		if(tempStr.IsEmpty() || tempStr.GetLength() > 20)
		{
			PopNormalTipInfo(pLangInfo->GetText(233));
			return 8;
		}
		break;
	case EDP_TEM: tempFloat = _ttof(tempStr); 
		if(tempFloat < m_dMinTem || tempFloat > m_dMaxTem)
		{
			CString str1(_T(""));
			str1.Format(pLangInfo->GetText(229),m_dMinTem,m_dMaxTem);
			PopNormalTipInfo(str1);
			return 5;
		}
		if(pSegItem->GetBeMeltSeg())	//熔解程序段低温段步骤温度不能大于高温段步骤温度
		{
			if(1 == pStepItem->m_eChkType)
			{
				Node<CStepItemInfo>* pNextStepNode = pSegItem->m_lnkStepItemInfo.GetAt(m_preEditDotInfo.m_step)->pNext;
				if(NULL != pNextStepNode)
				{
					if(tempFloat >= pNextStepNode->data.m_fAimTem)
					{
						PopNormalTipInfo(pLangInfo->GetText(237));
						return 6;
					}
				}	
			}
			else if(3 == pStepItem->m_eChkType)
			{
				Node<CStepItemInfo>* pPreStepNode = pSegItem->m_lnkStepItemInfo.GetAt(m_preEditDotInfo.m_step)->pPrevious;
				if(NULL != pPreStepNode)
				{
					if(tempFloat <= pPreStepNode->data.m_fAimTem)
					{
						PopNormalTipInfo(pLangInfo->GetText(237));
						return 7;
					}
				}	
			}
		}

		break;
	case EDP_TIME: tempInt = _ttoi(tempStr); 
		if(tempInt < m_iMinDelaySec || tempInt > m_iMaxDelaySec)
		{
			PopNormalTipInfo(pLangInfo->GetText(231));
			return 6;
		}	
		break;
	default: break;
	}

	switch(m_preEditDotInfo.m_eDotPosType)
	{
	case EDP_CYCLE: pSegItem->m_iCycleNum = _ttoi(tempStr);	break;
	case EDP_SEGNAME: pSegItem->m_strSegName = tempStr;	break;
	case EDP_TEM: pStepItem->m_fAimTem = _ttof(tempStr);	break;
	case EDP_TIME: pStepItem->m_iDelay = _ttoi(tempStr);	break;
	case EDP_COLLECTFLU: break;
	default: break;
	}

	if(m_edSegInfo.IsWindowVisible())
	{
		m_edSegInfo.ShowWindow(SW_HIDE);
	}

	Invalidate(FALSE);

	return 0;
}

int CMySegDlg::GetWidth(int colStart,int colEnd)
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
		width += m_pxColW;
	}
	return width;
}

int CMySegDlg::GetHeight(int rowStart,int rowEnd)
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
		width += m_pxRowH;
	}
	return width;
}

BOOL CMySegDlg::IsExistGradientStep()
{
	Node<CStepItemInfo>* pStepNode = NULL;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		while(NULL != pStepNode)
		{
			if(pStepNode->data.m_bSelGrad)
			{
				return TRUE;
			}
			pStepNode = pStepNode->pNext;
		}

		pSegNode = pSegNode->pNext;
	}
	
	return FALSE;
}

BOOL CMySegDlg::IsShowGradientData()
{
	return m_bUseGradInfo && m_bHaveGrad;
}

int CMySegDlg::GradientSetChanged(BOOL bRedraw)
{
	m_bHaveGrad = IsExistGradientStep();

	CalcTotalColumnWidth();
	CalcScrollPos();
	CalcSegStepToShow();

	if(bRedraw)
	{
		Invalidate(FALSE);
	}

	return 0;
}

void CMySegDlg::CorrectSegmentName()
{
	CString strSource, strTarget;
	CLanguageInfo* pLanguage = CLanguageInfo::GetInstance();
	if (pLanguage->GetLanguage() == LANGUAGE_CHS)
	{
		strSource = pLanguage->GetText(57, LANGUAGE_ENG);
		strTarget = pLanguage->GetText(57, LANGUAGE_CHS);
	}
	else
	{
		strSource = pLanguage->GetText(57, LANGUAGE_CHS);
		strTarget = pLanguage->GetText(57, LANGUAGE_ENG);
	}

	int iSegIndex = 1;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		pSegNode->data.CorrectNameLanguage(strSource, strTarget);
		pSegNode = pSegNode->pNext;
		iSegIndex ++;
	}
}

void CMySegDlg::CorrectSegmengIndex()
{
	int iSegIndex = 1;
	Node<CSegItemInfo>* pSegNode = m_lnkSegItemInfo.GetHead();
	while(NULL != pSegNode)
	{
		pSegNode->data.CorrectIndex(iSegIndex);
		pSegNode = pSegNode->pNext;
		iSegIndex ++;
	}
}

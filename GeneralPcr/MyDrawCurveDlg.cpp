// DrawCurve.cpp : implementation file
//

#include "stdafx.h"
#include "MyDrawCurveDlg.h"
#include <GdiPlus.h>
#include "MyPublicFunc.h"
#include "GeneralPcrDlg.h"
#include "AlgorithmSet.h"
#include "AxisZoomDlg.h"
#include "GeneralPcr.h"
#include "afxdialogex.h"

using namespace Gdiplus;

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)
// CMyDrawCurveDlg dialog

IMPLEMENT_DYNAMIC(CMyDrawCurveDlg, CDialogEx)

CMyDrawCurveDlg::CMyDrawCurveDlg(CWnd* pParent /*=NULL*/,BOOL beFilter /*= FALSE*/)
	: CDialogEx(CMyDrawCurveDlg::IDD, pParent)
{
	m_horiStartValue = 0;
	m_horiEndValue = 10;
	m_vertStartValue = 0;
	m_vertEndValue = 10;
	m_horiStrOutStep = 2;
	m_vertStrOutStep = 2;
	m_horiStepNums = 10;
	m_vertStepNums = 10;
	m_horiStep = (m_horiEndValue - m_horiStartValue) / m_horiStepNums;
	m_vertStep = (m_vertEndValue - m_vertStartValue) / m_vertStepNums;

	m_horiStrClr = RGB(0X97,0X97,0X97);
	m_horiLineClr = RGB(0XE7,0XE7,0XE7);
	m_vertStrClr = RGB(0X97,0X97,0X97);
	m_vertLineClr = RGB(0XE7,0XE7,0XE7);
	m_horiBaseLineClr = RGB(0XE7,0XE7,0XE7);
	m_vertBaseLineClr = RGB(0XE7,0XE7,0XE7);
	m_xSptClr = RGB(0,0,0);	//X轴分割线Spt=Separator
	m_roundTagInnerClr = RGB(255,255,255);
	m_horiLineWidth = 1;
	m_vertLineWidth = 1;
	m_horiSptWidth = 1;
	m_curveWidth = 2;
	m_bShowVertLine = FALSE;
	m_bShowHoriLine = FALSE;
	m_bShowHoriSpt = FALSE;
	m_bShowRoundTag = FALSE;
	m_horiBaseLineWidth = 1;
	m_vertBaseLineWidth = 1;
	m_bShowHoriBaseLine = TRUE;
	m_bShowVertBaseLine = TRUE;
	m_iVertLineInterval = 1;

	m_roundTagLineWidth = 1;
	m_roundTagRadius = 5;

	m_figureStartX = 0;
	m_figureEndX = 0;
	m_figureStartY = 0;
	m_figureEndY = 0;

	m_horiDecPtLen = 0;
	m_iHoriDecPtLenSet = 0;
	m_vertDecPtLen = 0;
	m_realUsedHoriDotLen = m_horiDecPtLen;
	m_realUsedVertDotLen = m_vertDecPtLen;

	m_capHeight = 0;
	m_lgdHeight = 0;
	m_leftIndent = 0;
	m_rightIndent = 0;
	m_topIndent = 0;
	m_bottomIndent = 0;

	m_bHaveCap = TRUE;
	m_bHaveLgd = TRUE;
	m_bLeanX = FALSE;

	m_rgbBkClr = RGB(0XFF,0XFF,0XFF);
	m_left = 0;
	m_top = 0;

	m_pLineInfo = NULL;
	m_lineNum = 0;

	m_pLineInfo = new CLineInfo[1];
	if(NULL != m_pLineInfo)
	{
		m_lineNum = 1;
	}

	m_eCurveType = GENERALCURVE;
	m_eXAxisMode = NORMALXAXIS;

	m_bShowXAxisExplain = FALSE;
	m_strXAxisExplain = _T("");

	m_bExpressionAutoXYMinMax = TRUE;
	m_dAxisHoriRt = 1;
	m_dAxisVertRt = 1;

	m_bHaveThreLineFunc = FALSE;
	m_bUseThreLineFunc = FALSE; 

	m_bHaveData = FALSE;

	m_pMemDc = NULL; 
	m_hBitmap = NULL;
	m_bFstPaint = TRUE;
	m_fstPaintWidth = 0;
	m_fstPaintHeight = 0;
	m_bThreMove = FALSE;

	m_eLineClrMode = ECM_DEFAULT;

	m_strXAxisTag = _T("");
	m_strYAxisTag = _T("");

	m_bstdCrv = FALSE;
	m_stdFrameH = 41;

	m_clrFourAngleBk = RGB(0XFF,0XFF,0XFF);
	m_bRoundRectStyle = TRUE;
	m_iRoundAngleSize = 8;
	m_clrIndepent = Color(0XF1,0XF4,0XF9);

	m_bUseFloatFunc = FALSE;
	m_dYAxisMinOfMax = 1060;
	m_bShieldYAxisMinOfMax = FALSE;
	m_bShowExpressoin = FALSE;

	m_iEprVertInterval = 6;
	m_iEprXStart = 182;
	m_iEprYStart = 8;
	m_iEprEndY = 0;

	m_strEprTag.Empty();

	m_bUseMinYDecre = FALSE;
	m_dMinYDecreThre = 100;
	m_dMinYDecrement = 100;
}

CMyDrawCurveDlg::~CMyDrawCurveDlg()
{
	ClearLine();

	if(NULL != m_pMemDc)
	{
		m_pMemDc->DeleteDC();
	}
	MyDeletePtr(&m_pMemDc);

	if(NULL != m_hBitmap)
	{
		::DeleteObject(m_hBitmap);
	}
}

void CMyDrawCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyDrawCurveDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(ID_DRAWCURVEDLG_BT_CLOSE, &CMyDrawCurveDlg::OnBnClickedClose)
	ON_WM_CREATE()
	ON_MESSAGE(WM_UPDATE_CURVE, &CMyDrawCurveDlg::OnUpdateCurve)
	ON_COMMAND(ID_CURVE_MENUEXPORTPICTURE,OnCurveMenuExportPicture)  
END_MESSAGE_MAP()

// CMyDrawCurveDlg message handlers

BOOL CMyDrawCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	SetWindowPos(this,m_left,m_top,m_dlgSize.cx,m_dlgSize.cy,SWP_NOZORDER);
	if(m_bHaveCap)
	{
		CenterWindow();
		SetRoundRectParam(27,27,TRUE);
	}

	CreateCtrl();
	SetCtrlPos();
	InitCtrl();
	ShowCtrl();

	SetMyFont(m_stTxFont,30,FW_NORMAL,TYPE_PIXEL);
	SetMultiDpiAdaptive();

	return TRUE;
}

void CMyDrawCurveDlg::OnPaint()
{
#ifdef _DEBUG
	clock_t startMs = clock();
	clock_t tempExistMs1 = 0,tempExistMs2 = 0,tempExistMs3 = 0;
#endif

	CPaintDC dc(this);
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC,memDCLine,memDcTotal;
	memDC.CreateCompatibleDC(&dc);
	memDCLine.CreateCompatibleDC(&dc);
	memDcTotal.CreateCompatibleDC(&dc);

	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	CBitmap bitMapLine;
	bitMapLine.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmapline = memDCLine.SelectObject(&bitMapLine);

	CBitmap bitMapTotal;
	bitMapTotal.CreateCompatibleBitmap(&dc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmapTotal = memDcTotal.SelectObject(&bitMapTotal);

	int iFigureStartY = m_figureStartY;
	//控件显示多个标曲关系式
	m_iEprEndY = 0;
	if(m_bShowExpressoin && IsExistVisibleExpression())
	{
		m_iEprEndY = GetVisibleExpressionHeight();
		m_iEprEndY += m_iEprYStart;
	}
	int iEprTopVertIndent = 0;
	if(m_iEprEndY > m_topIndent)
	{
		iEprTopVertIndent = m_iEprEndY - m_topIndent;
	}
	if(iEprTopVertIndent > 0 && (iFigureStartY + iEprTopVertIndent) < m_figureEndY)
	{
		iFigureStartY += iEprTopVertIndent;
	}

	CRect rcTempFigure(m_figureStartX,iFigureStartY,m_figureEndX,m_figureEndY);
	if(m_bRoundRectStyle)
	{
		memDcTotal.FillSolidRect(&rcClient,RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
		memDcTotal.FillSolidRect(&rcTempFigure,m_clrFourAngleBk);
		DrawRoundRectangleNew(&memDcTotal,rcTempFigure.left,rcTempFigure.top,rcTempFigure.Width() - 1,rcTempFigure.Height() - 1,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,TRUE,Color(0XE7,0XE7,0XE7),1,TRUE,m_clrIndepent,1);
	}
	else
	{
		memDcTotal.FillSolidRect(&rcClient,m_rgbBkClr);
	} 

	CFont* pOldFont = memDC.SelectObject(&m_fontSet);

	if(!m_bThreMove || m_bFstPaint)
	{
		memDC.FillSolidRect(&rcClient,m_rgbBkClr);

		int figureWidth = m_figureEndX - m_figureStartX;
		int figureHeight = m_figureEndY - iFigureStartY;

		double horiStepPixel = double(figureWidth) / m_horiStepNums;
		double vertStepPixel = double(figureHeight) / m_vertStepNums;

		double horiRatio = horiStepPixel / m_horiStep;
		double vertRatio = vertStepPixel / m_vertStep;
		m_dAxisHoriRt = horiRatio;
		m_dAxisVertRt = vertRatio;

		m_intHoriStep = m_horiStep;
		m_intVertStep = m_vertStep;

		int iFrameStartY = iFigureStartY - 1,iFrameEndY = m_figureEndY + 2;
		if(iFrameStartY < 0)
		{
			iFrameStartY = 0;
		}
		if(iFrameEndY < 0)
		{
			iFrameEndY = 0;
		}
		CRect tempRc(m_figureStartX,iFrameStartY,m_figureEndX,iFrameEndY);	//防止右侧、顶部的边框被数据区域覆盖
		memDC.FillSolidRect(tempRc,RGB(0XFA,0XFA,0XFA));
		
		if(!m_bRoundRectStyle)
		{
			CBrush brush;
			brush.CreateSolidBrush(RGB(164,164,164));
			memDC.FrameRect(tempRc,&brush);
			brush.DeleteObject();
		}

		memDC.SetBkMode(TRANSPARENT);

		CPoint pt;
		CPen pen;

		//绘制水平基线
		if(m_bShowHoriBaseLine)
		{
			pen.CreatePen(PS_SOLID,m_horiBaseLineWidth,m_horiBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);

			double circleEndValue = m_vertEndValue/* + m_vertStep*/; //为了避免浮点数精度问题的影响
			int index = 0;

			if(0 == m_vertStep)
			{
				//防止陷入死循环
				m_vertStep = 1;
			}

			for(double i = m_vertStartValue + m_vertStep;i + m_intVertStep <= circleEndValue;i += m_vertStep)
			{
				pt.x = m_figureStartX + 1;
				pt.y = m_figureEndY - (++index) * vertStepPixel;

				memDC.MoveTo(pt);
				pt.x = m_figureEndX - 1;
				memDC.LineTo(pt);
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		if(m_bShowVertBaseLine)
		{
			pen.CreatePen(PS_SOLID,m_vertBaseLineWidth,m_vertBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);

			double circleEndValue = m_horiEndValue + m_horiStep; //为了避免浮点数精度问题的影响
			pt.x = m_figureStartX;
			int index = 0;
			
			double dVertLineIndexValue = m_horiStep * m_iVertLineInterval;
			if(0 == dVertLineIndexValue)
			{
				//防止陷入死循环
				dVertLineIndexValue = 1;
			}

			for(double i = m_horiStartValue + dVertLineIndexValue;i + m_intHoriStep < circleEndValue;i += dVertLineIndexValue)
			{
				index += m_iVertLineInterval;

				pt.y = iFigureStartY + 1;
				pt.x = m_figureStartX + index * horiStepPixel;
				memDC.MoveTo(pt);
				pt.y = m_figureEndY - 1;
				memDC.LineTo(pt);
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制X轴
		if(m_bShowHoriLine)
		{
			pen.CreatePen(PS_SOLID,m_horiLineWidth,m_horiLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = m_figureEndY;
			memDC.MoveTo(pt);
			pt.x = m_figureEndX;
			memDC.LineTo(pt);

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制Y轴
		if(m_bShowVertLine)
		{
			pen.CreatePen(PS_SOLID,m_vertLineWidth,m_vertLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = iFigureStartY;
			memDC.MoveTo(pt);
			pt.y = m_figureEndY;
			memDC.LineTo(pt);

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制底部间隔线
		if(m_bShowHoriSpt)
		{
			pen.CreatePen(PS_SOLID,m_horiBaseLineWidth,m_horiBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			for(double i = m_horiStartValue;i <= m_horiEndValue;i += m_horiStep)
			{
				pt.y = m_figureEndY;
				memDC.MoveTo(pt);
				pt.y = m_figureEndY + 10;
				memDC.LineTo(pt);
				pt.x += horiStepPixel;
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//显示坐标轴内容说明
		if(m_bShowXAxisExplain)
		{
			CRect rcExplain(rcClient);
			rcExplain.bottom = m_topIndent;
			CSize strSize = memDC.GetTextExtent(m_strXAxisExplain);
			memDC.TextOut(rcExplain.Width() - strSize.cx - 10,(rcExplain.Height() - strSize.cy) / 2,m_strXAxisExplain);
		}

#ifdef _DEBUG
		tempExistMs1 = clock() - startMs;
		++tempExistMs1;
#endif

		if(NULL != m_pLineInfo)
		{
			COLORREF clrOld = memDC.SetTextColor(RGB(0X39,0X45,0X65));
			memDC.DrawText(m_strEprTag,m_rcEprTag,DT_LEFT | DT_TOP | DT_SINGLELINE);
			
			//绘制顶部显示的标曲关系式
			if(m_bShowExpressoin && GetVisibleExpressionHeight())
			{
				CFont* pOldFontExpressoin = memDC.SelectObject(&m_fontExpression.m_fontSet);
				int iEprStartY = m_iEprYStart;
				for(int i = 0;i < m_lineNum;++i)
				{
					if(m_pLineInfo[i].IsExpressionVisible())
					{
						CString strExpression = m_pLineInfo[i].GetExpressionString();
						memDC.SetTextColor(m_pLineInfo[i].GetLineClr(ECM_DEFAULT));
						memDC.TextOut(m_iEprXStart,iEprStartY,strExpression);
						iEprStartY += (m_iEprVertInterval + m_fontExpression.m_logFont.lfHeight);
					}
				}
				memDC.SelectObject(pOldFontExpressoin);
			}

			memDC.SetTextColor(clrOld);
			memDCLine.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

			//DrawLineGraphics(&memDCLine, horiRatio, vertRatio);
			DrawLineGDI(&memDCLine, horiRatio, vertRatio);
		}

		memDC.BitBlt(m_figureStartX,iFigureStartY,m_figureEndX - m_figureStartX,m_figureEndY - iFigureStartY,&memDCLine,m_figureStartX,iFigureStartY,SRCCOPY);

#ifdef _DEBUG
		tempExistMs2 = clock() - startMs;
		++tempExistMs2;
#endif

		//绘制垂直文字
		CString vertStr(_T("")),strFormat(_T(""));
		strFormat = _T("%.");
		strFormat.AppendFormat(_T("%d"),m_realUsedVertDotLen);
		strFormat += (_T("lf"));
		memDC.SetTextAlign(TA_CENTER);
		memDC.SetTextColor(RGB(0x56,0x56,0x56));
		memDC.SetTextAlign(TA_LEFT | TA_TOP);
		memDC.SelectObject(&m_fontSet);
		int strStartPos = m_figureEndY;
		double strOutStep = m_vertStep * m_vertStrOutStep;
		if(0 == strOutStep)
		{
			strOutStep = 100;
		}
		//int strOutStepPixel = vertStepPixel * m_vertStrOutStep;	//可能会导致精度不够问题
		
		for(double i = m_vertStartValue,j = 0;i <= m_vertEndValue;i += strOutStep,++j)
		{
			vertStr.Format(strFormat,i);
			CSize strSize = memDC.GetTextExtent(vertStr);
			strStartPos = m_figureEndY - (i - m_vertStartValue) * vertRatio;
			memDC.TextOut(m_figureStartX - strSize.cx - 10,strStartPos - strSize.cy / 2,vertStr);

			if(i + strOutStep > m_vertEndValue && (m_vertEndValue - i >= m_vertStep * 0.95))	
			{
				vertStr.Format(strFormat,m_vertEndValue);
				strStartPos = iFigureStartY;
				memDC.TextOut(m_figureStartX - strSize.cx - 10,strStartPos - strSize.cy / 2,vertStr);
				break;
			}
		}

		//绘制水平文字
		strFormat = _T("%.");
		strFormat.AppendFormat(_T("%d"),m_realUsedHoriDotLen);
		strFormat += (_T("lf"));
		CFont* pFont = memDC.GetCurrentFont();
		LOGFONT logFont;
		pFont->GetLogFont(&logFont);
		if(m_bLeanX)
		{
			logFont.lfEscapement = 900;
		}

		HFONT hFont = CreateFontIndirect(&logFont);
		memDC.SelectObject(hFont);
		CString horiStr(_T(""));
		strStartPos = m_figureStartX;
		strOutStep = m_horiStep * m_horiStrOutStep;
		if(0 == strOutStep)
		{
			strOutStep = 100;
		}
		//strOutStepPixel = horiStepPixel * m_horiStrOutStep;
		int index = 0;
		Node<CMyPoint> *pNodeMyPt = NULL;
		for(double i = m_horiStartValue;i <= m_horiEndValue;i += strOutStep)
		{
			strStartPos = m_figureStartX + (i - m_horiStartValue) * horiRatio;
			if(NORMALXAXIS == m_eXAxisMode)
			{
				horiStr.Format(strFormat,i);
			}
			else
			{
				int curSecond = int(i) / 1000;
				horiStr.Format(_T("%01d:%02d:%02d"),curSecond / 3600,(curSecond % 3600) / 60,curSecond % 60);
			}
			CSize strSize = memDC.GetTextExtent(horiStr);
			
			if(m_bLeanX)
			{
				memDC.TextOut(strStartPos - strSize.cy / 2,m_figureEndY + 10 + strSize.cx,horiStr);
			}
			else
			{
				int curEndX = strStartPos + strSize.cx / 2;
				if(curEndX > m_figureEndX)
				{
					strStartPos -= (curEndX - m_figureEndX);
				}
				memDC.TextOut(strStartPos - strSize.cx / 2,m_figureEndY + 10,horiStr);
			}	

			if(i + strOutStep > m_horiEndValue && (m_horiEndValue - i >= /*m_horiStep * 0.9*/ strOutStep * 0.6) && m_realUsedHoriDotLen < 2)	
			{
				if(NORMALXAXIS == m_eXAxisMode)
				{
					horiStr.Format(strFormat,m_horiEndValue);
				}
				else
				{
					int curSecond = int(m_horiEndValue) / 1000;
					horiStr.Format(_T("%01d:%02d:%02d"),curSecond / 3600,(curSecond % 3600) / 60,curSecond % 60);
				}

				strStartPos = m_figureEndX;
				if(m_bLeanX)
				{
					memDC.TextOut(strStartPos - strSize.cy / 2,m_figureEndY + 10 + strSize.cx,horiStr);
				}
				else
				{
					if(strSize.cx / 2 > m_rightIndent)
					{
						strStartPos -= (strSize.cx / 2 - m_rightIndent + 10);
					}
					memDC.TextOut(strStartPos - strSize.cx / 2,m_figureEndY + 10,horiStr);
				}	

				break;
			}
		}

		if(_T("") != m_strXAxisTag)
		{
			int x = m_figureEndX - 10;
			int y = m_figureEndY - 10;
			CSize strSize = memDC.GetTextExtent(m_strXAxisTag);
			memDC.TextOut(x - strSize.cx,y - strSize.cy,m_strXAxisTag);
		}

		if(_T("") != m_strYAxisTag)
		{
			int x = m_figureStartX + 10;
			int y = iFigureStartY + 10;
			CSize strSize = memDC.GetTextExtent(m_strYAxisTag);
			memDC.TextOut(x,y,m_strYAxisTag);
		}

		DeleteObject(hFont);
	}
	
	if(!m_bThreMove)
	{
		if(NULL != m_pMemDc)
		{
			m_pMemDc->DeleteDC();
			MyDeletePtr(&m_pMemDc);

			if(NULL != m_hBitmap)
			{
				::DeleteObject(m_hBitmap);
			}
		}
		m_pMemDc = new CDC;
		BOOL ret = m_pMemDc->CreateCompatibleDC(&memDC);
		m_hBitmap = ::CreateCompatibleBitmap(memDC.m_hDC,rcClient.Width(),rcClient.Height()); //创建兼容位图
		m_pMemDc->SelectObject(m_hBitmap);	
		m_pMemDc->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

		m_fstPaintWidth = rcClient.Width();
		m_fstPaintHeight = rcClient.Height();
		m_bFstPaint = FALSE;
	}
	else
	{
		memDC.BitBlt(0,0,m_fstPaintWidth,m_fstPaintHeight,m_pMemDc,0,0,SRCCOPY);
		m_bThreMove = FALSE;
	}

	if(m_bHaveThreLineFunc && m_bUseThreLineFunc && m_bHaveData)
	{
		DrawThreLine(&memDC);
	}

	if(m_bRoundRectStyle)
	{
		//检查使用稳定性
		memDC.TransparentBlt(rcTempFigure.left,rcTempFigure.top,rcTempFigure.Width(),rcTempFigure.Height(),&memDcTotal,rcTempFigure.left,rcTempFigure.top,rcTempFigure.Width(),rcTempFigure.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
	}

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOldBitmap);
	memDC.SelectObject(pOldFont);
	bitMap.DeleteObject();
	memDC.DeleteDC();

	memDCLine.SelectObject(pOldBitmapline);
	//memDCLine.SelectObject(pOldFont);
	bitMapLine.DeleteObject();
	memDCLine.DeleteDC();
	  
	memDcTotal.SelectObject(pOldBitmapTotal);
	bitMapTotal.DeleteObject();
	memDcTotal.DeleteDC();

#ifdef _DEBUG
	clock_t endMs = clock();
	clock_t existMs = endMs - startMs;
	++existMs;
#endif
}

//绘制线段,使用Gdi+,使用抗锯齿处理
void CMyDrawCurveDlg::DrawLineGraphics(CDC* pDC, double dHoriRatio, double dVertRatio)
{
	Graphics graphics(pDC->m_hDC);
	graphics.SetInterpolationMode(InterpolationMode::InterpolationModeBicubic);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);
	COLORREF lineClr = 0;
	for(int i = 0;i < m_lineNum;++i)
	{	
		if(!m_pLineInfo[i].m_bShow)
		{
			continue;
		}	

		lineClr = m_pLineInfo[i].GetLineClr(m_eLineClrMode);

		BYTE r = lineClr & 0XFF;
		BYTE g = (lineClr >> 8) & 0XFF;
		BYTE b = (lineClr >> 16) & 0XFF;
		Pen gdiplusPen(Color(r,g,b),m_curveWidth);

		if(STDLINE == m_eCurveType)
		{
			Point ptStart,ptEnd;
			if(m_pLineInfo[i].m_linearFitData.xLen > 1)
			{
				ptStart.X = (m_pLineInfo[i].m_linearMinPt.m_X  - m_horiStartValue) * dHoriRatio + m_figureStartX;
				ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearMinPt.m_Y - m_vertStartValue) * dVertRatio;
				ptEnd.X = (m_pLineInfo[i].m_linearMaxPt.m_X - m_horiStartValue) * dHoriRatio + m_figureStartX;
				ptEnd.Y = m_figureEndY - (m_pLineInfo[i].m_linearMaxPt.m_Y - m_vertStartValue) * dVertRatio;
				graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);	
			}

			for(int j = 0;j < m_pLineInfo[i].m_linearFitData.xLen;++j)
			{
				ptStart.X = (m_pLineInfo[i].m_linearFitData.pDataX[j] - m_horiStartValue) * dHoriRatio + m_figureStartX;
				ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearFitData.pDataY[j] - m_vertStartValue) * dVertRatio;
				graphics.DrawEllipse(&gdiplusPen,ptStart.X,ptStart.Y,4,4);
			}
		}
		else
		{   
			int dataNum = m_pLineInfo[i].m_lnkDots.GetSize();
			if(dataNum > 0)
			{
				Point* pCurvePt = new Point[dataNum];
				Node<CMyPoint>* pNode = m_pLineInfo[i].m_lnkDots.GetHead();
				int index = 0;
				while(NULL != pNode)
				{
					pCurvePt[index].X = (pNode->data.m_X - m_horiStartValue) * dHoriRatio + m_figureStartX;
					pCurvePt[index].Y = m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * dVertRatio;

					pNode->data.m_xPixel = pCurvePt[index].X;
					pNode->data.m_yPixel = pCurvePt[index].Y;
					++index;

					pNode = pNode->pNext;
				}

				//绘制圆圈
				if(m_bShowRoundTag)
				{
					CPen pen;
					pen.CreatePen(PS_SOLID,m_roundTagLineWidth,lineClr);
					pDC->SelectObject(&pen);
					CBrush brush;
					brush.CreateSolidBrush(m_roundTagInnerClr);
					pDC->SelectObject(&brush);
					for(int k = 0;k < dataNum;++k)
					{
						pDC->Ellipse(pCurvePt->X - m_roundTagRadius,pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
					}
					brush.DeleteObject();
					pen.DeleteObject();
				}

				graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum);
				delete [] pCurvePt;
			}
		}
	}

	//添加高亮显示
	for(int i = 0;i < m_vecHighLightIndex.size();++i)
	{
		int lineIndex = m_vecHighLightIndex[i];
		if(!m_pLineInfo[lineIndex].m_bShow)
		{
			continue;
		}	

		lineClr = m_pLineInfo[lineIndex].GetLineClr(m_eLineClrMode);

		BYTE r = lineClr & 0XFF;
		BYTE g = (lineClr >> 8) & 0XFF;
		BYTE b = (lineClr >> 16) & 0XFF;
		Pen gdiplusPen(Color(r,g,b),5);

		{   
			int dataNum = m_pLineInfo[lineIndex].m_lnkDots.GetSize();
			if(dataNum > 0)
			{
				Point* pCurvePt = new Point[dataNum];
				Node<CMyPoint>* pNode = m_pLineInfo[lineIndex].m_lnkDots.GetHead();
				int index = 0;
				while(NULL != pNode)
				{
					pCurvePt[index].X = (pNode->data.m_X - m_horiStartValue) * dHoriRatio + m_figureStartX;
					pCurvePt[index].Y = m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * dVertRatio;

					pNode->data.m_xPixel = pCurvePt[index].X;
					pNode->data.m_yPixel = pCurvePt[index].Y;
					++index;

					pNode = pNode->pNext;
				}

				//绘制圆圈
				if(m_bShowRoundTag)
				{
					CPen pen;
					pen.CreatePen(PS_SOLID,m_roundTagLineWidth,lineClr);
					pDC->SelectObject(&pen);
					CBrush brush;
					brush.CreateSolidBrush(m_roundTagInnerClr);
					pDC->SelectObject(&brush);
					for(int k = 0;k < dataNum;++k)
					{
						pDC->Ellipse(pCurvePt->X - m_roundTagRadius, pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
					}
					brush.DeleteObject();
					pen.DeleteObject();
				}

				graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum);
				delete [] pCurvePt;
			}
		}
	}

	graphics.ReleaseHDC(pDC->m_hDC);
}

void CMyDrawCurveDlg::DrawLineGDI(CDC* pDC, double dHoriRatio, double dVertRatio)
{
	for(int i = 0;i < m_lineNum;++i)
	{	
		if(!m_pLineInfo[i].m_bShow)
		{
			continue;
		}	

		CPen penLine;
		penLine.CreatePen(PS_SOLID, m_curveWidth, m_pLineInfo[i].GetLineClr(m_eLineClrMode));
		pDC->SelectObject(&penLine);
		if(STDLINE == m_eCurveType)
		{
			Point ptStart,ptEnd;
			if(m_pLineInfo[i].m_linearFitData.xLen > 1)
			{
				ptStart.X = (m_pLineInfo[i].m_linearMinPt.m_X  - m_horiStartValue) * dHoriRatio + m_figureStartX;
				ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearMinPt.m_Y - m_vertStartValue) * dVertRatio;
				ptEnd.X = (m_pLineInfo[i].m_linearMaxPt.m_X - m_horiStartValue) * dHoriRatio + m_figureStartX;
				ptEnd.Y = m_figureEndY - (m_pLineInfo[i].m_linearMaxPt.m_Y - m_vertStartValue) * dVertRatio;
				pDC->MoveTo(ptStart.X, ptStart.Y);
				pDC->LineTo(ptEnd.X , ptEnd.Y);
			}

			for(int j = 0;j < m_pLineInfo[i].m_linearFitData.xLen;++j)
			{
				ptStart.X = (m_pLineInfo[i].m_linearFitData.pDataX[j] - m_horiStartValue) * dHoriRatio + m_figureStartX;
				ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearFitData.pDataY[j] - m_vertStartValue) * dVertRatio;
				//graphics.DrawEllipse(&gdiplusPen,ptStart.X,ptStart.Y,4,4);
				pDC->Ellipse(ptStart.X, ptStart.Y, ptStart.X+4, ptStart.Y+4);
			}
		}
		else
		{   
			int dataNum = m_pLineInfo[i].m_lnkDots.GetSize();
			if(dataNum > 0)
			{
				Node<CMyPoint>* pNode = m_pLineInfo[i].m_lnkDots.GetHead();
				int index = 0;
				while(NULL != pNode)
				{
					int iPointX = int((pNode->data.m_X - m_horiStartValue) * dHoriRatio + m_figureStartX);
					int iPointY = int(m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * dVertRatio);
					pNode->data.m_xPixel = iPointX;
					pNode->data.m_yPixel = iPointY;

					if (index == 0)
						pDC->MoveTo(iPointX, iPointY);
					else
						pDC->LineTo(iPointX, iPointY);

					pNode = pNode->pNext;
					++index;

					//graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum);
				}

				//绘制圆圈
				//if(m_bShowRoundTag)
				//{
				//	CPen pen;
				//	pen.CreatePen(PS_SOLID,m_roundTagLineWidth, m_pLineInfo[i].GetLineClr(m_eLineClrMode));
				//	pDC->SelectObject(&pen);
				//	CBrush brush;
				//	brush.CreateSolidBrush(m_roundTagInnerClr);
				//	pDC->SelectObject(&brush);
				//	for(int k = 0;k < dataNum;++k)
				//	{
				//		pDC->Ellipse(pCurvePt->X - m_roundTagRadius,pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
				//	}
				//	brush.DeleteObject();
				//	pen.DeleteObject();
				//}
			}
		}
	}

	//添加高亮显示
	for(int i = 0;i < m_vecHighLightIndex.size();++i)
	{
		int lineIndex = m_vecHighLightIndex[i];
		if(!m_pLineInfo[lineIndex].m_bShow)
		{
			continue;
		}	

		CPen penLine;
		penLine.CreatePen(PS_SOLID, m_curveWidth, m_pLineInfo[i].GetLineClr(m_eLineClrMode));
		pDC->SelectObject(&penLine);
		int dataNum = m_pLineInfo[lineIndex].m_lnkDots.GetSize();
		if(dataNum > 0)
		{
			Node<CMyPoint>* pNode = m_pLineInfo[lineIndex].m_lnkDots.GetHead();
			int index = 0;
			while(NULL != pNode)
			{
				int iPointX = int((pNode->data.m_X - m_horiStartValue) * dHoriRatio + m_figureStartX);
				int iPointY = int(m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * dVertRatio);

				pNode->data.m_xPixel = iPointX;
				pNode->data.m_yPixel = iPointY;

				if (index == 0)
					pDC->MoveTo(iPointX, iPointY);
				else
					pDC->LineTo(iPointX, iPointY);

				//graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum);
				pNode = pNode->pNext;
				++index;
			}

			//绘制圆圈
			//if(m_bShowRoundTag)
			//{
			//	CPen pen;
			//	pen.CreatePen(PS_SOLID,m_roundTagLineWidth, m_pLineInfo[i].GetLineClr(m_eLineClrMode));
			//	pDC->SelectObject(&pen);
			//	CBrush brush;
			//	brush.CreateSolidBrush(m_roundTagInnerClr);
			//	pDC->SelectObject(&brush);
			//	for(int k = 0;k < dataNum;++k)
			//	{
			//		pDC->Ellipse(pCurvePt->X - m_roundTagRadius,pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
			//	}
			//	brush.DeleteObject();
			//	pen.DeleteObject();
			//}
		}
	}
}

int CMyDrawCurveDlg::DrawThreLine(CDC* pDC)
{
	if(NULL == pDC)
	{
		return -1;
	}

	int iChanNum = m_vecChanThreInfo.size();
	if(iChanNum < 1)
	{
		return 1;
	}

	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeAntiAlias);

	//阈值线按照倒序绘制，防止第一层的阈值线和选中拖动的阈值线不一致
	for(int i = iChanNum - 1;i >= 0;--i)
	{
		CThreLineInfo* pThreLineInfo = m_vecChanThreInfo[i].GetCurSelThreInfo();
		if(NULL == pThreLineInfo)
		{
			continue;
		}

		BOOL bShowLine = FALSE;
		if(pThreLineInfo->GetBeShow())
		{
			Pen gdiplusPen(pThreLineInfo->m_clrThreLine,pThreLineInfo->m_threLineNormalWidth);
			Point ptStart,ptEnd;
			ptStart.X = m_figureStartX;
			if(pThreLineInfo->GetBeFirstDrawThreLline())
			{
				if(pThreLineInfo->m_bAutoThre)
				{
					if (pThreLineInfo->m_dThreAuto >= m_vertStartValue && pThreLineInfo->m_dThreAuto <= m_vertEndValue)
						bShowLine = TRUE;
					ptStart.Y = m_figureEndY - (pThreLineInfo->m_dThreAuto - m_vertStartValue) * m_dAxisVertRt;
				}
				else
				{
					if (pThreLineInfo->m_dThre >= m_vertStartValue && pThreLineInfo->m_dThre <= m_vertEndValue)
						bShowLine = TRUE;
					ptStart.Y = m_figureEndY - (pThreLineInfo->m_dThre - m_vertStartValue) * m_dAxisVertRt;
				}
				pThreLineInfo->m_threLinePixelH = ptStart.Y;
				pThreLineInfo->SetBeFirstDrawThreLline(FALSE);
			}
			else
			{
				ptStart.Y = pThreLineInfo->m_threLinePixelH;
			}

			if (bShowLine)
			{
				ptEnd.X = m_figureEndX;
				ptEnd.Y = ptStart.Y;
				graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);	
				graphics.ReleaseHDC(pDC->m_hDC);			

				pThreLineInfo->m_rcThreLine.left = m_figureStartX;
				pThreLineInfo->m_rcThreLine.right = m_figureEndX;
				pThreLineInfo->m_rcThreLine.top = ptStart.Y - pThreLineInfo->m_threLineDiscernHalfH;
				pThreLineInfo->m_rcThreLine.bottom = ptStart.Y + pThreLineInfo->m_threLineNormalWidth + pThreLineInfo->m_threLineDiscernHalfH;
			}
		}
	}

	return 0;
}

BOOL CMyDrawCurveDlg::CurveToPic(vector<BOOL>& vecCurMonitorChan,CString strPicName)
{
#ifdef _DEBUG
	clock_t startMs = clock();
	clock_t tempExistMs1 = 0,tempExistMs2 = 0,tempExistMs3 = 0;
#endif

	CDC* pDc = GetDC();
	if(NULL == pDc)
	{
		return FALSE;
	}
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	memDC.CreateCompatibleDC(pDc);

	CDC memDCLine;
	memDCLine.CreateCompatibleDC(pDc);

	CBitmap bitMapLine;
	bitMapLine.CreateCompatibleBitmap(pDc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmapline = memDCLine.SelectObject(&bitMapLine);

	//定义图形大小
	int iWidth = rcClient.Width();
	int iHeight = rcClient.Height();
	int iPixel  = 16;

	//图形格式参数
	LPBITMAPINFO lpbmih = new BITMAPINFO;
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;
	lpbmih->bmiHeader.biBitCount = iPixel;
	lpbmih->bmiHeader.biCompression = BI_RGB;
	lpbmih->bmiHeader.biSizeImage = 0;
	lpbmih->bmiHeader.biXPelsPerMeter = 0;
	lpbmih->bmiHeader.biYPelsPerMeter = 0;
	lpbmih->bmiHeader.biClrUsed = 0;
	lpbmih->bmiHeader.biClrImportant = 0;

	//创建位图数据
	HBITMAP hBitMap = NULL;
	BYTE *pBits = NULL;

	hBitMap = CreateDIBSection(memDC.m_hDC,lpbmih,DIB_PAL_COLORS,(void **)&pBits,NULL,0);
	CBitmap bitMap;
	bitMap.Attach(hBitMap);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	CFont* pOldFont = memDC.SelectObject(&m_fontSet);

	if(!m_bThreMove || m_bFstPaint)
	{
		memDC.FillSolidRect(&rcClient,m_rgbBkClr);

		int figureWidth = m_figureEndX - m_figureStartX;
		int figureHeight = m_figureEndY - m_figureStartY;

		double horiStepPixel = double(figureWidth) / m_horiStepNums;
		double vertStepPixel = double(figureHeight) / m_vertStepNums;

		double horiRatio = horiStepPixel / m_horiStep;
		double vertRatio = vertStepPixel / m_vertStep;
		m_dAxisHoriRt = horiRatio;
		m_dAxisVertRt = vertRatio;

		m_intHoriStep = m_horiStep;
		m_intVertStep = m_vertStep;

		CRect tempRc(m_figureStartX,m_figureStartY,m_figureEndX,m_figureEndY);
		memDC.FillSolidRect(tempRc,RGB(245,247,250));
		CBrush brush;
		brush.CreateSolidBrush(RGB(164,164,164));
		memDC.FrameRect(tempRc,&brush);
		brush.DeleteObject();

		memDC.SetBkMode(TRANSPARENT);

		CPoint pt;
		CPen pen;

		//绘制水平基线
		if(m_bShowHoriBaseLine)
		{
			pen.CreatePen(PS_SOLID,m_horiBaseLineWidth,m_horiBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = m_figureEndY;
			memDC.MoveTo(pt);
			pt.x = m_figureEndX;
			memDC.LineTo(pt);
			pt.y = m_figureEndY;

			double circleEndValue = m_vertEndValue/* + m_vertStep*/; //为了避免浮点数精度问题的影响
			int index = 0;
			for(double i = m_vertStartValue + m_vertStep;i + m_intVertStep <= circleEndValue;i += m_vertStep)
			{
				pt.x = m_figureStartX + 1;
				pt.y = m_figureEndY - (++index) * vertStepPixel;
				memDC.MoveTo(pt);
				pt.x = m_figureEndX - 1;
				memDC.LineTo(pt);
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		if(m_bShowVertBaseLine)
		{
			pen.CreatePen(PS_SOLID,m_vertBaseLineWidth,m_vertBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);

			double circleEndValue = m_horiEndValue + m_horiStep; //为了避免浮点数精度问题的影响
			pt.x = m_figureStartX;
			int index = 0;
			for(double i = m_horiStartValue + m_horiStep;i + m_intHoriStep < circleEndValue;i += m_horiStep)
			{
				pt.y = m_figureStartY + 1;
				pt.x = m_figureStartX + (++index) * horiStepPixel;
				memDC.MoveTo(pt);
				pt.y = m_figureEndY - 1;
				memDC.LineTo(pt);
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制X轴
		if(m_bShowHoriLine)
		{
			pen.CreatePen(PS_SOLID,m_horiLineWidth,m_horiLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = m_figureEndY;
			memDC.MoveTo(pt);
			pt.x = m_figureEndX;
			memDC.LineTo(pt);

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制Y轴
		if(m_bShowVertLine)
		{
			pen.CreatePen(PS_SOLID,m_vertLineWidth,m_vertLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = m_figureStartY;
			memDC.MoveTo(pt);
			pt.y = m_figureEndY;
			memDC.LineTo(pt);

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制底部间隔线
		if(m_bShowHoriSpt)
		{
			pen.CreatePen(PS_SOLID,m_horiBaseLineWidth,m_horiBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			for(double i = m_horiStartValue;i <= m_horiEndValue;i += m_horiStep)
			{
				pt.y = m_figureEndY;
				memDC.MoveTo(pt);
				pt.y = m_figureEndY + 10;
				memDC.LineTo(pt);
				pt.x += horiStepPixel;
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//显示坐标轴内容说明
		if(m_bShowXAxisExplain)
		{
			CRect rcExplain(rcClient);
			rcExplain.bottom = m_topIndent;
			CSize strSize = memDC.GetTextExtent(m_strXAxisExplain);
			memDC.TextOut(rcExplain.Width() - strSize.cx - 10,(rcExplain.Height() - strSize.cy) / 2,m_strXAxisExplain);
		}

#ifdef _DEBUG
		tempExistMs1 = clock() - startMs;
		++tempExistMs1;
#endif

		memDCLine.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

		if(NULL != m_pLineInfo)
		{
			//绘制顶部显示的标曲关系式
			for(int i = 0;i < m_lineNum;++i)
			{
				if(m_pLineInfo[i].BeHaveFitData())
				{
					CString strDyeName = _T("");
					CRect rcExpression(rcClient);
					rcExpression.bottom = m_topIndent;

					if(m_pLineInfo[i].m_bShowExpression)
					{
						CString strExpression = m_pLineInfo[i].GetExpressionString();
						//strExpression.Format(_T("y=%.2lfx+%.2lf R=%.5lf"),m_pLineInfo[i].m_linearFitData.slope,m_pLineInfo[i].m_linearFitData.intercept,m_pLineInfo[i].m_linearFitData.rSquare);
						//strExpression = strDyeName + _T(": ") + strExpression + _T("  eff  99.43%");
						CSize strSize = memDCLine.GetTextExtent(strExpression);
						memDCLine.TextOut((rcExpression.Width() - strSize.cx) / 2,(rcExpression.Height() - strSize.cy) / 2,strExpression);
					}

					break;
				}
			}

			//绘制线段,使用Gdi+,使用抗锯齿处理
			Graphics graphics(memDCLine.m_hDC);
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);
			COLORREF lineClr = 0;
			for(int i = 0;i < m_lineNum;++i)
			{	
				if(!m_pLineInfo[i].m_bShow)
				{
					continue;
				}	

				lineClr = m_pLineInfo[i].GetLineClr(m_eLineClrMode) /*m_pLineInfo[i].m_lineClr*/;

				BYTE r = lineClr & 0XFF;
				BYTE g = (lineClr >> 8) & 0XFF;
				BYTE b = (lineClr >> 16) & 0XFF;
				Pen gdiplusPen(Color(r,g,b),m_curveWidth);

				if(STDLINE == m_eCurveType)
				{
					Point ptStart,ptEnd;
					if(m_pLineInfo[i].m_linearFitData.xLen > 1)
					{
						ptStart.X = (m_pLineInfo[i].m_linearMinPt.m_X  - m_horiStartValue) * horiRatio + m_figureStartX;
						ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearMinPt.m_Y - m_vertStartValue) * vertRatio;
						ptEnd.X = (m_pLineInfo[i].m_linearMaxPt.m_X - m_horiStartValue) * horiRatio + m_figureStartX;
						ptEnd.Y = m_figureEndY - (m_pLineInfo[i].m_linearMaxPt.m_Y - m_vertStartValue) * vertRatio;
						graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);	
					}

					for(int j = 0;j < m_pLineInfo[i].m_linearFitData.xLen;++j)
					{
						ptStart.X = (m_pLineInfo[i].m_linearFitData.pDataX[j] - m_horiStartValue) * horiRatio + m_figureStartX;
						ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearFitData.pDataY[j] - m_vertStartValue) * vertRatio;
						graphics.DrawEllipse(&gdiplusPen,ptStart.X,ptStart.Y,4,4);
					}
				}
				else
				{   
					int dataNum = m_pLineInfo[i].m_lnkDots.GetSize();
					if(dataNum > 0)
					{
						Point* pCurvePt = new Point[dataNum];
						Node<CMyPoint>* pNode = m_pLineInfo[i].m_lnkDots.GetHead();
						int index = 0;
						while(NULL != pNode)
						{
							pCurvePt[index].X = (pNode->data.m_X - m_horiStartValue) * horiRatio + m_figureStartX;
							pCurvePt[index++].Y = m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * vertRatio;
							pNode = pNode->pNext;
						}

						//绘制圆圈
						if(m_bShowRoundTag)
						{
							pen.CreatePen(PS_SOLID,m_roundTagLineWidth,lineClr);
							memDCLine.SelectObject(&pen);
							CBrush brush;
							brush.CreateSolidBrush(m_roundTagInnerClr);
							memDCLine.SelectObject(&brush);
							for(int k = 0;k < dataNum;++k)
							{
								memDCLine.Ellipse(pCurvePt->X - m_roundTagRadius,pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
							}
							brush.DeleteObject();
							pen.DeleteObject();
						}

						graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum,0.5);
						delete [] pCurvePt;
					}
				}
			}

			//添加高亮显示
			for(int i = 0;i < m_vecHighLightIndex.size();++i)
			{
				int lineIndex = m_vecHighLightIndex[i];
				if(!m_pLineInfo[lineIndex].m_bShow)
				{
					continue;
				}	

				lineClr = m_pLineInfo[lineIndex].GetLineClr(m_eLineClrMode);

				BYTE r = lineClr & 0XFF;
				BYTE g = (lineClr >> 8) & 0XFF;
				BYTE b = (lineClr >> 16) & 0XFF;
				Pen gdiplusPen(Color(r,g,b),5);

				{   
					int dataNum = m_pLineInfo[lineIndex].m_lnkDots.GetSize();
					if(dataNum > 0)
					{
						Point* pCurvePt = new Point[dataNum];
						Node<CMyPoint>* pNode = m_pLineInfo[lineIndex].m_lnkDots.GetHead();
						int index = 0;
						while(NULL != pNode)
						{
							pCurvePt[index].X = (pNode->data.m_X - m_horiStartValue) * horiRatio + m_figureStartX;
							pCurvePt[index++].Y = m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * vertRatio;
							pNode = pNode->pNext;
						}

						//绘制圆圈
						if(m_bShowRoundTag)
						{
							pen.CreatePen(PS_SOLID,m_roundTagLineWidth,lineClr);
							memDCLine.SelectObject(&pen);
							CBrush brush;
							brush.CreateSolidBrush(m_roundTagInnerClr);
							memDCLine.SelectObject(&brush);
							for(int k = 0;k < dataNum;++k)
							{
								memDCLine.Ellipse(pCurvePt->X - m_roundTagRadius,pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
							}
							brush.DeleteObject();
							pen.DeleteObject();
						}

						graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum,0.5);
						delete [] pCurvePt;
					}
				}
			}

			graphics.ReleaseHDC(memDCLine.m_hDC);
		}

		memDC.BitBlt(m_figureStartX,m_figureStartY,m_figureEndX - m_figureStartX,m_figureEndY - m_figureStartY,&memDCLine,m_figureStartX,m_figureStartY,SRCCOPY);

#ifdef _DEBUG
		tempExistMs2 = clock() - startMs;
		++tempExistMs2;
#endif

		//绘制垂直文字
		CString vertStr(_T("")),strFormat(_T(""));
		strFormat = _T("%.");
		strFormat.AppendFormat(_T("%d"),m_realUsedVertDotLen);
		strFormat += (_T("lf"));
		memDC.SetTextAlign(TA_CENTER);
		memDC.SetTextColor(RGB(0x99,0x99,0x99));
		memDC.SetTextAlign(TA_LEFT | TA_TOP);
		memDC.SelectObject(&m_fontSet);
		int strStartPos = m_figureEndY;
		double strOutStep = m_vertStep * m_vertStrOutStep;
		//int strOutStepPixel = vertStepPixel * m_vertStrOutStep;	//可能会导致精度不够问题
		double circleEnd = m_vertEndValue + m_vertStep;

		for(double i = m_vertStartValue,j = 0;i < circleEnd;i += strOutStep,++j)
		{
			strStartPos = m_figureEndY - (i - m_vertStartValue) * vertRatio;
			vertStr.Format(strFormat,i);

			CSize strSize = memDC.GetTextExtent(vertStr);
			memDC.TextOut(m_figureStartX - strSize.cx - 10,strStartPos - strSize.cy / 2,vertStr);
		}

		//绘制水平文字
		strFormat = _T("%.");
		strFormat.AppendFormat(_T("%d"),m_realUsedHoriDotLen);
		strFormat += (_T("lf"));
		CFont* pFont = memDC.GetCurrentFont();
		LOGFONT logFont;
		pFont->GetLogFont(&logFont);
		if(m_bLeanX)
		{
			logFont.lfEscapement = 900;
		}

		HFONT hFont = CreateFontIndirect(&logFont);
		memDC.SelectObject(hFont);
		CString horiStr(_T(""));
		strStartPos = m_figureStartX;
		strOutStep = m_horiStep * m_horiStrOutStep;
		//strOutStepPixel = horiStepPixel * m_horiStrOutStep;
		circleEnd = m_horiEndValue + m_horiStep;	//防止浮点型数据不能完全表示真实数据的问题
		int index = 0;
		Node<CMyPoint> *pNodeMyPt = NULL;
		for(double i = m_horiStartValue;i < circleEnd;i += strOutStep)
		{
			strStartPos = m_figureStartX + (i - m_horiStartValue) * horiRatio;
			if(NORMALXAXIS == m_eXAxisMode)
			{
				horiStr.Format(strFormat,i);
			}
			else
			{
				int curSecond = int(i) / 1000;
				horiStr.Format(_T("%01d:%02d:%02d"),curSecond / 3600,(curSecond % 3600) / 60,curSecond % 60);
			}

			CSize strSize = memDC.GetTextExtent(horiStr);
			if(m_bLeanX)
			{
				memDC.TextOut(strStartPos - strSize.cy / 2,m_figureEndY + 10 + strSize.cx,horiStr);
			}
			else
			{
				int curEndX = strStartPos + strSize.cx / 2;
				if(curEndX > m_figureEndX)
				{
					strStartPos -= (curEndX - m_figureEndX);
				}
				memDC.TextOut(strStartPos - strSize.cx / 2,m_figureEndY + 10,horiStr);
			}	
		}

		if(_T("") != m_strXAxisTag)
		{
			int x = m_figureEndX - 10;
			int y = m_figureEndY - 10;
			CSize strSize = memDC.GetTextExtent(m_strXAxisTag);
			memDC.TextOut(x - strSize.cx,y - strSize.cy,m_strXAxisTag);
		}

		if(_T("") != m_strYAxisTag)
		{
			int x = m_figureStartX + 10;
			int y = m_figureStartY + 10;
			CSize strSize = memDC.GetTextExtent(m_strYAxisTag);
			memDC.TextOut(x,y,m_strYAxisTag);
		}

		DeleteObject(hFont);
	}
	
	if(!m_bThreMove)
	{
		if(NULL != m_pMemDc)
		{
			m_pMemDc->DeleteDC();
			MyDeletePtr(&m_pMemDc);

			if(NULL != m_hBitmap)
			{
				::DeleteObject(m_hBitmap);
			}
		}
		m_pMemDc = new CDC;
		BOOL ret = m_pMemDc->CreateCompatibleDC(&memDC);
		m_hBitmap = ::CreateCompatibleBitmap(memDC.m_hDC,rcClient.Width(),rcClient.Height()); //创建兼容位图
		m_pMemDc->SelectObject(m_hBitmap);	
		m_pMemDc->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

		m_fstPaintWidth = rcClient.Width();
		m_fstPaintHeight = rcClient.Height();
		m_bFstPaint = FALSE;
	}
	else
	{
		memDC.BitBlt(0,0,m_fstPaintWidth,m_fstPaintHeight,m_pMemDc,0,0,SRCCOPY);
		m_bThreMove = FALSE;
	}

	if(m_bHaveThreLineFunc && m_bUseThreLineFunc && m_bHaveData)
	{
		DrawThreLine(&memDC);
	}

	memDCLine.SelectObject(pOldBitmapline);
	bitMapLine.DeleteObject();
	memDCLine.DeleteDC();


	BITMAPFILEHEADER bmfh;
	ZeroMemory(&bmfh,sizeof(BITMAPFILEHEADER));
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';

	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfSize = bmfh.bfOffBits + WIDTHBYTES(iWidth*((DWORD)iPixel)) * iHeight;

	int iBMPBytes = WIDTHBYTES(iWidth*((DWORD)iPixel)) * iHeight;
	CFile file;
	if(file.Open(strPicName,CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(pBits,iBMPBytes);
		file.Close();
	}

	memDC.SelectObject(pOldBitmap);
	memDC.SelectObject(pOldFont);
	bitMap.DeleteObject();
	memDC.DeleteDC();

#ifdef _DEBUG
	clock_t endMs = clock();
	clock_t existMs = endMs - startMs;
	++existMs;
#endif

	ReleaseDC(pDc);
	return TRUE;
}

int CMyDrawCurveDlg::CurveToPic(CString strPicName)
{
	CDC* pDc = GetDC();
	if(NULL == pDc)
	{
		return -1;
	}

	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC,memDCLine,memDcTotal;
	memDC.CreateCompatibleDC(pDc);
	memDCLine.CreateCompatibleDC(pDc);
	memDcTotal.CreateCompatibleDC(pDc);

	CBitmap bitMapLine;
	bitMapLine.CreateCompatibleBitmap(pDc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmapline = memDCLine.SelectObject(&bitMapLine);

	CBitmap bitMapTotal;
	bitMapTotal.CreateCompatibleBitmap(pDc,rcClient.Width(),rcClient.Height());
	CBitmap* pOldBitmapTotal = memDcTotal.SelectObject(&bitMapTotal);

	//定义图形大小
	int iWidth = rcClient.Width();
	int iHeight = rcClient.Height();
	int iPixel  = 32;

	//图形格式参数
	LPBITMAPINFO lpbmih = new BITMAPINFO;
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;
	lpbmih->bmiHeader.biBitCount = iPixel;
	lpbmih->bmiHeader.biCompression = BI_RGB;
	lpbmih->bmiHeader.biSizeImage = 0;
	lpbmih->bmiHeader.biXPelsPerMeter = 0;
	lpbmih->bmiHeader.biYPelsPerMeter = 0;
	lpbmih->bmiHeader.biClrUsed = 0;
	lpbmih->bmiHeader.biClrImportant = 0;

	//创建位图数据
	HBITMAP hBitMap = NULL;
	BYTE *pBits = NULL;

	hBitMap = CreateDIBSection(memDC.m_hDC,lpbmih,DIB_PAL_COLORS,(void **)&pBits,NULL,0);
	CBitmap bitMap;
	bitMap.Attach(hBitMap);
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	CRect rcTempFigure(m_figureStartX,m_figureStartY,m_figureEndX,m_figureEndY);
	if(m_bRoundRectStyle)
	{
		memDcTotal.FillSolidRect(&rcClient,RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
		memDcTotal.FillSolidRect(&rcTempFigure,m_clrFourAngleBk);
		DrawRoundRectangleNew(&memDcTotal,rcTempFigure.left,rcTempFigure.top,rcTempFigure.Width() - 1,rcTempFigure.Height() - 1,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,m_iRoundAngleSize,TRUE,Color(0XE7,0XE7,0XE7),1,TRUE,m_clrIndepent,1);
	}
	else
	{
		memDcTotal.FillSolidRect(&rcClient,m_rgbBkClr);
	}

	CFont* pOldFont = memDC.SelectObject(&m_fontSet);

	if(!m_bThreMove || m_bFstPaint)
	{
		memDC.FillSolidRect(&rcClient,m_rgbBkClr);

		int figureWidth = m_figureEndX - m_figureStartX;
		int figureHeight = m_figureEndY - m_figureStartY;

		double horiStepPixel = double(figureWidth) / m_horiStepNums;
		double vertStepPixel = double(figureHeight) / m_vertStepNums;

		double horiRatio = horiStepPixel / m_horiStep;
		double vertRatio = vertStepPixel / m_vertStep;
		m_dAxisHoriRt = horiRatio;
		m_dAxisVertRt = vertRatio;

		m_intHoriStep = m_horiStep;
		m_intVertStep = m_vertStep;

		int iFrameStartY = m_figureStartY - 1,iFrameEndY = m_figureEndY + 2;
		if(iFrameStartY < 0)
		{
			iFrameStartY = 0;
		}
		if(iFrameEndY < 0)
		{
			iFrameEndY = 0;
		}
		CRect tempRc(m_figureStartX,iFrameStartY,m_figureEndX,iFrameEndY);	//防止右侧、顶部的边框被数据区域覆盖
		memDC.FillSolidRect(tempRc,RGB(0XFA,0XFA,0XFA));

		if(!m_bRoundRectStyle)
		{
			CBrush brush;
			brush.CreateSolidBrush(RGB(164,164,164));
			memDC.FrameRect(tempRc,&brush);
			brush.DeleteObject();
		}

		memDC.SetBkMode(TRANSPARENT);

		CPoint pt;
		CPen pen;

		//绘制水平基线
		if(m_bShowHoriBaseLine)
		{
			pen.CreatePen(PS_SOLID,m_horiBaseLineWidth,m_horiBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = m_figureEndY;
			memDC.MoveTo(pt);
			pt.x = m_figureEndX;
			memDC.LineTo(pt);
			pt.y = m_figureEndY;

			double circleEndValue = m_vertEndValue/* + m_vertStep*/; //为了避免浮点数精度问题的影响
			int index = 0;
			for(double i = m_vertStartValue + m_vertStep;i + m_intVertStep <= circleEndValue;i += m_vertStep)
			{
				pt.x = m_figureStartX + 1;
				pt.y = m_figureEndY - (++index) * vertStepPixel;
				memDC.MoveTo(pt);
				pt.x = m_figureEndX - 1;
				memDC.LineTo(pt);
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		if(m_bShowVertBaseLine)
		{
			pen.CreatePen(PS_SOLID,m_vertBaseLineWidth,m_vertBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);

			double circleEndValue = m_horiEndValue + m_horiStep; //为了避免浮点数精度问题的影响
			pt.x = m_figureStartX;
			int index = 0;
			double dVertLineIndexValue = m_horiStep * m_iVertLineInterval;
			for(double i = m_horiStartValue + dVertLineIndexValue;i + m_intHoriStep < circleEndValue;i += dVertLineIndexValue)
			{
				index += m_iVertLineInterval;

				pt.y = m_figureStartY + 1;
				pt.x = m_figureStartX + index * horiStepPixel;
				memDC.MoveTo(pt);
				pt.y = m_figureEndY - 1;
				memDC.LineTo(pt);
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制X轴
		if(m_bShowHoriLine)
		{
			pen.CreatePen(PS_SOLID,m_horiLineWidth,m_horiLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = m_figureEndY;
			memDC.MoveTo(pt);
			pt.x = m_figureEndX;
			memDC.LineTo(pt);

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制Y轴
		if(m_bShowVertLine)
		{
			pen.CreatePen(PS_SOLID,m_vertLineWidth,m_vertLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			pt.y = m_figureStartY;
			memDC.MoveTo(pt);
			pt.y = m_figureEndY;
			memDC.LineTo(pt);

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//绘制底部间隔线
		if(m_bShowHoriSpt)
		{
			pen.CreatePen(PS_SOLID,m_horiBaseLineWidth,m_horiBaseLineClr);
			CPen* pOldPen = memDC.SelectObject(&pen);
			memDC.SelectObject(&pen);
			pt.x = m_figureStartX;
			for(double i = m_horiStartValue;i <= m_horiEndValue;i += m_horiStep)
			{
				pt.y = m_figureEndY;
				memDC.MoveTo(pt);
				pt.y = m_figureEndY + 10;
				memDC.LineTo(pt);
				pt.x += horiStepPixel;
			}

			memDC.SelectObject(pOldPen);
			pen.DeleteObject();
		}

		//显示坐标轴内容说明
		if(m_bShowXAxisExplain)
		{
			CRect rcExplain(rcClient);
			rcExplain.bottom = m_topIndent;
			CSize strSize = memDC.GetTextExtent(m_strXAxisExplain);
			memDC.TextOut(rcExplain.Width() - strSize.cx - 10,(rcExplain.Height() - strSize.cy) / 2,m_strXAxisExplain);
		}

		if(NULL != m_pLineInfo)
		{
			memDC.SetTextColor(RGB(0x56,0x56,0x56));
			//绘制顶部显示的标曲关系式
			for(int i = 0;i < m_lineNum;++i)
			{
				if(m_pLineInfo[i].m_bShow && m_pLineInfo[i].BeHaveFitData())
				{
					CString strDyeName = m_pLineInfo[i].m_strTaget;
					CRect rcExpression(rcClient);
					rcExpression.bottom = m_stdFrameH;

					if(m_pLineInfo[i].m_bShowExpression)
					{
						CString strExpression = m_pLineInfo[i].GetExpressionString();
						//strExpression.Format(_T("y=%.2lfx+%.2lf R=%.5lf"),m_pLineInfo[i].m_linearFitData.slope,m_pLineInfo[i].m_linearFitData.intercept,m_pLineInfo[i].m_linearFitData.rSquare);
						//strExpression = strDyeName + _T(": ") + strExpression + _T("  eff  99.43%");
						CSize strSize = memDC.GetTextExtent(strExpression);
						memDC.TextOut(/*(rcExpression.Width() - strSize.cx) / 2*/ 20,(rcExpression.Height() - strSize.cy) / 2,strExpression);

						break;
					}
				}
			}

			memDCLine.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

			//绘制线段,使用Gdi+,使用抗锯齿处理
			Graphics graphics(memDCLine.m_hDC);
			graphics.SetSmoothingMode(SmoothingModeAntiAlias);
			COLORREF lineClr = 0;
			for(int i = 0;i < m_lineNum;++i)
			{	
				if(!m_pLineInfo[i].m_bShow)
				{
					continue;
				}	

				lineClr = m_pLineInfo[i].GetLineClr(m_eLineClrMode);

				BYTE r = lineClr & 0XFF;
				BYTE g = (lineClr >> 8) & 0XFF;
				BYTE b = (lineClr >> 16) & 0XFF;
				Pen gdiplusPen(Color(r,g,b),m_curveWidth);

				if(STDLINE == m_eCurveType)
				{
					Point ptStart,ptEnd;
					if(m_pLineInfo[i].m_linearFitData.xLen > 1)
					{
						ptStart.X = (m_pLineInfo[i].m_linearMinPt.m_X  - m_horiStartValue) * horiRatio + m_figureStartX;
						ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearMinPt.m_Y - m_vertStartValue) * vertRatio;
						ptEnd.X = (m_pLineInfo[i].m_linearMaxPt.m_X - m_horiStartValue) * horiRatio + m_figureStartX;
						ptEnd.Y = m_figureEndY - (m_pLineInfo[i].m_linearMaxPt.m_Y - m_vertStartValue) * vertRatio;
						graphics.DrawLine(&gdiplusPen,ptStart,ptEnd);	
					}

					for(int j = 0;j < m_pLineInfo[i].m_linearFitData.xLen;++j)
					{
						ptStart.X = (m_pLineInfo[i].m_linearFitData.pDataX[j] - m_horiStartValue) * horiRatio + m_figureStartX;
						ptStart.Y = m_figureEndY - (m_pLineInfo[i].m_linearFitData.pDataY[j] - m_vertStartValue) * vertRatio;
						graphics.DrawEllipse(&gdiplusPen,ptStart.X,ptStart.Y,4,4);
					}
				}
				else
				{   
					int dataNum = m_pLineInfo[i].m_lnkDots.GetSize();
					if(dataNum > 0)
					{
						Point* pCurvePt = new Point[dataNum];
						Node<CMyPoint>* pNode = m_pLineInfo[i].m_lnkDots.GetHead();
						int index = 0;
						while(NULL != pNode)
						{
							pCurvePt[index].X = (pNode->data.m_X - m_horiStartValue) * horiRatio + m_figureStartX;
							pCurvePt[index++].Y = m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * vertRatio;
							pNode = pNode->pNext;
						}

						//绘制圆圈
						if(m_bShowRoundTag)
						{
							pen.CreatePen(PS_SOLID,m_roundTagLineWidth,lineClr);
							memDCLine.SelectObject(&pen);
							CBrush brush;
							brush.CreateSolidBrush(m_roundTagInnerClr);
							memDCLine.SelectObject(&brush);
							for(int k = 0;k < dataNum;++k)
							{
								memDCLine.Ellipse(pCurvePt->X - m_roundTagRadius,pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
							}
							brush.DeleteObject();
							pen.DeleteObject();
						}

						graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum);
						delete [] pCurvePt;
					}
				}
			}

			//添加高亮显示
			for(int i = 0;i < m_vecHighLightIndex.size();++i)
			{
				int lineIndex = m_vecHighLightIndex[i];
				if(!m_pLineInfo[lineIndex].m_bShow)
				{
					continue;
				}	

				lineClr = m_pLineInfo[lineIndex].GetLineClr(m_eLineClrMode);

				BYTE r = lineClr & 0XFF;
				BYTE g = (lineClr >> 8) & 0XFF;
				BYTE b = (lineClr >> 16) & 0XFF;
				Pen gdiplusPen(Color(r,g,b),5);

				{   
					int dataNum = m_pLineInfo[lineIndex].m_lnkDots.GetSize();
					if(dataNum > 0)
					{
						Point* pCurvePt = new Point[dataNum];
						Node<CMyPoint>* pNode = m_pLineInfo[lineIndex].m_lnkDots.GetHead();
						int index = 0;
						while(NULL != pNode)
						{
							pCurvePt[index].X = (pNode->data.m_X - m_horiStartValue) * horiRatio + m_figureStartX;
							pCurvePt[index++].Y = m_figureEndY - (pNode->data.m_Y - m_vertStartValue) * vertRatio;
							pNode = pNode->pNext;
						}

						//绘制圆圈
						if(m_bShowRoundTag)
						{
							pen.CreatePen(PS_SOLID,m_roundTagLineWidth,lineClr);
							memDCLine.SelectObject(&pen);
							CBrush brush;
							brush.CreateSolidBrush(m_roundTagInnerClr);
							memDCLine.SelectObject(&brush);
							for(int k = 0;k < dataNum;++k)
							{
								memDCLine.Ellipse(pCurvePt->X - m_roundTagRadius,pCurvePt->Y - m_roundTagRadius,pCurvePt->X + m_roundTagRadius,pCurvePt->Y + m_roundTagRadius);
							}
							brush.DeleteObject();
							pen.DeleteObject();
						}

						graphics.DrawCurve(&gdiplusPen,pCurvePt,dataNum);
						delete [] pCurvePt;
					}
				}
			}

			graphics.ReleaseHDC(memDCLine.m_hDC);
		}

		memDC.BitBlt(m_figureStartX,m_figureStartY,m_figureEndX - m_figureStartX,m_figureEndY - m_figureStartY,&memDCLine,m_figureStartX,m_figureStartY,SRCCOPY);

		//绘制垂直文字
		CString vertStr(_T("")),strFormat(_T(""));
		strFormat = _T("%.");
		strFormat.AppendFormat(_T("%d"),m_realUsedVertDotLen);
		strFormat += (_T("lf"));
		memDC.SetTextAlign(TA_CENTER);
		memDC.SetTextColor(RGB(0x56,0x56,0x56));
		memDC.SetTextAlign(TA_LEFT | TA_TOP);
		memDC.SelectObject(&m_fontSet);
		int strStartPos = m_figureEndY;
		double strOutStep = m_vertStep * m_vertStrOutStep;
		//int strOutStepPixel = vertStepPixel * m_vertStrOutStep;	//可能会导致精度不够问题
		double circleEnd = m_vertEndValue + m_vertStep;

		int preTextY = 0;
		for(double i = m_vertStartValue,j = 0;i < circleEnd;i += strOutStep,++j)
		{
			vertStr.Format(strFormat,i);
			CSize strSize = memDC.GetTextExtent(vertStr);
			strStartPos = m_figureEndY - (i - m_vertStartValue) * vertRatio;
			if(i > m_vertEndValue && (strStartPos - preTextY > strSize.cy + 16))	//第二个条件防止顶端Y轴标识字符重合
			{
				strStartPos = m_figureStartY;
				memDC.TextOut(m_figureStartX - strSize.cx - 10,strStartPos - strSize.cy / 2,vertStr);
				break;
			}

			memDC.TextOut(m_figureStartX - strSize.cx - 10,strStartPos - strSize.cy / 2,vertStr);
			preTextY = strStartPos - strSize.cy / 2;
		}

		//绘制水平文字
		strFormat = _T("%.");
		strFormat.AppendFormat(_T("%d"),m_realUsedHoriDotLen);
		strFormat += (_T("lf"));
		CFont* pFont = memDC.GetCurrentFont();
		LOGFONT logFont;
		pFont->GetLogFont(&logFont);
		if(m_bLeanX)
		{
			logFont.lfEscapement = 900;
		}

		HFONT hFont = CreateFontIndirect(&logFont);
		memDC.SelectObject(hFont);
		CString horiStr(_T(""));
		strStartPos = m_figureStartX;
		strOutStep = m_horiStep * m_horiStrOutStep;
		//strOutStepPixel = horiStepPixel * m_horiStrOutStep;
		circleEnd = m_horiEndValue + m_horiStep;	//防止浮点型数据不能完全表示真实数据的问题
		int index = 0;
		Node<CMyPoint> *pNodeMyPt = NULL;
		for(double i = m_horiStartValue;i < circleEnd;i += strOutStep)
		{
			strStartPos = m_figureStartX + (i - m_horiStartValue) * horiRatio;
			if(NORMALXAXIS == m_eXAxisMode)
			{
				horiStr.Format(strFormat,i);
			}
			else
			{
				int curSecond = int(i) / 1000;
				horiStr.Format(_T("%01d:%02d:%02d"),curSecond / 3600,(curSecond % 3600) / 60,curSecond % 60);
			}

			CSize strSize = memDC.GetTextExtent(horiStr);
			if(m_bLeanX)
			{
				memDC.TextOut(strStartPos - strSize.cy / 2,m_figureEndY + 10 + strSize.cx,horiStr);
			}
			else
			{
				int curEndX = strStartPos + strSize.cx / 2;
				if(curEndX > m_figureEndX)
				{
					strStartPos -= (curEndX - m_figureEndX);
				}
				memDC.TextOut(strStartPos - strSize.cx / 2,m_figureEndY + 10,horiStr);
			}	
		}

		/*if(TIMERXAXIS == m_eXAxisMode)
		{
		int x = m_figureEndX - 10;
		int y = m_figureEndY - 10;
		CString strTipInfo(_T("时间(h:mm:ss)"));
		CSize strSize = memDC.GetTextExtent(strTipInfo);
		memDC.TextOut(x,y,strTipInfo);
		}*/

		if(_T("") != m_strXAxisTag)
		{
			int x = m_figureEndX - 10;
			int y = m_figureEndY - 10;
			CSize strSize = memDC.GetTextExtent(m_strXAxisTag);
			memDC.TextOut(x - strSize.cx,y - strSize.cy,m_strXAxisTag);
		}

		if(_T("") != m_strYAxisTag)
		{
			int x = m_figureStartX + 10;
			int y = m_figureStartY + 10;
			CSize strSize = memDC.GetTextExtent(m_strYAxisTag);
			memDC.TextOut(x,y,m_strYAxisTag);
		}

		DeleteObject(hFont);
	}
	
	if(!m_bThreMove)
	{
		if(NULL != m_pMemDc)
		{
			m_pMemDc->DeleteDC();
			MyDeletePtr(&m_pMemDc);

			if(NULL != m_hBitmap)
			{
				::DeleteObject(m_hBitmap);
			}
		}
		m_pMemDc = new CDC;
		BOOL ret = m_pMemDc->CreateCompatibleDC(&memDC);
		m_hBitmap = ::CreateCompatibleBitmap(memDC.m_hDC,rcClient.Width(),rcClient.Height()); //创建兼容位图
		m_pMemDc->SelectObject(m_hBitmap);	
		m_pMemDc->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDC,0,0,SRCCOPY);

		m_fstPaintWidth = rcClient.Width();
		m_fstPaintHeight = rcClient.Height();
		m_bFstPaint = FALSE;
	}
	else
	{
		memDC.BitBlt(0,0,m_fstPaintWidth,m_fstPaintHeight,m_pMemDc,0,0,SRCCOPY);
		m_bThreMove = FALSE;
	}

	if(m_bHaveThreLineFunc && m_bUseThreLineFunc && m_bHaveData)
	{
		DrawThreLine(&memDC);
	}

	if(m_bRoundRectStyle)
	{
		//检查使用稳定性
		memDC.TransparentBlt(rcTempFigure.left,rcTempFigure.top,rcTempFigure.Width(),rcTempFigure.Height(),&memDcTotal,rcTempFigure.left,rcTempFigure.top,rcTempFigure.Width(),rcTempFigure.Height(),RGB(m_clrIndepent.GetR(),m_clrIndepent.GetG(),m_clrIndepent.GetB()));
	}

	memDCLine.SelectObject(pOldBitmapline);
	//memDCLine.SelectObject(pOldFont);
	bitMapLine.DeleteObject();
	memDCLine.DeleteDC();
	  
	memDcTotal.SelectObject(pOldBitmapTotal);
	bitMapTotal.DeleteObject();
	memDcTotal.DeleteDC();

	BITMAPFILEHEADER bmfh;
	ZeroMemory(&bmfh,sizeof(BITMAPFILEHEADER));
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';

	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfSize = bmfh.bfOffBits + WIDTHBYTES(iWidth*((DWORD)iPixel)) * iHeight;

	int iBMPBytes = WIDTHBYTES(iWidth*((DWORD)iPixel)) * iHeight;
	CFile file;
	if(file.Open(strPicName,CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(pBits,iBMPBytes);
		file.Close();
	}

	memDC.SelectObject(pOldBitmap);
	memDC.SelectObject(pOldFont);
	bitMap.DeleteObject();
	memDC.DeleteDC();

	return 0;
}

BOOL CMyDrawCurveDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CMyDrawCurveDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_bHaveThreLineFunc && m_bUseThreLineFunc)
	{
		int iChanNum = m_vecChanThreInfo.size();
		if(iChanNum < 1)
		{
			return;
		}
		for(int i = 0;i < iChanNum;++i)
		{
			CThreLineInfo* pThreLineInfo = m_vecChanThreInfo[i].GetCurSelThreInfo();
			if(NULL == pThreLineInfo)
			{
				continue;
			}

			if(pThreLineInfo->GetBeShow() && pThreLineInfo->m_rcThreLine.PtInRect(point))
			{
				pThreLineInfo->m_bThreLineSel = TRUE;
				break;
			}
		}
	}

	if(m_bHaveCap)
	{
		if(point.y < m_capHeight)
		{
			PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
			return;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
	return;
}

void CMyDrawCurveDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bHaveThreLineFunc && m_bUseThreLineFunc)
	{
		int iChanNum = m_vecChanThreInfo.size();
		for(int i = 0;i < iChanNum;++i)
		{
			CThreLineInfo* pThreLineInfo = m_vecChanThreInfo[i].GetCurSelThreInfo();
			if(NULL == pThreLineInfo)
			{
				continue;
			}
			if(pThreLineInfo->m_bThreLineSel)
			{
				pThreLineInfo->m_bThreLineSel = FALSE;
				CWnd* pParent = GetParent();
				if(NULL != pParent->GetSafeHwnd())
				{	
					pParent->SendMessage(WM_THRELINESTOPMOVE,i,0);
				}
				break;
			}
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
	return;
}

void CMyDrawCurveDlg::OnLButtonDblClk(UINT nFlags,CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcYAxis,rcXAxis;
	rcYAxis.left = 0;
	rcYAxis.right = m_leftIndent;
	rcYAxis.top = 0;
	rcYAxis.bottom = rect.Height();

	rcXAxis.left = 0;
	rcXAxis.right = rect.Width();
	rcXAxis.top = rect.Height() - m_bottomIndent;
	rcXAxis.bottom = rect.Height();

	if(!(rcYAxis.PtInRect(point) || rcXAxis.PtInRect(point)))
	{
		return;
	}

	CAxisZoomDlg axisZoomDlg;
	axisZoomDlg.SetUseDefaultFuncState(FALSE);

	axisZoomDlg.m_axisInfo.m_dXMin = m_horiStartValue;
	axisZoomDlg.m_axisInfo.m_dXMax = m_horiEndValue;
	axisZoomDlg.m_axisInfo.m_dYMin = m_vertStartValue;
	axisZoomDlg.m_axisInfo.m_dYMax = m_vertEndValue;

	if(IDOK == axisZoomDlg.DoModal())
	{
		if(axisZoomDlg.m_bChanged)
		{
			SetHoriStartValue(axisZoomDlg.m_newAxisInfo.m_dXMin);
			SetHoriEndValue(axisZoomDlg.m_newAxisInfo.m_dXMax);
			SetVertStartValue(axisZoomDlg.m_newAxisInfo.m_dYMin);
			SetVertEndValue(axisZoomDlg.m_newAxisInfo.m_dYMax,TRUE,FALSE);
		}
	}

	return;

	if(!m_bHaveData)
	{
		return;
	}
}

void CMyDrawCurveDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu menu1;
	menu1.CreatePopupMenu();
	menu1.AppendMenu(MF_STRING,ID_CURVE_MENUEXPORTPICTURE,theApp.m_pLangInfo->GetText(249));

	tagPOINT pt;
	pt.x = point.x;
	pt.y = point.y;
	ClientToScreen(&pt);
	menu1.TrackPopupMenu(TPM_RIGHTBUTTON,pt.x,pt.y,this);
	menu1.DestroyMenu();
}

void CMyDrawCurveDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL bHandMouse = FALSE;
	BOOL bThreMoving = FALSE;	//标识是否正在拖动阈值线
	if(m_bHaveThreLineFunc && m_bUseThreLineFunc && point.y > m_figureStartY && point.y < m_figureEndY)
	{
		int iChanNum = m_vecChanThreInfo.size();
		for(int i = 0;i < iChanNum;++i)
		{
			CThreLineInfo* pThreLineInfo = m_vecChanThreInfo[i].GetCurSelThreInfo();
			if(NULL == pThreLineInfo)
			{
				continue;
			}

			if((pThreLineInfo->GetBeShow() && pThreLineInfo->m_rcThreLine.PtInRect(point)) || pThreLineInfo->m_bThreLineSel)
			{
				bHandMouse = TRUE;
			}

			if(pThreLineInfo->m_bThreLineSel)
			{
				if(nFlags == MK_LBUTTON)
				{
					pThreLineInfo->m_threLinePixelH = point.y;
					pThreLineInfo->m_dThre = PixelHeightToYValue(point.y);
					
					if(pThreLineInfo->m_bAutoThre)
					{
						pThreLineInfo->m_bAutoThre = FALSE;
					}

					m_bThreMove = TRUE;
					Invalidate(FALSE);

					CWnd* pParent = GetParent();
					if(NULL != pParent->GetSafeHwnd())
					{	
						pParent->PostMessage(WM_THRECHANGED,i,0);
					}

					bThreMoving = TRUE;
				}
				else
				{
					pThreLineInfo->m_bThreLineSel = FALSE;
				}

				//设置关联阈值线信息
				CThreLineInfo* pAssociatedThreLineInfo = m_vecChanThreInfo[i].GetCurSelAssociatedThreInfo();
				if(NULL != pAssociatedThreLineInfo)
				{
					if(ELSF_LINEAR == m_vecChanThreInfo[i].m_eLineShowFormat)
					{
						if(pThreLineInfo->m_dThre > 0)
						{
							pAssociatedThreLineInfo->m_bAutoThre = FALSE;
							pAssociatedThreLineInfo->m_bFstDrawThreLine = TRUE;
							pAssociatedThreLineInfo->m_dThre = log10(pThreLineInfo->m_dThre);
						}
					}
					else
					{
						pAssociatedThreLineInfo->m_bAutoThre = FALSE;
						pAssociatedThreLineInfo->m_bFstDrawThreLine = TRUE;
						pAssociatedThreLineInfo->m_dThre = pow(10,pThreLineInfo->m_dThre);
					}
				}

				break;
			}
		}
	}

	if(bHandMouse)
	{
		HCURSOR hCur = LoadCursor(NULL,IDC_HAND);
		SetCursor(hCur);

		if(point.y > m_figureEndY || point.y < m_figureStartY)
		{
			return;
		}
	}
	else
	{
		HCURSOR hCur = LoadCursor(NULL,IDC_ARROW);
		SetCursor(hCur);
	}

	if(!bHandMouse && m_bUseFloatFunc)
	{
		//处理提示窗口信息
		for(int i = 0;i < m_lineNum;++i)
		{
			if(!m_pLineInfo[i].m_bShow)
			{
				continue;
			}

			Node<CMyPoint>* pNode = m_pLineInfo[i].m_lnkDots.GetHead();
			while(NULL != pNode)
			{
				if(abs(point.x - pNode->data.m_xPixel) < 4 && abs(point.y - pNode->data.m_yPixel) < 4)
				{
					m_floatDlg.m_ptDotInfo = pNode->data;
					m_floatDlg.m_lineInfo = m_pLineInfo[i];
					m_floatDlg.SetHoriDotNum(m_realUsedHoriDotLen);
					m_floatDlg.SetVertDotNum(m_realUsedVertDotLen);

					CRect rcFloatDlg(pNode->data.m_xPixel - 2,pNode->data.m_yPixel - 2,pNode->data.m_xPixel + m_floatDlg.m_iWndWidth,pNode->data.m_yPixel + m_floatDlg.m_iWndHeight);
					rcFloatDlg = GetFloatWndRegion(rcFloatDlg);
					m_floatDlg.MoveWindow(&rcFloatDlg,FALSE);
					m_floatDlg.ShowWindow(SW_SHOW);
					return;
				}
				pNode = pNode->pNext;
			}
		}
	}

	return;
}

BOOL CMyDrawCurveDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	return __super::OnMouseWheel(nFlags, zDelta, pt);

	if(!m_bHaveData)
	{
		return __super::OnMouseWheel(nFlags, zDelta, pt);
	}

	CRect rcWnd;
	GetWindowRect(&rcWnd);
	int tempInt = rcWnd.Height() / 3;
	int yOneThird = rcWnd.top + tempInt;
	int yTwoThird = yOneThird + tempInt;

	BOOL bTopZoomIn = FALSE,bTopZoomOut = FALSE,bBottomZoomIn = FALSE,bBottomZoomOut = FALSE;

	if(pt.y < yOneThird)
	{
		if(zDelta > 0)
		{
			bTopZoomIn = TRUE;
		}
		else
		{
			bTopZoomOut = TRUE;
		}
	}
	else if(pt.y > yTwoThird)
	{
		if(zDelta > 0)
		{
			bBottomZoomIn = TRUE;
		}
		else
		{
			bBottomZoomOut = TRUE; 
		}
	}
	else
	{
		if(zDelta > 0)
		{
			bTopZoomIn = TRUE;
			bBottomZoomIn = TRUE;	
		}
		else
		{
			bTopZoomOut = TRUE;
			bBottomZoomOut = TRUE;		
		}
	}
	
	if(bTopZoomIn)
	{
		if(m_vertEndValue > 0)
		{
			m_vertEndValue /= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
		else
		{
			m_vertEndValue *= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
	}
	if(bTopZoomOut)
	{
		if(m_vertEndValue > 0)
		{
			m_vertEndValue *= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
		else
		{
			m_vertEndValue /= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
	}

	if(bBottomZoomIn)
	{
		if(m_vertStartValue > 0)
		{
			m_vertStartValue *= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
		else
		{
			m_vertStartValue /= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
	}
	if(bBottomZoomOut)
	{
		if(m_vertStartValue > 0)
		{
			m_vertStartValue /= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
		else
		{
			m_vertStartValue *= CRVLYAXIS_LBDCLICKZOOMRATIO;
		}
	}

	if(bTopZoomIn || bTopZoomOut)
	{
		SetVertEndValue(m_vertEndValue);
	}
	if(bBottomZoomIn || bBottomZoomOut)
	{
		SetVertStartValue(m_vertStartValue);
	}
	Invalidate(FALSE);
	
	return __super::OnMouseWheel(nFlags, zDelta, pt);
}

void CMyDrawCurveDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	CRect rcClient;
	GetClientRect(&rcClient);
	m_figureStartX = m_leftIndent;
	m_figureEndX = rcClient.Width() - m_rightIndent;
	m_figureStartY = m_topIndent;
	if(m_bHaveCap)
	{
		m_figureStartY += m_capHeight;
	}
	if(m_bHaveLgd)
	{
		m_figureStartY += m_lgdHeight;
	}
	m_figureEndY = rcClient.Height() - m_bottomIndent;

	m_rcEprTag = CRect(m_leftIndent,m_iEprYStart,m_leftIndent + 120 * m_dHoriRatio,m_iEprYStart + 32 * m_dVertRatio);
}

void CMyDrawCurveDlg::SetHoriStepNums(double horiStepNums,BOOL bRedraw /*= FALSE*/)
{
	if(horiStepNums <= 0)
	{
		return;
	}

	m_horiStepNums = horiStepNums;
	m_horiStep = (m_horiEndValue - m_horiStartValue) / m_horiStepNums;

	if(m_horiStep == int(m_horiStep))
	{
		m_realUsedHoriDotLen = 0;
	}
	else
	{
		m_realUsedHoriDotLen = m_horiDecPtLen;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

double CMyDrawCurveDlg::GetHoriStepNums()
{
	return m_horiStepNums;
}

void CMyDrawCurveDlg::SetVertStepNums(double vertStepNums,BOOL bRedraw /*= FALSE*/)
{
	if(vertStepNums <= 0)
	{
		return;
	}

	m_vertStepNums = vertStepNums;
	m_vertStep = (m_vertEndValue - m_vertStartValue) / m_vertStepNums;

	if(m_vertStep == int(m_vertStep))
	{
		m_realUsedVertDotLen = 0;
	}
	else
	{
		m_realUsedVertDotLen = m_vertDecPtLen;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

double CMyDrawCurveDlg::GetVertStepNums()
{
	return m_vertStepNums;
}

void CMyDrawCurveDlg::SetHoriStep(double horiStep,BOOL bRedraw /*= FALSE*/)
{
	m_horiStep = horiStep;
	m_horiStepNums = (m_horiEndValue - m_horiStartValue) / m_horiStep;
	if(m_horiStep == int(m_horiStep))
	{
		 m_realUsedHoriDotLen = 0;
	}
	else
	{
		m_realUsedHoriDotLen = m_horiDecPtLen;
	}
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

double CMyDrawCurveDlg::GetHoriStep()
{
	return m_horiStep;
}

void CMyDrawCurveDlg::SetVertStep(double vertStep,BOOL bRedraw /*= FALSE*/)
{
	m_vertStep = vertStep;
	m_vertStepNums = (m_vertEndValue - m_vertStartValue) / m_vertStep;
	if(m_vertStep == int(m_vertStep))
	{
		m_realUsedVertDotLen = 0;
	}
	else
	{
		m_realUsedVertDotLen = m_vertDecPtLen;
	}
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

double CMyDrawCurveDlg::GetVertStep()
{
	return m_vertStep;
}

int CMyDrawCurveDlg::SetHoriAxisParam(int iStart,int iEnd,int iHoriStep,BOOL bRedraw /*= FALSE*/)
{
	if(iStart >= iEnd)
	{
		return -1;
	}
	if(0 == iHoriStep)
	{
		return -2;
	}

	m_horiStartValue = iStart;
	m_horiEndValue = iEnd;
	m_horiStep = iHoriStep;
	m_horiStepNums = (m_horiEndValue - m_horiStartValue) / m_horiStep;
	
	if(m_horiStepNums < 11)
	{
		m_horiStrOutStep = 1;
		m_iVertLineInterval = 1;
	}
	else if(m_horiStepNums < 31)
	{
		m_horiStrOutStep = 2;
		m_iVertLineInterval = 1;
	}
	else if(m_horiStepNums < 61)
	{
		m_horiStrOutStep = 3;
		m_iVertLineInterval = 1;
	}
	else 
	{
		m_horiStrOutStep = m_horiStepNums / 10;
		m_iVertLineInterval = m_horiStrOutStep;
		if(0 == (m_horiStrOutStep % 2))
		{
			m_iVertLineInterval /= 2; 
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return 0;
}

void CMyDrawCurveDlg::SetHoriStartValue(double horiStartValue,BOOL bRedraw /*= FALSE*/)
{
	m_horiStartValue = horiStartValue;
	m_horiStep = (m_horiEndValue - m_horiStartValue) / m_horiStepNums;

	SetThrelineFstShow();

	//如果默认的水平数字小数点位数是0，出现浮点数间距时，修改小数点位数为2，恢复整形间距时，小数点位数重新修改为0
	//if(0 == m_iHoriDecPtLenSet)
	//{
	//	if(int(m_horiStep) != m_horiStep)
	//	{
	//		m_horiDecPtLen = 2;
	//		m_realUsedHoriDotLen = 2;
	//	}
	//	else
	//	{
	//		if(m_realUsedHoriDotLen != m_iHoriDecPtLenSet)
	//		{
	//			m_horiDecPtLen = m_iHoriDecPtLenSet;
	//			m_realUsedHoriDotLen = m_iHoriDecPtLenSet;
	//		}
	//	}
	//}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

double CMyDrawCurveDlg::GetHoriStartValue()
{
	return m_horiStartValue;
}

void CMyDrawCurveDlg::SetHoriEndValue(double horiEndValue,BOOL bRedraw /*= FALSE*/)
{
	m_horiEndValue = horiEndValue;
	m_horiStep = (m_horiEndValue - m_horiStartValue) / m_horiStepNums;

	SetThrelineFstShow();
	
	//如果默认的水平数字小数点位数是0，出现浮点数间距时，修改小数点位数为2，恢复整形间距时，小数点位数重新修改为0
	//if(0 == m_iHoriDecPtLenSet)
	//{
	//	if(int(m_horiStep) != m_horiStep)
	//	{
	//		m_horiDecPtLen = 2;
	//		m_realUsedHoriDotLen = 2;
	//	}
	//	else
	//	{
	//		if(m_realUsedHoriDotLen != m_iHoriDecPtLenSet)
	//		{
	//			m_horiDecPtLen = m_iHoriDecPtLenSet;
	//			m_realUsedHoriDotLen = m_iHoriDecPtLenSet;
	//		}
	//	}
	//}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}
  
double CMyDrawCurveDlg::GetHoriEndValue()
{
	return m_horiEndValue;
}

void CMyDrawCurveDlg::SetVertStartValue(double vertStartValue,BOOL bRedraw /*= FALSE*/)
{
	m_vertStartValue = vertStartValue;
	m_vertStep = (m_vertEndValue - m_vertStartValue) / m_vertStepNums;
	if(m_vertStep == int(m_vertStep))
	{
		m_realUsedVertDotLen = 0;
	}
	else
	{
		m_realUsedVertDotLen = m_vertDecPtLen;
	}

	SetThrelineFstShow();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

double CMyDrawCurveDlg::GetVertStartValue()
{
	return m_vertStartValue;
}

void CMyDrawCurveDlg::SetVertEndValue(double vertEndValue,BOOL bRedraw /*= FALSE*/,BOOL bUseMinOfMax /*= TRUE*/)
{
	if(!m_bShieldYAxisMinOfMax && bUseMinOfMax && vertEndValue < m_dYAxisMinOfMax)
	{
		vertEndValue = m_dYAxisMinOfMax;
	}

	m_vertEndValue = vertEndValue;
	m_vertStep = (m_vertEndValue - m_vertStartValue) / m_vertStepNums;
	if(m_vertStep == int(m_vertStep))
	{
		m_realUsedVertDotLen = 0;
	}
	else
	{
		m_realUsedVertDotLen = m_vertDecPtLen;
	}

	SetThrelineFstShow();

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

double CMyDrawCurveDlg::GetVertEndValue()
{
	return m_vertEndValue;
}

void CMyDrawCurveDlg::SetHoriStrColor(COLORREF horiStrClr,BOOL bRedraw /*= FALSE*/)
{
	m_horiStrClr = horiStrClr;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

COLORREF CMyDrawCurveDlg::GetHoriStrColor()
{
	return m_horiStrClr;
}

void CMyDrawCurveDlg::SetHoriLineColor(COLORREF horiLineClr,BOOL bRedraw /*= FALSE*/)
{
	m_horiLineClr = horiLineClr;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

COLORREF CMyDrawCurveDlg::GetHoriLineColor()
{
	return m_horiLineClr;
}

void CMyDrawCurveDlg::SetVertStrColor(COLORREF vertStrClr,BOOL bRedraw /*= FALSE*/)
{
	m_vertStrClr = vertStrClr;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

COLORREF CMyDrawCurveDlg::GetVertStrColor()
{
	return m_vertStrClr;  
}

void CMyDrawCurveDlg::SetVertLineColor(COLORREF vertLineClr,BOOL bRedraw /*= FALSE*/)
{
	m_vertLineClr = vertLineClr;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

COLORREF CMyDrawCurveDlg::GetVertLineColor()
{
	return m_vertLineClr;
}

void CMyDrawCurveDlg::SetHoriBaseLineColor(COLORREF horiBaseLineClr,BOOL bRedraw /*= FALSE*/)
{
	m_horiBaseLineClr = horiBaseLineClr;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

COLORREF CMyDrawCurveDlg::GetHoriBaseLineColor()
{
	return m_horiBaseLineClr;
}

void CMyDrawCurveDlg::SetVertBaseLineColor(COLORREF vertBaseLineClr,BOOL bRedraw /*= FALSE*/)
{
	m_vertBaseLineClr = vertBaseLineClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

COLORREF CMyDrawCurveDlg::GetVertBaseLineColor()
{
	return m_vertBaseLineClr;
}

void CMyDrawCurveDlg::SetXSeparatorColor(COLORREF xSptClr,BOOL bRedraw /*= FALSE*/)
{
	m_xSptClr = xSptClr;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

COLORREF CMyDrawCurveDlg::GetXSeparatorColor()
{
	return m_xSptClr;
}

void CMyDrawCurveDlg::SetRoundTagInnerColor(COLORREF roundTagInnerClr,BOOL bRedraw /*= FALSE*/)
{
	m_roundTagInnerClr = roundTagInnerClr;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

COLORREF CMyDrawCurveDlg::GetRoundTagInnerColor()
{
	return m_roundTagInnerClr;
}

BOOL CMyDrawCurveDlg::SetRoundTagLineWidth(int roundTagLineWidth,BOOL bRedraw /*= FALSE*/)
{
	if(roundTagLineWidth > 100)
	{
		return FALSE;
	}
	m_roundTagLineWidth = roundTagLineWidth;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return TRUE;
}

UINT CMyDrawCurveDlg::GetRoundTagLineWidth()
{
	return m_roundTagLineWidth;
}

BOOL CMyDrawCurveDlg::SetRoundTagLineRadius(int roundTagLineRadius,BOOL bRedraw /*= FALSE*/)
{
	if(roundTagLineRadius > 100)
	{
		return FALSE;
	}
	m_roundTagRadius = roundTagLineRadius;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return TRUE;
}

UINT CMyDrawCurveDlg::GetRoundTagLineRadius()
{
	return m_roundTagRadius;
}

void CMyDrawCurveDlg::SetHoriLineWidth(int horiLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_horiLineWidth = horiLineWidth;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

int CMyDrawCurveDlg::GetHoriLineWidth()
{
	return m_horiLineWidth;
}

void CMyDrawCurveDlg::SetVertLineWidth(int vertLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_vertLineWidth = vertLineWidth;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

int CMyDrawCurveDlg::GetVertLineWidth()
{
	return m_vertLineWidth;
}

void CMyDrawCurveDlg::SetHoriSptWidth(int horiSptWidth,BOOL bRedraw /*= FALSE*/)
{
	m_horiSptWidth = horiSptWidth;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

int CMyDrawCurveDlg::GetHoriSptWidth()
{
	return m_horiSptWidth;
}

void CMyDrawCurveDlg::SetCurveWidth(int curveWidth,BOOL bRedraw /*= FALSE*/)
{
	m_curveWidth = curveWidth;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

int CMyDrawCurveDlg::GetCurveWidth()
{
	return m_curveWidth;
}

void CMyDrawCurveDlg::SetHoriBaseLineWidth(int horiBaseLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_horiBaseLineWidth = horiBaseLineWidth;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

int CMyDrawCurveDlg::GetHoriBaseLineWidth()
{
	return m_horiBaseLineWidth;
}

void CMyDrawCurveDlg::SetVertBaseLineWidth(int vertBaseLineWidth,BOOL bRedraw /*= FALSE*/)
{
	m_vertBaseLineWidth = vertBaseLineWidth;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

int CMyDrawCurveDlg::GetVertBaseLineWidth()
{
	return m_vertBaseLineWidth;
}

void CMyDrawCurveDlg::SetShowVertLine(BOOL bShowVertLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowVertLine = bShowVertLine;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

BOOL CMyDrawCurveDlg::GetShowVertLine()
{
	return m_bShowVertLine;
}

void CMyDrawCurveDlg::SetShowHoriLine(BOOL bShowHoriLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowHoriLine = bShowHoriLine;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

BOOL CMyDrawCurveDlg::GetShowHoriLine()
{
	return m_bShowHoriLine;
}

void CMyDrawCurveDlg::SetShowHoriSeparator(BOOL bShowHoriSpt,BOOL bRedraw /*= FALSE*/)
{
	m_bShowHoriSpt = bShowHoriSpt;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

BOOL CMyDrawCurveDlg::GetShowHoriSeparator()
{
	return m_bShowHoriSpt;
}

void CMyDrawCurveDlg::SetShowRoundTag(BOOL bShowRoundTag,BOOL bRedraw /*= FALSE*/)
{
	m_bShowRoundTag = bShowRoundTag;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
}

BOOL CMyDrawCurveDlg::GetShowRoundTag()
{
	return m_bShowRoundTag;
}

void CMyDrawCurveDlg::SetShowVertBaseLine(BOOL bShowVertBaseLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowVertBaseLine = bShowVertBaseLine;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

BOOL CMyDrawCurveDlg::GetShowVertBaseLine()
{
	return m_bShowVertBaseLine;
}

void CMyDrawCurveDlg::SetShowHoriBaseLine(BOOL bShowHoriBaseLine,BOOL bRedraw /*= FALSE*/)
{
	m_bShowHoriBaseLine = bShowHoriBaseLine;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
}

BOOL CMyDrawCurveDlg::GetShowHoriBaseLine()
{
	return m_bShowHoriBaseLine;
}

BOOL CMyDrawCurveDlg::SetHoriStringOutStep(int horiStrOutStep,BOOL bRedraw /*= FALSE*/)
{
	if(horiStrOutStep <= 0)
	{
		return FALSE;
	}
	m_horiStrOutStep = horiStrOutStep;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return TRUE;
}

int CMyDrawCurveDlg::GetHoriStringOutStep()
{
	return m_horiStrOutStep;
}

BOOL CMyDrawCurveDlg::SetVertStringOutStep(int vertStrOutStep,BOOL bRedraw /*= FALSE*/)
{
	if(vertStrOutStep <= 0)
	{
		return FALSE;
	}
	m_vertStrOutStep = vertStrOutStep;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return TRUE;
}

int CMyDrawCurveDlg::GetVertStringOutStep()
{
	return m_vertStrOutStep;
}

BOOL CMyDrawCurveDlg::SetHoriDecimalPointLength(UINT horiDecPtLen,BOOL bRedraw /*= FALSE*/)
{
	if(horiDecPtLen > 20)
	{
		return FALSE;
	}
	m_horiDecPtLen = horiDecPtLen;
	m_iHoriDecPtLenSet = horiDecPtLen;
	m_realUsedHoriDotLen = m_horiDecPtLen;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return TRUE;
}

UINT CMyDrawCurveDlg::GetHoriDecimalPointLength()
{
	return m_horiDecPtLen;
}

BOOL CMyDrawCurveDlg::SetVertDecimalPointLength(UINT vertDecPtLen,BOOL bRedraw /*= FALSE*/)
{
	if(vertDecPtLen > 20)
	{
		return FALSE;
	}
	m_vertDecPtLen = vertDecPtLen;
	m_realUsedVertDotLen = m_vertDecPtLen;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return TRUE;
}

UINT CMyDrawCurveDlg::GetVertDecimalPointLength()
{
	return m_vertDecPtLen;
}

COLORREF CMyDrawCurveDlg::GetChannelCurveColor(int chanId)
{
	COLORREF clr = RGB(0,0,0);
	if(1 == chanId)
	{
		clr = RGB(211,75,209);
	}
	else if(2 == chanId)
	{
		clr = RGB(26,125,244);
	}
	else if(3 == chanId)
	{
		clr = RGB(250,109,62);
	}
	else if(4 == chanId)
	{
		clr = RGB(235,217,33);
	}
	else
	{
		clr = RGB(60,198,190);
	}

	return clr;
}

void CMyDrawCurveDlg::SetCaptionHeight(int capHeight,BOOL bRedraw /*= FALSE*/)
{
	m_capHeight = capHeight;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return;
}

int CMyDrawCurveDlg::GetCaptionHeght()
{
	return m_capHeight;
}

BOOL CMyDrawCurveDlg::SetRoundRectParam(int roundX,int roundY,BOOL bRedraw /*= FALSE*/)
{
	m_roundX = roundX;
	m_roundY = roundY;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			if(SetRoundRect(m_roundX,m_roundY))
			{
				return TRUE;
			}
			else
			{
				return FALSE;
			}	
		}
	}
	return TRUE;
}

void CMyDrawCurveDlg::GetRoundRectParam(int& roundX,int& roundY)
{
	roundX = m_roundX;
	roundY = m_roundY;
}

void CMyDrawCurveDlg::SetDlgSize(int dlgWidth,int dlgHeight,BOOL bRedraw /*= FALSE*/)
{
	m_dlgSize.cx = dlgWidth;
	m_dlgSize.cy = dlgHeight;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return;
}

void CMyDrawCurveDlg::GetDlgSize(int& dlgWidth,int& dlgHeight)
{
	dlgWidth = m_dlgSize.cx;
	dlgHeight = m_dlgSize.cy;
	return;
}

void CMyDrawCurveDlg::SetLeftTopPosition(int left,int top,BOOL bRedraw /*= FALSE*/)
{
	m_left = left;
	m_top = top;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			MoveWindow(m_left,m_top,m_dlgSize.cx,m_dlgSize.cy);
		}
	}
	return;
}

void CMyDrawCurveDlg::GetLeftTopPosition(int& left,int& top)
{

}

void CMyDrawCurveDlg::SetLegendHeight(int legendHeight,BOOL bRedraw /*= FALSE*/)
{
	m_lgdHeight = legendHeight;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return;
}

void CMyDrawCurveDlg::GetLegendHeight(int& legendHeight)
{
	legendHeight = m_lgdHeight;
	return;
}

void CMyDrawCurveDlg::SetLeftIndent(int leftIndent,BOOL bRedraw /*= FALSE*/)
{
	m_leftIndent = leftIndent;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return;
}

void CMyDrawCurveDlg::GetLeftIndent(int& leftIndent)
{
	leftIndent = m_leftIndent;
	return;
}

void CMyDrawCurveDlg::SetRightIndent(int rightIndent,BOOL bRedraw /*= FALSE*/)
{
	m_rightIndent = rightIndent;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return;
}

void CMyDrawCurveDlg::GetRightIndent(int& rightIndent)
{
	rightIndent = m_rightIndent;
	return;
}

void CMyDrawCurveDlg::SetTopIndent(int topIndent,BOOL bRedraw /*= FALSE*/)
{
	m_topIndent = topIndent;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return;
}

void CMyDrawCurveDlg::GetTopIndent(int& topIndent)
{
	topIndent = m_topIndent;
	return;
}

void CMyDrawCurveDlg::SetBottomIndent(int bottomIndent,BOOL bRedraw /*= FALSE*/)
{
	m_bottomIndent = bottomIndent;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

void CMyDrawCurveDlg::GetBottomIndent(int& bottomIndent)
{
	bottomIndent = m_bottomIndent;
	return;
}

void CMyDrawCurveDlg::HandSetExpressionXYAxisMinMax(BOOL bRedraw /*= FALSE*/)
{
	if(m_lineNum < 1)
	{
		return;
	}

	double minX = 0,maxX = 1,minY = 0,maxY = 0;
	double minXTemp = 0,maxXTemp = 1,minYTemp = 0,maxYTemp = 0;
	m_pLineInfo[0].GetLineXYMinMax(minX,maxX,minY,maxY);
	m_pLineInfo[0].GetDotXYMinMax(minXTemp,maxXTemp,minYTemp,maxYTemp);
	if(minX > minXTemp)
	{
		minX = minXTemp;
	}
	if(maxX < maxXTemp)
	{
		maxX = maxXTemp;
	}
	if(minY > minYTemp)
	{
		minY = minYTemp;
	}
	if(maxY < maxYTemp)
	{
		maxY = maxYTemp;
	}

	for(int i = 1;i < m_lineNum;++i)
	{
		m_pLineInfo[i].GetLineXYMinMax(minXTemp,maxXTemp,minYTemp,maxYTemp);
		if(minX > minXTemp)
		{
			minX = minXTemp;
		}
		if(maxX < maxXTemp)
		{
			maxX = maxXTemp;
		}
		if(minY > minYTemp)
		{
			minY = minYTemp;
		}
		if(maxY < maxYTemp)
		{
			maxY = maxYTemp;
		}

		m_pLineInfo[i].GetDotXYMinMax(minXTemp,maxXTemp,minYTemp,maxYTemp);

		if(minX > minXTemp)
		{
			minX = minXTemp;
		}
		if(maxX < maxXTemp)
		{
			maxX = maxXTemp;
		}
		if(minY > minYTemp)
		{
			minY = minYTemp;
		}
		if(maxY < maxYTemp)
		{
			maxY = maxYTemp; 
		}
	}

	if(minX == maxX)
	{
		maxX = minX + 1;
	}
	if(minY == maxY)
	{
		maxY = minY + 1;
	}

	//表达式计算最大最小值后，向上向下取整，防止标曲首点或者尾点显示不完全
	if(ceil(maxY) - maxY < 0.2)
	{
		maxY = ceil(maxY) + 0.5;
	}
	else if(ceil(maxY) - maxY > 0.5)
	{
		maxY += 0.5;
	}
	else
	{
		maxY = ceil(maxY);
	}

	if(minY - floor(minY) < 0.2)
	{
		minY = floor(minY) - 0.5;
	}
	else if(minY - floor(minY) > 0.5)
	{
		minY -= 0.5;
	}
	else
	{
		minY = floor(minY);
	}

	if(ceil(maxX) - maxX < 0.2)
	{
		maxX = ceil(maxX) + 0.5;
	}
	else if(ceil(maxX) - maxX > 0.5)
	{
		maxX += 0.5;
	}
	else
	{
		maxX = ceil(maxX);
	}

	if(minX - floor(minX) < 0.2)
	{
		minX = floor(minX) - 0.5;
	}
	else if(minX - floor(minX) > 0.5)
	{
		minX -= 0.5;
	}
	else
	{
		minX = floor(minX);
	}
	
	SetVertEndValue(maxY);
	SetVertStartValue(minY);
	SetHoriEndValue(maxX);
	SetHoriStartValue(minX);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

void CMyDrawCurveDlg::HandSetGeneralLineXYAxisMinMax(double dMultiplier /*= 1.1*/,BOOL bRedraw /*= FALSE*/)
{
	if(0 == dMultiplier)
	{
		return;
	}

	if(m_lineNum < 1)
	{
		return;
	}

	double minX = 0,maxX = 1,minY = 0,maxY = 0;
	double minXTemp = 0,maxXTemp = 1,minYTemp = 0,maxYTemp = 0;

	int startIndex = 0;
	BOOL bChanged = FALSE,bFst = TRUE;

	for(int i = startIndex;i < m_lineNum;++i)
	{
		if(!m_pLineInfo[i].m_bShow)
		{
			continue;
		}

		if(m_pLineInfo[i].GetGeneralDotMinMax(minXTemp,maxXTemp,minYTemp,maxYTemp))
		{
			bChanged = TRUE;

			if(bFst)
			{
				minX = minXTemp;
				maxX = maxXTemp;
				minY = minYTemp;
				maxY = maxYTemp;
				bFst = FALSE;
			}
			else
			{
				if(minX > minXTemp)
				{
					minX = minXTemp;
				}
				if(maxX < maxXTemp)
				{
					maxX = maxXTemp;
				}
				if(minY > minYTemp)
				{
					minY = minYTemp;
				}
				if(maxY < maxYTemp)
				{
					maxY = maxYTemp;
				}
			}
		}
	}

	if(bChanged)
	{
		double newMax = 0;
		if(maxY < 0)
		{
			newMax = maxY / dMultiplier;
		}
		else
		{
			newMax = maxY * dMultiplier;
		}
		
		//20220627，孙文郎，防止出现“曲线中点的最小值能显示，曲线的最小值未能显示出来”
		if(m_bUseMinYDecre && minY < m_dMinYDecreThre)
		{
			minY -= m_dMinYDecrement;
		}
		else
		{
			if(minY >= 0)
			{
				minY = minY / dMultiplier;
			}
			else
			{
				minY = minY * dMultiplier;
			}
		}

		SetVertEndValue(newMax);
		SetVertStartValue(minY);
		
		SetHoriEndValue(maxX);
		SetHoriStartValue(minX);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

void CMyDrawCurveDlg::HandSetGeneralLineYAxisMinMax(double dMultiplier /*= 1.1*/,BOOL bRedraw /*= FALSE*/)
{
	if(0 == dMultiplier)
	{
		return;
	}

	if(m_lineNum < 1)
	{
		return;
	}

	double minX = 0,maxX = 1,minY = 0,maxY = 0;
	double minXTemp = 0,maxXTemp = 1,minYTemp = 0,maxYTemp = 0;

	int startIndex = 0;
	BOOL bYAxisSelected = FALSE;
	if(bYAxisSelected)
	{
		int tempInt = 1;
		for(;tempInt < m_lineNum;++tempInt)
		{
			if(m_pLineInfo[tempInt].m_bShow)
			{
				break;
			}
		}
		startIndex = tempInt;
	}

	if(startIndex >= m_lineNum)
	{
		return;
	}
	m_pLineInfo[startIndex].GetGeneralDotMinMax(minX,maxX,minY,maxY);
	++startIndex;

	BOOL bChanged = FALSE;

	for(int i = startIndex;i < m_lineNum;++i)
	{
		if(!m_pLineInfo[i].m_bShow)
		{
			continue;
		}

		if(m_pLineInfo[i].GetGeneralDotMinMax(minXTemp,maxXTemp,minYTemp,maxYTemp))
		{
			bChanged = TRUE;

			if(minX > minXTemp)
			{
				minX = minXTemp;
			}
			if(maxX < maxXTemp)
			{
				maxX = maxXTemp;
			}
			if(minY > minYTemp)
			{
				minY = minYTemp;
			}
			if(maxY < maxYTemp)
			{
				maxY = maxYTemp;
			}
		}
	}

	if(bChanged)
	{
		double newMax = 0;
		if(maxY < 0)
		{
			newMax = maxY / dMultiplier;
		}
		else
		{
			newMax = maxY * dMultiplier;
		}

		//20220617，孙文郎，防止出现“曲线中点的最小值能显示，曲线的最小值未能显示出来”
		if(m_bUseMinYDecre && minY < m_dMinYDecreThre)
		{
			minY -= m_dMinYDecrement;
		}
		else
		{
			if(minY >= 0)
			{
				minY = minY / dMultiplier;
			}
			else
			{
				minY = minY * dMultiplier;
			}
		}

		SetVertEndValue(newMax);
		SetVertStartValue(minY);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}


//lzh:设置曲线的数量
BOOL CMyDrawCurveDlg::SetLineNum(int lineNum, BOOL bRedraw /*= FALSE*/)
{
	if (lineNum < 1)
	{
		return FALSE;
	}

	if (NULL != m_pLineInfo)
	{
		delete[] m_pLineInfo;
		m_pLineInfo = NULL;
	}

	m_pLineInfo = new CLineInfo[lineNum];

	BOOL ret = FALSE;
	if (NULL != m_pLineInfo)
	{
		m_lineNum = lineNum;
		ret = TRUE;
	}
	else
	{
		m_lineNum = 0;
	}

	if (bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return ret;
}

int CMyDrawCurveDlg::SetLineParam(int lineIndex,COLORREF lineClr,int chanId,BOOL bShow,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo)
	{
		return -1;
	}

	if(lineIndex >= m_lineNum)
	{
		return -2;
	}

	m_pLineInfo[lineIndex].m_lineClr = lineClr;
	m_pLineInfo[lineIndex].m_bShow = bShow;
	m_pLineInfo[lineIndex].m_chanId = chanId;

	IndexToRowColId(lineIndex - chanId * REALTUBENUM,m_pLineInfo[lineIndex].m_rowId,m_pLineInfo[lineIndex].m_colId);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyDrawCurveDlg::SetLineParam(int iChanId,CString strDyeName,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo)
	{
		return -1;
	}

	for(int i = 0;i < m_lineNum;++i)
	{
		if(iChanId == m_pLineInfo[i].GetChanId())
		{
			m_pLineInfo[i].SetDyeName(strDyeName);
			break;
		}
	}

	if(bRedraw && m_bShowExpressoin && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyDrawCurveDlg::SetLineShow(int lineIndex,BOOL bShow,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo)
	{
		return -1;
	}

	if(lineIndex < 0)
	{
		return -2;
	}

	if(lineIndex >= m_lineNum)
	{
		return -3;
	}

	m_pLineInfo[lineIndex].m_bShow = bShow;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyDrawCurveDlg::SetLineXAxisMillisecondTime(int lineIndex,BOOL bXAxisMsTime,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo)
	{
		return -1;
	}

	if(lineIndex < 0)
	{
		return -2;
	}

	if(lineIndex >= m_lineNum)
	{
		return -3;
	}

	m_pLineInfo[lineIndex].SetBeXAxisMillisecondTime(bXAxisMsTime);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

void CMyDrawCurveDlg::SetCurveType(ECURVETYPE eCurveType,BOOL bRedraw /*= FALSE*/)
{
	m_eCurveType = eCurveType;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
}

ECURVETYPE CMyDrawCurveDlg::GetCurveType()
{
	return m_eCurveType;
}

void CMyDrawCurveDlg::SetLineColorMode(ELINECLRMODE eLineClrMode,BOOL bRedraw /*= FALSE*/)
{
	m_eLineClrMode = eLineClrMode;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

ELINECLRMODE CMyDrawCurveDlg::GetLineColorMode()
{
	return m_eLineClrMode;
}

void CMyDrawCurveDlg::SetThreLineDataType(ELINEDATATYPE eLineDataType,BOOL bRedraw /*= FALSE*/)
{
	int iChanNum = m_vecChanThreInfo.size();
	if(iChanNum < 1)
	{
		return;
	}

	for(int i = 0;i < iChanNum;++i)
	{
		m_vecChanThreInfo[i].m_eLineDataType = eLineDataType;
	}
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

int CMyDrawCurveDlg::GetThreLineDataType(int iChanId,ELINEDATATYPE& eLineDataType)
{
	if(iChanId < 0)
	{
		return -1;
	}

	if(iChanId >= m_vecChanThreInfo.size())
	{
		return -2;
	}
	eLineDataType = m_vecChanThreInfo[iChanId].m_eLineDataType;
	return 0;
}

int CMyDrawCurveDlg::SetThreLineShowForamt(ELINESHOWFORMAT eLineShowFormat,BOOL bRedraw /*= FALSE*/)
{
	int iChanNum = m_vecChanThreInfo.size();
	if(iChanNum < 1)
	{
		return -1;
	}

	ELINESHOWFORMAT ePreLineShowFormat = ELSF_LINEAR;
	for(int i = 0;i < iChanNum;++i)
	{
		ePreLineShowFormat = m_vecChanThreInfo[i].m_eLineShowFormat;
		m_vecChanThreInfo[i].m_eLineShowFormat = eLineShowFormat;
		if(ePreLineShowFormat != eLineShowFormat)
		{
			CThreLineInfo* pThreLineInfo = m_vecChanThreInfo[i].GetCurSelThreInfo();
			if(NULL != pThreLineInfo)
			{
				pThreLineInfo->m_bFstDrawThreLine = TRUE;
			}
		}
	}
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyDrawCurveDlg::GetThreLineShowForamt(int iChanId,ELINESHOWFORMAT& eLineShowFormat)
{
	if(iChanId < 0)
	{
		return -1;
	}
	if(iChanId >= m_vecChanThreInfo.size())
	{
		return -2;
	}
	eLineShowFormat = m_vecChanThreInfo[iChanId].m_eLineShowFormat;
	return 0;
}

int CMyDrawCurveDlg::SetBeShowThreLine(int iChanId,BOOL bShowThreLine,BOOL bRedraw /*= FALSE*/)
{
	if(iChanId < 0)
	{
		return -1;
	}
	int iChanNum = m_vecChanThreInfo.size();
	if(iChanId >= iChanNum)
	{
		return -2;
	}

	m_vecChanThreInfo[iChanId].m_tliNormolize.m_bShow = bShowThreLine;
	m_vecChanThreInfo[iChanId].m_tliDeltaRn.m_bShow = bShowThreLine;
	m_vecChanThreInfo[iChanId].m_tliLogNormolize.m_bShow = bShowThreLine;
	m_vecChanThreInfo[iChanId].m_tliLogDeltaRn.m_bShow = bShowThreLine;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyDrawCurveDlg::GetBeShowThreLine(int iChanId,BOOL& bShowThreLine)
{
	if(iChanId < 0)
	{
		return -1;
	}
	if(iChanId >= m_vecChanThreInfo.size())
	{
		return -2;
	}
	bShowThreLine = m_vecChanThreInfo[iChanId].m_tliNormolize.m_bShow;
	return 0;
}

int CMyDrawCurveDlg::SetThreLineColor(int iChanId,Color clrThreLine,BOOL bRedraw /*= FALSE*/)
{
	if(iChanId < 0)
	{
		return -1;
	}
	if(iChanId >= m_vecChanThreInfo.size())
	{
		return -2;
	}
	m_vecChanThreInfo[iChanId].m_tliNormolize.SetThreLineColor(clrThreLine);
	m_vecChanThreInfo[iChanId].m_tliDeltaRn.SetThreLineColor(clrThreLine);
	m_vecChanThreInfo[iChanId].m_tliLogNormolize.SetThreLineColor(clrThreLine);
	m_vecChanThreInfo[iChanId].m_tliLogDeltaRn.SetThreLineColor(clrThreLine);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

int CMyDrawCurveDlg::GetThreLineColor(int iChanId,Color& clrThreLine)
{
	if(iChanId < 0)
	{
		return -1;
	}
	if(iChanId >= m_vecChanThreInfo.size())
	{
		return -2;
	}
	clrThreLine = m_vecChanThreInfo[iChanId].m_tliNormolize.GetThreLineColor();	
	return 0;
}

int CMyDrawCurveDlg::SetBeShowExpression(BOOL bShowExpression,BOOL bRedraw /*= FALSE*/)	
{
	m_bShowExpressoin = bShowExpression;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return 0;
}

BOOL CMyDrawCurveDlg::GetBeShowExpression()
{
	return m_bShowExpressoin;
}



double CMyDrawCurveDlg::PixelHeightToYValue(int pixelH)
{
	return (m_figureEndY - pixelH) / m_dAxisVertRt + m_vertStartValue;
}

void CMyDrawCurveDlg::AddHighlightIndex(int index,BOOL bUnique,BOOL bRedraw /*= FALSE*/)
{
	if(index < 0)
	{
		return;
	}
	if(bUnique)
	{
		m_vecHighLightIndex.clear();
	}
	m_vecHighLightIndex.push_back(index);
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyDrawCurveDlg::GetHighlightIndex(vector<int>& vecHighlightIndex)
{
	vecHighlightIndex = m_vecHighLightIndex;
}

CRect CMyDrawCurveDlg::GetFloatWndRegion(CRect rcOrigin)
{
	CRect rect = rcOrigin;
	ClientToScreen(&rect);
	if(rect.right > GeneCRDlg.right)
	{
		if(rect.left <= GeneCRDlg.right && rect.left - rcOrigin.Width() >= 0)
		{
			rect.right = rect.left;
			rect.left = rect.right - rcOrigin.Width();
		}
	}

	return rect;
}

int CMyDrawCurveDlg::SetThrelineFstShow(BOOL bRedraw /*= FALSE*/)
{
	if(!(m_bHaveThreLineFunc && m_bUseFloatFunc))
	{
		return 1;
	}

	int iThreNum = m_vecChanThreInfo.size();
	for(int i = 0;i < iThreNum;++i)
	{
		m_vecChanThreInfo[i].m_tliNormolize.m_bFstDrawThreLine = TRUE;
		m_vecChanThreInfo[i].m_tliDeltaRn.m_bFstDrawThreLine = TRUE;
		m_vecChanThreInfo[i].m_tliLogNormolize.m_bFstDrawThreLine = TRUE;
		m_vecChanThreInfo[i].m_tliLogDeltaRn.m_bFstDrawThreLine = TRUE;
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

CString CMyDrawCurveDlg::GetExpressionString()
{
	for(int i = 0;i < m_lineNum;++i)
	{
		if(m_pLineInfo[i].m_bShow && m_pLineInfo[i].BeHaveFitData() && m_pLineInfo[i].m_bShowExpression)
		{
			return m_pLineInfo[i].GetExpressionString();
		}
	}

	return _T("");
}

BOOL CMyDrawCurveDlg::IsExistVisibleExpression()
{
	if(NULL == m_pLineInfo)
	{
		return FALSE;
	}

	for(int i = 0;i < m_lineNum;++i)
	{
		if(m_pLineInfo[i].IsExpressionVisible())
		{
			return TRUE;
		}
	}

	return FALSE;
}

int CMyDrawCurveDlg::GetVisibleExpressionCount()
{
	if(NULL == m_pLineInfo)
	{
		return 0;
	}

	int iCount = 0;

	for(int i = 0;i < m_lineNum;++i)
	{
		if(m_pLineInfo[i].IsExpressionVisible())
		{
			++iCount;
		}
	}

	return iCount;
}

int CMyDrawCurveDlg::GetVisibleExpressionHeight()
{
	int iCount = GetVisibleExpressionCount();
	if(iCount < 1)
	{
		return 0;
	}

	return iCount * (m_iEprVertInterval + m_fontExpression.m_logFont.lfHeight);
}

void CMyDrawCurveDlg::SetExpressionTag(CString strData,BOOL bRedraw /*= FALSE*/)
{
	m_strEprTag = strData;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		InvalidateRect(m_rcEprTag,FALSE);
	}
	return;
}

CString CMyDrawCurveDlg::GetExpressionTag()
{
	return m_strEprTag;
}

void CMyDrawCurveDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
}

void CMyDrawCurveDlg::SetHaveCaptionOrNot(BOOL bHaveCap,BOOL bRedraw /*= FALSE*/)
{
	m_bHaveCap = bHaveCap;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

BOOL CMyDrawCurveDlg::GetHaveCaptionOrNot()
{
	return m_bHaveCap;
}

void CMyDrawCurveDlg::SetHaveLegendOrNot(BOOL bHaveLgd,BOOL bRedraw /*= FALSE*/)
{
	m_bHaveLgd = bHaveLgd;
	if(bRedraw)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return;
}

BOOL CMyDrawCurveDlg::GetHaveLegendOrNot()
{
	return m_bHaveLgd;
}

void CMyDrawCurveDlg::SetLeanXOrNot(BOOL bLeanX)
{
	m_bLeanX = bLeanX;
}

BOOL CMyDrawCurveDlg::GetLeanXOrNot()
{
	return m_bLeanX;
}

void CMyDrawCurveDlg::SetBackgrounColor(BOOL bkClr,BOOL bRedraw /*= FALSE*/)
{
	m_rgbBkClr = bkClr;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

COLORREF CMyDrawCurveDlg::GetBackgrounColor()
{
	return m_rgbBkClr;
}

void CMyDrawCurveDlg::SetXAxisMode(EXAXISMODE eXAxisMode,BOOL bRedraw /*= FALSE*/)
{
	m_eXAxisMode = eXAxisMode;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}
	return;
}

EXAXISMODE CMyDrawCurveDlg::GetXAxisMode()
{
	return m_eXAxisMode;
}

void CMyDrawCurveDlg::SetBeHaveData(BOOL bHaveData)
{
	m_bHaveData = bHaveData;
	return;
}

BOOL CMyDrawCurveDlg::GetBeHaveData()
{
	return m_bHaveData;
}

int CMyDrawCurveDlg::SetExpressionShowState(int lineId,BOOL bShowExpression,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo || lineId >= m_lineNum || lineId < 0)
	{
		return -1;
	}
	m_pLineInfo[lineId].SetExpressionShowState(bShowExpression);

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return 0;
}

int CMyDrawCurveDlg::GetExpressionShowState(int lineId,BOOL& bShowExpression)
{
	if(NULL == m_pLineInfo || lineId >= m_lineNum || lineId < 0)
	{
		return -1;
	}
	bShowExpression = m_pLineInfo[lineId].GetExpressionShowState();
	return 0;
}

void CMyDrawCurveDlg::SetXAxisExplainShowState(BOOL bShowXAxisExplain,BOOL bRedraw /*= FALSE*/)
{
	m_bShowXAxisExplain = bShowXAxisExplain;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

BOOL CMyDrawCurveDlg::GetXAxisExplainShowState()
{
	return m_bShowXAxisExplain;
}

void CMyDrawCurveDlg::SetXAxisExplain(CString strXAxisExplain,BOOL bRedraw /*= FALSE*/)
{
	m_strXAxisExplain = strXAxisExplain;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

CString CMyDrawCurveDlg::GetXAxisExplain()
{
	return m_strXAxisExplain;
}

void CMyDrawCurveDlg::SetExpressionAutoXYMinMax(BOOL bExpressionAutoXYMinMax,BOOL bRedraw /*= FALSE*/)
{
	m_bExpressionAutoXYMinMax = bExpressionAutoXYMinMax;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

BOOL CMyDrawCurveDlg::GetExpressionAutoXYMinMax()
{
	return m_bExpressionAutoXYMinMax;
}

void CMyDrawCurveDlg::SetBeHaveThreLineFunc(BOOL bHaveThreLineFunc,BOOL bRedraw /*= FALSE*/)
{
	m_bHaveThreLineFunc = bHaveThreLineFunc;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

BOOL CMyDrawCurveDlg::GetBeHaveThreLineFunc()
{
	return m_bHaveThreLineFunc;
}

void CMyDrawCurveDlg::SetBeUseThreLineFunc(BOOL bUseThreLineFunc,BOOL bRedraw /*= FALSE*/)
{
	m_bUseThreLineFunc = bUseThreLineFunc;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

BOOL CMyDrawCurveDlg::GetBeUseThreLineFunc()
{
	return m_bUseThreLineFunc;
}

void CMyDrawCurveDlg::SetXAxisTag(CString xAxisTag,BOOL bRedraw /*= FALSE*/)
{
	m_strXAxisTag = xAxisTag;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

CString CMyDrawCurveDlg::GetXAxisTag()
{
	return m_strXAxisTag;
}

void CMyDrawCurveDlg::SetYAxisTag(CString yAxisTag,BOOL bRedraw /*= FALSE*/)
{
	m_strYAxisTag = yAxisTag;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

CString CMyDrawCurveDlg::GetYAxisTag()
{
	return m_strYAxisTag;
}

void CMyDrawCurveDlg::SetBeStdCrv(BOOL bStdCrv,BOOL bRedraw /*= FALSE*/)
{
	m_bstdCrv = bStdCrv;
	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return;
}

BOOL CMyDrawCurveDlg::GetBeStdCrv()
{
	return m_bstdCrv;
}

void CMyDrawCurveDlg::SetBeUseFloatFunc(BOOL bUseFloatFunc)
{
	m_bUseFloatFunc = bUseFloatFunc;
}

BOOL CMyDrawCurveDlg::GetBeUseFloatFunc()
{
	return m_bUseFloatFunc;
}

void CMyDrawCurveDlg::SetYAxisMinOfMax(double dMinOfMax)
{
	m_dYAxisMinOfMax = dMinOfMax;
}

double CMyDrawCurveDlg::GetYAxisMinOfMax()
{
	return m_dYAxisMinOfMax;
}

void CMyDrawCurveDlg::SetBeShieldYAxisMinOfMax(BOOL bShield)
{
	m_bShieldYAxisMinOfMax = bShield;
}

BOOL CMyDrawCurveDlg::GetBeShieldYAxisMinOfMax()
{
	return m_bShieldYAxisMinOfMax;
}

int CMyDrawCurveDlg::SetLinearFitData(int lineId,LINEARFITDATA* pLinearFitData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo || lineId >= m_lineNum || lineId < 0)
	{
		return -1;
	}

	if(0 != m_pLineInfo[lineId].SetLinearFitData(pLinearFitData))
	{
		return -2;
	}

	m_bHaveData = TRUE;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return 0;
}

int CMyDrawCurveDlg::SetLinearFitData(int iLineId,CLinearFitData* pLinearFitData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo || iLineId >= m_lineNum || iLineId < 0)
	{
		return -1;
	}

	if(0 != m_pLineInfo[iLineId].SetLinearFitData(pLinearFitData))
	{
		return -2;
	}

	m_bHaveData = TRUE;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate();
	}

	return 0;
}

void CMyDrawCurveDlg::SetMinYDecrementParam(BOOL bUse,double dThre,double dDecre)
{
	m_bUseMinYDecre = bUse;
	m_dMinYDecreThre = dThre;
	m_dMinYDecrement = dDecre;
	return;
}

BOOL CMyDrawCurveDlg::IsUseMinYDecrement()
{
	return m_bUseMinYDecre;
}

double CMyDrawCurveDlg::GetMinYDecrementThreshold()
{
	return m_dMinYDecreThre;
}

double CMyDrawCurveDlg::GetMinYDecrement()
{
	return m_dMinYDecrement;
}

int CMyDrawCurveDlg::AddNewDot(int lineIndex,CMyPoint& myPoint,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == m_pLineInfo)
	{
		return -1;
	}

	if(lineIndex >= m_lineNum)
	{
		return -2;
	}

	m_pLineInfo[lineIndex].AddDot(myPoint);

	m_bHaveData = TRUE;

	if(GENERALCURVE == m_eCurveType || (STDLINE == m_eCurveType && m_bExpressionAutoXYMinMax))
	{
		if(myPoint.m_Y > GetVertEndValue())
		{
			//SetVertEndValue(myPoint.m_Y * 1.3);
		}
		if(myPoint.m_Y < GetVertStartValue())
		{
			//SetVertStartValue(myPoint.m_Y);
		}
		if(myPoint.m_X > GetHoriEndValue())
		{
			SetHoriEndValue(myPoint.m_X);
		}
		if(myPoint.m_X < GetHoriStartValue())
		{
			SetHoriStartValue(myPoint.m_X);
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyDrawCurveDlg::AddNewDotAutoCompareXYMinMax(int lineIndex,CMyPoint& myPoint,BOOL bRedraw /*= FALSE*/,double dZoomRatio /*= 1.3*/)
{
	if(NULL == m_pLineInfo)
	{
		return -1;
	}

	if(lineIndex >= m_lineNum)
	{
		return -2;
	}

	m_pLineInfo[lineIndex].AddDot(myPoint);

	m_bHaveData = TRUE;

	if(GENERALCURVE == m_eCurveType || (STDLINE == m_eCurveType && m_bExpressionAutoXYMinMax))
	{
		if(myPoint.m_Y > GetVertEndValue())
		{
			SetVertEndValue(myPoint.m_Y * dZoomRatio);
		}
		if(myPoint.m_Y < GetVertStartValue())
		{
			if(0 == dZoomRatio)
			{
				SetVertStartValue(myPoint.m_Y);
			}
			else 
			{	
				if(myPoint.m_Y >= 0)
				{
					SetVertStartValue(myPoint.m_Y / dZoomRatio);
				}
				else
				{
					SetVertStartValue(myPoint.m_Y * dZoomRatio);
				}
			}
		}
		if(myPoint.m_X > GetHoriEndValue())
		{
			SetHoriEndValue(myPoint.m_X);
		}
		if(myPoint.m_X < GetHoriStartValue())
		{
			SetHoriStartValue(myPoint.m_X);
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

void CMyDrawCurveDlg::ClearLine(BOOL bRedraw /*= FALSE*/)
{
	m_bHaveData = FALSE;

	if(NULL != m_pLineInfo)
	{
		delete [] m_pLineInfo;
		m_pLineInfo = NULL;
	}
	m_lineNum = 0;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}
	return;
}

void CMyDrawCurveDlg::DeleteLineData(BOOL bRedraw)
{
	m_bHaveData = FALSE;

	if(NULL != m_pLineInfo)
	{
		for(int i = 0;i < m_lineNum;++i)
		{
			m_pLineInfo[i].m_lnkDots.Clear();

			delete [] m_pLineInfo[i].m_linearFitData.pDataX;
			m_pLineInfo[i].m_linearFitData.pDataX = NULL;
			delete [] m_pLineInfo[i].m_linearFitData.pDataY;
			m_pLineInfo[i].m_linearFitData.pDataY = NULL;
			m_pLineInfo[i].m_linearFitData.xLen = 0;
			m_pLineInfo[i].m_linearFitData.yLen = 0;
			m_pLineInfo[i].m_linearFitData.intercept = 0;
			m_pLineInfo[i].m_linearFitData.rSquare = 0;
			m_pLineInfo[i].m_linearFitData.slope = 0;
			m_pLineInfo[i].m_linearMinPt.m_X = 0;
			m_pLineInfo[i].m_linearMinPt.m_Y = 0;
			m_pLineInfo[i].m_linearMaxPt.m_X = 0;
			m_pLineInfo[i].m_linearMaxPt.m_Y = 0;
		}

		if(bRedraw && NULL != GetSafeHwnd())
		{
			Invalidate(FALSE);
		}
	}

	return;
}

int CMyDrawCurveDlg::DeleteOneLineData(int lineIndex,BOOL bRedraw)
{
	if(1 == m_lineNum)
	{
		m_bHaveData = FALSE;
	}

	if(lineIndex < 0)
	{
		return -1;
	}
	if(lineIndex >= m_lineNum)
	{
		return -2;
	}
	
	if(NULL == m_pLineInfo)
	{
		return 1;
	}

	CLineInfo& lineInfo = m_pLineInfo[lineIndex];

	lineInfo.m_lnkDots.Clear();

	delete [] lineInfo.m_linearFitData.pDataX;
	lineInfo.m_linearFitData.pDataX = NULL;
	delete [] lineInfo.m_linearFitData.pDataY;
	lineInfo.m_linearFitData.pDataY = NULL;
	lineInfo.m_linearFitData.xLen = 0;
	lineInfo.m_linearFitData.yLen = 0;
	lineInfo.m_linearFitData.intercept = 0;
	lineInfo.m_linearFitData.rSquare = 0;
	lineInfo.m_linearFitData.slope = 0;
	lineInfo.m_linearMinPt.m_X = 0;
	lineInfo.m_linearMinPt.m_Y = 0;
	lineInfo.m_linearMaxPt.m_X = 0;
	lineInfo.m_linearMaxPt.m_Y = 0;

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CMyDrawCurveDlg::SetLineString(int lineIndex,int iChanIndex,CString strTag,CString strDyeName,CString strTarget)
{
	if(NULL == m_pLineInfo)
	{
		return -1;
	}

	if(lineIndex >= m_lineNum)
	{
		return -2;
	}

	m_pLineInfo[lineIndex].m_chanId = iChanIndex;
	m_pLineInfo[lineIndex].m_strHoleTag = strTag;
	m_pLineInfo[lineIndex].m_strDyeName = strDyeName;
	m_pLineInfo[lineIndex].m_strTaget = strTarget;

	return 0;
}

void CMyDrawCurveDlg::InitCtrl()
{
	InitBt();
	InitSt();
}

void CMyDrawCurveDlg::SetCtrlPos()
{
	m_stCapInfo.SetWindowPos(this,30 * m_dHoriRatio,20 * m_dVertRatio,170 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
	m_btClose.SetWindowPos(this,1300 * m_dHoriRatio,24 * m_dVertRatio,32 * m_dHoriRatio,32 * m_dVertRatio,SWP_NOZORDER);
}

void CMyDrawCurveDlg::ShowCtrl()
{
	if(m_bHaveCap)
	{
		m_btClose.ShowWindow(SW_SHOW);
		m_stCapInfo.ShowWindow(SW_SHOW);
	}
}

void CMyDrawCurveDlg::CreateBt()
{
	m_btClose.Create(_T(""), WS_CHILD | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_DRAWCURVEDLG_BT_CLOSE);
}

void CMyDrawCurveDlg::CreateSt()
{
	m_stCapInfo.Create(_T("标题"), WS_CHILD, CRect(0,0,0,0), this,ID_DRAWCURVEDLG_ST_CAPTION);
}

void CMyDrawCurveDlg::InitBt()
{
	
}

void CMyDrawCurveDlg::InitSt()
{
	m_stCapInfo.SetBkColor(RGB(12,170,226));
	m_stCapInfo.SetMyFont(m_stTxFont,30,FW_NORMAL,TYPE_PIXEL);
	m_stCapInfo.SetWordStyle(DT_VCENTER | DT_LEFT);
	m_stCapInfo.SetTextColor(RGB(240,240,240));
}

void CMyDrawCurveDlg::OnBnClickedClose()
{
	OnOK();
}

int CMyDrawCurveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_iEprVertInterval *= m_dVertRatio;
	m_iEprXStart *= m_dHoriRatio;
	m_iEprYStart *= m_dVertRatio;

	m_floatDlg.Create(IDD_POPUPDLG,this);

	return 0;
}

LRESULT CMyDrawCurveDlg::OnUpdateCurve(WPARAM wParam, LPARAM lParam)
{
	if(0 == wParam)
	{
		if(NULL != GetSafeHwnd())
		{
			Invalidate();
		}
	}
	return 0;
}

void CMyDrawCurveDlg::OnCurveMenuExportPicture()
{
	CFileDialog dlg(FALSE,_T(".bmp"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Picture (*.bmp)|*.bmp||"),NULL);
	if(IDCANCEL == dlg.DoModal()) 
	{
		return ;
	}
	CString strFileName = dlg.GetPathName();

	CurveToPic(strFileName);
	return;
}

BOOL CMyDrawCurveDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
{
	CRgn rgnWnd;
	CRect rcWnd;
	GetClientRect(&rcWnd);
	rgnWnd.CreateRoundRectRgn(rcWnd.left,rcWnd.top,rcWnd.right,rcWnd.bottom,x,y);
	if(!SetWindowRgn(rgnWnd,TRUE))
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}



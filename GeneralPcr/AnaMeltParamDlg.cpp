// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "AnaMeltParamDlg.h"
#include "GeneralPcr.h"

// CAnaMeltParamDlg dialog
IMPLEMENT_DYNAMIC(CAnaMeltParamDlg, CDialogEx)

CAnaMeltParamDlg::CAnaMeltParamDlg(CExperimentData* pExpData,CWnd* pParent /*=NULL*/)
	: CDialogEx(CAnaMeltParamDlg::IDD, pParent)
{
	m_frameWidth = 1;
	m_roundValue = 10;
	m_iWndWidth = 865;
	m_iWndHeight = 666;
	m_iCaptionHeight = 52;
	m_pExpData = pExpData;
	m_bCrossParamChanged = FALSE;
	if(NULL != m_pExpData)
	{
		m_meltAnaParamChangeInfo.SetChannelNum(m_pExpData->GetChannelCount());
	}
}

CAnaMeltParamDlg::~CAnaMeltParamDlg()
{
	
}

void CAnaMeltParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAnaMeltParamDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_ANAMELTPARAMDLG_BT_SAVE,&CAnaMeltParamDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_ANAMELTPARAMDLG_BT_CANCEL,&CAnaMeltParamDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CAnaMeltParamDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	double dHoriRatio = m_dHoriRatio;
	double dVertRatio = m_dVertRatio;

	CRect rect;
	GetClientRect(&rect);

	CDC memDc,memDc2;
	memDc.CreateCompatibleDC(pDC);
	memDc2.CreateCompatibleDC(pDC);

	CBitmap bitMapCB,bitMapCB2;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);
	bitMapCB2.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB2 = memDc2.SelectObject(&bitMapCB2);

	memDc.SetBkMode(TRANSPARENT);
	memDc.BitBlt(0,0,rect.Width(),rect.Height(),pDC,0,0,SRCCOPY);
	memDc2.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(0,0,0));

	BLENDFUNCTION blendFunc;
	blendFunc.BlendOp = AC_SRC_OVER;
	blendFunc.SourceConstantAlpha = CLRALPHAVALUE;
	blendFunc.AlphaFormat = AC_SRC_OVER;
	blendFunc.BlendFlags = 0;
	BOOL ret = memDc.AlphaBlend(0,0,rect.Width(),rect.Height(),&memDc2,0,0,rect.Width(),rect.Height(),blendFunc);

	CRect rcUseful;
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2 - m_iWndWidth / 5 * 2.2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;

	CPen pen;
	CPen* pOldPen = NULL;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCaptionHeight,rcUseful.Width(),rcUseful.Height() - m_iCaptionHeight,0,0,m_roundValue,m_roundValue,FALSE,Color(197,197,197),m_frameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCaptionHeight,m_roundValue,m_roundValue,0,0,FALSE,Color(3,91,199),2,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);

	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(pLangInfo->GetText(311),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCaptionHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	memDc.SetTextColor(CLR_2LEVEL_THEME);
	memDc.FillSolidRect(rcUseful.left + 30 * m_dHoriRatio,rcUseful.top + 74 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.FillSolidRect(rcUseful.left + 34 * m_dHoriRatio,rcUseful.top + 70 * m_dVertRatio,4 * m_dVertRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.DrawText(pLangInfo->GetText(121),CRect(rcUseful.left + 44 * m_dHoriRatio,rcUseful.top + 62 * m_dVertRatio,rcUseful.left + 244 * m_dHoriRatio,rcUseful.top + 89 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.FillSolidRect(rcUseful.left + 30 * m_dHoriRatio,rcUseful.top + 340 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.FillSolidRect(rcUseful.left + 34 * m_dHoriRatio,rcUseful.top + 336 * m_dVertRatio,4 * m_dVertRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.DrawText(pLangInfo->GetText(126),CRect(rcUseful.left + 44 * m_dHoriRatio,rcUseful.top + 328 * m_dVertRatio,rcUseful.left + 244 * m_dHoriRatio,rcUseful.top + 355 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.SelectObject(pOldFont);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	memDc2.SelectObject(&pOldBitmapCB2);
	bitMapCB2.DeleteObject();
	memDc2.DeleteDC();

	return TRUE;
}

int CAnaMeltParamDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	m_iCaptionHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CAnaMeltParamDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < m_iCaptionHeight)
	{
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,point.y));
	}
	else
	{
		CDialogEx::OnLButtonDown(nFlags, point);
	}
#endif

	return;
}

BOOL CAnaMeltParamDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0 * m_dHoriRatio,0 * m_dVertRatio,m_iWndWidth * m_dHoriRatio,m_iWndHeight * m_dVertRatio,SWP_NOZORDER);
	CenterWindow(GetParent());
#endif
	
	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	SetCrossSetParam();
	SetLstMeltParam();

	ShowCtrl();

	return TRUE;
}

void CAnaMeltParamDlg::CreateCtrl()
{
	CreateBt();
	CreateCkBt();
	CreateSt();
	CreateEd();
	CreateCb();
	CreateLst();
}

void CAnaMeltParamDlg::CreateBt()
{
	m_btSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ANAMELTPARAMDLG_BT_SAVE);
	m_btCancel.Create(_T("取消"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ANAMELTPARAMDLG_BT_CANCEL);
}

void CAnaMeltParamDlg::CreateCkBt()
{
	
}

void CAnaMeltParamDlg::CreateSt()
{
	
}

void CAnaMeltParamDlg::CreateEd()
{
	
}

void CAnaMeltParamDlg::CreateCb()
{
	
}

void CAnaMeltParamDlg::CreateLst()
{
	m_lstCrossSet.Create(IDD_MIDDLEDLG,this);
	m_lstMeltParam.Create(IDD_MIDDLEDLG,this);
}

void CAnaMeltParamDlg::InitDlg()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}	

	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CAnaMeltParamDlg::InitCtrl()
{
	InitBt();
	InitCkBt();
	InitSt();
	InitEd();
	InitCb();
	InitLst();
}

void CAnaMeltParamDlg::InitBt()
{
	int iFontH = 18 * m_dTxRatio;

	m_btSave.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
#ifndef GRADIENT_SOME_BTBK
	m_btSave.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),FALSE);
	m_btSave.SetBkColor(COLOR_BTBK_NORMAL,COLOR_BTBK_HOVER,COLOR_BTBK_CLICKED,COLOR_BTBK_DISABLED);
#else
	m_btSave.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btSave.SetBkColor(Color(0X38,0XE0,0XEF),Color(0X3A,0XEF,0XFF),COLOR_SOFT_THEME,Color(0XF1,0XF1,0XF1));
	m_btSave.SetBkColorEnd(COLOR_SOFT_THEME,Color(0X38,0XE0,0XEF),Color(0X02,0X8D,0X99),Color(0XD8,0XD8,0XD8));
#endif
	m_btSave.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XC6,0XC4,0XC4));

	m_btCancel.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btCancel.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CAnaMeltParamDlg::InitCkBt()
{
	
}

void CAnaMeltParamDlg::InitSt()
{
	
}

void CAnaMeltParamDlg::InitEd()
{
	
}

void CAnaMeltParamDlg::InitCb()
{
	
}

void CAnaMeltParamDlg::InitLst()
{
	InitLstCrossSet();
	InitLstMeltParam();
}

void CAnaMeltParamDlg::InitLstCrossSet()
{
	int iFtH = 15 * m_dTxRatio;
	int iFtH2 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFtH = 17 * m_dTxRatio;
		iFtH2 = 17 * m_dTxRatio;
	}

	m_lstCrossSet.SetHeaderHeight(28 * m_dVertRatio);
	m_lstCrossSet.SetMyHeaderFont(m_stTxFont,iFtH,FW_NORMAL,TYPE_PIXEL);
	m_lstCrossSet.SetHeaderBkColor(RGB(228,231,236));
	m_lstCrossSet.SetHeaderTextColor(RGB(0X59,0X59,0X59));
	m_lstCrossSet.SetMyTitleFont(m_stTxFont,iFtH,FW_BOLD,TYPE_PIXEL);
	m_lstCrossSet.SetItemHeight(30 * m_dVertRatio);
	m_lstCrossSet.SetBeShowRowSelectState(FALSE);

	m_lstCrossSet.m_horiScroll.m_arrowWH = 28 * m_dHoriRatio;
	m_lstCrossSet.m_horiScroll.m_scrollH = 28 * m_dVertRatio;
	m_lstCrossSet.m_vertScroll.m_arrowWH = 28 * m_dHoriRatio;
	m_lstCrossSet.m_vertScroll.m_scrollW = 28 * m_dVertRatio;
	m_lstCrossSet.SetMyFont(m_stTxFont,iFtH2,FW_NORMAL,TYPE_PIXEL);
	m_lstCrossSet.SetTextClr1(RGB(0X39,0X45,0X65));

	m_lstCrossSet.SetFuncEditCtrlFont(m_stTxFont,iFtH2,FW_NORMAL,TYPE_PIXEL);

	return;
}

void CAnaMeltParamDlg::InitLstMeltParam()
{
	int iFtH = 15 * m_dTxRatio;
	int iFtH2 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFtH = 17 * m_dTxRatio;
		iFtH2 = 17 * m_dTxRatio;
	}

	m_lstMeltParam.SetHeaderHeight(24 * m_dVertRatio);
	m_lstMeltParam.SetMyHeaderFont(m_stTxFont,iFtH,FW_NORMAL,TYPE_PIXEL);
	m_lstMeltParam.SetMyFont(m_stTxFont,iFtH2,FW_NORMAL,TYPE_PIXEL);
	m_lstMeltParam.SetTextClr1(RGB(0X39,0X45,0X65));
	m_lstMeltParam.SetHeaderTextColor(RGB(0X39,0X45,0X65));
	m_lstMeltParam.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_lstMeltParam.SetItemHeight(37 * m_dVertRatio);
	m_lstMeltParam.SetRowBkSelectedColor(RGB(0XF1,0XF1,0XF1));
	m_lstMeltParam.SetBeShowRowSelectState(FALSE);

	int iTempInt1 = 34 * m_dVertRatio;
	m_lstMeltParam.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstMeltParam.m_horiScroll.m_scrollH = iTempInt1;
	m_lstMeltParam.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstMeltParam.m_vertScroll.m_scrollW = iTempInt1;

	m_lstMeltParam.InsertColumn(0,_T("序号"));
	m_lstMeltParam.InsertColumn(1,_T("通道"));
	m_lstMeltParam.InsertColumn(2,_T("最低温度"));
	m_lstMeltParam.InsertColumn(3,_T("最高温度"));
	m_lstMeltParam.InsertColumn(4,_T("峰高阈值"));

	m_lstMeltParam.SetColumnEditable(2,TRUE);
	m_lstMeltParam.SetColumnEditable(3,TRUE);
	m_lstMeltParam.SetColumnEditable(4,TRUE);

	double dHoriRatio = m_dHoriRatio;
	int width = 809 * dHoriRatio;
	int tempInt = 46 * dHoriRatio;

	m_lstMeltParam.SetColumnWidth(0,tempInt);
	width -= tempInt;
	tempInt = 70 * dHoriRatio;
	m_lstMeltParam.SetColumnWidth(1,tempInt);
	width -= tempInt;
	tempInt = 245 * dHoriRatio;
	m_lstMeltParam.SetColumnWidth(2,tempInt);
	width -= tempInt;
	tempInt = 245 * dHoriRatio;
	m_lstMeltParam.SetColumnWidth(3,tempInt);
	width -= tempInt;
	width -= m_lstMeltParam.GetFrameLineWidth() * 2;
	m_lstMeltParam.SetColumnWidth(4,width);

	CMaxMinInfo maxMinInfo;
	maxMinInfo.SetBeOnlyFloatNumber(TRUE);
	maxMinInfo.SetBeSetMinValue(TRUE);
	maxMinInfo.SetMinValue(4);
	maxMinInfo.SetBeSetMaxValue(TRUE);
	maxMinInfo.SetMaxValue(100);
	maxMinInfo.SetBeLessThanMinSetMin(FALSE);
	m_lstMeltParam.SetColumnMaxMinInfo(2,maxMinInfo);
	m_lstMeltParam.SetColumnMaxMinInfo(3,maxMinInfo);

	maxMinInfo.ClearMaxMinLimit();
	maxMinInfo.SetBeOnlyFloatNumber(TRUE);
	maxMinInfo.SetBeSetMinValue(TRUE);
	maxMinInfo.SetMinValue(0);
	maxMinInfo.SetBeMinEqualSignWork(FALSE);
	m_lstMeltParam.SetColumnMaxMinInfo(4,maxMinInfo);

	return;
}

void CAnaMeltParamDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2 - m_iWndWidth / 5 * 2.2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;
#endif

	m_lstMeltParam.SetWindowPos(this,rcUseful.left + 30 * m_dHoriRatio,rcUseful.top + 90 * m_dVertRatio,809 * m_dHoriRatio,220 * m_dVertRatio,SWP_NOZORDER);
	m_lstCrossSet.SetWindowPos(this,rcUseful.left + 30 * m_dHoriRatio,rcUseful.top + 356 * m_dVertRatio,809 * m_dHoriRatio,210 * m_dVertRatio,SWP_NOZORDER);
	m_btSave.SetWindowPos(this,rcUseful.left + 617 * m_dHoriRatio,rcUseful.top + 600 * m_dVertRatio,90 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
	m_btCancel.SetWindowPos(this,rcUseful.left + 731 * m_dHoriRatio,rcUseful.top + 600 * m_dVertRatio,90 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
}

void CAnaMeltParamDlg::ShowCtrl()
{
	m_lstMeltParam.ShowWindow(SW_SHOW);
	m_lstCrossSet.ShowWindow(SW_SHOW);
	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);

	return;
}

void CAnaMeltParamDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_lstMeltParam.SetColumnText(0,pLangInfo->GetText(111));
	m_lstMeltParam.SetColumnText(1,pLangInfo->GetText(112));
	m_lstMeltParam.SetColumnText(2,pLangInfo->GetText(122));
	m_lstMeltParam.SetColumnText(3,pLangInfo->GetText(123));
	m_lstMeltParam.SetColumnText(4,pLangInfo->GetText(124));

	m_btSave.SetWindowText(pLangInfo->GetText(148));
	m_btCancel.SetWindowText(pLangInfo->GetText(150));

	return;
}

void CAnaMeltParamDlg::OnBnClickedSave()
{
	m_lstCrossSet.EndEDit();
	m_lstMeltParam.EndEDit();

	GetCrossSetParam();
	GetLstMeltParam();

	OnOK();
	return;
}

void CAnaMeltParamDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}

BOOL CAnaMeltParamDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

int CAnaMeltParamDlg::SetCrossSetParam()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	float** ppCrosstalkPara = m_pExpData->GetMeltCrosstalkPara();
	if(NULL == ppCrosstalkPara)
	{
		return -2;
	}

	int iChanCount = m_pExpData->GetChannelCount();

	//PS96不显示校准通道的串扰信息
#ifdef PS96INSTRUMENT
	--iChanCount;
#endif

	if(iChanCount < 1)
	{
		return -3;
	}

	//设置行列数
	SetColumnRowNum(iChanCount);

	CString strTemp(_T(""));
	for(int i = 0;i < iChanCount;++i)
	{
		for(int j = 0;j < iChanCount;++j)
		{
			strTemp.Format(_T("%.3lf"),ppCrosstalkPara[i][j]);
			m_lstCrossSet.SetItemText(i,j + 1,strTemp);
		}
	}

	m_lstCrossSet.BottomBlankSetAutoFitHeight();

	m_lstCrossSet.Invalidate(FALSE);
	
	return 0;
}

int CAnaMeltParamDlg::GetCrossSetParam()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	float** ppCrosstalkPara = m_pExpData->GetMeltCrosstalkPara();
	if(NULL == ppCrosstalkPara)
	{
		return -2;
	}

	int iChanCount = m_lstCrossSet.GetRowNum();
	if(iChanCount < 1)
	{
		return -3;
	}

	int iColumnNum = m_lstCrossSet.GetColNum();
	if(iChanCount != iColumnNum - 1)
	{
		return -4;
	}

	float* pNewData = new float[iChanCount * iChanCount];
	if(NULL == pNewData)
	{
		return -5;
	}

	for(int i = 0;i < iChanCount;++i)
	{
		for(int j = 0;j < iChanCount;++j)
		{
			pNewData[i * iChanCount + j] = _ttof(m_lstCrossSet.GetItemText(i,j + 1));
		}
	}

	for(int i = 0;i < iChanCount;++i)
	{
		for(int j = 0;j < iChanCount;++j)
		{
			if(pNewData[i * iChanCount + j] != ppCrosstalkPara[i][j])
			{
				m_bCrossParamChanged = TRUE;
				goto COMPAREEND;
			}
		}
	}
	
COMPAREEND:
	if(m_bCrossParamChanged)
	{
		for(int i = 0;i < iChanCount;++i)
		{
			for(int j = 0;j < iChanCount;++j)
			{
				ppCrosstalkPara[i][j] = pNewData[i * iChanCount + j];
			}
		}
	}

	delete [] pNewData;

	return 0;
}

int CAnaMeltParamDlg::SetLstMeltParam()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	m_lstMeltParam.Clear();
	CDataAnalysisPara* pAnalysisPara = m_pExpData->GetAnalysisPara();
	if(NULL == pAnalysisPara)
	{
		m_lstMeltParam.Invalidate(FALSE);
		return -2;
	}

	CString tempStr(_T(""));
	int iMeltParamCount = pAnalysisPara->GetMeltCurveParaCount();
	for(int i = 0;i < iMeltParamCount;++i)
	{
		CPtrArray* pAryMeltCrvParam = pAnalysisPara->GetArrayMeltCurveParaPtr();
		tagAnaMeltCurvePara* pAnaMeltCrvParam = (tagAnaMeltCurvePara*)pAryMeltCrvParam->GetAt(i); 
		if(NULL == pAnaMeltCrvParam)
		{
			continue;
		}

		m_lstMeltParam.InsertRow(i);

		tempStr.Format(_T("%d"),i + 1);
		m_lstMeltParam.SetItemText(i,0,tempStr);

		m_lstMeltParam.SetItemText(i,1,pAnaMeltCrvParam->strDye);
		
		tempStr.Format(_T("%.2lf"),pAnaMeltCrvParam->dMinTemp);
		m_lstMeltParam.SetItemText(i,2,tempStr);
		tempStr.Format(_T("%.2lf"),pAnaMeltCrvParam->dMaxTemp);
		m_lstMeltParam.SetItemText(i,3,tempStr);
		tempStr.Format(_T("%.2lf"),pAnaMeltCrvParam->dThreshold);
		m_lstMeltParam.SetItemText(i,4,tempStr);
	}

	m_lstMeltParam.BottomBlankSetAutoFitHeight();
	m_lstMeltParam.Invalidate(FALSE);

	return 0;
}

int CAnaMeltParamDlg::GetLstMeltParam()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	CDataAnalysisPara* pAnalysisPara = m_pExpData->GetAnalysisPara();
	if(NULL == pAnalysisPara)
	{
		return -2;
	}

	int iRowNum = m_lstMeltParam.GetRowNum();
	if(pAnalysisPara->SetMeltCurveParaCount(iRowNum) < 0)
	{
		return -3;
	}

	if(iRowNum < 1)
	{
		return 1;
	}

	CString strTemp(_T("")),str1(_T("")),str2(_T("")),str3(_T("")),str1Old(_T("")),str2Old(_T("")),str3Old(_T(""));
	tagAnaMeltCurvePara* pAnaMeltCrvParam = NULL;
	for(int i = 0;i < iRowNum;++i)
	{
		pAnaMeltCrvParam = pAnalysisPara->GetMeltCurveParaBy(i); 
		if(NULL == pAnaMeltCrvParam)
		{
			continue;
		}

		pAnaMeltCrvParam->strDye = m_lstMeltParam.GetItemText(i,1);
		 
		str1 = m_lstMeltParam.GetItemText(i,2);
		str2 = m_lstMeltParam.GetItemText(i,3);
		str3 = m_lstMeltParam.GetItemText(i,4);

		str1Old.Format(_T("%.2lf"),pAnaMeltCrvParam->dMinTemp);
		str2Old.Format(_T("%.2lf"),pAnaMeltCrvParam->dMaxTemp);
		str3Old.Format(_T("%.2lf"),pAnaMeltCrvParam->dThreshold);

		pAnaMeltCrvParam->dMinTemp = _ttof(str1);
		pAnaMeltCrvParam->dMaxTemp = _ttof(str2);
		pAnaMeltCrvParam->dThreshold = _ttof(str3);

		if(i < m_meltAnaParamChangeInfo.m_iChanNum)
		{
			CChanMeltAnaParamChangeInfo& temp = m_meltAnaParamChangeInfo.m_vecChanMeltChangeInfo[i];
			if(str1 != str1Old)
			{
				temp.m_bMinTemChanged = TRUE;
			}
			if(str2 != str2Old)
			{
				temp.m_bMaxTemChanged = TRUE;
			}
			if(str3 != str3Old)
			{
				temp.m_bPeakThreChanged = TRUE;
			}
		}
	}

	return 0;
}

int CAnaMeltParamDlg::SetColumnRowNum(int iChanNum,BOOL bRedraw /*= FALSE*/)
{
	if(iChanNum < 1)
	{
		return -1;
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString strTemp(_T("")),str1(_T("通道"));
	m_lstCrossSet.InsertColumn(0,pLangInfo->GetText(51));

	CMaxMinInfo maxMinInfo;
	maxMinInfo.SetBeOnlyFloatNumber(TRUE);
	maxMinInfo.SetBeSetMinValue(TRUE);
	maxMinInfo.SetMinValue(0);
	maxMinInfo.SetBeSetMaxValue(TRUE);
	maxMinInfo.SetMaxValue(1);
	maxMinInfo.SetBeMaxMinSimultaneousEffect(TRUE);
	
	str1 = pLangInfo->GetText(51);
	tagAnaMeltCurvePara* pAnaMeltCrvParam = NULL;
	for(int i = 0;i < iChanNum;++i)
	{
		strTemp.Empty();
		pAnaMeltCrvParam = m_pExpData->GetMeltAnaParamAt(i); 
		if(NULL != pAnaMeltCrvParam)
		{
			strTemp = pAnaMeltCrvParam->strDye;
		}
		if(strTemp.IsEmpty())
		{
			strTemp.Format(_T("%s%d"),str1,i + 1);
		}
		m_lstCrossSet.InsertColumn(i + 1,strTemp);

		m_lstCrossSet.SetColumnEditable(i + 1,TRUE);
		m_lstCrossSet.SetColumnMaxMinInfo(i + 1,maxMinInfo);
	}

	for(int i = 0;i < iChanNum;++i)
	{
		m_lstCrossSet.InsertRowSetHeight(i,30 * m_dVertRatio,FALSE);

		strTemp.Empty();
		pAnaMeltCrvParam = m_pExpData->GetMeltAnaParamAt(i); 
		if(NULL != pAnaMeltCrvParam)
		{
			strTemp = pAnaMeltCrvParam->strDye;
		}
		if(strTemp.IsEmpty())
		{
			strTemp.Format(_T("%s%d"),str1,i + 1);
		}
		m_lstCrossSet.SetItemText(i,0,strTemp);

		m_lstCrossSet.SetCellDisableState(i,i + 1,TRUE);
	}

	//设置列宽
	int iWidth = 809 * m_dHoriRatio;
	int iTempInt = 87 * m_dHoriRatio;

	m_lstCrossSet.SetColumnWidth(0,iTempInt);
	iWidth -= iTempInt;

	iTempInt = 120 * m_dHoriRatio;
	if(iChanNum > 0)
	{
		iTempInt = iWidth / iChanNum;
	}

	for(int i = 0;i < iChanNum;++i)
	{
		if(i == iChanNum - 1)
		{
			iWidth -= m_lstCrossSet.GetFrameLineWidth() * 2;
			m_lstCrossSet.SetColumnWidth(i + 1,iWidth);
		}
		else
		{
			m_lstCrossSet.SetColumnWidth(i + 1,iTempInt);
			iWidth -= iTempInt;
		}
	}
	
	if(bRedraw && NULL != m_lstCrossSet.GetSafeHwnd())
	{
		m_lstCrossSet.Invalidate(FALSE);
	}

	return 0;
}


// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "FilterParamSetDlg.h"
#include "GeneralPcr.h"

// CFilterParamSetDlg dialog
IMPLEMENT_DYNAMIC(CFilterParamSetDlg, CDialogEx)

CFilterParamSetDlg::CFilterParamSetDlg(CExperimentData* pExpData,CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterParamSetDlg::IDD, pParent)
{
	m_hWndParent = NULL;
	m_iAngleRadius = 20;
	m_iFrameWidth = 1;
	m_iWndWidth = 800;
	m_iWndHeight = 550;
	m_iCapHeight = 60;
	m_bChanged = FALSE;
	m_pExpData = pExpData;
}

CFilterParamSetDlg::~CFilterParamSetDlg()
{
}

void CFilterParamSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFilterParamSetDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_FILTERPARAMSETDLG_BT_DEFAULTSET,&CFilterParamSetDlg::OnBnClickedDefaultSet)
	ON_BN_CLICKED(ID_FILTERPARAMSETDLG_BT_SAVE,&CFilterParamSetDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_FILTERPARAMSETDLG_BT_CANCEL,&CFilterParamSetDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CFilterParamSetDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(NULL == pLangInfo)
	{
		return TRUE;
	}

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

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCapHeight,rcUseful.Width(),rcUseful.Height(),0,0,m_iAngleRadius,m_iAngleRadius,FALSE,Color(197,197,197),m_iFrameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCapHeight,m_iAngleRadius,m_iAngleRadius,0,0,FALSE,COLOR_SOFT_THEME,2,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	memDc.DrawText(_T("nWnd"),CRect(rcUseful.left + 290 * m_dHoriRatio,rcUseful.top + 100 * m_dVertRatio,rcUseful.left + 365 * m_dHoriRatio,rcUseful.top + 140 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(_T("Times"),CRect(rcUseful.left + 490 * m_dHoriRatio,rcUseful.top + 100 * m_dVertRatio,rcUseful.left + 565 * m_dHoriRatio,rcUseful.top + 140 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(_T("Forward-M"),CRect(rcUseful.left + 170 * m_dHoriRatio,rcUseful.top + 150 * m_dVertRatio,rcUseful.left + 270 * m_dHoriRatio,rcUseful.top + 210 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(_T("Backward-N"),CRect(rcUseful.left + 370 * m_dHoriRatio,rcUseful.top + 150 * m_dVertRatio,rcUseful.left + 470 * m_dHoriRatio,rcUseful.top + 210 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(_T("Times"),CRect(rcUseful.left + 570 * m_dHoriRatio,rcUseful.top + 150 * m_dVertRatio,rcUseful.left + 670 * m_dHoriRatio,rcUseful.top + 210 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(pLangInfo->GetText(177),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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

int CFilterParamSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	m_iCapHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CFilterParamSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < m_iCapHeight)
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

BOOL CFilterParamSetDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0,0,m_iWndWidth,m_iWndHeight,SWP_NOZORDER);
	SetRoundRect(m_iAngleRadius,m_iAngleRadius);
	CenterWindow(GetParent());
#endif

	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();
	SetFilterParam();

	ShowCtrl();

	return TRUE;
}

void CFilterParamSetDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateEd();
	CreateCkbt();
}

void CFilterParamSetDlg::CreateBt()
{
	m_btDefSet.Create(_T("默认"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_BT_DEFAULTSET);
	m_btSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_BT_SAVE);
	m_btCancel.Create(_T("取消"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_BT_CANCEL);
}

void CFilterParamSetDlg::CreateCkbt()
{
	m_ckbtMedian.Create(_T("Median"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_CKBT_MEDIAN);
	m_ckbtFilter1.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_CKBT_FILTER1);
	m_ckbtFilter2.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_CKBT_FILTER2);
	m_ckbtFilter3.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_CKBT_FILTER3);
	m_ckbtFilter4.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_CKBT_FILTER4);
	m_ckbtFilter5.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_FILTERPARAMSETDLG_CKBT_FILTER5);
}

void CFilterParamSetDlg::CreateSt()
{
	
}

void CFilterParamSetDlg::CreateEd()
{
	m_edMedianWnd.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_MEDIANWND);
	m_edMidianTimes.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_MEDIANWND);

	m_edFilter1M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER1M);
	m_edFilter1N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER1N);
	m_edFilter1Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER1TIMES);

	m_edFilter2M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER2M);
	m_edFilter2N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER2N);
	m_edFilter2Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER2TIMES);

	m_edFilter3M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER3M);
	m_edFilter3N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER3N);
	m_edFilter3Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER3TIMES);

	m_edFilter4M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER4M);
	m_edFilter4N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER4N);
	m_edFilter4Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER4TIMES);

	m_edFilter5M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER5M);
	m_edFilter5N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER5N);
	m_edFilter5Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_FILTERPARAMSETDLG_ED_FILTER5TIMES);
}

void CFilterParamSetDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,18 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CFilterParamSetDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitCkbt();
}

void CFilterParamSetDlg::InitBt()
{
	int iFontH = 18 * CFontSet::m_dTxRatio;

	m_btDefSet.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btDefSet.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

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

void CFilterParamSetDlg::InitCkbt()
{
	InitCheckBt(&m_ckbtMedian);
	InitCheckBt(&m_ckbtFilter1);
	InitCheckBt(&m_ckbtFilter2);
	InitCheckBt(&m_ckbtFilter3);
	InitCheckBt(&m_ckbtFilter4);
	InitCheckBt(&m_ckbtFilter5);
}

void CFilterParamSetDlg::InitSt()
{
	
}

void CFilterParamSetDlg::InitEd()
{
	CString tempStr(_T(""));
	int iFontSize = 18 * m_dTxRatio;

	CString tempstr(_T(""));

	InitEditCtrlParam(&m_edMedianWnd);
	InitEditCtrlParam(&m_edMidianTimes);

	InitEditCtrlParam(&m_edFilter1M);
	InitEditCtrlParam(&m_edFilter1N);
	InitEditCtrlParam(&m_edFilter1Times);

	InitEditCtrlParam(&m_edFilter2M);
	InitEditCtrlParam(&m_edFilter2N);
	InitEditCtrlParam(&m_edFilter2Times);

	InitEditCtrlParam(&m_edFilter3M);
	InitEditCtrlParam(&m_edFilter3N);
	InitEditCtrlParam(&m_edFilter3Times);

	InitEditCtrlParam(&m_edFilter4M);
	InitEditCtrlParam(&m_edFilter4N);
	InitEditCtrlParam(&m_edFilter4Times);

	InitEditCtrlParam(&m_edFilter5M);
	InitEditCtrlParam(&m_edFilter5N);
	InitEditCtrlParam(&m_edFilter5Times);
}

void CFilterParamSetDlg::SetCtrlPos()
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

	int dTempInt1 = 90 * m_dVertRatio;
	int dTempInt2 = 76 * m_dHoriRatio;
	int dTempInt3 = 20 * m_dVertRatio;
	int dTempInt4 = 370 * m_dHoriRatio;
	int dTempInt5 = 170 * m_dHoriRatio;
	int dTempInt6 = 570 * m_dHoriRatio;
	int iTempInt7 = 100 * m_dHoriRatio;
	int iTempInt8 = 40 * m_dHoriRatio;

	m_ckbtMedian.SetWindowPos(this,rcUseful.left + dTempInt1,rcUseful.top + 110 * m_dVertRatio,90 * m_dHoriRatio,dTempInt3,SWP_NOZORDER);
	m_ckbtFilter1.SetWindowPos(this,rcUseful.left + dTempInt1,rcUseful.top + 230 * m_dVertRatio,dTempInt2,dTempInt3,SWP_NOZORDER);
	m_ckbtFilter2.SetWindowPos(this,rcUseful.left + dTempInt1,rcUseful.top + 290 * m_dVertRatio,dTempInt2,dTempInt3,SWP_NOZORDER);
	m_ckbtFilter3.SetWindowPos(this,rcUseful.left + dTempInt1,rcUseful.top + 350 * m_dVertRatio,dTempInt2,dTempInt3,SWP_NOZORDER);
	m_ckbtFilter4.SetWindowPos(this,rcUseful.left + dTempInt1,rcUseful.top + 410 * m_dVertRatio,dTempInt2,dTempInt3,SWP_NOZORDER);
	m_ckbtFilter5.SetWindowPos(this,rcUseful.left + dTempInt1,rcUseful.top + 470 * m_dVertRatio,dTempInt2,dTempInt3,SWP_NOZORDER);

	m_edMedianWnd.SetWindowPos(this,rcUseful.left + dTempInt4,rcUseful.top + 100 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edMidianTimes.SetWindowPos(this,rcUseful.left + dTempInt6,rcUseful.top + 100 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);

	m_edFilter1M.SetWindowPos(this,rcUseful.left + dTempInt5,rcUseful.top + 220 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter1N.SetWindowPos(this,rcUseful.left + dTempInt4,rcUseful.top + 220 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter1Times.SetWindowPos(this,rcUseful.left + dTempInt6,rcUseful.top + 220 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);

	m_edFilter2M.SetWindowPos(this,rcUseful.left + dTempInt5,rcUseful.top + 280 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter2N.SetWindowPos(this,rcUseful.left + dTempInt4,rcUseful.top + 280 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter2Times.SetWindowPos(this,rcUseful.left + dTempInt6,rcUseful.top + 280 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);

	m_edFilter3M.SetWindowPos(this,rcUseful.left + dTempInt5,rcUseful.top + 340 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter3N.SetWindowPos(this,rcUseful.left + dTempInt4,rcUseful.top + 340 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter3Times.SetWindowPos(this,rcUseful.left + dTempInt6,rcUseful.top + 340 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);

	m_edFilter4M.SetWindowPos(this,rcUseful.left + dTempInt5,rcUseful.top + 400 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter4N.SetWindowPos(this,rcUseful.left + dTempInt4,rcUseful.top + 400 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter4Times.SetWindowPos(this,rcUseful.left + dTempInt6,rcUseful.top + 400 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);

	m_edFilter5M.SetWindowPos(this,rcUseful.left + dTempInt5,rcUseful.top + 460 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter5N.SetWindowPos(this,rcUseful.left + dTempInt4,rcUseful.top + 460 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);
	m_edFilter5Times.SetWindowPos(this,rcUseful.left + dTempInt6,rcUseful.top + 460 * m_dVertRatio,iTempInt7,iTempInt8,SWP_NOZORDER);

	m_btDefSet.SetWindowPos(this,rcUseful.left + dTempInt5,rcUseful.top + 545 * m_dVertRatio,90 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
	m_btSave.SetWindowPos(this,rcUseful.left + 570 * m_dHoriRatio,rcUseful.top + 545 * m_dVertRatio,90 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
	m_btCancel.SetWindowPos(this,rcUseful.left + 685 * m_dHoriRatio,rcUseful.top + 545 * m_dVertRatio,90 * m_dHoriRatio,40 * m_dVertRatio,SWP_NOZORDER);
}

void CFilterParamSetDlg::ShowCtrl()
{
	//m_btDefSet.ShowWindow(SW_SHOW);
	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);
	m_ckbtMedian.ShowWindow(SW_SHOW);
	m_ckbtFilter1.ShowWindow(SW_SHOW);
	m_ckbtFilter2.ShowWindow(SW_SHOW);
	m_ckbtFilter3.ShowWindow(SW_SHOW);
	m_ckbtFilter4.ShowWindow(SW_SHOW);
	m_ckbtFilter5.ShowWindow(SW_SHOW);

	m_edMedianWnd.ShowWindow(SW_SHOW);
	m_edMidianTimes.ShowWindow(SW_SHOW);

	m_edFilter1M.ShowWindow(SW_SHOW);
	m_edFilter1N.ShowWindow(SW_SHOW);
	m_edFilter1Times.ShowWindow(SW_SHOW);

	m_edFilter2M.ShowWindow(SW_SHOW);
	m_edFilter2N.ShowWindow(SW_SHOW);
	m_edFilter2Times.ShowWindow(SW_SHOW);

	m_edFilter3M.ShowWindow(SW_SHOW);
	m_edFilter3N.ShowWindow(SW_SHOW);
	m_edFilter3Times.ShowWindow(SW_SHOW);

	m_edFilter4M.ShowWindow(SW_SHOW);
	m_edFilter4N.ShowWindow(SW_SHOW);
	m_edFilter4Times.ShowWindow(SW_SHOW);

	m_edFilter5M.ShowWindow(SW_SHOW);
	m_edFilter5N.ShowWindow(SW_SHOW);
	m_edFilter5Times.ShowWindow(SW_SHOW);
}

void CFilterParamSetDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(NULL == pLangInfo)
	{
		return;
	}

	m_btSave.SetWindowText(pLangInfo->GetText(148));
	m_btCancel.SetWindowText(pLangInfo->GetText(150));

	return;
}

void CFilterParamSetDlg::OnBnClickedDefaultSet()
{
	
	return;
}

void CFilterParamSetDlg::OnBnClickedSave()
{
	GetFilterParam();
	
	OnOK();
	return;
}

void CFilterParamSetDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}

BOOL CFilterParamSetDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

void CFilterParamSetDlg::InitEditCtrlParam(CMyNewEdit* pEdit)
{
	if(NULL == pEdit)
	{
		return;
	}

	pEdit->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEdit->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEdit->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEdit->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	pEdit->SetMyFont(m_stTxFont,18 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
	pEdit->SetTextClr1(RGB(0X56,0X56,0X56));

	return;
}

void CFilterParamSetDlg::InitCheckBt(CMyCheckButton* pCkbt)
{
	if(NULL == pCkbt->GetSafeHwnd())
	{
		return;
	}

	int iFontH = 18 * m_dTxRatio;

	pCkbt->SetPngBkPic(_T(".\\BkPic\\CheckBox.png"),4,20 * m_dHoriRatio);
	pCkbt->SetIconTextInterval(8 * m_dHoriRatio);
	pCkbt->SetTextColor(RGB(0X60,0X62,0X66),CLR_SOFT_THEME);
	pCkbt->SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	pCkbt->SetBeNeedRedrawBk(TRUE);
	pCkbt->SetMyFontChecked(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

int CFilterParamSetDlg::SetFilterParam()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	int iFilterCount = m_pExpData->GetFilterCount();
	if(iFilterCount < 1)
	{
		return -2;
	}

	CString strTemp(_T(""));

	tagFilterItem* pFilterItem = NULL;
	for(int i = 0;i < iFilterCount;++i)
	{
		pFilterItem = m_pExpData->GetFilterBy(i);
		if(NULL == pFilterItem)
		{
			continue;
		}
		if(FILTER_NONE == pFilterItem->nType)
		{
			continue;
		}

		if(FILTER_MEDIAN == pFilterItem->nType)
		{
			m_ckbtMedian.SetBtChecked(0 != pFilterItem->iFilterUser);
			strTemp.Format(_T("%d"),pFilterItem->iPara1);
			m_edMedianWnd.SetText(strTemp);
			strTemp.Format(_T("%d"),pFilterItem->iTimes);
			m_edMidianTimes.SetText(strTemp);
		}
		else
		{
			CMyCheckButton* pCkbt = NULL;
			CMyNewEdit *pEd1 = NULL,*pEd2 = NULL,*pEd3 = NULL;;
			if(1 == i)
			{
				pCkbt = &m_ckbtFilter1;
				pEd1 = &m_edFilter1M;
				pEd2 = &m_edFilter1N;
				pEd3 = &m_edFilter1Times;
			}
			else if(2 == i)
			{
				pCkbt = &m_ckbtFilter2;
				pEd1 = &m_edFilter2M;
				pEd2 = &m_edFilter2N;
				pEd3 = &m_edFilter2Times;
			}
			else if(3 == i)
			{
				pCkbt = &m_ckbtFilter3;
				pEd1 = &m_edFilter3M;
				pEd2 = &m_edFilter3N;
				pEd3 = &m_edFilter3Times;
			}
			else if(4 == i)
			{
				pCkbt = &m_ckbtFilter4;
				pEd1 = &m_edFilter4M;
				pEd2 = &m_edFilter4N;
				pEd3 = &m_edFilter4Times;
			}
			else if(5 == i)
			{
				pCkbt = &m_ckbtFilter5;
				pEd1 = &m_edFilter5M;
				pEd2 = &m_edFilter5N;
				pEd3 = &m_edFilter5Times;
			}

			if(NULL != pCkbt->GetSafeHwnd())
			{
				pCkbt->SetBtChecked(0 != pFilterItem->iFilterUser);
			}
			if(NULL != pEd1->GetSafeHwnd())
			{
				strTemp.Format(_T("%d"),pFilterItem->iPara1);
				pEd1->SetText(strTemp);
			}
			if(NULL != pEd2->GetSafeHwnd())
			{
				strTemp.Format(_T("%d"),pFilterItem->iPara2);
				pEd2->SetText(strTemp);
			}
			if(NULL != pEd3->GetSafeHwnd())
			{
				strTemp.Format(_T("%d"),pFilterItem->iTimes);
				pEd3->SetText(strTemp);
			}
		}
	}

	return 0;
}

int CFilterParamSetDlg::GetFilterParam()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	int iFilterCount = m_pExpData->GetFilterCount();
	if(iFilterCount < 1)
	{
		return -2;
	}

	tagFilterItem* pFilterTotal = new tagFilterItem[6];
	if(NULL == pFilterTotal)
	{
		return -3;
	}

	pFilterTotal[0].iFilterUser = m_ckbtMedian.GetCheckState();
	pFilterTotal[0].nType = FILTER_MEDIAN;
	pFilterTotal[0].iPara1 = _ttoi(m_edMedianWnd.GetText());
	pFilterTotal[0].iTimes = _ttoi(m_edMidianTimes.GetText());

	pFilterTotal[1].iFilterUser = m_ckbtFilter1.GetCheckState();
	pFilterTotal[1].nType = FILTER_FM_BN;
	pFilterTotal[1].iPara1 = _ttoi(m_edFilter1M.GetText());
	pFilterTotal[1].iPara2 = _ttoi(m_edFilter1N.GetText());
	pFilterTotal[1].iTimes = _ttoi(m_edFilter1Times.GetText());

	pFilterTotal[2].iFilterUser = m_ckbtFilter2.GetCheckState();
	pFilterTotal[2].nType = FILTER_FM_BN;
	pFilterTotal[2].iPara1 = _ttoi(m_edFilter2M.GetText());
	pFilterTotal[2].iPara2 = _ttoi(m_edFilter2N.GetText());
	pFilterTotal[2].iTimes = _ttoi(m_edFilter2Times.GetText());

	pFilterTotal[3].iFilterUser = m_ckbtFilter3.GetCheckState();
	pFilterTotal[3].nType = FILTER_FM_BN;
	pFilterTotal[3].iPara1 = _ttoi(m_edFilter3M.GetText());
	pFilterTotal[3].iPara2 = _ttoi(m_edFilter3N.GetText());
	pFilterTotal[3].iTimes = _ttoi(m_edFilter3Times.GetText());

	pFilterTotal[4].iFilterUser = m_ckbtFilter4.GetCheckState();
	pFilterTotal[4].nType = FILTER_FM_BN;
	pFilterTotal[4].iPara1 = _ttoi(m_edFilter4M.GetText());
	pFilterTotal[4].iPara2 = _ttoi(m_edFilter4N.GetText());
	pFilterTotal[4].iTimes = _ttoi(m_edFilter4Times.GetText());

	pFilterTotal[5].iFilterUser = m_ckbtFilter5.GetCheckState();
	pFilterTotal[5].nType = FILTER_FM_BN;
	pFilterTotal[5].iPara1 = _ttoi(m_edFilter5M.GetText());
	pFilterTotal[5].iPara2 = _ttoi(m_edFilter5N.GetText());
	pFilterTotal[5].iTimes = _ttoi(m_edFilter5Times.GetText());

	tagFilterItem* pFilterItem = NULL;
	for(int i = 0;i < iFilterCount;++i)
	{
		pFilterItem = m_pExpData->GetFilterBy(i);
		if(NULL == pFilterItem)
		{
			continue;
		}

		if(0 == i)
		{
			pFilterTotal[0].iPara2 = pFilterItem->iPara2;
		}
		if(BeDifferent(&(pFilterTotal[i]),pFilterItem))
		{
			m_bChanged = TRUE;
			break;
		}
	}

	if(m_bChanged)
	{
		for(int i = 0;i < iFilterCount;++i)
		{
			pFilterItem = m_pExpData->GetFilterBy(i);
			if(NULL == pFilterItem)
			{
				continue;
			}
			*pFilterItem = pFilterTotal[i]; 
		}
	}
	
	delete [] pFilterTotal;

	return 0;
}

BOOL CFilterParamSetDlg::BeDifferent(tagFilterItem* pItemA,tagFilterItem* pItemB)
{
	if(NULL == pItemA || NULL == pItemB)
	{
		return FALSE;
	}

	return pItemA->iFilterUser != pItemB->iFilterUser ||
		   pItemA->nType != pItemB->nType ||
		   pItemA->iTimes != pItemB->iTimes ||
		   pItemA->iPara1 != pItemB->iPara1 ||
		   pItemA->iPara2 != pItemB->iPara2;
}

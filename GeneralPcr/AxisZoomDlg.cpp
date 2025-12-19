// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "AxisZoomDlg.h"
#include "GeneralPcr.h"

// CAxisZoomDlg dialog
IMPLEMENT_DYNAMIC(CAxisZoomDlg, CDialogEx)

CAxisZoomDlg::CAxisZoomDlg(CWnd* pParent /*=NULL*/,BOOL bHaveLogFunc /*= FALSE*/,BOOL bLogMode /*= FALSE*/)
	: CDialogEx(CAxisZoomDlg::IDD, pParent)
{
	m_hWndParent = NULL;
	m_iCrvId = 0;
	m_iAngleRadius = 20;
	m_iFrameWidth = 1;
	m_iWndWidth = 600;
	m_iWndHeight = 456;
	m_iCapHeight = 60;
	m_bChanged = FALSE;

	m_bHaveLogFunc = bHaveLogFunc;
	m_bLogMode = bLogMode;
	m_bLogModeChanged = FALSE;
	m_bNewLogMode = FALSE;
	m_bUseDefFunc = TRUE;
}

CAxisZoomDlg::~CAxisZoomDlg()
{

}

void CAxisZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAxisZoomDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_AXISZOOMDLG_BT_DEFAULTSET,&CAxisZoomDlg::OnBnClickedDefaultSet)
	ON_BN_CLICKED(ID_AXISZOOMDLG_BT_SAVE,&CAxisZoomDlg::OnBnClickedSave)
	ON_BN_CLICKED(ID_AXISZOOMDLG_BT_CANCEL,&CAxisZoomDlg::OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CAxisZoomDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

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
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2 - m_iWndWidth / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCapHeight,rcUseful.Width(),rcUseful.Height() - m_iCapHeight,0,0,m_iAngleRadius,m_iAngleRadius,FALSE,Color(197,197,197),m_iFrameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCapHeight,m_iAngleRadius,m_iAngleRadius,0,0,FALSE,COLOR_SOFT_THEME,m_iFrameWidth,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	DWORD dwTxStyle = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	double dX1 = rcUseful.left + 32 * m_dHoriRatio;
	double dX2 = rcUseful.left + 306 * m_dHoriRatio;
	CRect rcMinTx = CRect(dX1,rcUseful.top + 140 * m_dVertRatio,rcUseful.left + 102 * m_dHoriRatio,rcUseful.top + 167 * m_dVertRatio);
	CRect rcMaxTx = CRect(dX2,rcUseful.top + 140 * m_dVertRatio,rcUseful.left + 376 * m_dHoriRatio,rcUseful.top + 167 * m_dVertRatio);
#ifdef AXISZOOMDLG_MINMAX_INMIDDLE
	dX1 = rcUseful.left + 84 * m_dHoriRatio;
	dX2 = rcUseful.left + 358 * m_dHoriRatio;

	rcMinTx = CRect(dX1,rcUseful.top + 140 * m_dVertRatio,rcUseful.left + 284 * m_dHoriRatio,rcUseful.top + 167 * m_dVertRatio);
	rcMaxTx = CRect(dX2,rcUseful.top + 140 * m_dVertRatio,rcUseful.left + 558 * m_dHoriRatio,rcUseful.top + 167 * m_dVertRatio);
#endif 

	memDc.DrawText(pLangInfo->GetText(101),rcMinTx,dwTxStyle);
	memDc.DrawText(pLangInfo->GetText(102),rcMaxTx,dwTxStyle);

#ifdef AXISZOOMDLG_MINMAX_INMIDDLE
	dX1 = rcUseful.left + 32 * m_dHoriRatio;
	dX2 = rcUseful.left + 306 * m_dHoriRatio;
#endif
	
	memDc.DrawText(pLangInfo->GetText(103),CRect(dX1,rcUseful.top + 199 * m_dVertRatio,rcUseful.left + 72 * m_dHoriRatio,rcUseful.top + 223 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(pLangInfo->GetText(103),CRect(dX2,rcUseful.top + 199 * m_dVertRatio,rcUseful.left + 346 * m_dHoriRatio,rcUseful.top + 223 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.DrawText(pLangInfo->GetText(104),CRect(dX1,rcUseful.top + 255 * m_dVertRatio,rcUseful.left + 72 * m_dHoriRatio,rcUseful.top + 279 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(pLangInfo->GetText(104),CRect(dX2,rcUseful.top + 255 * m_dVertRatio,rcUseful.left + 346 * m_dHoriRatio,rcUseful.top + 279 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(pLangInfo->GetText(293),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);

	CPen pen;
	pen.CreatePen(PS_SOLID,1,RGB(0XD8,0XD8,0XD8));
	CPen* pOldPen = memDc.SelectObject(&pen);
	memDc.MoveTo(rcUseful.left,rcUseful.top + 367 * m_dVertRatio);
	memDc.LineTo(rcUseful.left + m_iWndWidth,rcUseful.top + 367 * m_dVertRatio);
	memDc.SelectObject(pOldPen);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	memDc2.SelectObject(&pOldBitmapCB2);
	bitMapCB2.DeleteObject();
	memDc2.DeleteDC();

	return TRUE;
}

int CAxisZoomDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	m_iCapHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CAxisZoomDlg::OnLButtonDown(UINT nFlags, CPoint point)
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

BOOL CAxisZoomDlg::OnInitDialog()
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

	ShowCtrl();

	return TRUE;
}

void CAxisZoomDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateEd();
	CreateCkbt();
}

void CAxisZoomDlg::CreateBt()
{
	m_btDefSet.Create(_T("默认"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_AXISZOOMDLG_BT_DEFAULTSET);
	m_btSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_AXISZOOMDLG_BT_SAVE);
	m_btCancel.Create(_T("取消"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_AXISZOOMDLG_BT_CANCEL);
}

void CAxisZoomDlg::CreateSt()
{
	
}

void CAxisZoomDlg::CreateEd()
{
	m_edXMin.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_AXISZOOMDLG_ED_XMIN);
	m_edXMax.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_AXISZOOMDLG_ED_XMAX);
	m_edYMin.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_AXISZOOMDLG_ED_YMIN);
	m_edYMax.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0, 0, 0, 0), this,ID_AXISZOOMDLG_ED_YMAX);
}

void CAxisZoomDlg::CreateCkbt()
{
	m_ckbtLogMode.Create(_T("Log"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_AXISZOOMDLG_CKBT_LOGMODE);
}

void CAxisZoomDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CAxisZoomDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitCkbt();
}

void CAxisZoomDlg::InitBt()
{
	int iFontH = 16 * m_dTxRatio;

	m_btDefSet.SetMyFont(m_stTxFont,iFontH);
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

	m_btCancel.SetMyFont(m_stTxFont,iFontH);
	m_btCancel.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CAxisZoomDlg::InitSt()
{
	
}

void CAxisZoomDlg::InitEd()
{
	InitEditCtrlParam(&m_edXMin);
	InitEditCtrlParam(&m_edXMax);
	InitEditCtrlParam(&m_edYMin);
	InitEditCtrlParam(&m_edYMax);

	SetAxisInfo(m_axisInfo,FALSE);
}

void CAxisZoomDlg::InitCkbt()
{
	int iFontH = 20 * m_dTxRatio;

	m_ckbtLogMode.SetBtChecked(m_bLogMode);
	m_ckbtLogMode.SetPngBkPic(_T(".\\BkPic\\CheckBox.png"),4,20 * m_dHoriRatio);
	m_ckbtLogMode.SetBeNeedRedrawBk(TRUE);
	m_ckbtLogMode.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_ckbtLogMode.SetMyFontChecked(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CAxisZoomDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2 - m_iWndWidth / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;
#endif

	int iTempInt1 = 40 * m_dVertRatio;
	int iTempInt2 = 200 * m_dHoriRatio;
	int iTempInt3 = 90 * m_dHoriRatio;
	int iTempInt4 = 392 * m_dVertRatio;
	int iTempInt5 = 32 * m_dHoriRatio;

	m_edXMin.SetWindowPos(this,rcUseful.left + 84 * m_dHoriRatio,rcUseful.top + 191 * m_dVertRatio,iTempInt2,iTempInt1,SWP_NOZORDER);
	m_edXMax.SetWindowPos(this,rcUseful.left + 358 * m_dHoriRatio,rcUseful.top + 191 * m_dVertRatio,iTempInt2,iTempInt1,SWP_NOZORDER);
	m_edYMin.SetWindowPos(this,rcUseful.left + 84 * m_dHoriRatio,rcUseful.top + 247 * m_dVertRatio,iTempInt2,iTempInt1,SWP_NOZORDER);
	m_edYMax.SetWindowPos(this,rcUseful.left + 358 * m_dHoriRatio,rcUseful.top + 247 * m_dVertRatio,iTempInt2,iTempInt1,SWP_NOZORDER);

	m_ckbtLogMode.SetWindowPos(this,rcUseful.left + iTempInt5,rcUseful.top + 320 * m_dVertRatio,iTempInt3,iTempInt1,SWP_NOZORDER);

	m_btDefSet.SetWindowPos(this,rcUseful.left + iTempInt5,rcUseful.top + iTempInt4,iTempInt3,iTempInt1,SWP_NOZORDER);
	m_btSave.SetWindowPos(this,rcUseful.left + 356 * m_dHoriRatio,rcUseful.top + iTempInt4,iTempInt3,iTempInt1,SWP_NOZORDER);
	m_btCancel.SetWindowPos(this,rcUseful.left + 470 * m_dHoriRatio,rcUseful.top + iTempInt4,iTempInt3,iTempInt1,SWP_NOZORDER);
}

void CAxisZoomDlg::ShowCtrl()
{
#ifndef PS96INSTRUMENT
	if(m_bHaveLogFunc)
	{
		m_ckbtLogMode.ShowWindow(SW_SHOW);
	}
#endif

	if(m_bUseDefFunc)
	{
		m_btDefSet.ShowWindow(SW_SHOW);
	}

	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);
	m_edXMin.ShowWindow(SW_SHOW);
	m_edXMax.ShowWindow(SW_SHOW);
	m_edYMin.ShowWindow(SW_SHOW);
	m_edYMax.ShowWindow(SW_SHOW);
}

void CAxisZoomDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_btSave.SetWindowText(pLangInfo->GetText(148));
	m_btCancel.SetWindowText(pLangInfo->GetText(150));
	m_btDefSet.SetWindowText(pLangInfo->GetText(295));

	return;
}

void CAxisZoomDlg::OnBnClickedDefaultSet()
{
	if(m_ckbtLogMode.GetCheckState())
	{
		SetAxisInfo(m_defLinearAxisInfo.CalculateLogValaue(),TRUE);
	}
	else
	{
		SetAxisInfo(m_defLinearAxisInfo,TRUE);
	}

	return;
}

void CAxisZoomDlg::OnBnClickedSave()
{
	CString strTemp(_T(""));

	strTemp = m_edXMin.GetText();
	m_newAxisInfo.m_dXMin = _ttof(strTemp);
	strTemp = m_edXMax.GetText();
	m_newAxisInfo.m_dXMax = _ttof(strTemp);
	strTemp = m_edYMin.GetText();
	m_newAxisInfo.m_dYMin = _ttof(strTemp);
	strTemp = m_edYMax.GetText();
	m_newAxisInfo.m_dYMax = _ttof(strTemp);

	if(m_newAxisInfo.m_dXMin >= m_newAxisInfo.m_dXMax || m_newAxisInfo.m_dYMin >= m_newAxisInfo.m_dYMax)
	{
		strTemp = theApp.m_pLangInfo->GetText(162);
		PopWarningTipInfo(strTemp);
		return;
	}

	if(m_newAxisInfo.m_dYMax > 65535)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(259));
		return;
	}

	m_bChanged = !(m_axisInfo == m_newAxisInfo);

	m_bNewLogMode = m_ckbtLogMode.GetCheckState();
	m_bLogModeChanged = (m_bLogMode != m_bNewLogMode);

	OnOK();
	return;
}

void CAxisZoomDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}

void CAxisZoomDlg::SetAxisInfo(CAxisInfo axisInfo,BOOL bRedraw)
{
	CString strTemp(_T(""));
	strTemp.Format(_T("%.2lf"),axisInfo.m_dXMin);
	m_edXMin.SetText(strTemp,bRedraw);
	strTemp.Format(_T("%.2lf"),axisInfo.m_dXMax);
	m_edXMax.SetText(strTemp,bRedraw);
	strTemp.Format(_T("%.2lf"),axisInfo.m_dYMin);
	m_edYMin.SetText(strTemp,bRedraw);
	strTemp.Format(_T("%.2lf"),axisInfo.m_dYMax);
	m_edYMax.SetText(strTemp,bRedraw);
}

BOOL CAxisZoomDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

void CAxisZoomDlg::InitEditCtrlParam(CMyNewEdit* pEdit)
{
	if(NULL == pEdit)
	{
		return;
	}

	pEdit->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEdit->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEdit->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEdit->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	pEdit->SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
	pEdit->SetTextClr1(RGB(0X56,0X56,0X56));

	return;
}

BOOL CAxisZoomDlg::GetNewLogMode()
{
	return m_bNewLogMode;
}

void CAxisZoomDlg::SetUseDefaultFuncState(BOOL bUseState)
{
	m_bUseDefFunc = bUseState;
}

BOOL CAxisZoomDlg::GetUseDefaultFuncState()
{
	return m_bUseDefFunc;
}





// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SystemSetDlg.h"
#include "GeneralPcr.h"

// CSystemSetDlg dialog
IMPLEMENT_DYNAMIC(CSystemSetDlg,CDialogEx)

CSystemSetDlg::CSystemSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemSetDlg::IDD, pParent)
{
	m_hWndParent = NULL;
	m_iAngleRadius = 20;
	m_iFrameW = 1;
	m_iWndW = 1600;
	m_iWndH = 920;
	m_iCapH = 60;
	m_bChanged = FALSE;
}

CSystemSetDlg::~CSystemSetDlg()
{

}

void CSystemSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSystemSetDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_SYSTEMSETDLG_BT_DEFAULTSET,OnBnClickedDefaultSet)
	ON_BN_CLICKED(ID_SYSTEMSETDLG_BT_SAVE,OnBnClickedSave)
	ON_BN_CLICKED(ID_SYSTEMSETDLG_BT_CANCEL,OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CSystemSetDlg::OnEraseBkgnd(CDC* pDC)
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
	rcUseful.left = rect.left + (rect.Width() - m_iWndW) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndH) / 2;
	rcUseful.right = rcUseful.left + m_iWndW;
	rcUseful.bottom = rcUseful.top + m_iWndH;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCapH,rcUseful.Width(),rcUseful.Height() - m_iCapH,0,0,m_iAngleRadius,m_iAngleRadius,FALSE,Color(197,197,197),m_iFrameW,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCapH,m_iAngleRadius,m_iAngleRadius,0,0,FALSE,COLOR_SOFT_THEME,2,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	int iX = rcUseful.left + 30 * m_dHoriRatio,iY = rcUseful.top + 80 * m_dVertRatio;
	int iW = 200 * m_dHoriRatio,iH = 30 * m_dVertRatio;

	memDc.FillSolidRect(iX,rcUseful.top + 93 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.FillSolidRect(iX + 4 * m_dHoriRatio,rcUseful.top + 89 * m_dVertRatio,4 * m_dVertRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	iX += 14 * m_dHoriRatio;
	memDc.DrawText(pLangInfo->GetText(433),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	iX = rcUseful.left + m_iWndW - 700 * m_dHoriRatio;
	memDc.FillSolidRect(iX,rcUseful.top + 93 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.FillSolidRect(iX + 4 * m_dHoriRatio,rcUseful.top + 89 * m_dVertRatio,4 * m_dVertRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	iX += 14 * m_dHoriRatio;
	memDc.DrawText(pLangInfo->GetText(435),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	iX = rcUseful.left + 30 * m_dHoriRatio;
	iY += 250 * m_dVertRatio;
	memDc.FillSolidRect(iX,iY + 13 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.FillSolidRect(iX + 4 * m_dHoriRatio,iY + 9 * m_dVertRatio,4 * m_dVertRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	iX += 14 * m_dHoriRatio;
	memDc.DrawText(pLangInfo->GetText(121),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	iY += 260 * m_dVertRatio;
	memDc.FillSolidRect(iX,iY + 13 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.FillSolidRect(iX + 4 * m_dHoriRatio,iY + 9 * m_dVertRatio,4 * m_dVertRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	iX += 14 * m_dHoriRatio;
	memDc.DrawText(pLangInfo->GetText(434),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	int iStartX = rcUseful.left + m_iWndW - 700 * m_dHoriRatio,iStartY = rcUseful.top + m_iWndH - 500 * m_dVertRatio;
	iX = iStartX;
	iY = iStartY - 30 * m_dVertRatio;
	memDc.FillSolidRect(iX,iY + 13 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDc.FillSolidRect(iX + 4 * m_dHoriRatio,iY + 9 * m_dVertRatio,4 * m_dVertRatio,12 * m_dVertRatio,CLR_SOFT_THEME);
	iX += 14 * m_dHoriRatio;
	memDc.DrawText(pLangInfo->GetText(177),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	iW = 75 * m_dHoriRatio,iH = 40 * m_dVertRatio;
	iX = iStartX + 198 * m_dHoriRatio;
	iY = iStartY;

	memDc.DrawText(_T("nWnd"),CRect(iX,iY,iX + iW,iY + iH),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	iX += 200 * m_dHoriRatio;
	memDc.DrawText(_T("Times"),CRect(iX,iY,iX + iW,iY + iH),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	iX = iStartX + 80 * m_dHoriRatio;
	iY = iStartY + 60 * m_dVertRatio;
	iW = 100 * m_dHoriRatio;
	memDc.DrawText(_T("Forward-M"),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iX += 200 * m_dHoriRatio;
	memDc.DrawText(_T("Backward-N"),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	iX += 200 * m_dHoriRatio;
	memDc.DrawText(_T("Times"),CRect(iX,iY,iX + iW,iY + iH),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(pLangInfo->GetText(432),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapH - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);
	Node<CRect>* pNodeRst = m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	memDc2.SelectObject(&pOldBitmapCB2);
	bitMapCB2.DeleteObject();
	memDc2.DeleteDC();

	return TRUE;
}

int CSystemSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndW *= m_dHoriRatio;
	m_iWndH *= m_dVertRatio;
	m_iCapH *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CSystemSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < m_iCapH)
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

BOOL CSystemSetDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0,0,m_iWndW,m_iWndH,SWP_NOZORDER);
	SetRoundRect(m_iAngleRadius,m_iAngleRadius);
	CenterWindow(GetParent());
#endif

	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	SetFilterParam();
	SetCrossSetParam();
	SetMeltCrossSetParam();
	SetLstAmlAnaBaseParam();
	SetLstMeltParam();

	ShowCtrl();

	return TRUE;
}

void CSystemSetDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateEd();
	CreateCkbt();
	CreateLst();
}

void CSystemSetDlg::CreateBt()
{
	m_btDefSet.Create(_T("默认"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_BT_DEFAULTSET);
	m_btSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_BT_SAVE);
	m_btCancel.Create(_T("取消"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_BT_CANCEL);
}

void CSystemSetDlg::CreateCkbt()
{
	m_ckbtMedian.Create(_T("Median"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_CKBT_MEDIAN);
	m_ckbtFilter1.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_CKBT_FILTER1);
	m_ckbtFilter2.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_CKBT_FILTER2);
	m_ckbtFilter3.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_CKBT_FILTER3);
	m_ckbtFilter4.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_CKBT_FILTER4);
	m_ckbtFilter5.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SYSTEMSETDLG_CKBT_FILTER5);
}

void CSystemSetDlg::CreateSt()
{
	
}

void CSystemSetDlg::CreateEd()
{
	CRect rect(0,0,0,0);

	m_edMedianWnd.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_MEDIANWND);
	m_edMidianTimes.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_MEDIANWND);

	m_edFilter1M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER1M);
	m_edFilter1N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER1N);
	m_edFilter1Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER1TIMES);

	m_edFilter2M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER2M);
	m_edFilter2N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER2N);
	m_edFilter2Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER2TIMES);

	m_edFilter3M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER3M);
	m_edFilter3N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER3N);
	m_edFilter3Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER3TIMES);

	m_edFilter4M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER4M);
	m_edFilter4N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER4N);
	m_edFilter4Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER4TIMES);

	m_edFilter5M.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER5M);
	m_edFilter5N.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER5N);
	m_edFilter5Times.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rect,this,ID_SYSTEMSETDLG_ED_FILTER5TIMES);

	return;
}

void CSystemSetDlg::CreateLst()
{
	m_lstAmlParam.Create(IDD_MIDDLEDLG,this);
	m_lstCrossSet.Create(IDD_MIDDLEDLG,this);
	m_lstMeltParam.Create(IDD_MIDDLEDLG,this);
	m_lstMeltCrossSet.Create(IDD_MIDDLEDLG,this);
}

void CSystemSetDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,18 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CSystemSetDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitCkbt();
	InitLst();
}

void CSystemSetDlg::InitBt()
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

void CSystemSetDlg::InitCkbt()
{
	InitCheckBt(&m_ckbtMedian);
	InitCheckBt(&m_ckbtFilter1);
	InitCheckBt(&m_ckbtFilter2);
	InitCheckBt(&m_ckbtFilter3);
	InitCheckBt(&m_ckbtFilter4);
	InitCheckBt(&m_ckbtFilter5);
}

void CSystemSetDlg::InitSt()
{
	
}

void CSystemSetDlg::InitEd()
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

void CSystemSetDlg::InitLst()
{
	InitLstAmlAnaParam();
	InitCrossSetAnaParam();
	InitLstMeltParam();
	InitLstMeltCrossSet();

	return;
}

void CSystemSetDlg::InitLstAmlAnaParam()
{
	int iFontH = 15 * m_dTxRatio;
	int iFontH2 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 16 * m_dTxRatio;
		iFontH2 = 17 * m_dTxRatio;
	}

	m_lstAmlParam.SetHeaderHeight(28 * m_dVertRatio);
	m_lstAmlParam.SetMyHeaderFont(m_stTxFont,iFontH);
	m_lstAmlParam.SetHeaderBkColor(Color(0XF1,0XF4,0XFa));
	m_lstAmlParam.SetHeaderTextColor(RGB(0X39,0X45,0X65));
	m_lstAmlParam.SetMyTitleFont(m_stTxFont,iFontH,FW_BOLD);
	m_lstAmlParam.SetItemHeight(30 * m_dVertRatio);

	int iTempInt1 = 34 * m_dVertRatio;
	m_lstAmlParam.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstAmlParam.m_horiScroll.m_scrollH = iTempInt1;
	m_lstAmlParam.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstAmlParam.m_vertScroll.m_scrollW = iTempInt1;
	m_lstAmlParam.SetMyFont(m_stTxFont,iFontH2);
	m_lstAmlParam.SetTextClr1(RGB(0X39,0X45,0X65));
	m_lstAmlParam.SetBeShowRowSelectState(FALSE);

	m_lstAmlParam.SetFuncEditCtrlFont(m_stTxFont,iFontH2);

	m_lstAmlParam.InsertColumn(0,theApp.m_pLangInfo->GetText(111));
	m_lstAmlParam.InsertColumn(1,theApp.m_pLangInfo->GetText(112));
	m_lstAmlParam.InsertColumn(2,theApp.m_pLangInfo->GetText(294));
	m_lstAmlParam.InsertColumn(3,theApp.m_pLangInfo->GetText(114));
	m_lstAmlParam.InsertColumn(4,theApp.m_pLangInfo->GetText(115));
	m_lstAmlParam.InsertColumn(5,theApp.m_pLangInfo->GetText(117));
	m_lstAmlParam.InsertColumn(6,theApp.m_pLangInfo->GetText(118));
	m_lstAmlParam.InsertColumn(7,theApp.m_pLangInfo->GetText(119));

	CLinkList<CString> lnkCbStr;

	m_lstAmlParam.SetColumnEditable(3,TRUE);
	m_lstAmlParam.SetColumnEditable(4,TRUE);
	m_lstAmlParam.SetColumnEditable(6,TRUE);
	m_lstAmlParam.SetColumnEditable(7,TRUE);

	m_lstAmlParam.SetColumnCheckFunc(2,TRUE);
	m_lstAmlParam.SetColumnCheckFunc(5,TRUE);

	int width = 809 * m_dHoriRatio;
	int tempInt = 52 * m_dHoriRatio;

	m_lstAmlParam.SetColumnWidth(0,tempInt);
	width -= tempInt;
	tempInt = 72 * m_dHoriRatio;
	m_lstAmlParam.SetColumnWidth(1,tempInt);
	width -= tempInt;
	tempInt = 100 * m_dHoriRatio;
	m_lstAmlParam.SetColumnWidth(2,tempInt);
	width -= tempInt;
	tempInt = 100 * m_dHoriRatio;
	m_lstAmlParam.SetColumnWidth(3,tempInt);
	width -= tempInt;
	tempInt = 100 * m_dHoriRatio;
	m_lstAmlParam.SetColumnWidth(4,tempInt);
	width -= tempInt;
	tempInt = 100 * m_dHoriRatio;
	m_lstAmlParam.SetColumnWidth(5,tempInt);
	width -= tempInt;
	tempInt = 142 * m_dHoriRatio;
	m_lstAmlParam.SetColumnWidth(6,tempInt);
	width -= tempInt;
	width -= m_lstAmlParam.GetFrameLineWidth() * 2;
	m_lstAmlParam.SetColumnWidth(7,width);

	CLinkList<int> lnkControledCol;
	int colId = 3;
	lnkControledCol.AddTail(colId);
	colId = 4;
	lnkControledCol.AddTail(colId);
	m_lstAmlParam.SetMapColumnDisableControl(2,lnkControledCol);

	lnkControledCol.Clear();
	colId = 6;
	lnkControledCol.AddTail(colId);
	colId = 7;
	lnkControledCol.AddTail(colId);
	m_lstAmlParam.SetMapColumnDisableControl(5,lnkControledCol);

	m_lstAmlParam.AddPairToMapDisabledStateKeyString(_T("FALSE"),FALSE);
	m_lstAmlParam.AddPairToMapDisabledStateKeyString(_T("TRUE"),TRUE);

	CMaxMinInfo maxMinInfo;
	maxMinInfo.SetBeOnlyIntergerNumber(TRUE);
	maxMinInfo.SetBeSetMinValue(TRUE);
	maxMinInfo.SetMinValue(1);
	maxMinInfo.SetBeSetMaxValue(TRUE);
	maxMinInfo.SetMaxValue(100);
	maxMinInfo.SetBeMaxMinSimultaneousEffect(TRUE);
	m_lstAmlParam.SetColumnMaxMinInfo(3,maxMinInfo);
	m_lstAmlParam.SetColumnMaxMinInfo(4,maxMinInfo);

	maxMinInfo.ClearMaxMinLimit();
	maxMinInfo.SetBeOnlyFloatNumber(TRUE);
	maxMinInfo.SetBeAllowNegtiveNumber(TRUE);
	m_lstAmlParam.SetColumnMaxMinInfo(7,maxMinInfo);

	return;
}

void CSystemSetDlg::InitCrossSetAnaParam()
{
	int iFontH = 15 * m_dTxRatio;
	int iFontH2 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
		iFontH2 = 17 * m_dTxRatio;
	}

	m_lstCrossSet.SetHeaderHeight(28 * m_dVertRatio);
	m_lstCrossSet.SetMyHeaderFont(m_stTxFont,iFontH);
	m_lstCrossSet.SetHeaderBkColor(RGB(228,231,236));
	m_lstCrossSet.SetHeaderTextColor(RGB(0X59,0X59,0X59));
	m_lstCrossSet.SetMyTitleFont(m_stTxFont,iFontH,FW_BOLD);
	m_lstCrossSet.SetItemHeight(30 * m_dVertRatio);
	m_lstCrossSet.SetBeShowRowSelectState(FALSE);

	m_lstCrossSet.m_horiScroll.m_arrowWH = 28 * m_dHoriRatio;
	m_lstCrossSet.m_horiScroll.m_scrollH = 28 * m_dVertRatio;
	m_lstCrossSet.m_vertScroll.m_arrowWH = 28 * m_dHoriRatio;
	m_lstCrossSet.m_vertScroll.m_scrollW = 28 * m_dVertRatio;
	m_lstCrossSet.SetMyFont(m_stTxFont,iFontH2);
	m_lstCrossSet.SetTextClr1(RGB(0X39,0X45,0X65));

	m_lstCrossSet.SetFuncEditCtrlFont(m_stTxFont,iFontH2);

	return;
}

void CSystemSetDlg::InitLstMeltParam()
{
	int iFtH = 15 * m_dTxRatio;
	int iFtH2 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFtH = 17 * m_dTxRatio;
		iFtH2 = 17 * m_dTxRatio;
	}

	m_lstMeltParam.SetHeaderHeight(24 * m_dVertRatio);
	m_lstMeltParam.SetMyHeaderFont(m_stTxFont,iFtH);
	m_lstMeltParam.SetMyFont(m_stTxFont,iFtH2);
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

	m_lstMeltParam.InsertColumn(0,theApp.m_pLangInfo->GetText(111));
	m_lstMeltParam.InsertColumn(1,theApp.m_pLangInfo->GetText(112));
	m_lstMeltParam.InsertColumn(2,theApp.m_pLangInfo->GetText(122));
	m_lstMeltParam.InsertColumn(3,theApp.m_pLangInfo->GetText(123));
	m_lstMeltParam.InsertColumn(4,theApp.m_pLangInfo->GetText(124));

	m_lstMeltParam.SetColumnEditable(2,TRUE);
	m_lstMeltParam.SetColumnEditable(3,TRUE);
	m_lstMeltParam.SetColumnEditable(4,TRUE);

	int width = 809 * m_dHoriRatio;
	int tempInt = 46 * m_dHoriRatio;

	m_lstMeltParam.SetColumnWidth(0,tempInt);
	width -= tempInt;
	tempInt = 70 * m_dHoriRatio;
	m_lstMeltParam.SetColumnWidth(1,tempInt);
	width -= tempInt;
	tempInt = 245 * m_dHoriRatio;
	m_lstMeltParam.SetColumnWidth(2,tempInt);
	width -= tempInt;
	tempInt = 245 * m_dHoriRatio;
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

void CSystemSetDlg::InitLstMeltCrossSet()
{
	int iFtH = 15 * m_dTxRatio;
	int iFtH2 = 16 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFtH = 17 * m_dTxRatio;
		iFtH2 = 17 * m_dTxRatio;
	}

	m_lstMeltCrossSet.SetHeaderHeight(28 * m_dVertRatio);
	m_lstMeltCrossSet.SetMyHeaderFont(m_stTxFont,iFtH);
	m_lstMeltCrossSet.SetHeaderBkColor(RGB(228,231,236));
	m_lstMeltCrossSet.SetHeaderTextColor(RGB(0X59,0X59,0X59));
	m_lstMeltCrossSet.SetMyTitleFont(m_stTxFont,iFtH,FW_BOLD);
	m_lstMeltCrossSet.SetItemHeight(30 * m_dVertRatio);
	m_lstMeltCrossSet.SetBeShowRowSelectState(FALSE);

	m_lstMeltCrossSet.m_horiScroll.m_arrowWH = 28 * m_dHoriRatio;
	m_lstMeltCrossSet.m_horiScroll.m_scrollH = 28 * m_dVertRatio;
	m_lstMeltCrossSet.m_vertScroll.m_arrowWH = 28 * m_dHoriRatio;
	m_lstMeltCrossSet.m_vertScroll.m_scrollW = 28 * m_dVertRatio;
	m_lstMeltCrossSet.SetMyFont(m_stTxFont,iFtH2);
	m_lstMeltCrossSet.SetTextClr1(RGB(0X39,0X45,0X65));

	m_lstMeltCrossSet.SetFuncEditCtrlFont(m_stTxFont,iFtH2);

	return;
}

void CSystemSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcInit = rect;
	ClearRectToDelete();

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWndW) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndH) / 2;
	rcUseful.right = rcUseful.left + m_iWndW;
	rcUseful.bottom = rcUseful.top + m_iWndH;
#endif

	int iY = rcUseful.top + 110 * m_dVertRatio;

	//扩增分析参数
	m_lstAmlParam.MoveWindow(rcUseful.left + 30 * m_dHoriRatio,iY,809 * m_dHoriRatio,206 * m_dVertRatio,FALSE);
	AddRect(rcUseful.left + 30 * m_dHoriRatio,iY,809 * m_dHoriRatio,206 * m_dVertRatio);

	//串扰系数
	m_lstCrossSet.MoveWindow(rcUseful.left + m_iWndW - 700 * m_dHoriRatio,iY,660 * m_dHoriRatio,210 * m_dVertRatio,FALSE);
	AddRect(rcUseful.left + m_iWndW - 700 * m_dHoriRatio,iY,660 * m_dHoriRatio,210 * m_dVertRatio);

	//熔曲分析参数
	iY += 250 * m_dVertRatio;
	m_lstMeltParam.MoveWindow(rcUseful.left + 30 * m_dHoriRatio,iY,809 * m_dHoriRatio,220 * m_dVertRatio,FALSE);
	AddRect(rcUseful.left + 30 * m_dHoriRatio,iY,809 * m_dHoriRatio,220 * m_dVertRatio);

	iY += 260 * m_dVertRatio;
	m_lstMeltCrossSet.MoveWindow(rcUseful.left + 30 * m_dHoriRatio,iY,809 * m_dHoriRatio,220 * m_dVertRatio,FALSE);
	AddRect(rcUseful.left + 30 * m_dHoriRatio,iY,809 * m_dHoriRatio,220 * m_dVertRatio);

	//滤波参数
	int iRgnW = 700 * m_dHoriRatio,iRgnH = 500 * m_dVertRatio;
	int iStartX = rcUseful.left + m_iWndW - iRgnW,iStartY = rcUseful.top + m_iWndH - iRgnH;
	int iX = 0,iW = 90 * m_dHoriRatio,iH = 20 * m_dVertRatio,iVertItl = 60 * m_dVertRatio;
	iX = iStartX;
	iY = iStartY + 10 * m_dVertRatio;
	m_ckbtMedian.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += 100 * m_dVertRatio;
	m_ckbtFilter1.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_ckbtFilter2.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_ckbtFilter3.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_ckbtFilter4.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_ckbtFilter5.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);

	iX += 280 * m_dHoriRatio;
	iY = iStartY;
	iW = 100 * m_dHoriRatio;
	iH = 40 * m_dVertRatio;
	m_edMedianWnd.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iX += 200 * m_dHoriRatio;
	m_edMidianTimes.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);

	iX = iStartX + 80 * m_dHoriRatio;
	iY = iStartY + 100 * m_dVertRatio;
	iW = 100 * m_dHoriRatio;
	m_edFilter1M.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter2M.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter3M.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter4M.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter5M.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);

	iX += 200 * m_dHoriRatio;
	iY = iStartY + 100 * m_dVertRatio;
	m_edFilter1N.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter2N.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter3N.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter4N.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter5N.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;

	iX += 200 * m_dHoriRatio;
	iY = iStartY + 100 * m_dVertRatio;
	m_edFilter1Times.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter2Times.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter3Times.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter4Times.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);
	iY += iVertItl;
	m_edFilter5Times.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);

	int iBtW = 90 * m_dHoriRatio,iBtH = 40 * m_dVertRatio;
	iX = iStartX + iRgnW - iBtW * 3 - 60 * m_dHoriRatio;
	iY = iStartY + iRgnH - iBtH - 20 * m_dVertRatio;
	m_btDefSet.MoveWindow(iX,iY,iBtW,iBtH,FALSE);

	iX = rcUseful.left + m_iWndW - iBtW * 2 - 40 * m_dHoriRatio;
	m_btSave.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRect(iX,iY,iBtW,iBtH);

	iX += (iBtW + 20 * m_dHoriRatio);
	m_btCancel.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRect(iX,iY,iBtW,iBtH);

	CalcRectResult();

	return;
}

void CSystemSetDlg::ShowCtrl()
{
	m_lstAmlParam.ShowWindow(SW_SHOW);
	m_lstCrossSet.ShowWindow(SW_SHOW);
	m_lstMeltParam.ShowWindow(SW_SHOW);
	m_lstMeltCrossSet.ShowWindow(SW_SHOW);

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

void CSystemSetDlg::SetCtrlText()
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

void CSystemSetDlg::OnBnClickedDefaultSet()
{
	
	return;
}

void CSystemSetDlg::OnBnClickedSave()
{
	GetLstAmlAnaBaseParam();
	GetLstMeltParam();
	GetFilterParam();
	GetCrossSetParam();
	GetMeltCrossSetParam();
	
	OnOK();
	return;
}

void CSystemSetDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}

BOOL CSystemSetDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

void CSystemSetDlg::InitEditCtrlParam(CMyNewEdit* pEdit)
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

void CSystemSetDlg::InitCheckBt(CMyCheckButton* pCkbt)
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

int CSystemSetDlg::SetLstAmlAnaBaseParam()
{
	m_lstAmlParam.Clear();

	CString strTemp(_T(""));
	int iAmpParamCount = theApp.m_vecAmplifyPara.size();

	//PS96不显示校准通道分析参数信息
#ifdef PS96INSTRUMENT
	--iAmpParamCount;
#endif

	for(int i = 0;i < iAmpParamCount;++i)
	{
		tagAnaAmplifyPara& anaAmp = theApp.m_vecAmplifyPara[i];
		
		m_lstAmlParam.InsertRowSetHeight(i,24 * m_dVertRatio);
		strTemp.Format(_T("%d"),i + 1);
		m_lstAmlParam.SetItemText(i,0,strTemp);

		m_lstAmlParam.SetItemText(i,1,anaAmp.strDye);

		m_lstAmlParam.SetCellCheckState(i,2,anaAmp.bAutoBaseline);

		strTemp.Format(_T("%d"),anaAmp.iBaselineStart);
		m_lstAmlParam.SetItemText(i,3,strTemp);
		strTemp.Format(_T("%d"),anaAmp.iBaselineEnd);
		m_lstAmlParam.SetItemText(i,4,strTemp);

		m_lstAmlParam.SetCellCheckState(i,5,SPLINE_THRESHOLD_AUTO == anaAmp.nThresholdType);
		strTemp.Format(_T("%.2lf"),anaAmp.dNormalizeThreshold);
		m_lstAmlParam.SetItemText(i,6,strTemp);
		strTemp.Format(_T("%.2lf"),anaAmp.dDeltaRnThreshold);
		m_lstAmlParam.SetItemText(i,7,strTemp);
	}

	m_lstAmlParam.BottomBlankSetAutoFitHeight();

	m_lstAmlParam.Invalidate(FALSE);

	return 0;
}

int CSystemSetDlg::GetLstAmlAnaBaseParam()
{
	int iRowNum = m_lstAmlParam.GetRowNum();
	if(iRowNum < 1)
	{
		return -2;
	}

	tagAnaAmplifyPara anaAml;

	CString strTemp(_T(""));

	for(int i = 0;i < iRowNum;++i)
	{
		anaAml = theApp.m_vecAmplifyPara[i];

		anaAml.strDye = m_lstAmlParam.GetItemText(i,1);

		BOOL bChecked = FALSE;
		m_lstAmlParam.GetCellCheckState(i,2,anaAml.bAutoBaseline);

		anaAml.iBaselineStart = _ttoi(m_lstAmlParam.GetItemText(i,3));
		anaAml.iBaselineEnd = _ttoi(m_lstAmlParam.GetItemText(i,4));

		bChecked = FALSE;
		m_lstAmlParam.GetCellCheckState(i,5,bChecked);
		if(bChecked)
		{
			anaAml.nThresholdType = SPLINE_THRESHOLD_AUTO;
		}
		else
		{
			anaAml.nThresholdType = SPLINE_THRESHOLD_NORMALIZE;
		}

		anaAml.dNormalizeThreshold = _ttof(m_lstAmlParam.GetItemText(i,6));
		anaAml.dDeltaRnThreshold = _ttof(m_lstAmlParam.GetItemText(i,7));

		theApp.m_vecAmplifyPara[i] = anaAml;
	}

	return 0;
}

int CSystemSetDlg::SetLstMeltParam()
{
	m_lstMeltParam.Clear();
	
	CString strTemp(_T(""));
	int iMeltParamCount = theApp.m_vecMeltCurvePara.size();

	//PS96不显示校准通道分析参数信息
#ifdef PS96INSTRUMENT
	--iMeltParamCount;
#endif

	for(int i = 0;i < iMeltParamCount;++i)
	{
		tagAnaMeltCurvePara& anaMelt = theApp.m_vecMeltCurvePara[i]; 
		m_lstMeltParam.InsertRow(i);

		strTemp.Format(_T("%d"),i + 1);
		m_lstMeltParam.SetItemText(i,0,strTemp);

		m_lstMeltParam.SetItemText(i,1,anaMelt.strDye);

		strTemp.Format(_T("%.2lf"),anaMelt.dMinTemp);
		m_lstMeltParam.SetItemText(i,2,strTemp);
		strTemp.Format(_T("%.2lf"),anaMelt.dMaxTemp);
		m_lstMeltParam.SetItemText(i,3,strTemp);
		strTemp.Format(_T("%.2lf"),anaMelt.dThreshold);
		m_lstMeltParam.SetItemText(i,4,strTemp);
	}

	m_lstMeltParam.BottomBlankSetAutoFitHeight();
	m_lstMeltParam.Invalidate(FALSE);

	return 0;
}

int CSystemSetDlg::GetLstMeltParam()
{
	int iRowNum = m_lstMeltParam.GetRowNum();
	for(int i = 0;i < iRowNum;++i)
	{
		tagAnaMeltCurvePara anaMelt = theApp.m_vecMeltCurvePara[i];

		anaMelt.strDye = m_lstMeltParam.GetItemText(i,1);
		anaMelt.dMinTemp = _ttof(m_lstMeltParam.GetItemText(i,2));
		anaMelt.dMaxTemp = _ttof(m_lstMeltParam.GetItemText(i,3));
		anaMelt.dThreshold = _ttof(m_lstMeltParam.GetItemText(i,4));

		theApp.m_vecMeltCurvePara[i] = anaMelt;
	}

	return 0;
}

int CSystemSetDlg::SetFilterParam()
{
	int iFilterCount = theApp.m_vecFlt.size();
	if(iFilterCount < 1)
	{
		return -2;
	}

	CString strTemp(_T(""));

	for(int i = 0;i < iFilterCount;++i)
	{
		tagFilterItem& fltItem = theApp.m_vecFlt[i];
		if(FILTER_NONE == fltItem.nType)
		{
			continue;
		}

		if(FILTER_MEDIAN == fltItem.nType)
		{
			m_ckbtMedian.SetBtChecked(0 != fltItem.iFilterUser);
			strTemp.Format(_T("%d"),fltItem.iPara1);
			m_edMedianWnd.SetText(strTemp);
			strTemp.Format(_T("%d"),fltItem.iTimes);
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
				pCkbt->SetBtChecked(0 != fltItem.iFilterUser);
			}
			if(NULL != pEd1->GetSafeHwnd())
			{
				strTemp.Format(_T("%d"),fltItem.iPara1);
				pEd1->SetText(strTemp);
			}
			if(NULL != pEd2->GetSafeHwnd())
			{
				strTemp.Format(_T("%d"),fltItem.iPara2);
				pEd2->SetText(strTemp);
			}
			if(NULL != pEd3->GetSafeHwnd())
			{
				strTemp.Format(_T("%d"),fltItem.iTimes);
				pEd3->SetText(strTemp);
			}
		}
	}

	return 0;
}

int CSystemSetDlg::GetFilterParam()
{
	theApp.m_vecFlt.clear();

	tagFilterItem fltItem;

	fltItem.iFilterUser = m_ckbtMedian.GetCheckState();
	fltItem.nType = FILTER_MEDIAN;
	fltItem.iPara1 = _ttoi(m_edMedianWnd.GetText());
	fltItem.iTimes = _ttoi(m_edMidianTimes.GetText());
	theApp.m_vecFlt.push_back(fltItem);

	fltItem.iFilterUser = m_ckbtFilter1.GetCheckState();
	fltItem.nType = FILTER_FM_BN;
	fltItem.iPara1 = _ttoi(m_edFilter1M.GetText());
	fltItem.iPara2 = _ttoi(m_edFilter1N.GetText());
	fltItem.iTimes = _ttoi(m_edFilter1Times.GetText());
	theApp.m_vecFlt.push_back(fltItem);

	fltItem.iFilterUser = m_ckbtFilter2.GetCheckState();
	fltItem.nType = FILTER_FM_BN;
	fltItem.iPara1 = _ttoi(m_edFilter2M.GetText());
	fltItem.iPara2 = _ttoi(m_edFilter2N.GetText());
	fltItem.iTimes = _ttoi(m_edFilter2Times.GetText());
	theApp.m_vecFlt.push_back(fltItem);

	fltItem.iFilterUser = m_ckbtFilter3.GetCheckState();
	fltItem.nType = FILTER_FM_BN;
	fltItem.iPara1 = _ttoi(m_edFilter3M.GetText());
	fltItem.iPara2 = _ttoi(m_edFilter3N.GetText());
	fltItem.iTimes = _ttoi(m_edFilter3Times.GetText());
	theApp.m_vecFlt.push_back(fltItem);

	fltItem.iFilterUser = m_ckbtFilter4.GetCheckState();
	fltItem.nType = FILTER_FM_BN;
	fltItem.iPara1 = _ttoi(m_edFilter4M.GetText());
	fltItem.iPara2 = _ttoi(m_edFilter4N.GetText());
	fltItem.iTimes = _ttoi(m_edFilter4Times.GetText());
	theApp.m_vecFlt.push_back(fltItem);

	fltItem.iFilterUser = m_ckbtFilter5.GetCheckState();
	fltItem.nType = FILTER_FM_BN;
	fltItem.iPara1 = _ttoi(m_edFilter5M.GetText());
	fltItem.iPara2 = _ttoi(m_edFilter5N.GetText());
	fltItem.iTimes = _ttoi(m_edFilter5Times.GetText());
	theApp.m_vecFlt.push_back(fltItem);

	return 0;
}

int CSystemSetDlg::SetCrossSetParam()
{
	float** ppCrosstalkPara = theApp.m_ppFCrosstalkPara;
	if(NULL == ppCrosstalkPara)
	{
		return -2;
	}

	int iChanCount = theApp.m_iDefChanNum;

	//PS96不显示校准通道的串扰信息
#ifdef PS96INSTRUMENT
	--iChanCount;
#endif

	if(iChanCount < 1)
	{
		return -3;
	}

	//设置行列数
	SetCrossSetColumnRowNum(iChanCount);

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

int CSystemSetDlg::GetCrossSetParam()
{
	float** ppCrosstalkPara = theApp.m_ppFCrosstalkPara;
	if(NULL == ppCrosstalkPara)
	{
		return -1;
	}

	int iChanCount = m_lstCrossSet.GetRowNum();
	if(iChanCount < 1)
	{
		return -2;
	}

	int iColumnNum = m_lstCrossSet.GetColNum();
	if(iChanCount != iColumnNum - 1)
	{
		return -3;
	}

	for(int i = 0;i < iChanCount;++i)
	{
		for(int j = 0;j < iChanCount;++j)
		{
			ppCrosstalkPara[i][j] = _ttof(m_lstCrossSet.GetItemText(i,j + 1));
		}
	}

	return 0;
}

int CSystemSetDlg::SetCrossSetColumnRowNum(int iChanNum,BOOL bRedraw /*= FALSE*/)
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

	int iNum = theApp.m_vecDefDyeName.size();

	str1 = pLangInfo->GetText(51);
	for(int i = 0;i < iChanNum;++i)
	{
		if(i < iNum)
		{
			strTemp = theApp.m_vecDefDyeName[i];
		}
		else
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

		if(i < iNum)
		{
			strTemp = theApp.m_vecDefDyeName[i];
		}
		else
		{
			strTemp.Format(_T("%s%d"),str1,i + 1);
		}

		m_lstCrossSet.SetItemText(i,0,strTemp);
		m_lstCrossSet.SetCellDisableState(i,i + 1,TRUE);
	}

	//设置列宽
	int iWidth = 660 * m_dHoriRatio;
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

int CSystemSetDlg::SetMeltCrossSetParam()
{
	float** ppCrosstalkPara = theApp.m_ppFMeltCrosstalk;
	if(NULL == ppCrosstalkPara)
	{
		return -2;
	}

	int iChanCount = theApp.m_iDefChanNum;

	//PS96不显示校准通道的串扰信息
#ifdef PS96INSTRUMENT
	--iChanCount;
#endif

	if(iChanCount < 1)
	{
		return -3;
	}

	//设置行列数
	SetMeltCrossSetColumnRowNum(iChanCount);

	CString strTemp(_T(""));
	for(int i = 0;i < iChanCount;++i)
	{
		for(int j = 0;j < iChanCount;++j)
		{
			strTemp.Format(_T("%.3lf"),ppCrosstalkPara[i][j]);
			m_lstMeltCrossSet.SetItemText(i,j + 1,strTemp);
		}
	}

	m_lstMeltCrossSet.BottomBlankSetAutoFitHeight();

	m_lstMeltCrossSet.Invalidate(FALSE);

	return 0;
}

int CSystemSetDlg::GetMeltCrossSetParam()
{
	float** ppCrosstalkPara = theApp.m_ppFMeltCrosstalk;
	if(NULL == ppCrosstalkPara)
	{
		return -1;
	}

	int iChanCount = m_lstMeltCrossSet.GetRowNum();
	if(iChanCount < 1)
	{
		return -2;
	}

	int iColumnNum = m_lstMeltCrossSet.GetColNum();
	if(iChanCount != iColumnNum - 1)
	{
		return -3;
	}

	for(int i = 0;i < iChanCount;++i)
	{
		for(int j = 0;j < iChanCount;++j)
		{
			ppCrosstalkPara[i][j] = _ttof(m_lstMeltCrossSet.GetItemText(i,j + 1));
		}
	}

	return 0;
}

int CSystemSetDlg::SetMeltCrossSetColumnRowNum(int iChanNum,BOOL bRedraw /*= FALSE*/)
{
	if(iChanNum < 1)
	{
		return -1;
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	CString strTemp(_T("")),str1(_T("通道"));
	m_lstMeltCrossSet.InsertColumn(0,pLangInfo->GetText(51));

	CMaxMinInfo maxMinInfo;
	maxMinInfo.SetBeOnlyFloatNumber(TRUE);
	maxMinInfo.SetBeSetMinValue(TRUE);
	maxMinInfo.SetMinValue(0);
	maxMinInfo.SetBeSetMaxValue(TRUE);
	maxMinInfo.SetMaxValue(1);
	maxMinInfo.SetBeMaxMinSimultaneousEffect(TRUE);

	int iNum = theApp.m_vecDefDyeName.size();

	str1 = pLangInfo->GetText(51);
	for(int i = 0;i < iChanNum;++i)
	{
		if(i < iNum)
		{
			strTemp = theApp.m_vecDefDyeName[i];
		}
		else
		{
			strTemp.Format(_T("%s%d"),str1,i + 1);
		}

		m_lstMeltCrossSet.InsertColumn(i + 1,strTemp);
		m_lstMeltCrossSet.SetColumnEditable(i + 1,TRUE);
		m_lstMeltCrossSet.SetColumnMaxMinInfo(i + 1,maxMinInfo);
	}

	for(int i = 0;i < iChanNum;++i)
	{
		m_lstMeltCrossSet.InsertRowSetHeight(i,30 * m_dVertRatio,FALSE);

		if(i < iNum)
		{
			strTemp = theApp.m_vecDefDyeName[i];
		}
		else
		{
			strTemp.Format(_T("%s%d"),str1,i + 1);
		}

		m_lstMeltCrossSet.SetItemText(i,0,strTemp);
		m_lstMeltCrossSet.SetCellDisableState(i,i + 1,TRUE);
	}

	//设置列宽
	int iWidth = 809 * m_dHoriRatio;
	int iTempInt = 87 * m_dHoriRatio;

	m_lstMeltCrossSet.SetColumnWidth(0,iTempInt);
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
			iWidth -= m_lstMeltCrossSet.GetFrameLineWidth() * 2;
			m_lstMeltCrossSet.SetColumnWidth(i + 1,iWidth);
		}
		else
		{
			m_lstMeltCrossSet.SetColumnWidth(i + 1,iTempInt);
			iWidth -= iTempInt;
		}
	}

	if(bRedraw && NULL != m_lstCrossSet.GetSafeHwnd())
	{
		m_lstMeltCrossSet.Invalidate(FALSE);
	}

	return 0;
}





// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "AddCycleDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"

// CAddCycleDlg dialog
IMPLEMENT_DYNAMIC(CAddCycleDlg, CDialogEx)

CAddCycleDlg::CAddCycleDlg(CMySegEditInfo segEditInfo,CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddCycleDlg::IDD, pParent)
{
	m_segEditInfo = segEditInfo;
	m_iFrameW = 1;
	m_iRA = 10;
	m_iWndW = 330;
	m_iWndH = 316;
	m_iCapH = 50;
	m_iCycleChangeNum = 0;
	m_iEngExtraW = 52;
}

CAddCycleDlg::~CAddCycleDlg()
{

}

void CAddCycleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAddCycleDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_ADDCYCLEDLG_BT_SAVE,OnBnClickedSave)
	ON_BN_CLICKED(ID_ADDCYCLEDLG_BT_CANCEL,OnBnClickedCancel)
END_MESSAGE_MAP()

BOOL CAddCycleDlg::OnEraseBkgnd(CDC* pDC)
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

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCapH,rcUseful.Width(),rcUseful.Height() - m_iCapH,0,0,m_iRA,m_iRA,TRUE,Color(197,197,197),m_iFrameW,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCapH,m_iRA,m_iRA,0,0,TRUE,COLOR_SOFT_THEME,m_iFrameW,TRUE,COLOR_SOFT_THEME,1);
	
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = memDc.SelectObject(&m_fontSet);

	int iX1 = rcUseful.left + 18 * m_dHoriRatio,iX2 = rcUseful.left + 124 * m_dHoriRatio,iX3 = rcUseful.left + 127 * m_dHoriRatio,iX4 = rcUseful.left + 306 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iX2 += m_iEngExtraW;
		iX3 += m_iEngExtraW;
		iX4 += m_iEngExtraW;
	}
	
	memDc.DrawText(pLangInfo->GetText(441),CRect(iX1,rcUseful.top + 70 * m_dVertRatio,iX2,rcUseful.top + 110 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(m_segEditInfo.m_strSegName,CRect(iX3,rcUseful.top + 70 * m_dVertRatio,iX4,rcUseful.top + 110 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS | DT_SINGLELINE);

	memDc.DrawText(pLangInfo->GetText(440),CRect(iX1,rcUseful.top + 130 * m_dVertRatio,iX2,rcUseful.top + 170 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	CString strTemp(_T(""));
	strTemp.Format(_T("%d"),m_segEditInfo.m_cycleNum);
	memDc.DrawText(strTemp,CRect(iX3,rcUseful.top + 130 * m_dVertRatio,iX4,rcUseful.top + 170 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.DrawText(pLangInfo->GetText(439),CRect(iX1,rcUseful.top + 190 * m_dVertRatio,iX2,rcUseful.top + 226 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(pLangInfo->GetText(46),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.right - 10,rcUseful.top + m_iCapH - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
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

int CAddCycleDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iEngExtraW *= m_dHoriRatio;
	m_iWndW *= m_dHoriRatio;
	m_iWndH *= m_dVertRatio;
	m_iCapH *= m_dVertRatio;

	if(theApp.GetBeEnglish())
	{
		m_iWndW += m_iEngExtraW;
	}

	return CDialogEx::OnCreate(lpCreateStruct);
}

void CAddCycleDlg::OnLButtonDown(UINT nFlags, CPoint point)
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

BOOL CAddCycleDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0 * m_dHoriRatio,0 * m_dVertRatio,330 * m_dHoriRatio,220 * m_dVertRatio,SWP_NOZORDER);
	SetRoundRect(m_iRA,m_iRA);
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

void CAddCycleDlg::CreateCtrl()
{
	CreateBt();
	CreateCkBt();
	CreateSt();
	CreateEd();
}

void CAddCycleDlg::CreateBt()
{
	m_btSave.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ADDCYCLEDLG_BT_SAVE);
	m_btCancel.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ADDCYCLEDLG_BT_CANCEL);
}

void CAddCycleDlg::CreateCkBt()
{
	
}

void CAddCycleDlg::CreateSt()
{
	
}

void CAddCycleDlg::CreateEd()
{
	m_edAddCycleNum.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_ADDCYCLEDLG_ED_ADDCYCLENUM);
}

void CAddCycleDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CAddCycleDlg::InitCtrl()
{
	InitBt();
	InitCkBt();
	InitSt();
	InitEd();
}

void CAddCycleDlg::InitBt()
{
	int iFontH = 16 * CFontSet::m_dTxRatio;

	m_btSave.SetMyFont(m_stTxFont,iFontH);
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

void CAddCycleDlg::InitCkBt()
{
	
}

void CAddCycleDlg::InitSt()
{
	
}

void CAddCycleDlg::InitEd()
{
	int iFtH = 16 * m_dTxRatio;

	CString strTemp(_T(""));

	m_edAddCycleNum.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_edAddCycleNum.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_edAddCycleNum.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_edAddCycleNum.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	m_edAddCycleNum.SetMyFont(m_stTxFont,iFtH);
	
	CMaxMinInfo mmInfo;
	mmInfo.SetBeOnlyIntergerNumber(TRUE);
	m_edAddCycleNum.SetMaxMinInfo(mmInfo);

	return;
}

void CAddCycleDlg::SetCtrlPos()
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

	int iX = rcUseful.left + 127 * m_dHoriRatio,iY = rcUseful.top + 190 * m_dVertRatio,iW = 180 * m_dHoriRatio,iH = 40 * m_dVertRatio;
	if(theApp.GetBeEnglish())
	{
		iX += m_iEngExtraW;
	}

	m_edAddCycleNum.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);

	iY = rcUseful.top + 192 * m_dVertRatio;
	iW = 90 * m_dHoriRatio;

	iY += 60 * m_dVertRatio;
	m_btSave.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);

	iX += (iW + 12 * m_dHoriRatio);
	m_btCancel.MoveWindow(iX,iY,iW,iH,FALSE);
	AddRect(iX,iY,iW,iH);

	CalcRectResult();

	return;
}

void CAddCycleDlg::ShowCtrl()
{
	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);
	m_edAddCycleNum.ShowWindow(SW_SHOW);
}

void CAddCycleDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_btSave.SetWindowText(pLangInfo->GetText(149));
	m_btCancel.SetWindowText(pLangInfo->GetText(150));

	return;
}

void CAddCycleDlg::OnBnClickedSave()
{
	int m_iCycleChangeNum = _ttoi(m_edAddCycleNum.GetText());
	int iNewCycle = m_segEditInfo.m_cycleNum + m_iCycleChangeNum;
	if(iNewCycle < MINCYCLENUM || iNewCycle > MAXCYCLENUM)
	{
		CString str1(_T(""));
		str1.Format(theApp.m_pLangInfo->GetText(438),MINCYCLENUM,MAXCYCLENUM);
		PopWarningTipInfo(str1);
		return;
	}

	OnOK();
	return;
}

void CAddCycleDlg::OnBnClickedCancel()
{
	OnCancel();
}

BOOL CAddCycleDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

int CAddCycleDlg::GetCycleChangeNum()
{
	return m_iCycleChangeNum;
}



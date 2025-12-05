// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SystemSetDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"

// CSystemSetDlg dialog
IMPLEMENT_DYNAMIC(CSystemSetDlg, CDialogEx)

CSystemSetDlg::CSystemSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSystemSetDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
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
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CSystemSetDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(&memDc,0,0,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

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

	return TRUE;
}

void CSystemSetDlg::OnSize(UINT nType,int cx,int cy)
{
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	__super::OnSize(nType,cx,cy);
}	

BOOL CSystemSetDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	CreateCtrl();
	InitDlg();
	InitCtrl();
	m_bCtrlCreated = TRUE;
	SetCtrlPos();
	SetCtrlText();
	ShowCtrl();

	return TRUE;
}

void CSystemSetDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateEd();
	CreateLst();
}

void CSystemSetDlg::CreateBt()
{

}

void CSystemSetDlg::CreateSt()
{
	
}

void CSystemSetDlg::CreateEd()
{
	
}

void CSystemSetDlg::CreateLst()
{
	m_lstUserInfo.Create(IDD_MIDDLEDLG,this);
}

void CSystemSetDlg::InitDlg()
{
	SetTextClr1(RGB(86,86,86));
	SetMyFont(theApp.m_strTextFont,14,FW_NORMAL,TYPE_PIXEL);
}

void CSystemSetDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitLst();
}

void CSystemSetDlg::InitBt()
{
	
}

void CSystemSetDlg::InitSt()
{
	
}

void CSystemSetDlg::InitEd()
{
	
}

void CSystemSetDlg::InitLst()
{
	int iFontH = 16 * m_dTxRatio;

	m_lstUserInfo.SetHeaderHeight(24 * m_dVertRatio);
	m_lstUserInfo.SetMyHeaderFont(theApp.m_strTextFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstUserInfo.SetScrollArrowWH(4 * m_dHoriRatio,8 * m_dVertRatio);
	m_lstUserInfo.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_lstUserInfo.SetItemHeight(53 * m_dVertRatio);

	int iTempInt1 = 34 * m_dVertRatio;
	m_lstUserInfo.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstUserInfo.m_horiScroll.m_scrollH = iTempInt1;
	m_lstUserInfo.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstUserInfo.m_vertScroll.m_scrollW = iTempInt1;
	m_lstUserInfo.SetMyFont(theApp.m_strTextFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstUserInfo.SetTextClr1(RGB(0X39,0X45,0X65));

	m_lstUserInfo.InsertColumn(0,_T(""),FALSE);
	m_lstUserInfo.InsertColumn(1,_T(""),FALSE);
	m_lstUserInfo.InsertColumn(2,_T(""),FALSE);
	m_lstUserInfo.InsertColumn(3,_T(""),FALSE);
	
	int iTempInt2 = 0,iW = 1720 * m_dHoriRatio;
	iTempInt2 = 60 * m_dHoriRatio;
	m_lstUserInfo.SetColumnWidth(0,iTempInt2);
	iW -= iTempInt2;
	iTempInt2 = 466 * m_dHoriRatio;
	m_lstUserInfo.SetColumnWidth(1,iTempInt2);
	iW -= iTempInt2;
	iTempInt2 = 540 * m_dHoriRatio;
	m_lstUserInfo.SetColumnWidth(2,iTempInt2);
	iW -= iTempInt2;
	iW -= m_lstUserInfo.GetFrameLineWidth() * 2;
	m_lstUserInfo.SetColumnWidth(3,iW);

	return;
}

void CSystemSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(rect);

	m_rcInit = rect;
	ClearRectToDelete();

	m_lstUserInfo.MoveWindow(16 * m_dHoriRatio,16 * m_dVertRatio,1720 * m_dHoriRatio,840 * m_dVertRatio,FALSE);
	AddRectToDelete(16 * m_dHoriRatio,16 * m_dVertRatio,1720 * m_dHoriRatio,840 * m_dVertRatio);

	CalcRectResult();

	return;
}

void CSystemSetDlg::ShowCtrl()
{
	m_lstUserInfo.ShowWindow(SW_SHOW);
}

void CSystemSetDlg::SetCtrlText()
{
	m_lstUserInfo.SetColumnText(0,theApp.m_pLangInfo->GetText(29));
	m_lstUserInfo.SetColumnText(1,theApp.m_pLangInfo->GetText(350));
	m_lstUserInfo.SetColumnText(2,theApp.m_pLangInfo->GetText(351));
	m_lstUserInfo.SetColumnText(3,theApp.m_pLangInfo->GetText(352));

	return;
}

void CSystemSetDlg::ResetLstColumnText()
{
	m_lstUserInfo.SetColumnText(0,theApp.m_pLangInfo->GetText(29));
	m_lstUserInfo.SetColumnText(1,theApp.m_pLangInfo->GetText(350));
	m_lstUserInfo.SetColumnText(2,theApp.m_pLangInfo->GetText(351));
	m_lstUserInfo.SetColumnText(3,theApp.m_pLangInfo->GetText(352));

	return;
}

int CSystemSetDlg::RefreshLanguageDisplay()
{
	int iFontH = 16 * m_dTxRatio;

	m_lstUserInfo.SetMyHeaderFont(theApp.m_strTextFont,iFontH);
	m_lstUserInfo.SetMyFont(theApp.m_strTextFont);
	ResetLstColumnText();

	return 0;
}





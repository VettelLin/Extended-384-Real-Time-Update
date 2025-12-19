// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "RunStateDlg.h"

// CRunStateDlg dialog
IMPLEMENT_DYNAMIC(CRunStateDlg, CDialogEx)

CRunStateDlg::CRunStateDlg(BOOL bPrjManage /*= FALSE*/,CWnd* pParent /*=NULL*/)
	: CDialogEx(CRunStateDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
}

CRunStateDlg::~CRunStateDlg()
{
}

void CRunStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRunStateDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CRunStateDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);
	
	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),RGB(255,255,255));

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	memDc.FillSolidRect(0,0,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(&memDc,0,0,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	clock_t ckStart = clock();	
	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	ckStart = clock() - ckStart;

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

void CRunStateDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

BOOL CRunStateDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	InitDlg();
	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	return TRUE;
}

void CRunStateDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();
}

void CRunStateDlg::CreateBt()
{
	
}

void CRunStateDlg::CreateSt()
{
	
}

void CRunStateDlg::CreateCb()
{
	
}

void CRunStateDlg::CreateEd()
{
	
}

void CRunStateDlg::InitDlg()
{
	SetTextClr1(RGB(0X66,0X66,0X66));
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CRunStateDlg::InitSonDlg()
{
	InitMonitorBaseInfoDlg();
}

void CRunStateDlg::InitMonitorBaseInfoDlg()
{
	m_mntBaseInfoDlg.Create(IDD_MIDDLEDLG,this);
}

void CRunStateDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitCb();
}

void CRunStateDlg::InitBt()
{
	
}

void CRunStateDlg::InitSt()
{
	
}

void CRunStateDlg::InitCb()
{
	return;
}

void CRunStateDlg::InitEd()
{

}

void CRunStateDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	m_rcManage.AddRectToDelete(rect.left,rect.top,rect.Width(),rect.Height());
	m_mntBaseInfoDlg.SetWindowPos(this,rect.left,rect.top,rect.Width(),rect.Height(),SWP_NOZORDER);

	m_rcManage.CalcRectResult();
}

void CRunStateDlg::ShowCtrl()
{
	m_mntBaseInfoDlg.ShowWindow(SW_SHOW);
}

void CRunStateDlg::SetCtrlEnableState(BOOL bEnabled)
{
	
}

void CRunStateDlg::ClearPage()
{
	
}

int CRunStateDlg::RefreshLanguageDisplay()
{
	m_mntBaseInfoDlg.RefreshLanguageDisplay();
	return 0;
}


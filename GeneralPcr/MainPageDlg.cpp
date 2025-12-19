// SamSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "MainPageDlg.h"
#include "afxdialogex.h"


// CMainPageDlg dialog

IMPLEMENT_DYNAMIC(CMainPageDlg, CDialogEx)

CMainPageDlg::CMainPageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPageDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
	m_pCurSelSwt = NULL;
	m_pCurSelWnd = NULL;
	m_iSwitchBtBkH = 40;
	m_iSwitchBtH = 32;
	m_iSwitchBtBkW = 244;
	m_iSwitchBtW = 120;
}

CMainPageDlg::~CMainPageDlg()
{

}

void CMainPageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMainPageDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_MAINPAGEDLG_BT_NEW,&CMainPageDlg::OnBnClickedBtNew)
	ON_BN_CLICKED(ID_MAINPAGEDLG_BT_OPEN,&CMainPageDlg::OnBnClickedBtOpen)
END_MESSAGE_MAP()

BOOL CMainPageDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	int iTempInt1 = 336 * m_dHoriRatio;

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),RGB(0XE6,0XF4,0XF5));

	DrawRoundRectangleNew(&memDc,16 * m_dHoriRatio,12 * m_dVertRatio,1376 * m_dHoriRatio,rect.Height() - 12 * m_dVertRatio - 8 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	DrawRoundRectangleNew(&memDc,32 * m_dHoriRatio,28 * m_dVertRatio,m_iSwitchBtBkW,m_iSwitchBtBkH,8,8,8,8,TRUE,Color(0XE7,0XE7,0XE7),1,TRUE,COLOR_SOFT_THEME,1);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

void CMainPageDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

int CMainPageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iSwitchBtBkH *= m_dVertRatio;
	if(0 != (m_iSwitchBtBkH % 2))
	{
		m_iSwitchBtBkH += 1;
	}
	m_iSwitchBtH *= m_dVertRatio;
	if(0 != (m_iSwitchBtH % 2))
	{
		m_iSwitchBtH -= 1;
	}

	m_iSwitchBtBkW *= m_dHoriRatio;
	if(0 != (m_iSwitchBtBkW % 2))
	{
		m_iSwitchBtBkW += 1;
	}
	m_iSwitchBtW *= m_dHoriRatio;
	if(0 != (m_iSwitchBtW % 2))
	{
		m_iSwitchBtW -= 1;
	}

	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CMainPageDlg::OnBnClickedBtNew()
{
	SetPageSelected(EHEDM_FIGURE);
}

void CMainPageDlg::OnBnClickedBtOpen()
{
	SetPageSelected(EHEDM_LIST);
}

BOOL CMainPageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitDlg();
	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	SetPageSelected(EHEDM_FIGURE);

	return TRUE;
}

void CMainPageDlg::InitDlg()
{

}

void CMainPageDlg::InitSonDlg()
{
	
}

void CMainPageDlg::CreateCtrl()
{
	CreateBt();
	CreateLst();
	CreateSamPlateDlg();
}

void CMainPageDlg::CreateBt()
{
	m_btNew.Create(_T("新建"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_MAINPAGEDLG_BT_NEW);
	m_btOpen.Create(_T("打开"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_MAINPAGEDLG_BT_OPEN);
}

void CMainPageDlg::CreateLst()
{
	
}

void CMainPageDlg::CreateSamPlateDlg()
{
	
}

void CMainPageDlg::InitCtrl()
{
	InitBt();
	InitLst();
	InitSamPlateDlg();
}

void CMainPageDlg::InitBt()
{
	int iFontH = 16 * CFontSet::m_dTxRatio;

	m_btNew.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_btNew.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_btNew.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_btNew.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_btNew.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	
	m_btOpen.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_btOpen.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_btOpen.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_btOpen.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_btOpen.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CMainPageDlg::InitLst()
{
	
}

void CMainPageDlg::InitSamPlateDlg()
{
	
}

void CMainPageDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iTempInt3 = (m_iSwitchBtBkW - m_iSwitchBtW * 2) / 2;;
	int iXStart = 32 * m_dHoriRatio + iTempInt3;
	int iYStart = 28 * m_dVertRatio + (m_iSwitchBtBkH - m_iSwitchBtH) / 2;

	m_btNew.SetWindowPos(this,iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);
	m_btOpen.SetWindowPos(this,iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);
}

void CMainPageDlg::ShowCtrl()
{
	//m_btNew.ShowWindow(SW_SHOW);
	//m_btOpen.ShowWindow(SW_SHOW);
}

void CMainPageDlg::SetPageSelected(EHISTORYEXPDISPALYMODE eExpDisplayMode)
{
	if(EHEDM_FIGURE == eExpDisplayMode)
	{
		
	}
	else if(EHEDM_LIST == eExpDisplayMode)
	{
		
	}

	return;
}


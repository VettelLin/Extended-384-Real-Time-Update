// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BaseSetDlg.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"

// CBaseSetDlg dialog
IMPLEMENT_DYNAMIC(CBaseSetDlg, CDialogEx)

CBaseSetDlg::CBaseSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBaseSetDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
}

CBaseSetDlg::~CBaseSetDlg()
{
}

void CBaseSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBaseSetDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CBaseSetDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	memDC.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDC,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	
	memDC.SelectObject(pOldBitmap);
	bitMap.DeleteObject();
	memDC.DeleteDC();

	return TRUE;
}

void CBaseSetDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	return;
}

BOOL CBaseSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	return TRUE;
}

void CBaseSetDlg::InitSonDlg()
{
	m_instrumentInfoDlg.Create(IDD_MIDDLEDLG,this);
	m_expeInfoDlg.Create(IDD_MIDDLEDLG,this);
}

void CBaseSetDlg::CreateCtrl()
{
	CreateSt();
}

void CBaseSetDlg::CreateSt()
{
	
}

void CBaseSetDlg::InitCtrl()
{
	InitSt();
}

void CBaseSetDlg::InitSt()
{
	
}

void CBaseSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	int iHeight = rect.Height() - 12 * m_dVertRatio - 10 * m_dVertRatio;

	m_rcManage.AddRectToDelete(16 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,iHeight);
	m_expeInfoDlg.SetWindowPos(this,16 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,iHeight,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(900 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,iHeight);
	m_instrumentInfoDlg.SetWindowPos(this,900 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,iHeight,SWP_NOZORDER);

	m_rcManage.CalcRectResult();
}

void CBaseSetDlg::ShowCtrl()
{
	m_instrumentInfoDlg.ShowWindow(SW_SHOW);
	m_expeInfoDlg.ShowWindow(SW_SHOW);
}

void CBaseSetDlg::GetBaseInfo(CExperimentInfo& info)
{
	m_expeInfoDlg.GetExperiInfo(info);
	m_instrumentInfoDlg.GetInstruInfo(info);

	return;
}

void CBaseSetDlg::SetBaseInfo(CExperimentInfo& info)
{
	m_expeInfoDlg.SetExperiInfo(info);
	m_instrumentInfoDlg.SetInstruInfo(info);
	return;
}

void CBaseSetDlg::SetCtrlEnableState(BOOL bEnabled)
{
	m_instrumentInfoDlg.SetCtrlEnableState(bEnabled);
	m_expeInfoDlg.SetCtrlEnableState(bEnabled);
}

void CBaseSetDlg::ClearPage()
{
	m_instrumentInfoDlg.ClearPage();
	m_expeInfoDlg.ClearPage();
}

int CBaseSetDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	m_expeInfoDlg.SetEditMode(m_eExpEditMode);
	m_instrumentInfoDlg.SetEditMode(m_eExpEditMode);
	return 0;
}

void CBaseSetDlg::FileOpened(CExperimentData* pExpData)
{
	if(NULL == pExpData)
	{
		return;
	}

	SetBaseInfo(*pExpData->GetExperimentInfo());
	return;
}

CString CBaseSetDlg::GetExpName()
{
	return m_expeInfoDlg.GetExpName();
}

int CBaseSetDlg::RefreshLanguageDisplay()
{
	m_expeInfoDlg.RefreshLanguageDisplay();
	m_instrumentInfoDlg.RefreshLanguageDisplay();

	return 0;
}


void CBaseSetDlg::SetSelectedInstrument(CString strModel, CString strSN)
{
	m_instrumentInfoDlg.SetSelectedInstrument(strModel, strSN);
}

void CBaseSetDlg::ReshowInstrumentList()
{
	m_instrumentInfoDlg.ReshowInstrumentList();
}

// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "SetDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"

// CSetDlg dialog
IMPLEMENT_DYNAMIC(CSetDlg, CDialogEx)

CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetDlg::IDD, pParent)
{
	m_iFunBtnHeight = 40;
	m_pCurSelSwt = NULL;
	m_pCurSelDlg = NULL;
	m_bCtrlCreated = FALSE;
}

CSetDlg::~CSetDlg()
{

}

void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSetDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()

	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_ADD,OnBnClickedAdd)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_EDIT,OnBnClickedEdit)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_DELETE,OnBnClickedDelete) 

	ON_BN_CLICKED(ID_SETDLG_SWTBT_LISSET,OnBnClickedSwtLisSet)
	ON_BN_CLICKED(ID_SETDLG_SWTBT_USERMANAGESET,OnBnClickedSwtUserManageSet)
	ON_BN_CLICKED(ID_SETDLG_SWTBT_PS96SET,OnBnClickedSwtPs96Set)
	ON_BN_CLICKED(ID_SETDLG_SWTBT_SYSTEMSET,OnBnClickedSwtSystemSet)
END_MESSAGE_MAP()

BOOL CSetDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	CRect rectFun = rect;
	rectFun.bottom = rectFun.top + int(m_iFunBtnHeight*m_dVertRatio);
	pDC->FillSolidRect(rectFun, RGB(255, 255, 255));

	int iStartY = int((12+m_iFunBtnHeight) * m_dVertRatio);
	DrawRoundRectangleNew(pDC,0,iStartY,int(136 * m_dHoriRatio),rect.Height() - iStartY,0,16,0,0,FALSE,Color(0XFF,0XFF,0XFF),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	return TRUE;
}

BOOL CSetDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	CreateBt();
	InitDlg();
	IntFunBtn();
	InitBt();
	m_bCtrlCreated = TRUE;
	SetCtrlPos();

	SetCtrlText();
	SetPageSelected(PS96SETDLG);

	ShowCtrl();

	return TRUE;
}

void CSetDlg::CreateBt()
{
	m_btnAdd.Create(_T("新增"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_ADD);
	m_btnEdit.Create(_T("编辑"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_EDIT);
	m_btnDelete.Create(_T("删除"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_DELETE);

	m_swtLisSet.Create(_T("LIS设置"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SETDLG_SWTBT_LISSET);
	m_swtUserManageSet.Create(_T("用户管理"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SETDLG_SWTBT_USERMANAGESET);
	m_swtInstru1Set.Create(_T("PS96 设置"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SETDLG_SWTBT_PS96SET);
	m_swtSysSet.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SETDLG_SWTBT_SYSTEMSET);
}

void CSetDlg::InitDlg()
{
	SetTextClr1(RGB(86,86,86));
	SetMyFont(m_stTxFont,14,FW_NORMAL,TYPE_PIXEL);

	m_lisSetDlg.Create(IDD_MIDDLEDLG,this);
	m_userManageSetDlg.Create(IDD_MIDDLEDLG,this);
	m_ps96SetDlg.Create(IDD_MIDDLEDLG,this);
}


void CSetDlg::IntFunBtn()
{
	int iFontH = 16 * m_dTxRatio;

	int iTempInt1 = 16 * m_dHoriRatio;
	int iTempInt2 = 10 * m_dHoriRatio;

	m_btnAdd.SetPngBkPic(_T(".\\BkPic\\btnew.png"),2,iTempInt1,0,iTempInt2);
	m_btnAdd.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnAdd.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btnEdit.SetPngBkPic(_T(".\\BkPic\\btedit.png"),2,iTempInt1,0,iTempInt2);
	m_btnEdit.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnEdit.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btnDelete.SetPngBkPic(_T(".\\BkPic\\btdelete.png"),2,iTempInt1,0,iTempInt2);
	m_btnDelete.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnDelete.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);
}

void CSetDlg::InitBt()
{
	int iFontH = 18 * m_dTxRatio;

	CIconBtInfo iconBtInfo;

	if(theApp.GetBeEnglish())
	{
		iconBtInfo.m_rcIcon = Rect(10 * m_dHoriRatio,18 * m_dVertRatio,24 * m_dHoriRatio,24 * m_dVertRatio);
		iconBtInfo.m_rcText = CRect(35 * m_dHoriRatio,16 * m_dVertRatio,135 * m_dHoriRatio,43 * m_dVertRatio);
	}
	else
	{
		iconBtInfo.m_rcIcon = Rect(12 * m_dHoriRatio,18 * m_dVertRatio,24 * m_dHoriRatio,24 * m_dVertRatio);
		iconBtInfo.m_rcText = CRect(40 * m_dHoriRatio,16 * m_dVertRatio,132 * m_dHoriRatio,43 * m_dVertRatio);
	}

	iconBtInfo.m_bRoundRectBk = FALSE;
	iconBtInfo.m_iSonIconNum = 2;
	iconBtInfo.m_bSonIconPosOrder = FALSE;
	iconBtInfo.m_clrNormal = Color(0XFF,0XFF,0XFF);
	iconBtInfo.m_clrHover = Color(0XFF,0XFF,0XFF);

	iconBtInfo.m_clrSelected = COLOR_SOFT_THEME;
	iconBtInfo.m_clrDisabled = COLOR_SOFT_THEME;

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\SystemSet.png");
	m_swtInstru1Set.SetMyFont(m_stTxFont,iFontH);
	m_swtInstru1Set.SetIconBtInfo(iconBtInfo);
	m_swtInstru1Set.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\UserManage.png");
	m_swtUserManageSet.SetMyFont(m_stTxFont,iFontH);
	m_swtUserManageSet.SetIconBtInfo(iconBtInfo);
	m_swtUserManageSet.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\system.png");
	m_swtSysSet.SetMyFont(m_stTxFont,iFontH);
	m_swtSysSet.SetIconBtInfo(iconBtInfo);
	m_swtSysSet.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	m_swtLisSet.SetMyFont(m_stTxFont,iFontH);
	m_swtLisSet.SetIconBtInfo(iconBtInfo);
	m_swtLisSet.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	return;
}

void CSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iTempInt1 = 24 * m_dVertRatio;
	int iTempInt2 = 7 * m_dVertRatio;
	int iTempInt3 = 72 * m_dHoriRatio;

	m_btnAdd.SetWindowPos(this, int(18 * m_dHoriRatio), iTempInt2, iTempInt3, iTempInt1,SWP_NOZORDER);
	m_btnEdit.SetWindowPos(this, int(118 * m_dHoriRatio),iTempInt2, iTempInt3,iTempInt1,SWP_NOZORDER);
	m_btnDelete.SetWindowPos(this, int(218 * m_dHoriRatio),iTempInt2, iTempInt3,iTempInt1,SWP_NOZORDER);

	int startX = 0 * m_dHoriRatio;
	int startY = int((28+m_iFunBtnHeight) * m_dVertRatio);
	int swtWidth = 136 * m_dHoriRatio;
	int iHeight = 60 * m_dVertRatio;
	int iInterval = 26 * m_dVertRatio;
	int iTempH = iHeight + iInterval;

	m_swtInstru1Set.MoveWindow(startX,startY,swtWidth,iHeight,FALSE);
	startY += iTempH;
	m_swtUserManageSet.MoveWindow(startX,startY,swtWidth,iHeight,FALSE);
	startY += iTempH;
	m_swtSysSet.MoveWindow(startX,startY,swtWidth,iHeight,FALSE);

	startY += iTempH;
	m_swtLisSet.MoveWindow(startX,startY,swtWidth,iHeight,FALSE);
	
	int dlgH = 882 * m_dVertRatio;
	m_lisSetDlg.MoveWindow(int(152 * m_dHoriRatio),int((12+m_iFunBtnHeight) * m_dVertRatio),int(1752 * m_dHoriRatio), dlgH,FALSE);
	m_userManageSetDlg.MoveWindow(int(152 * m_dHoriRatio),int((12+m_iFunBtnHeight) * m_dVertRatio),int(1752 * m_dHoriRatio),dlgH,FALSE);
	m_ps96SetDlg.MoveWindow(int(152 * m_dHoriRatio),int((12+m_iFunBtnHeight) * m_dVertRatio),int(1752 * m_dHoriRatio),dlgH,FALSE);

	return;
}

void CSetDlg::ShowCtrl()
{
	m_btnAdd.ShowWindow(SW_SHOW);
	m_btnEdit.ShowWindow(SW_SHOW);
	m_btnDelete.ShowWindow(SW_SHOW);

	m_swtInstru1Set.ShowWindow(SW_SHOW);
	m_swtUserManageSet.ShowWindow(SW_SHOW);
	//m_swtSysSet.ShowWindow(SW_SHOW);
	//m_swtLisSet.ShowWindow(SW_SHOW);

	return;
}

void CSetDlg::SetCtrlText()
{
	m_btnAdd.SetWindowText(theApp.m_pLangInfo->GetText(372));
	m_btnEdit.SetWindowText(theApp.m_pLangInfo->GetText(353));
	m_btnDelete.SetWindowText(theApp.m_pLangInfo->GetText(172));

	m_swtInstru1Set.SetWindowText(theApp.m_pLangInfo->GetText(205));
	m_swtUserManageSet.SetWindowText(theApp.m_pLangInfo->GetText(349));
	m_swtSysSet.SetWindowText(theApp.m_pLangInfo->GetText(431));
	return;
}

void CSetDlg::SetPageSelected(SETPAGETYPE eSetPageType)
{
	if(LISSETDLG == eSetPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtLisSet.EnableWindow(FALSE);
		m_lisSetDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_lisSetDlg;
		m_pCurSelSwt = &m_swtLisSet;
	}
	else if(USERMANAGESETDLG == eSetPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtUserManageSet.EnableWindow(FALSE);
		m_userManageSetDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_userManageSetDlg;
		m_pCurSelSwt = &m_swtUserManageSet;
	}
	else if(PS96SETDLG == eSetPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtInstru1Set.EnableWindow(FALSE);
		m_ps96SetDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_ps96SetDlg;
		m_pCurSelSwt = &m_swtInstru1Set;
	}

	UpdateFuncBtMode();

	return;
}

void CSetDlg::UpdateFuncBtMode()
{
	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;

	if(NULL != m_ps96SetDlg.GetSafeHwnd() && m_ps96SetDlg.IsWindowVisible())
	{
		m_btnAdd.ShowWindow(SW_HIDE);
		m_btnEdit.ShowWindow(SW_HIDE);
		m_btnDelete.ShowWindow(SW_HIDE);
	}
	else if(NULL != m_userManageSetDlg.GetSafeHwnd() && m_userManageSetDlg.IsWindowVisible())
	{
		m_btnAdd.ShowWindow(SW_SHOW);
		m_btnEdit.ShowWindow(SW_SHOW);
		m_btnDelete.ShowWindow(SW_SHOW);
	}
}

void CSetDlg::OnBnClickedSwtLisSet()
{
	SetPageSelected(LISSETDLG);
}

void CSetDlg::OnBnClickedSwtUserManageSet()
{
	SetPageSelected(USERMANAGESETDLG);
}

void CSetDlg::OnBnClickedSwtPs96Set()
{
	SetPageSelected(PS96SETDLG);
}

void CSetDlg::OnBnClickedSwtSystemSet()
{
	SetPageSelected(SYSTEMSETDLG);
}

void CSetDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	__super::OnSize(nType, cx, cy);
}	

int CSetDlg::RefreshLanguageDisplay()
{
	m_swtInstru1Set.SetMyFont(m_stTxFont);
	m_swtInstru1Set.SetWindowText(theApp.m_pLangInfo->GetText(205));

	m_swtUserManageSet.SetMyFont(m_stTxFont);
	m_swtUserManageSet.SetWindowText(theApp.m_pLangInfo->GetText(349));

	m_swtSysSet.SetMyFont(m_stTxFont);
	m_swtSysSet.SetWindowText(theApp.m_pLangInfo->GetText(431));

	m_ps96SetDlg.RefreshLanguageDisplay();
	m_userManageSetDlg.RefreshLanguageDisplay();

	return 0;
}

void CSetDlg::OnBnClickedAdd()
{
	if(m_userManageSetDlg.IsWindowVisible())
	{
		m_userManageSetDlg.AddUser();
	}
}

void CSetDlg::OnBnClickedEdit()
{
	if(m_userManageSetDlg.IsWindowVisible())
	{
		m_userManageSetDlg.EditUser();
	}
}

void CSetDlg::OnBnClickedDelete()
{
	if(m_userManageSetDlg.IsWindowVisible())
	{
		m_userManageSetDlg.DeleteUser();
	}
}
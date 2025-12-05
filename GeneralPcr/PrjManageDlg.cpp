// PrjManageDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "PrjManageDlg.h"
#include "ReagentItemManager.h"
#include "GeneralPcrDlg.h"
#include "afxdialogex.h"
#include "InstrumentPCR.h"
#include "PublicInfo.h"

// CPrjManageDlg dialog

IMPLEMENT_DYNAMIC(CPrjManageDlg, CDialogEx)

CPrjManageDlg::CPrjManageDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrjManageDlg::IDD, pParent)
{
	m_iFunBtnHeight = 40;
	m_bCtrlCreated = FALSE;
	m_pMainDlg = (CGeneralPcrDlg*)pParent;
	m_pCurSelSwt = NULL;
	m_pCurSelWnd = NULL;
	m_iSwitchBtBkH = 40;
	m_iSwitchBtH = 32;
	m_iSwitchBtBkW = 244;
	m_iSwitchBtW = 120;
}

CPrjManageDlg::~CPrjManageDlg()
{

}

void CPrjManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPrjManageDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_NEW,OnBnClickedNew)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_OPEN,OnBnClickedOpen)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_SAVE,OnBnClickedSave)	
	ON_BN_CLICKED(ID_BOTTOMFUNCDLG_BT_DELETE,OnBnClickedDelete) 
	ON_MESSAGE(WM_ROWSELECTCHANGE,OnMsgRowSelectedChange)
	ON_MESSAGE(WM_PRJ_CHANNEL_DYENAME_CHANGED,OnMsgChannelDyeNameChanged)
	ON_BN_CLICKED(ID_PRJMANAGEDLG_SWTBT_PRJBASEINFODLG,OnBnClickedSwtBtPrjBaseInfoDlg)
	ON_BN_CLICKED(ID_PRJMANAGEDLG_SWTBT_RSTJUDGEDLG,OnBnClickedSwtBtJudgeDlg)
END_MESSAGE_MAP()

BOOL CPrjManageDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	CRect rectFun = rect;
	rectFun.bottom = rectFun.top + int(m_iFunBtnHeight*m_dVertRatio);
	pDC->FillSolidRect(rectFun, RGB(255, 255, 255));

	int iStartY = int((12+m_iFunBtnHeight) * m_dVertRatio);
	DrawRoundRectangleNew(pDC,0,iStartY,int(136 * m_dHoriRatio), rect.Height() - iStartY,0,16,0,0,FALSE,Color(0XFF,0XFF,0XFF),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	return TRUE;
}

void CPrjManageDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	CDialogEx::OnSize(nType, cx, cy);
}

int CPrjManageDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

BOOL CPrjManageDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);

	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();
	SetPageSelected(EPMSP_BASE);

	ShowCtrl();

	return TRUE;
}

void CPrjManageDlg::CreateCtrl()
{
	m_btnNew.Create(_T("新建"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_NEW);
	m_btnOpen.Create(_T("打开"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_OPEN);
	m_btnSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_SAVE);
	m_btnDelete.Create(_T("删除"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0 ,0, 0, 0),this,ID_BOTTOMFUNCDLG_BT_DELETE);

	m_swtBtPrjBaseInfoDlg.Create(_T("基本信息"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PRJMANAGEDLG_SWTBT_PRJBASEINFODLG);
	m_swtBtRstJudgeDlg.Create(_T("结果判定"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PRJMANAGEDLG_SWTBT_RSTJUDGEDLG);

	m_lstSelPrj.Create(IDD_PRJMANAGEDLG,this);
	m_prjManageBaseInfoDlg.Create(IDD_MIDDLEDLG,this);
}

void CPrjManageDlg::InitCtrl()
{
	int iFontH = 16 * m_dTxRatio;
	int iTempInt1 = 16 * m_dHoriRatio;
	int iTempInt2 = 10 * m_dHoriRatio;

	m_btnNew.SetPngBkPic(_T(".\\BkPic\\btnew.png"),2,iTempInt1,0,iTempInt2);
	m_btnNew.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnNew.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btnOpen.SetPngBkPic(_T(".\\BkPic\\btopen.png"),2,iTempInt1,0,iTempInt2);
	m_btnOpen.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnOpen.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btnSave.SetPngBkPic(_T(".\\BkPic\\btsave.png"),2,iTempInt1,0,iTempInt2);
	m_btnSave.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnSave.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_btnDelete.SetPngBkPic(_T(".\\BkPic\\btdelete.png"),2,iTempInt1,0,iTempInt2);
	m_btnDelete.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btnDelete.SetWordStyle(DT_LEFT | DT_SINGLELINE | DT_VCENTER);

	m_swtBtPrjBaseInfoDlg.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtPrjBaseInfoDlg.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtPrjBaseInfoDlg.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtPrjBaseInfoDlg.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtPrjBaseInfoDlg.SetMyFont(m_stTxFont,iFontH);

	m_swtBtRstJudgeDlg.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtRstJudgeDlg.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtRstJudgeDlg.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtRstJudgeDlg.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtRstJudgeDlg.SetMyFont(m_stTxFont,iFontH);

	InitLst();
}

void CPrjManageDlg::InitLst()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_lstSelPrj.InsertColumn(0,_T("序号"));
	m_lstSelPrj.InsertColumn(1,_T("项目名称"));
	m_lstSelPrj.InsertColumn(2,_T("项目类型"));

	m_lstSelPrj.SetHeaderHeight(24 * m_dVertRatio);
	int iTempInt1 = 34 * m_dVertRatio;
	m_lstSelPrj.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstSelPrj.m_horiScroll.m_scrollH = iTempInt1;
	m_lstSelPrj.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstSelPrj.m_vertScroll.m_scrollW = iTempInt1;

	m_lstSelPrj.SetMyTitleFont(m_stTxFont,iFontH,FW_MEDIUM,TYPE_PIXEL);

	m_lstSelPrj.SetBeSingleSelect(TRUE);
	m_lstSelPrj.SetBeTellParentRowSelChange(TRUE);
	m_lstSelPrj.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstSelPrj.SetTextClr1(RGB(0X39,0X45,0X65));
	m_lstSelPrj.SetMyHeaderFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstSelPrj.SetBeSingleSelect(TRUE);
	m_lstSelPrj.SetFourAngleBkColor(CLR_SOFT_LIGHT);
	m_lstSelPrj.SetItemHeight(54 * m_dVertRatio);
	m_lstSelPrj.SetRowBkSelectedColor(RGB(0XF1,0XF1,0XF1));

	int iTempInt2 = 320 * m_dHoriRatio,iTempInt3 = 0;
	iTempInt3 = 60 * m_dHoriRatio;
	m_lstSelPrj.SetColumnWidth(0,iTempInt3);
	iTempInt2 -= iTempInt3;
	iTempInt3 = 155 * m_dHoriRatio;
	m_lstSelPrj.SetColumnWidth(1,iTempInt3);
	iTempInt2 -= iTempInt3;
	iTempInt2 -= m_lstSelPrj.GetFrameLineWidth() * 2;
	m_lstSelPrj.SetColumnWidth(2,iTempInt2);

	SetPrjManageLstInfo();

	return;
}

void CPrjManageDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iTempInt1 = 24 * m_dVertRatio;
	int iTempInt2 = 7 * m_dVertRatio;
	int iTempInt3 = 72 * m_dHoriRatio;

	m_btnNew.SetWindowPos(this, int(18 * m_dHoriRatio), iTempInt2, iTempInt3, iTempInt1,SWP_NOZORDER);
	m_btnOpen.SetWindowPos(this, int(118 * m_dHoriRatio),iTempInt2, iTempInt3,iTempInt1,SWP_NOZORDER);
	m_btnSave.SetWindowPos(this, int(218 * m_dHoriRatio),iTempInt2, iTempInt3,iTempInt1,SWP_NOZORDER);
	m_btnDelete.SetWindowPos(this, int(318 * m_dHoriRatio),iTempInt2, iTempInt3,iTempInt1,SWP_NOZORDER);

	m_lstSelPrj.SetWindowPos(this,int(16 * m_dHoriRatio),int((12+m_iFunBtnHeight) * m_dVertRatio),320 * m_dHoriRatio,882 * m_dVertRatio,SWP_NOZORDER);
	
	iTempInt3 = (m_iSwitchBtBkW - m_iSwitchBtW * 2) / 2;
	int iXStart = int(352 * m_dHoriRatio) + iTempInt3;
	int iYStart = int((12+m_iFunBtnHeight) * m_dVertRatio) + (m_iSwitchBtBkH - m_iSwitchBtH) / 2;
	m_swtBtPrjBaseInfoDlg.SetWindowPos(this,iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);

	m_swtBtRstJudgeDlg.SetWindowPos(this,iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);
#ifndef HIDE_PROJECT_RESULTCHECK_PAGE
	m_rcManage.AddRectToDelete(iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH);
#endif

	m_prjManageBaseInfoDlg.SetWindowPos(this, int(352 * m_dHoriRatio), int(60 * m_dVertRatio), int(1560 * m_dHoriRatio), int(832 * m_dVertRatio),SWP_NOZORDER);
}

void CPrjManageDlg::ShowCtrl()
{
	m_btnNew.ShowWindow(SW_SHOW);
	m_btnOpen.ShowWindow(SW_SHOW);
	m_btnSave.ShowWindow(SW_SHOW);
	m_btnDelete.ShowWindow(SW_SHOW);

	m_lstSelPrj.ShowWindow(SW_SHOW);
	m_prjManageBaseInfoDlg.ShowWindow(SW_SHOW);
	m_swtBtPrjBaseInfoDlg.ShowWindow(SW_SHOW);

#ifndef HIDE_PROJECT_RESULTCHECK_PAGE
	m_swtBtRstJudgeDlg.ShowWindow(SW_SHOW);
#endif
}

void CPrjManageDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_btnNew.SetWindowText(pLangInfo->GetText(167));
	m_btnOpen.SetWindowText(pLangInfo->GetText(168));
	m_btnSave.SetWindowText(pLangInfo->GetText(169));
	m_btnDelete.SetWindowText(pLangInfo->GetText(172));

	m_swtBtPrjBaseInfoDlg.SetWindowText(pLangInfo->GetText(188));
	m_swtBtRstJudgeDlg.SetWindowText(pLangInfo->GetText(129));

	m_lstSelPrj.SetColumnText(0,pLangInfo->GetText(29));
	m_lstSelPrj.SetColumnText(1,pLangInfo->GetText(30));
	m_lstSelPrj.SetColumnText(2,pLangInfo->GetText(31));

	return;
}

int CPrjManageDlg::SetPrjManageLstInfo()
{
	m_lstSelPrj.Clear();

	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
	{
		return -1;
	}

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
	{
		return -2;
	}

	CString strTemp(_T(""));
	int iReagentItemNum = pAryItem->GetSize();
	CReagentItem* pReagentItem = NULL;

	for(int i = 0;i < iReagentItemNum;++i)
	{
		pReagentItem = (CReagentItem*)pAryItem->GetAt(i);
		if(NULL == pReagentItem)
		{
			continue;
		}

		InsertOneItemToList(i, pReagentItem);
	}

	if(iReagentItemNum < 1 && NULL != m_prjManageBaseInfoDlg.GetSafeHwnd())
	{
		m_prjManageBaseInfoDlg.SetEditState(FALSE);
	}
	else
	{
		pReagentItem = (CReagentItem*)pAryItem->GetAt(0);
		if(NULL != pReagentItem)
		{
			SetReagentItem(*pReagentItem);
		}
		m_lstSelPrj.SetRowSelectState(0,TRUE,TRUE,TRUE);
	}

	return 0;
}

void CPrjManageDlg::SetReagentItem(CReagentItem& reagentItem,BOOL bSetEditableState /*= TRUE*/)
{
	m_prjManageBaseInfoDlg.SetReagentItem(reagentItem);
	if(bSetEditableState)
	{
		SetPrjEditableState(reagentItem.GetBeEditable(),TRUE);
	}
	return;
}

void CPrjManageDlg::GetReagentItem(CReagentItem& reagentItem)
{
	m_prjManageBaseInfoDlg.GetReagentItem(reagentItem);
}

void CPrjManageDlg::SetPrjEditableState(BOOL bEditable,BOOL bRedraw)
{
	m_prjManageBaseInfoDlg.SetEditState(bEditable,bRedraw);
}

LRESULT CPrjManageDlg::OnMsgRowSelectedChange(WPARAM wParam, LPARAM lParam)
{
	int itemCount = m_lstSelPrj.GetRowNum();
	vector<int> vecSelectedRowId;
	BOOL bSelected = FALSE;

	vecSelectedRowId = m_lstSelPrj.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		return 1;
	}

	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
	{
		return -1;
	}

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
	{
		return -2;
	}

	SetReagentItem(*(CReagentItem*)pAryItem->GetAt(vecSelectedRowId[0]));

	return 0;
}

LRESULT CPrjManageDlg::OnMsgChannelDyeNameChanged(WPARAM wParam,LPARAM lParam)
{
	int iRowNum = m_lstSelPrj.GetRowNum();
	if(iRowNum < 1)
	{
		return -1;
	}

	vector<int> vecSelectedRowId = m_lstSelPrj.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		return -3;
	}

	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
	{
		return -4;
	}

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
	{
		return -5;
	}

	CReagentItem* pReagentItem = (CReagentItem*)pAryItem->GetAt(vecSelectedRowId[0]);
	if(NULL == pReagentItem)
	{
		return -6;
	}

	pReagentItem->DyeNameChanged(m_prjManageBaseInfoDlg.m_iDyeChangeChanId,m_prjManageBaseInfoDlg.m_strDyeChangeName);

	return 0;
}

void CPrjManageDlg::OnBnClickedSwtBtPrjBaseInfoDlg()
{

}

void CPrjManageDlg::OnBnClickedSwtBtJudgeDlg()
{

}

void CPrjManageDlg::SetNewPrjBaseInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/)
{
	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
	{
		return;
	}

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
	{
		return;
	}	

	vector<int> vecSelectedRowId;
	vecSelectedRowId = m_lstSelPrj.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		return;
	}

	CReagentItem* pReagentItem = NULL;
	pReagentItem = (CReagentItem*)pAryItem->GetAt(vecSelectedRowId[0]);
	if(NULL == pReagentItem)
	{
		return;
	}

	if(ITEM_PRJBASE_EDPRJNAME == eItemInfoType)
	{
		m_lstSelPrj.SetItemText(vecSelectedRowId[0],1,strData,TRUE);
		pReagentItem->SetName(strData);
	}
	else if(ITEM_PRJBASE_CBPRJTYPE == eItemInfoType)
	{
		m_lstSelPrj.SetItemText(vecSelectedRowId[0],2,strData,TRUE);

		eExperimentType eType = EXP_TYPE_CUSTOM;
		if(strData == theApp.m_pLangInfo->GetText(33))
		{
			eType = EXP_TYPE_CUSTOM;
		}
		else if(strData == theApp.m_pLangInfo->GetText(219))
		{
			eType = EXP_TYPE_ABSOLUTE;
		}
		else if(strData == theApp.m_pLangInfo->GetText(220))
		{
			eType = EXP_TYPE_MELT;
		}
		pReagentItem->SetType(eType);
	}

	return;
}

void CPrjManageDlg::SetPageSelected(EPRJMANAGESONPAGE ePrjManageSonPage)
{
	if(EPMSP_BASE == ePrjManageSonPage)
	{
		if(NULL != m_pCurSelWnd->GetSafeHwnd())
		{
			m_pCurSelWnd->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBtPrjBaseInfoDlg.EnableWindow(FALSE);
		m_prjManageBaseInfoDlg.ShowWindow(SW_SHOW);
		m_pCurSelWnd = &m_prjManageBaseInfoDlg;
		m_pCurSelSwt = &m_swtBtPrjBaseInfoDlg;
	}
	else if(EPMSP_RSTJUDGE == ePrjManageSonPage)
	{
		
	}

	return;
}

int CPrjManageDlg::ResetPrjManageLstText()
{
	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
	{
		return -1;
	}

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
	{
		return -2;
	}

	CString strTemp(_T(""));
	int iReagentItemNum = pAryItem->GetSize();
	CReagentItem* pReagentItem = NULL;

	for(int i = 0;i < iReagentItemNum;++i)
	{
		pReagentItem = (CReagentItem*)pAryItem->GetAt(i);
		if(NULL == pReagentItem)
		{
			continue;
		}

		strTemp.Empty();
		eExperimentType eExperType = pReagentItem->GetType();
		switch(eExperType)
		{
		case EXP_TYPE_CUSTOM:
			strTemp = theApp.m_pLangInfo->GetText(33);
			break;
		case EXP_TYPE_ABSOLUTE:
			strTemp = theApp.m_pLangInfo->GetText(219);
			break;
		case EXP_TYPE_MELT:
			strTemp = theApp.m_pLangInfo->GetText(220);
			break;
		case EXP_TYPE_RELATIVE:
			strTemp = _T("相对定量");
			break;
		case EXP_TYPE_ALLELE:
			strTemp = _T("等位基因");
			break;
		default:
			strTemp.Empty();
			ASSERT(FALSE);
			break;
		}
		m_lstSelPrj.SetItemText(i,2,strTemp);
	}

	return 0;
}

int CPrjManageDlg::RefreshLanguageDisplay()
{
	m_swtBtPrjBaseInfoDlg.SetMyFont(m_stTxFont);
	m_swtBtPrjBaseInfoDlg.SetWindowText(theApp.m_pLangInfo->GetText(188));

	m_swtBtRstJudgeDlg.SetMyFont(m_stTxFont);
	m_swtBtRstJudgeDlg.SetWindowText(theApp.m_pLangInfo->GetText(129));
	
	//列表框
	int iFontH = 14 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}
	m_lstSelPrj.SetMyTitleFont(m_stTxFont,iFontH,FW_MEDIUM);
	m_lstSelPrj.SetMyFont(m_stTxFont,iFontH);
	m_lstSelPrj.SetMyHeaderFont(m_stTxFont,iFontH,FW_NORMAL);

	m_lstSelPrj.SetColumnText(0,theApp.m_pLangInfo->GetText(29));
	m_lstSelPrj.SetColumnText(1,theApp.m_pLangInfo->GetText(30));
	m_lstSelPrj.SetColumnText(2,theApp.m_pLangInfo->GetText(31));
	ResetPrjManageLstText();

	m_prjManageBaseInfoDlg.RefreshLanguageDisplay();

	return 0;
}

void CPrjManageDlg::InsertOneItemToList(int iIndex, CReagentItem* pItem)
{
	CString strTemp;
	m_lstSelPrj.InsertRowSetHeight(iIndex,54 * m_dVertRatio);
	strTemp.Format(_T("%d"), iIndex+1);
	m_lstSelPrj.SetItemText(iIndex,0,strTemp);
	m_lstSelPrj.SetItemText(iIndex,1,pItem->GetName());

	eExperimentType eExperType = pItem->GetType();
	switch(eExperType)
	{
	case EXP_TYPE_CUSTOM:
		strTemp = theApp.m_pLangInfo->GetText(33);
		break;
	case EXP_TYPE_ABSOLUTE:
		strTemp = theApp.m_pLangInfo->GetText(219);
		break;
	case EXP_TYPE_MELT:
		strTemp = theApp.m_pLangInfo->GetText(220);
		break;
	case EXP_TYPE_RELATIVE:
		strTemp = _T("相对定量");
		break;
	case EXP_TYPE_ALLELE:
		strTemp = _T("等位基因");
		break;
	default:
		strTemp.Empty();
		ASSERT(FALSE);
		break;
	}

	m_lstSelPrj.SetItemText(iIndex,2,strTemp);

	return;
}


void CPrjManageDlg::OnBnClickedNew()
{
	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
		return;

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
		return;

	//如果存在未保存的项目，禁止再次新建项目（新建项目信息的指针默认在数组的尾端）
	int iPrjNum = pAryItem->GetCount();
	if(iPrjNum > 0)
	{
		if(pItemManager->GetBeNewPrj(iPrjNum - 1))
		{
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(166));
			return;
		}
	}

	CReagentItem* pReagentItem = pItemManager->NewOneItem();
	if(NULL == pReagentItem)
	{
		return;
	}

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	CInstancePCR* pInstance = pInstrument->GetAvailableInstance();
	if (pInstance == NULL)
	{
		CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
		pInstrument->CreateInstanceDemo(pPublicInfo->GetUserName());
		pInstance = pInstrument->GetAvailableInstance();
	}

#ifdef NEWPROJECT_INITALL
	pReagentItem->InitReagentItem(&theApp.m_listDyeInfo,pInstance->GetScanChannelInfoArray());

#else
	pReagentItem->InitChannelInfo(&theApp.m_listDyeInfo,pInstance->GetScanChannelInfoArray());
#endif

	int itemCount = m_lstSelPrj.GetRowNum();

	InsertOneItemToList(itemCount, pReagentItem);

	m_lstSelPrj.SetRowSelectState(itemCount,TRUE,TRUE,TRUE);

	SetReagentItem(*pReagentItem);
	pAryItem->Add(pReagentItem);
}

void CPrjManageDlg::OnBnClickedOpen()
{
	CString strOpenFormat, strSuffix;
	strSuffix.Format(_T(".%s"), PCR_ITEM_FILE_SUFFIX);
	strOpenFormat.Format(_T("QPCR Project File(*.%s)|*.%s|QPCR Template File(*.%s)|*.%s||"), PCR_ITEM_FILE_SUFFIX, PCR_ITEM_FILE_SUFFIX, PCR_TEMPLATE_FILE_SUFFIX, PCR_TEMPLATE_FILE_SUFFIX);
	CFileDialog dlgFile(TRUE, strSuffix, NULL, OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT, strOpenFormat, this);
	if(IDOK == dlgFile.DoModal())
	{
		CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
		CReagentItem* pReagentItem = pItemManager->NewOneItem();
		if(NULL == pReagentItem)
			return;

		BOOL bOpenFile = FALSE;
		CString strFileExt = dlgFile.GetFileExt();
		if (strFileExt == PCR_ITEM_FILE_SUFFIX)
		{
			CString strPathName = dlgFile.GetPathName();
			bOpenFile = pReagentItem->OpenFile(strPathName);
		}
		else if (strFileExt == PCR_TEMPLATE_FILE_SUFFIX)
		{
			CString strPathName = dlgFile.GetPathName();
			CExperimentData* pExperiment = new CExperimentData;
			if (pExperiment != NULL)
			{
				bOpenFile = pExperiment->OpenFile(strPathName) == 0;
				if (bOpenFile)
				{
					pReagentItem->CopyFrom(pExperiment->GetReagentItem());
					//if (pReagentItem->GetSegementCount() == 0)
					//{
					//	pReagentItem->SetSegmentInfo(pExperiment->GetSegment());
					//}

					//if (pReagentItem->GetAnalysisPara()->GetAmplifyParaCount() == 0)
					//{
					//	pReagentItem->GetAnalysisPara()->CopyFrom(pExperiment->GetAnalysisPara());
					//}
				}

				delete pExperiment;
				pExperiment = NULL;
			}
		}

		if (bOpenFile)
		{
			int itemCount = m_lstSelPrj.GetRowNum();
			InsertOneItemToList(itemCount, pReagentItem);
			m_lstSelPrj.SetRowSelectState(itemCount,TRUE,TRUE,TRUE);

			SetReagentItem(*pReagentItem);
			pItemManager->GetArrayItems()->Add(pReagentItem);
		}
	}
}

void CPrjManageDlg::OnBnClickedSave()
{
	int iRowNum = m_lstSelPrj.GetRowNum();
	if(iRowNum < 1 || NULL == theApp.m_pLangInfo)
		return;

	vector<int> vecSelectedRowId = m_lstSelPrj.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(276));
		return;
	}

	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
		return;

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if (pAryItem == NULL)
		return;

	CString strTemp(_T(""));	
	if(m_prjManageBaseInfoDlg.m_prjBaseInfoDlg.m_edPrjName.GetText().IsEmpty())
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(277));
		return;
	}

	int iReactVolume = _ttoi(m_prjManageBaseInfoDlg.m_prjBaseInfoDlg.m_edReactSystem.GetText());
	if(iReactVolume < REACTVOLUME_MINVALUE || iReactVolume > REACTVOLUME_MAXVALUE)
	{
		CString strFormat(_T(""));
		strFormat = theApp.m_pLangInfo->GetText(278);
		strTemp.Format(strFormat,REACTVOLUME_MINVALUE,REACTVOLUME_MAXVALUE);
		PopNormalTipInfo(strTemp);
		return;
	}

	CLinkList<CSegItemInfo> lnkSegItemInfo;
	m_prjManageBaseInfoDlg.m_segDlg.GetSegmentInfo(lnkSegItemInfo);
	if(lnkSegItemInfo.GetSize() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(158));
		return;
	}

	int iAmlBaseParamStatus = m_prjManageBaseInfoDlg.IsAmlBaseParamLegal();
	if (iAmlBaseParamStatus == 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(487));
		return;
	}
	else if (iAmlBaseParamStatus == 2)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(488));
		return;
	}

	CReagentItem* pReagentItem = (CReagentItem*)pAryItem->GetAt(vecSelectedRowId[0]);
	if(NULL != pReagentItem)
	{
		GetReagentItem(*pReagentItem);
	}

	m_lstSelPrj.Invalidate(FALSE);

	//同步更新实验-选择项目页面项目列表框
	if(NULL != m_pMainDlg->GetSafeHwnd())
	{
		m_pMainDlg->m_workDlg.SavePrj(vecSelectedRowId[0],pItemManager->GetBeNewPrj(vecSelectedRowId[0]));
	}

	//保存最新项目信息到文件中
	pItemManager->OutputItems(_T(""),NULL);
}

void CPrjManageDlg::OnBnClickedDelete()
{
	int rowNum = m_lstSelPrj.GetRowNum();
	if(rowNum < 1)
		return;

	vector<int> vecSelectedRowId = m_lstSelPrj.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(209));
		return;
	}

	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
	{
		return;
	}

	if (!pItemManager->CanDelete(vecSelectedRowId[0]))
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(210));
		return;
	}

	if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(208)))
	{
		return;
	}

	pItemManager->DeleteItem(vecSelectedRowId[0]);
	m_lstSelPrj.DeleteMultiRow(vecSelectedRowId);	//m_lstSelPrj是 行单选类型

	int iRowNum = m_lstSelPrj.GetRowNum();
	if(0 == iRowNum)
	{
		CReagentItem reagentItem;
		SetReagentItem(reagentItem,FALSE);
		SetPrjEditableState(FALSE,TRUE);
	}
	else 
	{
		CPtrArray* pAryItem = pItemManager->GetArrayItems();
		if(NULL == pAryItem)
		{
			return;
		}

		if(iRowNum > vecSelectedRowId[0])
		{
			m_lstSelPrj.SetRowSelectState(vecSelectedRowId[0],TRUE);
			SetReagentItem(*(CReagentItem*)pAryItem->GetAt(vecSelectedRowId[0]));
			CString strTemp(_T(""));
			for(int i = vecSelectedRowId[0];i < iRowNum;++i)
			{
				strTemp.Format(_T("%d"),i + 1);
				m_lstSelPrj.SetItemText(i,0,strTemp);
			}
		}
		else
		{
			m_lstSelPrj.SetRowSelectState(iRowNum - 1,TRUE);
			SetReagentItem(*(CReagentItem*)pAryItem->GetAt(vecSelectedRowId[0] - 1));
		}
	}
	m_lstSelPrj.Invalidate(FALSE);

	//同步更新实验-选择项目页面项目列表框
	if(!pItemManager->GetBeNewPrj(vecSelectedRowId[0]) && NULL != m_pMainDlg->GetSafeHwnd())
	{
		m_pMainDlg->m_workDlg.DeletePrj(vecSelectedRowId[0]);
	}
}

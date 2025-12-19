// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "UserManageSetDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"
#include "AddUserDlg.h"
#include "ConfigureDB.h"
#include "ConfigureDBInfo.h"
#include "EditUserDlg.h"

// CUserManageSetDlg dialog
IMPLEMENT_DYNAMIC(CUserManageSetDlg, CDialogEx)

CUserManageSetDlg::CUserManageSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserManageSetDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
}

CUserManageSetDlg::~CUserManageSetDlg()
{
}

void CUserManageSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUserManageSetDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CUserManageSetDlg::OnEraseBkgnd(CDC* pDC)
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

	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
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

void CUserManageSetDlg::OnSize(UINT nType,int cx,int cy)
{
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	__super::OnSize(nType,cx,cy);
}	

BOOL CUserManageSetDlg::OnInitDialog()
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

void CUserManageSetDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateEd();
	CreateLst();
}

void CUserManageSetDlg::CreateBt()
{

}

void CUserManageSetDlg::CreateSt()
{
	
}

void CUserManageSetDlg::CreateEd()
{
	
}

void CUserManageSetDlg::CreateLst()
{
	m_lstUserInfo.Create(IDD_MIDDLEDLG,this);
}

void CUserManageSetDlg::InitDlg()
{
	SetTextClr1(RGB(86,86,86));
	SetMyFont(m_stTxFont,14,FW_NORMAL,TYPE_PIXEL);
}

void CUserManageSetDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitLst();
}

void CUserManageSetDlg::InitBt()
{
	
}

void CUserManageSetDlg::InitSt()
{
	
}

void CUserManageSetDlg::InitEd()
{
	
}

void CUserManageSetDlg::InitLst()
{
	int iFontH = 16 * m_dTxRatio;

	m_lstUserInfo.SetHeaderHeight(24 * m_dVertRatio);
	m_lstUserInfo.SetMyHeaderFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstUserInfo.SetScrollArrowWH(4 * m_dHoriRatio,8 * m_dVertRatio);
	m_lstUserInfo.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_lstUserInfo.SetItemHeight(53 * m_dVertRatio);

	int iTempInt1 = 34 * m_dVertRatio;
	m_lstUserInfo.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstUserInfo.m_horiScroll.m_scrollH = iTempInt1;
	m_lstUserInfo.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstUserInfo.m_vertScroll.m_scrollW = iTempInt1;
	m_lstUserInfo.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
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

	InitUserInfo();

	return;
}

int CUserManageSetDlg::InitUserInfo()
{
	CConfigureDB* pUserDB = CConfigureDB::GetInstance();
	if(NULL == pUserDB)
	{
		return -1;
	}

	CUserInfoList listUser;
	if(!pUserDB->SearchUser(&listUser))
	{
		return -2;
	}

	CString strTemp(_T(""));
	int iIndex = 0,iRowH = 53 * m_dVertRatio;
	POSITION pos = listUser.GetHeadPosition();
	while(NULL != pos)
	{
		CDBUserInfo* pUser = listUser.GetNext(pos);
		if(NULL == pUser)
		{
			continue;	
		}

		m_lstUserInfo.InsertRowSetHeight(iIndex,iRowH);
		strTemp.Format(_T("%d"),iIndex + 1);
		m_lstUserInfo.SetItemText(iIndex,0,strTemp);
		m_lstUserInfo.SetItemText(iIndex,1,pUser->GetName());

		if(pUser->GetBeOrdinaryPermission())
		{
			strTemp = theApp.m_pLangInfo->GetText(355);
		}
		else
		{
			strTemp = theApp.m_pLangInfo->GetText(356);
		}
		m_lstUserInfo.SetItemText(iIndex,2,strTemp);

		m_lstUserInfo.SetItemText(iIndex++,3,pUser->GetMnemonic());

		SAFE_DELETE(pUser);
	}
	listUser.RemoveAll();

	return 0;
}

void CUserManageSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	m_lstUserInfo.SetWindowPos(this,16 * m_dHoriRatio,16 * m_dVertRatio,1720 * m_dHoriRatio,840 * m_dVertRatio,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(16 * m_dHoriRatio,16 * m_dVertRatio,1720 * m_dHoriRatio,840 * m_dVertRatio);

	m_rcManage.CalcRectResult();

	return;
}

void CUserManageSetDlg::ShowCtrl()
{
	m_lstUserInfo.ShowWindow(SW_SHOW);
}

void CUserManageSetDlg::SetCtrlText()
{
	m_lstUserInfo.SetColumnText(0,theApp.m_pLangInfo->GetText(29));
	m_lstUserInfo.SetColumnText(1,theApp.m_pLangInfo->GetText(350));
	m_lstUserInfo.SetColumnText(2,theApp.m_pLangInfo->GetText(351));
	m_lstUserInfo.SetColumnText(3,theApp.m_pLangInfo->GetText(352));

	return;
}

void CUserManageSetDlg::AddUser()
{
	if(theApp.GetBeOrdinaryPermission())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(354));
		return;
	}

	CAddUserDlg addUserDlg;
	if(IDOK != addUserDlg.DoModal())
	{
		return;
	}

	CString strTemp(_T(""));
	int iLstRowNum = m_lstUserInfo.GetRowNum();
	strTemp.Format(_T("%d"),iLstRowNum + 1);
	m_lstUserInfo.InsertRowSetHeight(iLstRowNum,53 * m_dVertRatio);
	m_lstUserInfo.SetItemText(iLstRowNum,0,strTemp);
	m_lstUserInfo.SetItemText(iLstRowNum,1,addUserDlg.m_userInfo.GetName());
	if(addUserDlg.m_userInfo.GetBeOrdinaryPermission())
	{
		strTemp = theApp.m_pLangInfo->GetText(355);
	}
	else
	{
		strTemp = theApp.m_pLangInfo->GetText(356);
	}
	m_lstUserInfo.SetItemText(iLstRowNum,2,strTemp);

	m_lstUserInfo.SetItemText(iLstRowNum,3,addUserDlg.m_userInfo.GetMnemonic(),TRUE);

	CConfigureDB* pUserDB = CConfigureDB::GetInstance();
	if(NULL != pUserDB)
	{
		pUserDB->InsertUserRecord(&addUserDlg.m_userInfo);
	}

	return;
}

void CUserManageSetDlg::EditUser()
{
	if(theApp.GetBeOrdinaryPermission())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(373));
		return;
	}

	int iLstRowNum = m_lstUserInfo.GetRowNum();
	if(iLstRowNum < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(374));
		return;
	}

	vector<int> vecSelectedRow = m_lstUserInfo.GetSelectedRowId();
	if(vecSelectedRow.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(375));
		return;
	}

	if(vecSelectedRow.size() > 1)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(376));
		return;
	}

	int iCurSelIndex = vecSelectedRow[0];

	CConfigureDB* pUserDB = CConfigureDB::GetInstance();
	if(NULL == pUserDB)
	{
		return;
	}

	CDBUserInfo userInfo;
	if(!pUserDB->SearchUser(m_lstUserInfo.GetItemText(iCurSelIndex,1),userInfo))
	{
		return;
	}

	CEditUserDlg editUserDlg;
	editUserDlg.m_userInfo = userInfo;
	if(IDOK != editUserDlg.DoModal())
	{
		return;
	}
	
	if(editUserDlg.IsUserInfoChanged())
	{
		CString strTemp(_T(""));
		m_lstUserInfo.SetItemText(iCurSelIndex,1,editUserDlg.m_newUserInfo.GetName());
		if(editUserDlg.m_newUserInfo.GetBeOrdinaryPermission())
		{
			strTemp = theApp.m_pLangInfo->GetText(355);
		}
		else
		{
			strTemp = theApp.m_pLangInfo->GetText(356);
		}
		m_lstUserInfo.SetItemText(iCurSelIndex,2,strTemp);
		m_lstUserInfo.SetItemText(iCurSelIndex,3,editUserDlg.m_newUserInfo.GetMnemonic(),TRUE);

		CConfigureDB* pUserDB = CConfigureDB::GetInstance();
		if(NULL != pUserDB)
		{
			pUserDB->UpdateUserRecord(&editUserDlg.m_newUserInfo);
		}
	}

	return;
}

void CUserManageSetDlg::DeleteUser()
{
	if(theApp.GetBeOrdinaryPermission())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(364));
		return;
	}

	int iLstRowNum = m_lstUserInfo.GetRowNum();
	if(iLstRowNum < 1)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(365));
		return;
	}

	vector<int> vecSelectedRow = m_lstUserInfo.GetSelectedRowId();
	if(vecSelectedRow.size() < 1)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(366));
		return;
	}

	if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(367)))
	{
		return;
	}

	for(int i = 0;i < vecSelectedRow.size();++i)
	{
		CDBUserInfo dbUserInfo;
		dbUserInfo.SetName(m_lstUserInfo.GetItemText(vecSelectedRow[i],1));
		CConfigureDB* pUserDB = CConfigureDB::GetInstance();
		if(NULL != pUserDB)
		{
			pUserDB->DeleteUserRecord(&dbUserInfo);
		}
	}

	m_lstUserInfo.DeleteMultiRow(vecSelectedRow);
	iLstRowNum = m_lstUserInfo.GetRowNum();
	CString strTemp(_T(""));
	for(int i = vecSelectedRow[0];i < iLstRowNum;++i)
	{
		strTemp.Format(_T("%d"),i + 1);
		m_lstUserInfo.SetItemText(i,0,strTemp);
	}
	m_lstUserInfo.Invalidate(FALSE);

	return;
}

void CUserManageSetDlg::ResetLstColumnText()
{
	m_lstUserInfo.SetColumnText(0,theApp.m_pLangInfo->GetText(29));
	m_lstUserInfo.SetColumnText(1,theApp.m_pLangInfo->GetText(350));
	m_lstUserInfo.SetColumnText(2,theApp.m_pLangInfo->GetText(351));
	m_lstUserInfo.SetColumnText(3,theApp.m_pLangInfo->GetText(352));

	return;
}

int CUserManageSetDlg::RefreshLanguageDisplay()
{
	int iFontH = 16 * m_dTxRatio;

	m_lstUserInfo.SetMyHeaderFont(m_stTxFont,iFontH);
	m_lstUserInfo.SetMyFont(m_stTxFont);
	ResetLstColumnText();

	return 0;
}





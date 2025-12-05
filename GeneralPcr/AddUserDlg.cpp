#include "stdafx.h"

#include "AddUserDlg.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "ConfigureDB.h"

// CAddUserDlg dialog
IMPLEMENT_DYNAMIC(CAddUserDlg, CDialogEx)

CAddUserDlg::CAddUserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddUserDlg::IDD, pParent)
{
	m_iWndW = 390;
	m_iWndH = 460;
	m_iFrameW = 1;
	m_iRoundAngle = 10;
	m_pCkbtCurUserType = NULL;
	m_iCapH = 60;
}

CAddUserDlg::~CAddUserDlg()
{
}

void CAddUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAddUserDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND() 
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_ADDUSERDLG_BT_SAVE,OnBnClickedSave)
	ON_BN_CLICKED(ID_ADDUSERDLG_BT_CANCEL,OnBnClickedCancel)
	ON_BN_CLICKED(ID_ADDUSERDLG_CKBT_GENERALUSER,OnBnClickedCkbtGeneralUser)
	ON_BN_CLICKED(ID_ADDUSERDLG_CKBT_ADMINUSER,OnBnClickedCkbtAdminUser)
END_MESSAGE_MAP()

BOOL CAddUserDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0 * dHoriRatio,0 * dVertRatio,m_iWndW,m_iWndH,SWP_NOZORDER);
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

void CAddUserDlg::CreateCtrl()
{
	CreateBt();
	CreateCkBt();
	CreateSt();
	CreateEd();
}

void CAddUserDlg::CreateBt()
{
	m_btSave.Create(_T("保存"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ADDUSERDLG_BT_SAVE);
	m_btCancel.Create(_T("取消"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ADDUSERDLG_BT_CANCEL);
}

void CAddUserDlg::CreateCkBt()
{
	m_ckbtGeneralUser.Create(_T("普通用户"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ADDUSERDLG_CKBT_GENERALUSER);
	m_ckbtAdminUser.Create(_T("管理员用户"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_ADDUSERDLG_CKBT_ADMINUSER);
}

void CAddUserDlg::CreateSt()
{

}

void CAddUserDlg::CreateEd()
{
	m_edUserName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_ADDUSERDLG_ED_USERNAME);
	m_edMnemonic.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_ADDUSERDLG_ED_MNEMONIC);
	m_edCode.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_ADDUSERDLG_ED_CODE);
	m_edReCode.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_ADDUSERDLG_ED_RECODE);
}

void CAddUserDlg::InitDlg()
{
	SetTextClr1(RGB(86,86,86));
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CAddUserDlg::InitCtrl()
{
	InitBt();
	InitCkBt();
	InitSt();
	InitEd();
}

void CAddUserDlg::InitBt()
{
	int iFontH = 16 * m_dTxRatio;

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

	return;
}

void CAddUserDlg::InitCkBt()
{
	int iFontH = 16 * m_dTxRatio;

	m_ckbtGeneralUser.SetPngBkPic(_T(".\\BkPic\\singleselect.png"),4,14 * m_dHoriRatio);
	m_ckbtGeneralUser.SetTextColor(RGB(0X60,0X62,0X66),CLR_SOFT_THEME);
	m_ckbtGeneralUser.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_ckbtGeneralUser.SetBeNeedRedrawBk(TRUE);
	m_ckbtGeneralUser.SetBeParentProcessMouseClick(TRUE);
	m_ckbtGeneralUser.SetMyFontChecked(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);

	m_ckbtAdminUser.SetPngBkPic(_T(".\\BkPic\\singleselect.png"),4,14 * m_dHoriRatio);
	m_ckbtAdminUser.SetTextColor(RGB(0X60,0X62,0X66),CLR_SOFT_THEME);
	m_ckbtAdminUser.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_ckbtAdminUser.SetBeNeedRedrawBk(TRUE);
	m_ckbtAdminUser.SetBeParentProcessMouseClick(TRUE);
	m_ckbtAdminUser.SetMyFontChecked(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	if(!theApp.GetBeAdminPermission())
	{
		m_ckbtAdminUser.EnableWindow(FALSE);
	}

	//默认为普通用户
	m_ckbtGeneralUser.SetBtChecked(TRUE);
	m_ckbtGeneralUser.EnableWindow(FALSE);
	m_pCkbtCurUserType = &m_ckbtGeneralUser;

	return;
}

void CAddUserDlg::InitSt()
{

}

void CAddUserDlg::InitEd()
{
	InitOneEdit(&m_edUserName);
	InitOneEdit(&m_edMnemonic);
	InitOneEdit(&m_edCode);
	InitOneEdit(&m_edReCode);

	return;
}

void CAddUserDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWndW) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndH) / 2;
	rcUseful.right = rcUseful.left + m_iWndW;
	rcUseful.bottom = rcUseful.top + m_iWndH;
#endif

	int iX = rcUseful.left + 120 * m_dHoriRatio;
	int iH = 40 * m_dVertRatio,iW = 240 * m_dHoriRatio;

	if(theApp.GetBeEnglish())
	{
		iX += 10 * m_dHoriRatio;
		iW -= 10 * m_dHoriRatio;
	}

	m_edUserName.SetWindowPos(this,iX,rcUseful.top + 90 * m_dVertRatio,iW,iH,SWP_NOZORDER);
	m_ckbtGeneralUser.SetWindowPos(this,iX,rcUseful.top + 146 * m_dVertRatio,iW,22 * m_dVertRatio,SWP_NOZORDER);
	m_ckbtAdminUser.SetWindowPos(this,iX,rcUseful.top + 184 * m_dVertRatio,iW,22 * m_dVertRatio,SWP_NOZORDER);
	m_edMnemonic.SetWindowPos(this,iX,rcUseful.top + 222 * m_dVertRatio,iW,iH,SWP_NOZORDER);
	m_edCode.SetWindowPos(this,iX,rcUseful.top + 278 * m_dVertRatio,iW,iH,SWP_NOZORDER);
	m_edReCode.SetWindowPos(this,iX,rcUseful.top + 334 * m_dVertRatio,iW,iH,SWP_NOZORDER);

	m_btSave.SetWindowPos(this,rcUseful.left + 156 * m_dHoriRatio,rcUseful.top + 402 * m_dVertRatio,90 * m_dHoriRatio,iH,SWP_NOZORDER);
	m_btCancel.SetWindowPos(this,rcUseful.left + 270 * m_dHoriRatio,rcUseful.top + 402 * m_dVertRatio,90 * m_dHoriRatio,iH,SWP_NOZORDER);
}

void CAddUserDlg::ShowCtrl()
{
	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);
	m_edUserName.ShowWindow(SW_SHOW);
	m_edMnemonic.ShowWindow(SW_SHOW);
	m_ckbtGeneralUser.ShowWindow(SW_SHOW);
	m_ckbtAdminUser.ShowWindow(SW_SHOW);
	m_edCode.ShowWindow(SW_SHOW);
	m_edReCode.ShowWindow(SW_SHOW);
}

void CAddUserDlg::SetCtrlText()
{
	m_btSave.SetWindowText(theApp.m_pLangInfo->GetText(148));
	m_btCancel.SetWindowText(theApp.m_pLangInfo->GetText(150));

	m_ckbtGeneralUser.SetWindowText(theApp.m_pLangInfo->GetText(355));
	m_ckbtAdminUser.SetWindowText(theApp.m_pLangInfo->GetText(356));

	return;
}

int CAddUserDlg::InitOneEdit(CMyNewEdit* pEdit)
{
	if(NULL == pEdit)
	{
		return -1;
	}

	int iFontH = 16 * m_dTxRatio;

	pEdit->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEdit->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEdit->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEdit->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	pEdit->SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	pEdit->SetTextClr1(RGB(0X56,0X56,0X56));

	return 0;
}

int CAddUserDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int iRet = CDialogEx::OnCreate(lpCreateStruct);

	m_iWndW *= m_dHoriRatio;
	m_iWndH *= m_dVertRatio;
	m_iCapH *= m_dVertRatio;

	return iRet;
}

BOOL CAddUserDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC memDc,memDc2;
	memDc.CreateCompatibleDC(pDC);
	memDc2.CreateCompatibleDC(pDC);

	CBitmap bitMapCB,bitMapCB2;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	bitMapCB2.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);
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

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),rcUseful.Height(),m_iRoundAngle,m_iRoundAngle,m_iRoundAngle,m_iRoundAngle,FALSE,Color(197,197,197),m_iFrameW,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCapH,m_iRoundAngle,m_iRoundAngle,0,0,FALSE,Color(3,91,199),2,TRUE,COLOR_SOFT_THEME,1);

	memDc.SetBkMode(TRANSPARENT);
	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);

	int iTempInt1 = rcUseful.left + 20 * m_dHoriRatio,iTempInt2 = rcUseful.left + 120 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iTempInt2 += 10 * m_dHoriRatio;
	}

	memDc.DrawText(theApp.m_pLangInfo->GetText(350),CRect(iTempInt1,rcUseful.top + 90 * m_dVertRatio,iTempInt2,rcUseful.top + 130 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(theApp.m_pLangInfo->GetText(351),CRect(iTempInt1,rcUseful.top + 146 * m_dVertRatio,iTempInt2,rcUseful.top + 176 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(theApp.m_pLangInfo->GetText(352),CRect(iTempInt1,rcUseful.top + 222 * m_dVertRatio,iTempInt2,rcUseful.top + 264 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(theApp.m_pLangInfo->GetText(357),CRect(iTempInt1,rcUseful.top + 278 * m_dVertRatio,iTempInt2,rcUseful.top + 318 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDc.DrawText(theApp.m_pLangInfo->GetText(358),CRect(iTempInt1,rcUseful.top + 334 * m_dVertRatio,iTempInt2,rcUseful.top + 374 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(theApp.m_pLangInfo->GetText(359),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapH - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	memDc.SelectObject(pOldFont);

	pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	memDc2.SelectObject(&pOldBitmapCB2);
	bitMapCB2.DeleteObject();
	memDc2.DeleteDC();

	return TRUE;
}

void CAddUserDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < 38)
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

void CAddUserDlg::OnBnClickedSave()
{
	CString strTemp(_T("")),strTemp2(_T(""));

	strTemp = m_edUserName.GetText();
	if(strTemp.IsEmpty())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(360));
		return;
	}
	BOOL bFind = FALSE;
	CConfigureDB* pDb = CConfigureDB::GetInstance();
	if(NULL != pDb && pDb->FindUserRecord(strTemp))
	{
		bFind = TRUE;
	}
	if(!bFind)
	{
		if(_T("admin") == strTemp || _T("user") == strTemp || _T("BWRD") == strTemp)
		{
			bFind = TRUE;
		}
	}
	if(bFind)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(363));
		return;
	}
	m_userInfo.SetName(strTemp);

	strTemp = m_edCode.GetText();
	if(strTemp.IsEmpty())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(361));
		return;
	}
	if(strTemp.GetLength() < PASSWORD_LEN_MIN)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(378));
		return;
	}

	strTemp2 = m_edReCode.GetText();
	if(strTemp != strTemp2)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(362));
		return;
	}
	m_userInfo.SetPassword(strTemp);

	m_userInfo.SetCreateTime(GetCurrentTmTime(3));

	if(m_ckbtGeneralUser.GetCheckState())
	{
		m_userInfo.SetPermissionType(EPT_ORDINARY);
	}
	else
	{
		m_userInfo.SetPermissionType(EPT_ADMIN);
	}
	
	m_userInfo.SetMnemonic(m_edMnemonic.GetText());

	OnOK();

	return;
}

void CAddUserDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}

void CAddUserDlg::OnBnClickedCkbtGeneralUser()
{
	if(NULL != m_pCkbtCurUserType->GetSafeHwnd())
	{
		m_pCkbtCurUserType->SetBtChecked(!m_pCkbtCurUserType->GetCheckState());
		m_pCkbtCurUserType->EnableWindow(TRUE);
	}

	m_ckbtGeneralUser.SetBtChecked(TRUE);
	m_ckbtGeneralUser.EnableWindow(FALSE);
	m_pCkbtCurUserType = &m_ckbtGeneralUser;

	return;
}

void CAddUserDlg::OnBnClickedCkbtAdminUser()
{
	if(NULL != m_pCkbtCurUserType->GetSafeHwnd())
	{
		m_pCkbtCurUserType->SetBtChecked(!m_pCkbtCurUserType->GetCheckState());
		m_pCkbtCurUserType->EnableWindow(TRUE);
	}

	m_ckbtAdminUser.SetBtChecked(TRUE);
	m_ckbtAdminUser.EnableWindow(FALSE);
	m_pCkbtCurUserType = &m_ckbtAdminUser;

	return;
}

// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "ExpeInfoDlg.h"
#include "PublicInfo.h"

// CExpeInfoDlg dialog
IMPLEMENT_DYNAMIC(CExpeInfoDlg, CDialogEx)

CExpeInfoDlg::CExpeInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CExpeInfoDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
}

CExpeInfoDlg::~CExpeInfoDlg()
{

}

void CExpeInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CExpeInfoDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_EXPEINFODLG_BT_SELECTDIRECTORY,OnBnClickedSelectPath)
	ON_MESSAGE(WM_TELLPARENTTEXTCHANGED,OnMsgTellParentTextChanged)
END_MESSAGE_MAP()

BOOL CExpeInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap bitMap;
	bitMap.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitMap);

	memDC.FillSolidRect(0,0,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(&memDC,rect.left,rect.top,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	memDC.SetBkMode(TRANSPARENT);
	memDC.FillSolidRect(28 * m_dHoriRatio,33 * m_dVertRatio,12 * m_dHoriRatio,4 * m_dVertRatio,CLR_SOFT_THEME);
	memDC.FillSolidRect(32 * m_dHoriRatio,29 * m_dVertRatio,4 * m_dHoriRatio,12 * m_dVertRatio,CLR_SOFT_THEME);

	CFont* pOldFont = memDC.SelectObject(&m_fontSet);

	memDC.SetTextColor(CLR_SOFT_THEME);

	memDC.DrawText(pLangInfo->GetText(21),CRect(44 * m_dHoriRatio,24 * m_dVertRatio,316 * m_dHoriRatio,46 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	memDC.SetTextColor(RGB(0X39,0X45,0X65));

	int iEndX = 190 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iEndX += 30 * m_dHoriRatio; 
	}

	UINT uiTxFormat = DT_LEFT | DT_VCENTER | DT_SINGLELINE;

	memDC.DrawTextW(pLangInfo->GetText(7),CRect(48 * m_dHoriRatio,138 * m_dVertRatio,iEndX,165 * m_dVertRatio),uiTxFormat);
	memDC.DrawText(pLangInfo->GetText(19),CRect(48 * m_dHoriRatio,218 * m_dVertRatio,iEndX,245 * m_dVertRatio),uiTxFormat);
	memDC.DrawText(pLangInfo->GetText(20),CRect(48 * m_dHoriRatio,298 * m_dVertRatio,iEndX,325 * m_dVertRatio),uiTxFormat);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDC,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDC,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	
	memDC.SelectObject(pOldFont);

	memDC.SelectObject(pOldBitmap);
	bitMap.DeleteObject();
	memDC.DeleteDC();

	return TRUE;
}

void CExpeInfoDlg::OnSize(UINT nType,int cx,int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	return;
}

void CExpeInfoDlg::OnBnClickedSelectPath()
{
	CPublicInfo* pInfo = CPublicInfo::GetInstance();
	CFolderPickerDialog folderDlg(pInfo->GetUserDataSavePath(),0,this,0);

	if(IDOK == folderDlg.DoModal())
	{
		CString selPath = folderDlg.GetPathName();
		m_edSavePath.SetText(selPath);
	}

	return;
}

LRESULT CExpeInfoDlg::OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam)
{
	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	if(NULL == pMainDlg->m_workDlg.GetSafeHwnd())
	{
		return 1;
	}

	CString strTemp(_T(""));
	strTemp = m_edExpeName.GetText();

	CMiddleDlg* pMiddlgDlg = pMainDlg->m_workDlg.m_pCurSelPageDlg;
	if(NULL != pMiddlgDlg->GetSafeHwnd() && NULL != pMiddlgDlg->m_pExpData)
	{
		if(CExperimentData::STATUS_NEW == pMiddlgDlg->m_pExpData->GetStatus())
		{
			//新建的实验 实验名称和文件名称 保持一致
			pMainDlg->m_workDlg.m_fileManageDlg.SetExperName(pMainDlg->m_workDlg.m_pCurSelPageDlg->GetPageId(),strTemp);
		}
	}

	return 2;
}

BOOL CExpeInfoDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

	SetMyFont(m_stTxFont,18 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);

	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();

	return TRUE;
}

void CExpeInfoDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCb();
	CreateEd();
}

void CExpeInfoDlg::CreateBt()
{
	m_btSelectDir.Create(_T("浏览"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_EXPEINFODLG_BT_SELECTDIRECTORY);
}

void CExpeInfoDlg::CreateSt()
{
	
}

void CExpeInfoDlg::CreateCb()
{
	
}

void CExpeInfoDlg::CreateEd()
{
	CRect rcCreate(0,0,0,0);

	m_edExpeName.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, rcCreate, this,ID_EXPEINFODLG_ED_EXPENAME);
	m_edSavePath.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, rcCreate, this,ID_EXPEINFODLG_ED_SAVEPATH);
	m_edRemark.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate, this,ID_EXPEINFODLG_ST_REMARK);
}

void CExpeInfoDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitCb();
	InitEd();
}

void CExpeInfoDlg::InitBt()
{
	int iFontH = 18 * m_dTxRatio;

	m_btSelectDir.SetMyFont(m_stTxFont,iFontH);
	m_btSelectDir.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CExpeInfoDlg::InitSt()
{
	
}

void CExpeInfoDlg::InitCb()
{
	
}

void CExpeInfoDlg::InitEd()
{
	int iFontH = 18 * m_dTxRatio;

	InitOneEd(&m_edExpeName);
	InitOneEd(&m_edSavePath);
	InitOneEd(&m_edRemark);

	return;
}

int CExpeInfoDlg::InitOneEd(CMyNewEdit* pEdit)
{
	if(NULL == pEdit)
	{
		return -1;
	}

	int iFontH = 18 * m_dTxRatio;

	pEdit->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEdit->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEdit->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEdit->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	pEdit->SetMyFont(m_stTxFont,iFontH);
	pEdit->SetTellParentTxChangedParam(TRUE,0);

	return 0;
}

void CExpeInfoDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcInit = rect;
	ClearRectToDelete();

	int iStartX = 192 * m_dHoriRatio;
	int iTempInt1 = 688 * m_dHoriRatio;
	int iTempInt2 = 40 * m_dVertRatio;
	int iTempInt3 = 160 * m_dVertRatio;
	int iTempInt4 = 480 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iStartX += 30 * m_dHoriRatio;
		iTempInt1 += 30 * m_dHoriRatio;
	}

	m_btSelectDir.SetWindowPos(this,iTempInt1,211 * m_dVertRatio,80 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);
	AddRectToDelete(iTempInt1,211 * m_dVertRatio,80 * m_dHoriRatio,iTempInt2);

	m_edExpeName.SetWindowPos(this,iStartX,131 * m_dVertRatio,iTempInt4,iTempInt2,SWP_NOZORDER);
	AddRectToDelete(iStartX,131 * m_dVertRatio,iTempInt4,iTempInt2);

	m_edSavePath.SetWindowPos(this,iStartX,211 * m_dVertRatio,iTempInt4,iTempInt2,SWP_NOZORDER);
	AddRectToDelete(iStartX,211 * m_dVertRatio,iTempInt4,iTempInt2);

	m_edRemark.SetWindowPos(this,iStartX,291 * m_dVertRatio,iTempInt4,iTempInt2,SWP_NOZORDER);
	AddRectToDelete(iStartX,291 * m_dVertRatio,iTempInt4,iTempInt2);

	CalcRectResult();

	return;
}

void CExpeInfoDlg::ShowCtrl()
{
	m_btSelectDir.ShowWindow(SW_SHOW);
	m_edExpeName.ShowWindow(SW_SHOW);
	m_edSavePath.ShowWindow(SW_SHOW);
	m_edRemark.ShowWindow(SW_SHOW);
}

void CExpeInfoDlg::SetCtrlText()
{
	m_btSelectDir.SetWindowText(theApp.m_pLangInfo->GetText(181));

	return;
}

void CExpeInfoDlg::GetExperiInfo(CExperimentInfo& experiInfo)
{
	experiInfo.SetExpName(m_edExpeName.GetText());
	experiInfo.SetFilePath(m_edSavePath.GetText());
	experiInfo.SetComments(m_edRemark.GetText()) ;
}

void CExpeInfoDlg::SetExperiInfo(CExperimentInfo& experiInfo)
{
	m_edExpeName.SetText(experiInfo.GetExpName());
	m_edSavePath.SetText(experiInfo.GetFilePath());
	m_edRemark.SetText(experiInfo.GetComments());

	return;
}

void CExpeInfoDlg::SetCtrlEnableState(BOOL bEnabled)
{
	m_btSelectDir.EnableWindow(bEnabled);
	m_edExpeName.SetBeEditable(bEnabled);
	m_edSavePath.SetBeEditable(bEnabled);
	m_edRemark.SetBeEditable(bEnabled);
}

void CExpeInfoDlg::ClearPage()
{
	m_edExpeName.SetText(_T(""));
	m_edSavePath.SetText(_T(""));
	m_edRemark.SetText(_T(""));
}

int CExpeInfoDlg::RefreshLanguageDisplay()
{
	int iFontH = 18 * m_dTxRatio;

	SetMyFont(m_stTxFont);

	m_btSelectDir.SetMyFont(m_stTxFont,iFontH);
	m_btSelectDir.SetWindowText(theApp.m_pLangInfo->GetText(181));

	m_edExpeName.SetMyFont(m_stTxFont,iFontH);
	m_edExpeName.TextParamChanged();

	m_edSavePath.SetMyFont(m_stTxFont,iFontH);
	m_edSavePath.TextParamChanged();

	m_edRemark.SetMyFont(m_stTxFont,iFontH);
	m_edRemark.TextParamChanged();

	return 0;
}

int CExpeInfoDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	if(EEEM_RUNNING == m_eExpEditMode)
	{
		m_edExpeName.SetBeEditable(FALSE,TRUE);
		m_edSavePath.SetBeEditable(FALSE,TRUE);
		m_edRemark.SetBeEditable(FALSE,TRUE);
		m_btSelectDir.EnableWindow(FALSE);
	}
	else if(EEEM_END == m_eExpEditMode)
	{
		m_edExpeName.SetBeEditable(TRUE,TRUE);
		m_edSavePath.SetBeEditable(TRUE,TRUE);
		m_edRemark.SetBeEditable(TRUE,TRUE);
		m_btSelectDir.EnableWindow(TRUE);
	}

	return 0;
}

CString CExpeInfoDlg::GetExpName()
{
	return m_edExpeName.GetText();
}

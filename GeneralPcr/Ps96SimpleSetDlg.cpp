// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "Ps96SimpleSetDlg.h"
#include "GeneralPcr.h"
#include "PublicInfo.h"
#include "Ps96AddTemplateDlg.h"

// CPs96SimpleSetDlg dialog
IMPLEMENT_DYNAMIC(CPs96SimpleSetDlg, CDialogEx)

CPs96SimpleSetDlg::CPs96SimpleSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPs96SimpleSetDlg::IDD, pParent)
{
	m_iAngleRadius = 20;
	m_iFrameWidth = 1;
	m_iWndWidth = 900;
	m_iWndHeight = 760;
	m_iCapHeight = 60;

	m_pBackupData = NULL;
}

CPs96SimpleSetDlg::~CPs96SimpleSetDlg()
{
	SAFE_DELETE(m_pBackupData);
}

void CPs96SimpleSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPs96SimpleSetDlg, CDialogEx)
	ON_WM_ERASEBKGND() 
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(ID_PS96SIMPLESETDLG_CKBT_RSTHORI,OnBnClickedCkbtExportRstHori)
	ON_BN_CLICKED(ID_PS96SIMPLESETDLGG_CKBT_RSTVERT,OnBnClickedCkbtExportRstVert)
	ON_BN_CLICKED(ID_PS96SIMPLESETDLG_BT_SAVE,OnBnClickedSave)
	ON_BN_CLICKED(ID_PS96SIMPLESETDLG_BT_CANCEL,OnBnClickedCancel)
	ON_BN_CLICKED(ID_SIMPLESET_BT_ADDTEMPLATE,OnBnClickedAddTemplate)
	ON_BN_CLICKED(ID_SIMPLESET_BT_EDITTEMPLATE,OnBnClickedEditTemplate)
	ON_BN_CLICKED(ID_SIMPLESET_BT_DELETETEMPLATE,OnBnClickedDeleteTemplate)
	ON_BN_CLICKED(ID_SIMPLESET_BT_IMPORTTEMPLATE,OnBnClickedImportTemplate)
	ON_BN_CLICKED(ID_SIMPLESET_BT_EXPORTTEMPLATE,OnBnClickedExportTemplate)
END_MESSAGE_MAP()

BOOL CPs96SimpleSetDlg::OnEraseBkgnd(CDC* pDC)
{
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
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;

	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top + m_iCapHeight,rcUseful.Width(), rcUseful.Height() - m_iCapHeight,0,0,m_iAngleRadius,m_iAngleRadius,FALSE,Color(197,197,197),m_iFrameWidth,TRUE,Color(255,255,255),1);
	DrawRoundRectangleNew(&memDc,rcUseful.left,rcUseful.top,rcUseful.Width(),m_iCapHeight,m_iAngleRadius,m_iAngleRadius,0,0,FALSE,COLOR_SOFT_THEME,m_iFrameWidth,TRUE,COLOR_SOFT_THEME,1);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	DWORD dwTxStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
	
	int iX1 = rcUseful.left + 20 * m_dHoriRatio;
	memDc.DrawText(theApp.m_pLangInfo->GetText(394), CRect(iX1, int(rcUseful.top + 70*m_dVertRatio), int(rcUseful.left + 350*m_dHoriRatio), int(rcUseful.top + 150*m_dVertRatio)), dwTxStyle);
	//memDc.DrawText(theApp.m_pLangInfo->GetText(393),CRect(iX1,rcUseful.top + 130*m_dVertRatio,rcUseful.left + 150 * m_dHoriRatio,rcUseful.top + 200 * m_dVertRatio),dwTxStyle);

	memDc.SetTextColor(RGB(255,255,255));
	memDc.DrawText(theApp.m_pLangInfo->GetText(293),CRect(rcUseful.left + 10,rcUseful.top + 5,rcUseful.left + rcUseful.Width() - 10,rcUseful.top + m_iCapHeight - 5),DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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

int CPs96SimpleSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_iWndWidth *= m_dHoriRatio;
	m_iWndHeight *= m_dVertRatio;
	m_iCapHeight *= m_dVertRatio;
	return CDialogEx::OnCreate(lpCreateStruct);
}

void CPs96SimpleSetDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
#ifdef POPDLG_USESHADEBK
	CDialogEx::OnLButtonDown(nFlags, point);
#else
	if(point.y < m_iCapHeight)
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

BOOL CPs96SimpleSetDlg::OnInitDialog()
{ 
	CDialogEx::OnInitDialog();

#ifdef POPDLG_USESHADEBK
	SetWindowPos(GetParent(),theApp.m_rcWorkArea.left,theApp.m_rcWorkArea.top,theApp.m_rcWorkArea.Width(),theApp.m_rcWorkArea.Height(),SWP_NOZORDER);
#else
	SetWindowPos(GetParent(),0,0,m_iWndWidth,m_iWndHeight,SWP_NOZORDER);
	SetRoundRect(m_iAngleRadius,m_iAngleRadius);
	CenterWindow(GetParent());
#endif

	m_pBackupData = new CReportTemplateData;
	CPublicInfo::GetInstance()->GetReportTemplateData()->CopyTo(m_pBackupData);

	CreateCtrl();
	InitDlg();
	InitCtrl();
	SetCtrlPos();

	SetCtrlText();

	ShowCtrl();

	return TRUE;
}

void CPs96SimpleSetDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateCkbt();

	m_lstReportTemplate.Create(IDD_MIDDLEDLG,this);
}

void CPs96SimpleSetDlg::CreateBt()
{
	m_btSave.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLG_BT_SAVE);
	m_btCancel.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLG_BT_CANCEL);

	m_btNewTemplate.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SIMPLESET_BT_ADDTEMPLATE);
	m_btEditTemplate.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SIMPLESET_BT_EDITTEMPLATE);
	m_btDeleteTemplate.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SIMPLESET_BT_DELETETEMPLATE);
	m_btImportTemplate.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SIMPLESET_BT_IMPORTTEMPLATE);
	m_btExportTemplate.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SIMPLESET_BT_EXPORTTEMPLATE);
}

void CPs96SimpleSetDlg::CreateSt()
{
	
}

void CPs96SimpleSetDlg::CreateCkbt()
{
	m_ckbtRstHori.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLG_CKBT_RSTHORI);
	m_ckbtRstVert.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLESETDLGG_CKBT_RSTVERT);
}

void CPs96SimpleSetDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,30 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CPs96SimpleSetDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitEd();
	InitCkbt();
	InitLst();
}

void CPs96SimpleSetDlg::InitBt()
{
	int iFontH = 26 * m_dTxRatio;

	m_btSave.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btSave.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	m_btSave.SetBkColor(Color(0X38,0XE0,0XEF),Color(0X3A,0XEF,0XFF),COLOR_SOFT_THEME,Color(0XF1,0XF1,0XF1));
	m_btSave.SetBkColorEnd(COLOR_SOFT_THEME,Color(0X38,0XE0,0XEF),Color(0X02,0X8D,0X99),Color(0XD8,0XD8,0XD8));
	m_btSave.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XC6,0XC4,0XC4));

	m_btCancel.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btCancel.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btNewTemplate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btNewTemplate.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btEditTemplate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btEditTemplate.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btDeleteTemplate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btDeleteTemplate.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btImportTemplate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btImportTemplate.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btExportTemplate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_btExportTemplate.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
}

void CPs96SimpleSetDlg::InitSt()
{
	
}

void CPs96SimpleSetDlg::InitEd()
{
}

void CPs96SimpleSetDlg::InitCkbt()
{
	InitOneCkbt(&m_ckbtRstHori);
	InitOneCkbt(&m_ckbtRstVert);

	BOOL bData = theApp.m_ps96IvdSet.IsExportResultHorizontal();
	m_ckbtRstHori.SetBtChecked(bData);
	m_ckbtRstVert.SetBtChecked(!bData);
	if(bData)
	{
		m_ckbtRstHori.EnableWindow(FALSE);
	}
	else
	{
		m_ckbtRstVert.EnableWindow(FALSE);
	}

	return;
}

void CPs96SimpleSetDlg::InitOneCkbt(CMyCheckButton* pCkbt)
{
	if(NULL == pCkbt)
	{
		return;
	}

	int iFontH = 26 * m_dTxRatio;

	pCkbt->SetPngBkPic(_T(".\\BkPic\\singleselect2.png"),4,28 * m_dHoriRatio);
	pCkbt->SetIconTextInterval(6 * m_dHoriRatio);
	pCkbt->SetBeNeedRedrawBk(TRUE);
	pCkbt->SetMyFont(m_stTxFont,iFontH);
	pCkbt->SetMyFontChecked(m_stTxFont,iFontH);
	pCkbt->SetBeParentProcessMouseClick(TRUE);

	return;
}


void CPs96SimpleSetDlg::InitLst()
{
	int iFontH = 26 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 27 * m_dTxRatio;
	}

	m_lstReportTemplate.InsertColumn(0,theApp.m_pLangInfo->GetText(28),FALSE);
	m_lstReportTemplate.InsertColumn(1,theApp.m_pLangInfo->GetText(29),FALSE);
	m_lstReportTemplate.InsertColumn(2,theApp.m_pLangInfo->GetText(296),FALSE);

	m_lstReportTemplate.SetHeaderHeight(70 * m_dVertRatio);
	int iTempInt1 = 34 * m_dVertRatio;
	m_lstReportTemplate.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstReportTemplate.m_horiScroll.m_scrollH = iTempInt1;
	m_lstReportTemplate.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstReportTemplate.m_vertScroll.m_scrollW = iTempInt1;

	m_lstReportTemplate.SetMyTitleFont(m_stTxFont,iFontH,FW_MEDIUM,TYPE_PIXEL);

	m_lstReportTemplate.SetLargeCheckIconUseState(TRUE);
	m_lstReportTemplate.SetBeSingleSelect(TRUE);
	m_lstReportTemplate.SetBeAllowCancelRowSelect(TRUE);
	m_lstReportTemplate.SetBeTellParentRowSelChange(TRUE);
	m_lstReportTemplate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstReportTemplate.SetTextClr1(RGB(0X39,0X45,0X65));
	m_lstReportTemplate.SetMyHeaderFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_lstReportTemplate.SetBeSingleSelect(TRUE);
	m_lstReportTemplate.SetColumnCheckFunc(0,TRUE);
	m_lstReportTemplate.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_lstReportTemplate.SetItemHeight(60 * m_dVertRatio);
	m_lstReportTemplate.SetRowBkSelectedColor(RGB(0XF1,0XF1,0XF1));

	int tempInt1 = 0,tempInt2 = 668 * m_dHoriRatio;
	tempInt1 = 150 * m_dHoriRatio;
	m_lstReportTemplate.SetColumnWidth(0,tempInt1);
	tempInt2 -= tempInt1;
	m_lstReportTemplate.SetColumnWidth(1,tempInt1);
	tempInt2 -= tempInt1;
	tempInt2 -= m_lstReportTemplate.GetFrameLineWidth()*2;
	m_lstReportTemplate.SetColumnWidth(2,tempInt2);

	SetListTemplate();

	return;
}


int CPs96SimpleSetDlg::SetListTemplate()
{
	m_lstReportTemplate.Clear();
	CString strTemp(_T(""));

	int iTemNum = m_pBackupData->GetTemplateCount();
	for(int i = 0;i < iTemNum;++i)
	{
		m_lstReportTemplate.InsertRowSetHeight(i, 60 * m_dVertRatio);
		strTemp.Format(_T("%d"),i + 1);
		m_lstReportTemplate.SetItemText(i, 1, strTemp);
		m_lstReportTemplate.SetItemText(i, 2, m_pBackupData->GetStringBy(CReportTemplateData::DATA_NAME, i));
	}

	return 0;
}

void CPs96SimpleSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	CRect rcUseful;
#ifdef POPDLG_USESHADEBK
	rcUseful.left = rect.left + (rect.Width() - m_iWndWidth) / 2;
	rcUseful.top = rect.top + (rect.Height() - m_iWndHeight) / 2;
	rcUseful.right = rcUseful.left + m_iWndWidth;
	rcUseful.bottom = rcUseful.top + m_iWndHeight;
#endif

	int iBtnHeight = int(60 * m_dVertRatio);
	int iBtnWidth = int(160 * m_dVertRatio);
	int iStartY = int(80 * m_dVertRatio);
	int iSpace = int(15 * m_dVertRatio);

	m_ckbtRstHori.SetWindowPos(this,rcUseful.left + 360 * m_dHoriRatio, rcUseful.top + iStartY, iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_ckbtRstVert.SetWindowPos(this,rcUseful.left + 540 * m_dHoriRatio, rcUseful.top + iStartY, iBtnWidth, iBtnHeight,SWP_NOZORDER);

	iStartY += iBtnHeight + iSpace;
	int iListWidth = int (668 * m_dHoriRatio);
	int iListHeight = int (485 * m_dVertRatio);
	m_lstReportTemplate.SetWindowPos(this, int(rcUseful.left + 16*m_dHoriRatio), rcUseful.top + iStartY, iListWidth, iListHeight,SWP_NOZORDER);

	m_btNewTemplate.SetWindowPos(this, int(rcUseful.left + 700 * m_dHoriRatio), rcUseful.top + iStartY, iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_btEditTemplate.SetWindowPos(this, int(rcUseful.left + 700 * m_dHoriRatio), rcUseful.top + iStartY + iBtnHeight + iSpace, iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_btDeleteTemplate.SetWindowPos(this, int(rcUseful.left + 700 * m_dHoriRatio), rcUseful.top + iStartY + (iBtnHeight + iSpace)*2, iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_btImportTemplate.SetWindowPos(this, int(rcUseful.left + 700 * m_dHoriRatio), rcUseful.top + iStartY + (iBtnHeight + iSpace)*3, iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_btExportTemplate.SetWindowPos(this, int(rcUseful.left + 700 * m_dHoriRatio), rcUseful.top + iStartY + (iBtnHeight + iSpace)*4, iBtnWidth, iBtnHeight,SWP_NOZORDER);

	iStartY += int(530*m_dVertRatio);
	m_btSave.SetWindowPos(this, int(rcUseful.left + 500*m_dHoriRatio), rcUseful.top + iStartY,iBtnWidth, iBtnHeight,SWP_NOZORDER);
	m_btCancel.SetWindowPos(this, int(rcUseful.left + 700 * m_dHoriRatio), rcUseful.top + iStartY, iBtnWidth, iBtnHeight,SWP_NOZORDER);

	return;
}

void CPs96SimpleSetDlg::ShowCtrl()
{
	m_btSave.ShowWindow(SW_SHOW);
	m_btCancel.ShowWindow(SW_SHOW);
	m_ckbtRstHori.ShowWindow(SW_SHOW);
	m_ckbtRstVert.ShowWindow(SW_SHOW);

	m_lstReportTemplate.ShowWindow(SW_SHOW);
	m_btNewTemplate.ShowWindow(SW_SHOW);
	m_btEditTemplate.ShowWindow(SW_SHOW);
	m_btDeleteTemplate.ShowWindow(SW_SHOW);
	m_btImportTemplate.ShowWindow(SW_SHOW);
	m_btExportTemplate.ShowWindow(SW_SHOW);

	return;
}

void CPs96SimpleSetDlg::SetCtrlText()
{
	m_btSave.SetWindowText(theApp.m_pLangInfo->GetText(148));
	m_btCancel.SetWindowText(theApp.m_pLangInfo->GetText(150));
	m_ckbtRstHori.SetWindowText(theApp.m_pLangInfo->GetText(197));
	m_ckbtRstVert.SetWindowText(theApp.m_pLangInfo->GetText(198));

	m_btNewTemplate.SetWindowText(theApp.m_pLangInfo->GetText(501));
	m_btEditTemplate.SetWindowText(theApp.m_pLangInfo->GetText(502));
	m_btDeleteTemplate.SetWindowText(theApp.m_pLangInfo->GetText(503));
	m_btImportTemplate.SetWindowText(theApp.m_pLangInfo->GetText(515));
	m_btExportTemplate.SetWindowText(theApp.m_pLangInfo->GetText(516));

	return;
}

void CPs96SimpleSetDlg::OnBnClickedCkbtExportRstHori()
{
	BOOL bCkbtState = m_ckbtRstHori.GetCheckState();
	m_ckbtRstHori.SetBtChecked(!bCkbtState,TRUE);
	m_ckbtRstHori.EnableWindow(FALSE);

	m_ckbtRstVert.SetBtChecked(bCkbtState,TRUE);
	m_ckbtRstVert.EnableWindow(TRUE);

	return;
}

void CPs96SimpleSetDlg::OnBnClickedCkbtExportRstVert()
{
	BOOL bCkbtState = m_ckbtRstVert.GetCheckState();
	m_ckbtRstVert.SetBtChecked(!bCkbtState,TRUE);
	m_ckbtRstVert.EnableWindow(FALSE);

	m_ckbtRstHori.SetBtChecked(bCkbtState,TRUE);
	m_ckbtRstHori.EnableWindow(TRUE);

	return;
}

void CPs96SimpleSetDlg::OnBnClickedSave()
{
	CPs96IvdSet ivdSet;
	ivdSet.SetResultExportOrder(m_ckbtRstHori.GetCheckState());

	if(ivdSet != theApp.m_ps96IvdSet)
	{
		theApp.m_ps96IvdSet = ivdSet;
		theApp.WritePs96IvdSet();
	}

	if (m_pBackupData->GetChanged())
	{
		CPublicInfo::GetInstance()->GetReportTemplateData()->CopyFrom(m_pBackupData);
		CPublicInfo::GetInstance()->GetReportTemplateData()->SetChanged(TRUE);
		CPublicInfo::GetInstance()->GetReportTemplateData()->SaveFile();
	}

	OnOK();
	return;
}

void CPs96SimpleSetDlg::OnBnClickedCancel()
{
	OnCancel();
	return;
}

BOOL CPs96SimpleSetDlg::SetRoundRect(int x /*= 5*/,int y /*= 5*/)
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

void CPs96SimpleSetDlg::InitEditCtrlParam(CMyNewEdit* pEdit)
{
	if(NULL == pEdit)
	{
		return;
	}

	pEdit->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEdit->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEdit->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEdit->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	pEdit->SetMyFont(m_stTxFont,26 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
	pEdit->SetTextClr1(RGB(0X56,0X56,0X56));

	return;
}



void CPs96SimpleSetDlg::OnBnClickedAddTemplate()
{
	CPs96AddTemplateDlg dlg;
	dlg.m_pReportData = m_pBackupData;
	if (dlg.DoModal() == IDOK)
	{
		int iIndex = m_pBackupData->AddTemplate(CReportTemplateData::DATA_NAME, dlg.m_strName);
		m_pBackupData->SetStringTo(CReportTemplateData::DATA_LAB_NAME, dlg.m_strLabName, iIndex);
		m_pBackupData->SetStringTo(CReportTemplateData::DATA_ADDRESS, dlg.m_strLabAdress, iIndex);
		m_pBackupData->SetStringTo(CReportTemplateData::DATA_CONTACT_URL, dlg.m_strContactUrl, iIndex);
		m_pBackupData->SetStringTo(CReportTemplateData::DATA_CONTACT_EMAIL, dlg.m_strContactEmail, iIndex);
		m_pBackupData->SetStringTo(CReportTemplateData::DATA_CONTACT_PHONE, dlg.m_strContactPhone, iIndex);
		m_pBackupData->SetStringTo(CReportTemplateData::DATA_TEST_NAME, dlg.m_strTestName, iIndex);
		m_pBackupData->SetStringTo(CReportTemplateData::DATA_DISCLAIMER, dlg.m_strDisclaimer, iIndex);
		m_pBackupData->SetChanged(TRUE);

		CString strTemp;
		m_lstReportTemplate.InsertRowSetHeight(iIndex, 70 * m_dVertRatio, TRUE);
		strTemp.Format(_T("%d"), iIndex+1);
		m_lstReportTemplate.SetItemText(iIndex, 1, strTemp);
		m_lstReportTemplate.SetItemText(iIndex, 2, dlg.m_strName);
	}
}

void CPs96SimpleSetDlg::OnBnClickedEditTemplate()
{
	vector<int> vecSelRow = m_lstReportTemplate.GetSelectedRowId();
	if(vecSelRow.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(380));
		return;
	}

	int iSelIndex = vecSelRow[0];

	CPs96AddTemplateDlg dlg;
	dlg.m_pReportData = m_pBackupData;
	dlg.m_bAddTemplage = FALSE;
	dlg.m_strName = m_pBackupData->GetStringBy(CReportTemplateData::DATA_NAME, iSelIndex);
	dlg.m_strLabName = m_pBackupData->GetStringBy(CReportTemplateData::DATA_LAB_NAME, iSelIndex);
	dlg.m_strLabAdress = m_pBackupData->GetStringBy(CReportTemplateData::DATA_ADDRESS, iSelIndex);
	dlg.m_strContactUrl = m_pBackupData->GetStringBy(CReportTemplateData::DATA_CONTACT_URL, iSelIndex);
	dlg.m_strContactEmail = m_pBackupData->GetStringBy(CReportTemplateData::DATA_CONTACT_EMAIL, iSelIndex);
	dlg.m_strContactPhone = m_pBackupData->GetStringBy(CReportTemplateData::DATA_CONTACT_PHONE, iSelIndex);
	dlg.m_strTestName = m_pBackupData->GetStringBy(CReportTemplateData::DATA_TEST_NAME, iSelIndex);
	dlg.m_strDisclaimer = m_pBackupData->GetStringBy(CReportTemplateData::DATA_DISCLAIMER, iSelIndex);

	if (dlg.DoModal() == IDOK)
	{
		if (dlg.m_bChanged)
		{
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_NAME, dlg.m_strName, iSelIndex);
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_LAB_NAME, dlg.m_strLabName, iSelIndex);
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_ADDRESS, dlg.m_strLabAdress, iSelIndex);
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_CONTACT_URL, dlg.m_strContactUrl, iSelIndex);
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_CONTACT_EMAIL, dlg.m_strContactEmail, iSelIndex);
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_CONTACT_PHONE, dlg.m_strContactPhone, iSelIndex);
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_TEST_NAME, dlg.m_strTestName, iSelIndex);
			m_pBackupData->SetStringTo(CReportTemplateData::DATA_DISCLAIMER, dlg.m_strDisclaimer, iSelIndex);
			m_pBackupData->SetChanged(TRUE);

			m_lstReportTemplate.SetItemText(iSelIndex, 2, dlg.m_strName, TRUE);
		}
	}
}

void CPs96SimpleSetDlg::OnBnClickedDeleteTemplate()
{
	vector<int> vecSelRow = m_lstReportTemplate.GetSelectedRowId();
	if(vecSelRow.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(380));
		return;
	}

	if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(381)))
	{
		return;
	}

	//m_pParent->DeleteTemplate(vecSelRow[0]);
	m_pBackupData->DeleteTemplate(vecSelRow[0]);
	m_pBackupData->SetChanged(TRUE);

	m_lstReportTemplate.DeleteRow(vecSelRow[0]);

	return;
}

void CPs96SimpleSetDlg::OnBnClickedImportTemplate()
{
	CString strOpenFormat, strSuffix;
	strOpenFormat.Format(_T("Report Template File(*.%s)|*.%s||"), PCR_REPORT_TEMPLATE_FILE_SUFFIX,PCR_REPORT_TEMPLATE_FILE_SUFFIX);
	strSuffix = PCR_REPORT_TEMPLATE_FILE_SUFFIX;;

	CFileDialog fileDlg(TRUE,strSuffix,NULL,OFN_HIDEREADONLY |OFN_OVERWRITEPROMPT,strOpenFormat, this);
	if(IDOK != fileDlg.DoModal())
	{
		return;
	}

	CString strFilename = fileDlg.GetPathName();
	CReportTemplateData* pData = new CReportTemplateData;
	if (pData->OpenFile(strFilename))
	{
		CString strName = pData->GetStringBy(CReportTemplateData::DATA_NAME, 0); // 一定存在一个模板
		if (m_pBackupData->FindSameName(strName))
		{
			PopNormalTipInfo(theApp.m_pLangInfo->GetText(513));
		}
		else
		{
			int iAddIndex = m_pBackupData->AddTemplate(pData->GetTemplate(0));
			m_pBackupData->SetChanged(TRUE);

			CString strTemp;
			m_lstReportTemplate.InsertRowSetHeight(iAddIndex, 70 * m_dVertRatio, TRUE);
			strTemp.Format(_T("%d"), iAddIndex+1);
			m_lstReportTemplate.SetItemText(iAddIndex, 1, strTemp);
			m_lstReportTemplate.SetItemText(iAddIndex, 2, m_pBackupData->GetStringBy(CReportTemplateData::DATA_NAME, iAddIndex));

			PopNormalTipInfo(theApp.m_pLangInfo->GetText(517));
		}
	}
	else
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(518));
	}

	SAFE_DELETE(pData);
}

void CPs96SimpleSetDlg::OnBnClickedExportTemplate()
{
	vector<int> vecSelRow = m_lstReportTemplate.GetSelectedRowId();
	if(vecSelRow.size() < 1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(380));
		return;
	}
	int iTemplateIndex = vecSelRow[0];
	CString strTemplateName = m_pBackupData->GetStringBy(CReportTemplateData::DATA_NAME, iTemplateIndex);

	CString strSaveFormat(_T(""));
	strSaveFormat.Format(_T("Report Template File(*.%s)|*.%s||"), PCR_REPORT_TEMPLATE_FILE_SUFFIX,PCR_REPORT_TEMPLATE_FILE_SUFFIX);

	CFileDialog fileDlg(FALSE, _T(""), strTemplateName, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, strSaveFormat);
	if(IDOK != fileDlg.DoModal())
		return;

	CString strFilename = fileDlg.GetPathName();
	CReportTemplateData* pData = new CReportTemplateData;
	pData->AddTemplate(m_pBackupData->GetTemplate(iTemplateIndex));
	if (pData->SaveFile(strFilename))
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(519));
	}
	else
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(520));
	}

	SAFE_DELETE(pData);
}



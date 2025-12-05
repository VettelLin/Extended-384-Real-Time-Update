// SkipDlg.cpp : implementation file
//

#include "stdafx.h"

#include "Ps96SetDlg.h"
#include "SystemSetDlg.h"
#include "GeneralPcrDlg.h"
#include "GeneralPcr.h"

// CPs96SetDlg dialog
IMPLEMENT_DYNAMIC(CPs96SetDlg, CDialogEx)

CPs96SetDlg::CPs96SetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPs96SetDlg::IDD, pParent)
{
	m_bCtrlCreated = FALSE;
}

CPs96SetDlg::~CPs96SetDlg()
{

}

void CPs96SetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPs96SetDlg,CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_PS96SETDLG_BT_ANASET,OnBnClickedBtAnaSet)
	ON_BN_CLICKED(ID_PS96SETDLG_BT_SAVELIDTEM,OnBnClickedBtSaveLidTem)
	ON_BN_CLICKED(ID_PS96SETDLG_BT_SAVELIDPREHEATTEM,OnBnClickedBtSaveLidPreheatTem)
	ON_BN_CLICKED(ID_PS96SETDLG_BT_READLIDTEM,OnBnClickedBtReadLidTem)
	ON_BN_CLICKED(ID_PS96SETDLG_BT_READLIDPREHEATTEM,OnBnClickedBtReadLidPreheatTem)

	ON_BN_CLICKED(ID_PS96SETDLG_CKBT_EXPORTSAMHORI,OnBnClickedCkbtExportSamHori)
	ON_BN_CLICKED(ID_PS96SETDLG_CKBT_EXPORTSAMVERT,OnBnClickedCkbtExportSamVert)
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
END_MESSAGE_MAP()

BOOL CPs96SetDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);
	DrawRoundRectangleNew(&memDc,0,0,rect.Width() - 1,rect.Height() - 1,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	memDc.SetBkMode(TRANSPARENT);
	memDc.SetTextColor(m_txClr1);
	CFont* pOldFont = memDc.SelectObject(&m_fontSet);

	int iStartX = 10 * m_dHoriRatio,iEndX = 0,iY = 36 * m_dVertRatio,iH = 40 * m_dVertRatio,iVertItl = 60 * m_dVertRatio,iVertIt2 = 72 * m_dVertRatio,iBtW = 100 * m_dHoriRatio;
	if(theApp.GetBeEnglish())
	{
		iEndX = 268 * m_dHoriRatio;
	}
	else
	{
		iEndX = 136 * m_dHoriRatio;
	}

	CRect rc(iStartX,iY,iEndX,iY + iH);
	memDc.DrawText(theApp.m_pLangInfo->GetText(85),rc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	CRect rc2(iEndX + iBtW + 15 * m_dHoriRatio,iY,iEndX + iBtW + 35 * m_dHoriRatio,iY + iH);
	memDc.DrawText(_T("℃"),rc2,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	if (GetShowPreheadLid())
	{
		iY += iVertItl;
		CRect rc3(iStartX,iY,iEndX,iY + iH);
		memDc.DrawText(theApp.m_pLangInfo->GetText(391),rc3,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		CRect rc4(iEndX + iBtW + 15 * m_dHoriRatio,iY,iEndX + iBtW + 35 * m_dHoriRatio,iY + iH);
		memDc.DrawText(_T("℃"),rc4,DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	iY += iVertIt2;
	iH = 22 * m_dVertRatio;
	rc.SetRect(10 * m_dHoriRatio,iY,210 * m_dHoriRatio,iY + iH);
	memDc.DrawText(theApp.m_pLangInfo->GetText(199),rc,DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	//语言标识
	CRect rc5(1000 * m_dHoriRatio,36 * m_dVertRatio,1190 * m_dHoriRatio,76 * m_dVertRatio);
	memDc.DrawText(theApp.m_pLangInfo->GetText(388),rc5,DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	clock_t ckStart = clock();	
	Node<CRect>* pNodeRst = m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	ckStart = clock() - ckStart;

	memDc.SelectObject(pOldFont);

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

BOOL CPs96SetDlg::OnInitDialog()
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

void CPs96SetDlg::CreateCtrl()
{
	CreateBt();
	CreateCkbt();
	CreateSt();
	CreateEd();
	CreateCb();
	CreateLst();
}

void CPs96SetDlg::CreateBt()
{
	m_btSaveLidTem.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SETDLG_BT_SAVELIDTEM);
	m_btSaveLidPreheatTem.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SETDLG_BT_SAVELIDPREHEATTEM);
	m_btAnaSet.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SETDLG_BT_ANASET);
	m_btReadLidTem.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SETDLG_BT_READLIDTEM);
	m_btReadLidPreheatTem.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SETDLG_BT_READLIDPREHEATTEM);
	return;
}

void CPs96SetDlg::CreateCkbt()
{
	m_ckbtExportSamHori.Create(_T("横向导出"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SETDLG_CKBT_EXPORTSAMHORI);
	m_ckbtExportSamVert.Create(_T("纵向导出"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SETDLG_CKBT_EXPORTSAMVERT);
	return;
}

void CPs96SetDlg::CreateSt()
{
	
}

void CPs96SetDlg::CreateEd()
{
	m_edLidTem.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_PS96SETDLG_ED_LIDTEM);
	m_edLidTemInit.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL, CRect(0,0,0,0), this,ID_PS96SETDLG_ED_LIDTEMINIT);
}

void CPs96SetDlg::CreateCb()
{
	CRect rcCreate(0,0,0,0);

	m_cbLangSel.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,rcCreate,this,ID_PS96SETDLG_CB_LANGUAGESELECT);
}

void CPs96SetDlg::CreateLst()
{
	
}

void CPs96SetDlg::InitDlg()
{
	SetTextClr1(RGB(0X39,0X45,0X65));
	SetMyFont(m_stTxFont,18 * m_dTxRatio);
}

void CPs96SetDlg::InitCtrl()
{
	InitBt();
	InitCkbt();
	InitSt();
	InitEd();
	InitCb();
	InitLst();
}

void CPs96SetDlg::InitBt()
{
	int iFtH = 18 * m_dTxRatio;

	m_btSaveLidTem.SetMyFont(m_stTxFont,iFtH);
	m_btSaveLidTem.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btSaveLidPreheatTem.SetMyFont(m_stTxFont,iFtH);
	m_btSaveLidPreheatTem.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btReadLidTem.SetMyFont(m_stTxFont,iFtH);
	m_btReadLidTem.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btReadLidPreheatTem.SetMyFont(m_stTxFont,iFtH);
	m_btReadLidPreheatTem.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	m_btAnaSet.SetMyFont(m_stTxFont,iFtH);
	m_btAnaSet.SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);

	return;
}

void CPs96SetDlg::InitCkbt()
{
	int iFontH = 18 * m_dTxRatio;

	m_ckbtExportSamHori.SetPngBkPic(_T(".\\BkPic\\singleselect.png"),4,14 * m_dHoriRatio);
	m_ckbtExportSamHori.SetIconTextInterval(4 * m_dHoriRatio);
	m_ckbtExportSamHori.SetTextColor(RGB(0X60,0X62,0X66),CLR_SOFT_THEME);
	m_ckbtExportSamHori.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_ckbtExportSamHori.SetBeNeedRedrawBk(TRUE);
	m_ckbtExportSamHori.SetBeParentProcessMouseClick(TRUE);
	m_ckbtExportSamHori.SetMyFontChecked(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	if(ESEM_HORI == theApp.m_eSamExportMode)
	{
		m_ckbtExportSamHori.SetBtChecked(TRUE);
		m_ckbtExportSamHori.EnableWindow(FALSE);
	}

	m_ckbtExportSamVert.SetPngBkPic(_T(".\\BkPic\\singleselect.png"),4,14 * m_dHoriRatio);
	m_ckbtExportSamVert.SetIconTextInterval(4 * m_dHoriRatio);
	m_ckbtExportSamVert.SetTextColor(RGB(0X60,0X62,0X66),CLR_SOFT_THEME);
	m_ckbtExportSamVert.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_ckbtExportSamVert.SetBeNeedRedrawBk(TRUE);
	m_ckbtExportSamVert.SetBeParentProcessMouseClick(TRUE);
	m_ckbtExportSamVert.SetMyFontChecked(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	if(ESEM_VERT == theApp.m_eSamExportMode)
	{
		m_ckbtExportSamVert.SetBtChecked(TRUE);
		m_ckbtExportSamVert.EnableWindow(FALSE);
	}

	return;
}

void CPs96SetDlg::InitSt()
{
	
}

void CPs96SetDlg::InitEd()
{
	InitOneEd(&m_edLidTem);
	InitOneEd(&m_edLidTemInit);

	return;
}

int CPs96SetDlg::InitOneEd(CMyNewEdit* pEd)
{
	if(NULL == pEd)
	{
		return -1; 
	}

	int iFontH = 18 * m_dTxRatio;

	pEd->SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	pEd->SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	pEd->SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	pEd->SetTextColor(RGB(0X66,0X66,0X66),RGB(0X66,0X66,0X66),RGB(0X66,0X66,0X66),RGB(0XC6,0XC6,0XC6));
	pEd->SetMyFont(m_stTxFont,iFontH);

	return 0;
}

void CPs96SetDlg::InitCb()
{
	int iFontH = 16 * m_dTxRatio;

	m_cbLangSel.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_cbLangSel.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_cbLangSel.SetSelectedBkColor(CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME,CLR_SOFT_THEME);
	m_cbLangSel.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XC6,0XC6,0XC6));
	m_cbLangSel.SetMyFont(m_stTxFont);
	m_cbLangSel.m_selWnd.SetMyFont(m_stTxFont);
	m_cbLangSel.SetWorkState(EWS_READONLY);
	m_cbLangSel.m_iRightIndent = 44 * m_dHoriRatio;
	m_cbLangSel.m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	m_cbLangSel.m_arrowInfo.m_arrowW = 12 * m_dHoriRatio;
	m_cbLangSel.m_arrowInfo.m_arrowH = 8 * m_dVertRatio;
	m_cbLangSel.SetTellParentSetNewText(TRUE,ITEM_SELECT_LANGUAGE);

	m_cbLangSel.InsertString(0,theApp.m_pLangInfo->GetText(389));
	m_cbLangSel.InsertString(1,theApp.m_pLangInfo->GetText(390));

	m_cbLangSel.SetCurSel(theApp.m_pLangInfo->GetLanguage(),FALSE,TRUE);

	return;
}

void CPs96SetDlg::InitLst()
{
	
}

void CPs96SetDlg::SetCtrlPos()
{
	ResetCtrlPos();
	return;
}

void CPs96SetDlg::ResetCtrlPos()
{
	CRect rect;
	GetClientRect(rect);

	m_rcInit = rect;
	ClearRectToDelete();

	int iX = 0,iX2 = 0,iY = 36 * m_dVertRatio,iBtW = 100 * m_dHoriRatio,iBtH = 40 * m_dVertRatio,iVertItl = 60 * m_dVertRatio,iTempInt1 = 16 * m_dHoriRatio,iTempInt2 = 0,iTempInt3 = 0;
	int iTpInt4 = 0,iVertItl2 = 72 * m_dVertRatio;

	if(theApp.GetBeEnglish())
	{
		iX = 270 * m_dHoriRatio;
		iTempInt2 = iX;
	}
	else
	{
		iX = 140 * m_dHoriRatio;
		iTempInt2 = iX;
	}
	iTempInt3 = iX + 114 * m_dHoriRatio;

	m_edLidTem.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	iTpInt4 = iX + iBtW + 25 * m_dHoriRatio + iTempInt1;
	m_btSaveLidTem.MoveWindow(iTpInt4,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iTpInt4,iY,iBtW,iBtH);

	iX2 = iTpInt4 + iBtW + 32 * m_dHoriRatio;
	m_btReadLidTem.MoveWindow(iX2,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX2,iY,iBtW,iBtH);

	if (GetShowPreheadLid())
	{
		iY += iVertItl;
		m_edLidTemInit.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
		AddRectToDelete(iX,iY,iBtW,iBtH);

		m_btSaveLidPreheatTem.MoveWindow(iTpInt4,iY,iBtW,iBtH,FALSE);
		AddRectToDelete(iTpInt4,iY,iBtW,iBtH);

		m_btReadLidPreheatTem.MoveWindow(iX2,iY,iBtW,iBtH,FALSE);
		AddRectToDelete(iX2,iY,iBtW,iBtH);
	}

	iX = iTempInt2;
	iY += iVertItl2;
	iBtW = 110 *m_dHoriRatio;
	iBtH = 22 * m_dVertRatio;
	m_ckbtExportSamHori.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	iX = iTempInt3;
	m_ckbtExportSamVert.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	iX = 1200 * m_dHoriRatio;
	iY = 36 * m_dVertRatio;
	iBtW = 200 * m_dHoriRatio;
	iBtH = 40 * m_dVertRatio;
	m_cbLangSel.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	iY += iVertItl2;
	m_btAnaSet.MoveWindow(iX,iY,iBtW,iBtH,FALSE);
	AddRectToDelete(iX,iY,iBtW,iBtH);

	CalcRectResult();

	return;
}

void CPs96SetDlg::ShowCtrl()
{
	m_edLidTem.ShowWindow(SW_SHOW);
	m_btSaveLidTem.ShowWindow(SW_SHOW);
	m_btReadLidTem.ShowWindow(SW_SHOW);

	if (GetShowPreheadLid())
	{
		m_edLidTemInit.ShowWindow(SW_SHOW);
		m_btSaveLidPreheatTem.ShowWindow(SW_SHOW);
		m_btReadLidPreheatTem.ShowWindow(SW_SHOW);
	}
	else
	{
		m_edLidTemInit.ShowWindow(SW_HIDE);
		m_btSaveLidPreheatTem.ShowWindow(SW_HIDE);
		m_btReadLidPreheatTem.ShowWindow(SW_HIDE);
	}

	m_ckbtExportSamHori.ShowWindow(SW_SHOW);
	m_ckbtExportSamVert.ShowWindow(SW_SHOW);

	m_cbLangSel.ShowWindow(SW_SHOW);
	m_btAnaSet.ShowWindow(SW_SHOW);
	
	return;
}

void CPs96SetDlg::SetCtrlText()
{
	m_btSaveLidTem.SetWindowText(theApp.m_pLangInfo->GetText(148));
	m_btSaveLidPreheatTem.SetWindowText(theApp.m_pLangInfo->GetText(148));
	m_btReadLidTem.SetWindowText(theApp.m_pLangInfo->GetText(452));
	m_btReadLidPreheatTem.SetWindowText(theApp.m_pLangInfo->GetText(452));
	m_btAnaSet.SetWindowText(theApp.m_pLangInfo->GetText(432));
	m_ckbtExportSamHori.SetWindowText(theApp.m_pLangInfo->GetText(197));
	m_ckbtExportSamVert.SetWindowText(theApp.m_pLangInfo->GetText(198));

	return;
}

void CPs96SetDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	__super::OnSize(nType, cx, cy);
}	

void CPs96SetDlg::OnBnClickedBtAnaSet()
{
	CSystemSetDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		theApp.WriteSystemAnalyseParam();
	}
	return;
}

void CPs96SetDlg::OnBnClickedBtSaveLidTem()
{
	SaveLidTem();

	return;
}

void CPs96SetDlg::OnBnClickedBtSaveLidPreheatTem()
{
	SaveLidPreheatTem();
	return;
}

void CPs96SetDlg::OnBnClickedBtReadLidTem()
{
	ReadLidTem();
	return;
}

void CPs96SetDlg::OnBnClickedBtReadLidPreheatTem()
{
	ReadLidPreheatTem();
	return;
}

void CPs96SetDlg::OnBnClickedCkbtExportSamHori()
{
	BOOL bCkbtState = m_ckbtExportSamHori.GetCheckState();
	m_ckbtExportSamHori.SetBtChecked(!bCkbtState,TRUE);
	m_ckbtExportSamHori.EnableWindow(FALSE);

	m_ckbtExportSamVert.SetBtChecked(bCkbtState,TRUE);
	m_ckbtExportSamVert.EnableWindow(TRUE);
	theApp.m_eSamExportMode = ESEM_HORI;
	theApp.WriteParam();
	return;
}

void CPs96SetDlg::OnBnClickedCkbtExportSamVert()
{
	BOOL bCkbtState = m_ckbtExportSamVert.GetCheckState();
	m_ckbtExportSamVert.SetBtChecked(!bCkbtState,TRUE);
	m_ckbtExportSamVert.EnableWindow(FALSE);

	m_ckbtExportSamHori.SetBtChecked(bCkbtState,TRUE);
	m_ckbtExportSamHori.EnableWindow(TRUE);
	theApp.m_eSamExportMode = ESEM_VERT;
	theApp.WriteParam();
	return;
}

LRESULT CPs96SetDlg::OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam)
{
	EITEMINFOTYPE eItemInfoType = (EITEMINFOTYPE)wParam;

	switch(eItemInfoType)
	{
	case ITEM_SELECT_LANGUAGE: LanguageChanged();	break;

	default:	break;
	}

	return 0;
}

int CPs96SetDlg::SaveLidTem()
{
	CString strNewLidTem = m_edLidTem.GetText();
	int iLidTem = _ttoi(strNewLidTem);
	if(iLidTem < LIDTEMMIN || iLidTem > LIDTEMMAX)
	{
		CString strTemp(_T("")),strFormat(_T(""));
		strFormat = theApp.m_pLangInfo->GetText(275);
		strTemp.Format(strFormat,LIDTEMMIN,LIDTEMMAX,LIDTEMMIN,LIDTEMMAX);
		PopWarningTipInfo(strTemp);
		return -1;
	}

	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	if(0 == pMainDlg->SetLidTem(iLidTem))
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(164));
	}
	else
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(165));
	}

	return 0;
}

int CPs96SetDlg::ReadLidTem()
{
	double dLidTem = 0;
	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	if(0 == pMainDlg->ReadLidTem(dLidTem))
	{
		CString strTemp(_T(""));
		strTemp.Format(_T("%.2lf"),dLidTem);
		m_edLidTem.SetText(strTemp,TRUE,TRUE);
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(453));
	}
	else
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(454));
	}

	return 0;
}

int CPs96SetDlg::SaveLidPreheatTem()
{
	CString strNewLidTemInit = m_edLidTemInit.GetText();
	int iLidTemInit = _ttoi(strNewLidTemInit);
	if(iLidTemInit < LIDTEMMIN || iLidTemInit > LIDTEMMAX)
	{
		CString strTemp(_T("")),strFormat(_T(""));
		strFormat = theApp.m_pLangInfo->GetText(430);
		strTemp.Format(strFormat,LIDTEMMIN,LIDTEMMAX,LIDTEMMIN,LIDTEMMAX);
		PopWarningTipInfo(strTemp);
		return -1;
	}

	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	if(0 == pMainDlg->SetLidPreheatTem(iLidTemInit))
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(164));
	}
	else
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(165));
	}

	return 0;
}

int CPs96SetDlg::ReadLidPreheatTem()
{
	float fLidPreheatTem = 0;
	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	if(0 == pMainDlg->ReadLidPreheatTem(fLidPreheatTem))
	{
		CString strTemp(_T(""));
		strTemp.Format(_T("%.2lf"),fLidPreheatTem);
		m_edLidTemInit.SetText(strTemp,TRUE,TRUE);
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(455));
	}
	else
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(456));
	}

	return 0;
}

int CPs96SetDlg::RefreshLanguageDisplay()
{
	SetMyFont(m_stTxFont);

	m_edLidTem.SetMyFont(m_stTxFont);
	m_edLidTem.TextParamChanged();

	m_edLidTemInit.SetMyFont(m_stTxFont);
	m_edLidTemInit.TextParamChanged();

	m_cbLangSel.SetMyFont(m_stTxFont);
	m_cbLangSel.m_selWnd.SetMyFont(m_stTxFont);
	
	int iCurSel = m_cbLangSel.GetCurSel();
	m_cbLangSel.SetItemText(0,theApp.m_pLangInfo->GetText(389));
	m_cbLangSel.SetItemText(1,theApp.m_pLangInfo->GetText(390));
	m_cbLangSel.TextParamChanged();
	m_cbLangSel.RefreshText(iCurSel);

	int iFontH = 18 * m_dTxRatio;
	m_ckbtExportSamHori.SetMyFont(m_stTxFont);
	m_ckbtExportSamHori.SetMyFontChecked(m_stTxFont,iFontH);
	m_ckbtExportSamHori.SetWindowText(theApp.m_pLangInfo->GetText(197));

	m_ckbtExportSamVert.SetMyFont(m_stTxFont);
	m_ckbtExportSamVert.SetMyFontChecked(m_stTxFont,iFontH);
	m_ckbtExportSamVert.SetWindowText(theApp.m_pLangInfo->GetText(198));

	m_btSaveLidTem.SetMyFont(m_stTxFont);
	m_btSaveLidTem.SetWindowText(theApp.m_pLangInfo->GetText(148));

	m_btSaveLidPreheatTem.SetMyFont(m_stTxFont);
	m_btSaveLidPreheatTem.SetWindowText(theApp.m_pLangInfo->GetText(148));

	m_btReadLidTem.SetMyFont(m_stTxFont);
	m_btReadLidTem.SetWindowText(theApp.m_pLangInfo->GetText(452));

	m_btReadLidPreheatTem.SetMyFont(m_stTxFont);
	m_btReadLidPreheatTem.SetWindowText(theApp.m_pLangInfo->GetText(452));

	m_btAnaSet.SetMyFont(m_stTxFont);
	m_btAnaSet.SetWindowText(theApp.m_pLangInfo->GetText(432));

	ResetCtrlPos();

	Invalidate();

	return 0;
}

int CPs96SetDlg::LanguageChanged()
{
	int iCurSel = m_cbLangSel.GetCurSel();
	if(iCurSel < 0)
	{
		return -1;
	}

	theApp.SetLanguage((eLanguageUI)iCurSel);
	theApp.ResetTextFont();

	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	pMainDlg->RefreshLanguageDisplay();

	return 0;
}

BOOL CPs96SetDlg::GetShowPreheadLid()
{
	BOOL bShow = TRUE;
#ifdef N96INSTRUMENT
	bShow = FALSE;
#endif

//#ifdef PS96INSTRUMENT	
//	bShow = FALSE;
//#endif

	return bShow;
}

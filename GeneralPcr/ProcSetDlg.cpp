// ProcSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "MiddleDlg.h"
#include "ProcSetDlg.h"
#include "ReagentItemManager.h"
#include "AddCycleDlg.h"
#include "MySegEditInfo.h"
#include "afxdialogex.h"

// CProcSetDlg dialog

IMPLEMENT_DYNAMIC(CProcSetDlg, CDialogEx)

CProcSetDlg::CProcSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProcSetDlg::IDD, pParent),m_samBaseInfoEditDlg(this,this),m_prjBaseInfoDlg(FALSE,this,this)
{
	m_pParent = NULL;
	if(NULL != pParent)
	{
		m_pParent = (CMiddleDlg*)pParent;
	}
	m_bCtrlCreated = FALSE;
}

CProcSetDlg::~CProcSetDlg()
{

}

void CProcSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProcSetDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_PROCSETDLG_BT_INSERTSEGMENT,OnBnClickedInsertSegment)
	ON_BN_CLICKED(ID_PROCSETDLG_BT_INSERTSTEP,OnBnClickedInsertStep)
	ON_BN_CLICKED(ID_PROCSETDLG_BT_EDITSTEP,OnBnClickedEditStep)
	ON_BN_CLICKED(ID_PROCSETDLG_BT_EDITSEGMENT,OnBnClickedEditSegment)
	ON_BN_CLICKED(ID_PROCSETDLG_BT_DELETE,OnBnClickedDelete)
	ON_BN_CLICKED(ID_PROCSETDLG_BT_ADDCYCLE,OnBnClickedAddCycle)

	ON_COMMAND(ID_MENU_INSERTSEGBEFORE, OnFileMenuSegInsertBefore)
	ON_COMMAND(ID_MENU_INSERTSEGAFTER, OnFileMenuSegInsertAfter)
	ON_COMMAND(ID_MENU_INSERTSEGLAST, OnFileMenuSegInsertLast)

	ON_COMMAND(ID_MENU_INSERTSTEPBEFORE, OnFileMenuStepInsertBefore)
	ON_COMMAND(ID_MENU_INSERTSTEPAFTER, OnFileMenuStepInsertAfter)
	ON_COMMAND(ID_MENU_INSERTSTEPLAST, OnFileMenuStepInsertLast)

	ON_MESSAGE(WM_TELLPARENTSITESELECTCHANGED,OnMsgPlateSiteSelectChanged)
	ON_MESSAGE(WM_ROWSELECTCHANGE,OnMsgRowSelectedChange)
	ON_MESSAGE(SAMPLATE_DELETEKEY_PRESSED,OnMsgSamplateDeleteKeyPressed)
END_MESSAGE_MAP()

BOOL CProcSetDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	CRect rect; 
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	int iTempInt1 = 336 * m_dHoriRatio;

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	DrawRoundRectangleNew(&memDc,738 * m_dHoriRatio,12 * m_dVertRatio,1030 * m_dHoriRatio,298 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	memDc.FillSolidRect(1630 * m_dHoriRatio,33 * m_dVertRatio,4 * m_dHoriRatio,14 * m_dVertRatio,CLR_SOFT_THEME);
	
	CString strTemp = pLangInfo->GetText(35);
	memDc.DrawText(strTemp,CRect(1645 * m_dHoriRatio,28 * m_dVertRatio,1750 * m_dHoriRatio,52 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	memDc.SelectObject(pOldFont);

	CRect rcLegend;
	rcLegend.left = 352 * m_dHoriRatio;
	rcLegend.right = rcLegend.left + 868 * m_dHoriRatio;
	rcLegend.top = 327 * m_dVertRatio + 505 * m_dVertRatio;
	rcLegend.bottom = rcLegend.top + 27 * m_dVertRatio;
	DrawLegend(&memDc,&rcLegend,m_segLgdInfo);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_lnkRcRst.GetHead();
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

void CProcSetDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

BOOL CProcSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitDlg();
	InitSegLegend();
	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	SetCtrlText();
	m_samBaseInfoEditDlg.SetEditableState(FALSE);

	ShowCtrl();

	return TRUE;
}

void CProcSetDlg::InitDlg()
{
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
	SetTextClr1(RGB(0X39,0X45,0X65));
}

void CProcSetDlg::InitSegLegend()
{
	CSingleLgdInfo singleLgdInfo;

#ifdef N16USENEWSAMPLETYPE
	singleLgdInfo.m_clrFill = Color(0X9B,0X51,0XE0);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(211);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X21,0X96,0X53);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(212);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XEB,0X57,0X57);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(215);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X2F,0X80,0XED);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(214);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XF2,0X99,0X4A);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(213);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

#ifdef CAN_YOUNG
	singleLgdInfo.m_clrFill = Color(0,0X70,0XC0);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(457);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);
#endif

#else
	singleLgdInfo.m_clrFill = Color(0X20,0X44,0X7C);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(211);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X20,0X9F,0X85);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(212);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X33,0X83,0XFD);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(213);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0X24,0XB7,0XEA);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(214);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XC5,0X64,0X62);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(215);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XF2,0X31,0X31);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(216);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0XF0,0X31,0XF2);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(217);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);

	singleLgdInfo.m_clrFill = Color(0,0X70,0XC0);
	singleLgdInfo.m_strTx = theApp.m_pLangInfo->GetText(218);
	m_segLgdInfo.AddSingleLegendInfo(singleLgdInfo);
#endif

	m_segLgdInfo.m_iLeftIndent = 41 * m_dHoriRatio; 
	m_segLgdInfo.m_iRoundRadius = 6 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrIconToTx = 8 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrTxToIcon = 20 * m_dHoriRatio; 

	int iFontH = 13 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		m_segLgdInfo.m_iOneCharW = 10 * m_dHoriRatio;
		iFontH = 15 * m_dTxRatio;
	}
	else
	{
		m_segLgdInfo.m_iOneCharW = 14 * m_dHoriRatio;
	}

	m_segLgdInfo.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_segLgdInfo.SetTextClr1(RGB(0X39,0X45,0X65));
}

void CProcSetDlg::InitSonDlg()
{
	m_prjBaseInfoDlg.Create(IDD_MIDDLEDLG,this);
	InitSamBaseInfoEditDlg();
}

void CProcSetDlg::InitSamBaseInfoEditDlg()
{
	m_samBaseInfoEditDlg.Create(IDD_MIDDLEDLG,this);
}

void CProcSetDlg::CreateCtrl()
{
	CreateBt();
	CreateLst();
	CreateSamPlateDlg();
	CreateSeg();
}

void CProcSetDlg::CreateBt()
{
	m_btInsertSegment.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PROCSETDLG_BT_INSERTSEGMENT);
	m_btInsertStep.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PROCSETDLG_BT_INSERTSTEP);
	m_btEditStep.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PROCSETDLG_BT_EDITSTEP);
	m_btEditSegment.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PROCSETDLG_BT_EDITSEGMENT);
	m_btDelete.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PROCSETDLG_BT_DELETE);
	m_btAddCycle.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PROCSETDLG_BT_ADDCYCLE);
}

void CProcSetDlg::CreateLst()
{
	m_lstSelPrj.Create(IDD_MIDDLEDLG,this);
}

void CProcSetDlg::CreateSamPlateDlg()
{
	m_samPlateDlg.Create(IDD_MIDDLEDLG,this);
}

void CProcSetDlg::CreateSeg()
{
	m_segDlg.Create(IDD_MIDDLEDLG,this);
}

void CProcSetDlg::InitCtrl()
{
	InitBt();
	InitLst();
	InitSamPlateDlg();
	InitSeg();
}

void CProcSetDlg::InitBt()
{
	InitInsertBt(&m_btInsertSegment);
	m_btInsertSegment.SetSplitLineXPos(94 * m_dHoriRatio);

	InitInsertBt(&m_btInsertStep);
	m_btInsertStep.SetSplitLineXPos(82 * m_dHoriRatio);

	InitOtherBt(&m_btEditStep);
	InitOtherBt(&m_btEditSegment);
	InitOtherBt(&m_btDelete);
	
	InitOtherBt(&m_btAddCycle);
	m_btAddCycle.EnableWindow(FALSE);

	return;
}

void CProcSetDlg::InitInsertBt(CMyInsertBt *pInsertBt)
{
	if(NULL == pInsertBt->GetSafeHwnd())
	{
		return;
	}

	int iFontH = 13 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	pInsertBt->m_arrowInfo.m_eArrowStyle = EAS_LEFTRIGHT;
	pInsertBt->m_arrowInfo.m_arrowW = 8 * m_dHoriRatio;
	pInsertBt->m_arrowInfo.m_arrowH = 4 * m_dVertRatio;
	pInsertBt->SetMyFont(m_stTxFont,iFontH);
	return;
}

void CProcSetDlg::InitOtherBt(CMyButton *pBt)
{
	if(NULL == pBt->GetSafeHwnd())
	{
		return;
	}

	int iFontH = 13 * CFontSet::m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 15 * CFontSet::m_dTxRatio;
	}

	pBt->SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),FALSE);
	pBt->SetBkColor(Color(0XF1,0XF4,0XF9),Color(0XFF,0XFF,0XFF),Color(0XE7,0XE7,0XE7),Color(0XF1,0XF4,0XF9));
	pBt->SetEdgeColor(Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8));
	pBt->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	pBt->SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	return;
}

void CProcSetDlg::InitLst()
{
	InitLstSelectPrj();
}

void CProcSetDlg::InitLstSelectPrj()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_lstSelPrj.InsertColumn(0,theApp.m_pLangInfo->GetText(28));
	m_lstSelPrj.InsertColumn(1,theApp.m_pLangInfo->GetText(29));

#ifdef PS96INSTRUMENT
	if(theApp.GetBeIVDUser())
	{
		m_lstSelPrj.InsertColumn(2,theApp.m_pLangInfo->GetText(296));
	}
	else
	{
		m_lstSelPrj.InsertColumn(2,theApp.m_pLangInfo->GetText(30));
	}
#else
	m_lstSelPrj.InsertColumn(2,theApp.m_pLangInfo->GetText(30));
#endif
	
	m_lstSelPrj.SetHeaderHeight(24 * m_dVertRatio);
	int iTempInt1 = 34 * m_dVertRatio;
	m_lstSelPrj.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstSelPrj.m_horiScroll.m_scrollH = iTempInt1;
	m_lstSelPrj.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstSelPrj.m_vertScroll.m_scrollW = iTempInt1;

	m_lstSelPrj.SetMyTitleFont(m_stTxFont,iFontH,FW_MEDIUM);

	m_lstSelPrj.SetBeSingleSelect(TRUE);
	m_lstSelPrj.SetBeAllowCancelRowSelect(TRUE);
	m_lstSelPrj.SetBeTellParentRowSelChange(TRUE);
	m_lstSelPrj.SetMyFont(m_stTxFont,iFontH);
	m_lstSelPrj.SetTextClr1(RGB(0X39,0X45,0X65));
	m_lstSelPrj.SetMyHeaderFont(m_stTxFont,iFontH);
	m_lstSelPrj.SetBeSingleSelect(TRUE);
	m_lstSelPrj.SetColumnCheckFunc(0,TRUE);
	m_lstSelPrj.SetFourAngleBkColor(CLR_SOFT_LIGHT);
	m_lstSelPrj.SetItemHeight(54 * m_dVertRatio);
	m_lstSelPrj.SetRowBkSelectedColor(RGB(0XF1,0XF1,0XF1));

	int tempInt1 = 0,tempInt2 = 320 * m_dHoriRatio;
	tempInt1 = 60 * m_dHoriRatio;
	m_lstSelPrj.SetColumnWidth(0,tempInt1);
	tempInt2 -= tempInt1;
	tempInt1 = 60 * m_dHoriRatio;
	m_lstSelPrj.SetColumnWidth(1,tempInt1);
	tempInt2 -= tempInt1;
	tempInt2 -= m_lstSelPrj.GetFrameLineWidth() * 2;
	m_lstSelPrj.SetColumnWidth(2,tempInt2);

#ifdef PS96INSTRUMENT
	if(theApp.GetBeIVDUser())
	{
#ifdef IVD_USE_TEMPALTE_LIST
		SetSelectPrjLstInfoIVDNoProject();
#else
		SetSelectPrjLstInfo();
#endif
	}
	else
	{
		SetSelectPrjLstInfo();
	}
#else
	SetSelectPrjLstInfo();
#endif

	return;
}

void CProcSetDlg::InitSamPlateDlg()
{
	int iFtH = 18 * m_dTxRatio;
	int iFtAbbrH = 14 * m_dTxRatio;

	m_samPlateDlg.SetRowHeaderWidth(SAMPLATE_WH * m_dHoriRatio);
	m_samPlateDlg.SetColumnHeaderHeight(SAMPLATE_WH * m_dVertRatio);
	m_samPlateDlg.SetFixedRowHeightColumnWidth(1 * m_dVertRatio,3 * m_dHoriRatio);
	m_samPlateDlg.SetInterval(1 * m_dHoriRatio,1 * m_dVertRatio);
	
	m_samPlateDlg.SetRowColNum(32, 12);

	m_samPlateDlg.SetBkColor(RGB(0XE6,0XF4,0XF5));
	m_samPlateDlg.SetMyFont(m_stTxFont,iFtH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFtH);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFtH,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFtAbbrH);
	m_samPlateDlg.SetBeTellParentRowSelChange(TRUE);
	m_samPlateDlg.SetBeSingleSelect(TRUE);
	m_samPlateDlg.SetBeParentProcessDeleteKey(TRUE);

#ifdef PS96INSTRUMENT
	m_samPlateDlg.SetSiteIdShowState(FALSE);
	m_samPlateDlg.SetLastChanBlockShowState(FALSE);
#else 
	#ifdef N96INSTRUMENT
		m_samPlateDlg.SetSiteIdShowState(FALSE);
	#endif
#endif
}

void CProcSetDlg::InitSeg()
{
	int iFtH = 14 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFtH = 15 * m_dTxRatio;
	}
	
	m_segDlg.m_fontStepTag.SetMyFont(m_stTxFont,iFtH);
	m_segDlg.m_fontGeneral.SetMyFont(m_stTxFont,iFtH);
	m_segDlg.m_fontSegTag.SetMyFont(m_stTxFont,iFtH,FW_MEDIUM);
	m_segDlg.SetFuncEditCtrlFont(m_stTxFont,iFtH);

	int iTempInt1 = 34 * m_dVertRatio;
	m_segDlg.m_horiScroll.m_arrowWH = iTempInt1;
	m_segDlg.m_horiScroll.m_scrollH = iTempInt1;
	m_segDlg.m_vertScroll.m_arrowWH = iTempInt1;
	m_segDlg.m_vertScroll.m_scrollW = iTempInt1;
	m_segDlg.SetScrollArrowWH(4 * m_dHoriRatio,8 * m_dVertRatio);

	m_segDlg.SetSegmentString(theApp.m_pLangInfo->GetText(57));
	m_segDlg.SetStepString(theApp.m_pLangInfo->GetText(58));
	m_segDlg.SetGradientString(theApp.m_pLangInfo->GetText(449));
	m_segDlg.SetColumnString(theApp.m_pLangInfo->GetText(450));
	m_segDlg.SetRangeString(theApp.m_pLangInfo->GetText(451));

#ifdef PS96INSTRUMENT
	if(!theApp.GetBeIVDUser())
	{
		m_segDlg.SetDefaultAmlProgramForProject();
	}
#else
	m_segDlg.SetDefaultAbsoluteQuantification();
#endif

#ifndef PS96INSTRUMENT
	//m_segDlg.SetCollectFluTagType(ECFTT_TRIANGLE);
#endif

	return;
}
 
void CProcSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcInit = rect;
	ClearRectToDelete();

	int iTempInt2 = 327 * m_dVertRatio;
	int iTempInt3 = rect.Height() - iTempInt2 - 8 * m_dVertRatio;

	m_lstSelPrj.MoveWindow(16 * m_dHoriRatio,12 * m_dVertRatio,320 * m_dHoriRatio,rect.Height() - 12 * m_dVertRatio - 8 * m_dVertRatio,FALSE);
	AddRectToDelete(16 * m_dHoriRatio,12 * m_dVertRatio,320 * m_dHoriRatio,rect.Height() - 12 * m_dVertRatio - 8 * m_dVertRatio);

	m_prjBaseInfoDlg.MoveWindow(352 * m_dHoriRatio,12 * m_dVertRatio,369 * m_dHoriRatio,298 * m_dVertRatio,FALSE);
	AddRectToDelete(352 * m_dHoriRatio,12 * m_dVertRatio,369 * m_dHoriRatio,298 * m_dVertRatio);

	m_segDlg.MoveWindow(754 * m_dHoriRatio,28 * m_dVertRatio,836 * m_dHoriRatio,274 * m_dVertRatio,FALSE);
	AddRectToDelete(754 * m_dHoriRatio,28 * m_dVertRatio,836 * m_dHoriRatio,274 * m_dVertRatio);

	m_samPlateDlg.MoveWindow(352 * m_dHoriRatio,iTempInt2,868 * m_dHoriRatio,505 * m_dVertRatio,FALSE);
	AddRectToDelete(352 * m_dHoriRatio,iTempInt2,868 * m_dHoriRatio,505 * m_dVertRatio);

	m_samBaseInfoEditDlg.MoveWindow(1237 * m_dHoriRatio,iTempInt2,532 * m_dHoriRatio,iTempInt3,FALSE);
	AddRectToDelete(1237 * m_dHoriRatio,iTempInt2,532 * m_dHoriRatio,iTempInt3);

	int iTempInt1 = 1630 * m_dHoriRatio;
	int iTempInt4 = 110 * m_dHoriRatio;
	int iTempInt5 = 28 * m_dVertRatio;

	m_btInsertSegment.MoveWindow(iTempInt1,64 * m_dVertRatio,121 * m_dHoriRatio,iTempInt5,FALSE);
	AddRectToDelete(iTempInt1,64 * m_dVertRatio,121 * m_dHoriRatio,iTempInt5);

	m_btEditSegment.MoveWindow(iTempInt1,104 * m_dVertRatio,iTempInt4,iTempInt5,FALSE);
	AddRectToDelete(iTempInt1,104 * m_dVertRatio,iTempInt4,iTempInt5);

	m_btInsertStep.MoveWindow(iTempInt1,144 * m_dVertRatio,iTempInt4,iTempInt5,FALSE);
	AddRectToDelete(iTempInt1,144 * m_dVertRatio,iTempInt4,iTempInt5);

	m_btEditStep.MoveWindow(iTempInt1,184 * m_dVertRatio,iTempInt4,iTempInt5,FALSE);
	AddRectToDelete(iTempInt1,184 * m_dVertRatio,iTempInt4,iTempInt5);

	m_btDelete.MoveWindow(iTempInt1,224 * m_dVertRatio,iTempInt4,iTempInt5,FALSE);
	AddRectToDelete(iTempInt1,224 * m_dVertRatio,iTempInt4,iTempInt5);

	m_btAddCycle.MoveWindow(iTempInt1,264 * m_dVertRatio,iTempInt4,iTempInt5,FALSE);
#ifndef HIDE_ADDCYCLE_FUNC
	AddRectToDelete(iTempInt1,264 * m_dVertRatio,iTempInt4,iTempInt5);
#endif

	CalcRectResult();

	return;
}

void CProcSetDlg::ShowCtrl()
{
	m_lstSelPrj.ShowWindow((SW_SHOW));
	m_prjBaseInfoDlg.ShowWindow((SW_SHOW));
	m_segDlg.ShowWindow((SW_SHOW));
	m_samPlateDlg.ShowWindow((SW_SHOW));
	m_samBaseInfoEditDlg.ShowWindow((SW_SHOW));

	m_btInsertSegment.ShowWindow(SW_SHOW);
	m_btInsertStep.ShowWindow(SW_SHOW);
	m_btEditStep.ShowWindow(SW_SHOW);
	m_btEditSegment.ShowWindow(SW_SHOW);
	m_btDelete.ShowWindow(SW_SHOW);

#ifndef HIDE_ADDCYCLE_FUNC
	m_btAddCycle.ShowWindow(SW_SHOW);
#endif
}

void CProcSetDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_btInsertSegment.SetWindowText(pLangInfo->GetText(36));
	m_btInsertStep.SetWindowText(pLangInfo->GetText(41));
	m_btEditStep.SetWindowText(pLangInfo->GetText(44));
	m_btEditSegment.SetWindowText(pLangInfo->GetText(40));
	m_btDelete.SetWindowText(pLangInfo->GetText(45));
	m_btAddCycle.SetWindowText(pLangInfo->GetText(46));

	return;
}

int CProcSetDlg::SetRowColNum(int iRowNum,int iColNum)
{
	if(iRowNum < 1)
	{
		return -1;
	}
	if(iColNum < 1)
	{
		return -2;
	}
	m_samPlateDlg.SetRowColNum(iRowNum,iColNum);
	
	return 0;
}

void CProcSetDlg::SetPlateSelectedSiteSamInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/)
{
	BOOL bFindSamTypeNone = FALSE;
	CTubeInfo tubeInfo;
	if(ITEMSAMTYPE == eItemInfoType)	//�ж��޸�֮ǰ�����������Ƿ���ڿ�����
	{
		for(int i = 0;i < m_vecPlateSelectedSite.size();++i)
		{
			m_samPlateDlg.GetSamInfo(m_vecPlateSelectedSite[i],tubeInfo);
			if(SAMPLE_TYPE_NONE == tubeInfo.GetSampleType())
			{
				bFindSamTypeNone = TRUE;
				break;
			}
		}
		
		if(intData == SAMPLE_TYPE_NONE)
		{
			bFindSamTypeNone = TRUE;
		}
	}

	for(int i = 0;i < m_vecPlateSelectedSite.size();++i)
	{
		m_samPlateDlg.SetSamInfo(m_vecPlateSelectedSite[i],eItemInfoType,strData,intData,dData);
	}

	m_samPlateDlg.Invalidate(FALSE);

	ResetSamSetDlgTubeInfo();
	return;
}

int CProcSetDlg::SetPlateSelectedSiteSamInfo(CTubeChanEditInfo tubeChanEditInfo)
{
	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;

	vector<int> vecSiteToChanged;
	
#ifdef TUBE_USESAMEDYEANDCORLOR
	if(tubeChanEditInfo.GetBeAllTubeSameSet())
	{
		for(int i = 0;i < m_samPlateDlg.m_iTotalSiteNum;++i)
		{
			vecSiteToChanged.push_back(i);
		}
	}
	else
	{
		vecSiteToChanged = m_vecPlateSelectedSite;
	}
#else
	vecSiteToChanged = m_vecPlateSelectedSite;
#endif

	for(int i = 0;i < vecSiteToChanged.size();++i)
	{
		if(0 != m_samPlateDlg.GetTubeInfoPtr(vecSiteToChanged[i],&pTubeInfo))
		{
			return -1;
		}

		//Ⱦ����Ϣ���ʹ������ģʽ����̬���ӡ�ɾ�� ���� ҳ�����ʱ���Ѿ��̶�
		pDyeTargetInfo = pTubeInfo->FindTargetInfoByChannel(tubeChanEditInfo.m_iRowId + 1);
		if(NULL == pDyeTargetInfo)
		{
			return -2;
		}

		if(0 == tubeChanEditInfo.m_iColId)
		{
			pDyeTargetInfo->SetBeSelected(tubeChanEditInfo.m_bChecked);
		}
		else if(2 == tubeChanEditInfo.m_iColId)
		{
			pDyeTargetInfo->SetDyeName(tubeChanEditInfo.m_strData);
		}
		else if(3 == tubeChanEditInfo.m_iColId)
		{
			pDyeTargetInfo->SetTargetName(tubeChanEditInfo.m_strData);
		}
		else if(4 == tubeChanEditInfo.m_iColId)
		{
			pDyeTargetInfo->SetTargetColor(tubeChanEditInfo.m_clrData);
		}
		else if(5 == tubeChanEditInfo.m_iColId)
		{
			pDyeTargetInfo->SetQuantity(tubeChanEditInfo.m_dData,tubeChanEditInfo.m_iDpd);
		}

		pTubeInfo = NULL;
	}

	m_samPlateDlg.Invalidate(FALSE);

	ResetSamSetDlgTubeInfo();

	//Ⱦ�����Ƹı䣬����ͨ���л���ť�ͷ���ҳ����ֵ��Ӧͨ���ı�ʶ
	if(2 == tubeChanEditInfo.m_iColId)
	{
		CString strNewDyeName = m_samPlateDlg.GetOneChannelDyeNameFromTubeInfo(tubeChanEditInfo.m_iRowId);
		if(!strNewDyeName.IsEmpty() && NULL != m_pParent->GetSafeHwnd())
		{
			m_pParent->SampleDyeNameChanged(tubeChanEditInfo.m_iRowId,strNewDyeName);
		}
	}

	return 0;
}

int CProcSetDlg::ReSetTubeInfo(vector<int> &vecPlateSelectedSite,vector<CTubeInfo> &vecTubeInfo)
{
	BOOL bFindSelected = FALSE;
	for(int i = 0;i < vecPlateSelectedSite.size();++i)
	{
		m_samPlateDlg.SetSamInfo(vecPlateSelectedSite[i],vecTubeInfo[i]);

		if(!bFindSelected)
		{
			for(int j = 0;j < m_vecPlateSelectedSite.size();++j)
			{
				if(vecPlateSelectedSite[i] == m_vecPlateSelectedSite[j])
				{
					bFindSelected = TRUE;
					break;
				}
			}
		}	
	}
	m_samPlateDlg.Invalidate(FALSE);

	//�жϸı���Ϣ�Ŀ�λ�Ƿ��ڵ�ǰѡ�п�λ��
	if(bFindSelected)
	{
		OnMsgPlateSiteSelectChanged(0,0);
	}

	return 0;
}

int CProcSetDlg::ResetSamSetDlgTubeInfo()
{
	//������������ҳ�������Ϣ
	if(m_vecPlateSelectedSite.size() > 0)
	{
		vector<CTubeInfo> vecTubeInfo;
		for(int i = 0;i < m_vecPlateSelectedSite.size();++i)
		{
			CTubeInfo tempTubeInfo;
			m_samPlateDlg.GetSamInfo(m_vecPlateSelectedSite[i],tempTubeInfo);
			vecTubeInfo.push_back(tempTubeInfo);
		}

		CMiddleDlg* pMiddleDlg = (CMiddleDlg*)GetParent();
		if(NULL != pMiddleDlg->GetSafeHwnd())
		{
			pMiddleDlg->ResetTubeInfo(m_vecPlateSelectedSite,vecTubeInfo, 0);
		}

		return 0;
	}

	return -1;
}

void CProcSetDlg::OnBnClickedInsertSegment()
{
#ifdef NEW_SEGMENT_INSERTBT
	CPoint pt;
	GetCursorPos(&pt);
	CRect rcBtInsert;
	m_btInsertSegment.GetWindowRect(&rcBtInsert);

	if(pt.x < rcBtInsert.right - RIFHTWIDTH)
	{
		m_segDlg.InsertSegment(EIS_AFTER);
	}
	else
	{
		CMenu menuMain;
		menuMain.CreatePopupMenu();

		menuMain.AppendMenu(MF_STRING,ID_MENU_INSERTSEGBEFORE,theApp.m_pLangInfo->GetText(254));
		menuMain.AppendMenu(MF_STRING,ID_MENU_INSERTSEGAFTER,theApp.m_pLangInfo->GetText(255));
		menuMain.AppendMenu(MF_STRING,ID_MENU_INSERTSEGLAST,theApp.m_pLangInfo->GetText(256));


		//m_bMenuPopUped = TRUE;
		menuMain.TrackPopupMenu(TPM_RIGHTBUTTON,rcBtInsert.left,rcBtInsert.bottom,this);
		menuMain.DestroyMenu();
	}
#else
	m_segDlg.InsertSegment();
#endif
	return;
}

void CProcSetDlg::OnBnClickedInsertStep()
{
#ifdef NEW_SEGMENT_INSERTBT
	CPoint pt;
	GetCursorPos(&pt);
	CRect rcBtStep;
	m_btInsertStep.GetWindowRect(&rcBtStep);
	if(pt.x < rcBtStep.right - RIFHTWIDTH)
	{
		m_segDlg.InsertStep(EIS_AFTER);
	}
	else
	{
		CMenu menuMain;
		menuMain.CreatePopupMenu();

		menuMain.AppendMenu(MF_STRING,ID_MENU_INSERTSTEPBEFORE,theApp.m_pLangInfo->GetText(257));
		menuMain.AppendMenu(MF_STRING,ID_MENU_INSERTSTEPAFTER,theApp.m_pLangInfo->GetText(258));
		menuMain.AppendMenu(MF_STRING,ID_MENU_INSERTSTEPLAST,theApp.m_pLangInfo->GetText(256));

		menuMain.TrackPopupMenu(TPM_RIGHTBUTTON,rcBtStep.left,rcBtStep.bottom,this);
		menuMain.DestroyMenu();
	}
#else
	m_segDlg.InsertStep();
#endif
	return;
}

void CProcSetDlg::OnBnClickedEditStep()
{
	CLinkList<CSegItemInfo>& lnkSegItem = m_segDlg.m_lnkSegItemInfo;
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	if(lnkSegItem.GetSize() < 1)
	{
		PopNormalTipInfo(pLangInfo->GetText(240));
		return;
	}

	Node<CSegItemInfo>* pNodeSegItem = lnkSegItem.GetHead();
	int segIndex = 0;
	while(NULL != pNodeSegItem)
	{
		Node<CStepItemInfo>* pNodeStepItem = pNodeSegItem->data.m_lnkStepItemInfo.GetHead();
		int stepIndex = 0;
		while(NULL != pNodeStepItem)
		{
			if(pNodeStepItem->data.m_bSel)
			{
				m_segDlg.EditStep(segIndex,stepIndex);
				return;
			}
			pNodeStepItem = pNodeStepItem->pNext;
			++stepIndex;
		}
		pNodeSegItem = pNodeSegItem->pNext;
		++segIndex;
	}

	PopNormalTipInfo(pLangInfo->GetText(241));
	return;
}

void CProcSetDlg::OnBnClickedEditSegment()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CLinkList<CSegItemInfo>& lnkSegItem = m_segDlg.m_lnkSegItemInfo;

	if(lnkSegItem.GetSize() < 1)
	{
		PopNormalTipInfo(pLangInfo->GetText(240));
		return;
	}

	Node<CSegItemInfo>* pNodeSegItem = lnkSegItem.GetHead();
	int segIndex = 0;
	while(NULL != pNodeSegItem)
	{
		Node<CStepItemInfo>* pNodeStepItem = pNodeSegItem->data.m_lnkStepItemInfo.GetHead();
		int stepIndex = 0;
		while(NULL != pNodeStepItem)
		{
			if(pNodeStepItem->data.m_bSel)
			{
				m_segDlg.EditSegment(segIndex);
				return;
			}
			pNodeStepItem = pNodeStepItem->pNext;
			++stepIndex;
		}
		pNodeSegItem = pNodeSegItem->pNext;
		++segIndex;
	}

	PopNormalTipInfo(pLangInfo->GetText(242));
	return;
}

void CProcSetDlg::OnBnClickedDelete()
{
	m_segDlg.DeleteSelectedItem();
	return;
}

void CProcSetDlg::OnBnClickedAddCycle()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return;
	}

	int iSelSegNum = m_segDlg.GetSelectedSegmentCount();

	if(1 != iSelSegNum)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(437));
		return;
	}

	int iSegId = m_segDlg.GetFirstSelectedSegmentId();
	if(iSegId < 0)
	{
		return;
	}

	CSegItemInfo& segItem = *m_segDlg.m_lnkSegItemInfo[iSegId];
	if(segItem.GetBeMeltSeg())
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(443));
		return;
	}
	
	if(iSegId > m_pParent->m_iCurSeg)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(442));
		return;
	}

	CMySegEditInfo segEditInfo;
	segEditInfo.m_strSegName = segItem.m_strSegName;
	segEditInfo.m_cycleNum = segItem.m_iCycleNum;

	CAddCycleDlg addCycleDlg(segEditInfo);
	if(IDOK != addCycleDlg.DoModal())
	{
		return;
	}

	int iCycleChangeNum = addCycleDlg.GetCycleChangeNum();
	if(0 == iCycleChangeNum)
	{
		return;
	}

	if(iSegId > m_pParent->m_iCurSeg)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(442));
		return;
	}

	if(segItem.m_iCycleNum + iCycleChangeNum < m_pParent->m_iCurCycle)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(444));
		return;
	}

	if(0 != m_pParent->m_pExpData->AddCycle(iSegId,iCycleChangeNum))
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(445));
		return;
	}

	//ˢ�����̱༭�ؼ���ʾ
	segItem.m_iCycleNum += iCycleChangeNum;
	m_segDlg.Invalidate(FALSE);

	m_pParent->CycleAddSuccessful(iSegId,iCycleChangeNum);

	//����ʵ����ʱ��
	if(NULL != m_pParent->m_pRunCtrl)
	{
		CInstancePCR* pInstance = m_pParent->m_pRunCtrl->GetInstancePCR();
		if(NULL != pInstance)
		{
			CExperimentInfo* pExpInfo = m_pParent->m_pExpData->GetExperimentInfo();
			if(NULL != pExpInfo)
			{
				int iExpTotalSec = pInstance->CalculateExperimentTotalTimeSec(m_pParent->m_pExpData->GetSegment(),pExpInfo->GetLidHeader(),20);
				if(pInstance->GetBeN96PCR())	//96��QPCR���Ӻ���������Ķ�����ʱʱ��
				{
					iExpTotalSec += m_pParent->m_pExpData->GetTotalVolumeDelaySec(TRUE);
				}
				pExpInfo->SetTotalSec(iExpTotalSec);
				m_pParent->RefreshExpTotalTime(iExpTotalSec);
			}
		}
	}

	//ˢ�����ߺ�������Ϣ
	m_pParent->SetCurveHoriAxisParam();

	PopNormalTipInfo(theApp.m_pLangInfo->GetText(446));

	return;
}

void CProcSetDlg::OnFileMenuSegInsertBefore()
{
	m_segDlg.InsertSegment(EIS_BEFORE);
}

void CProcSetDlg::OnFileMenuSegInsertAfter()
{
	m_segDlg.InsertSegment(EIS_AFTER);
}

void CProcSetDlg::OnFileMenuSegInsertLast()
{
	m_segDlg.InsertSegment(EIS_LAST);
}

void CProcSetDlg::OnFileMenuStepInsertBefore()
{
	m_segDlg.InsertStep(EIS_BEFORE);
}

void CProcSetDlg::OnFileMenuStepInsertAfter()
{
	m_segDlg.InsertStep(EIS_AFTER);
}

void CProcSetDlg::OnFileMenuStepInsertLast()
{
	m_segDlg.InsertStep(EIS_LAST);
}

LRESULT CProcSetDlg::OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam)
{
	m_samPlateDlg.GetPlateSelectedSite(m_vecPlateSelectedSite);
	if(m_vecPlateSelectedSite.size() < 1)
	{
		m_samBaseInfoEditDlg.SetDefaultColor();
		m_samBaseInfoEditDlg.SetEditableState(FALSE);
		return 0;
	}

	if(!m_samBaseInfoEditDlg.GetEditableState())
	{
		m_samBaseInfoEditDlg.SetEditableState(TRUE);
	}

	CTubeInfo tubeInfo,*pTubeInfo = NULL;
	BOOL bDensityEditable = TRUE;
	int iSeletedSiteCount = m_vecPlateSelectedSite.size();

	if(1 == iSeletedSiteCount)
	{
		m_samPlateDlg.GetSamInfo(m_vecPlateSelectedSite[0],tubeInfo);
	}
	else
	{
		m_samPlateDlg.GetSameTubeInfo(tubeInfo);

		//tubeInfo������ͨ����Ϣ���ж�Ũ���Ƿ�ɱ༭
		for(int i = 0;i < iSeletedSiteCount;++i)
		{
			pTubeInfo = m_samPlateDlg.GetTubeInfoPtr();
			if(NULL == pTubeInfo)
			{
				bDensityEditable = FALSE;
				break;
			}

			eSampleType nSamType = (pTubeInfo[m_vecPlateSelectedSite[i]]).GetSampleType();
			if(SAMPLE_TYPE_STANDARD != nSamType && SAMPLE_TYPE_CALIBRATOR != nSamType)
			{
				bDensityEditable = FALSE;
				break;
			}
		}
	}
	
	//ѡ�ж������ʱ������Ũ���еı༭״̬
	if(iSeletedSiteCount > 1)
	{
		m_samBaseInfoEditDlg.SetTubeInfo(tubeInfo,FALSE);
		m_samBaseInfoEditDlg.SetDensityEditableState(bDensityEditable);
	}
	else
	{
		m_samBaseInfoEditDlg.SetTubeInfo(tubeInfo,TRUE);
	}

	return 1;
}

LRESULT CProcSetDlg::OnMsgRowSelectedChange(WPARAM wParam, LPARAM lParam)
{
	if(theApp.GetBeIVDUser())
	{
#ifdef IVD_USE_TEMPALTE_LIST
		if(0 != HandleTemplateFileLstRowSelectChanged())
		{
			return 1;
		}
		else
		{
			return 0;
		}
#else
		if(0 != HandleProjectLstRowSelectChanged())
		{
			return 1;
		}
		else
		{
			return 0;
		}
#endif
	}
	else
	{
		if(0 != HandleProjectLstRowSelectChanged())
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

LRESULT CProcSetDlg::OnMsgSamplateDeleteKeyPressed(WPARAM wParam, LPARAM lParam)
{
	if(NULL != m_pParent->GetSafeHwnd())
	{
		m_pParent->Delete();
	}
	return 0;
}

int CProcSetDlg::SimulatePlateSiteSelectChanged()
{
	m_samPlateDlg.GetPlateSelectedSite(m_vecPlateSelectedSite);
	if(m_vecPlateSelectedSite.size() < 1)
	{
		m_samBaseInfoEditDlg.SetDefaultColor();
		m_samBaseInfoEditDlg.SetEditableState(FALSE);
		return 0;
	}

	if(!m_samBaseInfoEditDlg.GetEditableState())
	{
		m_samBaseInfoEditDlg.SetEditableState(TRUE);
	}

	CTubeInfo tubeInfo,*pTubeInfo = NULL;
	BOOL bDensityEditable = TRUE;
	int iSeletedSiteCount = m_vecPlateSelectedSite.size();

	if(1 == iSeletedSiteCount)
	{
		m_samPlateDlg.GetSamInfo(m_vecPlateSelectedSite[0],tubeInfo);
	}
	else
	{
		m_samPlateDlg.GetSameTubeInfo(tubeInfo);

		//tubeInfo������ͨ����Ϣ���ж�Ũ���Ƿ�ɱ༭
		for(int i = 0;i < iSeletedSiteCount;++i)
		{
			pTubeInfo = m_samPlateDlg.GetTubeInfoPtr();
			if(NULL == pTubeInfo)
			{
				bDensityEditable = FALSE;
				break;
			}

			eSampleType nSamType = (pTubeInfo[m_vecPlateSelectedSite[i]]).GetSampleType();
			if(SAMPLE_TYPE_STANDARD != nSamType && SAMPLE_TYPE_CALIBRATOR != nSamType)
			{
				bDensityEditable = FALSE;
				break;
			}
		}
	}

	//ѡ�ж������ʱ������Ũ���еı༭״̬
	if(iSeletedSiteCount > 1)
	{
		m_samBaseInfoEditDlg.SetTubeInfo(tubeInfo,FALSE);
		m_samBaseInfoEditDlg.SetDensityEditableState(bDensityEditable);
	}
	else
	{
		m_samBaseInfoEditDlg.SetTubeInfo(tubeInfo,TRUE);
	}

	return 1;
}

void CProcSetDlg::ClearPage(BOOL bRedraw /*= TRUE*/)
{
	m_segDlg.Clear(bRedraw);
	//m_samPlateDlg.Clear(bRedraw);
	m_samBaseInfoEditDlg.Clear();
	//������
}

void CProcSetDlg::ClearExperiment(BOOL bRedraw /*= TRUE*/)
{
	m_segDlg.Clear(bRedraw);
	m_samPlateDlg.DeleteAllTubeInfo();
	m_samBaseInfoEditDlg.Clear();
	m_prjBaseInfoDlg.ClearPage();

	return;
}

int CProcSetDlg::FileOpened(CExperimentData* pExpData /*= NULL*/)
{
	ClearPage();
	m_segDlg.SetSegmentInfo(pExpData->GetSegment(),TRUE);

	vector<CTubeInfo*> vecTubeInfoPtr;
	pExpData->GetAllTubeInfoPtr(vecTubeInfoPtr);
	m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,TRUE);

	//������Ŀ������Ϣ
	m_prjBaseInfoDlg.SetPrjBaseInfo(pExpData->GetReagentItem());

	return 0;
}

int CProcSetDlg::SavePrj(int iIndex,BOOL bNew)
{
	if(iIndex < 0)
	{
		return -1;
	}
	int iRowNum = m_lstSelPrj.GetRowNum();
	if(iIndex >= iRowNum + 1)
	{
		return -2;
	}

	CReagentItemManager* pItemManager = CReagentItemManager::GetInstance();
	if(NULL == pItemManager)
	{
		return -3;
	}

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
	{
		return -4;
	}

	CReagentItem* pReagentItem = (CReagentItem*)pAryItem->GetAt(iIndex);
	if(NULL == pReagentItem)
	{
		return -5;
	}

	CString strTemp(_T(""));
	if(bNew)
	{
		m_lstSelPrj.InsertRowSetHeight(iIndex,54 * CFontSet::m_dVertRatio);
		strTemp.Format(_T("%d"),iIndex + 1);
		m_lstSelPrj.SetItemText(iIndex,1,strTemp);
	}
	else
	{
		vector<int> vecSelectedRowId = m_lstSelPrj.GetSelectedRowId();
		if(vecSelectedRowId.size() > 0)
		{
			int curSel = vecSelectedRowId[0];
			if(curSel == iIndex)
			{
				//����������Ŀ��Ϣ��ʵ��������
				SetProjectInfo(*pReagentItem);
			}
		}
	}
	
	m_lstSelPrj.SetItemText(iIndex,2,pReagentItem->GetName(),TRUE);

	return 0;
}

int CProcSetDlg::DeletePrj(int iIndex)
{
	if(iIndex < 0)
	{
		return -1;
	}
	int iRowNum = m_lstSelPrj.GetRowNum();
	if(iIndex >= iRowNum)
	{
		return -2;
	}

	m_lstSelPrj.DeleteRow(iIndex,FALSE);

	iRowNum = m_lstSelPrj.GetRowNum();
	CString strTemp(_T("")); 
	for(int i = iIndex;i < iRowNum;++i)
	{
		strTemp.Format(_T("%d"),i + 1);
		m_lstSelPrj.SetItemText(i,1,strTemp);
	}
	m_lstSelPrj.Invalidate(FALSE);

	return 0;
}

int CProcSetDlg::SetSelectPrjLstInfo()
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

		m_lstSelPrj.InsertRowSetHeight(i,54 * CFontSet::m_dVertRatio);
		strTemp.Format(_T("%d"),i + 1);
		m_lstSelPrj.SetItemText(i,1,strTemp);
		m_lstSelPrj.SetItemText(i,2,pReagentItem->GetName());
	}

	return 0;
}

int CProcSetDlg::SetSelectPrjLstInfoIVDNoProject()
{
	m_lstSelPrj.Clear();
	CString strTemp(_T(""));

	int iTemNum = theApp.m_lnkTotalTemFileInfo.GetSize();
	for(int i = 0;i < iTemNum;++i)
	{
		m_lstSelPrj.InsertRowSetHeight(i,54 * CFontSet::m_dVertRatio);
		strTemp.Format(_T("%d"),i + 1);
		m_lstSelPrj.SetItemText(i,1,strTemp);
		m_lstSelPrj.SetItemText(i,2,theApp.m_lnkTotalTemFileInfo[i]->m_strTitle);
	}

	m_lstSelPrj.InsertRowSetHeight(iTemNum,54 * CFontSet::m_dVertRatio);
	strTemp.Format(_T("%d"),iTemNum + 1);
	m_lstSelPrj.SetItemText(iTemNum,1,strTemp);
	m_lstSelPrj.SetItemText(iTemNum,2,_T("NONE"));

	return 0;
}

int CProcSetDlg::SetProjectInfo(CReagentItem& reagentItem)
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}
	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	CString strTemp(_T(""));

	m_prjBaseInfoDlg.m_edPrjName.SetText(reagentItem.GetName());
	m_prjBaseInfoDlg.m_cbPrjType.SetCurSel(reagentItem.GetType(),TRUE,TRUE);
	strTemp.Format(_T("%d"),reagentItem.GetReactVolume());
	m_prjBaseInfoDlg.m_edReactSystem.SetText(strTemp);
	m_prjBaseInfoDlg.m_edUnit.SetText(reagentItem.GetUnit());

	m_segDlg.SetSegmentInfo(reagentItem.GetSegment(),TRUE);

	//����ͨ��ѡ����Ϣ
	CReagentItem* pReagentItem = m_pParent->m_pExpData->GetReagentItem();
	if(NULL != pReagentItem)
	{
		//pReagentItem->CopyChanTargetInfo(&reagentItem);
		pReagentItem->CopyFrom(&reagentItem);

		//���ÿ�λͨ����Ϣ
		int iCount = reagentItem.GetChannelTargetCount();
		CItemChannelInfo* pChannel = NULL;
		for (int i = 0;i < iCount;++i)
		{
			pChannel = reagentItem.GetChannelTargetBy(i);
			if(NULL != pChannel)
			{
				m_samPlateDlg.SetAllTubeChanInfo(i,pChannel->GetUse(),pChannel->GetDyeName(),pChannel->GetTargetName(0));
			}
		}
	}

	//��������/������������
	CDataAnalysisPara* pAnalysisPara = m_pParent->m_pExpData->GetAnalysisPara();
	if(NULL == pAnalysisPara)
	{
		pAnalysisPara = new CDataAnalysisPara;
	}
	if(NULL != pAnalysisPara)
	{
		pAnalysisPara->CopyFrom(reagentItem.GetAnalysisPara());
	}

	//���ƴ���ϵ��
	m_pParent->m_pExpData->SetChannelCrossTalkParam(reagentItem.GetCrosstalkPara(), reagentItem.GetSelChannelCount());

	//ˢ�¿�λͨ����ʾ
	SimulatePlateSiteSelectChanged();

	return 0;
}

int CProcSetDlg::HandleProjectLstRowSelectChanged()
{
	int iRowNum = m_lstSelPrj.GetRowNum();
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
		return 2;
	}

	CPtrArray* pAryItem = pItemManager->GetArrayItems();
	if(NULL == pAryItem)
	{
		return 3;
	}

	CReagentItem* pReagentItem = (CReagentItem*)pAryItem->GetAt(vecSelectedRowId[0]);
	if(NULL == pReagentItem)
	{
		return 4;
	}

	SetProjectInfo(*pReagentItem);

	return 0;
}

int CProcSetDlg::HandleTemplateFileLstRowSelectChanged()
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	int iRowNum = m_lstSelPrj.GetRowNum();
	vector<int> vecSelectedRowId;
	BOOL bSelected = FALSE;

	vecSelectedRowId = m_lstSelPrj.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		return -3;
	}

	if(vecSelectedRowId[0] == m_lstSelPrj.GetRowNum() - 1)
	{
		//���
		if(NULL != m_pParent->GetSafeHwnd())
		{
			m_pParent->ClearExperiment();
		}

		return 0;
	}

	if(vecSelectedRowId[0] >= theApp.m_lnkTotalTemFileInfo.GetSize())
	{
		return -4;
	}

	CTemplateFileInfo& temFileInfo = *theApp.m_lnkTotalTemFileInfo[vecSelectedRowId[0]];
	if(!temFileInfo.IsFileExist())
	{
		return -5;
	}

#ifdef _DEBUG
	clock_t ckStart = clock();
#endif
	CExperimentData expData;
	if(0 != expData.OpenFile(temFileInfo.m_strFilePath,TRUE))
	{
		return -6;
	}

#ifdef _DEBUG
	clock_t ckExist = clock() - ckStart;
#endif

	/*if(0 != m_pParent->m_pExpData->OpenFile(temFileInfo.m_strFilePath,TRUE))
	{
		return -6;
	}*/
	
	vector<CTubeInfo*> vecPTubeInfo;
	expData.GetAllTubeInfoPtr(vecPTubeInfo);
	m_pParent->m_pExpData->SetTubeInfo(vecPTubeInfo);
	m_pParent->m_pExpData->SetSegment(expData.GetSegment());
	m_pParent->m_pExpData->SetBeTemplateFile(TRUE);
	m_pParent->m_pExpData->GetExperimentInfo()->CopyExperiment(*expData.GetExperimentInfo());

#ifdef _DEBUG
	clock_t ckExist1 = clock() - ckStart;
#endif

#ifdef PS96INSTRUMENT
	//PS96 IVDģʽ����ʾУ׼ͨ��
	if(theApp.GetBeIVDUser())
	{
		m_pParent->m_pExpData->SetTubeChannelSelectState(5,FALSE);
	}
#endif

#ifdef _DEBUG
	clock_t ckExist2 = clock() - ckStart;
#endif

	CReagentItem* pReagentItem = m_pParent->m_pExpData->GetReagentItem();
	CReagentItem* pReagentItemSource = expData.GetReagentItem();
	pReagentItem->SetName(pReagentItemSource->GetName());
	pReagentItem->SetType(pReagentItemSource->GetType());
	pReagentItem->SetReactVolume(pReagentItemSource->GetReactVolume());
	pReagentItemSource->SetUnit(pReagentItemSource->GetUnit());
	pReagentItem->CopyChanTargetInfo(pReagentItemSource);

	//����ģ���ļ��е���Ϣ
	m_pParent->FileOpened();

	//ˢ�¿�λͨ����ʾ
	SimulatePlateSiteSelectChanged();

	if(IDYES == PopQueryTipInfo(theApp.m_pLangInfo->GetText(298)))
	{
		m_pParent->Import();
	}

	return 0;
}

void CProcSetDlg::SetTemplateListSelectedRow(int iIndex)
{
	vector<int> vecSelId;
	vecSelId.push_back(iIndex);
	m_lstSelPrj.SetSelectedRowId(vecSelId,FALSE);
	m_lstSelPrj.SetCellCheckState(iIndex,0,TRUE,TRUE);
	return;
}

int CProcSetDlg::NewTemplateAdded(int iIndex,BOOL bRedraw /*= TRUE*/)
{
	CString strTemp(_T(""));
	m_lstSelPrj.InsertRowSetHeight(iIndex,54 * CFontSet::m_dVertRatio);
	if(NULL != theApp.m_lnkTotalTemFileInfo[iIndex])
	{
		strTemp = theApp.m_lnkTotalTemFileInfo[iIndex]->m_strTitle;
	}
	m_lstSelPrj.SetItemText(iIndex,2,strTemp);

	int iNewRowNum = m_lstSelPrj.GetRowNum();
	for(int i = iIndex;i < iNewRowNum;++i)
	{
		strTemp.Format(_T("%d"),i + 1);
		m_lstSelPrj.SetItemText(i,1,strTemp);
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		Invalidate(FALSE);
	}

	return 0;
}

int CProcSetDlg::SetGradientFuncShieldState(BOOL bData)
{
	m_segDlg.SetShieldGradientSetState(bData);
	return 0;
}

int CProcSetDlg::SetTemperatureRateShowState(BOOL bData)
{
	m_segDlg.SetTemperatureRateShowState(bData);
	return 0;
}

int CProcSetDlg::GetAllSampleType(vector<eSampleType>& vecSamType)
{
	return m_samPlateDlg.GetAllSampleType(vecSamType);
}

void CProcSetDlg::ResetLstTitle()
{
	m_lstSelPrj.SetColumnText(0,theApp.m_pLangInfo->GetText(28));
	m_lstSelPrj.SetColumnText(1,theApp.m_pLangInfo->GetText(29));

#ifdef PS96INSTRUMENT
	if(theApp.GetBeIVDUser())
	{
		m_lstSelPrj.SetColumnText(2,theApp.m_pLangInfo->GetText(296));
	}
	else
	{
		m_lstSelPrj.SetColumnText(2,theApp.m_pLangInfo->GetText(30));
	}
#else
	m_lstSelPrj.SetColumnText(2,theApp.m_pLangInfo->GetText(30),FALSE);
#endif
}

int CProcSetDlg::RefreshLanguageDisplay()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	BOOL bEnglish = theApp.GetBeEnglish();

	SetMyFont(m_stTxFont);

	int iFontH = 13 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_btInsertSegment.SetMyFont(m_stTxFont,iFontH);
	m_btInsertSegment.SetWindowText(pLangInfo->GetText(36));

	m_btInsertStep.SetMyFont(m_stTxFont,iFontH);
	m_btInsertStep.SetWindowText(pLangInfo->GetText(41));

	m_btEditStep.SetMyFont(m_stTxFont,iFontH);
	m_btEditStep.SetWindowText(pLangInfo->GetText(44));

	m_btEditSegment.SetMyFont(m_stTxFont,iFontH);
	m_btEditSegment.SetWindowText(pLangInfo->GetText(40));

	m_btDelete.SetMyFont(m_stTxFont,iFontH);
	m_btDelete.SetWindowText(pLangInfo->GetText(45));

	m_btAddCycle.SetMyFont(m_stTxFont,iFontH);
	m_btAddCycle.SetWindowText(pLangInfo->GetText(46));

	iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 15 * m_dTxRatio;
	}
	ResetLstTitle();
	m_lstSelPrj.SetMyTitleFont(m_stTxFont,iFontH,FW_MEDIUM);
	m_lstSelPrj.SetMyFont(m_stTxFont,iFontH);
	m_lstSelPrj.SetMyHeaderFont(m_stTxFont,iFontH);

	iFontH = 18 * m_dTxRatio;
	int iFontAbbrH = 14 * m_dTxRatio;
	m_samPlateDlg.SetMyFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFontH,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFontAbbrH);

	iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 15 * m_dTxRatio;
	}
	m_segDlg.m_fontStepTag.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.m_fontGeneral.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.m_fontSegTag.SetMyFont(m_stTxFont,iFontH,FW_MEDIUM);
	m_segDlg.SetFuncEditCtrlFont(m_stTxFont,iFontH);

	m_segDlg.SetSegmentString(theApp.m_pLangInfo->GetText(57));
	m_segDlg.SetStepString(theApp.m_pLangInfo->GetText(58));
	m_segDlg.SetGradientString(theApp.m_pLangInfo->GetText(449));
	m_segDlg.SetColumnString(theApp.m_pLangInfo->GetText(450));
	m_segDlg.SetRangeString(theApp.m_pLangInfo->GetText(451));
	m_segDlg.CorrectSegmentName();

	iFontH = 13 * m_dTxRatio;
	if(bEnglish)
	{
		m_segLgdInfo.m_iOneCharW = 10 * m_dHoriRatio;
		iFontH = 15 * m_dTxRatio;
	}
	else
	{
		m_segLgdInfo.m_iOneCharW = 14 * m_dHoriRatio;
	}
	CLegendInfo::ResetSegLegendText(&m_segLgdInfo);
	m_segLgdInfo.SetMyFont(m_stTxFont,iFontH);

	m_samBaseInfoEditDlg.RefreshLanguageDisplay();
	m_prjBaseInfoDlg.RefreshLanguageDisplay();

	return 0;
}

int CProcSetDlg::SetEditMode(EEXPEDITMODE eExpEditMode)  
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	if(EEEM_IVD == m_eExpEditMode)	//����ʱ��IVDģʽ
	{
		m_lstSelPrj.SetBeEditable(TRUE,TRUE);
		m_segDlg.SetBeEditable(FALSE);
		m_btInsertSegment.EnableWindow(FALSE);
		m_btInsertStep.EnableWindow(FALSE);
		m_btEditStep.EnableWindow(FALSE);
		m_btEditSegment.EnableWindow(FALSE);
		m_btDelete.EnableWindow(FALSE);
	}
	else if(EEEM_NOLIMIT == m_eExpEditMode)
	{
		m_lstSelPrj.SetBeEditable(TRUE,TRUE);
		m_segDlg.SetBeEditable(TRUE);
		m_btInsertSegment.EnableWindow(TRUE);
		m_btInsertStep.EnableWindow(TRUE);
		m_btEditStep.EnableWindow(TRUE);
		m_btEditSegment.EnableWindow(TRUE);
		m_btDelete.EnableWindow(TRUE);
	}
	else if(EEEM_RUNNING == m_eExpEditMode)
	{
		m_lstSelPrj.SetBeEditable(FALSE,TRUE);
		m_segDlg.SetBeEditable(FALSE);
		m_btInsertSegment.EnableWindow(FALSE);
		m_btInsertStep.EnableWindow(FALSE);
		m_btEditStep.EnableWindow(FALSE);
		m_btEditSegment.EnableWindow(FALSE);
		m_btDelete.EnableWindow(FALSE);
		m_btAddCycle.EnableWindow(TRUE);
	}
	else if(EEEM_END == m_eExpEditMode)
	{
#ifdef PS96INSTRUMENT
		if(theApp.GetBeIVDUser())
		{
			m_lstSelPrj.SetBeEditable(FALSE,TRUE);
			m_segDlg.SetBeEditable(FALSE);
			m_btInsertSegment.EnableWindow(FALSE);
			m_btInsertStep.EnableWindow(FALSE);
			m_btEditStep.EnableWindow(FALSE);
			m_btEditSegment.EnableWindow(FALSE);
			m_btDelete.EnableWindow(FALSE);
		}
		else
		{
			m_lstSelPrj.SetBeEditable(TRUE,TRUE);
			m_segDlg.SetBeEditable(TRUE);
			m_btInsertSegment.EnableWindow(TRUE);
			m_btInsertStep.EnableWindow(TRUE);
			m_btEditStep.EnableWindow(TRUE);
			m_btEditSegment.EnableWindow(TRUE);
			m_btDelete.EnableWindow(TRUE);
		}
#else
		m_lstSelPrj.SetBeEditable(TRUE,TRUE);
		m_segDlg.SetBeEditable(TRUE);
		m_btInsertSegment.EnableWindow(TRUE);
		m_btInsertStep.EnableWindow(TRUE);
		m_btEditStep.EnableWindow(TRUE);
		m_btEditSegment.EnableWindow(TRUE);
		m_btDelete.EnableWindow(TRUE);
#endif
		m_btAddCycle.EnableWindow(FALSE);
	}

	m_prjBaseInfoDlg.SetEditMode(eExpEditMode);
	m_samBaseInfoEditDlg.SetEditMode(eExpEditMode);

	return 0;
}

void CProcSetDlg::SetNewPrjBaseInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/)
{
	if(ITEM_PRJBASE_CBPRJTYPE == eItemInfoType)
	{
		int iIndex = m_prjBaseInfoDlg.m_cbPrjType.GetCurSel();
		if(0 == iIndex || 1 == iIndex)
		{
			m_segDlg.SetDefaultAbsoluteQuantification();
		}
		else if(2 == iIndex)
		{
			m_segDlg.SetDefaultStandardMeltingCurve();
		}
	}
	
	return;
}



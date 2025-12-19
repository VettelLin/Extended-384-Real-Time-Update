// SamSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "SamSetDlg.h"
#include "TubeInfo.h"
#include "afxdialogex.h"


// CSamSetDlg dialog

IMPLEMENT_DYNAMIC(CSamSetDlg, CDialogEx)

CSamSetDlg::CSamSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSamSetDlg::IDD, pParent),m_samInfoDlg(this,this)
{
	m_bCtrlCreated = FALSE;
	m_pParent = NULL;
	if(NULL != pParent)
	{
		m_pParent = (CMiddleDlg*)pParent;
	}
	m_pCurSelSwt = NULL;
	m_pCurSelWnd = NULL;
	m_iSwitchBtBkH = 40;
	m_iSwitchBtH = 32;
	m_iSwitchBtBkW = 244;
	m_iSwitchBtW = 120;
}

CSamSetDlg::~CSamSetDlg()
{

}

void CSamSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSamSetDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_SAMSETDLG_SWTBT_PLATE,&CSamSetDlg::OnBnClickedSwtBtPlate)
	ON_BN_CLICKED(ID_SAMSETDLG_SWTBT_CHART,&CSamSetDlg::OnBnClickedSwtBtChart)
	ON_MESSAGE(WM_TELLPARENTSITESELECTCHANGED,OnMsgPlateSiteSelectChanged) 
	ON_MESSAGE(WM_ROWSELECTCHANGE,OnMsgLstSelectChanged)
	ON_MESSAGE(SAMPLATE_DELETEKEY_PRESSED,OnMsgSamplateDeleteKeyPressed)
END_MESSAGE_MAP()

BOOL CSamSetDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect; 
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	int iTempInt1 = 336 * m_dHoriRatio;

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	DrawRoundRectangleNew(&memDc,16 * m_dHoriRatio,12 * m_dVertRatio,1376 * m_dHoriRatio,rect.Height() - 12 * m_dVertRatio - 8 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	
	DrawRoundRectangleNew(&memDc,32 * m_dHoriRatio,28 * m_dVertRatio,m_iSwitchBtBkW,m_iSwitchBtBkH,8,8,8,8,TRUE,Color(0XE7,0XE7,0XE7),1,TRUE,COLOR_SOFT_THEME,1);

	CRect rcLegend;
	rcLegend.left = 32 * m_dHoriRatio;
	rcLegend.right = rcLegend.left + 1344 * m_dHoriRatio;
	rcLegend.top = 84 * m_dVertRatio + 746 * m_dVertRatio;
	rcLegend.bottom = rcLegend.top + 27 * m_dVertRatio;

	BOOL bShowLegend = (NULL != m_samPlateDlg.GetSafeHwnd() && m_samPlateDlg.IsWindowVisible());
	if(bShowLegend)
	{
		DrawLegend(&memDc,&rcLegend,m_segLgdInfo);
	}

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	
	if(bShowLegend)
	{
		pDC->BitBlt(rcLegend.left,rcLegend.top,rcLegend.Width(),rcLegend.Height(),&memDc,rcLegend.left,rcLegend.top,SRCCOPY);
	}

	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

void CSamSetDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

int CSamSetDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

void CSamSetDlg::OnBnClickedSwtBtPlate()
{
	SetPageSelected(SAMPLATE);
}

void CSamSetDlg::OnBnClickedSwtBtChart()
{
	SetPageSelected(SAMCHART);
}

LRESULT CSamSetDlg::OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam)
{
	if(0 != m_samPlateDlg.GetCurSelectSite(m_vecCurSelSite))
	{
		m_samInfoDlg.ClearSamInfo();
		if(m_samInfoDlg.GetEditable())
		{
			m_samInfoDlg.SetEditable(FALSE);
		}
		return 0;
	}

	vector<int> vecSelRowId;
	for(int i = 0;i < m_vecCurSelSite.size();++i)
	{
		for(int j = 0;j < m_vecPlateSamIndex.size();++j)
		{
			if(m_vecCurSelSite[i] == m_vecPlateSamIndex[j])
			{
				vecSelRowId.push_back(j);
				break;
			}
		}
	}
	m_lstSamInfo.SetSelectedRowId(vecSelRowId);

	if(m_vecCurSelSite.size() > 0)
	{
		if(!m_samInfoDlg.GetEditable())
		{
			m_samInfoDlg.SetEditable(TRUE);
		}
	}
	else
	{
		m_samInfoDlg.SetEditable(FALSE);
	}

	CTubeInfo tubeInfo;
	if(1 == m_vecCurSelSite.size())
	{
		m_samPlateDlg.GetSamInfo(m_vecCurSelSite[0],tubeInfo);
	}
	else
	{
		m_samPlateDlg.GetSameTubeInfo(tubeInfo);
	}
	m_samInfoDlg.SetSamInfo(tubeInfo);

	return 1;
}

LRESULT CSamSetDlg::OnMsgLstSelectChanged(WPARAM wParam,LPARAM lParam)
{
	vector<int> vecSelectedRowId = m_lstSamInfo.GetSelectedRowId();
	if(vecSelectedRowId.size() < 1)
	{
		m_samInfoDlg.ClearSamInfo();
		if(m_samInfoDlg.GetEditable())
		{
			m_samInfoDlg.SetEditable(FALSE);
		}
		return 0;
	}

	m_vecCurSelSite.clear();
	for(int i = 0;i < vecSelectedRowId.size();++i)
	{
		m_vecCurSelSite.push_back(m_vecPlateSamIndex[vecSelectedRowId[i]]);
	}
	m_samPlateDlg.SetCurSelectSite(m_vecCurSelSite);

	if(!m_samInfoDlg.GetEditable())
	{
		m_samInfoDlg.SetEditable(TRUE);
	}

	CTubeInfo tubeInfo;
	if(1 == m_vecCurSelSite.size())
	{
		m_samPlateDlg.GetSamInfo(m_vecCurSelSite[0],tubeInfo);
		m_samInfoDlg.SetSamInfo(tubeInfo);
	}
	else
	{
		m_samInfoDlg.ClearSamInfo();
	}

	return 1;
}

LRESULT CSamSetDlg::OnMsgSamplateDeleteKeyPressed(WPARAM wParam, LPARAM lParam)
{
	if(NULL != m_pParent->GetSafeHwnd())
	{
		m_pParent->Delete();
	}
	return 0;
}

BOOL CSamSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitDlg();
	InitSegLegend();
	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	SetCtrlText();

	SetPageSelected(SAMPLATE);

	return TRUE;
}

void CSamSetDlg::InitDlg()
{

}

void CSamSetDlg::InitSegLegend()
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

	m_segLgdInfo.m_iLeftIndent = 46 * m_dHoriRatio; 
	m_segLgdInfo.m_iRoundRadius = 6 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrIconToTx = 8 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrTxToIcon = 80 * m_dHoriRatio; 

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

void CSamSetDlg::InitSonDlg()
{
	m_samInfoDlg.Create(IDD_MIDDLEDLG,this);
	m_samInfoDlg.SetEditable(FALSE);
}

void CSamSetDlg::CreateCtrl()
{
	CreateBt();
	CreateLst();
	CreateSamPlateDlg();
}

void CSamSetDlg::CreateBt()
{
	m_swtBtPlate.Create(_T("�װ�ģʽ"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SAMSETDLG_SWTBT_PLATE);
	m_swtBtChart.Create(_T("����ģʽ"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SAMSETDLG_SWTBT_CHART);
}

void CSamSetDlg::CreateLst()
{
	m_lstSamInfo.Create(IDD_MIDDLEDLG,this);
}

void CSamSetDlg::CreateSamPlateDlg()
{
	m_samPlateDlg.Create(IDD_MIDDLEDLG,this);
}

void CSamSetDlg::InitCtrl()
{
	InitBt();
	InitLst();
	InitSamPlateDlg();
}

void CSamSetDlg::InitBt()
{
	int iFontH = 16 * m_dTxRatio;

	m_swtBtPlate.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtPlate.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtPlate.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtPlate.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtPlate.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	
	m_swtBtChart.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtChart.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtChart.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtChart.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtChart.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
}

void CSamSetDlg::InitLst()
{
	int iFtH = 14 * m_dTxRatio,iFtDensity = 12 * m_dTxRatio;
	if(theApp.GetBeEnglish())
	{
		iFtH = 15 * m_dTxRatio;
	}

	m_lstSamInfo.SetHeaderHeight(24 * m_dVertRatio);
	m_lstSamInfo.SetMyHeaderFont(m_stTxFont,iFtH);
	m_lstSamInfo.SetMyFont(m_stTxFont,iFtH);
	m_lstSamInfo.SetMyDensityFont(m_stTxFont,iFtDensity);
	m_lstSamInfo.SetTextClr1(RGB(0X59,0X59,0X59));
	m_lstSamInfo.SetItemHeight(54 * m_dVertRatio);

	int iTempInt1 = 34 * m_dVertRatio;
	m_lstSamInfo.m_horiScroll.m_arrowWH = iTempInt1;
	m_lstSamInfo.m_horiScroll.m_scrollH = iTempInt1;
	m_lstSamInfo.m_vertScroll.m_arrowWH = iTempInt1;
	m_lstSamInfo.m_vertScroll.m_scrollW = iTempInt1;

	m_lstSamInfo.InsertColumn(0, _T(""));
	m_lstSamInfo.InsertColumn(1, _T(""));
	m_lstSamInfo.InsertColumn(2, _T(""));
	m_lstSamInfo.InsertColumn(3, _T(""));
	m_lstSamInfo.InsertColumn(4, _T(""));
	m_lstSamInfo.InsertColumn(5, _T(""));
	m_lstSamInfo.InsertColumn(6,_T(""));
	m_lstSamInfo.InsertColumn(7, _T(""));
	m_lstSamInfo.InsertColumn(8, _T(""));
	m_lstSamInfo.InsertColumn(9, _T(""));
	m_lstSamInfo.InsertColumn(10, _T(""));
	m_lstSamInfo.InsertColumn(11, _T(""));
	m_lstSamInfo.InsertColumn(12, _T(""));
	m_lstSamInfo.InsertColumn(13, _T(""));
	m_lstSamInfo.InsertColumn(14, _T(""));
	m_lstSamInfo.InsertColumn(15, _T(""));
	m_lstSamInfo.InsertColumn(16, _T(""));
	m_lstSamInfo.InsertColumn(17, _T(""));

	m_lstSamInfo.SetBeTellParentRowSelChange(TRUE);
	m_lstSamInfo.SetColumnMultiDensityState(5,TRUE);

	ResetLstColumnWidth();

	return;
}

void CSamSetDlg::InitSamPlateDlg()
{
	int iFtH = 18 * m_dTxRatio;
	int iFtH2 = 18 * m_dTxRatio;
	int iFtAbbrH = 14 * m_dTxRatio;

	SetRowColNum(32, 12);

	m_samPlateDlg.SetBkColor(RGB(0XE6,0XF4,0XF5));
	m_samPlateDlg.SetRowHeaderWidth(SAMPLATE_WH * m_dHoriRatio);
	m_samPlateDlg.SetColumnHeaderHeight(SAMPLATE_WH * m_dVertRatio);
	m_samPlateDlg.SetFixedRowHeightColumnWidth(1 * m_dVertRatio,4 * m_dHoriRatio);
	m_samPlateDlg.SetRectWidth(100 * m_dHoriRatio);
	m_samPlateDlg.SetRectHeight(70 * m_dVertRatio);

#ifdef SAMPLATE_16SITE
	m_samPlateDlg.SetInterval(10 * m_dHoriRatio,16 * m_dVertRatio);
#else
	m_samPlateDlg.SetInterval(1 * m_dHoriRatio,1 * m_dVertRatio);
#endif

	m_samPlateDlg.SetMyFont(m_stTxFont,iFtH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFtH2);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFtH2,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFtAbbrH);
	m_samPlateDlg.SetBeTellParentRowSelChange(TRUE);
	m_samPlateDlg.SetBeSingleSelect(TRUE);
	m_samPlateDlg.SetFourAngleBkColor(RGB(0XFF,0XFF,0XFF));
	m_samPlateDlg.SetBeShowBottomText(TRUE);
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

void CSamSetDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

	int iTempInt3 = (m_iSwitchBtBkW - m_iSwitchBtW * 2) / 2;;
	int iXStart = 16 * m_dHoriRatio + iTempInt3;
	int iYStart = 14 * m_dHoriRatio + (m_iSwitchBtBkH - m_iSwitchBtH) / 2;

	m_rcManage.AddRectToDelete(iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH);
	m_swtBtPlate.SetWindowPos(this,iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH);
	m_swtBtChart.SetWindowPos(this,iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);

	int iTempInt = rect.Height() - 42 * m_dVertRatio - 4 * m_dVertRatio - 1;

	m_rcManage.AddRectToDelete(16 * m_dHoriRatio,42 * m_dVertRatio,1344 * m_dHoriRatio,iTempInt);
	m_samPlateDlg.SetWindowPos(this,16 * m_dHoriRatio,42 * m_dVertRatio,1344 * m_dHoriRatio,746 * m_dVertRatio,SWP_NOZORDER);
	m_lstSamInfo.SetWindowPos(this,316 * m_dHoriRatio,42 * m_dVertRatio,1344 * m_dHoriRatio,iTempInt,SWP_NOZORDER);

	int iTempInt2 = rect.Height() - 6 * m_dVertRatio - 4 * m_dVertRatio - 1;

	m_rcManage.AddRectToDelete(1408 * m_dHoriRatio,12 * m_dVertRatio,360 * m_dHoriRatio,iTempInt2);
	m_samInfoDlg.SetWindowPos(this,1408 * m_dHoriRatio,12 * m_dVertRatio,360 * m_dHoriRatio,iTempInt2,SWP_NOZORDER);

	m_rcManage.CalcRectResult();
}

void CSamSetDlg::ShowCtrl()
{
	m_swtBtPlate.ShowWindow(SW_SHOW);
	m_swtBtChart.ShowWindow(SW_SHOW);
	m_samPlateDlg.ShowWindow(SW_SHOW);
	m_samInfoDlg.ShowWindow(SW_SHOW);
}

void CSamSetDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
	m_swtBtPlate.SetWindowText(pLangInfo->GetText(60));
	m_swtBtChart.SetWindowText(pLangInfo->GetText(61));

	m_lstSamInfo.SetColumnText(0,pLangInfo->GetText(111));
	m_lstSamInfo.SetColumnText(1,pLangInfo->GetText(76));
	m_lstSamInfo.SetColumnText(2,pLangInfo->GetText(47));
	m_lstSamInfo.SetColumnText(3,pLangInfo->GetText(48));
	m_lstSamInfo.SetColumnText(4,pLangInfo->GetText(64));
	m_lstSamInfo.SetColumnText(5,pLangInfo->GetText(95));

	m_lstSamInfo.SetColumnText(6,pLangInfo->GetText(96));
	m_lstSamInfo.SetColumnText(7,pLangInfo->GetText(66));
	m_lstSamInfo.SetColumnText(8,pLangInfo->GetText(67));
	m_lstSamInfo.SetColumnText(9,pLangInfo->GetText(68));
	m_lstSamInfo.SetColumnText(10,pLangInfo->GetText(69));
	m_lstSamInfo.SetColumnText(11,pLangInfo->GetText(70));

	m_lstSamInfo.SetColumnText(12,pLangInfo->GetText(71));
	m_lstSamInfo.SetColumnText(13,pLangInfo->GetText(72));
	m_lstSamInfo.SetColumnText(14,pLangInfo->GetText(73));
	m_lstSamInfo.SetColumnText(15,pLangInfo->GetText(74));
	m_lstSamInfo.SetColumnText(16,pLangInfo->GetText(75));
	m_lstSamInfo.SetColumnText(17,pLangInfo->GetText(20));

	return;
}

void CSamSetDlg::SetPageSelected(ESAMDISPALYMODE eSamDisplayMode)
{
	if(SAMPLATE == eSamDisplayMode)
	{
		if(NULL != m_pCurSelWnd->GetSafeHwnd())
		{
			m_pCurSelWnd->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBtPlate.EnableWindow(FALSE);
		m_samPlateDlg.ShowWindow(SW_SHOW);
		m_pCurSelWnd = &m_samPlateDlg;
		m_pCurSelSwt = &m_swtBtPlate;
	}
	else if(SAMCHART == eSamDisplayMode)
	{
		if(NULL != m_pCurSelWnd->GetSafeHwnd())
		{
			m_pCurSelWnd->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBtChart.EnableWindow(FALSE);
		m_lstSamInfo.ShowWindow(SW_SHOW);
		m_pCurSelWnd = &m_lstSamInfo;
		m_pCurSelSwt = &m_swtBtChart;
	}

	return;
}

int CSamSetDlg::SetRowColNum(int iRowNum,int iColNum)
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
	m_lstSamInfo.Clear(TRUE);

	return 0;
}

void CSamSetDlg::ReSetLstSamInfo(vector<CTubeInfo*>& vecSamInfoPtr,BOOL bRedraw /*= FALSE*/)
{
	m_lstSamInfo.Clear();
	m_vecPlateSamIndex.clear();

	CString tempStr(_T(""));
	int index = 0;

	int iRowNum = m_samPlateDlg.GetRowNum();
	int iColNum = m_samPlateDlg.GetColNum();

	int iTubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
	{
		kEnd = iRowNum;
		tEnd = iColNum;
	}
	else
	{
		kEnd = iColNum;
		tEnd = iRowNum;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
			{
				iTubeIndex = k * iColNum + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColNum + k;
				index1 = t;
				index2 = k + 1;
			}

			// Bounds guard: avoid out-of-range access if tube list size < expected grid size
			if(iTubeIndex < 0 || iTubeIndex >= (int)vecSamInfoPtr.size())
			{
				continue;
			}
			if(NULL == vecSamInfoPtr[iTubeIndex])
			{
				continue;
			}

			if(vecSamInfoPtr[iTubeIndex]->GetBeNoneSampleType())
			{
				continue;
			}
			tempStr.Format(_T("%d"),index + 1);
			m_lstSamInfo.InsertRowSetHeight(index,54* m_dVertRatio,FALSE);
			m_lstSamInfo.SetItemText(index,0,tempStr);

			m_samPlateDlg.GetSiteTag(iTubeIndex,tempStr);
			m_lstSamInfo.SetItemText(index,1,tempStr);

			m_vecPlateSamIndex.push_back(iTubeIndex);

			SetLstSamInfo(index++,*vecSamInfoPtr[iTubeIndex]);
		}
	}

	OnMsgPlateSiteSelectChanged(0,0);

	if(bRedraw && NULL != m_lstSamInfo.GetSafeHwnd())
	{
		m_lstSamInfo.Invalidate(FALSE);
	}

	return;
}

void CSamSetDlg::ReSetLstSamInfo(vector<CTubeInfo>& vecSamInfo,BOOL bRedraw /*= FALSE*/)
{
	m_lstSamInfo.Clear();
	m_vecPlateSamIndex.clear();

	CString tempStr(_T(""));
	int index = 0;

	int iRowNum = m_samPlateDlg.GetRowNum();
	int iColNum = m_samPlateDlg.GetColNum();

	int iTubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
	{
		kEnd = iRowNum;
		tEnd = iColNum;
	}
	else
	{
		kEnd = iColNum;
		tEnd = iRowNum;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
			{
				iTubeIndex = k * iColNum + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColNum + k;
				index1 = t;
				index2 = k + 1;
			}

			// Bounds guard to prevent vector subscript out of range
			if(iTubeIndex < 0 || iTubeIndex >= (int)vecSamInfo.size())
			{
				continue;
			}

			if(SAMPLE_TYPE_NONE == vecSamInfo[iTubeIndex].GetSampleType())
			{
				continue;
			}
			tempStr.Format(_T("%d"),index + 1);
			m_lstSamInfo.InsertRowSetHeight(index,54* m_dVertRatio,FALSE);
			m_lstSamInfo.SetItemText(index,0,tempStr);

			m_samPlateDlg.GetSiteTag(iTubeIndex,tempStr);
			m_lstSamInfo.SetItemText(index,1,tempStr);

			m_vecPlateSamIndex.push_back(iTubeIndex);

			SetLstSamInfo(index++,vecSamInfo[iTubeIndex]);
		}
	}

	OnMsgPlateSiteSelectChanged(0,0);

	if(bRedraw && NULL != m_lstSamInfo.GetSafeHwnd())
	{
		m_lstSamInfo.Invalidate(FALSE);
	}

	return;
}

void CSamSetDlg::ReSetLstSamInfo(BOOL bRedraw /*= FALSE*/)
{
	vector<CTubeInfo> vecSamInfo;
	m_samPlateDlg.GetSamInfo(vecSamInfo);

	m_lstSamInfo.Clear();
	m_vecPlateSamIndex.clear();

	CString tempStr(_T(""));
	int index = 0;

	int iRowNum = m_samPlateDlg.GetRowNum();
	int iColNum = m_samPlateDlg.GetColNum();

	int iTubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
	{
		kEnd = iRowNum;
		tEnd = iColNum;
	}
	else
	{
		kEnd = iColNum;
		tEnd = iRowNum;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
			{
				iTubeIndex = k * iColNum + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColNum + k;
				index1 = t;
				index2 = k + 1;
			}

			// Bounds guard to prevent vector subscript out of range
			if(iTubeIndex < 0 || iTubeIndex >= (int)vecSamInfo.size())
			{
				continue;
			}

			if(SAMPLE_TYPE_NONE == vecSamInfo[iTubeIndex].GetSampleType())
			{
				continue;
			}
			tempStr.Format(_T("%d"),index + 1);
			m_lstSamInfo.InsertRowSetHeight(index,54* m_dVertRatio,FALSE);
			m_lstSamInfo.SetItemText(index,0,tempStr);

			m_samPlateDlg.GetSiteTag(iTubeIndex,tempStr);
			m_lstSamInfo.SetItemText(index,1,tempStr);

			m_vecPlateSamIndex.push_back(iTubeIndex);

			SetLstSamInfo(index++,vecSamInfo[iTubeIndex]);
		}
	}

	OnMsgPlateSiteSelectChanged(0,0);

	if(bRedraw && NULL != m_lstSamInfo.GetSafeHwnd())
	{
		m_lstSamInfo.Invalidate(FALSE);
	}

	return;
}

int CSamSetDlg::SetLstSamInfo(int index,CTubeInfo& tubeInfo)
{
	if(index < 0)
	{
		return -1;
	}
	if(index >= m_lstSamInfo.GetRowNum())
	{
		return -2;
	}

	CString strTemp(_T(""));

	m_lstSamInfo.SetItemText(index,2,CMySamPalteDlg::SampleTypeToText(tubeInfo.GetSampleType()));
	m_lstSamInfo.SetItemText(index,3,tubeInfo.GetSampleName());
	m_lstSamInfo.SetItemText(index,4,tubeInfo.GetSampleID());
	if(tubeInfo.GetBeNeedDensity())
	{
		strTemp = tubeInfo.GetAllChannelDensityString();
	}
	else
	{
		strTemp = _T("");
	}
	m_lstSamInfo.SetItemText(index,5,strTemp); 

	m_lstSamInfo.SetItemText(index,6,tubeInfo.GetTubeFlag()); 
	m_lstSamInfo.SetItemText(index,7,tubeInfo.GetPatientName());
	m_lstSamInfo.SetItemText(index,8,tubeInfo.GetSexualityString());
	m_lstSamInfo.SetItemText(index,9,tubeInfo.GetAgeString()); 
	m_lstSamInfo.SetItemText(index,10,tubeInfo.GetCollectDate());

	m_lstSamInfo.SetItemText(index,11,tubeInfo.GetProposer());
	m_lstSamInfo.SetItemText(index,12,tubeInfo.GetDepartment());
	m_lstSamInfo.SetItemText(index,13,tubeInfo.GetCaseID()); 
	m_lstSamInfo.SetItemText(index,14,tubeInfo.GetBedNo()); 
	m_lstSamInfo.SetItemText(index,15,tubeInfo.GetHospital());
	m_lstSamInfo.SetItemText(index,16,tubeInfo.GetClinicID()); 
	m_lstSamInfo.SetItemText(index,17,tubeInfo.GetComments());

	return 0;
}

int CSamSetDlg::SetLstSamInfo(int index,CTubeInfo& tubeInfo,EITEMINFOTYPE eItemInfoType)
{
	if(index < 0)
	{
		return -1;
	}
	if(index >= m_lstSamInfo.GetRowNum())
	{
		return -2;
	}

	CString strTemp(_T(""));

	switch(eItemInfoType)
	{
	case ITEMSAMNAME: m_lstSamInfo.SetItemText(index,3,tubeInfo.GetSampleName()); break;
	case ITEMSAMTYPE: 
		{
			m_lstSamInfo.SetItemText(index,2,CMySamPalteDlg::SampleTypeToText(tubeInfo.GetSampleType()));

			if(tubeInfo.GetBeNeedDensity())
			{
				
			}
			else
			{
				m_lstSamInfo.SetItemText(index,5,_T(""));
			}
		}
		break;

	case ITEMSTDDENSITY: /*strTemp.Format(_T("%.4lf"),samInfo.m_dDensity); m_lstSamInfo.SetItemText(index,5,strTemp);*/ break;
	case ITEMREPETITIVETUBE:  break;
	case ITEMSAMID:  m_lstSamInfo.SetItemText(index,4,tubeInfo.GetSampleID()); break;
	case ITEMPATIENTNAME: m_lstSamInfo.SetItemText(index,7,tubeInfo.GetPatientName()); break;
	case ITEMSEXUALITY:	m_lstSamInfo.SetItemText(index,8,tubeInfo.GetSexualityString());	break;

	case ITEMAGE: m_lstSamInfo.SetItemText(index,9,tubeInfo.GetAgeString()); break;
	case ITEMSAMCOLLECTDATE: m_lstSamInfo.SetItemText(index,10,tubeInfo.GetCollectDate()); break;
	case ITEMSENDINGDOCTOR: m_lstSamInfo.SetItemText(index,11,tubeInfo.GetProposer()); break;
	case ITEMSENDINGSECTION: m_lstSamInfo.SetItemText(index,12,tubeInfo.GetDepartment()); break;
	case ITEMANAMNESISNO: m_lstSamInfo.SetItemText(index,13,tubeInfo.GetCaseID()); break;
	case ITEMBEDNO: m_lstSamInfo.SetItemText(index,14,tubeInfo.GetBedNo()); break;
	case ITEMHOSPITALID: m_lstSamInfo.SetItemText(index,15,tubeInfo.GetHospital()); break;
	case ITEMOUTPATIENTNO: m_lstSamInfo.SetItemText(index,16,tubeInfo.GetClinicID()); break;
	case ITEMREMARKS: m_lstSamInfo.SetItemText(index,17,tubeInfo.GetComments()); break;

	default:	break;
	}

	return 0;
}

int CSamSetDlg::LstSamLanguageChanged()
{
	int iLstRowNum = m_lstSamInfo.GetRowNum();
	if(iLstRowNum < 1)
	{
		return 1;
	}

	vector<CTubeInfo> vecSamInfo;
	m_samPlateDlg.GetSamInfo(vecSamInfo);

	int iIndex = 0;

	int iRowNum = m_samPlateDlg.GetRowNum();
	int iColNum = m_samPlateDlg.GetColNum();

	int iTubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
	{
		kEnd = iRowNum;
		tEnd = iColNum;
	}
	else
	{
		kEnd = iColNum;
		tEnd = iRowNum;
	}

	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == theApp.m_setInfo.m_eSamSortWay)
			{
				iTubeIndex = k * iColNum + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColNum + k;
				index1 = t;
				index2 = k + 1;
			}

			// Bounds guard for language refresh as well
			if(iTubeIndex < 0 || iTubeIndex >= (int)vecSamInfo.size())
			{
				continue;
			}

			if(SAMPLE_TYPE_NONE == vecSamInfo[iTubeIndex].GetSampleType())
			{
				continue;
			}

			if(iIndex >= 0 && iIndex < iLstRowNum)
			{
				m_lstSamInfo.SetItemText(iIndex,2,CMySamPalteDlg::SampleTypeToText(vecSamInfo[iTubeIndex].GetSampleType()));
				m_lstSamInfo.SetItemText(iIndex++,8,vecSamInfo[iTubeIndex].GetSexualityString());
			}
		}
	}

	return 0;
}

void CSamSetDlg::SetPlateSelectedSiteSamInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/)
{
	// 先同步常用信息变化
	if (CYSamInfoChangedProcess(eItemInfoType, strData, intData, dData) != 0)
	{
		return;
	}

	// 如果当前没有任何选中孔位，但用户在“All”场景或直接编辑右侧信息框时希望批量应用，
	// 则对整板所有孔位执行（以当前板行列数为准）
	std::vector<int> vecTargets = m_vecCurSelSite;
	if (vecTargets.empty())
	{
		int totalRows = m_samPlateDlg.GetRowNum();
		int totalCols = m_samPlateDlg.GetColNum();
		for (int r = 0; r < totalRows; ++r)
		{
			for (int c = 0; c < totalCols; ++c)
			{
				vecTargets.push_back(r * totalCols + c);
			}
		}
	}

	BOOL bFindSamTypeNone = FALSE;
	CTubeInfo tubeInfo;

	// 判断是否存在空样本类型
	if (ITEMSAMTYPE == eItemInfoType)
	{
		for (int i = 0; i < (int)vecTargets.size(); ++i)
		{
			m_samPlateDlg.GetSamInfo(vecTargets[i], tubeInfo);
			if (SAMPLE_TYPE_NONE == tubeInfo.GetSampleType())
			{
				bFindSamTypeNone = TRUE;
				break;
			}
		}
	}

	// 批量写入更改
	for (int i = 0; i < (int)vecTargets.size(); ++i)
	{
		m_samPlateDlg.SetSamInfo(vecTargets[i], eItemInfoType, strData, intData, dData);
	}

	// 若存在空样本类型，重绘并刷新列表
	if (bFindSamTypeNone)
	{
		std::vector<CTubeInfo> vecTubeInfo;
		m_samPlateDlg.GetSamInfo(vecTubeInfo);
		ReSetLstSamInfo(vecTubeInfo);
	}

	// 更新列表中被选中的对应行
	for (int i = 0; i < (int)vecTargets.size(); ++i)
	{
		BOOL bFind = FALSE;
		int index = 0;
		for (int k = 0; k < m_vecPlateSamIndex.size(); ++k)
		{
			if (vecTargets[i] == m_vecPlateSamIndex[k])
			{
				index = k;
				bFind = TRUE;
				break;
			}
		}

		if (!bFind)
		{
			continue;
		}

		m_samPlateDlg.GetSamInfo(vecTargets[i], tubeInfo);
		SetLstSamInfo(index, tubeInfo, eItemInfoType);
	}

	m_samPlateDlg.Invalidate(FALSE);
	m_lstSamInfo.Invalidate(FALSE);
	ResetProcSetDlgTubeInfo();
	return;
}

int CSamSetDlg::CYSamInfoChangedProcess(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/)
{
	if(NULL == m_pParent)
	{
		return -1;
	}

	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return -2;
	}

	switch(eItemInfoType)
	{
	case ITEM_CYSAMINFO_CB_SAMCATEGORY:	pExpData->SetCYSampleCategory(strData); break;
	case ITEM_CYSAMINFO_ED_PRODUCTBN: pExpData->SetCYProductBatchNumber(strData); break;
	case ITEM_CYSAMINFO_ED_SAMCOUNT: pExpData->SetCYSampleCount(strData); break;
	case ITEM_CYSAMINFO_ED_SAMSTATE: pExpData->SetCYSampleState(strData); break;
	case ITEM_CYSAMINFO_ED_COMMENT: pExpData->SetCYComment(strData); break;
	case ITEM_CYSAMINFO_ED_TESTITEM: pExpData->SetCYTestItem(strData); break;
	case ITEM_CYSAMINFO_DT_COLLECTDATE: pExpData->SetCYCollectDate(strData); break;
	default: return 0;
	}

	return 1;
}

void CSamSetDlg::AutoSetPlateSelectedSiteSamId(CSamNameAutoSetInfo samNameAutoSetInfo)
{
	int index = 0;
	CString tempStr(_T(""));
	CString strSuffix(_T("")),strFinalName(_T(""));
	int newId = 0;
	int count = 0;

	int iRowNum = m_samPlateDlg.GetRowNum();
	int iColNum = m_samPlateDlg.GetColNum();

	int tubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == samNameAutoSetInfo.m_eSamSortWay)
	{
		kEnd = iRowNum;
		tEnd = iColNum;
	}
	else
	{
		kEnd = iColNum;
		tEnd = iRowNum;
	}

	BOOL bSelected = FALSE;
	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == samNameAutoSetInfo.m_eSamSortWay)
			{
				tubeIndex = k * iColNum + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				tubeIndex = t * iColNum + k;
				index1 = t;
				index2 = k + 1;
			}

			bSelected = FALSE;
			for(int i = 0;i < m_vecCurSelSite.size();++i)
			{
				if(tubeIndex == m_vecCurSelSite[i])
				{
					bSelected = TRUE;
					break;
				}
			}

			if(!bSelected)
			{
				continue;
			}

			BOOL bFind = FALSE;
			for(int m = 0;m < m_vecPlateSamIndex.size();++m)
			{
				if(tubeIndex == m_vecPlateSamIndex[m])
				{
					index = m;
					bFind = TRUE;
					break;
				}
			}

			if(!bFind)
			{
				continue;
			}

			tempStr = _T("%0");
			tempStr.AppendFormat(_T("%d"),samNameAutoSetInfo.m_bitNum);
			tempStr += _T("d");
			newId = samNameAutoSetInfo.m_startValue + count++ * samNameAutoSetInfo.m_interval;
			strSuffix.Format(tempStr,newId);
			strFinalName = samNameAutoSetInfo.m_strSamIdPrefix + strSuffix;
			m_samPlateDlg.SetSamInfo(tubeIndex,ITEMSAMNAME,strFinalName);
			m_lstSamInfo.SetItemText(index,3,strFinalName);
		}
	}

	m_samPlateDlg.Invalidate();
	return;
}

void CSamSetDlg::AutoSetPlateSelectedSiteCalibratorDensity(CCalibAutoSetInfo calibAutoSetInfo)
{
	int index = 0;
	CString tempStr(_T(""));
	double dNewDensity = calibAutoSetInfo.m_dStartDensity;
	int count = 0;

	int iRowNum = m_samPlateDlg.GetRowNum();
	int iColNum = m_samPlateDlg.GetColNum();

	int tubeIndex = 0;
	int kEnd = 0,tEnd = 0;
	int index1 = 0,index2 = 0;
	if(ESSW_HORI == calibAutoSetInfo.m_eSamSortWay)
	{
		kEnd = iRowNum;
		tEnd = iColNum;
	}
	else
	{
		kEnd = iColNum;
		tEnd = iRowNum;
	}

	BOOL bSelected = FALSE,bFirst = TRUE;
	for(int k = 0;k < kEnd;++k)
	{
		for(int t = 0;t < tEnd;++t)
		{
			if(ESSW_HORI == calibAutoSetInfo.m_eSamSortWay)
			{
				tubeIndex = k * iColNum + t;
				index1 = k;
				index2 = t + 1;
			}
			else
			{
				tubeIndex = t * iColNum + k;
				index1 = t;
				index2 = k + 1;
			}

			bSelected = FALSE;
			for(int i = 0;i < m_vecCurSelSite.size();++i)
			{
				if(tubeIndex == m_vecCurSelSite[i])
				{
					bSelected = TRUE;
					break;
				}
			}

			if(!bSelected)
			{
				continue;
			}

			CTubeInfo tubeInfo;
			m_samPlateDlg.GetSamInfo(tubeIndex,tubeInfo);
			if(SAMPLE_TYPE_STANDARD != tubeInfo.GetSampleType() && SAMPLE_TYPE_CALIBRATOR != tubeInfo.GetSampleType())
			{
				continue;
			}

			BOOL bFind = FALSE;
			for(int m = 0;m < m_vecPlateSamIndex.size();++m)
			{
				if(tubeIndex == m_vecPlateSamIndex[m])
				{
					index = m;
					bFind = TRUE;
					break;
				}
			}

			if(!bFind)
			{
				continue;
			}

			if(0 != calibAutoSetInfo.m_dilutionMulitiples)
			{
				if(0 == (count % calibAutoSetInfo.m_repeatNum))
				{
					if(!bFirst)
					{
						if(EDD_INCREASE == calibAutoSetInfo.m_eDilutionDirection)
						{
							dNewDensity *= calibAutoSetInfo.m_dilutionMulitiples;
						}
						else
						{
							dNewDensity /= calibAutoSetInfo.m_dilutionMulitiples;
						}
					}
					else
					{
						bFirst = FALSE;
					}
				}
				++count;
			}

			m_samPlateDlg.SetSamInfo(tubeIndex,ITEMSTDDENSITY,_T(""),0,dNewDensity);

			tempStr.Format(_T("%.2lf"),dNewDensity);
			m_lstSamInfo.SetItemText(index,5,tempStr);
		}
	}

	m_samPlateDlg.Invalidate();
	return;
}

int CSamSetDlg::ReSetTubeInfo(vector<int> &vecPlateSelectedSite,vector<CTubeInfo> &vecTubeInfo)
{
	BOOL bFindSelected = FALSE;
	for(int i = 0;i < vecPlateSelectedSite.size();++i)
	{
		m_samPlateDlg.SetSamInfo(vecPlateSelectedSite[i],vecTubeInfo[i]);

		if(!bFindSelected)
		{
			for(int j = 0;j < m_vecCurSelSite.size();++j)
			{
				if(vecPlateSelectedSite[i] == m_vecCurSelSite[j])
				{
					bFindSelected = TRUE;
					break;
				}
			}
		}	
	}
	m_samPlateDlg.Invalidate(FALSE);

	//����������Ϣ�б���
	BOOL bNeedResetLst = FALSE;
	int iLstRowNum = m_vecPlateSamIndex.size();
	if(iLstRowNum > 0)
	{
		for(int i = 0;i < vecPlateSelectedSite.size();++i)
		{
			BOOL bFindSameIndex = FALSE;
			int iSameIndex = 0;
			for(int j = 0;j < m_vecPlateSamIndex.size();++j)
			{
				if(vecPlateSelectedSite[i] == m_vecPlateSamIndex[j])
				{
					bFindSameIndex = TRUE;
					iSameIndex = j;
					break;
				}
			}

			if(bFindSameIndex)
			{
				if(vecTubeInfo[i].GetBeNoneSampleType())
				{
					bNeedResetLst = TRUE;
					break;
				}
			}
			else
			{
				if(!vecTubeInfo[i].GetBeNoneSampleType())
				{
					bNeedResetLst = TRUE;
					break;
				}
			}
		}
	}
	else
	{
		for(int i = 0;i < vecTubeInfo.size();++i)
		{
			if(!vecTubeInfo[i].GetBeNoneSampleType())
			{
				bNeedResetLst = TRUE;
				break;
			}
		}
	}
	
	if(bNeedResetLst)
	{
		vector<CTubeInfo> vecTubeInfo;
		m_samPlateDlg.GetSamInfo(vecTubeInfo);
		ReSetLstSamInfo(vecTubeInfo,TRUE);
	}
	else
	{
		if(iLstRowNum > 0)
		{
			for(int i = 0;i < vecPlateSelectedSite.size();++i)
			{
				for(int j = 0;j < m_vecPlateSamIndex.size();++j)
				{
					if(vecPlateSelectedSite[i] == m_vecPlateSamIndex[j])
					{
						SetLstSamInfo(j,vecTubeInfo[i]);
						break;
					}
				}
			}
			m_lstSamInfo.Invalidate(FALSE);
		}
	}

	//����ı���Ϣ�Ŀ�λ�Ƿ��ڵ�ǰѡ�п�λ�У�����������Ϣ����
	if(bFindSelected)
	{
		OnMsgPlateSiteSelectChanged(0,0);
	}

	return 0;
}

int CSamSetDlg::ResetProcSetDlgTubeInfo()
{
	//������������ҳ�������Ϣ
	if(m_vecCurSelSite.size() > 0)
	{
		vector<CTubeInfo> vecTubeInfo;
		for(int i = 0;i < m_vecCurSelSite.size();++i)
		{
			CTubeInfo tempTubeInfo;
			m_samPlateDlg.GetSamInfo(m_vecCurSelSite[i],tempTubeInfo);
			vecTubeInfo.push_back(tempTubeInfo);
		}

		CMiddleDlg* pMiddleDlg = (CMiddleDlg*)GetParent();
		if(NULL != pMiddleDlg->GetSafeHwnd())
		{
			pMiddleDlg->ResetTubeInfo(m_vecCurSelSite,vecTubeInfo, 1);
		}

		return 0;
	}

	return -1;
}

void CSamSetDlg::ClearPage(BOOL bRedraw /*= TRUE*/)
{
	//m_samPlateDlg.Clear(bRedraw);
	m_lstSamInfo.Clear(bRedraw);
	m_samInfoDlg.ClearSamInfo();
	//������
}

int CSamSetDlg::FileOpened(CExperimentData* pExpData /*= NULL*/)
{
	ClearPage();

	vector<CTubeInfo*> vecTubeInfoPtr;
	pExpData->GetAllTubeInfoPtr(vecTubeInfoPtr);
	m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,TRUE);
	ReSetLstSamInfo(vecTubeInfoPtr,TRUE);

	//���ÿ����ṫ��������Ϣ
	m_samInfoDlg.SetCYSamInfo(pExpData);

	return 0;
}

int CSamSetDlg::DeleteSelectedSampleInfo()
{
	return m_samPlateDlg.DeleteSelectedTubeInfo(theApp.GetBeIVDUser());
}

void CSamSetDlg::ResetLstTitle()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_lstSamInfo.SetColumnText(0,pLangInfo->GetText(111));
	m_lstSamInfo.SetColumnText(1,pLangInfo->GetText(76));
	m_lstSamInfo.SetColumnText(2,pLangInfo->GetText(47));
	m_lstSamInfo.SetColumnText(3,pLangInfo->GetText(48));
	m_lstSamInfo.SetColumnText(4,pLangInfo->GetText(64));
	m_lstSamInfo.SetColumnText(5,pLangInfo->GetText(95));

	m_lstSamInfo.SetColumnText(6,pLangInfo->GetText(96));
	m_lstSamInfo.SetColumnText(7,pLangInfo->GetText(66));
	m_lstSamInfo.SetColumnText(8,pLangInfo->GetText(67));
	m_lstSamInfo.SetColumnText(9,pLangInfo->GetText(68));
	m_lstSamInfo.SetColumnText(10,pLangInfo->GetText(69));
	m_lstSamInfo.SetColumnText(11,pLangInfo->GetText(70));

	m_lstSamInfo.SetColumnText(12,pLangInfo->GetText(71));
	m_lstSamInfo.SetColumnText(13,pLangInfo->GetText(72));
	m_lstSamInfo.SetColumnText(14,pLangInfo->GetText(73));
	m_lstSamInfo.SetColumnText(15,pLangInfo->GetText(74));
	m_lstSamInfo.SetColumnText(16,pLangInfo->GetText(75));
	m_lstSamInfo.SetColumnText(17,pLangInfo->GetText(20));
}

void CSamSetDlg::ResetLstColumnWidth()
{
	if(theApp.GetBeEnglish())
	{
		int data = 1600 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(0,46 * m_dHoriRatio);
		data -= 46 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(1,72 * m_dHoriRatio);
		data -= 72 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(2,106 * m_dHoriRatio);
		data -= 106 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(3,102 * m_dHoriRatio);
		data -= 102 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(4,130 * m_dHoriRatio);
		data -= 131 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(5,170 * m_dHoriRatio);
		data -= 131 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(6,81 * m_dHoriRatio);
		data -= 48 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(7,76 * m_dHoriRatio);
		data -= 76 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(8,48 * m_dHoriRatio);
		data -= 48 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(9,49 * m_dHoriRatio);
		data -= 49 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(10,99 * m_dHoriRatio);
		data -= 99 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(11,79 * m_dHoriRatio);
		data -= 79 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(12,78 * m_dHoriRatio);
		data -= 78 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(13,62 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(14,62 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(15,100 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(16,72 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;

		data -= m_lstSamInfo.GetFrameLineWidth() * 2;
		m_lstSamInfo.SetColumnWidth(17,data);	
	}
	else
	{
		int data = 1504 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(0,46 * m_dHoriRatio);
		data -= 46 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(1,72 * m_dHoriRatio);
		data -= 72 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(2,106 * m_dHoriRatio);
		data -= 106 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(3,102 * m_dHoriRatio);
		data -= 102 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(4,131 * m_dHoriRatio);
		data -= 131 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(5,131 * m_dHoriRatio);
		data -= 131 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(6,48 * m_dHoriRatio);
		data -= 48 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(7,76 * m_dHoriRatio);
		data -= 76 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(8,48 * m_dHoriRatio);
		data -= 48 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(9,49 * m_dHoriRatio);
		data -= 49 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(10,99 * m_dHoriRatio);
		data -= 99 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(11,79 * m_dHoriRatio);
		data -= 79 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(12,78 * m_dHoriRatio);
		data -= 78 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(13,62 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(14,62 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(15,62 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;
		m_lstSamInfo.SetColumnWidth(16,62 * m_dHoriRatio);
		data -= 62 * m_dHoriRatio;

		data -= m_lstSamInfo.GetFrameLineWidth() * 2;
		m_lstSamInfo.SetColumnWidth(17,data);
	}

	return;
}

int CSamSetDlg::RefreshLanguageDisplay()
{
	BOOL bEnglish = theApp.GetBeEnglish();

	int iFtH = 14 * m_dTxRatio,iFtDensity = 12 * m_dHoriRatio;
	if(bEnglish)
	{
		iFtH = 15 * m_dTxRatio;
	}

	ResetLstTitle();
	ResetLstColumnWidth();
	m_lstSamInfo.SetMyHeaderFont(m_stTxFont,iFtH);
	m_lstSamInfo.SetMyFont(m_stTxFont,iFtH);
	m_lstSamInfo.SetMyDensityFont(m_stTxFont,iFtDensity);
	LstSamLanguageChanged();

	m_swtBtPlate.SetMyFont(m_stTxFont);
	m_swtBtPlate.SetWindowText(theApp.m_pLangInfo->GetText(60));

	m_swtBtChart.SetMyFont(m_stTxFont);
	m_swtBtChart.SetWindowText(theApp.m_pLangInfo->GetText(61));

	int iFtH2 = 18 * m_dTxRatio;
	int iFtAbbrH = 14 * m_dTxRatio;

	m_samPlateDlg.SetMyFont(m_stTxFont);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFtH2);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFtH2,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFtAbbrH);

	iFtH = 13 * m_dTxRatio;
	if(bEnglish)
	{
		m_segLgdInfo.m_iOneCharW = 10 * m_dHoriRatio;
		iFtH = 15 * m_dTxRatio;
	}
	else
	{
		m_segLgdInfo.m_iOneCharW = 14 * m_dHoriRatio;
	}
	CLegendInfo::ResetSegLegendText(&m_segLgdInfo);
	m_segLgdInfo.SetMyFont(m_stTxFont,iFtH);

	m_samInfoDlg.RefreshLanguageDisplay();

	return 0;
}

int CSamSetDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	if(m_eExpEditMode == eExpEditMode)
	{
		return 1;
	}

	m_eExpEditMode = eExpEditMode;

	m_samInfoDlg.SetEditMode(eExpEditMode);

	return 0;
}



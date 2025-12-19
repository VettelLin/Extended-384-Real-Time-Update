// RunMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "RunMonitorDlg.h"
#include "MiddleDlg.h"
#include "afxdialogex.h"


// CRunMonitorDlg dialog

IMPLEMENT_DYNAMIC(CRunMonitorDlg, CDialogEx)

CRunMonitorDlg::CRunMonitorDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CRunMonitorDlg::IDD, pParent),m_stLidTem(TRUE),m_stTubeTem(TRUE)
{
	m_bCtrlCreated = FALSE;
	m_pCurSelAmlMelt = NULL;
	m_pCurSelAMDlg = NULL;
	m_pCurSelSwt = NULL;
	m_pCurSelDlg = NULL;
	m_curSonPageType = ERST_RUNSEGDLG; 
	m_eCurAmlMeltPageType = ERAMPT_ORIGINAML;
	m_pParent = NULL;
	if(NULL != pParent)
	{
		m_pParent = (CMiddleDlg*)pParent;
	}

	m_iSwitchBtBkH = 40;
	m_iSwitchBtH = 32;
	m_iSwitchBtBkW = 244;
	m_iSwitchBtW = 120;
	m_bInstruHaveLid = TRUE;

	m_rcLidInstruRegion = CRect(363,528,581,552);
}

CRunMonitorDlg::~CRunMonitorDlg()
{

}

void CRunMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRunMonitorDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_RUNMONITORDLG_SWTBT_RUNSEGMENT,OnBnClickedSwtBtRunSegment)
	ON_BN_CLICKED(ID_RUNMONITORDLG_SWTBT_TEMCURVE,OnBnClickedSwtBtTemCurve)
	ON_BN_CLICKED(ID_RUNMONITORDLG_SWTBT_ORIGINAML,OnBnClickedSwtBtOriginAml)
	ON_BN_CLICKED(ID_RUNMONITORDLG_SWTBT_ORIGINMELT,OnBnClickedSwtBtOriginMelt)
	ON_MESSAGE(WM_TELLPARENTSITESELECTCHANGED,OnMsgPlateSiteSelectChanged)
END_MESSAGE_MAP()

BOOL CRunMonitorDlg::OnEraseBkgnd(CDC* pDC)
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
	CRect rect;
	GetClientRect(&rect);

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	CBitmap bitMapCB;
	bitMapCB.CreateCompatibleBitmap(pDC,rect.Width(),rect.Height());
	CBitmap* pOldBitmapCB = memDc.SelectObject(&bitMapCB);

	memDc.FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	CFont* pOldFont = memDc.SelectObject(&m_fontSet);
	memDc.SetTextColor(m_txClr1);
	memDc.SetBkMode(TRANSPARENT);

	DrawRoundRectangleNew(&memDc,16 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,480 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	DrawRoundRectangleNew(&memDc,16 * m_dHoriRatio,504 * m_dVertRatio,868 * m_dHoriRatio,354 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	DrawRoundRectangleNew(&memDc,900 * m_dHoriRatio,504 * m_dVertRatio,868 * m_dHoriRatio,354 * m_dVertRatio,8,8,8,8,TRUE,Color(0XD8,0XD8,0XD8),1,TRUE,Color(0XFF,0XFF,0XFF),1);

	DrawRoundRectangleNew(&memDc,32 * m_dHoriRatio,28 * m_dVertRatio,m_iSwitchBtBkW,m_iSwitchBtBkH,8,8,8,8,TRUE,Color(0XE7,0XE7,0XE7),1,TRUE,COLOR_SOFT_THEME,1);
	DrawRoundRectangleNew(&memDc,32 * m_dHoriRatio,520 * m_dVertRatio,m_iSwitchBtBkW,m_iSwitchBtBkH,8,8,8,8,TRUE,Color(0XE7,0XE7,0XE7),1,TRUE,COLOR_SOFT_THEME,1);

	if(ERST_TEMCRV == m_curSonPageType)
	{
		DWORD dwWordStyle = DT_LEFT | DT_VCENTER | DT_SINGLELINE;
		if(m_bInstruHaveLid)
		{
			CRect rcRound1(363 * m_dHoriRatio,534 * m_dVertRatio,376 * m_dHoriRatio,547 * m_dVertRatio);
			DrawRound(&memDc,rcRound1,TRUE,Color(0XFF,0XFF,0XFF),1,TRUE,Color(0X9B,0X2F,0XAE));
			memDc.DrawText(pLangInfo->GetText(81),CRect(384 * m_dHoriRatio,528 * m_dVertRatio,456 * m_dHoriRatio,552 * m_dVertRatio),dwWordStyle);

			CRect rcRound2(488 * m_dHoriRatio,534 * m_dVertRatio,501 * m_dHoriRatio,547 * m_dVertRatio);
			DrawRound(&memDc,rcRound2,TRUE,Color(0XFF,0XFF,0XFF),1,TRUE,Color(0X40,0X54,0XB2));
			memDc.DrawText(pLangInfo->GetText(82),CRect(509 * m_dHoriRatio,528 * m_dVertRatio,581 * m_dHoriRatio,552 * m_dVertRatio),dwWordStyle);

			m_rcLidInstruRegion.left = 363 * m_dHoriRatio;
			m_rcLidInstruRegion.top = 528 * m_dVertRatio;
			m_rcLidInstruRegion.right = 581 * m_dHoriRatio;
			m_rcLidInstruRegion.bottom = 552 * m_dVertRatio;
		}
		else
		{
			CRect rcRound1(363 * m_dHoriRatio,534 * m_dVertRatio,376 * m_dHoriRatio,547 * m_dVertRatio);
			DrawRound(&memDc,rcRound1,TRUE,Color(0XFF,0XFF,0XFF),1,TRUE,Color(0X40,0X54,0XB2));
			memDc.DrawText(pLangInfo->GetText(82),CRect(384 * m_dHoriRatio,528 * m_dVertRatio,456 * m_dHoriRatio,552 * m_dVertRatio),dwWordStyle);

			m_rcLidInstruRegion.left = 363 * m_dHoriRatio;
			m_rcLidInstruRegion.top = 528 * m_dVertRatio;
			m_rcLidInstruRegion.right = 456 * m_dHoriRatio;
			m_rcLidInstruRegion.bottom = 552 * m_dVertRatio;
		}
	}

	CRect rcLegend;
	rcLegend.left = 901 * m_dHoriRatio;
	rcLegend.right = rcLegend.left + 868 * m_dHoriRatio;
	rcLegend.top = 12 * m_dVertRatio + 453 * m_dVertRatio;
	rcLegend.bottom = rcLegend.top + 27 * m_dVertRatio;
	DrawLegend(&memDc,&rcLegend,m_segLgdInfo);

	//pDC->BitBlt(0,0,rect.Width(),rect.Height(),&memDc,0,0,SRCCOPY);

	Node<CRect>* pNodeRst = m_rcManage.m_lnkRcRst.GetHead();
	while(NULL != pNodeRst)
	{
		pDC->BitBlt(pNodeRst->data.left,pNodeRst->data.top,pNodeRst->data.Width(),pNodeRst->data.Height(),&memDc,pNodeRst->data.left,pNodeRst->data.top,SRCCOPY);
		pNodeRst = pNodeRst->pNext;
	}
	
	memDc.SelectObject(pOldFont);
	memDc.SelectObject(pOldBitmapCB);
	bitMapCB.DeleteObject();
	memDc.DeleteDC();

	return TRUE;
}

void CRunMonitorDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}
	return;
}

int CRunMonitorDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
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

	m_rcLidInstruRegion.left *= m_dHoriRatio;
	m_rcLidInstruRegion.top *= m_dVertRatio;
	m_rcLidInstruRegion.right *= m_dHoriRatio;
	m_rcLidInstruRegion.bottom *= m_dVertRatio;

	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CRunMonitorDlg::OnBnClickedSwtBtRunSegment()
{
	SetPageSelected(ERST_RUNSEGDLG);
}

void CRunMonitorDlg::OnBnClickedSwtBtTemCurve()
{
	SetPageSelected(ERST_TEMCRV);
}

void CRunMonitorDlg::OnBnClickedSwtBtOriginAml()
{
	SetPageSelected(ERAMPT_ORIGINAML);
}

void CRunMonitorDlg::OnBnClickedSwtBtOriginMelt()
{
	SetPageSelected(ERAMPT_ORIGINMELT);
}

LRESULT CRunMonitorDlg::OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam)
{
	m_samPlateDlg.GetPlateSelectedSite(m_vecPlateSelectedSite);
	
	//��λѡ��״̬�ı䣬������ʾ����
	UpdateCurveSelectState(FALSE);
	ResetCurveMaxMinValue(TRUE);

	return 1;
}

BOOL CRunMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitDlg();
	InitSegLegend();
	InitSonDlg();
	CreateCtrl();
	InitCrv();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos();
	ShowCtrl();

	SetCtrlText();

	SetPageSelected(ERAMPT_ORIGINAML);
	SetPageSelected(ERST_RUNSEGDLG);

	return TRUE;
}

void CRunMonitorDlg::InitDlg()
{
	SetTextClr1(RGB(0X66,0X66,0X66));
	SetMyFont(m_stTxFont,16 * m_dTxRatio,FW_NORMAL,TYPE_PIXEL);
}

void CRunMonitorDlg::InitSegLegend()
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

	m_segLgdInfo.m_iLeftIndent = 41 * m_dHoriRatio; 
	m_segLgdInfo.m_iRoundRadius = 6 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrIconToTx = 8 * m_dHoriRatio; 
	m_segLgdInfo.m_iInrTxToIcon = 20 * m_dHoriRatio; 
#endif

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

	m_segLgdInfo.SetMyFont(m_stTxFont,iFontH);
	m_segLgdInfo.SetTextClr1(RGB(0X39,0X45,0X65));
}

void CRunMonitorDlg::InitSonDlg()
{
	InitRunStateDlg();
}

void CRunMonitorDlg::InitRunStateDlg()
{
	m_runStateDlg.Create(IDD_MIDDLEDLG,this);
}

void CRunMonitorDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();
	CreateSeg();
	CreateSamPlateDlg();
}
  
void CRunMonitorDlg::CreateBt()
{
	m_swtBtSegMonitor.Create(_T("���г���"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_RUNMONITORDLG_SWTBT_RUNSEGMENT);
	m_swtBtTemMonitor.Create(_T("�¶�����"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_RUNMONITORDLG_SWTBT_TEMCURVE);
	m_swtBtAmlOri.Create(_T("��������"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_RUNMONITORDLG_SWTBT_ORIGINAML);
	m_swtBtMeltOri.Create(_T("ԭʼ�۽�����"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_RUNMONITORDLG_SWTBT_ORIGINMELT);
}

void CRunMonitorDlg::CreateSt()
{
	m_stLidTem.Create(_T("  ��"), WS_CHILD, CRect(0,0,0,0), this,ID_RUNMONITORDLG_ST_LITTEM);
	m_stTubeTem.Create(_T("  ��"), WS_CHILD, CRect(0,0,0,0), this,ID_RUNMONITORDLG_ST_TUBETEM);
}

void CRunMonitorDlg::CreateSeg()
{
	m_segDlg.Create(IDD_MIDDLEDLG,this);
}

void CRunMonitorDlg::CreateSamPlateDlg()
{
	m_samPlateDlg.Create(IDD_MIDDLEDLG,this);
}

void CRunMonitorDlg::InitCrv()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_crvAmlOri.SetYAxisMinOfMax(2000);
	m_crvAmlOri.SetBeUseFloatFunc(TRUE);
	m_crvAmlOri.SetVertEndValue(DEFAULT_FLUMAX);
	m_crvAmlOri.SetVertStepNums(10);
	m_crvAmlOri.SetVertStringOutStep(1);

	m_crvAmlOri.SetYAxisTag(_T("Fn"));
	m_crvAmlOri.SetXAxisTag(_T("ѭ����"));

	m_crvAmlOri.SetHoriEndValue(40);
	m_crvAmlOri.SetHoriStartValue(1);
	m_crvAmlOri.SetHoriStep(1);
	m_crvAmlOri.SetHoriStringOutStep(3);

	m_crvAmlOri.SetLeftIndent(70 * m_dHoriRatio);
	m_crvAmlOri.SetRightIndent(23 * m_dVertRatio);
	m_crvAmlOri.SetTopIndent(21 * m_dHoriRatio);
	m_crvAmlOri.SetBottomIndent(30 * m_dVertRatio);

	m_crvAmlOri.SetVertDecimalPointLength(0);
	m_crvAmlOri.SetHaveCaptionOrNot(FALSE);
	m_crvAmlOri.SetHaveLegendOrNot(FALSE);
	m_crvAmlOri.SetBackgrounColor(RGB(255,255,255));
	m_crvAmlOri.Create(IDD_MIDDLEDLG,this);
	m_crvAmlOri.SetMyFont(m_stTxFont,iFontH);
	m_crvAmlOri.SetMinYDecrementParam(TRUE,100,100);
	m_crvAmlOri.ShowWindow(SW_SHOW);

	m_crvMeltOri.SetYAxisMinOfMax(2000);
	m_crvMeltOri.SetBeUseFloatFunc(TRUE);
	m_crvMeltOri.SetVertEndValue(DEFAULT_FLUMAX);
	m_crvMeltOri.SetVertStepNums(10);
	m_crvMeltOri.SetVertStringOutStep(1);

	m_crvMeltOri.SetYAxisTag(_T("Fn"));
	m_crvMeltOri.SetXAxisTag(_T("�¶�(��)"));

	m_crvMeltOri.SetHoriStartValue(60);
	m_crvMeltOri.SetHoriEndValue(90);
	m_crvMeltOri.SetHoriStep(1);
	m_crvMeltOri.SetHoriStringOutStep(5);
	m_crvMeltOri.SetHoriDecimalPointLength(2);

	m_crvMeltOri.SetLeftIndent(70 * m_dHoriRatio);
	m_crvMeltOri.SetRightIndent(23 * m_dVertRatio);
	m_crvMeltOri.SetTopIndent(21 * m_dHoriRatio);
	m_crvMeltOri.SetBottomIndent(30 * m_dVertRatio);

	m_crvMeltOri.SetVertDecimalPointLength(0);
	m_crvMeltOri.SetHaveCaptionOrNot(FALSE);
	m_crvMeltOri.SetHaveLegendOrNot(FALSE);
	m_crvMeltOri.SetBackgrounColor(RGB(255,255,255));
	m_crvMeltOri.Create(IDD_MIDDLEDLG,this);
	m_crvMeltOri.SetMyFont(m_stTxFont,iFontH);

	//m_crvTem.SetBeUseFloatFunc(TRUE);
	m_crvTem.SetYAxisMinOfMax(1);
	m_crvTem.SetVertEndValue(120);
	m_crvTem.SetVertStepNums(12);
	m_crvTem.SetVertLineColor(RGB(222,222,222));
	m_crvTem.SetVertBaseLineColor(RGB(234,236,239));

	m_crvTem.SetYAxisTag(_T("�¶�(��)"));
	m_crvTem.SetXAxisTag(_T("ʱ��(h:mm:ss)"));

	m_crvTem.SetHoriStartValue(0);
	m_crvTem.SetHoriEndValue(300000);
	m_crvTem.SetHoriStepNums(7);
	m_crvTem.SetHoriLineColor(RGB(222,222,222));
	m_crvTem.SetHoriBaseLineColor(RGB(234,236,239));
	m_crvTem.SetShowRoundTag(TRUE);
	m_crvTem.SetRoundTagLineRadius(3);

	m_crvTem.SetLeftIndent(70 * m_dHoriRatio);
	m_crvTem.SetRightIndent(10 * m_dVertRatio);
	m_crvTem.SetTopIndent(10 * m_dHoriRatio);
	m_crvTem.SetBottomIndent(40 * m_dVertRatio);

	m_crvTem.SetLineColorMode(ECM_DEFAULT);

	m_crvTem.SetHaveCaptionOrNot(FALSE);
	m_crvTem.SetHaveLegendOrNot(FALSE);
	m_crvTem.SetCurveWidth(1);
	m_crvTem.SetBackgrounColor(RGB(255,255,255));
	m_crvTem.Create(IDD_MIDDLEDLG,this);
	m_crvTem.SetMyFont(m_stTxFont,iFontH);
	m_crvTem.SetXAxisMode(TIMERXAXIS);
	
	m_crvTem.SetLineNum(2);
	m_crvTem.SetLineParam(0,RGB(0X40,0X54,0XB2),0,TRUE);
	m_crvTem.SetLineParam(1,RGB(0X9B,0X2F,0XAE),0,TRUE);
	m_crvTem.SetLineXAxisMillisecondTime(0,TRUE);
	m_crvTem.SetLineXAxisMillisecondTime(1,TRUE);
}

void CRunMonitorDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitSeg();
	InitSamPlateDlg();
}

void CRunMonitorDlg::InitBt()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}

	m_swtBtAmlOri.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtAmlOri.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtAmlOri.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtAmlOri.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtAmlOri.SetMyFont(m_stTxFont,iFontH);

	m_swtBtMeltOri.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtMeltOri.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtMeltOri.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtMeltOri.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtMeltOri.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);

	m_swtBtSegMonitor.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtSegMonitor.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtSegMonitor.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtSegMonitor.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtSegMonitor.SetMyFont(m_stTxFont,iFontH);

	m_swtBtTemMonitor.SetRoundRectParam(TRUE,CLR_SOFT_THEME,FALSE);
	m_swtBtTemMonitor.SetEdgeColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XE7,0XE7,0XE7),Color(0XE7,0XE7,0XE7));
	m_swtBtTemMonitor.SetBkColor(COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF));
	m_swtBtTemMonitor.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),CLR_2LEVEL_THEME,CLR_2LEVEL_THEME);
	m_swtBtTemMonitor.SetMyFont(m_stTxFont,iFontH);
}

void CRunMonitorDlg::InitSt()
{
	int iFontH = 16 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 17 * m_dTxRatio;
	}

	m_stLidTem.SetTextColor(RGB(0X66,0X66,0X66));
	m_stLidTem.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_stLidTem.SetWordStyle(DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	m_stTubeTem.SetTextColor(RGB(0X66,0X66,0X66));
	m_stTubeTem.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_stTubeTem.SetWordStyle(DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void CRunMonitorDlg::InitSeg()
{
	int iFontH = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_segDlg.SetGradientInfoUseState(FALSE);
	m_segDlg.m_fontStepTag.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.m_fontGeneral.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.m_fontSegTag.SetMyFont(m_stTxFont,iFontH);
	m_segDlg.SetBeMonitorFunc(TRUE);

	int iTempInt1 = 34 * m_dVertRatio;
	m_segDlg.m_horiScroll.m_arrowWH = iTempInt1;
	m_segDlg.m_horiScroll.m_scrollH = iTempInt1;
	m_segDlg.m_vertScroll.m_arrowWH = iTempInt1;
	m_segDlg.m_vertScroll.m_scrollW = iTempInt1;

	m_segDlg.SetScrollArrowWH(4 * m_dHoriRatio,8 * m_dVertRatio);

	m_segDlg.SetBeEditable(FALSE);
}

void CRunMonitorDlg::InitSamPlateDlg()
{
	int iFontH = 18 * m_dTxRatio;
	int iFontH2 = 14 * m_dTxRatio;
	if(theApp.m_pLangInfo->GetBeEnglish())
	{
		iFontH = 20 * m_dTxRatio;
	}
	m_samPlateDlg.SetRowHeaderWidth(SAMPLATE_WH * m_dHoriRatio);
	m_samPlateDlg.SetColumnHeaderHeight(SAMPLATE_WH * m_dVertRatio);
	// Smaller cells (quarter of previous)
	m_samPlateDlg.SetFixedRowHeightColumnWidth(1 * m_dVertRatio,2 * m_dHoriRatio);
	m_samPlateDlg.SetInterval(1 * m_dHoriRatio,1 * m_dVertRatio);

	m_samPlateDlg.SetRowColNum(32, 12);

	m_samPlateDlg.SetMyFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFontH,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFontH2);
	m_samPlateDlg.SetBeTellParentRowSelChange(TRUE);
	m_samPlateDlg.SetBeSingleSelect(TRUE);
	m_samPlateDlg.SetBeShieldDeleteKey(TRUE);

#ifdef PS96INSTRUMENT
	m_samPlateDlg.SetSiteIdShowState(FALSE);
	m_samPlateDlg.SetLastChanBlockShowState(FALSE);
#else 
	#ifdef N96INSTRUMENT
		m_samPlateDlg.SetSiteIdShowState(FALSE);
	#endif
#endif
}

void CRunMonitorDlg::SetCtrlPos()
{
	int tempInt1 = 1259 * m_dHoriRatio - 2;
	int tempInt2 = 677 * m_dVertRatio - 2;
	int tempInt3 = 1269 * m_dHoriRatio;
	int tempInt4 = 411 * m_dVertRatio;

	int iTempInt5 = (m_iSwitchBtBkW - m_iSwitchBtW * 2) / 2;
	int iXStart = 32 * m_dHoriRatio + iTempInt5;
	int iYStart = 28 * m_dVertRatio + (m_iSwitchBtBkH - m_iSwitchBtH) / 2;

	CRect rect;
	GetClientRect(&rect);

	m_rcManage.m_rcInit = rect;
	m_rcManage.ClearRectToDelete();

#ifdef SHIELDMELTFUNC 
	m_rcManage.AddRectToDelete(iXStart,iYStart,244 * m_dHoriRatio - iTempInt5 * 2,m_iSwitchBtH);
	m_swtBtAmlOri.SetWindowPos(this,iXStart,iYStart,244 * m_dHoriRatio - iTempInt5 * 2,m_iSwitchBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH);
	m_swtBtMeltOri.SetWindowPos(this,iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);
#else
	m_rcManage.AddRectToDelete(iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH);
	m_swtBtAmlOri.SetWindowPos(this,iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH);
	m_swtBtMeltOri.SetWindowPos(this,iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);
#endif

	iYStart = 520 * m_dVertRatio + (m_iSwitchBtBkH - m_iSwitchBtH) / 2;
	m_rcManage.AddRectToDelete(iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH);
	m_swtBtSegMonitor.SetWindowPos(this,iXStart,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);
	m_rcManage.AddRectToDelete(iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH);
	m_swtBtTemMonitor.SetWindowPos(this,iXStart + m_iSwitchBtW,iYStart,m_iSwitchBtW,m_iSwitchBtH,SWP_NOZORDER);

	//m_rcManage.AddRectToDelete(332 * m_dHoriRatio,524 * m_dVertRatio,65 * m_dHoriRatio,32 * m_dVertRatio);
	m_stLidTem.SetWindowPos(this,332 * m_dHoriRatio,524 * m_dVertRatio,65 * m_dHoriRatio,32 * m_dVertRatio,SWP_NOZORDER);
	//m_rcManage.AddRectToDelete(460 * m_dHoriRatio,375 * m_dVertRatio,65 * m_dHoriRatio,32 * m_dVertRatio);
	m_stTubeTem.SetWindowPos(this,460 * m_dHoriRatio,375 * m_dVertRatio,65 * m_dHoriRatio,32 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(20 * m_dHoriRatio,72 * m_dVertRatio,860 * m_dHoriRatio,416 * m_dVertRatio);
	m_crvAmlOri.SetWindowPos(this,20 * m_dHoriRatio,72 * m_dVertRatio,860 * m_dHoriRatio,416 * m_dVertRatio,SWP_NOZORDER);
	m_crvMeltOri.SetWindowPos(this,20 * m_dHoriRatio,72 * m_dVertRatio,860 * m_dHoriRatio,416 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(32 * m_dHoriRatio,576 * m_dVertRatio,836 * m_dHoriRatio,274 * m_dVertRatio);
	m_crvTem.SetWindowPos(this,32 * m_dHoriRatio,576 * m_dVertRatio,836 * m_dHoriRatio,274 * m_dVertRatio,SWP_NOZORDER);
	m_segDlg.SetWindowPos(this,32 * m_dHoriRatio,576 * m_dVertRatio,836 * m_dHoriRatio,274 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(901 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,453 * m_dVertRatio);
	m_samPlateDlg.SetWindowPos(this,901 * m_dHoriRatio,12 * m_dVertRatio,868 * m_dHoriRatio,453 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.AddRectToDelete(900 * m_dHoriRatio,504 * m_dVertRatio,868 * m_dHoriRatio,354 * m_dVertRatio);
	m_runStateDlg.SetWindowPos(this,900 * m_dHoriRatio,504 * m_dVertRatio,868 * m_dHoriRatio,354 * m_dVertRatio,SWP_NOZORDER);

	m_rcManage.CalcRectResult();
}

void CRunMonitorDlg::ShowCtrl()
{
	m_swtBtAmlOri.ShowWindow(SW_SHOW);

#ifndef SHIELDMELTFUNC
	m_swtBtMeltOri.ShowWindow(SW_SHOW);
#endif
	
	m_swtBtSegMonitor.ShowWindow(SW_SHOW);
	m_swtBtTemMonitor.ShowWindow(SW_SHOW);
	m_crvAmlOri.ShowWindow(SW_SHOW);
	m_segDlg.ShowWindow(SW_SHOW);
	m_samPlateDlg.ShowWindow(SW_SHOW);
	m_runStateDlg.ShowWindow(SW_SHOW);
	//m_stLidTem.ShowWindow(SW_SHOW);
	//m_stTubeTem.ShowWindow(SW_SHOW);
}

void CRunMonitorDlg::SetCtrlText()
{
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_swtBtSegMonitor.SetWindowText(pLangInfo->GetText(79));
	m_swtBtTemMonitor.SetWindowText(pLangInfo->GetText(80));

#ifdef MONITOR_USE_AMP_DELTARN

#ifdef SHIELDMELTFUNC
	m_swtBtAmlOri.SetWindowText(pLangInfo->GetText(89));
#else
	m_swtBtAmlOri.SetWindowText(pLangInfo->GetText(300));
#endif

#else
	m_swtBtAmlOri.SetWindowText(pLangInfo->GetText(91));
#endif
	
	m_swtBtMeltOri.SetWindowText(pLangInfo->GetText(93));

	m_crvTem.SetYAxisTag(pLangInfo->GetText(184));
	m_crvTem.SetXAxisTag(pLangInfo->GetText(185));
	m_crvMeltOri.SetXAxisTag(pLangInfo->GetText(184));
	m_crvAmlOri.SetXAxisTag(pLangInfo->GetText(56));

	return;
}

void CRunMonitorDlg::SetCrvLineParam(vector<CTubeInfo>& vecTubeInfo)
{
	if(vecTubeInfo.size() < 1)
	{
		return;
	}
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return;
	}
	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return;
	}

	int iChanNum = pExpData->GetChannelCount();
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	if(iChanNum < MINIMUM_CHANNUM)
	{
		return;
	}
	if(iTubeNum < MINIMUM_TUBENUM)
	{
		return;
	}
	if(iTubeNum != vecTubeInfo.size())
	{
		return;
	}

	int iMaxLineNum = iTubeNum * iChanNum;
	m_crvAmlOri.SetLineNum(iMaxLineNum);
	m_crvMeltOri.SetLineNum(iMaxLineNum);

	int index = 0;
	COLORREF clrLine = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	CString strTubeLabel(_T("")),strDyeName(_T("")),strTargetName(_T(""));

	for(int i = 0;i < iTubeNum;++i)
	{
		for(int j = 0;j < iChanNum;++j)
		{
			pDyeTargetInfo = vecTubeInfo[i].GetTargetInfo(j);
			if(NULL != pDyeTargetInfo)
			{
				clrLine = pDyeTargetInfo->GetTargetColor();
				strDyeName = pDyeTargetInfo->GetDyeName();
				strTargetName = pDyeTargetInfo->GetTargetName();
			}
			else
			{
				if(0 == j)
				{
					clrLine = CHANNEL1COLOR;
				}
				else if(1 == j)
				{
					clrLine = CHANNEL2COLOR;
				}
				else if(2 == j)
				{
					clrLine = CHANNEL3COLOR;
				} 
				else if(3 == j)
				{
					clrLine = CHANNEL4COLOR;
				}
				else if(4 == j)
				{
					clrLine = CHANNEL5COLOR;
				}
				else if(5 == j)
				{
					clrLine = CHANNEL6COLOR;
				}

				strDyeName = _T("");
				strTargetName = _T("");
			}

			index = j * iTubeNum + i;

			m_crvAmlOri.SetLineParam(index,clrLine,j,FALSE);
			m_crvMeltOri.SetLineParam(index,clrLine,j,FALSE);

			{
				int displayCols = pExpData->GetColCount();
				int displayRows = pExpData->GetRowCount();
				if (displayRows == 16 && displayCols == 24)
				{
					displayCols = 12;
				}
				strTubeLabel = vecTubeInfo[i].GetTubeLabel(displayCols);
			}
			m_crvAmlOri.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvMeltOri.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
		}
	}

	return;
}

void CRunMonitorDlg::SetCrvLineParam(vector<CTubeInfo*>& vecPTubeInfo)
{
	if(vecPTubeInfo.size() < 1)
	{
		return;
	}
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return;
	}
	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return;
	}

	int iChanNum = pExpData->GetChannelCount();
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	if(iChanNum < MINIMUM_CHANNUM)
	{
		return;
	}
	if(iTubeNum < MINIMUM_TUBENUM)
	{
		return;
	}

	if(iTubeNum != vecPTubeInfo.size())
	{
		return;
	}

	int iMaxLineNum = iTubeNum * iChanNum;
	m_crvAmlOri.SetLineNum(iMaxLineNum);
	m_crvMeltOri.SetLineNum(iMaxLineNum);

	int index = 0;
	COLORREF clrLine = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	CString strTubeLabel(_T("")),strDyeName(_T("")),strTargetName(_T(""));
	CTubeInfo* pTubeInfo = NULL;

	for(int i = 0;i < iTubeNum;++i)
	{
		pTubeInfo = vecPTubeInfo[i];
		if(NULL == pTubeInfo)
		{
			continue;
		}

		for(int j = 0;j < iChanNum;++j)
		{
			pDyeTargetInfo = pTubeInfo->GetTargetInfo(j);
			if(NULL != pDyeTargetInfo)
			{
				clrLine = pDyeTargetInfo->GetTargetColor();
				strDyeName = pDyeTargetInfo->GetDyeName();
				strTargetName = pDyeTargetInfo->GetTargetName();
			}
			else
			{
				if(0 == j)
				{
					clrLine = CHANNEL1COLOR;
				}
				else if(1 == j)
				{
					clrLine = CHANNEL2COLOR;
				}
				else if(2 == j)
				{
					clrLine = CHANNEL3COLOR;
				} 
				else if(3 == j)
				{
					clrLine = CHANNEL4COLOR;
				}
				else if(4 == j)
				{
					clrLine = CHANNEL5COLOR;
				}
				else if(5 == j)
				{
					clrLine = CHANNEL6COLOR;
				}

				strDyeName = _T("");
				strTargetName = _T("");
			}

			index = j * iTubeNum + i;

			m_crvAmlOri.SetLineParam(index,clrLine,j,FALSE);
			m_crvMeltOri.SetLineParam(index,clrLine,j,FALSE);

			{
				int displayCols = pExpData->GetColCount();
				int displayRows = pExpData->GetRowCount();
				if (displayRows == 16 && displayCols == 24)
				{
					displayCols = 12;
				}
				strTubeLabel = pTubeInfo->GetTubeLabel(displayCols);
			}
			m_crvAmlOri.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvMeltOri.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
		}
	}

	return;
}

void CRunMonitorDlg::SetCrvLineParam(CTubeInfo* pTubeInfo,int iSiteNum)
{
	if(NULL == pTubeInfo || iSiteNum < 1)
	{
		return;
	}

	if(NULL == m_pParent->GetSafeHwnd())
	{
		return;
	}
	CExperimentData* pExpData = m_pParent->m_pExpData;
	if(NULL == pExpData)
	{
		return;
	}

	int iChanNum = pExpData->GetChannelCount();
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	if(iChanNum < MINIMUM_CHANNUM)
	{
		return;
	}
	if(iTubeNum < MINIMUM_TUBENUM)
	{
		return;
	}
	if(iSiteNum != iTubeNum)
	{
		return;
	}

	int iMaxLineNum = iTubeNum * iChanNum;
	m_crvAmlOri.SetLineNum(iMaxLineNum);
	m_crvMeltOri.SetLineNum(iMaxLineNum);

	int index = 0;
	COLORREF clrLine = 0;
	CDyeTargetInfo* pDyeTargetInfo = NULL;
	CString strTubeLabel(_T("")),strDyeName(_T("")),strTargetName(_T(""));

	for(int i = 0;i < iTubeNum;++i)
	{
		for(int j = 0;j < iChanNum;++j)
		{
			pDyeTargetInfo = pTubeInfo[i].GetTargetInfo(j);
			if(NULL != pDyeTargetInfo)
			{
				clrLine = pDyeTargetInfo->GetTargetColor();
				strDyeName = pDyeTargetInfo->GetDyeName();
				strTargetName = pDyeTargetInfo->GetTargetName();
			}
			else
			{
				if(0 == j)
				{
					clrLine = CHANNEL1COLOR;
				}
				else if(1 == j)
				{
					clrLine = CHANNEL2COLOR;
				}
				else if(2 == j)
				{
					clrLine = CHANNEL3COLOR;
				} 
				else if(3 == j)
				{
					clrLine = CHANNEL4COLOR;
				}
				else if(4 == j)
				{
					clrLine = CHANNEL5COLOR;
				}
				else if(5 == j)
				{
					clrLine = CHANNEL6COLOR;
				}

				strDyeName = _T("");
				strTargetName = _T("");
			}

			index = j * iTubeNum + i;

			m_crvAmlOri.SetLineParam(index,clrLine,j,FALSE);
			m_crvMeltOri.SetLineParam(index,clrLine,j,FALSE);

			{
				int displayCols = pExpData->GetColCount();
				int displayRows = pExpData->GetRowCount();
				if (displayRows == 16 && displayCols == 24)
				{
					displayCols = 12;
				}
				strTubeLabel = pTubeInfo[i].GetTubeLabel(displayCols);
			}
			m_crvAmlOri.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
			m_crvMeltOri.SetLineString(index,j,strTubeLabel,strDyeName,strTargetName);
		}
	}

	return;
}

void CRunMonitorDlg::UpdateCurveSelectState(BOOL bRedraw /*= FALSE*/)
{
	CExperimentData* pExpData = pExpData = m_pParent->m_pExpData;
	CTubeInfo* pTubeInfo = m_samPlateDlg.GetTubeInfoPtr();
	if(NULL == pTubeInfo)
		return;

	CDyeTargetInfo* pDyeTargetInfo = NULL;
	int iChanNum = pExpData->GetChannelCount();
	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	if(iTubeNum != m_samPlateDlg.m_iTotalSiteNum)
	{
		return;
	}

	BOOL bDeveloper = theApp.m_curUserInfo.GetName() == DEFAULT_DEVELOPER;
	vector<int> vecChanSelState = pExpData->GetMonitorChannelBtSelectState();

	if(theApp.GetBeIVDUser())
	{
		if(vecChanSelState.size() >= 6)
		{
			vecChanSelState[5] = 0;
		}
	}

	int iLineId = 0;	
	BOOL bShowLine = FALSE,bValidSamType = FALSE;
	for(int k = 0;k < iChanNum;++k)
	{
		for(int i = 0;i < iTubeNum;++i)
		{
			iLineId = k * iTubeNum + i;
			bShowLine = FALSE;

			pDyeTargetInfo = pTubeInfo[i].GetTargetInfo(k);
			if(NULL != pDyeTargetInfo && (pDyeTargetInfo->GetBeSelected() || bDeveloper))
			{
#ifdef PS96INSTRUMENT
				//PS96�������������Ϊ��������������IDΪ�գ�����ʾ��Ӧ����
				bValidSamType = !pTubeInfo[i].GetBeNoneSampleType();
				if(bValidSamType)
				{
					if(pTubeInfo[i].GetBeUnknown() && pTubeInfo[i].GetSampleID().IsEmpty())
					{
						bValidSamType = FALSE;
					}
				}
				if(bValidSamType && k < vecChanSelState.size() && (1 == vecChanSelState[k]))
#else
				if(!pTubeInfo[i].GetBeNoneSampleType() && k < vecChanSelState.size() && (1 == vecChanSelState[k]))
#endif
				{
					for(int t = 0;t < m_vecPlateSelectedSite.size();++t)
					{
						if(i == m_vecPlateSelectedSite[t])
						{
							bShowLine = TRUE;
							break;
						}
					}
				}
			}
			
			m_crvAmlOri.SetLineShow(iLineId,bShowLine);
			m_crvMeltOri.SetLineShow(iLineId,bShowLine);
		}
	}

	if(bRedraw && NULL != GetSafeHwnd())
	{
		m_crvAmlOri.Invalidate(FALSE);
		m_crvMeltOri.Invalidate(FALSE);
	}
	return;
}

void CRunMonitorDlg::SetPageSelected(ERUNMONITORSONPAGETYPE sonPageType)
{
	if(ERST_RUNSEGDLG == sonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBtSegMonitor.EnableWindow(FALSE);
		m_segDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_segDlg;
		m_pCurSelSwt = &m_swtBtSegMonitor;
		m_curSonPageType = ERST_RUNSEGDLG; 
	}
	else if(ERST_TEMCRV == sonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBtTemMonitor.EnableWindow(FALSE);
		m_crvTem.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_crvTem;
		m_pCurSelSwt = &m_swtBtTemMonitor;
		m_curSonPageType = ERST_TEMCRV;
	}

	InvalidateRect(&m_rcLidInstruRegion);

	return;
}

void CRunMonitorDlg::SetPageSelected(ERUNMONITOR_AMLMELTPAGETYPE sonPageType)
{
	if(ERAMPT_ORIGINAML == sonPageType)
	{
		if(NULL != m_pCurSelAMDlg->GetSafeHwnd())
		{
			m_pCurSelAMDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelAmlMelt->GetSafeHwnd())
		{
			m_pCurSelAmlMelt->EnableWindow(TRUE);
		}
		m_swtBtAmlOri.EnableWindow(FALSE);
		m_crvAmlOri.ShowWindow(SW_SHOW);
		m_pCurSelAMDlg = &m_crvAmlOri;
		m_pCurSelAmlMelt = &m_swtBtAmlOri;
		m_eCurAmlMeltPageType = ERAMPT_ORIGINAML; 
	}
	else if(ERAMPT_ORIGINMELT == sonPageType)
	{
		if(NULL != m_pCurSelAMDlg->GetSafeHwnd())
		{
			m_pCurSelAMDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelAmlMelt->GetSafeHwnd())
		{
			m_pCurSelAmlMelt->EnableWindow(TRUE);
		}
		m_swtBtMeltOri.EnableWindow(FALSE);
		m_crvMeltOri.ShowWindow(SW_SHOW);
		m_pCurSelAMDlg = &m_crvMeltOri;
		m_pCurSelAmlMelt = &m_swtBtMeltOri;
		m_eCurAmlMeltPageType = ERAMPT_ORIGINMELT;
	}

	return;
}

void CRunMonitorDlg::ClearPage(BOOL bRedraw /*= TRUE*/)
{
	m_samPlateDlg.DeleteAllTubeInfo();
	m_segDlg.Clear(bRedraw);
	m_crvAmlOri.DeleteLineData(bRedraw);
	m_crvMeltOri.DeleteLineData(bRedraw);	
	m_crvTem.DeleteLineData(bRedraw);
	m_runStateDlg.m_mntBaseInfoDlg.ClearPage();

	//��ռ��ҳ��ͨ��ѡ��ť��ѡ��״̬
	CExperimentData* pExpData = m_pParent->m_pExpData;
	pExpData->ResetMonitorChannelBtSelectState();
	return;
}

void CRunMonitorDlg::ClearExperiment(BOOL bRedraw /*= TRUE*/)
{
	m_samPlateDlg.DeleteAllTubeInfo();
	m_segDlg.Clear(bRedraw);
	m_crvAmlOri.DeleteLineData(bRedraw);
	m_crvMeltOri.DeleteLineData(bRedraw);	
	m_crvTem.DeleteLineData(bRedraw);
	m_runStateDlg.m_mntBaseInfoDlg.ClearPage();

	//��ռ��ҳ��ͨ��ѡ��ť��ѡ��״̬
	CExperimentData* pExpData = m_pParent->m_pExpData;
	pExpData->ResetMonitorChannelBtSelectState();
	return;
}

int CRunMonitorDlg::FileOpened(CExperimentData* pExpData /*= NULL*/,BOOL bTemplate /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		pExpData = m_pParent->m_pExpData;
	}
	if(NULL == pExpData)
	{
		return -1;
	}

	ClearPage(FALSE);

	//���ļ������÷���ҳ��ͨ��Ϊȫѡ״̬
	pExpData->SetMonitorChannelBtAllSelected();
	m_pParent->m_funcBtDlg.SetChanBtSelectState(pExpData->GetMonitorChannelBtSelectState());

	if(!pExpData->GetBeTemplateFile())
	{
		//����ʵ��״̬��Ϣ������򿪵���ģ���ļ���������ʵ��״̬��Ϣ
		m_runStateDlg.m_mntBaseInfoDlg.SetMonitorBaseInfo(pExpData->GetExperimentInfo());

		vector<CTubeInfo*> vecPTubeInfo;
		pExpData->GetAllTubeInfoPtr(vecPTubeInfo);
		m_samPlateDlg.SetTubeInfo(vecPTubeInfo);

		// ��ʾ�¶�����
		SetTemperatureData(pExpData, TRUE);

		//����������ɫ
		SetCrvLineParam(vecPTubeInfo);

		//������������
		SetCurveData(pExpData,TRUE);

		m_segDlg.SetSegmentInfo(pExpData->GetSegment(),TRUE);
	}

	//�������ߺ�������Ϣ
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	int iCrvEndValue = pExpData->GetFirstEndCollectCycleNum();
	if(iAmpDataLen > 1 && iAmpDataLen < iCrvEndValue)
	{
		iCrvEndValue = iAmpDataLen;
	}
	if(iCrvEndValue < 1)
	{
		iCrvEndValue = CURVE_X_DEFMAXVALUE;
	}
	SetCurveHoriAxisParam(1,iCrvEndValue,1,FALSE);

	//�����۽����ߺ�������Ϣ
	double dStartTem = 60,dEndTem = 90;
	pExpData->GetFirstMeltCollectStartAndEndTemperature(dStartTem,dEndTem);
	m_crvMeltOri.SetHoriStartValue(dStartTem);
	m_crvMeltOri.SetHoriEndValue(dEndTem);

	m_crvAmlOri.Invalidate(FALSE);
	m_crvMeltOri.Invalidate(FALSE);

	return 0;
}

int CRunMonitorDlg::ReAnalysed(CExperimentData* pExpData /*= NULL*/)
{
	m_samPlateDlg.SetTubeInfo(pExpData->GetTubeInfoArrayPtr(),TRUE);

	//����������ɫ
	vector<CTubeInfo*> vecPTubeInfo;
	pExpData->GetAllTubeInfoPtr(vecPTubeInfo);
	SetCrvLineParam(vecPTubeInfo);

	//������������
	SetCurveData(pExpData,TRUE);

	return 0;
}

int CRunMonitorDlg::SetTemperatureData(CExperimentData* pExpData,BOOL bRedraw)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	int iTempCount = pExpData->GetTempDataCount();
	if (iTempCount == 0)
		return 0;

	double* pdValueTime = new double[iTempCount];
	pExpData->GetTempTimeData(iTempCount, pdValueTime);

	double* pdValueTemp = new double[iTempCount];
	pExpData->GetTempChannelData(CTemperatureData::TEMP_CH_PET, 0, iTempCount, pdValueTemp);

	CMyPoint pt;
	for(int k=0; k<iTempCount;++k)
	{
		pt.m_X = pdValueTime[k];
		pt.m_Y = pdValueTemp[k];
		m_crvTem.AddNewDot(0, pt);
	}

	if(pExpData->GetExperimentInfo()->GetLidHeader())
	{
		pExpData->GetTempChannelData(CTemperatureData::TEMP_CH_LID, 0, iTempCount, pdValueTemp);

		for(int k=0; k<iTempCount;++k)
		{
			pt.m_X = pdValueTime[k];
			pt.m_Y = pdValueTemp[k];
			m_crvTem.AddNewDot(1, pt);
		}
	}

	delete pdValueTime;
	delete pdValueTemp;

	if(NULL != m_crvTem.GetSafeHwnd())
	{
		m_crvTem.HandSetGeneralLineXYAxisMinMax();
	}

	if(bRedraw)
		m_crvTem.Invalidate();

	return 0;
}


int CRunMonitorDlg::SetCurveData(CExperimentData* pExpData,BOOL bRedraw /*= FALSE*/)
{
	if(NULL == pExpData)
	{
		return -1;
	}

	int iChanNum = pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;

	int iTubeNum = pExpData->GetPlateInfo()->GetTubeCount();
	int iAmpDataLen = pExpData->GetAmplifyDataNum();
	int iMeltDataLen = pExpData->GetMeltDataNum();
	int iLineId = 0;
	CMyPoint pt;
	CTubeInfo* pTubeInfo = NULL;

	eDataType eAmpDataType = FLU_FILTER;
#ifdef MONITOR_USE_AMP_DELTARN
	//eAmpDataType = FLU_DELTARN;
	eAmpDataType = FLU_FILTER;
#endif

	m_crvAmlOri.DeleteLineData(FALSE);
	m_crvMeltOri.DeleteLineData(FALSE);

	//��ʾ��������������
	for(int j = 0;j < iTubeNum;++j)
	{
		pTubeInfo = pExpData->GetTubeInfoAt(j);
		if(NULL == pTubeInfo)
		{
			//��¼������Ϣ
			continue;
		}
		if(SAMPLE_TYPE_NONE == pTubeInfo->GetSampleType())
		{
			continue;
		}

		for(int i = 0;i < iChanNum;++i)
		{
			iLineId = i * iTubeNum + j;
			for(int k = 0;k < iAmpDataLen;++k)
			{
				pt.m_X = k + 1;
				pt.m_Y = pExpData->GetAmplifyChannelCycleFluY(j + 1,eAmpDataType,i,k);
				m_crvAmlOri.AddNewDot(iLineId,pt);
			}
			for(int k = 0;k < iMeltDataLen;++k)
			{
				pt.m_X = pExpData->GetMeltCycleX(j + 1,k);
				pt.m_Y = pExpData->GetMeltChannelCycleFluY(j + 1,FLU_FILTER,i,k);
				m_crvMeltOri.AddNewDot(iLineId,pt);
			}
		}	
	}

	if(NULL != m_crvAmlOri.GetSafeHwnd())
	{
		m_crvAmlOri.HandSetGeneralLineXYAxisMinMax();
	}
	if(NULL != m_crvMeltOri.GetSafeHwnd())
	{
		m_crvMeltOri.HandSetGeneralLineXYAxisMinMax();
	}

	if(bRedraw)
	{
		if(NULL != m_crvAmlOri.GetSafeHwnd())
		{
			m_crvAmlOri.Invalidate(FALSE);
		}
		if(NULL != m_crvMeltOri.GetSafeHwnd())
		{
			m_crvMeltOri.Invalidate(FALSE);
		}
	}

	return 0;
}

int CRunMonitorDlg::CurrentSegmentStepChangedOnRun(int iCycle,int iSeg,int iStep)
{
	if(-1 != m_segDlg.m_priorItem && -1 != m_segDlg.m_priorStep)
	{
		m_segDlg.UpdateCurSelectStepCycleInfo(m_segDlg.m_priorItem,m_segDlg.m_priorStep,iCycle,FALSE);
	}

	m_segDlg.UpdateCurSelectStepCycleInfo(iSeg,iStep,iCycle + 1,TRUE,FALSE);
	m_segDlg.SetSegmentSetpShow(iSeg,iStep,TRUE);	

	m_segDlg.m_priorItem = iSeg;
	m_segDlg.m_priorStep = iStep;

	return 0;
}

void CRunMonitorDlg::SetCurveHoriAxisParam(int iStart,int iEnd,int iHoriStep,BOOL bRedraw /*= TRUE*/)
{
	m_crvAmlOri.SetHoriAxisParam(iStart,iEnd,iHoriStep,bRedraw);
}

int CRunMonitorDlg::ResetCurveMaxMinValue(BOOL bRedraw)
{
	if(NULL == m_pParent->GetSafeHwnd())
	{
		return -1;
	}

	if(NULL == m_pParent->m_pExpData)
	{
		return -2;
	}

	if(NULL != m_crvAmlOri.GetSafeHwnd())
	{
		m_crvAmlOri.HandSetGeneralLineYAxisMinMax();
	}
	if(NULL != m_crvMeltOri.GetSafeHwnd())
	{
		m_crvMeltOri.HandSetGeneralLineYAxisMinMax();
	}

	if(bRedraw)
	{
		if(NULL != m_crvAmlOri.GetSafeHwnd())
		{
			m_crvAmlOri.Invalidate(FALSE);
		}
		if(NULL != m_crvMeltOri.GetSafeHwnd())
		{
			m_crvMeltOri.Invalidate(FALSE);
		}
	}

	return 0;
}

int CRunMonitorDlg::RefreshLanguageDisplay()
{
	BOOL bEnglish = theApp.m_pLangInfo->GetBeEnglish();

	SetMyFont(m_stTxFont);

	int iFontH = 16 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 17 * m_dTxRatio;
	}

	m_swtBtAmlOri.SetMyFont(m_stTxFont,iFontH);
#ifdef MONITOR_USE_AMP_DELTARN
	#ifdef SHIELDMELTFUNC
		m_swtBtAmlOri.SetWindowText(theApp.m_pLangInfo->GetText(89));
	#else
		m_swtBtAmlOri.SetWindowText(theApp.m_pLangInfo->GetText(300));
	#endif
#else
	m_swtBtAmlOri.SetWindowText(theApp.m_pLangInfo->GetText(91));
#endif

	m_swtBtMeltOri.SetMyFont(m_stTxFont,iFontH);
	m_swtBtMeltOri.SetWindowText(theApp.m_pLangInfo->GetText(93));

	m_swtBtSegMonitor.SetMyFont(m_stTxFont,iFontH);
	m_swtBtSegMonitor.SetWindowText(theApp.m_pLangInfo->GetText(79));

	m_swtBtTemMonitor.SetMyFont(m_stTxFont,iFontH);
	m_swtBtTemMonitor.SetWindowText(theApp.m_pLangInfo->GetText(80));

	iFontH = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 15 * m_dTxRatio;
	}

	m_crvAmlOri.SetXAxisTag(theApp.m_pLangInfo->GetText(56));
	m_crvAmlOri.SetMyFont(m_stTxFont,iFontH);

	m_crvMeltOri.SetXAxisTag(theApp.m_pLangInfo->GetText(184));
	m_crvMeltOri.SetMyFont(m_stTxFont,iFontH);

	m_crvTem.SetYAxisTag(theApp.m_pLangInfo->GetText(184));
	m_crvTem.SetXAxisTag(theApp.m_pLangInfo->GetText(185));
	m_crvTem.SetMyFont(m_stTxFont,iFontH);

	iFontH = 18 * m_dTxRatio;
	int iFontH2 = 14 * m_dTxRatio;
	if(bEnglish)
	{
		iFontH = 20 * m_dTxRatio;
	}
	m_samPlateDlg.SetMyFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyHeaderFont(m_stTxFont,iFontH);
	m_samPlateDlg.SetMyTitleFont(m_stTxFont,iFontH,FW_SEMIBOLD);
	m_samPlateDlg.SetMyFontAbbr(m_stTxFont,iFontH2);

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

	m_runStateDlg.RefreshLanguageDisplay();

	return 0;
}

int CRunMonitorDlg::CycleAddSuccessful(int iSegId,int iCount)
{
	if(iSegId < 0)
	{
		return -1;
	}

	if(iSegId >= m_segDlg.m_lnkSegItemInfo.GetSize())
	{
		return -2;
	}

	m_segDlg.m_lnkSegItemInfo[iSegId]->m_iCycleNum += iCount;
	m_segDlg.Invalidate(FALSE);
	return 0;
}

void CRunMonitorDlg::ShowRemainTimeOnce()
{
	CExperimentInfo* pExpInfo = m_pParent->m_pExpData->GetExperimentInfo();
	int iHasRunSec = pExpInfo->GetHasRunSec() + 1;
	pExpInfo->SetHasRunSec(iHasRunSec);

	int iTotalSec = pExpInfo->GetTotalSec();
	int iSurplusSec = iTotalSec - iHasRunSec;
	if(iSurplusSec < 0)
	{
		iSurplusSec = 0;
	}
	pExpInfo->SetRemainSec(iSurplusSec);
	CString strSurplusTime = SecondToHourMinuteSecond(iSurplusSec);
	m_runStateDlg.m_mntBaseInfoDlg.m_stRemainSec.SetWindowText(strSurplusTime);
}

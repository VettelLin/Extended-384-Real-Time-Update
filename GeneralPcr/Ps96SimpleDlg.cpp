#include "StdAfx.h"
#include "Ps96SimpleDlg.h"
#include "Ps96IvdDlg.h"
#include "WorkDlg.h"
#include "InstrumentPCR.h"
#include "GeneralPcr.h"
#include "PublicInfo.h"
#include "IFormatT.h"
#include "Ps96SimpleSetDlg.h"

CPs96SimpleDlg::CPs96SimpleDlg(void):m_assaySelDlg(this),m_loadSamDlg(this)
{
	m_eCurSelPage = EPS96SSPT_NONE;
}

CPs96SimpleDlg::~CPs96SimpleDlg(void)
{

}

BEGIN_MESSAGE_MAP(CPs96SimpleDlg, CMiddleDlg)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(ID_PS96SIMPLEDLG_BT_RUNSTOP,OnBnClickedRunStop)
	ON_BN_CLICKED(ID_PS96SIMPLEDLG_BT_SET,OnBnClickedSet)
	ON_BN_CLICKED(ID_PS96SIMPLEDLG_SWTBT_ASSAYSELECT,OnBnClickedSwtAssaySelect)
	ON_BN_CLICKED(ID_PS96SIMPLEDLG_SWTBT_SAMINFO,OnBnClickedSwtSamInfo)

	ON_MESSAGE(WN_EXPERIMENT_FLU_CHANGED,OnMsgExperFluChanged)
	ON_MESSAGE(WM_EXPERIMENT_RUN,OnMsgExperRun)
	ON_MESSAGE(WM_EXPERIMENT_FINISH,OnMsgExperFinished)
	ON_MESSAGE(WM_SEGMENT_OVER,OnMsgSegmentOver)
	ON_MESSAGE(WM_MELT_ONETEM_OVER,OnMsgMeltOneTemOver)
	ON_MESSAGE(WN_EXPERIMENT_TEMP_CHANGED,OnMsgExperTempChanged)
	ON_MESSAGE(WN_EXPERIMENT_STEP_CHANGED,OnMsgExperStepChanged)
	ON_MESSAGE(WM_RUNPOWEROFFPROTECT,OnMsgRunPowerOffProtect)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_INSTRUMENTINFODLG_BT_FINDINSTRUMENT,OnBnClickedFindInstrument)
	ON_MESSAGE(WM_TELLPARENTSETNEWTEXT,OnMsgTellParentSetNewText)
END_MESSAGE_MAP()

BOOL CPs96SimpleDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	CFont* pOldFont = pDC->SelectObject(&m_fontSet);
	pDC->SetTextColor(m_txClr1);
	pDC->SetBkMode(TRANSPARENT);

	int iStartX = 10 * m_dHoriRatio,iEndX = 210 * m_dHoriRatio;
	pDC->DrawText(theApp.m_pLangInfo->GetText(88),CRect(1500 * m_dHoriRatio,20 * m_dVertRatio,1700 * m_dHoriRatio,90 * m_dVertRatio),DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	pDC->DrawText(theApp.m_pLangInfo->GetText(7),CRect(iStartX,10 * m_dVertRatio,iEndX,60 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	pDC->DrawText(theApp.m_pLangInfo->GetText(19),CRect(iStartX,75 * m_dVertRatio,iEndX,125 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	pDC->DrawText(theApp.m_pLangInfo->GetText(23),CRect(iStartX,135 * m_dVertRatio,iEndX,185 * m_dVertRatio),DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
	pDC->SelectObject(pOldFont);

	return TRUE;
}

void CPs96SimpleDlg::OnBnClickedRunStop()
{
	OnBnClickedSwtBtRunStop();
}

void CPs96SimpleDlg::OnBnClickedSet()
{
	CPs96SimpleSetDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		m_assaySelDlg.ReshowReportTemplate();
	}

	return;
}

void CPs96SimpleDlg::OnBnClickedSwtAssaySelect()
{
	SetPageSelected(EPS96SSPT_ASSAYSELECT);
	return;
}

void CPs96SimpleDlg::OnBnClickedSwtSamInfo()
{
	SetPageSelected(EPS96SSPT_LOADSAM);
	return;
}

BOOL CPs96SimpleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	InitDlg();
	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos(); 

	SetCtrlText();

	ShowCtrl();

	SetPageSelected(EPS96SSPT_ASSAYSELECT);

	CInstancePCR* pInstance = CInstrumentPCR::GetInstance()->GetAvailableInstance();
	if (pInstance != NULL && pInstance->GetInstanceStatus() == INSTANCE_TEMP_HIGH)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(479));
	}
	else if (pInstance != NULL && pInstance->GetInstanceStatus() == INSTANCE_TEMP_LOW)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(480));
	}
	else if (pInstance != NULL && pInstance->GetInstanceStatus() == INSTANCE_FLUO_HIGH)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(482));
	}
	else if (pInstance != NULL && pInstance->GetInstanceStatus() == INSTANCE_FLUO_LOW)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(483));
	}

	if (pInstance != NULL && pInstance->CanRunExp())
		m_btRunStop.EnableWindow(TRUE);
	else
		m_btRunStop.EnableWindow(FALSE);

	return TRUE;
}

void CPs96SimpleDlg::InitDlg()
{
	SetMyFont(m_stTxFont,24 * m_dTxRatio,FW_MEDIUM,TYPE_PIXEL);
	SetTextClr1(CLR_2LEVEL_THEME);
}

void CPs96SimpleDlg::InitSonDlg()
{
	m_assaySelDlg.Create(IDD_MIDDLEDLG,this);
	m_loadSamDlg.Create(IDD_MIDDLEDLG,this);
}

void CPs96SimpleDlg::CreateCtrl()
{
	CreateBt();
	CreateSt();

	m_cbInstrumentType.Create(WS_CHILD | WS_TABSTOP | ES_AUTOHSCROLL,CRect(0,0,0,0),this,ID_INSTRUMENTINFODLG_CB_INSTRUMENTTYPE);
}

void CPs96SimpleDlg::CreateBt()
{
	m_btnFindInstrument.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_INSTRUMENTINFODLG_BT_FINDINSTRUMENT);

	m_btRunStop.Create(_T("运行"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLEDLG_BT_RUNSTOP);
	m_btSet.Create(_T(""),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLEDLG_BT_SET);
	m_swtBtAssaySel.Create(_T("实验选择"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLEDLG_SWTBT_ASSAYSELECT);
	m_swtBtSamInfo.Create(_T("样本信息"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_PS96SIMPLEDLG_SWTBT_SAMINFO);
}

void CPs96SimpleDlg::CreateSt()
{
	m_stcConnectState.Create(_T("连接状态"),WS_CHILD, CRect(0,0,0,0), this,ID_GENERALPCRDLG_ST_CONNECTSTATE);

	m_stRemainTm.Create(_T("00:00:00"),WS_CHILD, CRect(0,0,0,0),this,ID_PS96SIMPLEDLG_ST_REMAINTIME);
	m_stAssayName.Create(_T(""),WS_CHILD, CRect(0,0,0,0),this,ID_PS96SIMPLEDLG_ST_ASSAYNAME);
	m_stFileSaveFolder.Create(_T(""),WS_CHILD, CRect(0,0,0,0),this,ID_PS96SIMPLEDLG_ST_FILESAVEFOLDER);
}

void CPs96SimpleDlg::InitCtrl()
{
	InitBt();
	InitSt();
	InitCb();
}

void CPs96SimpleDlg::InitBt()
{
	InitOneBt(&m_btnFindInstrument);
	//m_btnFindInstrument.SetRoundRectParam(TRUE,CLR_SOFT_LIGHT,TRUE);

	InitOneBt(&m_btSet);
	m_btSet.SetRoundRectParam(TRUE,CLR_SOFT_LIGHT,TRUE);

	InitOneBt(&m_btRunStop);
	m_btRunStop.SetRoundRectParam(TRUE,CLR_SOFT_LIGHT,TRUE);
	m_btRunStop.SetMyFont(m_stTxFont, 64 * m_dTxRatio);

	
	CIconBtInfo iconBtInfo;
	iconBtInfo.m_bUseIcon = FALSE;
	iconBtInfo.m_rcIcon = Rect(18 * m_dHoriRatio,18 * m_dVertRatio,24 * m_dHoriRatio,24 * m_dVertRatio);
	iconBtInfo.m_rcText = CRect(2 * m_dHoriRatio,16 * m_dVertRatio,218 * m_dHoriRatio,43 * m_dVertRatio);
	iconBtInfo.m_bRoundRectBk = FALSE;
	iconBtInfo.m_iSonIconNum = 2;
	iconBtInfo.m_bSonIconPosOrder = FALSE;
	iconBtInfo.m_clrNormal = Color(0XFF,0XFF,0XFF);
	iconBtInfo.m_clrHover = Color(0XFF,0XFF,0XFF);
	iconBtInfo.m_clrSelected = COLOR_SOFT_THEME;
	iconBtInfo.m_clrDisabled = COLOR_SOFT_THEME;

	int iFontH = 26 * m_dTxRatio;

	m_swtBtAssaySel.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtBtAssaySel.SetIconBtInfo(iconBtInfo);
	m_swtBtAssaySel.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	m_swtBtSamInfo.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtBtSamInfo.SetIconBtInfo(iconBtInfo);
	m_swtBtSamInfo.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL != pInstrument)
	{
		m_btRunStop.EnableWindow(pInstrument->IsExistIdleInstrument());
	}

	return;
}

void CPs96SimpleDlg::InitOneBt(CMyButton* pBt)
{
	if(NULL == pBt)
	{
		return;
	}

	int iFontH = 26 * m_dTxRatio;

	pBt->SetMyFont(m_stTxFont,iFontH);
	pBt->SetRoundRectParam(TRUE,RGB(0XFF,0XFF,0XFF),TRUE);
	pBt->SetBkColor(Color(0XF1,0XF4,0XF9),Color(0XFF,0XFF,0XFF),Color(0XE7,0XE7,0XE7),Color(0XF1,0XF4,0XF9));
	pBt->SetEdgeColor(Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8),Color(0XD8,0XD8,0XD8));
	pBt->SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));

	return;
}

void CPs96SimpleDlg::InitSt()
{
	int iFontH = 24 * m_dTxRatio,iFontH2 = 40 * m_dTxRatio;
	
	m_stRemainTm.SetBDynamic(TRUE);
	m_stRemainTm.SetBkColor(RGB(0XFF,0X06,0X1A));
	m_stRemainTm.SetTextColor(RGB(0XFF,0XFF,0XFF));
	m_stRemainTm.SetMyFont(m_stTxFont,iFontH2,FW_BOLD);
	m_stRemainTm.SetWordStyle(DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS);

	m_stAssayName.SetBDynamic(TRUE);
	m_stAssayName.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stAssayName.SetTextColor(CLR_2LEVEL_THEME);
	m_stAssayName.SetMyFont(m_stTxFont,iFontH);
	m_stAssayName.SetWordStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);

	m_stFileSaveFolder.SetBDynamic(TRUE);
	m_stFileSaveFolder.SetBkColor(RGB(0XFF,0XFF,0XFF));
	m_stFileSaveFolder.SetTextColor(CLR_2LEVEL_THEME);
	m_stFileSaveFolder.SetMyFont(m_stTxFont,iFontH);
	m_stFileSaveFolder.SetWordStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);

	m_stcConnectState.SetMyFont(m_stTxFont, iFontH, FW_NORMAL,TYPE_PIXEL);
	m_stcConnectState.SetWordStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);

	return;
}

void CPs96SimpleDlg::InitCb()
{
	int iFontH = 24 * m_dTxRatio;

	m_cbInstrumentType.SetFrameColor(Color(0XD8,0XD8,0XD8),COLOR_SOFT_THEME,COLOR_SOFT_THEME,Color(0XD8,0XD8,0XD8));
	m_cbInstrumentType.SetMainWndBkColor(Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XFF,0XFF,0XFF),Color(0XF7,0XF7,0XF7));
	m_cbInstrumentType.SetSelectedBkColor(RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB),RGB(0X7E,0XD3,0XDB));
	m_cbInstrumentType.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0X97,0X97,0X97));
	m_cbInstrumentType.SetMyFont(m_stTxFont,iFontH);
	m_cbInstrumentType.m_selWnd.SetMyFont(m_stTxFont,iFontH);
	m_cbInstrumentType.SetWorkState(EWS_READONLY);
	m_cbInstrumentType.SetTellParentSetNewText(TRUE, 1);
	m_cbInstrumentType.m_iRightIndent = 44 * m_dHoriRatio;
	m_cbInstrumentType.m_arrowInfo.m_clrArrow = Color(0X39,0X45,0X65);
	m_cbInstrumentType.m_arrowInfo.m_arrowW = 12 * m_dHoriRatio;
	m_cbInstrumentType.m_arrowInfo.m_arrowH = 8 * m_dVertRatio;

	InitInstrument();
}

void CPs96SimpleDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	int iSwtBtW = 200 * m_dHoriRatio,iSwtBtH = 60 * m_dVertRatio,iBtW = 200 * m_dHoriRatio,iBtH = 200 * m_dVertRatio;
	int iStartX = 60 * m_dHoriRatio,iStartY = 270 * m_dVertRatio,iWndW = rect.Width() - 60 * m_dHoriRatio * 2,iWndH = rect.Height() - 270 * m_dVertRatio;

	m_assaySelDlg.SetWindowPos(this,iStartX,iStartY,iWndW,iWndH,SWP_NOZORDER);
	m_loadSamDlg.SetWindowPos(this,iStartX,iStartY,iWndW,iWndH,SWP_NOZORDER);

	if(theApp.GetBeEnglish())
	{
		iSwtBtW = 220 * m_dHoriRatio;
	}

	iStartY = 220 * m_dVertRatio;
	m_swtBtAssaySel.SetWindowPos(this,iStartX,iStartY,iSwtBtW,iSwtBtH,SWP_NOZORDER);	
	m_swtBtSamInfo.SetWindowPos(this,iStartX + iSwtBtW,iStartY,iSwtBtW,iSwtBtH,SWP_NOZORDER);

	m_btRunStop.SetWindowPos(this, int(1200 * m_dHoriRatio), int(20 * m_dVertRatio), iBtW, iBtH, SWP_NOZORDER);

	m_stRemainTm.SetWindowPos(this, int(1500 * m_dHoriRatio), int(100 * m_dVertRatio), int(200 * m_dHoriRatio), int(120 *m_dVertRatio), SWP_NOZORDER);

	m_btSet.SetWindowPos(this,1760 * m_dHoriRatio,100 * m_dVertRatio, int(120 * m_dHoriRatio),70 *m_dVertRatio,SWP_NOZORDER);

	int iTempInt1 = 50 * m_dVertRatio;
	m_stAssayName.SetWindowPos(this,220 * m_dHoriRatio,10 * m_dVertRatio,900 * m_dHoriRatio,iTempInt1,SWP_NOZORDER);
	m_stFileSaveFolder.SetWindowPos(this,220 * m_dHoriRatio,75 * m_dVertRatio,900 * m_dHoriRatio,iTempInt1,SWP_NOZORDER);
	
	m_cbInstrumentType.SetWindowPos(this,int(220 * m_dHoriRatio), int(140 * m_dVertRatio),450 * m_dHoriRatio,iTempInt1,SWP_NOZORDER);
	m_btnFindInstrument.SetWindowPos(this,int((220+460) * m_dHoriRatio), int(140 * m_dVertRatio), int(200 * m_dHoriRatio),iTempInt1,SWP_NOZORDER);

	m_rectConntectState = CRect(int((220+460+230) * m_dHoriRatio), int(140 * m_dVertRatio), int((220+460+230) * m_dHoriRatio) + int(210 * m_dHoriRatio),int(140 * m_dVertRatio)+ iTempInt1);
	m_stcConnectState.SetWindowPos(this,int((220+460+230) * m_dHoriRatio), int(140 * m_dVertRatio), int(210 * m_dHoriRatio),iTempInt1,SWP_NOZORDER);
}

void CPs96SimpleDlg::ShowCtrl()
{
	m_cbInstrumentType.ShowWindow(SW_SHOW);
	m_btnFindInstrument.ShowWindow(SW_SHOW);

	m_stRemainTm.ShowWindow(SW_SHOW);
	m_stAssayName.ShowWindow(SW_SHOW);
	m_stFileSaveFolder.ShowWindow(SW_SHOW);
	m_btRunStop.ShowWindow(SW_SHOW);
	m_btSet.ShowWindow(SW_SHOW);
	m_swtBtAssaySel.ShowWindow(SW_SHOW);
	m_swtBtSamInfo.ShowWindow(SW_SHOW);

	m_stcConnectState.ShowWindow(SW_SHOW);

	return;
}

void CPs96SimpleDlg::SetCtrlText()
{
	m_btRunStop.SetWindowText(theApp.m_pLangInfo->GetText(4));
	m_btSet.SetWindowText(theApp.m_pLangInfo->GetText(293));
	m_swtBtAssaySel.SetWindowText(theApp.m_pLangInfo->GetText(379));
	m_swtBtSamInfo.SetWindowText(theApp.m_pLangInfo->GetText(382));
	m_btnFindInstrument.SetWindowText(theApp.m_pLangInfo->GetText(474));

	SetConnectState(m_bInstruConnected);
}

void CPs96SimpleDlg::SetExpDataPtr(CExperimentData* pExpData)
{
	m_pExpData = pExpData;
	m_assaySelDlg.SetExpDataPtr(pExpData);
	m_loadSamDlg.SetExpDataPtr(pExpData);

	if (pExpData->HasRunData())
	{
		m_btRunStop.EnableWindow(FALSE);
	}

	return;
}

void CPs96SimpleDlg::EmbodyBaseSetInfo()
{
	if(NULL == m_pExpData)
	{
		return;
	}

	m_stAssayName.SetWindowText(m_pExpData->GetExperName());
	m_stFileSaveFolder.SetWindowText(m_pExpData->GetExperSaveFolder());

	return;
}

int CPs96SimpleDlg::RefreshExpDlgRunState(BOOL bOccupy)
{
	if(bOccupy)
	{
		return 1;
	}

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL != pInstrument)
	{
		m_btRunStop.EnableWindow(pInstrument->IsExistIdleInstrument());
	}

	return 0;
}

int CPs96SimpleDlg::Run()
{
	m_lnkPeltiaTem.Clear();
	m_lnkLidTem.Clear();

	if(NULL == m_pExpData)
	{
		return -1;
	}

	int iCurSel = m_cbInstrumentType.GetCurSel();
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(iCurSel);
	if(NULL == pInstance)
	{
		return -3;
	}
	if (pInstance->GetInstanceStatus() == INSTANCE_USING)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(475));
		return -3;
	}

	m_pExpData->GetExperimentInfo()->SetInstrument(pInstance->GetModel());
	m_pExpData->GetExperimentInfo()->SetInstrumentSN(pInstance->GetSN());
	m_pExpData->GetExperimentInfo()->SetLidTemp(pInstance->GetLidTem());

	CReportTemplateData* pReportData = CPublicInfo::GetInstance()->GetReportTemplateData();
	if (pReportData->GetTemplateCount() == 0 || pReportData->GetCurSel() == -1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(521));
		return -6;
	}

	//判断实验流程是否设置
	CLinkList<CSegItemInfo>& lnkSegItem = m_pExpData->GetSegment();
	if(lnkSegItem.GetSize() < 1)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(224));
		return -7;
	}

	//判断实验流程是否包括数据采集步骤并提示
	if(!m_pExpData->CheckSegmentExistDataCollect())
	{
		if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(225)))
		{
			return -8;
		}
	}

	//判断扩增程序段是否设置了15个及以上的循环数,
	if(m_pExpData->IsExistAmpSegmentLessThanCycleNum(15))
	{
		if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(316)))
		{
			return -20;
		}
	}

	//PS96仪器，运行前检测是否有设置样本ID，如果所有孔位都未设置样本ID，则不能运行实验
	if(m_pExpData->IsAllNoneSampleType())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(312));
		return -17;
	}

	if(!m_pExpData->IsSampleIdSetAtExceptNoneSampleType())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(299));
		return -9;
	}

	//创建备份实验文件
	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
	m_pExpData->CreateBackupFilePath(pPublicInfo->GetStartupPath());
	m_pExpData->SetScanStartPos(pInstance->GetScanStartPos());

	//如果有数据提示用户数据将删除
	if(m_pExpData->HasRunData())
	{
		m_pExpData->DeleteRunData();
	}

	CExperimentInfo* pExpInfo = m_pExpData->GetExperimentInfo();
	//计算实验总时间
	int iExpTotalSec = pInstance->CalculateExperimentTotalTimeSec(m_pExpData->GetSegment(),pExpInfo->GetLidHeader(),20);
	pExpInfo->SetTotalSec(iExpTotalSec);

	//实验可以运行，清空监控页面数据
	

	m_pExpData->m_tmStart = CTime::GetCurrentTime();
	m_pExpData->GetExperimentInfo()->SetStartTime(m_pExpData->m_tmStart); 


	//记录运行实验的仪器硬件信息(文件保存路径更新后才更好硬件信息)
	CHardwareInfo hardwareInfo;
	hardwareInfo.m_strModel = pInstance->GetModel();
	hardwareInfo.m_strSN = pInstance->GetSN();
	hardwareInfo.m_strCode = pInstance->GetCode();
	hardwareInfo.m_strVersion = pInstance->GetVersion();
	hardwareInfo.m_strSoftwareVer = pInstance->GetSoftwareVersion();
	m_pExpData->SetHardwareInfo(hardwareInfo);

	//实验开始运行前，读取并设置热盖温度，对已经打开的实验文件有必要这样操作，新建的实验这样操作也不算多余
	double dLidTem = 0;
	if(pInstance->ReadLidTemperatureFromEEPROMPLCUsed(dLidTem))
	{
		if(pInstance->GetLidTem() != pExpInfo->GetLidTemp())
		{
			pExpInfo->SetLidTemp(pInstance->GetLidTem());
		}
	}
	else
	{
		// pMainDlg->SetInstrumentConnectState(0);
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(226));
		return -11;
	}

	//PS96实验开始前比较仪器温度和第一个步骤温度，如果仪器温度较高，给出降温提示，不能开始实验
	float fCurTemp = 0.0f;
	BOOL bCurToTarget = FALSE;
	if(pInstance->ReadPCRTemp(fCurTemp, bCurToTarget))
	{

		float fFstStepTem = lnkSegItem.GetAt(0)->data.m_lnkStepItemInfo.GetAt(0)->data.m_fAimTem;
		if(fCurTemp - fFstStepTem > 1)
		{
			//PS96仪器开机后，默认设置的温度为45℃
			if(fFstStepTem < 45)
			{
				if(!pInstance->SetMultiTemp(fFstStepTem,fFstStepTem,fFstStepTem,4.0,TEMP_CTRL_NORMAL))
				{
					PopWarningTipInfo(theApp.m_pLangInfo->GetText(272));
					return -12;
				}
			}

			PopWarningTipInfo(theApp.m_pLangInfo->GetText(270));
			return -13;
		}
	}
	else
	{
		//pMainDlg->SetInstrumentConnectState(0);
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(271));
		return -14;
	}


	// lzh 不检测lid关闭
	//检测仓门是否盖上
/*	int iLidState = 0;
	int iRet = pInstance->GetLidState(iLidState);
	if(0 != iRet)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(421));
		return -16;
	}

	if(1 == iLidState)
	{
	PopWarningTipInfo(theApp.m_pLangInfo->GetText(422));
		return -17;
	}*/

	//PS96IVD模式下，实验开始前，由用户选择工程文件保存路径（结果报告自动在改路径下导出）
SELECTPATH:
	{
		BOOL bFind = FALSE;
		CFileFind fileFind;
		CString strTemp(_T("")),strDefName(_T("")),strSerialSuffix(_T("")),strRstName(_T("")),strTitle(_T(""));
		strTemp.Format(_T("Experiemnt File(*%s)|*%s||"),PCR_DATA_FILE_SUFFIX_DOT,PCR_DATA_FILE_SUFFIX_DOT);

		//获取序列号后三位
		strSerialSuffix = pInstance->GetSN();
		if(strSerialSuffix.GetLength() > 3)
		{
			strSerialSuffix = strSerialSuffix.Right(3);
		}
		strDefName = GetCurrentTmTime(3,_T("-"),_T("-"),_T("-")) + _T("_") + strSerialSuffix + _T("#");

		CFileDialog dlg(FALSE,PCR_DATA_FILE_SUFFIX_DOT,strDefName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strTemp,NULL);
		dlg.m_ofn.lpstrTitle = _T("Select file save path");
		//dlg.m_ofn.lpstrInitialDir = m_pExpData->GetExperimentInfo()->GetFilePath();

		if(IDCANCEL == dlg.DoModal()) 
		{
			return -15;
		}

		strTitle = dlg.GetFileTitle();

		strRstName = dlg.GetFolderPath() + _T("\\") + dlg.GetFileTitle() + _T(".xlsx");
		bFind = fileFind.FindFile(strRstName);
		if(bFind)
		{
			CString strText(_T(""));
			strText.Format(theApp.m_pLangInfo->GetText(297),strRstName);
			if(IDYES == PopQueryTipInfo(strText))
			{
				goto SELECTPATH;
			}
			else
			{
				//防止原有同名文件被覆盖，新的实验文件名后加上日期后缀,20220707，孙文郎
				strTitle += _T("-");
				strTitle += GetCurrentTmTimeW().c_str();
				strTitle += _T("_");
				strTitle += strSerialSuffix;
				strTitle += _T("#");
			}
		}
		else
		{
			//路径相同的同名工程文件如果已经存在，也在新的实验文件名称后面添加日期后缀,20220707，孙文郎
			bFind = fileFind.FindFile(dlg.GetPathName());
			if(bFind)
			{
				strTitle += _T("-");
				strTitle += GetCurrentTmTimeW().c_str();
				strTitle += _T("_");
				strTitle += strSerialSuffix;
				strTitle += _T("#");
			}
		}

		strTemp = dlg.GetFolderPath();
		m_pExpData->SetIVDFileRstSaveFolder(strTemp);
		pExpInfo->SetFilePath(strTemp);
		CPublicInfo::GetInstance()->SetUserDataSavePath(strTemp);

		m_pExpData->SetIVDFileRstSaveName(strTitle);
		pExpInfo->SetExpName(strTitle);

		m_pExpData->SetFileOpenPath(_T(""));	//IVD模式，运行实验时，使用实验前用户选择的文件保存路径

		//实验运行条件检测完成，检查实验名称保存路径是否改变，并重置实验文件保存路径
		CPs96WorkDlg* pParent = (CPs96WorkDlg*)GetParent();
		if(NULL != pParent->GetSafeHwnd())
		{
			pParent->m_ps96FileManagerDlg.SetExperName(GetPageId(),strTitle);
			pParent->m_ps96FileManagerDlg.SetPageFilePath(GetPageId(),strTemp);
		}
		m_pExpData->SetFilePath(pExpInfo->GetSaveDataFileName());
	}

	m_pExpData->m_pInstance = pInstance;

	//如果实验文件和仪器中的温度通道数不一致，以仪器中的通道数为准
	if(m_pExpData->GetTempChannelCount() != pInstance->GetTempChannelCount())
	{
		m_pExpData->SetTempChannelCount(pInstance->GetTempChannelCount());
	}

	//记录实验用户名
	m_pExpData->SetUserName(theApp.m_curUserInfo.GetName());

	if(NULL == m_pRunCtrl)
	{
		m_pRunCtrl = new CExpRunCtrl(pInstance,this);
		m_pRunCtrl->SetLanguage(theApp.m_pLangInfo);
	}
	CExpRunPara* pRunParam = new CExpRunPara;
	m_pRunCtrl->Run(m_pExpData,pRunParam);

	//本次实验加入断电保护
	CRunProgressRecord runPrgRcd;
	runPrgRcd.m_iPageId = m_pageId;
	runPrgRcd.m_strFilePath = m_pExpData->GetFilePath();
	if(-1 == runPrgRcd.m_strFilePath.Find(PCR_DATA_FILE_SUFFIX_DOT))
	{
		runPrgRcd.m_strFilePath += PCR_DATA_FILE_SUFFIX_DOT;
	}
	runPrgRcd.m_strSN = pInstance->GetSN();
	runPrgRcd.m_ttStart = time(NULL);

	runPrgRcd.m_fMeltStartTem = m_pExpData->GetFirstMeltStepStartTem();
	runPrgRcd.m_fMeltEndTem = m_pExpData->GetFirstMeltStepEndTem();

	theApp.m_powerOffProtect.AddRunProgress(runPrgRcd);

	InitFluBackUp();

	return 0;
}

int CPs96SimpleDlg::RunPowerOffProtectExperiment()
{
	m_lnkPeltiaTem.Clear();
	m_lnkLidTem.Clear();

	if(NULL == m_pExpData)
	{
		return -1;
	}

	CExperimentInfo* pExpInfo = m_pExpData->GetExperimentInfo();
	if(NULL == pExpInfo)
	{
		return -2;
	}

	CString strModel = pExpInfo->GetInstrument();
	CString strSN = pExpInfo->GetInstrumentSN();
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(strModel,strSN);
	if(NULL == pInstance)
	{
		return -3;
	}

	//判断实验流程是否包括数据采集步骤并提示
	if(!m_pExpData->CheckSegmentExistDataCollect())
	{
		if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(225)))
		{
			return -8;
		}
	}

	//创建备份实验文件
	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
	//m_pExpData->CreateBackupFilePath(pPublicInfo->GetStartupPath());

	//计算实验总时间
	//int iExpTotalSec = pInstance->CalculateExperimentTotalTimeSec(m_pExpData->GetSegment(),pExpInfo->GetLidHeader(),20);
	//pExpInfo->SetTotalSec(iExpTotalSec);

	//已运行时间减去热盖升温时间
	if(pInstance->HasLid() && pExpInfo->GetLidHeader())
	{
		pExpInfo->SetHasRunSec(pExpInfo->GetHasRunSec() - pInstance->GetLidHeatNormalSecond());
	}

	m_pExpData->m_tmStart = CTime::GetCurrentTime();
	//pExpInfo->SetStartTime(m_pExpData->m_tmStart); 

	//实验运行条件检测完成，检查实验名称保存路径是否改变，并重置实验文件保存路径
	CWorkDlg* pParent = (CWorkDlg*)GetParent();
	if(NULL != pParent->GetSafeHwnd())
	{
		pParent->m_fileManageDlg.SetExperName(GetPageId(),pExpInfo->GetExpName());
		pParent->m_fileManageDlg.SetPageFilePath(GetPageId(),pExpInfo->GetSaveDataFileName());
	}

	//记录运行实验的仪器硬件信息(文件保存路径更新后才更好硬件信息)
	CHardwareInfo hardwareInfo;
	hardwareInfo.m_strModel = pInstance->GetModel();
	hardwareInfo.m_strSN = pInstance->GetSN();
	hardwareInfo.m_strCode = pInstance->GetCode();
	hardwareInfo.m_strVersion = pInstance->GetVersion();
	hardwareInfo.m_strSoftwareVer = pInstance->GetSoftwareVersion();
	m_pExpData->SetHardwareInfo(hardwareInfo);

	CPs96IvdDlg* pMainDlg = (CPs96IvdDlg*)theApp.m_pMainWnd;

	//实验开始运行前，读取并设置热盖温度，对已经打开的实验文件有必要这样操作，新建的实验这样操作也不算多余
	double dLidTem = 0;
	if(pInstance->ReadLidTemperatureFromEEPROMPLCUsed(dLidTem))
	{
		if(pInstance->GetLidTem() != pExpInfo->GetLidTemp())
		{
			pExpInfo->SetLidTemp(pInstance->GetLidTem());
			m_baseSetDlg.m_instrumentInfoDlg.Invalidate();
		}
	}
	else
	{
		//pMainDlg->SetInstrumentConnectState(0);
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(226));
		return -11;
	}

	//PS96实验开始前比较仪器温度和第一个步骤温度，如果仪器温度较高，给出降温提示，不能开始实验
	{
		float fCurTemp = 0.0f;
		BOOL bCurToTarget = FALSE;
		if(pInstance->ReadPCRTemp(fCurTemp, bCurToTarget))
		{
			CLinkList<CSegItemInfo>& lnkSegItem = m_pExpData->GetSegment();
			float fFstStepTem = lnkSegItem.GetAt(0)->data.m_lnkStepItemInfo.GetAt(0)->data.m_fAimTem;
			if(fCurTemp - fFstStepTem > 1)
			{
				//PS96仪器开机后，默认设置的温度为45℃
				if(fFstStepTem < 45)
				{
					if(!pInstance->SetMultiTemp(fFstStepTem,fFstStepTem,fFstStepTem,4.0,TEMP_CTRL_NORMAL))
					{
						PopWarningTipInfo(theApp.m_pLangInfo->GetText(272));
						return -12;
					}
				}

				PopWarningTipInfo(theApp.m_pLangInfo->GetText(270));
				return -13;
			}
		}
		else
		{
			//pMainDlg->SetInstrumentConnectState(0);
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(271));
			return -14;
		}
	}

	m_pExpData->SetIVDFileRstSaveFolder(pExpInfo->GetFilePath());

	//PS96运行断电保护实验时，报告名称加上后缀名，防止重名的excel（只读模式）文件保存失败，20220825，孙文郎
	m_pExpData->SetIVDFileRstSaveName(pExpInfo->GetExpName() + theApp.m_pLangInfo->GetText(348));
	m_pExpData->SetFilePath(pExpInfo->GetSaveDataFileName());

	m_pExpData->m_pInstance = pInstance;

	//如果实验文件和仪器中的温度通道数不一致，以仪器中的通道数为准
	if(m_pExpData->GetTempChannelCount() != pInstance->GetTempChannelCount())
	{
		m_pExpData->SetTempChannelCount(pInstance->GetTempChannelCount());
	}
	m_pExpData->SetScanStartPos(pInstance->GetScanStartPos());


	if(NULL == m_pRunCtrl)
	{
		m_pRunCtrl = new CExpRunCtrl(pInstance,this);
		m_pRunCtrl->SetLanguage(theApp.m_pLangInfo);
	}
	m_pRunCtrl->SetPowerOffExpInfo(TRUE,m_runRrgRcd.m_iSegId,m_runRrgRcd.m_iStepId,m_runRrgRcd.m_iCycleId);
	CExpRunPara* pRunParam = new CExpRunPara;
	m_pRunCtrl->Run(m_pExpData,pRunParam);

	//本次实验加入断电保护
	CRunProgressRecord runPrgRcd;
	runPrgRcd.m_iPageId = m_pageId;
	runPrgRcd.m_strFilePath = m_pExpData->GetFilePath();
	if(-1 == runPrgRcd.m_strFilePath.Find(PCR_DATA_FILE_SUFFIX_DOT))
	{
		runPrgRcd.m_strFilePath += PCR_DATA_FILE_SUFFIX_DOT;
	}
	runPrgRcd.m_strSN = pInstance->GetSN();
	runPrgRcd.m_ttStart = time(NULL);

	runPrgRcd.m_fMeltStartTem = m_pExpData->GetFirstMeltStepStartTem();
	runPrgRcd.m_fMeltEndTem = m_pExpData->GetFirstMeltStepEndTem();

	theApp.m_powerOffProtect.AddRunProgress(runPrgRcd);

	InitFluBackUp();

	return 0;
}

int CPs96SimpleDlg::Stop()
{
	if(NULL == m_pRunCtrl)
	{
		return -1;
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	if(IDYES != PopQueryTipInfo(pLangInfo->GetText(156)))
	{
		return -3;
	}

	m_pRunCtrl->AskStop();
	m_btRunStop.EnableWindow(FALSE);

	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgExperFluChanged(WPARAM wParam,LPARAM lParam)
{
	if(NULL == m_pExpData)
	{
		return 1;
	}

	//新的数据产生后，保存工程文件
	m_pExpData->SaveFile();
	//保存备份工程文件
	m_pExpData->SaveFile(1);

	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgExperRun(WPARAM wParam,LPARAM lParam)
{
	CPs96IvdDlg* pMainDlg = (CPs96IvdDlg*)theApp.m_pMainWnd;
	if(NULL == pMainDlg->GetSafeHwnd())
	{
		//记录错误信息
		return 1;
	}

	if(NULL == m_pExpData)
	{
		return 2;
	}

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_pExpData->SetStatus(CExperimentData::STATUS_USING);
	if(NULL != m_pExpData->m_pInstance)
	{
		m_pExpData->m_pInstance->SetInstanceStatus(INSTANCE_USING);
	}

	//设置开始按钮状态
	m_btRunStop.SetIconBtPath(_T(".\\BkPic\\stop.png"));
	m_btRunStop.SetWindowText(pLangInfo->GetText(5));
	m_cbInstrumentType.EnableWindow(FALSE);
	m_btnFindInstrument.EnableWindow(FALSE);

	m_bExpRunning = TRUE;

	//设置运行中实验对应的页面切换按钮在最左侧
	pMainDlg->m_workDlg.m_ps96FileManagerDlg.SetPageFirstColumn(GetPageId());

	//设置监控页面总时间
	m_stRemainTm.SetWindowText(SecondToHourMinuteSecond(m_pExpData->GetExperimentInfo()->GetTotalSec()));

	//启动倒计时定时器
	SetTimer(TIEMRID,1000,NULL);

	//实验开始时保存文件
	m_pExpData->SaveFile();

	RefreshExpNameAndSavePath();

	SetEditMode(EEEM_RUNNING);

	pMainDlg->m_workDlg.RefreshExpDlgRunState(m_pageId,TRUE);

	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgExperFinished(WPARAM wParam,LPARAM lParam)
{
	KillTimer(TIEMRID);

	CPs96IvdDlg* pMainDlg = (CPs96IvdDlg*)theApp.m_pMainWnd;
	if(NULL == pMainDlg->GetSafeHwnd())
	{
		//记录错误信息
		return 1;
	}

	if(NULL == m_pExpData)
	{
		return 2;
	}

	CExperimentInfo* pExpInfo = m_pExpData->GetExperimentInfo();
	if(NULL == pExpInfo)
	{
		return 3;
	}

	m_pExpData->SetStatus(CExperimentData::STATUS_SAVE);
	if(NULL != m_pExpData->m_pInstance)
	{
		m_pExpData->m_pInstance->SetInstanceStatus(INSTANCE_READY);
		m_pExpData->m_pInstance = NULL;
	}

	//如果监控页面剩余时间不为零，则清零
	if(pExpInfo->GetRemainSec() > 0 && STATUS_STOP == wParam)
	{
		pExpInfo->SetRemainSec(0);
	}

	//如果实验正常结束或者中途停止了实验，重新计算实验总时间，防止实验开始时间、实验结束时间、实验总时间不一致
	if(!m_bPowerOffExp && (STATUS_STOP == wParam || STATUS_ASK_STOP == wParam))
	{
		CTimeSpan tmSpan = m_pExpData->m_tmEnd - m_pExpData->m_tmStart;

		int iTotalSec = tmSpan.GetTotalSeconds();
		pExpInfo->SetTotalSec(iTotalSec);
		pExpInfo->SetHasRunSec(iTotalSec);
	}

	//设置实验页面切换按钮为普通状态
	pMainDlg->m_workDlg.m_ps96FileManagerDlg.SetPageSwitchBtState(m_pageId,FALSE);

	//更新实验状态信息
	CString strSurplusTime = SecondToHourMinuteSecond(pExpInfo->GetRemainSec());
	m_stRemainTm.SetWindowText(strSurplusTime);

	DataProcess(TRUE);

	ClearBackupFileParam();

	//实验结束，保存工程文件
	m_pExpData->SaveFile();
	//实验结束，保存备份工程文件
	m_pExpData->SaveFile(1);

	m_bExpRunning = FALSE;

	//弹出提示窗口
	if(STATUS_STOP_ERROR == wParam)
	{
		//PopWarningTipInfo(theApp.m_pLangInfo->GetText(387));
	}
	else
	{
		int iTipStrId = 155;
		if(STATUS_ASK_STOP == wParam)
		{
			iTipStrId = 157;
		}
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(iTipStrId));
	}

	//更新实验状态信息
	m_btRunStop.SetIconBtPath(_T(".\\BkPic\\start.png"));
	m_btRunStop.SetWindowText(theApp.m_pLangInfo->GetText(4));
	m_cbInstrumentType.EnableWindow(TRUE);
	m_btnFindInstrument.EnableWindow(TRUE);

	//实验结束后，恢复部分控件的编辑状态
	SetEditMode(EEEM_END);

	//PS96IVD模式下，自动导出结果报告
	ExportResultReport(m_pExpData->GetIVDResultFileSavePath(),m_pExpData->GetIVDResultFileSavePath2());

	//实验结束时，如果没有通信问题或者台式机断电问题，将该轮实验从断电保护中移除
	if(STATUS_ASK_STOP == wParam || STATUS_STOP == wParam)
	{
		//可进一步判断流程是否执行完毕
		theApp.m_powerOffProtect.DeleteRunProgress(m_pageId);
	}

	m_bPowerOffExp = FALSE;
								
	pMainDlg->m_workDlg.RefreshExpDlgRunState(m_pageId,FALSE);
	
	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgSegmentOver(WPARAM wParam,LPARAM lParam)
{
	//实验程序运行完毕后，或者熔曲步骤运行完毕后，进入清理流程之前，如果没有通信问题，将该轮实验从断电保护中移除
	theApp.m_powerOffProtect.DeleteRunProgress(m_pageId);
	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgMeltOneTemOver(WPARAM wParam,LPARAM lParam)
{
	//实时更新已经采集数据并保存的熔曲实时温度点
	theApp.m_powerOffProtect.SetRunProgress(m_pageId,wParam / 100.0);
	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgExperTempChanged(WPARAM wParam,LPARAM lParam)
{
	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgExperStepChanged(WPARAM wParam,LPARAM lParam)
{
	if(NULL == m_pRunCtrl)
	{
		return 1;
	}

	int iCurCyle = m_pRunCtrl->GetCurCycle();
	int iCurSeg = m_pRunCtrl->GetCurSegment();
	int iCurStep = m_pRunCtrl->GetCurStep();

	theApp.m_powerOffProtect.SetRunProgress(m_pageId,iCurSeg,iCurStep,iCurCyle);

	m_loadSamDlg.CurrentSegmentStepChangedOnRun(iCurCyle,iCurSeg,iCurStep);

	//当前运行程序段、步骤或者循环数改变后，及时保存温度信息、计时信息等，防止“出现异常情况时，工程文件里保存的信息和异常时刻的实际信息偏差太大”，此时不保存备份文件，20220823，孙文郎
	m_pExpData->SaveFile();

	return 0;
}

LRESULT CPs96SimpleDlg::OnMsgRunPowerOffProtect(WPARAM wParam,LPARAM lParam)
{
	RunPowerOffProtectExperiment();
	return 0;
}

void CPs96SimpleDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)      
	{      
	case TIEMRID: 
		{
			if(NULL == m_pExpData)
			{
				return;
			}
			CExperimentInfo* pExpInfo = m_pExpData->GetExperimentInfo();
			if(NULL == pExpInfo)
			{
				return;
			}
			
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
			m_stRemainTm.SetWindowText(strSurplusTime);
		}

		break;      
	default:   
		break;     

	} 
	CDialogEx::OnTimer(nIDEvent);

	return;
}

void CPs96SimpleDlg::SetPageSelected(EPS96SIMPLE_SON_PAGE_TYPE eSonPageType)
{
	if(m_eCurSelPage == eSonPageType)
	{
		return;
	}

	if(EPS96SSPT_ASSAYSELECT == eSonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBtAssaySel.EnableWindow(FALSE);
		m_assaySelDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_assaySelDlg;
		m_pCurSelSwt = &m_swtBtAssaySel;
	}
	else if(EPS96SSPT_LOADSAM == eSonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBtSamInfo.EnableWindow(FALSE);
		m_loadSamDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_loadSamDlg;
		m_pCurSelSwt = &m_swtBtSamInfo;
	}

	m_eCurSelPage = eSonPageType;

	return;
}

void CPs96SimpleDlg::RefreshExpNameAndSavePath()
{
	if(NULL == m_pExpData)
	{
		return;
	}

	m_stAssayName.SetWindowText(m_pExpData->GetExperimentInfo()->GetExpName());
	m_stFileSaveFolder.SetWindowText(m_pExpData->GetExperimentInfo()->GetFilePath());
	
	return;
}

void CPs96SimpleDlg::RefreshExpEmbody()
{
	m_assaySelDlg.RefreshExpEmbody();
	m_loadSamDlg.RefreshExpEmbody();

	DataProcess(TRUE);

	return;
}

void CPs96SimpleDlg::ClearExpEmbody()
{
	m_assaySelDlg.ClearExpEmbody();
	m_loadSamDlg.ClearExpEmbody();
	return;
}

void CPs96SimpleDlg::SetTemplateListSelectedRow(int iIndex)
{
	m_assaySelDlg.SetTemplateListSelectedRow(iIndex);
	return;
}

int CPs96SimpleDlg::NewTemplateAdded(int iIndex)
{
	m_assaySelDlg.NewTemplateAdded(iIndex);
	return 0;
}

int CPs96SimpleDlg::DeleteTemplate(int iIndex)
{
	CPs96WorkDlg* pParent = (CPs96WorkDlg*)GetParent();
	if(NULL == pParent->GetSafeHwnd())
	{
		return -1;
	}

	return	pParent->DeleteTemplate(iIndex);
}

void CPs96SimpleDlg::DeleteTemplateListRow(int iIndex)
{
	m_assaySelDlg.DeleteTemplateListRow(iIndex);
	return;
}

int CPs96SimpleDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	m_assaySelDlg.SetEditMode(eExpEditMode);
	m_loadSamDlg.SetEditMode(eExpEditMode);

	return 0;
}

int CPs96SimpleDlg::ImportId()
{
	return m_loadSamDlg.ImportId();
}

int CPs96SimpleDlg::DataProcess(BOOL bNewOver,int iAmlFlag /*= 5*/)
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	int iAmpDataLen = m_pExpData->GetAmplifyDataNum();
	int iMeltDataLen = m_pExpData->GetMeltDataNum();

	if(iAmpDataLen > 0)
	{
		if(!m_pExpData->ProcessAmplifyFluData(iAmlFlag))
		{
			return -3;
		}
	}

	if(iMeltDataLen > 0)
	{
		if(!m_pExpData->ProcessMeltFluData())
		{
			return -4;
		}
	}

	eDataType eDtType = FLU_DELTARN;
	m_pExpData->GenerateCt(bNewOver,METHOD_CT_SPLINE,eDtType);

	return 0;
}

void CPs96SimpleDlg::InitInstrument()
{
	m_cbInstrumentType.Clear(FALSE);

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	for(int i = 0;i < pInstrument->GetInstanceCount();++i)
	{
		CInstancePCR* pInstance = pInstrument->GetInstancePCR(i);
		if (pInstance->IsDemo())
			continue;

		CString strFlag = CombineModelAndSN(pInstance->GetModel(), pInstance->GetSN());
		m_cbInstrumentType.InsertString(i,strFlag);
	}
}

CString CPs96SimpleDlg::CombineModelAndSN(CString strModel,CString strSN)
{
	CString strFlag = strModel;
	if (!strSN.IsEmpty())
	{
		strFlag.Format(_T("%s(%s)"), strModel, strSN);
	}

	return strFlag;
}

void CPs96SimpleDlg::SetConnectState(BOOL bRunExp)
{
	int iTxId = 314;
	if(!m_bInstruConnected)
	{
		iTxId = 315;
	}
	m_stcConnectState.SetWindowText(theApp.m_pLangInfo->GetText(iTxId));
	InvalidateRect(m_rectConntectState);

	m_btRunStop.EnableWindow(bRunExp || m_bExpRunning);
}


void CPs96SimpleDlg::OnBnClickedFindInstrument()
{
	CString strOldSel;
	int iOldSel = m_cbInstrumentType.GetCurSel();
	if (iOldSel != -1)
	{
		m_cbInstrumentType.GetString(iOldSel, strOldSel);
	}

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if (pInstrument->Reinitialize(FALSE))
	{
		InitInstrument();

		int iCount = m_cbInstrumentType.GetItemCount();
		int iCurSel = -1;
		CString strItem;
		for (int i=0; i<iCount; i++)
		{
			m_cbInstrumentType.GetString(i, strItem);
			if (strOldSel == strItem)
			{
				iCurSel = i;
				break;
			}
		}
		m_cbInstrumentType.SetCurSel(iCurSel);

		GetParent()->SendMessage(WM_INSTRUMENT_REINITIALIZE);
	}
}


LRESULT CPs96SimpleDlg::OnMsgTellParentSetNewText(WPARAM wParam, LPARAM lParam)
{
	if (wParam == 1);
	{
		int iCurSel = m_cbInstrumentType.GetCurSel();
		CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
		CInstancePCR* pInstance = pInstrument->GetInstancePCR(iCurSel);	
		if (pInstance != NULL && !pInstance->IsDemo())
		{
			m_bInstruConnected = pInstance->GetInstanceStatus() == INSTANCE_READY || pInstance->GetInstanceStatus() == INSTANCE_USING;

			m_pExpData->GetExperimentInfo()->SetInstrument(pInstance->GetModel());
			m_pExpData->GetExperimentInfo()->SetInstrumentSN(pInstance->GetSN());
			m_pExpData->GetExperimentInfo()->SetLidTemp(pInstance->GetLidTem());
		}

		SetConnectState(pInstance->CanRunExp());
		Invalidate();
	}

	return 0;
}

void CPs96SimpleDlg::ReshowInstrumentList()
{
	if (m_bExpRunning)
		return;

	CString strOldSel;
	int iOldSel = m_cbInstrumentType.GetCurSel();
	if (iOldSel != -1)
	{
		m_cbInstrumentType.GetString(iOldSel, strOldSel);
	}

	InitInstrument();

	int iCount = m_cbInstrumentType.GetItemCount();
	int iCurSel = -1;
	CString strItem;
	for (int i=0; i<iCount; i++)
	{
		m_cbInstrumentType.GetString(i, strItem);
		if (strOldSel == strItem)
		{
			iCurSel = i;
			break;
		}
	}
	m_cbInstrumentType.SetCurSel(iCurSel);
}

void CPs96SimpleDlg::InitExperimentData(CExperimentData* pExpData)
{
	CMiddleDlg::InitExperimentData(pExpData);

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(pExpData->GetExperimentInfo()->GetInstrument(), pExpData->GetExperimentInfo()->GetInstrumentSN());
	if(NULL != pInstance)
	{
		SetSelectedInstrument(pInstance->GetModel(), pInstance->GetSN());
		SetConnectState(m_bInstruConnected);
	}
}

void CPs96SimpleDlg::SetSelectedInstrument(CString strModel, CString strSN)
{
	CString strFlag = CombineModelAndSN(strModel, strSN);

	int iCount = m_cbInstrumentType.GetItemCount();
	int iCurSel = -1;
	CString strItem;
	for (int i=0; i<iCount; i++)
	{
		m_cbInstrumentType.GetString(i, strItem);
		if (strFlag == strItem)
		{
			iCurSel = i;
			break;
		}
	}
	m_cbInstrumentType.SetCurSel(iCurSel);
}

// 导出实验结果，合并Admin和User
int CPs96SimpleDlg::ExportResult()
{
	CReportTemplateData* pReportData = CPublicInfo::GetInstance()->GetReportTemplateData();
	if (pReportData->GetTemplateCount() == 0 || pReportData->GetCurSel() == -1)
	{
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(521));
		return -1;
	}
	
	return CMiddleDlg::ExportResult();
}
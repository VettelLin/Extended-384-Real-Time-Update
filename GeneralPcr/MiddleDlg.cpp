// SkipDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GeneralPcr.h"
#include "GeneralPcr.h"
#include "GeneralPcrDlg.h"
#include "MiddleDlg.h"
#include "MyButton.h"
#include "afxdialogex.h"
#include "InstrumentPCR.h"
#include "InstanceDemo.h"
#include "CrossSetDlg.h"
#include "FilterParamSetDlg.h"
#include "libxl.h"
#include "PublicInfo.h"
#include "WorkDlg.h"
#include "AnaMeltParamDlg.h"
#include "RunProgressRecord.h"

// CMiddleDlg dialog

IMPLEMENT_DYNAMIC(CMiddleDlg, CDialogEx)

CMiddleDlg::CMiddleDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMiddleDlg::IDD, pParent),m_funcBtDlg(this),m_baseSetDlg(this),m_selPrjDlg(this),m_samSetDlg(this),m_runMonitorDlg(this),m_dataAnaDlg(this)
{
	m_iFunBtnHeight = 40;

	m_pCurSelSwt = NULL;
	m_pCurSelDlg = NULL;
	m_curSelSonPage = ESPT_BASEINFODLG;
	m_pageId = 0;
	m_bCtrlCreated = FALSE;
	m_bExpRunning = FALSE;
	m_pRunCtrl = NULL;
	m_pExpData = NULL;
	m_pBook = NULL;
	m_ppSheet = NULL;

	m_strBackupFilePath = _T("");

	m_lnkLidTem.SetLimitMaxParam(TRUE,5);
	m_lnkPeltiaTem.SetLimitMaxParam(TRUE,5);

	m_bPowerOffExp = FALSE;

	m_bInstruConnected = FALSE;


	m_iCurCycle = -1;
	m_iCurSeg = -1;
	m_iCurStep = -1;


	m_iColCount = 10;
	m_iStartCol = 1;
	m_iFirstPaneStart = 0;
	m_iFirstPaneEnd = 4;
	m_iSecondPaneStart = 6;
	m_iSecondPaneEnd = 7;
	m_iThirdPaneStart = 9;
	m_iThirdPaneEnd = 13;
	m_iFourthPaneStart = 15;
	m_iFourthPaneEnd = 37;
}

CMiddleDlg::~CMiddleDlg()
{
	ClearBackupFileParam();

	if(NULL != m_pRunCtrl)
	{
		delete m_pRunCtrl;
		m_pRunCtrl = NULL;
	}

	if (m_pExpData != NULL)
	{
		delete m_pExpData;
		m_pExpData = NULL;
	}
}

void CMiddleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMiddleDlg, CDialogEx)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_SKIPDLG_SWTBT_BASESET,&CMiddleDlg::OnBnClickedSwtBaseSet)
	ON_BN_CLICKED(ID_SKIPDLG_SWTBT_SELECTPROJECT,&CMiddleDlg::OnBnClickedSwtSelectProject)
	ON_BN_CLICKED(ID_SKIPDLG_SWTBT_SAMSET,&CMiddleDlg::OnBnClickedSwtSamSet)
	ON_BN_CLICKED(ID_SKIPDLG_SWTBT_RUNMONITOR,&CMiddleDlg::OnBnClickedSwtRunMonitor)
	ON_BN_CLICKED(ID_SKIPDLG_SWTBT_DATAANALYSE,&CMiddleDlg::OnBnClickedSwtDataAnalyse)

	ON_MESSAGE(WN_EXPERIMENT_FLU_CHANGED,&CMiddleDlg::OnMsgExperFluChanged)
	ON_MESSAGE(WM_EXPERIMENT_RUN,&CMiddleDlg::OnMsgExperRun)
	ON_MESSAGE(WM_EXPERIMENT_FINISH,&CMiddleDlg::OnMsgExperFinished)
	ON_MESSAGE(WM_SEGMENT_OVER,&CMiddleDlg::OnMsgSegmentOver)
	ON_MESSAGE(WM_MELT_ONETEM_OVER,&CMiddleDlg::OnMsgMeltOneTemOver)
	ON_MESSAGE(WN_EXPERIMENT_TEMP_CHANGED,&CMiddleDlg::OnMsgExperTempChanged)
	ON_MESSAGE(WN_EXPERIMENT_STEP_CHANGED,&CMiddleDlg::OnMsgExperStepChanged)
	ON_MESSAGE(WM_RUNPOWEROFFPROTECT,&CMiddleDlg::OnMsgRunPowerOffProtect)
	ON_MESSAGE(WM_CHANBTCLICKED,OnMsgChanBtClicked)

	ON_BN_CLICKED(ID_GENERALPCRDLG_SWTBT_RUNSTOP,OnBnClickedSwtBtRunStop)
	ON_MESSAGE(WM_INSTRUMENT_SELECTED,&CMiddleDlg::OnMsgInstrumentSelected)

	ON_MESSAGE(WN_EXPERIMENT_DISCONNECT,&CMiddleDlg::OnMsgExperimentDisconnect)

END_MESSAGE_MAP()

BOOL CMiddleDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(rect.left,rect.top,rect.Width(),rect.Height(),CLR_SOFT_LIGHT);

	int iStartY = int((12+m_iFunBtnHeight) * m_dVertRatio);
	DrawRoundRectangleNew(pDC,0,iStartY,int(136 * m_dHoriRatio),rect.Height() - iStartY,0,16,0,0,FALSE,Color(0XFF,0XFF,0XFF),1,TRUE,Color(0XFF,0XFF,0XFF),1);
	return TRUE;
}

void CMiddleDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if(m_bCtrlCreated)
	{
		SetCtrlPos();
	}

	return;
}

void CMiddleDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIEMRID)
	{
		if (m_bExpRunning && m_bInstruConnected)
		{
			m_runMonitorDlg.ShowRemainTimeOnce();
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CMiddleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	InitSonDlg();
	CreateCtrl();
	m_bCtrlCreated = TRUE;
	InitCtrl();
	SetCtrlPos(); 

	SetCtrlText();

	ShowCtrl();

	//PS96IVDģʽ�£�����ʾ��������ҳ������ݷ���ҳ��
#ifdef PS96INSTRUMENT
	if(theApp.GetBeIVDUser())
	{
		SetPageSelected(ESPT_PROCSET,TRUE);
	}
	else
	{
		SetPageSelected(ESPT_BASEINFODLG,TRUE);
	}
#else
	SetPageSelected(ESPT_BASEINFODLG,TRUE);
#endif

	//ֻ��PS96����IVDģʽ
#ifdef PS96INSTRUMENT
	if(theApp.GetBeIVDUser())
	{
		SetEditMode(EEEM_IVD);
	}
#endif

	return TRUE;
}

void CMiddleDlg::CreateCtrl()
{
	m_swtBaseSet.Create(_T("Base Settings"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SKIPDLG_SWTBT_BASESET);
	m_swtSelectPrj.Create(_T("Plate Edit"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SKIPDLG_SWTBT_SELECTPROJECT);
	m_swtSamSet.Create(_T("Sample"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SKIPDLG_SWTBT_SAMSET);
	m_swtRunMonitor.Create(_T("Monitor"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SKIPDLG_SWTBT_RUNMONITOR);
	m_swtDataAna.Create(_T("Analysis"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON,CRect(0,0,0,0),this,ID_SKIPDLG_SWTBT_DATAANALYSE);

	m_btnRunStop.Create(_T("Start"),WS_CHILD | WS_CLIPSIBLINGS | BS_PUSHBUTTON, CRect(0,0,0,0),this,ID_GENERALPCRDLG_SWTBT_RUNSTOP);
	m_stcConnectState.Create(_T("Status"),WS_CHILD, CRect(0,0,0,0), this,ID_GENERALPCRDLG_ST_CONNECTSTATE);

	m_funcBtDlg.Create(IDD_MIDDLEDLG, this);
}

void CMiddleDlg::InitCtrl()
{
	CIconBtInfo iconBtInfo;

	iconBtInfo.m_rcIcon = Rect( int(35 * m_dHoriRatio), int(20 * m_dVertRatio), int(36 * m_dHoriRatio), int(36 * m_dVertRatio));
	iconBtInfo.m_rcText = CRect(int(20 * m_dHoriRatio), int(58 * m_dVertRatio), int(72 * m_dHoriRatio), int(84 * m_dVertRatio));
	iconBtInfo.m_strIconPath = _T(".\\BkPic\\start.png");
	m_btnRunStop.SetMyFont(m_stTxFont, int(24*m_dTxRatio),FW_NORMAL,TYPE_PIXEL);
	m_btnRunStop.SetIconBtInfo(iconBtInfo);
	m_btnRunStop.SetTextColor(RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	int iFontH = 18 * m_dTxRatio;

	iconBtInfo.m_rcIcon = Rect(18 * m_dHoriRatio,18 * m_dVertRatio,24 * m_dHoriRatio,24 * m_dVertRatio);
	iconBtInfo.m_rcText = CRect(44 * m_dHoriRatio,16 * m_dVertRatio,134 * m_dHoriRatio,43 * m_dVertRatio);
	iconBtInfo.m_bRoundRectBk = FALSE;
	iconBtInfo.m_iSonIconNum = 2;
	iconBtInfo.m_bSonIconPosOrder = FALSE;
	iconBtInfo.m_clrNormal = Color(0XFF,0XFF,0XFF);
	iconBtInfo.m_clrHover = Color(0XFF,0XFF,0XFF);
	iconBtInfo.m_clrSelected = COLOR_SOFT_THEME;
	iconBtInfo.m_clrDisabled = COLOR_SOFT_THEME;

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\BaseSet.png");
	m_swtBaseSet.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtBaseSet.SetIconBtInfo(iconBtInfo);
	m_swtBaseSet.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\SamplateEdit.png");
	m_swtSelectPrj.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtSelectPrj.SetIconBtInfo(iconBtInfo);
	m_swtSelectPrj.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\SampleRegister.png");
	m_swtSamSet.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtSamSet.SetIconBtInfo(iconBtInfo);
	m_swtSamSet.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\RunMonitor.png");
	m_swtRunMonitor.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtRunMonitor.SetIconBtInfo(iconBtInfo);
	m_swtRunMonitor.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	iconBtInfo.m_strIconPath = _T(".\\BkPic\\DataAnalyse.png");
	m_swtDataAna.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtDataAna.SetIconBtInfo(iconBtInfo);
	m_swtDataAna.SetTextColor(RGB(0X39,0X45,0X65),RGB(0X39,0X45,0X65),RGB(0XFF,0XFF,0XFF),RGB(0XFF,0XFF,0XFF));

	//m_stcConnectState.SetTextColor(clrText);
	m_stcConnectState.SetMyFont(m_stTxFont, iFontH, FW_NORMAL,TYPE_PIXEL);
	m_stcConnectState.SetWordStyle(DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
}

void CMiddleDlg::SetCtrlPos()
{
	CRect rect;
	GetClientRect(&rect);

	m_funcBtDlg.SetWindowPos(this,0,0,rect.Width(),int(m_iFunBtnHeight*m_dVertRatio),SWP_NOZORDER);

	int startX = 0;
	int startY = int((28+m_iFunBtnHeight) * m_dVertRatio);
	int swtWidth = int(136 * m_dHoriRatio);
	int iHeight = 60 * m_dVertRatio;
	int iInterval = 26 * m_dVertRatio;
	int iTempH = iHeight + iInterval;

	CRect rectBtn(startX, startY, startX+swtWidth, startY+iHeight);
	m_swtBaseSet.MoveWindow(rectBtn);

	rectBtn.OffsetRect(0, iTempH);
	m_swtSelectPrj.MoveWindow(rectBtn);
	rectBtn.OffsetRect(0, iTempH);
	m_swtSamSet.MoveWindow(rectBtn);
	rectBtn.OffsetRect(0, iTempH);
	m_swtRunMonitor.MoveWindow(rectBtn);
	rectBtn.OffsetRect(0, iTempH);
	m_swtDataAna.MoveWindow(rectBtn);

	CRect rectRun;
	rectRun.left = rectBtn.left + rectBtn.Width()/12;
	rectRun.right = rectBtn.right - rectBtn.Width()/12;
	rectRun.bottom = rect.bottom - rectBtn.Height();
	rectRun.top = rectRun.bottom - int(rectBtn.Height() * 1.6);
	m_btnRunStop.MoveWindow(rectRun);

	m_rectConntectState = rectBtn;
	m_rectConntectState.left += 20;
	m_rectConntectState.bottom = rectRun.top - iInterval;
	m_rectConntectState.top = m_rectConntectState.bottom - iHeight;
	m_stcConnectState.MoveWindow(m_rectConntectState);

	startX = int(136 * m_dHoriRatio);
	int iTempInt3 = int(m_iFunBtnHeight * m_dVertRatio);
	int iTempInt1 = rect.Height() - iTempInt3;
	int iTempInt2 = rect.Width() - startX;

	m_baseSetDlg.SetWindowPos(this,startX,iTempInt3,iTempInt2,iTempInt1,SWP_NOZORDER);
	m_selPrjDlg.SetWindowPos(this,startX,iTempInt3,iTempInt2,iTempInt1,SWP_NOZORDER);
	m_samSetDlg.SetWindowPos(this,startX,iTempInt3,iTempInt2,iTempInt1,SWP_NOZORDER);
	m_runMonitorDlg.SetWindowPos(this,startX,iTempInt3,iTempInt2,iTempInt1,SWP_NOZORDER);
	m_dataAnaDlg.SetWindowPos(this,startX,iTempInt3,iTempInt2,iTempInt1,SWP_NOZORDER);

}

void CMiddleDlg::ShowCtrl()
{
	ShowBt();
}

void CMiddleDlg::ShowBt()
{
	m_funcBtDlg.ShowWindow(SW_SHOW);

	m_swtSelectPrj.ShowWindow(SW_SHOW);
	m_swtSamSet.ShowWindow(SW_SHOW);
	m_swtRunMonitor.ShowWindow(SW_SHOW);
	
	//PS96IVDģʽ�£�����ʾ��������ҳ������ݷ���ҳ��
#ifdef PS96INSTRUMENT
	if(!theApp.GetBeIVDUser())
	{
		m_swtBaseSet.ShowWindow(SW_SHOW);
		m_swtDataAna.ShowWindow(SW_SHOW);
	}
#else
	m_swtBaseSet.ShowWindow(SW_SHOW);
	m_swtDataAna.ShowWindow(SW_SHOW);
#endif

	m_btnRunStop.ShowWindow(SW_SHOW);
	m_stcConnectState.ShowWindow(SW_SHOW);
}

void CMiddleDlg::InitSonDlg()
{
	InitBaseSetDlg();
	InitSelectPrjDlg();
	InitSamSetDlg();
	InitRunMonitroDlg();
	InitDataAnaDlg();
}

void CMiddleDlg::InitBaseSetDlg()
{
	m_baseSetDlg.Create(IDD_MIDDLEDLG,this);
}

void CMiddleDlg::InitSelectPrjDlg()
{
	m_selPrjDlg.Create(IDD_MIDDLEDLG,this);
}

void CMiddleDlg::InitSamSetDlg()
{
	m_samSetDlg.Create(IDD_MIDDLEDLG,this);
}

void CMiddleDlg::InitRunMonitroDlg()
{
	m_runMonitorDlg.Create(IDD_MIDDLEDLG,this);
}

void CMiddleDlg::SetCtrlText()
{
	m_funcBtDlg.SetCtrlText();

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	m_swtBaseSet.SetWindowText(pLangInfo->GetText(13));
	m_swtSelectPrj.SetWindowText(pLangInfo->GetText(14));
	m_swtSamSet.SetWindowText(pLangInfo->GetText(15));
	m_swtRunMonitor.SetWindowText(pLangInfo->GetText(16));
	m_swtDataAna.SetWindowText(pLangInfo->GetText(17));

	if(m_bExpRunning)
		m_btnRunStop.SetWindowText(pLangInfo->GetText(5));
	else
		m_btnRunStop.SetWindowText(pLangInfo->GetText(4));

	SetConnectState(m_bInstruConnected);
}

void CMiddleDlg::InitDataAnaDlg()
{
	m_dataAnaDlg.Create(IDD_MIDDLEDLG,this);
}

void CMiddleDlg::OnBnClickedSwtBaseSet()
{
	SetPageSelected(ESPT_BASEINFODLG);
}

void CMiddleDlg::OnBnClickedSwtSelectProject()
{
	SetPageSelected(ESPT_PROCSET);
}

void CMiddleDlg::OnBnClickedSwtSamSet()
{
	SetPageSelected(ESPT_SAMSET);
}

void CMiddleDlg::OnBnClickedSwtRunMonitor()
{
	SetPageSelected(ESPT_RUNMONITOR);
}

void CMiddleDlg::OnBnClickedSwtDataAnalyse()
{
	SetPageSelected(ESPT_DATAANALYSE);
}

LRESULT CMiddleDlg::OnMsgExperFluChanged(WPARAM wParam,LPARAM lParam)
{
	if(NULL == m_pExpData)
	{
		return 1;
	}

	int iChanNum = m_pExpData->GetChannelCount();
	if (theApp.GetBeAdminPermission() || theApp.GetBeIVDUser())
		iChanNum --;
	int iTubeNum = m_pExpData->GetPlateInfo()->GetTubeCount();
	int iTubeIndex = 0;
	int iDataNumToProcess = wParam;

	CMyPoint pt;

	eDataType eAmpDataType = FLU_FILTER,eMeltDataType = FLU_FILTER;
#ifdef MONITOR_USE_AMP_DELTARN
	eAmpDataType = FLU_FILTER;
#endif

	if(0 == lParam)	//�µ��������ݲ���
	{
		if (iDataNumToProcess > m_pExpData->GetAmplifyDataNum())
			iDataNumToProcess = m_pExpData->GetAmplifyDataNum();

		if(!m_pExpData->ProcessAmplifyFluData())
		{

		}

		int iDataSize = sizeof(double) * iDataNumToProcess;
		double *pDYValue = NULL;
		pDYValue = new double[iDataNumToProcess];
		if(NULL == pDYValue)
		{
			return 2;
		}
		memset(pDYValue,0,iDataSize);

		m_runMonitorDlg.m_crvAmlOri.DeleteLineData(FALSE);

		for(int i = 0;i < iChanNum;++i)
		{
			int iRow = wParam + 2,iCol = 0;
			for(int j = 0;j < iTubeNum;++j)
			{
				iTubeIndex = i * iTubeNum + j;

				memset(pDYValue,0,iDataSize);
				m_pExpData->GetAmplifyChannelFluY(j + 1,eAmpDataType,i,iDataNumToProcess,pDYValue);
				for(int k = 0;k < iDataNumToProcess;++k)
				{
					pt.m_X = k + 1;
					pt.m_Y = pDYValue[k];
					//lzh:18�ܣ�ÿ��5�����ݣ�һ��90������
					m_runMonitorDlg.m_crvAmlOri.AddNewDotAutoCompareXYMinMax(iTubeIndex, pt);
				}

				//�µ��������ݲ��������浽�����ļ��У�excel�ļ���
				if(NULL != m_pBook && NULL != m_ppSheet && NULL != m_ppSheet[i] && !m_strBackupFilePath.IsEmpty())
				{
					m_pExpData->GetAmplifyChannelFluY(j + 1,FLU_ORIGINAL,i,iDataNumToProcess,pDYValue);
					m_ppSheet[i]->writeNum(iRow,++iCol,pDYValue[iDataNumToProcess - 1]);
				}		
			}
		}

		m_runMonitorDlg.m_crvAmlOri.HandSetGeneralLineYAxisMinMax(1.2,TRUE);

		delete [] pDYValue;
	}
	else if(1 == lParam)	//�µ��������ݲ���
	{
		if (iDataNumToProcess > m_pExpData->GetMeltDataNum())
			iDataNumToProcess = m_pExpData->GetMeltDataNum();

		if(!m_pExpData->MonitorProcessMeltFluData(iDataNumToProcess))
		{

		}

		double *pDX = NULL,*pDY = NULL,*pDYGain = NULL,*pDIndex = NULL;
		if(iDataNumToProcess > 0)
		{
			pDX = new double[iDataNumToProcess];
			if(NULL == pDX)
			{
				return 3;
			}

			pDY = new double[iDataNumToProcess];
			if(NULL == pDY)
			{
				delete [] pDX;
				return 4;
			}

			pDIndex = new double[iDataNumToProcess];
			if(NULL == pDIndex)
			{
				delete [] pDX;
				delete [] pDY;
				return 5;
			}

			int iTempSize = sizeof(double) * iDataNumToProcess;
			memset(pDX,0,iTempSize);
			memset(pDY,0,iTempSize);
			for(int i = 0;i < iDataNumToProcess;++i)
			{
				pDIndex[i] = i + 1;
			}
		}

		m_runMonitorDlg.m_crvMeltOri.DeleteLineData(FALSE);

		for(int i = 0;i < iChanNum;++i)
		{
			int iRow = wParam + 2,iCol = 0;
			for(int j = 0;j < iTubeNum;++j)
			{
				iTubeIndex = i * iTubeNum + j;

				m_pExpData->GetMeltCycleX(j + 1,iDataNumToProcess,pDX);
				m_pExpData->GetMeltChannelFluY(j + 1,FLU_FILTER,i,iDataNumToProcess,pDY);

				//���¶Ƚ��д�������ֹ���ߴ��
				double dSlope = 0,dIntercept = 0;
				if(iDataNumToProcess > 1 && 0 == LeastSquare(pDIndex,pDX,iDataNumToProcess,dSlope,dIntercept))
				{
					for(int k = 0;k < iDataNumToProcess;++k)
					{
						pDX[k] = pDIndex[k] * dSlope + dIntercept;
					}
				}

				for(int k = 0;k < iDataNumToProcess;++k)
				{
					pt.m_X = pDX[k];
					pt.m_Y = pDY[k];
					m_runMonitorDlg.m_crvMeltOri.AddNewDotAutoCompareXYMinMax(iTubeIndex,pt);
				}

				////�µ��������ݲ��������浽�����ļ��У�excel�ļ���
				//if(NULL != m_pBook && NULL != m_ppSheet && NULL != m_ppSheet[i] && !m_strBackupFilePath.IsEmpty())
				//{
				//	m_pExpData->GetAmplifyChannelFluY(j + 1,FLU_ORIGINAL,i,iDataNumToProcess,pDYValue);
				//	m_ppSheet[i]->writeNum(iRow,++iCol,pDYValue[iDataNumToProcess - 1]);
				//}		
			}
		}

		m_runMonitorDlg.m_crvMeltOri.HandSetGeneralLineYAxisMinMax(1.2,TRUE);

		if(iDataNumToProcess > 0)
		{
			delete [] pDX;
			delete [] pDY;
			delete [] pDIndex;
		}
	}

	//�µ����ݲ����󣬱��湤���ļ�
	m_pExpData->SaveFile();
	//���汸�ݹ����ļ�
	m_pExpData->SaveFile(1);

	//����д�뱸��excel�ļ�
	if(NULL != m_pBook && !m_strBackupFilePath.IsEmpty())
	{
		m_pBook->save(m_strBackupFilePath);
	}

	return 0;
}

LRESULT CMiddleDlg::OnMsgExperRun(WPARAM wParam,LPARAM lParam)
{
	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	if(NULL == pMainDlg->GetSafeHwnd())
	{
		//��¼������Ϣ
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


	//�������ߺ�������Ϣ
	SetCurveHoriAxisParam();

	//�����۽����ߺ�������Ϣ
	double dStartTem = 60,dEndTem = 90;
	m_pExpData->GetFirstMeltCollectStartAndEndTemperature(dStartTem,dEndTem);
	m_runMonitorDlg.m_crvMeltOri.SetHoriStartValue(dStartTem);
	m_runMonitorDlg.m_crvMeltOri.SetHoriEndValue(dEndTem);

	//�л������м��ҳ��
	m_bExpRunning = TRUE;
	SetRunBtnState(TRUE);
	SetPageSelected(ESPT_RUNMONITOR);

	//ʵ�鿪ʼ�����ü��ҳ��ͨ��Ϊȫѡ״̬
	m_pExpData->SetMonitorChannelBtAllSelected();
	m_funcBtDlg.SetChanBtSelectState(m_pExpData->GetMonitorChannelBtSelectState());

	//����������ʵ���Ӧ��ҳ���л���ť�������
	pMainDlg->m_workDlg.m_fileManageDlg.SetPageFirstColumn(GetPageId());

	//ʵ�鿪ʼ�����ü��ҳ��װ�Ϊȫѡ״̬
	m_runMonitorDlg.m_samPlateDlg.SetAllSelectState(TRUE,TRUE);
	m_runMonitorDlg.UpdateCurveSelectState(TRUE);

	//���ü��ҳ����ʱ��
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stTotalSec.SetWindowText(SecondToHourMinuteSecond(m_pExpData->GetExperimentInfo()->GetTotalSec()));

	//��������ʱ��ʱ��
	SetTimer(TIEMRID,1000,NULL);

	//ʵ�鿪ʼʱ�����ļ�
	m_pExpData->SaveFile();

	SetEditMode(EEEM_RUNNING);

	return 0;
}

LRESULT CMiddleDlg::OnMsgExperFinished(WPARAM wParam,LPARAM lParam)
{
	KillTimer(TIEMRID);

	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;
	if(NULL == pMainDlg->GetSafeHwnd())
	{
		//��¼������Ϣ
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

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	
	//������ҳ��ʣ��ʱ�䲻Ϊ�㣬������
	if(pExpInfo->GetRemainSec() > 0 && STATUS_STOP == wParam)
	{
		pExpInfo->SetRemainSec(0);
	}

	//���ʵ����������������;ֹͣ��ʵ�飬���¼���ʵ����ʱ�䣬��ֹʵ�鿪ʼʱ�䡢ʵ�����ʱ�䡢ʵ����ʱ�䲻һ��
	if(!m_bPowerOffExp && (STATUS_STOP == wParam || STATUS_ASK_STOP == wParam))
	{
		CTimeSpan tmSpan = m_pExpData->m_tmEnd - m_pExpData->m_tmStart;

		int iTotalSec = tmSpan.GetTotalSeconds();
		pExpInfo->SetTotalSec(iTotalSec);
		pExpInfo->SetHasRunSec(iTotalSec);
	}

	//����ʵ��ҳ���л���ťΪ��ͨ״̬
	pMainDlg->m_workDlg.m_fileManageDlg.SetPageSwitchBtState(m_pageId,FALSE);

	//���ü��ҳ����Ϣ
	CString strSurplusTime = SecondToHourMinuteSecond(pExpInfo->GetRemainSec());
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stEndTm.SetWindowText(pExpInfo->GetEndTime());
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stTotalSec.SetWindowText(SecondToHourMinuteSecond(pExpInfo->GetTotalSec()));
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stRemainSec.SetWindowText(strSurplusTime);

	//���÷���ҳ����Ϣ
	m_dataAnaDlg.ExperimentOver(m_pExpData);

	//ʵ�����ʱ����¼ÿ�����߿ؼ���Ĭ��ֵ
	m_dataAnaDlg.RecordDefaultAxisInfo();

	ClearBackupFileParam();

	//ʵ����������湤���ļ�
	m_pExpData->SaveFile();
	//ʵ����������汸�ݹ����ļ�
	m_pExpData->SaveFile(1);
	theApp.DeleteSavePathName(m_pExpData->GetExperimentInfo()->GetSaveDataFileName());

	m_bExpRunning = FALSE;
	SetRunBtnState(FALSE);

	if (m_pExpData->GetReconnectState() == 1)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(528));
	}
	else if (m_pExpData->GetReconnectState() == 2)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(527));
	}

	//������ʾ����
	if(STATUS_STOP_ERROR == wParam)
	{
		//PopWarningTipInfo(theApp.m_pLangInfo->GetText(387));
	}
	else
	{
		m_btnRunStop.EnableWindow(TRUE);

		int iTipStrId = 155;
		if(STATUS_ASK_STOP == wParam)
		{
			iTipStrId = 157;
		}
		PopNormalTipInfo(theApp.m_pLangInfo->GetText(iTipStrId));
	}
	CString strRunInfo = m_pRunCtrl->GetRunLog();
	if (!strRunInfo.IsEmpty())
	{
		PopNormalTipInfo(strRunInfo);
	}

	//ʵ������󣬻ָ����ֿؼ��ı༭״̬
	SetEditMode(EEEM_END);

	//ʵ�����ʱ�����û��ͨ���������̨ʽ���ϵ����⣬������ʵ��Ӷϵ籣�����Ƴ�
	if(STATUS_ASK_STOP == wParam || STATUS_STOP == wParam)
	{
		//�ɽ�һ���ж������Ƿ�ִ�����
		theApp.m_powerOffProtect.DeleteRunProgress(m_pageId);
	}

	m_bPowerOffExp = FALSE;

	return 0;
}

LRESULT CMiddleDlg::OnMsgSegmentOver(WPARAM wParam,LPARAM lParam)
{
	//ʵ�����������Ϻ󣬻�����������������Ϻ󣬽�����������֮ǰ�����û��ͨ�����⣬������ʵ��Ӷϵ籣�����Ƴ�
	theApp.m_powerOffProtect.DeleteRunProgress(m_pageId);
	return 0;
}

LRESULT CMiddleDlg::OnMsgMeltOneTemOver(WPARAM wParam,LPARAM lParam)
{
	//ʵʱ�����Ѿ��ɼ����ݲ����������ʵʱ�¶ȵ�
	theApp.m_powerOffProtect.SetRunProgress(m_pageId,wParam / 100.0);
	return 0;
}

LRESULT CMiddleDlg::OnMsgExperTempChanged(WPARAM wParam,LPARAM lParam)
{
	if(NULL == m_pExpData)
	{
		return 1;
	}

	double dTemPeltia1 = m_pExpData->GetCurTemp(CTemperatureData::TEMP_CH_PET) / 100.0;
	
	//�˲�����
	m_lnkPeltiaTem.AddTail(dTemPeltia1);
	dTemPeltia1 = GetAveragePeltiaTem();

	dTemPeltia1 = PeltiaTemAmend(dTemPeltia1);

	//�������ģ��ʵʱ�¶ȵ�ʵ����������
	m_pExpData->m_dCurTem = dTemPeltia1;

	CMyPoint pt;
	pt.m_X = m_pExpData->GetCurTempCollectTime();

	pt.m_Y = dTemPeltia1;
	m_runMonitorDlg.m_crvTem.AddNewDotAutoCompareXYMinMax(0,pt,TRUE,1.2);

	CString strTemp(_T("")),strTubeTem(_T(""));
	strTubeTem.Format(_T("%.1lf��"),pt.m_Y);
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stTemTube.SetWindowText(strTubeTem);

	if(m_pExpData->GetExperimentInfo()->GetLidHeader())
	{
		double dLidTem = m_pExpData->GetCurTemp(CTemperatureData::TEMP_CH_LID) / 100.0;

		//�˲�����
		m_lnkLidTem.AddTail(dLidTem);
		dLidTem = GetAverageLidTem();

		dLidTem = LidTemAmend(dLidTem);

		pt.m_Y = dLidTem;
		m_runMonitorDlg.m_crvTem.AddNewDotAutoCompareXYMinMax(1,pt,TRUE,1.2);

		strTemp.Format(_T("%.1lf��"),pt.m_Y);
		m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stTemLid.SetWindowText(strTemp);
	}

	return 0;
}

LRESULT CMiddleDlg::OnMsgExperStepChanged(WPARAM wParam,LPARAM lParam)
{
	if(NULL == m_pRunCtrl)
	{
		return 1;
	}

	int iCurCyle = m_pRunCtrl->GetCurCycle();
	int iCurSeg = m_pRunCtrl->GetCurSegment();
	int iCurStep = m_pRunCtrl->GetCurStep();

	m_iCurCycle = iCurCyle;
	m_iCurSeg = iCurSeg;
	m_iCurStep = iCurStep;

	m_runMonitorDlg.CurrentSegmentStepChangedOnRun(iCurCyle,iCurSeg,iCurStep);

	theApp.m_powerOffProtect.SetRunProgress(m_pageId,iCurSeg,iCurStep,iCurCyle);

	//��ǰ���г���Ρ��������ѭ�����ı�󣬼�ʱ�����¶���Ϣ����ʱ��Ϣ�ȣ���ֹ�������쳣���ʱ�������ļ��ﱣ�����Ϣ���쳣ʱ�̵�ʵ����Ϣƫ��̫�󡱣���ʱ�����汸���ļ���20220823��������
	m_pExpData->SaveFile();

	return 0;
}

LRESULT CMiddleDlg::OnMsgRunPowerOffProtect(WPARAM wParam,LPARAM lParam)
{
	RunPowerOffProtectExperiment();
	return 0;
}

LRESULT CMiddleDlg::OnMsgExperimentDisconnect(WPARAM wParam,LPARAM lParam)
{
	//RunPowerOffProtectExperiment();
	m_bInstruConnected = BOOL(wParam);
	SetConnectState(m_bInstruConnected);

	m_btnRunStop.EnableWindow(m_bInstruConnected);

	return 0;
}


void CMiddleDlg::SetPageSelected(ESON_PAGE_TYPE eSonPageType,BOOL bInit /*= FALSE*/)
{
	BOOL bShowChanBt = FALSE;
	if(ESPT_BASEINFODLG == eSonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtBaseSet.EnableWindow(FALSE);
		m_baseSetDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_baseSetDlg;
		m_pCurSelSwt = &m_swtBaseSet;

		m_funcBtDlg.SetModeExpBaseSet();
	}
	else if(ESPT_PROCSET == eSonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtSelectPrj.EnableWindow(FALSE);
		m_selPrjDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_selPrjDlg;
		m_pCurSelSwt = &m_swtSelectPrj;

		m_funcBtDlg.SetModeExpProcSet();
	}
	else if(ESPT_SAMSET == eSonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtSamSet.EnableWindow(FALSE);
		m_samSetDlg.ShowWindow(SW_SHOW);
		m_pCurSelDlg = &m_samSetDlg;
		m_pCurSelSwt = &m_swtSamSet;

		m_funcBtDlg.SetModeExpSamSet();
	}
	else if(ESPT_RUNMONITOR == eSonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd())
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtRunMonitor.EnableWindow(FALSE);
		m_runMonitorDlg.ShowWindow(SW_SHOW);
		m_runMonitorDlg.UpdateWindow();
		m_pCurSelDlg = &m_runMonitorDlg;
		m_pCurSelSwt = &m_swtRunMonitor;

		bShowChanBt = TRUE;

		m_funcBtDlg.SetModeExpRunMonitor();
	}
	else if(ESPT_DATAANALYSE == eSonPageType)
	{
		if(NULL != m_pCurSelDlg->GetSafeHwnd()) 
		{
			m_pCurSelDlg->ShowWindow(SW_HIDE);
		}
		if(NULL != m_pCurSelSwt->GetSafeHwnd())
		{
			m_pCurSelSwt->EnableWindow(TRUE);
		}
		m_swtDataAna.EnableWindow(FALSE);
		m_dataAnaDlg.ShowWindow(SW_SHOW);
		m_dataAnaDlg.UpdateWindow();
		m_pCurSelDlg = &m_dataAnaDlg;
		m_pCurSelSwt = &m_swtDataAna;

		bShowChanBt = TRUE;
		m_funcBtDlg.SetModeExpDataAnalyse();
	}

	m_curSelSonPage = eSonPageType;

	//����ͨ���л���ťѡ��״̬
	if(NULL != m_pExpData)
	{
		vector<int> vecChanSelState;
		if(ESPT_RUNMONITOR == eSonPageType)
		{
			vecChanSelState = m_pExpData->GetMonitorChannelBtSelectState();
		}
		else if(ESPT_DATAANALYSE == eSonPageType)
		{
			vecChanSelState = m_pExpData->GetDataAnaChannelBtSelectState();
		}
		m_funcBtDlg.SetChanBtSelectState(vecChanSelState);
	}

	//����ͨ���л���ť��ʾ״̬
	m_funcBtDlg.SetChanBtShowState(bShowChanBt);

	return;
}

void CMiddleDlg::UpdateFuncBtMode()
{
	if(NULL != m_baseSetDlg.GetSafeHwnd() && m_baseSetDlg.IsWindowVisible())
	{
		m_funcBtDlg.SetModeExpBaseSet();
		return;
	}
	else if(NULL != m_selPrjDlg.GetSafeHwnd() && m_selPrjDlg.IsWindowVisible())
	{
		m_funcBtDlg.SetModeExpProcSet();
		return;
	}
	else if(NULL != m_samSetDlg.GetSafeHwnd() && m_samSetDlg.IsWindowVisible())
	{
		m_funcBtDlg.SetModeExpSamSet();
		return;
	}
	else if(NULL != m_runMonitorDlg.GetSafeHwnd() && m_runMonitorDlg.IsWindowVisible())
	{
		m_funcBtDlg.SetModeExpRunMonitor();
		return;
	}
	else if(NULL != m_dataAnaDlg.GetSafeHwnd() && m_dataAnaDlg.IsWindowVisible())
	{
		m_funcBtDlg.SetModeExpDataAnalyse();
		return;
	}

	return;
}

void CMiddleDlg::SetPageId(int pageId)
{
	m_pageId = pageId;
}

int CMiddleDlg::GetPageId()
{
	return m_pageId;
}

int CMiddleDlg::Save()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	SynchronizeExperimentSet();

	m_pExpData->SaveFile();

	return 0;
}

// ��ʱʹ�����������Ϊ���Թ���ʹ��
BOOL CMiddleDlg::SaveAsFile(BOOL bTemplate)
{
	if(NULL == m_pExpData)
	{
		return FALSE;
	}

	CString strOldExpName = m_pExpData->GetFileName();
	if(CExperimentData::STATUS_NEW == m_pExpData->GetStatus())
	{
		strOldExpName = m_pExpData->GetExperimentInfo()->GetExpName();
	}

	SynchronizeExperimentSet();

	CString strTemp(_T(""));
	if(CExperimentData::STATUS_NEW == m_pExpData->GetStatus())
	{
		strTemp = m_pExpData->GetExperimentInfo()->GetExpName();
	}
	else
	{
		strTemp = m_pExpData->GetFileName();
	}

	CString strSaveFormat(_T("")), strSuffix(_T(""));
	if(bTemplate)
	{
		strSuffix.Format(_T(".%s"), PCR_TEMPLATE_FILE_SUFFIX);
		strSaveFormat.Format(_T("QPCR Template File(*.%s)|*.%s|All File(*.*)|*.*||"), PCR_TEMPLATE_FILE_SUFFIX, PCR_TEMPLATE_FILE_SUFFIX);	
	}
	else
	{
		strSuffix.Format(_T(".%s"), PCR_DATA_FILE_SUFFIX);
		strSaveFormat.Format(_T("QPCR Data File(*.%s)|*.%s|All File(*.*)|*.*||"), PCR_DATA_FILE_SUFFIX, PCR_DATA_FILE_SUFFIX);
	}
	
	CFileDialog fileDlg(FALSE,_T(""),strTemp, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,strSaveFormat);
	if(IDOK != fileDlg.DoModal())
	{
		return FALSE;
	}

	m_pExpData->SaveFile(fileDlg.GetPathName(),bTemplate);

	//����Ϊ�ļ�����󣬼��ʵ�����Ʊ���·���Ƿ�ı䣬������ʵ���ļ�����·��
	if(strOldExpName != fileDlg.GetFileTitle())
	{
		CWorkDlg* pParentWnd = (CWorkDlg*)GetParent();
		if(NULL != pParentWnd->GetSafeHwnd())
		{
			pParentWnd->m_fileManageDlg.SetExperName(GetPageId(),fileDlg.GetFileTitle());
			pParentWnd->m_fileManageDlg.SetPageFilePath(GetPageId(),fileDlg.GetPathName());
		}
	}
	m_pExpData->SetFilePath(fileDlg.GetPathName());

	return TRUE;
}

int CMiddleDlg::Delete()
{
	BOOL bIVDUser = theApp.GetBeIVDUser();
	if(m_selPrjDlg.IsWindowVisible())
	{
		vector<int> vecId;
		m_selPrjDlg.m_samPlateDlg.GetPlateSelectedSite(vecId);

#ifdef DELETE_PART_TUBEINFO
		m_selPrjDlg.m_samPlateDlg.DeleteTubeInfo(vecId,EDT_DELETE1,bIVDUser);
		m_samSetDlg.m_samPlateDlg.DeleteTubeInfo(vecId,EDT_DELETE1,bIVDUser);
#else
		m_selPrjDlg.m_samPlateDlg.DeleteTubeInfo(vecId,bIVDUser);
		m_samSetDlg.m_samPlateDlg.DeleteTubeInfo(vecId,bIVDUser);
#endif

		m_samSetDlg.ReSetLstSamInfo(TRUE);

		//ͬ����CExperiment����
		m_pExpData->SetTubeInfo(m_selPrjDlg.m_samPlateDlg.GetTubeInfoPtr(),m_selPrjDlg.m_samPlateDlg.m_iTotalSiteNum);

		return 0;
	}
	else if(m_samSetDlg.IsWindowVisible())
	{
		vector<int> vecId;
		m_samSetDlg.m_samPlateDlg.GetPlateSelectedSite(vecId);

#ifdef DELETE_PART_TUBEINFO
		m_samSetDlg.m_samPlateDlg.DeleteTubeInfo(vecId,EDT_DELETE2,bIVDUser);
		m_selPrjDlg.m_samPlateDlg.DeleteTubeInfo(vecId,EDT_DELETE2,bIVDUser);
#else
		m_samSetDlg.m_samPlateDlg.DeleteTubeInfo(vecId,bIVDUser);
		m_selPrjDlg.m_samPlateDlg.DeleteTubeInfo(vecId,bIVDUser);
#endif

		m_samSetDlg.ReSetLstSamInfo(TRUE);

		//ͬ����CExperiment����
		m_pExpData->SetTubeInfo(m_samSetDlg.m_samPlateDlg.GetTubeInfoPtr(),m_samSetDlg.m_samPlateDlg.m_iTotalSiteNum);

		return 0;
	}
	
	return -1;
}

int CMiddleDlg::ExportCompleteInfo(CString strFilePath)
{
	libxl::Book* pBook = xlCreateXMLBook();
	if(NULL == pBook)
	{
		PopWarningTipInfo(_T("Failed to create execel file!"));
		return -2;
	}

	pBook->setKey(_T("Halil Kural"), _T("windows-2723210a07c4e90162b26966a8jcdboe")); 

	int iRet1 = 0,iRet2,iRet3 = 0,iRet4 = 0,iRet5 = 0;
	if(m_pExpData->GetBePS96ExperFile())
	{
		iRet1 = ExprotPS96OldResultSheet(pBook);
	}
	else
	{
		iRet1 = ExportResultSheet(pBook);
	}
	if(0 != iRet1)
	{
		goto NEWSHEETERROR;
	}

	if(GetBeExportData())
	{
		iRet2 = ExportTubeInfoSheet(pBook,theApp.m_eSamExportMode);
		if(0 != iRet2)
		{
			goto NEWSHEETERROR;
		}

		iRet3 = ExportAmlDataSheet(pBook,theApp.m_eSamExportMode);
		if(0 != iRet3)
		{
			goto NEWSHEETERROR;
		}
	}
	
	iRet4 = ExportInstrumentInfoSheet(pBook);
	if(0 != iRet4)
	{
		goto NEWSHEETERROR;
	}
	
	iRet5 = ExportMeltData(pBook);
	if(iRet5 < 0)
	{
		goto NEWSHEETERROR;
	}

	if(pBook->save(strFilePath)) 
	{
		//�����ļ�Ϊֻ��ģʽ
		DWORD dwErrCode = 0;
		WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
		if(GetFileAttributesEx(strFilePath,GetFileExInfoStandard,&attrs))
		{
			DWORD dwNewFileAttr = attrs.dwFileAttributes | FILE_ATTRIBUTE_READONLY;
			if(!SetFileAttributesW(strFilePath,dwNewFileAttr))
			{
				dwErrCode = GetLastError();
			}
		}
		else
		{
			dwErrCode = GetLastError();
		}

		::ShellExecute(NULL,_T("open"),strFilePath,NULL,NULL,SW_SHOW);        
	}
	else
	{
		pBook->errorMessage();
	}

	pBook->release();

	return 0;

NEWSHEETERROR:
	pBook->release();
	return -3;
}

int CMiddleDlg::ExportTemperature(CString strFilePath /*= _T("")*/)
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	int iTemChanNum = m_pExpData->GetTempChannelCount();
	if(iTemChanNum < 0)
	{
		return -2;
	}
	++iTemChanNum;

	CTemperatureData* pTemData = m_pExpData->GetTemperatureData();
	if(NULL == pTemData)
	{
		return -3;
	}

	if(strFilePath.IsEmpty())
	{
		//����Ĭ������Ϊʵ�����ƣ�20220824��������
		CFileDialog dlg(FALSE,_T("xlsx"),m_baseSetDlg.GetExpName(),OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("Excel File(*.xlsx)|*.xlsx||"),NULL);
		if(IDCANCEL == dlg.DoModal()) 
		{
			return 1;
		}
		strFilePath = dlg.GetPathName();
	}

	libxl::Book* pBook = xlCreateXMLBook();
	if(NULL == pBook)
	{
		PopWarningTipInfo(_T("Failed to create execel file!"));
		return -2;
	}

	pBook->setKey(_T("Halil Kural"),_T("windows-2723210a07c4e90162b26966a8jcdboe")); 

	CString strTemp(_T(""));
	libxl::Sheet* pShtTem = pBook->addSheet(_T("Temperature"));
	if(NULL == pShtTem)
	{
		goto NEWSHEETERROR;
	}

	int iDataCount = pTemData->GetDataCount();
	int iLidChannelCount = pTemData->GetLidChannelCount();
	int iChannelCount = pTemData->GetChannelCount();
	int iRow = 0;
	int iCol = 0;
	pShtTem->writeStr(iRow, iCol++, _T("Time"));
	if (iLidChannelCount == 1)
	{
		pShtTem->writeStr(iRow, iCol++, _T("Lid Temperature"));
	}
	else
	{
		for (int i=0; i<iLidChannelCount; i++)
		{
			strTemp.Format(_T("Lid Channel(%d)"), i+1);
			pShtTem->writeStr(iRow, iCol++, strTemp);
		}
	}

	for (int i=0; i<iChannelCount; i++)
	{
		strTemp.Format(_T("Channel(%d)"), i+1);
		pShtTem->writeStr(iRow, iCol++, strTemp);
	}

	iRow = 1;
	for (int i=0; i<iDataCount; i++)
	{
		iCol = 0;
		pShtTem->writeNum(iRow, iCol++, pTemData->GetOneTimeData(i));
		for (int j=0; j<iLidChannelCount+iChannelCount; j++)
		{
			pShtTem->writeNum(iRow, iCol++, pTemData->GetOneTempData(j, i));
		}
		iRow ++;
	}

	double dBaseColWidth = pShtTem->colWidth(0);
	pShtTem->setCol(0, iLidChannelCount+iChannelCount, dBaseColWidth*1.8);

	if(pBook->save(strFilePath)) 
	{
		::ShellExecute(NULL,_T("open"),strFilePath,NULL,NULL,SW_SHOW);
	}
	else
	{
		pBook->errorMessage();
	}

	pBook->release();

	return 0;

NEWSHEETERROR:
	pBook->release();
	return -3;
}


// �����µ����ݸ�ʽ��ʹ�ñ���ģ��
int CMiddleDlg::ExportPS96ResultSheet(libxl::Book* pBook)
{
	vector<BOOL> vecSiteSelectChanState;	//�洢ÿ��ͨ���Ƿ��п�λѡ�У���һ��ѡ��Ҳ����ѡ��
	m_pExpData->GetChannelIsSelectedByAtLeastOneTube(vecSiteSelectChanState);	//���п�λ��δѡ�е�ͨ��������ʾCt����

	m_pExpData->GetSampleIdSiteIndexRelation(theApp.m_ps96IvdSet.IsExportResultHorizontal());

	CString strId(_T(""));
	vector<int> vecSiteIndex;
	int iRelationSize = m_pExpData->m_samIdSiteRelation.GetRelationCount();
	int iRealChanNum = m_pExpData->GetChannelCount();
	CReportTemplateData* pReportData = CPublicInfo::GetInstance()->GetReportTemplateData();
	BOOL bNegativeIC = FALSE;

	for(int k = 0;k < iRelationSize;++k)
	{
		bNegativeIC = FALSE;
		strId.Empty();
		vecSiteIndex.clear();
		m_pExpData->m_samIdSiteRelation.GetRelationAt(k,strId,vecSiteIndex);

		if(strId.IsEmpty())
			continue;
		if(_T("PC") == strId || _T("NC") == strId)
			continue;

		libxl::Sheet* pSheetId = pBook->addSheet(strId);
		if(NULL == pSheetId)
		{
			goto NEWSHEETERROR;
		}

		ExportPS96ResultSheetFormat(pBook, pSheetId);

		// ��һ����
		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFirstPaneStart, m_iStartCol, pReportData->GetStringBy(CReportTemplateData::DATA_LAB_NAME), 15);// ��������
		pSheetId->setMerge(m_iFirstPaneStart, m_iFirstPaneStart, m_iStartCol, m_iStartCol+m_iColCount-1);
		
		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFirstPaneStart+1, m_iStartCol, pReportData->GetStringBy(CReportTemplateData::DATA_ADDRESS));// ������ַ
		pSheetId->setMerge(m_iFirstPaneStart+1, m_iFirstPaneStart+1, m_iStartCol, m_iStartCol+m_iColCount-1);

		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFirstPaneStart+2, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(507));// URL
		pSheetId->writeStr(m_iFirstPaneStart+2, m_iStartCol+2, pReportData->GetStringBy(CReportTemplateData::DATA_CONTACT_URL));

		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFirstPaneStart+3, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(508));// Email
		pSheetId->writeStr(m_iFirstPaneStart+3, m_iStartCol+2, pReportData->GetStringBy(CReportTemplateData::DATA_CONTACT_EMAIL));

		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFirstPaneStart+4, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(509));// Phone
		pSheetId->writeStr(m_iFirstPaneStart+4, m_iStartCol+2, pReportData->GetStringBy(CReportTemplateData::DATA_CONTACT_PHONE));

		vector<CString> vecIDGeneName;
		CString strCollectYMD;
		CString strTechnician;
		CString strReviewer;

		for(int i = 0;i < vecSiteIndex.size();++i)
		{
			CTubeInfo* pTubeInfo = m_pExpData->GetTubeInfoAt(vecSiteIndex[i]);
			if(NULL == pTubeInfo ||  pTubeInfo->GetBeNoneSampleType())
				continue;

			if(strId.IsEmpty() && pTubeInfo->GetBeUnknown())	//����������ΪUnknown������IDΪ�գ�����������
				continue;

			//��¼��������
			strCollectYMD = pTubeInfo->GetCollectYMD();
			strTechnician = pTubeInfo->GetProposer();
			strReviewer = pTubeInfo->GetApplyDate();

			// ��������д�벡����Ϣ
			if(0 == i) 
			{	
				ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iThirdPaneStart, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(66));// ��������
				ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iThirdPaneStart, m_iStartCol+2, pTubeInfo->GetPatientName());

				ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iThirdPaneStart+1, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(418));// ����ID
				ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iThirdPaneStart+1, m_iStartCol+2, pTubeInfo->GetPatientID());

				ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iThirdPaneStart+2, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(68));// ��������
				ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iThirdPaneStart+2, m_iStartCol+2,pTubeInfo->GetAgeString());

				ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iThirdPaneStart+2, m_iStartCol+5, theApp.m_pLangInfo->GetCapitalTextWithColon(67));// �����Ա�
				ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iThirdPaneStart+2, m_iStartCol+7,pTubeInfo->GetSexualityString());

				ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iThirdPaneStart+3, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(64));// ����ID
				ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iThirdPaneStart+3, m_iStartCol+2,pTubeInfo->GetSampleID());

				ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iThirdPaneStart+4, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(514));// Remarks
				ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iThirdPaneStart+4, m_iStartCol+2, pTubeInfo->GetComments());
			}

			for(int j = 0;j < iRealChanNum;++j)
			{
				if(j < vecSiteSelectChanState.size() && !vecSiteSelectChanState[j])
					continue;

				CDyeTargetInfo*  pDyeTargetInfo = pTubeInfo->GetTargetInfo(j);
				if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())	//ͨ���Ƿ�ѡ���ڱ����б��ж���ռλ(��ռλ)
					continue;

				BOOL bIcGene = pTubeInfo->IsInternalControl(pDyeTargetInfo->GetTargetName());	
				//��������Խ��
				if(pDyeTargetInfo->GetBePositiveAnaResult())
				{
					if (bIcGene)
					{
						bNegativeIC = TRUE;
					}
					else
					{
						CString strGeneName = pDyeTargetInfo->GetTargetName();
						if(!strGeneName.IsEmpty())
						{
							BOOL bFind = FALSE;
							for(int t = 0;t < vecIDGeneName.size();++t)
							{
								if(strGeneName == vecIDGeneName[t])
								{
									bFind = TRUE;
									break;
								}
							}
							if(!bFind)
							{
								vecIDGeneName.push_back(strGeneName);
							}
						}
					}
				}
			}
		}

		// ��2����д�����������ʱ��
		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iSecondPaneStart, m_iStartCol, theApp.m_pLangInfo->GetCapitalText(398));// ����ʱ��
		ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iSecondPaneStart, m_iStartCol+2, strCollectYMD);
		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iSecondPaneStart+1, m_iStartCol, theApp.m_pLangInfo->GetCapitalText(399)); // ����ʱ��
		ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iSecondPaneStart+1, m_iStartCol+2, m_pExpData->GetEndTimeYMDString());

		// ��4���� д����ܽ������������
		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFourthPaneStart, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(510));// �������
		ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iFourthPaneStart, m_iStartCol+2, pReportData->GetStringBy(CReportTemplateData::DATA_TEST_NAME));

		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFourthPaneStart+2, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(419));// ����
		CString strTestResult;
		strTestResult.Empty();
		for(int t = 0;t < vecIDGeneName.size();++t)
		{
			if(t > 0)
			{
				strTestResult += _T(",");
			}
			strTestResult += vecIDGeneName[t];
		}
		if (strTestResult.IsEmpty())
		{
			if (bNegativeIC)
				strTestResult = NEGATIVE_TEXT;
			else
				strTestResult = INVALID_TEXT;
		}

		pSheetId->setMerge(m_iFourthPaneStart+2, m_iFourthPaneStart+4, m_iStartCol+2, m_iStartCol+m_iColCount-1);
		libxl::Format* pCellFormat = pSheetId->cellFormat(m_iFourthPaneStart+2, m_iStartCol+2);
		libxl::Font* pFont = pBook->addFont(pCellFormat->font());
		libxl::Format* pFormatAuto = pBook->addFormat(pCellFormat);
		pFormatAuto->setWrap(TRUE);
		pFormatAuto->setAlignH(libxl::ALIGNH_LEFT);
		pFormatAuto->setAlignV(libxl::ALIGNV_TOP);
		pFont->setUnderline(libxl::UNDERLINE_SINGLE);
		pFormatAuto->setFont(pFont);
		pSheetId->writeStr(m_iFourthPaneStart+2, m_iStartCol+2, strTestResult, pFormatAuto); // ����

		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFourthPaneStart+5, m_iStartCol, theApp.m_pLangInfo->GetCapitalText(400));// ʵ��Ա
		ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iFourthPaneStart+5, m_iStartCol+2, strTechnician);

		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFourthPaneStart+5, m_iStartCol+5, theApp.m_pLangInfo->GetCapitalText(401));// ���Ա
		ExportPS96ResultWriteStrUnderline(pBook, pSheetId, m_iFourthPaneStart+5, m_iStartCol+7, strReviewer);

		ExportPS96ResultWriteStrBold(pBook, pSheetId, m_iFourthPaneStart+7, m_iStartCol, theApp.m_pLangInfo->GetCapitalTextWithColon(511));// ��������
		pSheetId->setMerge(m_iFourthPaneStart+8, m_iFourthPaneEnd, m_iStartCol, m_iStartCol+m_iColCount-1);
		pCellFormat = pSheetId->cellFormat(m_iFourthPaneStart+8, m_iStartCol);
		pFormatAuto = pBook->addFormat(pCellFormat);
		pFormatAuto->setWrap(TRUE);
		pFormatAuto->setAlignH(libxl::ALIGNH_LEFT);
		pFormatAuto->setAlignV(libxl::ALIGNV_TOP);
		pSheetId->writeStr(m_iFourthPaneStart+8, m_iStartCol, pReportData->GetStringBy(CReportTemplateData::DATA_DISCLAIMER), pFormatAuto); // ����
	}

	return 0;

NEWSHEETERROR:
	return -3;
}

int CMiddleDlg::ExprotPS96OldResultSheet(libxl::Book* pBook)
{
	if(NULL == pBook)
	{
		return -1;
	}

	if(NULL == m_pExpData)
	{
		return -2;
	}

	int iChanNum = m_pExpData->GetChannelCount();
	int iColCount = m_pExpData->GetColCount();
	int iAmpDataLen = m_pExpData->GetAmplifyDataNum();
	int iRow = 1,iCol = 1,iTempCol = 0,iTempCol2 = 0;

	CTubeInfo* pTubeInfo = NULL;
	CDyeTargetInfo* pDyeTargetInfo = NULL;

	double dCt = 0;
	CString strResult(_T("")),strPNRst(_T(""));
	CString strTemp;
	
	vector<CString> vecTitle;	//�洢ÿ��ͨ���ڽ�������еı�������
	vector<BOOL> vecSiteSelectChanState;	//�洢ÿ��ͨ���Ƿ��п�λѡ�У���һ��ѡ��Ҳ����ѡ��

	CString strId(_T("")),strTotalRst(_T("")),strGeneName(_T(""));
	vector<int> vecSiteIndex,vecIdRowNum;	//vecIdRowNum����ÿ������IDʵ�����������
	vector<BOOL> vecPositiveValue,vecNegtiveValue;
	vector<CString> vecGeneName,vecIDGeneName;
	int iRstIndex = 0,iIdRowNum = 0,iRelationSize = 0;
	BOOL bPositive = FALSE,bNegtive = FALSE,bFind = FALSE,bIC = FALSE;

	//�ϲ�����ʱʹ��
	int iMergeStartRow = 2;
	libxl::IFormatT<TCHAR>* pCellFormat = NULL;

	libxl::Sheet* pSheetResult = pBook->addSheet(_T("Result"));
	if(NULL == pSheetResult)
	{
		goto NEWSHEETERROR;
	}

	pSheetResult->writeStr(0,0,theApp.m_pLangInfo->GetText(285));
	pSheetResult->setCol(0,0,16);	//width�����ؿ���1:9

	iRow = 1;
	iCol = 0;	
	
	//�������
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(29));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(76));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(47));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(64));
	iCol = 4;
	iTempCol2 = 4;
	iTempCol = 5;

	pSheetResult->writeStr(iRow,++iCol,_T("Result"));

	int iRealChanNum = iChanNum;
	if(m_pExpData->GetBeUseTurbidityCalibration())
	{
		iRealChanNum -= 1;
	}

	m_pExpData->GetResultFileDyeTargetTitle(vecTitle);	//��ȡCtֵ�����еı���
	m_pExpData->GetChannelIsSelectedByAtLeastOneTube(vecSiteSelectChanState);	//���п�λ��δѡ�е�ͨ��������ʾCt����
	for(int i = 0;i < iRealChanNum;++i)
	{
		if(i < vecSiteSelectChanState.size() && !vecSiteSelectChanState[i])
		{
			continue;
		}
		if(i >= vecTitle.size())
		{
			pSheetResult->writeStr(iRow,++iCol,_T(""));
		}
		else
		{
			pSheetResult->writeStr(iRow,++iCol,vecTitle[i]);
		}
	}

	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(95));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(30));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(55));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(54));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(98));

	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(66));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(67));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(68));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(69));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(70));

	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(71));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(72));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(73));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(74));
	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(75));

	pSheetResult->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(20));

	++iRow;
	iCol = 0;

	pSheetResult->setCol(1,1,7);
	pSheetResult->setCol(3,5,13);
	pSheetResult->setCol(6,10,14);
	pSheetResult->setCol(11,11,22);	//��׼ƷŨ����
	pSheetResult->setCol(12,26,14);

	m_pExpData->GetSampleIdSiteIndexRelation(theApp.m_ps96IvdSet.IsExportResultHorizontal());
	iRelationSize = m_pExpData->m_samIdSiteRelation.GetRelationCount();

	BOOL bFailNC = FALSE;
	BOOL bFailPC  = FALSE;

	for(int k = 0;k < iRelationSize;++k)
	{
		strId.Empty();
		vecSiteIndex.clear();
		m_pExpData->m_samIdSiteRelation.GetRelationAt(k,strId,vecSiteIndex);

		int iRstRow = -1,iRstCol = -1;
		strTotalRst.Empty();
		vecIDGeneName.clear();
		vecPositiveValue.clear();
		vecNegtiveValue.clear();

		iIdRowNum = 0;
		BOOL bExistICGene = FALSE,bICPositve = FALSE,bExistPositiveExceptICGene = FALSE;
		for(int i = 0;i < vecSiteIndex.size();++i)
		{
			pTubeInfo = m_pExpData->GetTubeInfoAt(vecSiteIndex[i]);
			if(NULL == pTubeInfo ||  pTubeInfo->GetBeNoneSampleType())
			{
				continue;
			}

			if(strId.IsEmpty() && pTubeInfo->GetBeUnknown())	//����������ΪUnknown������IDΪ�գ�����������
			{
				continue;
			}

			++iIdRowNum;

			if(pTubeInfo->IsExistInternalControl(bICPositve))
			{
				bExistICGene = TRUE;
			}
			if(pTubeInfo->IsExistPositiveExceptICGene())
			{
				bExistPositiveExceptICGene = TRUE;
			}

			bPositive = pTubeInfo->GetBePositive();
			bNegtive = pTubeInfo->GetBeNegtive();

			pSheetResult->writeNum(iRow,++iCol,++iRstIndex);
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetTubeLabel(iColCount));
			pSheetResult->writeStr(iRow,++iCol,CMySamPalteDlg::GetSampleTypeAbbr(pTubeInfo->GetSampleType()));

			iTempCol2 = ++iCol;	//��¼����ID���ڵ���

			iTempCol = ++iCol;	//��¼�����Result�����ڵ���

			strResult.Empty();
			vecGeneName.clear();

			vecPositiveValue.push_back(FALSE);
			vecNegtiveValue.push_back(FALSE);

			if(bPositive)
			{
				if(pTubeInfo->IsAllSelectedChannelPositive())
				{
					strPNRst = PASS_TEXT;
					vecPositiveValue[vecPositiveValue.size() - 1] = TRUE;
				}
				else
				{
					strPNRst = FAIL_TEXT;
				}
			}
			else if(bNegtive) 
			{
				if(pTubeInfo->IsAllSelectedChannelNegative())
				{
					strPNRst = PASS_TEXT;
					vecNegtiveValue[vecNegtiveValue.size() - 1] = TRUE;
				}
				else
				{
					strPNRst = FAIL_TEXT;
				}
			}

			for(int j = 0;j < iRealChanNum;++j)
			{
				if(j < vecSiteSelectChanState.size() && !vecSiteSelectChanState[j])
				{
					continue;
				}

				pDyeTargetInfo = pTubeInfo->GetTargetInfo(j);
				if(NULL == pDyeTargetInfo || !pDyeTargetInfo->GetBeSelected())	//ͨ���Ƿ�ѡ���ڱ����б��ж���ռλ(��ռλ)
				{
					dCt = 0;
				}
				else
				{
					dCt = pDyeTargetInfo->GetCt();
					if(dCt <= 1 || dCt >= iAmpDataLen)
					{
						dCt = 0;
					}

					if(pDyeTargetInfo->GetBePositiveAnaResult())
					{
						strGeneName = pDyeTargetInfo->GetTargetName();
						if(!strGeneName.IsEmpty())
						{
							bIC = CTubeInfo::IsInternalControl(strGeneName);
#ifdef PS96_IGNORE_IC
							bIC = FALSE;
#endif

							bFind = FALSE;
							for(int t = 0;t < vecGeneName.size();++t)
							{
								if(strGeneName == vecGeneName[t])
								{
									bFind = TRUE;
									break;
								}
							}
							if(!bFind && !bIC)	//��IC������������ʾ��Result��
							{
								vecGeneName.push_back(strGeneName);
							}

							bFind = FALSE;
							for(int t = 0;t < vecIDGeneName.size();++t)
							{
								if(strGeneName == vecIDGeneName[t])
								{
									bFind = TRUE;
									break;
								}
							}
							if(!bFind && !bIC)
							{
								vecIDGeneName.push_back(strGeneName);
							}
						}
					}
					else
					{
						//����ͨ���Ƿ���Ҫ��ʾCtֵ
					}
				}

				if(dCt > 0 && !strId.IsEmpty())	//δ��������ID����ͨ���������Ctֵ���ж����
				{
					strTemp.Format(_T("%.2f"), dCt);
					pSheetResult->writeStr(iRow, ++iCol, strTemp);
					// pSheetResult->writeNum(iRow,++iCol,dCt);
				}
				else
				{
					++iCol;
				}
			}

			for(int t = 0;t < vecGeneName.size();++t)
			{
				if(t > 0)
				{
					strResult += _T(";");
				}
				strResult += vecGeneName[t];
			}

#ifndef PC_NC_RESULT_MERGE
			if(_T("PC") == strId || _T("NC") == strId)	//δ��������ID����ͨ���������Ctֵ���ж����
			{
				if(bPositive || bNegtive)
				{
					strResult = strPNRst;
					pSheetResult->writeStr(iRow,iTempCol,strResult);	//NC��PC��λ�ж����
				}
			}
			else
#endif
			{
				if(0 == i)	//��¼��ͬ����ID(ID�ǿ�)�������������±�
				{
					iRstRow = iRow;
					iRstCol = iTempCol;
				}
			}

			pSheetResult->writeStr(iRow,++iCol,_T(""));	//��׼ƷŨ��
			pSheetResult->writeStr(iRow,++iCol,m_pExpData->GetProjectName());	//��Ŀ����

			pSheetResult->writeStr(iRow,++iCol,_T(""));	//����Ũ��
			pSheetResult->writeStr(iRow,++iCol,_T(""));
			pSheetResult->writeStr(iRow,++iCol,_T(""));

			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetPatientName());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetSexualityString());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetAgeString());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetCollectDate());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetProposer());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetDepartment());

			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetCaseID());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetBedNo());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetHospital());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetClinicID());
			pSheetResult->writeStr(iRow,++iCol,pTubeInfo->GetComments());

			++iRow;
			iCol = 0;
		}

		vecIdRowNum.push_back(iIdRowNum);

		for(int t = 0;t < vecIDGeneName.size();++t)
		{
			if(t > 0)
			{
				strTotalRst += _T(";");
			}
			strTotalRst += vecIDGeneName[t];
		}

#ifndef PS96_IGNORE_IC
		if (!bExistPositiveExceptICGene)
		{
			if (bExistICGene)
			{
				if (bICPositve)
					strTotalRst = NEGATIVE_TEXT;
				else
					strTotalRst = INVALID_TEXT;
			}
			else
			{
				strTotalRst = NEGATIVE_TEXT;
			}
		}
#endif

		//����ID�ǿգ�����ж����
#ifndef PC_NC_RESULT_MERGE
		if(!strId.IsEmpty() && _T("PC") != strId && _T("NC") != strId)
		{
			pSheetResult->writeStr(iRstRow,iRstCol,strTotalRst);
			pSheetResult->writeStr(iRstRow,iTempCol2,strId);
		}
#else
		if(!strId.IsEmpty())
		{
			if(_T("PC") == strId)
			{
				strTotalRst = PASS_TEXT;
				for(int i = 0;i < vecPositiveValue.size();++i)
				{
					if(!vecPositiveValue[i])
					{
						strTotalRst = FAIL_TEXT;
						bFailPC = TRUE;
						break;
					}
				}
			}
			else if(_T("NC") == strId)
			{
				strTotalRst = PASS_TEXT;
				for(int i = 0;i < vecNegtiveValue.size();++i)
				{
					if(!vecNegtiveValue[i])
					{
						strTotalRst = FAIL_TEXT;
						bFailNC = TRUE;
						break;
					}
				}
			}
			else
			{
				//if (bFailNC || bFailPC)
				//{
				//	strTotalRst = INVALID_TEXT;
				//}
			}

			pSheetResult->writeStr(iRstRow,iTempCol2,strId);
			pSheetResult->writeStr(iRstRow,iRstCol,strTotalRst);
		}
#endif
	}

	//�ϲ�����ID�к�Result��
	for(int k = 0;k < iRelationSize;++k)
	{
		strId.Empty();
		vecSiteIndex.clear();
		m_pExpData->m_samIdSiteRelation.GetRelationAt(k,strId,vecSiteIndex);

#ifndef PC_NC_RESULT_MERGE
		if(vecSiteIndex.size() > 1 && !strId.IsEmpty() && _T("PC") != strId && _T("NC") != strId)
#else
		if(vecSiteIndex.size() > 1 && !strId.IsEmpty())
#endif
		{
			pSheetResult->setMerge(iMergeStartRow,iMergeStartRow + vecSiteIndex.size() - 1,iTempCol2,iTempCol2);

			pCellFormat = pSheetResult->cellFormat(iMergeStartRow,iTempCol2);
			if(NULL != pCellFormat)
			{
				pCellFormat->setAlignV(libxl::ALIGNV_TOP);
				pCellFormat->setWrap();
				pSheetResult->setCellFormat(iMergeStartRow,iTempCol2,pCellFormat);
			}

			pSheetResult->setMerge(iMergeStartRow,iMergeStartRow + vecSiteIndex.size() - 1,iTempCol,iTempCol);
			pCellFormat = pSheetResult->cellFormat(iMergeStartRow,iTempCol);
			if(NULL != pCellFormat)
			{
				pCellFormat->setAlignV(libxl::ALIGNV_TOP);
				pCellFormat->setWrap();
				pSheetResult->setCellFormat(iMergeStartRow,iTempCol,pCellFormat);
			}
		}

		iMergeStartRow += vecIdRowNum[k];
	}

	return 0;

NEWSHEETERROR:
	return -3;
}

int CMiddleDlg::ExportResultSheet(libxl::Book* pBook)
{
	if(NULL == pBook)
	{
		return -1;
	}

	if(NULL == m_pExpData)
	{
		return -2;
	}

	CString strTemp(_T(""));

	CMyLstCtrl& lstRst = m_dataAnaDlg.m_lstAnaResult;
	int iRowNum = lstRst.GetRowNum();
	int iColNum = lstRst.GetColNum();

	libxl::Sheet* pSheetResult = pBook->addSheet(_T("Result"));
	if(NULL == pSheetResult)
	{
		goto NEWSHEETERROR;
	}

#ifdef CAN_YOUNG
	pSheetResult->writeStr(0,0,theApp.m_pLangInfo->GetText(285, LANGUAGE_ENG));
	// �������ݷ�������
	if (m_dataAnaDlg.GetCtDataType() == 0)
	{
		strTemp = theApp.m_pLangInfo->GetText(458);
	}
	else
	{
		strTemp = theApp.m_pLangInfo->GetText(459);
	}
	pSheetResult->writeStr(0,1,strTemp);

#else
	pSheetResult->writeStr(0,0,theApp.m_pLangInfo->GetText(285));
#endif

	int iRow = 1,iCol = 0;	
	for(int i=0; i<iColNum; ++i)
	{
#ifdef CAN_YOUNG
		pSheetResult->writeStr(iRow,++iCol, theApp.m_pLangInfo->ConvertTo(lstRst.GetColumnText(i), LANGUAGE_ENG));
#else
		pSheetResult->writeStr(iRow,++iCol, lstRst.GetColumnText(i));
#endif
	}

	pSheetResult->setCol(0,0,16);	//width�����ؿ���1:9
#ifdef CAN_YOUNG
	pSheetResult->setCol(1,1,13);
	pSheetResult->setCol(2,2,7);
#else
	pSheetResult->setCol(1,2,7);
#endif
	pSheetResult->setCol(3,10,13);
	pSheetResult->setCol(11,11,24);	//��׼ƷŨ����
	pSheetResult->setCol(12,26,14);	

	++iRow;
	for(int j = 0;j < iRowNum;++j)
	{
		iCol = 0;
		for(int i = 0;i < iColNum;++i)
		{
			strTemp = lstRst.GetItemText(j,i);
			if(strTemp.IsEmpty())
			{
				pSheetResult->writeStr(iRow,++iCol,strTemp);
				continue;
			}

			if(0 == i || 17 == i)	//��š�����
			{
				pSheetResult->writeNum(iRow,++iCol,_ttoi(strTemp));
			}
			else if(i >= 7 && i <= 10)	//Ũ�ȡ�ƽ��Ct��Tm1��Rm1
			{
				pSheetResult->writeNum(iRow,++iCol,_ttof(strTemp));
			}
			else if(CT_COLUMN_INDEX == i)	//Ctֵ
			{
				if(STR_NOCT == strTemp)
				{
					pSheetResult->writeStr(iRow,++iCol,strTemp);
				}
				else
				{
					pSheetResult->writeNum(iRow,++iCol,_ttof(strTemp));
				}
			}
			else
			{
				pSheetResult->writeStr(iRow,++iCol,strTemp);
			}
		}
		++iRow;
	}

	return 0;

NEWSHEETERROR:
	return -3;
}

int CMiddleDlg::ExportInstrumentInfoSheet(libxl::Book* pBook)
{
	if(NULL == pBook)
	{
		return -1;
	}

	if(NULL == m_pExpData)
	{
		return -2;
	}

	int iStartRow = 0,iRow = iStartRow,iStartCol = 0,iCol = iStartCol;
	CHardwareInfo hardwareInfo = m_pExpData->GetHardwareInfo();
	CString strTemp(_T(""));

	//����������ʵ����Ϣ
	libxl::Sheet* pShtInstru = pBook->addSheet(_T("Instrument"));
	if(NULL == pShtInstru)
	{
		goto NEWSHEETERROR;
	}

	pShtInstru->writeStr(0,0,theApp.m_pLangInfo->GetText(22));

	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(23));
	pShtInstru->writeStr(iRow,++iCol,hardwareInfo.m_strModel);
	iCol = 0;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(281));
	pShtInstru->writeStr(iRow,++iCol,hardwareInfo.m_strSN);
	iCol = 0;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(284));
	pShtInstru->writeStr(iRow,++iCol,hardwareInfo.m_strCode);
	iCol = 0;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(282));
	pShtInstru->writeStr(iRow,++iCol,hardwareInfo.m_strVersion);
	iCol = 0;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(283));
	pShtInstru->writeStr(iRow,++iCol,hardwareInfo.m_strSoftwareVer);

	iRow = 0;
	iCol = 3;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(7));
	pShtInstru->writeStr(iRow,++iCol,m_pExpData->GetExperName());
	iCol = 3;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(20));
	pShtInstru->writeStr(iRow,++iCol,m_pExpData->GetExperimentInfo()->GetComments());
	iCol = 3;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(286));
	pShtInstru->writeStr(iRow,++iCol,m_pExpData->GetStartTimeString());
	iCol = 3;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(287));
	pShtInstru->writeStr(iRow,++iCol,m_pExpData->GetEndTimeString());
	iCol = 3;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(288));
	pShtInstru->writeStr(iRow,++iCol,m_pExpData->GetTotalTimeString());
	iCol = 3;
	pShtInstru->writeStr(++iRow,++iCol,theApp.m_pLangInfo->GetText(481));
	strTemp = m_pExpData->GetAbnormalLog();
	if (m_pExpData->GetConnInterruptCount()> 0)
	{
		if (!strTemp.IsEmpty())
			strTemp += _T(";");
		strTemp += theApp.m_pLangInfo->GetText(528);
	}
	pShtInstru->writeStr(iRow,++iCol,strTemp);

	//���ͨ������ֵ����Ϣ
	iRow += 4;
	iCol = iStartCol;
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(51));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(54));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(52));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(423));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(424));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(119));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(118));
	
	double dData1 = 0,dData2 = 0;
	int iChanNum = m_pExpData->GetChannelCount();
	DWORD clrChan = 0;
	tagAnaAmplifyPara* pAnaAmpParam = NULL;
	tagScanChannelInfo* pScanChanInfo = NULL;

	for(int i = 0;i < iChanNum;++i)
	{
		if (m_pExpData->GetOneChannelDyeName(i) == _T("MOT"))
			continue;

		++iRow;
		iCol = iStartCol;
		pShtInstru->writeNum(iRow,++iCol,i + 1);

		clrChan = m_pExpData->GetChannelColor(i);
		strTemp.Format(_T("(%d,%d,%d)"),clrChan & 0XFF,(clrChan >> 8) & 0XFF,(clrChan >> 16) & 0XFF);
		pShtInstru->writeStr(iRow,++iCol,strTemp);

		pShtInstru->writeStr(iRow,++iCol,m_pExpData->GetOneChannelDyeName(i));
		
		pScanChanInfo = hardwareInfo.m_lnkScanChanInfo[i];
		if(NULL != pScanChanInfo)
		{
			pShtInstru->writeNum(iRow,++iCol,pScanChanInfo->iExWaveLen);
			pShtInstru->writeNum(iRow,++iCol,pScanChanInfo->iEmWaveLen);
		}
		else
		{
			++iCol;
			++iCol;
		}
		
		pAnaAmpParam = m_pExpData->GetAmplifyParaBy(i);
		if(NULL != pAnaAmpParam)
		{
			dData1 = pAnaAmpParam->dCalcDeltaRnThreshold;
			dData2 = pAnaAmpParam->dCalcNormalizeThreshold;

			pShtInstru->writeNum(iRow,++iCol,dData1);
			pShtInstru->writeNum(iRow,++iCol,dData2);
		}
	}

	//���������Ϣ
	iRow += 3;
	iCol = iStartCol;
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(131));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(184));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(425));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(426));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(427));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(428));
	pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(429));

	CLinkList<CSegItemInfo>& lnkSeg = m_pExpData->GetSegment();

	BOOL bCltAll = FALSE;
	int iSegNum = lnkSeg.GetSize(),iStepNum = 0,iIndex = 0,iNextStepTag = 0,iCycleNum = 0,iYesNoLangId = 0;
	CSegItemInfo* pSegItem = NULL;
	CStepItemInfo* pStepItem = NULL;
	for(int i = 0;i < iSegNum;++i)
	{
		pSegItem = lnkSeg[i];
		if(NULL == pSegItem)
		{
			continue;
		}

		iStepNum = pSegItem->GetStepNum();
		for(int j = 0;j < iStepNum;++j)
		{
			pStepItem = pSegItem->m_lnkStepItemInfo[j];
			if(NULL == pStepItem)
			{
				continue;
			}

			++iRow;
			iCol = iStartCol;

			pShtInstru->writeNum(iRow,++iCol,++iIndex);
			pShtInstru->writeNum(iRow,++iCol,pStepItem->m_fAimTem);
			pShtInstru->writeNum(iRow,++iCol,pStepItem->m_iDelay);

			bCltAll = pStepItem->GetBeCollectAll();
			if(bCltAll)
			{
				iNextStepTag = iIndex;
			}
			else
			{
				if(j == iStepNum - 1 && iStepNum > 1)
				{
					iNextStepTag = iIndex - iStepNum + 1;
				}
				else
				{
					iNextStepTag = 0;
				}
			}
			pShtInstru->writeNum(iRow,++iCol,iNextStepTag);
			
			iCycleNum = pSegItem->m_iCycleNum;
			if(bCltAll)
			{
				float fPreStepTem = m_pExpData->GetPreStepTemperature(i,j);
				if(0 != pStepItem->m_fIncrement && pStepItem->m_fAimTem > fPreStepTem)
				{
					iCycleNum = (pStepItem->m_fAimTem - fPreStepTem) / pStepItem->m_fIncrement;
				}
			}
			else
			{
				if(j < iStepNum - 1)
				{
					iCycleNum = 1;
				}
			}	
			pShtInstru->writeNum(iRow,++iCol,iCycleNum);

			if(pStepItem->GetBeNoneCollect())
			{
				iYesNoLangId = 152;
			}
			else
			{
				iYesNoLangId = 151;
			}
			pShtInstru->writeStr(iRow,++iCol,theApp.m_pLangInfo->GetText(iYesNoLangId));

			if(bCltAll)
			{
				pShtInstru->writeNum(iRow,++iCol,pStepItem->m_fIncrement);
			}
		}
	}

	pShtInstru->setCol(0,0,20);	//width�����ؿ���1:9
	pShtInstru->setCol(1,1,24);
	pShtInstru->setCol(2,2,29);
	pShtInstru->setCol(4,4,24);
	pShtInstru->setCol(5,5,29);
	pShtInstru->setCol(6,7,24);

	return 0;

NEWSHEETERROR:
	return -3;
}

int CMiddleDlg::ExportTubeInfoSheet(libxl::Book* pBook,ESAMINFOEXPORTMODE eSamExportMode)
{
	if(NULL == pBook)
	{
		return -1;
	}

	if(NULL == m_pExpData)
	{
		return -2;
	}

	int iRowCount = m_pExpData->GetRowCount();
	int iColCount = m_pExpData->GetColCount();
	int iRowSamSet = 1,iColSamSet = 1,iSamSetIndex = 0,iTubeIndex = 0,iMEnd = 0,iTEnd = 0,iIndex1 = 0,iIndex2 = 0;

	CString strTemp(_T(""));
	CTubeInfo* pTubeInfo = NULL;

	BOOL bHori = (ESEM_HORI == eSamExportMode);

	libxl::Sheet* pSheetTubeInfo = pBook->addSheet(_T("Sample set"));
	if(NULL == pSheetTubeInfo)
	{
		goto NEWSHEETERROR;
	}

	pSheetTubeInfo->writeStr(0,0,_T("Sample Set"));
	pSheetTubeInfo->setCol(0,0,16);	//width�����ؿ���1:9

	iColSamSet = 0;

#ifdef CAN_YOUNG
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(29, LANGUAGE_ENG)); // ���
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(76, LANGUAGE_ENG)); // ��Ӧ��
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(47, LANGUAGE_ENG)); // ��������
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(48, LANGUAGE_ENG)); // ��������
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(64, LANGUAGE_ENG)); // ����ID
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(20, LANGUAGE_ENG)); // ��ע
#else
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(29)); // ���
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(76)); // ��Ӧ��
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(47)); // ��������
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(48)); // ��������
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(64)); // ����ID
	pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,theApp.m_pLangInfo->GetText(20)); // ��ע
#endif
	pSheetTubeInfo->setCol(1,1,7);
	pSheetTubeInfo->setCol(3,5,13);
	//pSheetTubeInfo->setCol(6,6,14);
	//pSheetTubeInfo->setCol(9,9,14);
	//pSheetTubeInfo->setCol(10,16,16);
	pSheetTubeInfo->setCol(6,6,16);

	++iRowSamSet;
	if(bHori)
	{
		iMEnd = iRowCount;
		iTEnd = iColCount;
	}
	else
	{
		iMEnd = iColCount;
		iTEnd = iRowCount;
	}

	for(int m = 0;m < iMEnd;++m)
	{
		for(int t = 0;t < iTEnd;++t)
		{
			if(bHori)
			{
				iTubeIndex = m * iColCount + t;
				iIndex1 = m;
				iIndex2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColCount + m;
				iIndex1 = t;
				iIndex2 = m + 1;
			}

			pTubeInfo = m_pExpData->GetTubeInfoAt(iTubeIndex);
			if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType())
			{
				continue;
			}

			iColSamSet = 0;
			pSheetTubeInfo->writeNum(iRowSamSet,++iColSamSet,++iSamSetIndex);
			pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,pTubeInfo->GetTubeLabel(iColCount));
			pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,CMySamPalteDlg::SampleTypeToText(pTubeInfo->GetSampleType()));
			pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,pTubeInfo->GetSampleName());
			pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,pTubeInfo->GetSampleID());
			pSheetTubeInfo->writeStr(iRowSamSet,++iColSamSet,pTubeInfo->GetComments());
			++iRowSamSet;
		}
	}

	return 0;

NEWSHEETERROR:
	return -3;
}

int CMiddleDlg::ExportAmlDataSheet(libxl::Book* pBook,ESAMINFOEXPORTMODE eSamExportMode)
{
	if(NULL == pBook)
	{
		return -1;
	}

	if(NULL == m_pExpData)
	{
		return -2;
	}

	int iChanNum = m_pExpData->GetChannelCount();
	int iRowCount = m_pExpData->GetRowCount();
	int iColCount = m_pExpData->GetColCount();
	int iAmpDataLen = m_pExpData->GetAmplifyDataNum();

	int iRow = 1,iCol = 1,iTubeIndex = 0,iMEnd = 0,iTEnd = 0,iIndex1 = 0,iIndex2 = 0,iChanDataRowInterval = 2;	//iChanDataRowInterval--ͨ���������һ��ͨ�����һ�������м������
	
	CString strTemp(_T(""));
	CTubeInfo* pTubeInfo = NULL;

	BOOL bHori = (ESEM_HORI == eSamExportMode);
	BOOL bDeveloper = theApp.m_curUserInfo.GetName() == DEFAULT_DEVELOPER;

	libxl::Sheet* pSheetOrigin = pBook->addSheet(_T("Origin"));
	if(NULL == pSheetOrigin)
	{
		goto NEWSHEETERROR;
	}

	libxl::Sheet* pSheetCrosstalk = NULL;
	libxl::Sheet* pSheetFilteringMedian = NULL;
	libxl::Sheet* pSheetBaselineAdjust = NULL;
	libxl::Sheet* pSheetTurbidity = NULL;
	libxl::Sheet* pSheetFiltering = NULL;
	if (bDeveloper)
	{
		pSheetCrosstalk = pBook->addSheet(_T("Crosstalk"));
		if(NULL == pSheetOrigin)
		{
			goto NEWSHEETERROR;
		}

		pSheetFilteringMedian = pBook->addSheet(_T("Filtering Median"));
		if(NULL == pSheetOrigin)
		{
			goto NEWSHEETERROR;
		}

		pSheetBaselineAdjust = pBook->addSheet(_T("Baseline Adjust"));
		if(NULL == pSheetOrigin)
		{
			goto NEWSHEETERROR;
		}

		pSheetTurbidity = pBook->addSheet(_T("Turbidity"));
		if(NULL == pSheetOrigin)
		{
			goto NEWSHEETERROR;
		}

		pSheetFiltering = pBook->addSheet(_T("Filtering"));
		if(NULL == pSheetOrigin)
		{
			goto NEWSHEETERROR;
		}
	}

	libxl::Sheet* pSheetNormalize = pBook->addSheet(_T("Normalize"));
	if(NULL == pSheetNormalize)
	{
		goto NEWSHEETERROR;
	}
	libxl::Sheet* pSheetDeltaRn = pBook->addSheet(_T("DeltaRn"));
	if(NULL == pSheetDeltaRn)
	{
		goto NEWSHEETERROR;
	}

	pSheetOrigin->writeStr(0,0,_T("QPCR Fluoresence"));
	pSheetOrigin->setCol(0,0,16);	//width�����ؿ���1:9

	pSheetNormalize->writeStr(0,0,_T("QPCR Fluoresence"));
	pSheetNormalize->setCol(0,0,16);	//width�����ؿ���1:9

	pSheetDeltaRn->writeStr(0,0,_T("QPCR Fluoresence"));
	pSheetDeltaRn->setCol(0,0,16);	//width�����ؿ���1:9

	iRow = 1;
	for(int k = 0;k < iChanNum;++k)
	{
		//���ѭ�����ȱ�����Ϣ
		iCol = 0;

		strTemp = m_pExpData->GetOneChannelDyeName(k);
		if(strTemp.IsEmpty())
		{
			strTemp.Format(_T("Channel %d"),k + 1);
		}

		pSheetOrigin->writeStr(iRow,iCol,strTemp);
		if (bDeveloper)
		{
			pSheetCrosstalk->writeStr(iRow,iCol,strTemp);
			pSheetFilteringMedian->writeStr(iRow,iCol,strTemp);
			pSheetBaselineAdjust->writeStr(iRow,iCol,strTemp);
			pSheetTurbidity->writeStr(iRow,iCol,strTemp);
			pSheetFiltering->writeStr(iRow,iCol,strTemp);
		}

		pSheetNormalize->writeStr(iRow,iCol,strTemp);
		pSheetDeltaRn->writeStr(iRow,iCol,strTemp);

		iCol += 2;
		pSheetOrigin->writeStr(iRow,iCol,_T(""));
		if (bDeveloper)
		{
			pSheetCrosstalk->writeStr(iRow,iCol,_T(""));
			pSheetFilteringMedian->writeStr(iRow,iCol,_T(""));
			pSheetBaselineAdjust->writeStr(iRow,iCol,_T(""));
			pSheetTurbidity->writeStr(iRow,iCol,_T(""));
			pSheetFiltering->writeStr(iRow,iCol,_T(""));
		}
		pSheetNormalize->writeStr(iRow,iCol,_T(""));
		pSheetDeltaRn->writeStr(iRow,iCol,_T(""));

		//���ѭ����
		iCol = 1;
		++iRow;
		pSheetOrigin->writeStr(iRow,iCol,_T("Cycle"));
		if (bDeveloper)
		{
			pSheetCrosstalk->writeStr(iRow,iCol,_T("Cycle"));
			pSheetFilteringMedian->writeStr(iRow,iCol,_T("Cycle"));
			pSheetBaselineAdjust->writeStr(iRow,iCol,_T("Cycle"));
			pSheetTurbidity->writeStr(iRow,iCol,_T("Cycle"));
			pSheetFiltering->writeStr(iRow,iCol,_T("Cycle"));
		}
		pSheetNormalize->writeStr(iRow,iCol,_T("Cycle"));
		pSheetDeltaRn->writeStr(iRow,iCol,_T("Cycle"));

		for(int j = 0;j < iAmpDataLen;++j)
		{
			pSheetOrigin->writeNum(++iRow,iCol,j + 1);
			if (bDeveloper)
			{
				pSheetCrosstalk->writeNum(iRow,iCol,j + 1);
				pSheetFilteringMedian->writeNum(iRow,iCol,j + 1);
				pSheetBaselineAdjust->writeNum(iRow,iCol,j + 1);
				pSheetTurbidity->writeNum(iRow,iCol,j + 1);
				pSheetFiltering->writeNum(iRow,iCol,j + 1);
			}
			pSheetNormalize->writeNum(iRow,iCol,j + 1);
			pSheetDeltaRn->writeNum(iRow,iCol,j + 1);
		}
		iCol += 2;
		iRow += iChanDataRowInterval;
	}

	if(bHori)
	{
		iMEnd = iRowCount;
		iTEnd = iColCount;
	}
	else
	{
		iMEnd = iColCount;
		iTEnd = iRowCount;
	}

	iCol = 3;
	for(int m = 0;m < iMEnd;++m)
	{
		for(int t = 0;t < iTEnd;++t)
		{
			if(bHori)
			{
				iTubeIndex = m * iColCount + t;
				iIndex1 = m;
				iIndex2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColCount + m;
				iIndex1 = t;
				iIndex2 = m + 1;
			}

			pTubeInfo = m_pExpData->GetTubeInfoAt(iTubeIndex);
			if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType())
			{
				continue;
			}

			iRow = 2;
			for(int k = 0;k < iChanNum;++k)
			{
				strTemp.Format(_T("Tube_%s"),pTubeInfo->GetTubeLabel(iColCount));
				pSheetOrigin->writeStr(iRow,iCol,strTemp);
				if (bDeveloper)
				{
					pSheetCrosstalk->writeStr(iRow,iCol,strTemp);
					pSheetFilteringMedian->writeStr(iRow,iCol,strTemp);
					pSheetBaselineAdjust->writeStr(iRow,iCol,strTemp);
					pSheetTurbidity->writeStr(iRow,iCol,strTemp);
					pSheetFiltering->writeStr(iRow,iCol,strTemp);
				}
				pSheetNormalize->writeStr(iRow,iCol,strTemp);
				pSheetDeltaRn->writeStr(iRow,iCol,strTemp);

				double dData = 0;
				int iData = 0;
				for(int j = 0;j < iAmpDataLen;++j)
				{
					if (bDeveloper)
					{
						dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_ORIGINAL,k,j);
						pSheetOrigin->writeNum(++iRow,iCol,(int)dData);

						dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_CROSSTALK,k,j);
						pSheetCrosstalk->writeNum(iRow,iCol, dData);
						dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_FILTER_MEDIAN,k,j);
						pSheetFilteringMedian->writeNum(iRow,iCol, dData);
						dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_BASELINE_ADJUST,k,j);
						pSheetBaselineAdjust->writeNum(iRow,iCol, dData);
						dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_TURBIDITY,k,j);
						pSheetTurbidity->writeNum(iRow,iCol, dData);
						dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_FILTER,k,j);
						pSheetFiltering->writeNum(iRow,iCol, dData);
					}
					else
					{
						dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_TURBIDITY,k,j);
						pSheetOrigin->writeNum(++iRow,iCol,(int)dData);
					}

					dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_NORMALIZATE,k,j);
					pSheetNormalize->writeNum(iRow,iCol,dData);

					dData = m_pExpData->GetAmplifyChannelCycleFluY(iTubeIndex + 1,FLU_DELTARN,k,j);
					pSheetDeltaRn->writeNum(iRow,iCol,(int)dData);
				}	
				iRow += (iChanDataRowInterval + 1);
			}
			++iCol;
		}
	}

	return 0;

NEWSHEETERROR:
	return -3;
}


int CMiddleDlg::Analyse()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	if(!m_pExpData->HasRunData())
	{
		return 1;
	}

	//ʵ�����й����в�����ʹ�ú��������
	if(CExperimentData::STATUS_USING == m_pExpData->GetStatus())
	{
		return 2;
	}

	SynchronizeExperimentSetOnReAnalyse();

	//���ҳ����ڷ���ҳ��֮���������ݣ��������ݱ���������
	m_dataAnaDlg.ReAnalysed(m_pExpData);
	m_runMonitorDlg.ReAnalysed(m_pExpData);

	return 0;
}

int CMiddleDlg::SetCrossParam()
{
	CCrossSetDlg crossSetDlg(m_pExpData);
	if(IDOK != crossSetDlg.DoModal())
	{
		return 1;
	}

	if(crossSetDlg.m_bChanged)
	{
		//����ϵ���ı䣬��������

		//���ݴ���
		m_dataAnaDlg.DataProcess(TRUE);

		//������������������
		if(0 != m_dataAnaDlg.SetCurveData(m_pExpData,TRUE))
		{
			
		}

		//�������
		m_dataAnaDlg.StdLineProcess(m_pExpData);

		//ˢ�½���б�
		m_dataAnaDlg.ResetResultLst();
	}

	return 0;
}

int CMiddleDlg::SetFilterParam()
{
	CFilterParamSetDlg filterSetDlg(m_pExpData);
	if(IDOK != filterSetDlg.DoModal())
	{
		return 1;
	}

	if(filterSetDlg.m_bChanged)
	{
		//�˲������ı䣬��������

		//���ݴ���
		m_dataAnaDlg.DataProcess(TRUE,3);

		//������������������
		if(0 != m_dataAnaDlg.SetCurveData(m_pExpData,TRUE))
		{

		}

		//�������
		m_dataAnaDlg.StdLineProcess(m_pExpData);
		
		//ˢ�½���б�
		m_dataAnaDlg.ResetResultLst();
	}

	return 0;
}

int CMiddleDlg::SetBaseSet()
{
	if(m_dataAnaDlg.IsWindowVisible())
	{
		m_dataAnaDlg.SetBaseSet();
	}

	return 0;
}

int CMiddleDlg::SetMotChannelAdjust()
{
	if(m_dataAnaDlg.IsWindowVisible())
	{
		m_dataAnaDlg.SetMotChannelAdjust();
	}

	return 0;
}

int CMiddleDlg::SetMeltAnalyseParam()
{
	CAnaMeltParamDlg meltAnaDlg(m_pExpData);
	if(IDOK != meltAnaDlg.DoModal())
	{
		return 1;
	}

	if(meltAnaDlg.m_bCrossParamChanged || meltAnaDlg.m_meltAnaParamChangeInfo.IsAnaParamChanged())
	{
		//�������ݴ���
		if(!m_pExpData->ProcessMeltFluData())
		{
			
		}

		//��������
		if(0 != m_dataAnaDlg.SetMeltCurveData(m_pExpData,TRUE))
		{

		}

		//ˢ�½���б�
		m_dataAnaDlg.ResetResultLst();
	}

	return 0;
}

void CMiddleDlg::SynchronizeExperimentSet()
{
	if(NULL == m_pExpData)
	{
		return;
	}

	//��������ʱ���»�ȡ ����������Ϣ��������Ϣ��������Ϣ
	CExperimentInfo* pExpInfo = m_pExpData->GetExperimentInfo();
	if(NULL != pExpInfo)
	{
		m_baseSetDlg.GetBaseInfo(*pExpInfo);
	}

	m_selPrjDlg.m_prjBaseInfoDlg.GetPrjBaseInfo(m_pExpData->GetReagentItem());
	m_pExpData->SetSegment(m_selPrjDlg.m_segDlg.m_lnkSegItemInfo);

	/*vector<CTubeInfo> vecTubeInfo;
	m_selPrjDlg.m_samPlateDlg.GetSamInfo(vecTubeInfo);
	m_pExpData->SetTubeInfo(vecTubeInfo);*/

	m_pExpData->SetTubeInfo(m_selPrjDlg.m_samPlateDlg.GetTubeInfoPtr(),m_selPrjDlg.m_samPlateDlg.m_iTotalSiteNum);
}

void CMiddleDlg::SynchronizeSampleSet()
{
	if(NULL == m_pExpData)
	{
		return;
	}

	/*vector<CTubeInfo> vecTubeInfo;
	m_selPrjDlg.m_samPlateDlg.GetSamInfo(vecTubeInfo);
	m_pExpData->SetTubeInfo(vecTubeInfo);*/
	
	m_pExpData->SetTubeInfo(m_selPrjDlg.m_samPlateDlg.GetTubeInfoPtr(),m_selPrjDlg.m_samPlateDlg.m_iTotalSiteNum);

	return;
}

void CMiddleDlg::SynchronizeExperimentSetOnReAnalyse()
{
	if(NULL == m_pExpData)
	{
		return;
	}

	//�ط���ʱ���»�ȡ ������Ϣ
	m_pExpData->SetTubeInfo(m_selPrjDlg.m_samPlateDlg.GetTubeInfoPtr(),m_selPrjDlg.m_samPlateDlg.m_iTotalSiteNum);
	return;
}

int CMiddleDlg::Run()
{
	m_lnkPeltiaTem.Clear();
	m_lnkLidTem.Clear();

	m_iCurCycle = -1;
	m_iCurSeg = -1;
	m_iCurStep = -1;

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

	if (pInstance->GetInstanceStatus() == INSTANCE_USING)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(475));
		return -3;
	}

	BOOL bPs96Instru = pInstance->GetBePS96PCR();
	BOOL bIvdUser = theApp.GetBeIVDUser();
	BOOL bPs96IvdMode = bPs96Instru && bIvdUser;

	//ʵ���������
	CString strExperName(_T("")),strExperFileSavePath(_T("")),strOldName(_T("")),strOldPath(_T(""));
	strExperName = m_baseSetDlg.m_expeInfoDlg.m_edExpeName.GetText();
	if(strExperName.IsEmpty())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(221));
		return -4;
	}

	strExperFileSavePath = m_baseSetDlg.m_expeInfoDlg.m_edSavePath.GetText();
	if(strExperFileSavePath.IsEmpty())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(222));
		return -5;
	}

	//��¼ʵ���ļ����е�ʵ�����ƺͱ���·���������жϼ���������ʵ���ļ��������ļ��Ƿ�����ͬ��·��
	strOldName = m_pExpData->GetExperimentInfo()->GetExpName();
	strOldPath = m_pExpData->GetExperimentInfo()->GetFilePath();
	BOOL bSamePath = (strOldName == strExperName && strOldPath == strExperFileSavePath);

	//�ж�ʵ�鱣��·���Ƿ����
	if(bPs96IvdMode)
	{
#ifndef IVD_SELFSELECT_SAVEPATH
		if(!PathIsDirectory(strExperFileSavePath))
		{
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(223));
			return -6;
		}
#endif
	}
	else
	{
		if(!PathIsDirectory(strExperFileSavePath))
		{
			PopWarningTipInfo(theApp.m_pLangInfo->GetText(223));
			return -6;
		}
	}

	if(m_selPrjDlg.m_segDlg.m_lnkSegItemInfo.GetSize() < 1)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(224));
		return -7;
	}

	//ͬ��ʵ����Ϣ
	m_baseSetDlg.GetBaseInfo(*pExpInfo);

	//��ȡ��Ŀ������Ϣ
	m_selPrjDlg.m_prjBaseInfoDlg.GetPrjBaseInfo(m_pExpData->GetReagentItem());

	//��ȡʵ��������Ϣ
	m_pExpData->SetSegment(m_selPrjDlg.m_segDlg.m_lnkSegItemInfo);

	//�ж�ʵ�������Ƿ�������ݲɼ����貢��ʾ
	if(!m_pExpData->CheckSegmentExistDataCollect())
	{
		if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(225)))
		{
			return -8;
		}
	}

	//�ж�����������Ƿ�������15�������ϵ�ѭ����,
	if(m_pExpData->IsExistAmpSegmentLessThanCycleNum(15))
	{
		if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(316)))
		{
			return -20;
		}
	}

	CTubeInfo* pTubeInfo = m_selPrjDlg.m_samPlateDlg.GetTubeInfoPtr();
	int iSiteNum = m_selPrjDlg.m_samPlateDlg.m_iTotalSiteNum;
	m_pExpData->SetTubeInfo(pTubeInfo,iSiteNum);

	if(m_pExpData->IsAllNoneSampleType())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(312));
		return -17;
	}

	// PS96 启动前不再强制要求 Sample ID（允许空 ID 开始实验）

	//��������ʵ���ļ�
	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
	m_pExpData->CreateBackupFilePath(pPublicInfo->GetStartupPath());

	//�����������ʾ�û����ݽ�ɾ��
	if(m_pExpData->HasRunData())
	{
		if(bSamePath)
		{
			if(bPs96IvdMode)
			{
#ifndef IVD_SELFSELECT_SAVEPATH
				if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(163)))
				{
					return -10;
				}
#endif
			}
			else
			{
				if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(163)))
				{
					return -10;
				}
			}			
		}
		m_pExpData->DeleteRunData();
	}

	if(bPs96Instru)
	{
		//PS96ʵ�鿪ʼǰ�Ƚ������¶Ⱥ͵�һ�������¶ȣ���������¶Ƚϸߣ�����������ʾ�����ܿ�ʼʵ��
		if (m_pExpData->GetExperimentInfo()->GetTempCtrlMode() != TEMP_CTRL_NONE)
		{
			float fCurTemp = 0.0f;
			BOOL bCurToTarget = FALSE;
			if(pInstance->ReadPCRTemp(fCurTemp, bCurToTarget))
			{
				float fFstStepTem = m_selPrjDlg.m_segDlg.m_lnkSegItemInfo.GetAt(0)->data.m_lnkStepItemInfo.GetAt(0)->data.m_fAimTem;
				if(fCurTemp - fFstStepTem > 1)
				{
					//PS96����������Ĭ�����õ��¶�Ϊ45��
					if(fFstStepTem < 45)
					{
						if(!pInstance->SetMultiTemp(fFstStepTem,fFstStepTem,fFstStepTem,4.0,TEMP_CTRL_NORMAL))
						{
							PopWarningTipInfo(theApp.m_pLangInfo->GetText(272));
							return -12;
						}
					}

					//PopWarningTipInfo(theApp.m_pLangInfo->GetText(270));
					//return -13;
				}
			}
			else
			{
				// pMainDlg->SetInstrumentConnectState(0);
				PopWarningTipInfo(theApp.m_pLangInfo->GetText(271));
				return -14;
			}
		}
	
		// lzh �����lid�ر�
		//�������Ƿ����
		/*int iLidState = 0;
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
	}

	CString strSaveFile = m_pExpData->GetExperimentInfo()->GetSaveDataFileName();
	if (theApp.HasExpSaveFileUsed(strSaveFile))
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(476));
		return -5;
	}
	if (theApp.HasExpSaveFileExist(strSaveFile))
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(477));
		return -5;
	}

	//����ʵ����ʱ��
	int iExpTotalSec = pInstance->CalculateExperimentTotalTimeSec(m_pExpData->GetSegment(),pExpInfo->GetLidHeader(),20);
	if(pInstance->GetBeN96PCR())	//96��QPCR���Ӻ���������Ķ�����ʱʱ��
	{
		iExpTotalSec += m_pExpData->GetTotalVolumeDelaySec(TRUE);
	}
	pExpInfo->SetTotalSec(iExpTotalSec);
	m_pExpData->SetScanStartPos(pInstance->GetScanStartPos());

	//ʵ��������У���ռ��ҳ������
	m_runMonitorDlg.ClearPage(TRUE);

	//���ü��ҳ��װ���Ϣ
	m_runMonitorDlg.m_samPlateDlg.SetTubeInfo(pTubeInfo,iSiteNum);

	//����������ɫ��Ϣ
	m_runMonitorDlg.SetCrvLineParam(pTubeInfo,iSiteNum);
	
	//���ü��ҳ��������Ϣ
	m_runMonitorDlg.m_segDlg.ResetCurItemSubCycle();
	m_runMonitorDlg.m_segDlg.SetSegmentInfo(m_selPrjDlg.m_segDlg.m_lnkSegItemInfo);
	m_runMonitorDlg.m_segDlg.SetSegmentFullSelectState(FALSE,TRUE);

	m_pExpData->m_tmStart = CTime::GetCurrentTime();
	m_pExpData->GetExperimentInfo()->SetStartTime(m_pExpData->m_tmStart); 
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stStartTm.SetWindowText(pExpInfo->GetStartTime());

	//��շ���ҳ������
	m_dataAnaDlg.ClearPage();

	//ʵ���������������ɣ����ʵ�����Ʊ���·���Ƿ�ı䣬������ʵ���ļ�����·��
	if(!bPs96IvdMode)
	{
		CWorkDlg* pParent = (CWorkDlg*)GetParent();
		if(NULL != pParent->GetSafeHwnd())
		{
			pParent->m_fileManageDlg.SetExperName(GetPageId(),pExpInfo->GetExpName());
			pParent->m_fileManageDlg.SetPageFilePath(GetPageId(),pExpInfo->GetSaveDataFileName());
		}
		m_pExpData->SetFilePath(pExpInfo->GetSaveDataFileName());
	}

	//��¼����ʵ�������Ӳ����Ϣ(�ļ�����·�����º�Ÿ���Ӳ����Ϣ)
	CHardwareInfo hardwareInfo;
	hardwareInfo.m_strModel = pInstance->GetModel();
	hardwareInfo.m_strSN = pInstance->GetSN();
	hardwareInfo.m_strCode = pInstance->GetCode();
	hardwareInfo.m_strVersion = pInstance->GetVersion();
	hardwareInfo.m_strSoftwareVer = pInstance->GetSoftwareVersion();
	pInstance->GetScanChannelInfo(hardwareInfo.m_lnkScanChanInfo);
	m_pExpData->SetHardwareInfo(hardwareInfo);

	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;

	//ʵ�鿪ʼ����ǰ����ȡ�������ȸ��¶ȣ����Ѿ��򿪵�ʵ���ļ��б�Ҫ�����������½���ʵ����������Ҳ�������
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
		// pMainDlg->SetInstrumentConnectState(0);
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(226));
		return -11;
	}

	//PS96IVDģʽ�£�ʵ�鿪ʼǰ�����û�ѡ�񹤳��ļ�����·������������Զ��ڸ�·���µ�����
SELECTPATH:
	if(bPs96IvdMode)
	{
		BOOL bFind = FALSE;
		CFileFind fileFind;
		CString strTemp(_T("")),strDefName(_T("")),strSerialSuffix(_T("")),strRstName(_T("")),strTitle(_T(""));
		strTemp.Format(_T("Experiemnt File(*%s)|*%s||"),PCR_DATA_FILE_SUFFIX_DOT,PCR_DATA_FILE_SUFFIX_DOT);

		//��ȡ���кź���λ
		strSerialSuffix = pInstance->GetSN();
		if(strSerialSuffix.GetLength() > 3)
		{
			strSerialSuffix = strSerialSuffix.Right(3);
		}
		strDefName = GetCurrentTmTime(3,_T("-"),_T("-"),_T("-")) + _T("_") + strSerialSuffix + _T("#");

		CFileDialog dlg(FALSE,PCR_DATA_FILE_SUFFIX_DOT,strDefName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strTemp,NULL);
		dlg.m_ofn.lpstrTitle = _T("Select file save path");

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
				//��ֹԭ��ͬ���ļ������ǣ��µ�ʵ���ļ�����������ں�׺,20220707��������
				strTitle += _T("-");
				strTitle += GetCurrentTmTimeW().c_str();
				strTitle += _T("_");
				strTitle += strSerialSuffix;
				strTitle += _T("#");
			}
		}
		else
		{
			//·����ͬ��ͬ�������ļ�����Ѿ����ڣ�Ҳ���µ�ʵ���ļ����ƺ����������ں�׺,20220707��������
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

		m_pExpData->SetIVDFileRstSaveName(strTitle);
		pExpInfo->SetExpName(strTitle);

		m_pExpData->SetFileOpenPath(_T(""));	//IVDģʽ������ʵ��ʱ��ʹ��ʵ��ǰ�û�ѡ����ļ�����·��

		//ʵ���������������ɣ����ʵ�����Ʊ���·���Ƿ�ı䣬������ʵ���ļ�����·��
		CWorkDlg* pParent = (CWorkDlg*)GetParent();
		if(NULL != pParent->GetSafeHwnd())
		{
			pParent->m_fileManageDlg.SetExperName(GetPageId(),strTitle);
			pParent->m_fileManageDlg.SetPageFilePath(GetPageId(),strTemp);
		}
		m_pExpData->SetFilePath(pExpInfo->GetSaveDataFileName());
	}

	m_pExpData->m_pInstance = pInstance;

	//���ʵ���ļ��������е��¶�ͨ������һ�£��������е�ͨ����Ϊ׼
	if(m_pExpData->GetTempChannelCount() != pInstance->GetTempChannelCount())
	{
		m_pExpData->SetTempChannelCount(pInstance->GetTempChannelCount());
	}

	//��¼ʵ���û���
	m_pExpData->SetUserName(theApp.m_curUserInfo.GetName());

	if(NULL == m_pRunCtrl)
	{
		m_pRunCtrl = new CExpRunCtrl(pInstance,this);
		m_pRunCtrl->SetLanguage(theApp.m_pLangInfo);
	}
	CExpRunPara* pRunParam = new CExpRunPara;
	m_pRunCtrl->Run(m_pExpData,pRunParam);

	//����ʵ�����ϵ籣��
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

int CMiddleDlg::RunPowerOffProtectExperiment()
{
	m_lnkPeltiaTem.Clear();
	m_lnkLidTem.Clear();

	m_iCurCycle = -1;
	m_iCurSeg = -1;
	m_iCurStep = -1;

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

	BOOL bPs96Instru = pInstance->GetBePS96PCR();
	BOOL bIvdUser = theApp.GetBeIVDUser();
	BOOL bPs96IvdMode = bPs96Instru && bIvdUser;

	if(m_selPrjDlg.m_segDlg.m_lnkSegItemInfo.GetSize() < 1)
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(224));
		return -7;
	}

	//�ж�ʵ�������Ƿ�������ݲɼ����貢��ʾ
	if(!m_pExpData->CheckSegmentExistDataCollect())
	{
		if(IDYES != PopQueryTipInfo(theApp.m_pLangInfo->GetText(225)))
		{
			return -8;
		}
	}

	//ʵ�鿪ʼ����ǰ����ȡ�������ȸ��¶ȣ����Ѿ��򿪵�ʵ���ļ��б�Ҫ�����������½���ʵ����������Ҳ�������
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
		// pMainDlg->SetInstrumentConnectState(0);
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(226));
		return -11;
	}

	//PS96ʵ�鿪ʼǰ�Ƚ������¶Ⱥ͵�һ�������¶ȣ���������¶Ƚϸߣ�����������ʾ�����ܿ�ʼʵ��
	if(bPs96Instru)
	{
		if (m_pExpData->GetExperimentInfo()->GetTempCtrlMode() != TEMP_CTRL_NONE)
		{
			float fCurTemp = 0.0f;
			BOOL bCurToTarget = FALSE;
			if(pInstance->ReadPCRTemp(fCurTemp, bCurToTarget))
			{
				float fFstStepTem = m_selPrjDlg.m_segDlg.m_lnkSegItemInfo.GetAt(0)->data.m_lnkStepItemInfo.GetAt(0)->data.m_fAimTem;
				if(fCurTemp - fFstStepTem > 1)
				{
					//PS96����������Ĭ�����õ��¶�Ϊ45��
					if(fFstStepTem < 45)
					{
						if(!pInstance->SetMultiTemp(fFstStepTem,fFstStepTem,fFstStepTem,4.0,TEMP_CTRL_NORMAL))
						{
							PopWarningTipInfo(theApp.m_pLangInfo->GetText(272));
							return -12;
						}
					}

					//PopWarningTipInfo(theApp.m_pLangInfo->GetText(270));
					//return -13;
				}
			}
			else
			{
				// pMainDlg->SetInstrumentConnectState(0);
				PopWarningTipInfo(theApp.m_pLangInfo->GetText(271));
				return -14;
			}
		}
	}

	//ʵ���������
	CString strExperName(_T("")),strExperFileSavePath(_T("")),strOldName(_T("")),strOldPath(_T(""));
	strExperName = m_baseSetDlg.m_expeInfoDlg.m_edExpeName.GetText();
	if(strExperName.IsEmpty())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(221));
		return -4;
	}

	strExperFileSavePath = m_baseSetDlg.m_expeInfoDlg.m_edSavePath.GetText();
	if(strExperFileSavePath.IsEmpty())
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(222));
		return -5;
	}

	//�ж�ʵ�鱣��·���Ƿ����
	if(!PathIsDirectory(strExperFileSavePath))
	{
		PopWarningTipInfo(theApp.m_pLangInfo->GetText(223));
		return -6;
	}

	//��������ʵ���ļ�
	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();
	m_pExpData->CreateBackupFilePath(pPublicInfo->GetStartupPath());

	//����ʵ����ʱ��
	int iExpTotalSec = pInstance->CalculateExperimentTotalTimeSec(m_pExpData->GetSegment(),pExpInfo->GetLidHeader(),20);
	if(pInstance->GetBeN96PCR())	//96��QPCR���Ӻ���������Ķ�����ʱʱ��
	{
		iExpTotalSec += m_pExpData->GetTotalVolumeDelaySec(TRUE);
	}
	pExpInfo->SetTotalSec(iExpTotalSec);

	//������ʱ���ȥ�ȸ�����ʱ��
	if(pInstance->HasLid() && pExpInfo->GetLidHeader())
	{
		pExpInfo->SetHasRunSec(pExpInfo->GetHasRunSec() - pInstance->GetLidHeatNormalSecond());
	}

	//���ü��ҳ��������Ϣ
	m_runMonitorDlg.m_segDlg.ResetCurItemSubCycle();
	m_runMonitorDlg.m_segDlg.SetSegmentInfo(m_selPrjDlg.m_segDlg.m_lnkSegItemInfo);
	m_runMonitorDlg.m_segDlg.SetSegmentFullSelectState(FALSE,TRUE);

	//m_pExpData->m_tmStart = CTime::GetCurrentTime();
	//pExpInfo->SetStartTime(m_pExpData->m_tmStart); 
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stStartTm.SetWindowText(pExpInfo->GetStartTime());
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stEndTm.SetWindowText(_T(""));

	//��շ���ҳ������
	m_dataAnaDlg.ClearPage();

	//ʵ���������������ɣ����ʵ�����Ʊ���·���Ƿ�ı䣬������ʵ���ļ�����·��
	CWorkDlg* pParent = (CWorkDlg*)GetParent();
	if(NULL != pParent->GetSafeHwnd())
	{
		pParent->m_fileManageDlg.SetExperName(GetPageId(),pExpInfo->GetExpName());
		pParent->m_fileManageDlg.SetPageFilePath(GetPageId(),pExpInfo->GetSaveDataFileName());
	}
	m_pExpData->SetFilePath(pExpInfo->GetSaveDataFileName());

	//��¼����ʵ�������Ӳ����Ϣ(�ļ�����·�����º�Ÿ���Ӳ����Ϣ)
	CHardwareInfo hardwareInfo;
	hardwareInfo.m_strModel = pInstance->GetModel();
	hardwareInfo.m_strSN = pInstance->GetSN();
	hardwareInfo.m_strCode = pInstance->GetCode();
	hardwareInfo.m_strVersion = pInstance->GetVersion();
	hardwareInfo.m_strSoftwareVer = pInstance->GetSoftwareVersion();	
	pInstance->GetScanChannelInfo(hardwareInfo.m_lnkScanChanInfo);
	m_pExpData->SetHardwareInfo(hardwareInfo);

	CGeneralPcrDlg* pMainDlg = (CGeneralPcrDlg*)theApp.m_pMainWnd;

	m_pExpData->SetIVDFileRstSaveFolder(pExpInfo->GetFilePath());

	//PS96���жϵ籣��ʵ��ʱ���������Ƽ��Ϻ�׺������ֹ������excel��ֻ��ģʽ���ļ�����ʧ�ܣ�20220825��������
#ifdef PS96INSTRUMENT
	m_pExpData->SetIVDFileRstSaveName(pExpInfo->GetExpName() + theApp.m_pLangInfo->GetText(348));
#else
	m_pExpData->SetIVDFileRstSaveName(pExpInfo->GetExpName());
#endif
	m_pExpData->SetFilePath(pExpInfo->GetSaveDataFileName());

	m_pExpData->m_pInstance = pInstance;

	//���ʵ���ļ��������е��¶�ͨ������һ�£��������е�ͨ����Ϊ׼
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

	//����ʵ�����ϵ籣��
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

int CMiddleDlg::Stop()
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
	m_btnRunStop.EnableWindow(FALSE);

	return 0;
}

int CMiddleDlg::RefreshExpDlgRunState(BOOL bOccupy)
{
	return 0;
}

int CMiddleDlg::SetInstrumentLidTem(int iLidtem)
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	if(iLidtem <= 0)
	{
		return -2;
	}

	CString strModel = m_pExpData->GetExperimentInfo()->GetInstrument();
	CString strSN = m_pExpData->GetExperimentInfo()->GetInstrumentSN();
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL == pInstrument)
	{
		return -2;
	}
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(strModel,strSN);
	if(NULL == pInstance) 
	{
		return -3;
	}

	if(!pInstance->WriteLidTemperaturePLCUsed(iLidtem))
	{
		return -4;
	}

	return 0;
}

int CMiddleDlg::ReadInstrumentLidTem(double& dLidtem)
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	CString strModel = m_pExpData->GetExperimentInfo()->GetInstrument();
	CString strSN = m_pExpData->GetExperimentInfo()->GetInstrumentSN();
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL == pInstrument)
	{
		return -2;
	}
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(strModel,strSN);
	if(NULL == pInstance) 
	{
		return -3;
	}

	if(pInstance->IsDemo())
	{
		return -4;
	}

	if(!pInstance->ReadLidTemperatureFromEEPROMPLCUsed(dLidtem))
	{
		return -5;
	}

	return 0;
}

int CMiddleDlg::SetInstrumentLidPreheatTem(int iLidtem)
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	if(iLidtem <= 0)
	{
		return -2;
	}

	CString strModel = m_pExpData->GetExperimentInfo()->GetInstrument();
	CString strSN = m_pExpData->GetExperimentInfo()->GetInstrumentSN();
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL == pInstrument)
	{
		return -2;
	}
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(strModel,strSN);
	if(NULL == pInstance) 
	{
		return -3;
	}

	if(!pInstance->WriteLidPreheatTemperaturePLCUsed(iLidtem))
	{
		return -4;
	}

	return 0;
}

int CMiddleDlg::ReadInstrumentLidPreheatTem(float& fLidtem)
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	CString strModel = m_pExpData->GetExperimentInfo()->GetInstrument();
	CString strSN = m_pExpData->GetExperimentInfo()->GetInstrumentSN();
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	if(NULL == pInstrument)
	{
		return -2;
	}
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(strModel,strSN);
	if(NULL == pInstance) 
	{
		return -3;
	}

	if(pInstance->IsDemo())
	{
		return -4;
	}

	if(!pInstance->ReadLidPreheatTemperaturePLCUsed(fLidtem))
	{
		return -5;
	}

	return 0;
}

void CMiddleDlg::InitExperimentData(CExperimentData* pExpData)
{
	CPublicInfo* pPublicInfo = CPublicInfo::GetInstance();

	if(pExpData->GetBeNewExperFile() || pExpData->GetBeTemplateFile())
	{ 
		// ���Խ���ʵ�������
		BOOL bIsDemoPcr = FALSE;
		CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
		CInstancePCR* pInstance = pInstrument->GetInstancePCR(pExpData->GetExperimentInfo()->GetInstrument(), pExpData->GetExperimentInfo()->GetInstrumentSN());
		if(pInstance == NULL)
		{
			pInstance = pInstrument->GetAvailableInstance();
		}
		if(pInstance == NULL)
		{
			pInstrument->CreateInstanceDemo(pPublicInfo->GetUserName());
			pInstance = pInstrument->GetInstancePCR();
			bIsDemoPcr = TRUE;
			m_bInstruConnected = FALSE;
		}
		else if (pInstance->IsDemo())
		{
			m_bInstruConnected = FALSE;
		}
		else
		{
			m_bInstruConnected = TRUE;
		}
		ASSERT(pInstance != NULL);

		//����Ĭ��ʵ�����ƣ����캯���ж����������ʱ���ʽ��,20220812
		CString strSerialSuffix = pInstance->GetSN();
		if(strSerialSuffix.GetLength() > 3)
		{
			strSerialSuffix = strSerialSuffix.Right(3);
		}
		CString strDefName = GetCurrentTmTime(3,_T("-"),_T("-"),_T("-")) + _T("_") + strSerialSuffix + _T("#");

		while (theApp.HasExpNameExist(strDefName))
		{
			::Sleep(100);
			strDefName = GetCurrentTmTime(3,_T("-"),_T("-"),_T("-")) + _T("_") + strSerialSuffix + _T("#");
		}
		pExpData->GetExperimentInfo()->SetExpName(strDefName);

		// ��ʵ�����ݶ�����г�ʼ��
		pExpData->GetExperimentInfo()->SetInstrument(pInstance->GetModel());
		pExpData->GetExperimentInfo()->SetInstrumentSN(pInstance->GetSN());
		pExpData->GetExperimentInfo()->SetLidTemp(pInstance->GetLidTem());

		if (pExpData->GetBeNewExperFile())
		{
			// ���ÿװ�
			tagPlateModeInfo sPlateInfo;
			pInstance->GetPlateInfo(&sPlateInfo);
			pExpData->GetPlateInfo()->SetPlateInfo(&sPlateInfo);

			// �¶Ⱥ�ӫ��ͨ��
			pExpData->SetTempChannelCount(pInstance->GetTempChannelCount());
			pExpData->SetChannelCount(pInstance->GetScanChannelCount());	//�������ʼ����ÿ����λ��ͨ����Ϣ
			pExpData->InitDyeChannelInfo(&theApp.m_listDyeInfo,pInstance->GetScanChannelInfoArray());	//��ʼ������֧�ֵ�����Ⱦ�����ƺͻ������ƣ������ÿ�λÿ��ͨ����Ĭ��Ⱦ�����ƺ�Ŀ������

			//���ÿ�λ��ͨ��Ĭ����ɫ
			pExpData->SetTubeDefaultTargetColor();

			// ��ʼ���������ϵ��
			pExpData->InitAmpCrosstalkParam(theApp.m_ppFCrosstalkPara,theApp.m_iDefChanNum);
			pExpData->InitMeltCrosstalkParam(theApp.m_ppFMeltCrosstalk,theApp.m_iDefChanNum);

			// ��ʼ���˲�����
			pExpData->InitFilterInfo(theApp.m_vecFlt);
			pExpData->InitAlgorithmPara(pPublicInfo->GetStartupPath());

			//�½���ʵ�飬����Ĭ��Ⱦ����
			pExpData->SetTubeDefaultDyeName();

			//�½��ļ�ʱ����¼ÿ�����߿ؼ���Ĭ��ֵ
			if(pExpData->GetDefaultAxisCount() < 1)
			{
				m_dataAnaDlg.RecordDefaultAxisInfo();
			}
		}

		if(bIsDemoPcr)
		{
			((CInstanceDemo*)pInstance)->InitDemoData(pExpData->GetPlateInfo()->GetTubeCount());
		}
		
		// ������ز���
		pExpData->SetScanStartPos(pInstance->GetScanStartPos());
	}

	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(pExpData->GetExperimentInfo()->GetInstrument(), pExpData->GetExperimentInfo()->GetInstrumentSN());
	if(NULL != pInstance)
	{
		// У׼ͨ��
		eCalibrateMethod nCaliMode = pInstance->GetCalibrateMethod();
		pExpData->SetCalibrateMethod(nCaliMode);
		if (nCaliMode != CALIBRATE_NONE)
		{
			pExpData->SetCalibrateChannel(pInstance->GetCalibrateChannel());

			//������ģʽ��У׼ͨ��Ĭ��ѡ�У�У׼ͨ����Ϣ���ɱ༭��
			BOOL bShowCalibChanData = TRUE;
			if(theApp.GetBeIVDUser())
			{
				bShowCalibChanData = FALSE;
			}
			pExpData->SetTubeChannelSelectState(pInstance->GetCalibrateChannel() - 1,bShowCalibChanData);
		}

		// ѡ���豸
		if (m_baseSetDlg.m_hWnd != NULL)
		{
			m_baseSetDlg.SetSelectedInstrument(pInstance->GetModel(), pInstance->GetSN());
		}

		SetConnectState(m_bInstruConnected);
	}

	//����򿪵���ģ���ļ���������ز���
	if(NULL != pInstance && pExpData->GetBeTemplateFile())
	{
		//�¶�ͨ������Ϊ0����Ҫ��������
		pExpData->SetTempChannelCount(pInstance->GetTempChannelCount());
		pExpData->SetScanStartPos(pInstance->GetScanStartPos());
	}

	//��ʼ���װ�ؼ���Ϣ
	vector<CTubeInfo*> vecTubeInfoPtr;
	pExpData->GetAllTubeInfoPtr(vecTubeInfoPtr);
	m_selPrjDlg.m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,TRUE);
	m_samSetDlg.m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,TRUE);

	//��ʼ������ͨ����Ϣ
	int iDyeChanCount = pExpData->GetDyeChannelCount();
	for(int i = 0;i < iDyeChanCount;++i)
	{
		tagDyeChannelInfo* pDyeChanInfo = pExpData->GetDyeChannelInfo(i);
		m_selPrjDlg.m_samBaseInfoEditDlg.AddChannel(pDyeChanInfo);
	}

	//����ͨ��ѡ���б���Ĭ�ϵ�Ⱦ������ɫ
	m_selPrjDlg.m_samBaseInfoEditDlg.SetDefaultDyeName(m_pExpData->GetDefaultDyeName());
	m_selPrjDlg.m_samBaseInfoEditDlg.SetDefaultColor();
	//����Ĭ�ϻ���������ʱʹ�ã�20220414
	m_selPrjDlg.m_samBaseInfoEditDlg.SetDefaultGeneName();
	
	//�½�ʵ��ʱ����ʼ��ͨ��ѡ��ť״̬��Ϣ�����ļ�ʱ�����ͨ��ѡ��ť״̬��Ϣ��ͨ����Ŀ��һ�£�������ͨ��ѡ��ť״̬��Ϣ��20220326��������
	int iChanCount = pExpData->GetChannelCount();
	if(iChanCount < 0)
	{
		return;
	}
	vector<int> vecChanSelState;
	for(int i = 0;i < iChanCount;++i)
	{
		vecChanSelState.push_back(0);
	}
	if(iChanCount != pExpData->GetMonitorChannelBtSelectState().size())
	{
		pExpData->ResetMonitorChannelBtSelectState();
		pExpData->SetMonitorChannelBtSelectState(vecChanSelState);
	}
	if(iChanCount != pExpData->GetDataAnaChannelBtSelectState().size())
	{
		pExpData->ResetDataAnaChannelBtSelectState();
		pExpData->SetDataAnaChannelBtSelectState(vecChanSelState);
	}

	vector<CString> vecChanDyeName;
	if(pExpData->GetBeNewExperFile())
	{
		pInstance->GetInitChanDyeName(vecChanDyeName);
		pExpData->SetChannelDyeName(vecChanDyeName);
	}
	else
	{
		if(!pExpData->GetBeValidChannelDyeName())
		{
			pExpData->GetChannelDyeNameFromTubeInfo();
			//����������л�ȡ��ͨ����Ϣ������������������ʹ��Ĭ�ϵ�Ⱦ������
			if(!pExpData->GetBeValidChannelDyeName())
			{
				pInstance->GetInitChanDyeName(vecChanDyeName);
				pExpData->SetChannelDyeNameIfEmpty(vecChanDyeName);
			}
		}
	}
	
	//��ʼ��������������
	CDataAnalysisPara* pDataAnalysisParam = pExpData->GetAnalysisPara();
	if(NULL == pDataAnalysisParam)
	{
		pDataAnalysisParam = new CDataAnalysisPara;
		if(NULL == pDataAnalysisParam)
		{
			return;
		}
		pExpData->SetAnalysisPara(pDataAnalysisParam);
	}
	if(pExpData->GetBeNewExperFile() && iChanCount != pDataAnalysisParam->GetAmplifyParaCount())
	{
		for(int i = 0;i < iChanCount;++i)
		{
			tagAnaAmplifyPara* pAnaAmpParam = new tagAnaAmplifyPara;
			if(NULL == pAnaAmpParam)
			{
				return;
			}
			pAnaAmpParam->iChannelNo = i + 1;
			//Ĭ��ʹ���Զ�����
			pAnaAmpParam->bAutoBaseline = TRUE;
			//��ʼ��Ĭ��Ⱦ����
			if(i < vecChanDyeName.size())
			{
				pAnaAmpParam->strDye = vecChanDyeName[i];
			}

			//ʹ��ϵͳ˭֪�е�������������
			if(i < theApp.m_vecAmplifyPara.size())
			{
				*pAnaAmpParam = theApp.m_vecAmplifyPara[i];
			}

			pDataAnalysisParam->AddAmplifyPara(pAnaAmpParam);
		}
	}
	
	if(iChanCount != pDataAnalysisParam->GetMeltCurveParaCount())
	{
		for(int i = 0;i < iChanCount;++i)
		{
			tagAnaMeltCurvePara* pAnaMeltParam = new tagAnaMeltCurvePara;
			if(NULL == pAnaMeltParam)
			{
				return;
			}
			pAnaMeltParam->iChannelNo = i + 1;
			//��ʼ��Ĭ��Ⱦ����
			if(i < vecChanDyeName.size())
			{
				pAnaMeltParam->strDye = vecChanDyeName[i];
			}

			//ʹ��ϵͳ˭֪�е�������������
			if(i < theApp.m_vecMeltCurvePara.size())
			{
				*pAnaMeltParam = theApp.m_vecMeltCurvePara[i];
			}

			pDataAnalysisParam->AddMeltCurvePara(pAnaMeltParam);
		}
	}

	//���÷���ҳ��ͨ������ѡ���ѡ����Ϣ
	m_dataAnaDlg.SetCbSelectChanThreString();

	//���ļ��󣬸���ͨ����Ŀ�Զ����� ͨ����Ϣ�༭ �б�����и�
	m_selPrjDlg.m_samBaseInfoEditDlg.LstRowHeightAutoFit(pExpData->GetChannelCount());

	//���ñ�ʵ����ݶȹ����Ƿ�֧��
#ifdef N96INSTRUMENT
	SetGradientFuncShieldState(FALSE);
#else
	SetGradientFuncShieldState(TRUE);
#endif

	//N96������C1,C6��ʾ����������
#ifdef XINYI_THEME
	SetTemperatureRateShowState(TRUE);
#endif

#ifdef N96INSTRUMENT
	SetTemperatureRateShowState(TRUE);
#endif

	return;
}

int CMiddleDlg::Export()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	if(m_selPrjDlg.IsWindowVisible() || m_samSetDlg.IsWindowVisible())
	{
		SynchronizeSampleSet();

		int iIdVersion = 0;
#ifdef PS96INSTRUMENT
		iIdVersion = 1;
#endif
		m_pExpData->ExportSampleSetInfo(ESEM_VERT == theApp.m_eSamExportMode,iIdVersion);
	}
	else if(m_dataAnaDlg.IsWindowVisible())
	{
		ExportResult();
	}

	return 0;
}

int CMiddleDlg::ExportPicture()
{
	return m_dataAnaDlg.ExportPicture();
}

int CMiddleDlg::Import()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	if(m_selPrjDlg.IsWindowVisible() || m_samSetDlg.IsWindowVisible())
	{
		int iRet = 0;
#ifdef PS96INSTRUMENT
		vector<eSampleType> vecSamType;
		GetAllSampleType(vecSamType);
		iRet = m_pExpData->InputSampleSetInfo(&vecSamType);
		if(0 == iRet)
#else
		iRet = m_pExpData->InputSampleSetInfo();
		if(0 == iRet)
#endif
		{
			//����ɹ������¿װ�༭�������Ǽ�ҳ��
			vector<CTubeInfo*> vecTubeInfoPtr;
			m_pExpData->GetAllTubeInfoPtr(vecTubeInfoPtr);
			m_samSetDlg.m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,m_samSetDlg.m_samPlateDlg.IsWindowVisible());
			m_samSetDlg.ReSetLstSamInfo(vecTubeInfoPtr,m_samSetDlg.m_lstSamInfo.IsWindowVisible());
			m_samSetDlg.OnMsgPlateSiteSelectChanged(0,0);

			m_selPrjDlg.m_samPlateDlg.SetTubeInfo(vecTubeInfoPtr,m_selPrjDlg.m_samPlateDlg.IsWindowVisible());
			m_selPrjDlg.OnMsgPlateSiteSelectChanged(0,0);

#ifdef PS96INSTRUMENT
			PopNormalTipInfo(theApp.m_pLangInfo->GetText(344));
#endif
		}
#ifdef PS96INSTRUMENT
		else
		{
			if(-5 == iRet)
			{
				PopWarningTipInfo(theApp.m_pLangInfo->GetText(345));
				return -2;
			}
			else if(-6 == iRet || -7 == iRet || -8 == iRet || -9 == iRet || -10 == iRet)
			{
				PopWarningTipInfo(theApp.m_pLangInfo->GetText(346));
				return -3;
			}
			else if(-1 == iRet)
			{
				PopWarningTipInfo(theApp.m_pLangInfo->GetText(347));
				return -2;
			}
		}
#endif
	}

	return 0;
}

double CMiddleDlg::GetAverageLidTem()
{
	double dSum = 0;
	int iLidTemNum = m_lnkLidTem.GetSize();
	if(iLidTemNum < 1)
	{
		return 0;
	}

	for(int i = 0;i < iLidTemNum;++i)
	{
		dSum += *m_lnkLidTem[i];
	}
	return dSum / iLidTemNum;
}

double CMiddleDlg::GetAveragePeltiaTem()
{
	double dSum = 0;
	int iPeltiaTemNum = m_lnkPeltiaTem.GetSize();
	if(iPeltiaTemNum < 1)
	{
		return 0;
	}

	for(int i = 0;i < iPeltiaTemNum;++i)
	{
		dSum += *m_lnkPeltiaTem[i];
	}
	return dSum / iPeltiaTemNum;
}

double CMiddleDlg::PeltiaTemAmend(double dTem,double dAmendScope /*= 3.0*/)
{
#ifdef _DEBUG
	if(dTem > 66)
	{
		int tt = 0;
		++tt;
	}
	if(dTem > 70)
	{
		int tt = 0;
		++tt;
	}
#endif

	int iCurCyle = m_pRunCtrl->GetCurCycle();
	int iCurSeg = m_pRunCtrl->GetCurSegment();
	int iCurStep = m_pRunCtrl->GetCurStep();

	if(iCurCyle < 0 || iCurSeg < 0 || iCurStep < 0 || NULL == m_pExpData)
	{
		return dTem;
	}

	CLinkList<CSegItemInfo>& lnkSegInfo = m_pExpData->GetSegment();
	if(iCurSeg >= lnkSegInfo.GetSize())
	{
		return dTem;
	}

	CSegItemInfo* pSegItemInfo = lnkSegInfo[iCurSeg];
	if(iCurStep >= pSegItemInfo->m_lnkStepItemInfo.GetSize())
	{
		return dTem;
	}

	if(pSegItemInfo->m_lnkStepItemInfo[iCurStep]->GetBeCollectAll())
	{
		return dTem;
	}

	double dAimTem = pSegItemInfo->m_lnkStepItemInfo[iCurStep]->m_fAimTem;
	if(abs(dAimTem - dTem) > dAmendScope)
	{
		return dTem;
	}

	return dAimTem - (dAimTem - dTem) / 5;
}

double CMiddleDlg::LidTemAmend(double dTem,double dAmendScope /*= 3.0*/)
{
	if(NULL == m_pExpData)
	{
		return dTem;
	}

	double dAimTem = m_pExpData->GetExperimentInfo()->GetLidTemp();
	if(dTem - dAimTem < -1 * dAmendScope)
	{
		return dTem;
	}

	return dAimTem - (dAimTem - dTem) / 5;
}

int CMiddleDlg::InitFluBackUp()
{
	if(NULL == m_pExpData)
	{
		return -1;
	}
	int iChanCount = m_pExpData->GetChannelCount();
	if(iChanCount < 1)
	{
		return -2;
	}

	int iRowNum = m_pExpData->GetRowCount();
	if(iRowNum < 1)
	{
		return -3;
	}
	int iColNum = m_pExpData->GetColCount();
	if(iColNum < 1)
	{
		return -4;
	}
	int iTubeNum = m_pExpData->GetTubeCount();
	if(iTubeNum < 1)
	{
		return -5;
	}

	CString strFilePath(_T("")),strTemp(_T(""));;
	strFilePath = GetExeFileDirectory() + _T("Data2");
	if(!PathIsDirectory(strFilePath))
	{
		if(!CreateDirectory(strFilePath,NULL))
		{
			return -6;
		}
	}
	m_strBackupFilePath = strFilePath + _T("\\") + m_pExpData->GetBackupFileCreateTime() + _T(".xlsx");

	if(NULL != m_ppSheet)
	{
		delete [] m_ppSheet;
	}
	m_ppSheet = new libxl::Sheet*[iChanCount];
	if(NULL == m_ppSheet)
	{
		return -7;
	}

	if(NULL != m_pBook)
	{
		m_pBook->release();
		m_pBook = NULL;
	}
	m_pBook = xlCreateXMLBook();
	if(NULL == m_pBook)
	{
		goto ERROR_END;
	}

	m_pBook->setKey(_T("Halil Kural"), _T("windows-2723210a07c4e90162b26966a8jcdboe"));

	for(int i = 0;i < iChanCount;++i)
	{
		strTemp.Format(_T("%d"),i + 1);
		m_ppSheet[i] = m_pBook->addSheet(strTemp);

		if (NULL == m_ppSheet[i])
		{
			goto ERROR_END;
		}
	}

	for(int k = 0;k < iChanCount;++k)
	{
		int iRowId = 1,iColId = 0;
		for(int i = 0;i < iTubeNum;++i)
		{
			strTemp.Format(_T("Tube%c%d"),'A' + i / iColNum,i % iColNum + 1);
			m_ppSheet[k]->writeStr(iRowId,++iColId,strTemp);
		}
	}

	m_pBook->save(m_strBackupFilePath);

	return 0;

ERROR_END:
	ClearBackupFileParam();
	return -10;
}

void CMiddleDlg::ClearBackupFileParam()
{
	if(NULL != m_pBook)
	{
		m_pBook->release();
		m_pBook = NULL;
	}
	if(NULL != m_ppSheet)
	{
		delete [] m_ppSheet;
		m_ppSheet = NULL;
	}
	return;
}

int CMiddleDlg::SetEditMode(EEXPEDITMODE eExpEditMode)
{
	m_baseSetDlg.SetEditMode(eExpEditMode);
	m_selPrjDlg.SetEditMode(eExpEditMode);
	m_samSetDlg.SetEditMode(eExpEditMode);

	return 0;
}

int CMiddleDlg::SampleDyeNameChanged(int iChanId,CString strNewDyeName)
{
	if(NULL == m_pExpData)
	{
		return -1;
	}

	int iChanCount = m_pExpData->GetChannelCount();
	if(iChanCount < 1)
	{
		return -2;
	}

	if(iChanId < 0)
	{
		return -3;
	}

	if(iChanId >= iChanCount)
	{
		return -4;
	}

	if(strNewDyeName == m_pExpData->GetOneChannelDyeName(iChanId))
	{
		return 1;
	}

	//����ʵ���ļ��м�¼��ͨ��Ⱦ������Ϣ
	m_pExpData->SetOneChannelDyeName(iChanId,strNewDyeName);
	//���·���ҳ��ͨ��Ⱦ������ʶ
	m_dataAnaDlg.SetOneChannelDyeName(iChanId,strNewDyeName);
	//����ͨ���л���ť����
	m_funcBtDlg.SetOneChannelBtText(iChanId,strNewDyeName);

	return 0;
}

void CMiddleDlg::ClearExperiment()
{
	m_baseSetDlg.ClearPage();
	m_selPrjDlg.ClearExperiment();
	m_runMonitorDlg.ClearExperiment();
	m_dataAnaDlg.ClearPage();
	
	if(NULL == m_pExpData)
	{
		return;
	}

	//ɾ������
	if (m_pExpData->HasRunData())
	{
		m_pExpData->DeleteRunData();
	}
	m_pExpData->DeleteRunSegment();
	m_pExpData->DeleteTubeInfo();

	return;
}

void CMiddleDlg::FileOpened()
{
	m_baseSetDlg.FileOpened(m_pExpData);
	m_selPrjDlg.FileOpened(m_pExpData);
	m_samSetDlg.FileOpened(m_pExpData);
	m_dataAnaDlg.FileOpened(m_pExpData);
	m_runMonitorDlg.FileOpened(m_pExpData);

	//����򿪵��Ǵ������ļ����л�������ҳ�棨PS96IVDģʽ�л������ҳ�棩
	if(m_pExpData->HasRunData())
	{
		SetPageSelected(ESPT_DATAANALYSE);
	}
}

void CMiddleDlg::SetTemplateListSelectedRow(int iIndex)
{
	m_selPrjDlg.SetTemplateListSelectedRow(iIndex);
	return;
}

int CMiddleDlg::NewTemplateAdded(int iIndex)
{
	m_selPrjDlg.NewTemplateAdded(iIndex);
	return 0;
}

BOOL CMiddleDlg::GetBeExportData()
{
	if(NULL == m_pExpData)
	{
		return FALSE;
	}

	if(theApp.GetBeIVDUser() && m_pExpData->GetBePS96ExperFile())
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CMiddleDlg::IsExpRunning()
{
	return m_bExpRunning;
}

int CMiddleDlg::SetGradientFuncShieldState(BOOL bData)
{
	m_selPrjDlg.SetGradientFuncShieldState(bData);
	return 0;
}

int CMiddleDlg::SetTemperatureRateShowState(BOOL bData)
{
	m_selPrjDlg.SetTemperatureRateShowState(bData);
	return 0;
}

void CMiddleDlg::SetPowerOffExpInfo(CRunProgressRecord runPrgRcd)
{
	m_runRrgRcd = runPrgRcd;
	m_bPowerOffExp = TRUE;
	return;
}

int CMiddleDlg::GetAllSampleType(vector<eSampleType>& vecSamType)
{
	return m_selPrjDlg.GetAllSampleType(vecSamType);
}

int CMiddleDlg::UploadInspectResultToLis()
{
	return m_dataAnaDlg.UploadInspectResultToLis();
}

int CMiddleDlg::RefreshLanguageDisplay()
{
	int iFontH = 18 * m_dTxRatio;

	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString strSgementChs = pLangInfo->GetText(57, LANGUAGE_CHS);
	CString strSegmentEnu = pLangInfo->GetText(57, LANGUAGE_ENG);
	if (pLangInfo->GetLanguage() == LANGUAGE_CHS)
	{
		m_pExpData->CorrectSegmentNameByLanguage(strSegmentEnu, strSgementChs);
	}
	else
	{
		m_pExpData->CorrectSegmentNameByLanguage(strSgementChs, strSegmentEnu);
	}

	m_swtBaseSet.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtBaseSet.SetWindowText(pLangInfo->GetText(13));

	m_btnRunStop.SetMyFont(m_stTxFont,iFontH);
	m_btnRunStop.SetWindowText(pLangInfo->GetText(4));

	m_swtSelectPrj.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtSelectPrj.SetWindowText(pLangInfo->GetText(14));

	m_swtSamSet.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtSamSet.SetWindowText(pLangInfo->GetText(15));

	m_swtRunMonitor.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtRunMonitor.SetWindowText(pLangInfo->GetText(16));

	m_swtDataAna.SetMyFont(m_stTxFont,iFontH,FW_NORMAL,TYPE_PIXEL);
	m_swtDataAna.SetWindowText(pLangInfo->GetText(17));

	m_baseSetDlg.RefreshLanguageDisplay();
	m_selPrjDlg.RefreshLanguageDisplay();
	m_samSetDlg.RefreshLanguageDisplay();
	m_runMonitorDlg.RefreshLanguageDisplay();
	m_dataAnaDlg.RefreshLanguageDisplay();

	return 0;
}

int CMiddleDlg::CycleAddSuccessful(int iSegId,int iCount)
{
	return m_runMonitorDlg.CycleAddSuccessful(iSegId,iCount);
}

int CMiddleDlg::RefreshExpTotalTime(int iTotalSec)
{
	m_runMonitorDlg.m_runStateDlg.m_mntBaseInfoDlg.m_stTotalSec.SetWindowText(SecondToHourMinuteSecond(iTotalSec));
	return 0;
}

void CMiddleDlg::SetCurveHoriAxisParam()
{
	int iCrvEndValue = m_pExpData->GetFirstEndCollectCycleNum();
	if(iCrvEndValue < 1)
	{
		iCrvEndValue = CURVE_X_DEFMAXVALUE;
	}
	m_runMonitorDlg.SetCurveHoriAxisParam(1,iCrvEndValue,1);
	m_dataAnaDlg.SetCurveHoriAxisParam(1,iCrvEndValue,1);
	return;
}

void CMiddleDlg::SetExpDataPtr(CExperimentData* pExpData)
{
	m_pExpData = pExpData;


	return;
}

void CMiddleDlg::EmbodyBaseSetInfo()
{
	if(NULL == m_pExpData)
	{
		return;
	}
	m_baseSetDlg.SetBaseInfo(*m_pExpData->GetExperimentInfo());
	return;
}

void CMiddleDlg::RefreshExpEmbody()
{

	return;
}

void CMiddleDlg::DeleteTemplateListRow(int iIndex)
{
	return;
}

int CMiddleDlg::ExportMeltData(libxl::Book* pBook)
{
	if(!GetBeExportData())
	{
		return 1;
	}

	if(NULL == pBook)
	{
		return -1;
	}

	if(NULL == m_pExpData)
	{
		return -2;
	}

	int iMeltDataCount = m_pExpData->GetMeltDataNum();
	if(iMeltDataCount < 1)
	{
		return 2;
	}

	int iChanNum = m_pExpData->GetChannelCount();
	int iRowCount = m_pExpData->GetRowCount();
	int iColCount = m_pExpData->GetColCount();
	
	int iRow = 1,iCol = 1,iTubeNum = iRowCount * iColCount,iTempInt1 = 0;;
	CTubeInfo* pTubeInfo = NULL;
	ESAMSORTWAY eSamSortWay = ESSW_HORI;

	int iTubeIndex = 0,iMEnd = 0,iTEnd = 0,iIndex1 = 0,iIndex2 = 0;
	int iChanDataRowInterval = 2;	//ͨ���������һ��ͨ�����һ�������м����

	libxl::Sheet* pSheetMeltOrigin = pBook->addSheet(_T("Melting Origin"));
	if(NULL == pSheetMeltOrigin)
	{
		return -3;
	}

	libxl::Sheet* pSheetMeltPeak = pBook->addSheet(_T("Melting Peak"));
	if(NULL == pSheetMeltPeak)
	{
		return -4;
	}

	double* pDData = new double[iMeltDataCount];
	if(NULL == pDData)
	{
		return -5;
	}
	memset(pDData,0,sizeof(double) * iMeltDataCount);

	CString strTemp(_T(""));

	pSheetMeltOrigin->writeStr(0,0,_T("QPCR Fluoresence"));
	pSheetMeltOrigin->setCol(0,0,16);	//width�����ؿ���1:9

	pSheetMeltPeak->writeStr(0,0,_T("QPCR Fluoresence"));
	pSheetMeltPeak->setCol(0,0,16);	//width�����ؿ���1:9

	iRow = 1;
	for(int k = 0;k < iChanNum;++k)
	{
		//���ѭ�����ȱ�����Ϣ
		iCol = 0;

		strTemp = m_pExpData->GetOneChannelDyeName(k);
		if(strTemp.IsEmpty())
		{
			strTemp.Format(_T("Channel %d"),k + 1);
		}
		pSheetMeltOrigin->writeStr(iRow,iCol,strTemp);
		pSheetMeltPeak->writeStr(iRow,iCol,strTemp);

		iCol += 2;
		pSheetMeltOrigin->writeStr(iRow,iCol,_T(""));
		pSheetMeltPeak->writeStr(iRow,iCol,_T(""));

		//���ѭ����
		iCol = 1;
		++iRow;
		pSheetMeltOrigin->writeStr(iRow,iCol,_T("Temperature"));
		pSheetMeltPeak->writeStr(iRow,iCol,_T("Temperature"));

		//�����¶ȱ����п�
		if(0 == k)
		{
			pSheetMeltOrigin->setCol(iRow,iCol,12);	//width�����ؿ���1:9
			pSheetMeltOrigin->setCol(iRow,iCol,12);	//width�����ؿ���1:9
		}

		m_pExpData->GetMeltCycleX(iTubeIndex + 1,iMeltDataCount,pDData);
		for(int j = 0;j < iMeltDataCount;++j)
		{
			pSheetMeltOrigin->writeNum(++iRow,iCol,pDData[j]);
			pSheetMeltPeak->writeNum(iRow,iCol,pDData[j]);
		}
		iCol += 2;
		iRow += iChanDataRowInterval;
	}

	//��������
	if(ESSW_HORI == eSamSortWay)
	{
		iMEnd = iRowCount;
		iTEnd = iColCount;
	}
	else
	{
		iMEnd = iColCount;
		iTEnd = iRowCount;
	}

	iCol = 3;
	for(int m = 0;m < iMEnd;++m)
	{
		for(int t = 0;t < iTEnd;++t)
		{
			if(ESSW_HORI == eSamSortWay)
			{
				iTubeIndex = m * iColCount + t;
				iIndex1 = m;
				iIndex2 = t + 1;
			}
			else
			{
				iTubeIndex = t * iColCount + m;
				iIndex1 = t;
				iIndex2 = m + 1;
			}

			pTubeInfo = m_pExpData->GetTubeInfoAt(iTubeIndex);
			if(NULL == pTubeInfo || pTubeInfo->GetBeNoneSampleType())
			{
				continue;
			}

			//��������
			iRow = 2;
			for(int k = 0;k < iChanNum;++k)
			{
				strTemp.Format(_T("Tube_%s"),pTubeInfo->GetTubeLabel(iColCount));
				pSheetMeltOrigin->writeStr(iRow,iCol,strTemp);
				pSheetMeltPeak->writeStr(iRow,iCol,strTemp);

				iTempInt1 = iRow;
				m_pExpData->GetMeltChannelFluY(iTubeIndex + 1,FLU_FILTER,k,iMeltDataCount,pDData);
				for(int j = 0;j < iMeltDataCount;++j)
				{
					pSheetMeltOrigin->writeNum(++iRow,iCol,(int)pDData[j]);
				}
				m_pExpData->GetMeltChannelFluY(iTubeIndex + 1,MELT_GAINDATA,k,iMeltDataCount,pDData);
				for(int j = 0;j < iMeltDataCount;++j)
				{
					pSheetMeltPeak->writeNum(++iTempInt1,iCol,pDData[j]);
				}
	
				iRow += (iChanDataRowInterval + 1);
			}
			++iCol;
		}
	}

	delete [] pDData;

	return 0;
}
	
//��Ŀ�ѱ����棬���µ�ʵ��ҳ��
int CMiddleDlg::SaveProject(int iIndex,BOOL bNew)
{
	return m_selPrjDlg.SavePrj(iIndex, bNew);
}

//ɾ��һ����Ŀ
int CMiddleDlg::DeleteProject(int iIndex)
{
	return m_selPrjDlg.DeletePrj(iIndex);
}

void CMiddleDlg::SetDefaultGeneName()
{
	m_selPrjDlg.m_samBaseInfoEditDlg.SetDefaultGeneName();
}

BOOL CMiddleDlg::GetBeDataAnaDlgVisible()
{
	return m_dataAnaDlg.IsWindowVisible();
}
	
//ѡ����Ŀҳ��������Ϣ�ı��ͬ���������Ǽ�ҳ��
int CMiddleDlg::ResetTubeInfo(vector<int> &vecPlateSelectedSite,vector<CTubeInfo> &vecTubeInfo, int iFlag)
{
	if (iFlag == 0)
		return m_samSetDlg.ReSetTubeInfo(vecPlateSelectedSite, vecTubeInfo);
	else
		return m_selPrjDlg.ReSetTubeInfo(vecPlateSelectedSite, vecTubeInfo);
}

void CMiddleDlg::SetChannelDyeBtn()
{
	//��ȡȾ��ͨ������
	int iChanCount = m_pExpData->GetChannelCount();
	if(iChanCount < 1)
		return;

	//����ͨ���л���ť����,PS96IVDģʽ�²���ʾ����ͨ����Ϣ
	int iChanCountForShow = iChanCount;
	if((theApp.GetBeIVDUser() || theApp.GetBeAdminPermission()) && m_pExpData->GetBeUseTurbidityCalibration() && iChanCountForShow > 1)
	{
		iChanCountForShow = iChanCount - 1;
	}
	m_funcBtDlg.SetChannelBtNum(iChanCountForShow, m_pExpData->GetChannelDyeName(),FALSE);

	BOOL bShowChanBt = FALSE;
	//����ͨ���л���ťѡ��״̬
	vector<int> vecChanSelState;
	if(ESPT_RUNMONITOR == m_curSelSonPage)
	{
		vecChanSelState = m_pExpData->GetMonitorChannelBtSelectState();
		bShowChanBt = TRUE;
	}
	else if(ESPT_DATAANALYSE == m_curSelSonPage)
	{
		vecChanSelState = m_pExpData->GetDataAnaChannelBtSelectState();
		bShowChanBt = TRUE;
	}
	m_funcBtDlg.SetChanBtSelectState(vecChanSelState);

	//����ͨ���л���ť��ʾ״̬
	m_funcBtDlg.SetChanBtShowState(bShowChanBt);
}


LRESULT CMiddleDlg::OnMsgChanBtClicked(WPARAM wParam,LPARAM lParam)
{
	if(ESPT_RUNMONITOR == m_curSelSonPage)
	{
		m_pExpData->SetMonitorChannelBtSelectState(wParam,lParam);
		m_runMonitorDlg.UpdateCurveSelectState(FALSE);
		m_runMonitorDlg.ResetCurveMaxMinValue(TRUE);
	}
	else if(ESPT_DATAANALYSE == m_curSelSonPage)
	{
		m_pExpData->SetDataAnaChannelBtSelectState(wParam,lParam);
		m_dataAnaDlg.UpdateCurveSelectState(FALSE);
		m_dataAnaDlg.ResetCurveMaxMinValue(TRUE);
		m_dataAnaDlg.ResetResultLst();
		m_dataAnaDlg.RefreshTheChannelToShowThre(wParam,lParam);
	}

	return 0;
}

void CMiddleDlg::SetConnectState(BOOL bRunExp)
{
	if (theApp.GetBeIVDUser())
		return;

	int iTxId = 314;
	if(!m_bInstruConnected)
	{
		iTxId = 315;
	}
	m_stcConnectState.SetWindowText(theApp.m_pLangInfo->GetText(iTxId));
	InvalidateRect(m_rectConntectState);

	m_btnRunStop.EnableWindow(bRunExp || m_bExpRunning);
}

// ����ʵ�鿪ʼ�����
void CMiddleDlg::OnBnClickedSwtBtRunStop()
{
	if(m_bExpRunning)
	{
		Stop();
	}
	else
	{
		Run();
	}
}

void CMiddleDlg::SetRunBtnState(BOOL bRunning)
{
	if (bRunning)
	{
		m_btnRunStop.SetIconBtPath(_T(".\\BkPic\\stop.png"));
		m_btnRunStop.SetWindowText(theApp.m_pLangInfo->GetText(5));
	}
	else
	{
		m_btnRunStop.SetIconBtPath(_T(".\\BkPic\\start.png"));
		m_btnRunStop.SetWindowText(theApp.m_pLangInfo->GetText(4));
	}
}

LRESULT CMiddleDlg::OnMsgInstrumentSelected(WPARAM wParam,LPARAM lParam)
{
	int iInstrumentIndex = wParam;
	CInstrumentPCR* pInstrument = CInstrumentPCR::GetInstance();
	CInstancePCR* pInstance = pInstrument->GetInstancePCR(iInstrumentIndex);	
	if (pInstance != NULL && !pInstance->IsDemo())
	{
		m_bInstruConnected = pInstance->GetInstanceStatus() == INSTANCE_READY || pInstance->GetInstanceStatus() == INSTANCE_USING;

		m_pExpData->GetExperimentInfo()->SetInstrument(pInstance->GetModel());
		m_pExpData->GetExperimentInfo()->SetInstrumentSN(pInstance->GetSN());
		m_pExpData->GetExperimentInfo()->SetLidTemp(pInstance->GetLidTem());

		SetConnectState(pInstance->CanRunExp());
	}

	return 0;
}

void CMiddleDlg::SetInstrumentConnectState(CString strModel, CString strSN, BOOL bConnected)
{
	if (m_pExpData->GetExperimentInfo()->GetInstrument() == strModel && m_pExpData->GetExperimentInfo()->GetInstrumentSN() == strSN)
	{
		if (bConnected != m_bInstruConnected)
		{
			m_bInstruConnected = bConnected;
			SetConnectState(m_bInstruConnected);
		}
	}
}

void CMiddleDlg::ReshowInstrumentList()
{
	if (m_bExpRunning)
		return;

	m_baseSetDlg.ReshowInstrumentList();
}

void CMiddleDlg::SetSelectedInstrument(CString strModel, CString strSN)
{
	m_baseSetDlg.SetSelectedInstrument(strModel, strSN);
}

// ����ʵ�������ϲ�Admin��User
int CMiddleDlg::ExportResult()
{
	CString strTemp(_T("")),strDefName(_T("")),strRstPath(_T("")),strRstPath2(_T(""));
	strTemp = _T("Excel File(*.xlsx)|*.xlsx||");
	strDefName = m_pExpData->GetFileName();
	CFileDialog dlg(FALSE,_T("xlsx"),strDefName,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,strTemp,NULL);
	dlg.m_ofn.lpstrTitle = _T("Select file save path");
	if(IDCANCEL == dlg.DoModal()) 
	{
		return -1;
	}

	strRstPath = dlg.GetPathName();
	if (GetBeExportData())
	{
		strRstPath2 = dlg.GetFolderPath() + _T("\\") + dlg.GetFileTitle() + PS96IVD_DATA_RESULT_SUFFIX + _T(".xlsx");
	}
	else
	{
		strRstPath2 = dlg.GetFolderPath() + _T("\\") + dlg.GetFileTitle() + PS96IVD_OLD_RESULT_SUFFIX + _T(".xlsx");
	}
	

	ExportResultReport(strRstPath,strRstPath2);

	return 0;
}

int CMiddleDlg::ExportResultReport(CString strFilename, CString strFilename2)
{
	libxl::Book* pBook = xlCreateXMLBook();
	if(NULL == pBook)
	{ 
		PopWarningTipInfo(_T("Failed to create execel file!"));
		return -2;
	}
	pBook->setKey(_T("Halil Kural"), _T("windows-2723210a07c4e90162b26966a8jcdboe")); 

	int iRet1 = 0,iRet2 = 0;

	iRet1 = ExportPS96ResultSheet(pBook);
	if(0 != iRet1)
	{
		pBook->release();
		return -3;
	}

	iRet2 = ExportInstrumentInfoSheet(pBook);
	if(0 != iRet2)
	{
		pBook->release();
		return -3;
	}

	if(pBook->save(strFilename)) 
	{
		//�����ļ�Ϊֻ���ļ�
		DWORD dwErrCode = 0;
		WIN32_FILE_ATTRIBUTE_DATA attrs = {0};
		if(GetFileAttributesEx(strFilename,GetFileExInfoStandard,&attrs))
		{
			DWORD dwNewFileAttr = attrs.dwFileAttributes | FILE_ATTRIBUTE_READONLY;
			if(!SetFileAttributesW(strFilename,dwNewFileAttr))
			{
				dwErrCode = GetLastError();
			}
		}
		else
		{
			dwErrCode = GetLastError();
		}

		::ShellExecute(NULL,_T("open"),strFilename,NULL,NULL,SW_SHOW);        
	}
	else
	{
		pBook->errorMessage();
	}

	pBook->release();

	//�����ɰ汨��
	if(!strFilename2.IsEmpty())
	{
		ExportCompleteInfo(strFilename2);
	}

	// ����CSV��ʽ�ļ�
	CString strFileCSV = strFilename;
	strFileCSV.Replace(_T(".xlsx"), _T(".csv"));
	ExportResultReportToCSV(strFilename2, strFileCSV);

	return 0;
}

int CMiddleDlg::ExportResultReportToCSV(CString strFileExcel, CString strFilCSV)
{
	libxl::Book* book = xlCreateXMLBook();
	if(NULL == book)
	{
		AfxMessageBox(_T("Fail to create excel book!"));
		return -2;
	}
	book->setKey(_T("Halil Kural"), _T("windows-2723210a07c4e90162b26966a8jcdboe"));

	if (!book->load(strFileExcel))
	{
		AfxMessageBox(_T("Fail to open excel file!"));
		book->release();
		return -3;
	}

	libxl::Sheet* sheet = book->getSheet(0);
	if(NULL == sheet)
	{
		AfxMessageBox(_T("Fail to get sheet!"));
		book->release();
		return -4;
	}

	CStdioFile  file;
	CFileException e;
	if (!file.Open(strFilCSV,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone, &e))
	{
		e.ReportError();
		book->release();
		return -1;
	}

	int iLastCol = sheet->lastCol();
	int iLastRow = sheet->lastRow();
	CString strLine, strItem;

	for (int i=1; i<iLastRow; i++)
	{
		strItem = sheet->readStr(i, 1);
		if (strItem.IsEmpty())
		{
			break;
		}

		strLine = strItem;
		for (int j=2; j<iLastCol; j++)
		{
			strItem = sheet->readStr(i, j);
			//ASSERT(strItem.Find(_T("\r\n")) != -1);

			//strItem.Replace(_T(","), _T("",""));
			strLine += _T(",") + strItem;
		}

		strLine += _T("\n");
		file.WriteString(strLine);
	}

	book->release();

	file.Close();
}



void CMiddleDlg::ExportPS96ResultSheetFormat(libxl::Book* pBook, libxl::Sheet* pSheetId)
{
	double dBaseColWidth = pSheetId->colWidth(0);
	pSheetId->setCol(0,m_iStartCol-1, dBaseColWidth/15);
	//pSheetId->setCol(m_iStartCol, m_iStartCol+m_iColCount-1, dBaseColWidth*0.96);
	pSheetId->setCol(m_iStartCol, m_iStartCol+m_iColCount-1, dBaseColWidth*0.92);
	pSheetId->setCol(m_iStartCol+m_iColCount, m_iStartCol+m_iColCount, dBaseColWidth/15);

	double dBaseRowHeight = pSheetId->rowHeight(0);
	for (int i=m_iFirstPaneStart; i<=m_iFourthPaneEnd; i++)
	{
		if (i == m_iFirstPaneStart)
		{
			pSheetId->setRow(i, dBaseRowHeight*1.5);
		}
		else
		{
			pSheetId->setRow(i, dBaseRowHeight*1.2);
		}
	}
	pSheetId->setRow(m_iFourthPaneEnd+1, dBaseRowHeight/5);

	//ExportPS96ResultSheetRectBorder(pBook, pSheetId, m_iFirstPaneStart, m_iStartCol, m_iFirstPaneEnd, m_iStartCol+m_iColCount);
	ExportPS96ResultSheetRectBorder(pBook, pSheetId, m_iSecondPaneStart, m_iStartCol-1, m_iSecondPaneEnd, m_iStartCol+m_iColCount);
	ExportPS96ResultSheetRectBorder(pBook, pSheetId, m_iThirdPaneStart, m_iStartCol-1, m_iThirdPaneEnd, m_iStartCol+m_iColCount);
	ExportPS96ResultSheetRectBorder(pBook, pSheetId, m_iFourthPaneStart, m_iStartCol-1, m_iFourthPaneEnd+1, m_iStartCol+m_iColCount);

	pSheetId->setDisplayGridlines(false);
}

void CMiddleDlg::ExportPS96ResultSheetRectBorder(libxl::Book* pBook, libxl::Sheet* pSheetId, int iStartRow, int iStartCol, int iEndRow, int iEndCol)
{
	if (iEndRow-iStartRow < 0 || iEndCol - iStartCol < 0)
		return;

	libxl::Format* pCellFormat = NULL;
	// �Ľ�
	if (iEndRow-iStartRow == 0 && iEndCol - iStartCol == 0)
	{
		pCellFormat = pSheetId->cellFormat(iStartRow, iStartCol);
		libxl::Format* pBorderFormat = pBook->addFormat(pCellFormat);
		pBorderFormat->setBorder(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iStartRow, iStartCol, pBorderFormat);
	}
	else if (iEndRow-iStartRow > 0 && iEndCol - iStartCol == 0)
	{
		pCellFormat = pSheetId->cellFormat(iStartRow, iStartCol);
		libxl::Format* pBorderFormatTLR = pBook->addFormat(pCellFormat);
		pBorderFormatTLR->setBorderTop(libxl::BORDERSTYLE_THIN);
		pBorderFormatTLR->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pBorderFormatTLR->setBorderRight(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iStartRow, iStartCol, pBorderFormatTLR);

		pCellFormat = pSheetId->cellFormat(iEndRow, iStartCol);
		libxl::Format* pBorderFormatBLR= pBook->addFormat(pCellFormat);
		pBorderFormatBLR->setBorderBottom(libxl::BORDERSTYLE_THIN);
		pBorderFormatBLR->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pBorderFormatBLR->setBorderRight(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iEndRow, iStartCol, pBorderFormatBLR);

		pCellFormat = pSheetId->cellFormat(iStartRow+1, iStartCol);
		libxl::Format* pBorderFormatLR = pBook->addFormat(pCellFormat);
		pBorderFormatLR->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pBorderFormatLR->setBorderRight(libxl::BORDERSTYLE_THIN);
		for (int i=1; i<iEndRow-iStartRow; i++)
		{
			pSheetId->setCellFormat(iStartRow+i, iStartCol, pBorderFormatLR);
		}
	}
	else if (iEndRow-iStartRow == 0 && iEndCol - iStartCol > 0)
	{
		pCellFormat = pSheetId->cellFormat(iStartRow, iStartCol);
		libxl::Format* pBorderFormatLTB = pBook->addFormat(pCellFormat);
		pBorderFormatLTB->setBorderTop(libxl::BORDERSTYLE_THIN);
		pBorderFormatLTB->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pBorderFormatLTB->setBorderRight(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iStartRow, iStartCol, pBorderFormatLTB);

		pCellFormat = pSheetId->cellFormat(iStartRow, iEndCol);
		libxl::Format* pBorderFormatRTB= pBook->addFormat(pCellFormat);
		pBorderFormatRTB->setBorderBottom(libxl::BORDERSTYLE_THIN);
		pBorderFormatRTB->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pBorderFormatRTB->setBorderRight(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iStartRow, iEndCol, pBorderFormatRTB);

		pCellFormat = pSheetId->cellFormat(iStartRow, iStartCol+1);
		libxl::Format* pBorderFormatTB = pBook->addFormat(pCellFormat);
		pBorderFormatTB->setBorderTop(libxl::BORDERSTYLE_THIN);
		pBorderFormatTB->setBorderBottom(libxl::BORDERSTYLE_THIN);
		for (int i=1; i<iEndCol-iStartCol; i++)
		{
			pSheetId->setCellFormat(iStartRow, iStartCol+i, pBorderFormatTB);
		}
	}
	else
	{
		pCellFormat = pSheetId->cellFormat(iStartRow, iStartCol);
		libxl::Format* pBorderFormatLT = pBook->addFormat(pCellFormat);
		pBorderFormatLT->setBorderTop(libxl::BORDERSTYLE_THIN);
		pBorderFormatLT->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iStartRow, iStartCol, pBorderFormatLT);

		pCellFormat = pSheetId->cellFormat(iEndRow, iStartCol);
		libxl::Format* pBorderFormatLB= pBook->addFormat(pCellFormat);
		pBorderFormatLB->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pBorderFormatLB->setBorderBottom(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iEndRow, iStartCol, pBorderFormatLB);

		pCellFormat = pSheetId->cellFormat(iStartRow, iEndCol);
		libxl::Format* pBorderFormatRT = pBook->addFormat(pCellFormat);
		pBorderFormatRT->setBorderTop(libxl::BORDERSTYLE_THIN);
		pBorderFormatRT->setBorderRight(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iStartRow, iEndCol, pBorderFormatRT);

		pCellFormat = pSheetId->cellFormat(iEndRow, iEndCol);
		libxl::Format* pBorderFormatRB= pBook->addFormat(pCellFormat);
		pBorderFormatRB->setBorderBottom(libxl::BORDERSTYLE_THIN);
		pBorderFormatRB->setBorderRight(libxl::BORDERSTYLE_THIN);
		pSheetId->setCellFormat(iEndRow, iEndCol, pBorderFormatRB);

		pCellFormat = pSheetId->cellFormat(iStartRow+1, iStartCol);
		libxl::Format* pBorderFormatLeft = pBook->addFormat(pCellFormat);
		pBorderFormatLeft->setBorderLeft(libxl::BORDERSTYLE_THIN);
		pCellFormat = pSheetId->cellFormat(iStartRow+1, iEndCol);
		libxl::Format* pBorderFormatRight = pBook->addFormat(pCellFormat);
		pBorderFormatRight->setBorderRight(libxl::BORDERSTYLE_THIN);
		for (int i=1; i<iEndRow-iStartRow; i++)
		{
			pSheetId->setCellFormat(iStartRow+i, iStartCol, pBorderFormatLeft);
			pSheetId->setCellFormat(iStartRow+i, iEndCol, pBorderFormatRight);
		}

		pCellFormat = pSheetId->cellFormat(iStartRow, iStartCol+1);
		libxl::Format* pBorderFormatTop = pBook->addFormat(pCellFormat);
		pBorderFormatTop->setBorderTop(libxl::BORDERSTYLE_THIN);
		pCellFormat = pSheetId->cellFormat(iEndRow, iStartCol+1);
		libxl::Format* pBorderFormatBottom = pBook->addFormat(pCellFormat);
		pBorderFormatBottom->setBorderBottom(libxl::BORDERSTYLE_THIN);
		for (int i=1; i<iEndCol-iStartCol; i++)
		{
			pSheetId->setCellFormat(iStartRow, iStartCol+i, pBorderFormatTop);
			pSheetId->setCellFormat(iEndRow, iStartCol+i, pBorderFormatBottom);
		}
	}
}

void CMiddleDlg::ExportPS96ResultWriteStrBold(libxl::Book* pBook, libxl::Sheet* pSheetId, int iRow, int iCol, CString strText, int iFontSize)
{
	libxl::Format* pCellFormat = pSheetId->cellFormat(iRow, iCol);
	libxl::Format* pFormatBoldFont = pBook->addFormat(pCellFormat);
	libxl::Font* pFont = pBook->addFont(pCellFormat->font());
	pFont->setBold();
	if (iFontSize != -1)
		pFont->setSize(iFontSize);
	pFormatBoldFont->setFont(pFont);
	pSheetId->writeStr(iRow, iCol, strText, pFormatBoldFont);
}


void CMiddleDlg::ExportPS96ResultWriteStrUnderline(libxl::Book* pBook, libxl::Sheet* pSheetId, int iRow, int iCol, CString strText)
{
	libxl::Format* pCellFormat = pSheetId->cellFormat(iRow, iCol);
	libxl::Format* pFormatBoldFont = pBook->addFormat(pCellFormat);
	libxl::Font* pFont = pBook->addFont(pCellFormat->font());
	pFont->setUnderline(libxl::UNDERLINE_SINGLE);
	pFormatBoldFont->setFont(pFont);
	pSheetId->writeStr(iRow, iCol, strText, pFormatBoldFont);
}

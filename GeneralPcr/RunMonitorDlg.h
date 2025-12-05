#pragma once

#include "MySamPlateDlg.h"
#include "RunStateDlg.h"
#include "MyButton.h"
#include "MyDrawCurveDlg.h"
#include "MyStatic.h"
#include "MySegDlg.h"
#include "ExperimentData.h"
#include "LegendInfo.h"
#include "RectManage.h"

class CMiddleDlg;

typedef enum _RUNMONITOR_SONPAGETYPE
{
	ERST_RUNSEGDLG = 1,ERST_TEMCRV
}ERUNMONITORSONPAGETYPE;

typedef enum _RUNMONITOR_AMLMELTPAGETYPE
{
	ERAMPT_ORIGINAML = 1,ERAMPT_ORIGINMELT
}ERUNMONITOR_AMLMELTPAGETYPE;

class CRunMonitorDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CRunMonitorDlg)

public:
	CRunMonitorDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRunMonitorDlg();

// Dialog Data
	enum { IDD = IDD_RUNMONITORDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedSwtBtRunSegment();
	afx_msg void OnBnClickedSwtBtTemCurve();
	afx_msg void OnBnClickedSwtBtOriginAml();
	afx_msg void OnBnClickedSwtBtOriginMelt();
	afx_msg LRESULT OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam);

	virtual BOOL OnInitDialog();

			void InitDlg();
			void InitSegLegend();

			void InitSonDlg();
			void InitRunStateDlg();

			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateSeg();
			void CreateSamPlateDlg();
			void InitCrv();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitSeg();
			void InitSamPlateDlg();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			void SetCrvLineParam(vector<CTubeInfo>& vecTubeInfo);	//设置曲线控件线条参数
			void SetCrvLineParam(vector<CTubeInfo*>& vecPTubeInfo);
			void SetCrvLineParam(CTubeInfo* pTubeInfo,int iSiteNum);

			void UpdateCurveSelectState(BOOL bRedraw = FALSE);

			void SetPageSelected(ERUNMONITORSONPAGETYPE sonPageType);
			void SetPageSelected(ERUNMONITOR_AMLMELTPAGETYPE sonPageType);

			void ClearPage(BOOL bRedraw = TRUE);
			void ClearExperiment(BOOL bRedraw = TRUE);

			int FileOpened(CExperimentData* pExpData = NULL,BOOL bTemplate = FALSE);	//打开文件后设置监控页面
			int ReAnalysed(CExperimentData* pExpData = NULL);	//重新分析后设置监控页面

			int SetCurveData(CExperimentData* pExpData,BOOL bRedraw = FALSE);
			int SetTemperatureData(CExperimentData* pExpData,BOOL bRedraw);

			int CurrentSegmentStepChangedOnRun(int iCycle,int iSeg,int iStep);

			void SetCurveHoriAxisParam(int iStart,int iEnd,int iHoriStep,BOOL bRedraw = TRUE);

			int ResetCurveMaxMinValue(BOOL bRedraw);	//从新计算曲线控件最大最小值

			int RefreshLanguageDisplay();

			int CycleAddSuccessful(int iSegId,int iCount);
			void ShowRemainTimeOnce();

public:
	CMySamPalteDlg m_samPlateDlg;
	CMySegDlg m_segDlg;

	CRunStateDlg m_runStateDlg;

	CMyDrawCurveDlg m_crvAmlOri;
	CMyDrawCurveDlg m_crvMeltOri;	
	CMyDrawCurveDlg m_crvTem;

	CMyButton m_swtBtAmlOri;	//20220622，切换为显示DeltaRn数据，孙文郎
	CMyButton m_swtBtMeltOri;
	CMyButton m_swtBtSegMonitor;
	CMyButton m_swtBtTemMonitor;

	CMyStatic m_stLidTem;
	CMyStatic m_stTubeTem;

	CMiddleDlg* m_pParent;

	CMyButton* m_pCurSelAmlMelt;
	CWnd* m_pCurSelAMDlg;

	CMyButton* m_pCurSelSwt;
	CWnd* m_pCurSelDlg;

	ERUNMONITORSONPAGETYPE m_curSonPageType;
	ERUNMONITOR_AMLMELTPAGETYPE  m_eCurAmlMeltPageType;

	vector<int> m_vecPlateSelectedSite;	//记录孔板选中孔位下标

	CLegendInfo m_segLgdInfo;	//样本信息控件图例

private:
	BOOL m_bCtrlCreated;

	int m_iSwitchBtBkH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtBkW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值
	int m_iSwitchBtW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值

	BOOL m_bInstruHaveLid;	//标识仪器是否有热盖，没有热盖的话，不显示热盖温度
	CRect m_rcLidInstruRegion;	//标识 热盖、仪器温度图例区域，刷新页面时使用

	CRectManage m_rcManage;
};



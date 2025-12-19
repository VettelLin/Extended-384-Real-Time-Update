#pragma once

#include "MySamPlateDlg.h"
#include "SamLst.h"
#include "SamInfoDlg.h"
#include "SelfDefType.h"
#include "ExperimentData.h"
#include "LegendInfo.h"
#include "EditModeAbs.h"
#include "RectManage.h"
#include "InterfaceC.h"
#include "CYSamInfoDlg.h"

typedef enum _ESamDisplayMode
{
	SAMPLATE = 1,SAMCHART
}ESAMDISPALYMODE;

class CMiddleDlg;

class CSamSetDlg : public CDialogEx,public CFontSet,public CEditModeAbs,public CInterfaceC
{
	DECLARE_DYNAMIC(CSamSetDlg)

public:
	CSamSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamSetDlg();

// Dialog Data
	enum { IDD = IDD_SAMSETDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedSwtBtPlate();
	afx_msg void OnBnClickedSwtBtChart();
	afx_msg LRESULT OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgLstSelectChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgSamplateDeleteKeyPressed(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();

			void InitDlg();
			void InitSegLegend();
			void InitSonDlg();

			void CreateCtrl();
			void CreateBt();
			void CreateLst();
			void CreateSamPlateDlg();
			void InitCtrl();
			void InitBt();
			void InitLst();
			void InitSamPlateDlg();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			void SetPageSelected(ESAMDISPALYMODE eSamDisplayMode);

			int SetRowColNum(int iRowNum,int iColNum);

			void ReSetLstSamInfo(vector<CTubeInfo*>& vecSamInfoPtr,BOOL bRedraw = FALSE);
			void ReSetLstSamInfo(vector<CTubeInfo>& vecSamInfo,BOOL bRedraw = FALSE);
			void ReSetLstSamInfo(BOOL bRedraw = FALSE);

			int SetLstSamInfo(int index,CTubeInfo& tubeInfo);
			int SetLstSamInfo(int index,CTubeInfo& tubeInfo,EITEMINFOTYPE eItemInfoType);

			int LstSamLanguageChanged();

	virtual void SetPlateSelectedSiteSamInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData = 0,double dData = 0);
			int CYSamInfoChangedProcess(EITEMINFOTYPE eItemInfoType,CString strData,int intData = 0,double dData = 0);		

			void AutoSetPlateSelectedSiteSamId(CSamNameAutoSetInfo samNameAutoSetInfo);
			void AutoSetPlateSelectedSiteCalibratorDensity(CCalibAutoSetInfo calibAutoSetInfo);

			int ReSetTubeInfo(vector<int> &vecPlateSelectedSite,vector<CTubeInfo> &vecTubeInfo);	//选择项目页面样本信息改变后，同步到样本登记页面
			int ResetProcSetDlgTubeInfo();

			void ClearPage(BOOL bRedraw = TRUE);
			int FileOpened(CExperimentData* pExpData = NULL);

			int DeleteSelectedSampleInfo();	//删除选中孔位的样本信息

			void ResetLstTitle();
			void ResetLstColumnWidth();

			int RefreshLanguageDisplay();

	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);

public:
	CMySamPalteDlg m_samPlateDlg;
	CSamLst m_lstSamInfo;

#ifdef CAN_YOUNG
	CCYSamInfoDlg m_samInfoDlg;
#else
	CSamInfoDlg m_samInfoDlg;
#endif
	
	CMyButton m_swtBtPlate;
	CMyButton m_swtBtChart;

	CMiddleDlg* m_pParent;

	vector<int> m_vecPlateSamIndex;	//存储列表框每行对应的孔位下标（正向正序）

	vector<int> m_vecCurSelSite;	//记录当前选择的孔位

	CLegendInfo m_segLgdInfo;	//样本信息控件图例

private:
	BOOL m_bCtrlCreated;

	CMyButton* m_pCurSelSwt;
	CWnd* m_pCurSelWnd;

	int m_iSwitchBtBkH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtH;	//记录页面切换按钮外部圆角背景高度，确保高度是个偶数值
	int m_iSwitchBtBkW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值
	int m_iSwitchBtW;	//记录页面切换按钮外部圆角背景宽度，确保高度是个偶数值

	CRectManage m_rcManage;
};

#include "MiddleDlg.h"













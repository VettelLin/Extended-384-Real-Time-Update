#pragma once

#include "MyLstCtrl.h"
#include "MyButton.h"
#include "MyNewCb.h"
#include "MyNewEdit.h"
#include "MySamPlateDlg.h"
#include "MyDrawCurveDlg.h"
#include "ExperimentData.h"
#include "LegendInfo.h"
#include "StdProc.h"
#include "RectManage.h"

class CMiddleDlg;

typedef enum _AmlOriginalCurveType
{
	EAOCT_AMLCURVE = 1,EAOCT_ORIGINALCURVE
}EAMLORIGINALCURVETYPE;

typedef enum _MeltOriginalCurveType
{
	EMOCT_MELTPEAKCURVE = 1,EMOCT_ORIGINALMELTCRV
}EMELTORIGINALCURVETYPE;

class CDataAnaDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CDataAnaDlg)

public:
	CDataAnaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDataAnaDlg();

// Dialog Data
	enum { IDD = IDD_DATAANADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtAxisZoom();
	afx_msg void OnBnClickedBtAxisZoomSecond();
	afx_msg LRESULT OnMsgPlateSiteSelectChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentSetNewText(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgThreChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgTellParentTextChanged(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam);

	virtual BOOL OnInitDialog();

			void InitDlg();
			void InitSegLegend();
			void InitSonDlg();

			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateCb();
			void CreateEd();
			void CreateLst();
			void CreateSamPlateDlg();
			void InitCrv();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitCb();
			void InitEd();
			void InitLst();
			void InitSamPlateDlg();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			void SetCrvLineParam(vector<CTubeInfo>& vecTubeInfo);
			void SetCrvLineParam(vector<CTubeInfo*>& vecPTubeInfo);

			int SetCrvThreLineInfo();	//设置曲线控件阈值信息
			int RenewNormalizeCrvThreLineInfo(BOOL bRedraw = FALSE);	//更新归一化曲线控件阈值信息
			int RenewDeltaRnCrvThreLineInfo(BOOL bRedraw = FALSE);	//更新DeltaRn曲线控件阈值信息

			int ThreLineModified();	//在基本参数设置窗口中修改了手动阈值

			void UpdateCurveSelectState(BOOL bRedraw = FALSE);	//更新曲线、阈值线显示状态，

			int CurveAxisEdit(CMyDrawCurveDlg* pCrvCtrl,BOOL bHaveLogFunc = FALSE,BOOL bLogMode = FALSE);

			int SetBaseSet();	//编辑基本分析参数信息
			int SetMotChannelAdjust();	//编辑浊度校准参数信息

			void ClearPage(BOOL bRedraw = TRUE);
			int FileOpened(CExperimentData* pExpData = NULL);	//打开文件后设置分析页面
			int ExperimentOver(CExperimentData* pExpData = NULL);	//实验结束后设置分析页面
			int ReAnalysed(CExperimentData* pExpData = NULL);	//重新分析后设置分析页面

			int SetCurveData(CExperimentData* pExpData,BOOL bRedraw = FALSE);	//设置曲线控件中的曲线数据
			int SetMeltCurveData(CExperimentData* pExpData,BOOL bRedraw = FALSE);

			int SetNormalizeCurveData(CExperimentData* pExpData,BOOL bRedraw = FALSE);	//设置归一化曲线控件中的曲线数据
			int SetDeltaRnCurveData(CExperimentData* pExpData,BOOL bRedraw = FALSE);	//设置DeltaRn曲线控件中的曲线数据
			int SetFilterCurveData(CExperimentData* pExpData,BOOL bRedraw = FALSE);	//设置滤波后的原始数据曲线控件中的曲线数据

			void SetCurveHoriAxisParam(int iStart,int iEnd,int iHoriStep,BOOL bRedraw = TRUE);

			int DataProcess(BOOL bNewOver,int iAmlFlag = 5);	//数据分析

			void ResetResultLst();	//重新设置结果列表框内容
			void LstResultLanguageChanged();	//结果列表语言改变了

			void AddOneResult(CTubeInfo& tubeInfo,int iChanIndex,CString strCt,CString strTm = _T(""));

			int NewResultCalculated(int iChanId);	//通道结果信息（Ct值、Tm值）改变后，更新结果列表信息

			int ExportPicture();	//导出图片
			
			int AmlDataShowTypeChanged(int iNewIndex);	//m_cbSelCrvToShow选中项更改了

			int CheckAmlDataShowType();	//打开文件时，根据文件里的分析参数设置当前是查看归一化还是DeltaRn数据

			int StdLineProcess(CExperimentData* pExpData);	//计算所有通道的标曲
			int StdLineProcess(CExperimentData* pExpData,int iChanId);

			CString GetCtTitle();
			CString GetAverageCtTitle();

			int CbSelectCrvToShowChanged();	//下拉选择看选择项改变了
			int CbSelectChannelThresholdChanged();	//下拉选择看选择项改变了

			int ShowThreValue();	//打开文件、重新分析、实验结束后更新阈值显示

			int ResetCurveMaxMinValue(BOOL bRedraw);	//从新计算曲线控件最大最小值

			CExperimentData* GetExpDataPtr();

			void SetCbSelectChanThreString();	//设置当前显示阈值通道名称集
			
			int SetOneChannelDyeName(int iChanId,CString strNewDyeName);

			int NormalizeLogModeChanged();	//归一化数据对数坐标模式选项改变了
			int DeltaRnLogModeChanged();	//DeltaRn数据对数坐标模式选项改变了

			int RecordDefaultAxisInfo();	//记录每个曲线控件的默认值

			int AdjudicateAmpMeltWhichToShow();	//实验结束或者打开文件时，判断显示扩增类型数据还是熔曲类型数据

			int RefreshTheChannelToShowThre(int iBtIndex,BOOL bChecked);	//刷新需要显示阈值的通道（发生于通道选中状态改变）

			int UploadInspectResultToLis();	//上传数据到LIS系统
			int SendDataByTcpIp(vector<CStringA>& vecLisInfo);

			void ResetLstTitle();
			void ResetLstColumnWidth();

			int RefreshLanguageDisplay();

public:
	int GetCtDataType() { return m_iPreCbSelIndex; }

public:
	CMyButton m_btAxisZoom;	
	CMyButton m_btAxisZoom2nd;

	CMyNewCb m_cbSelCrvToShow;
	CMyNewCb m_cbSelChanThre;

	CMyNewEdit m_edThreValue;

	CMyDrawCurveDlg m_crvAml;	//归一化数据
	CMyDrawCurveDlg m_crvAmlDelRn;	//DeltaRn数据
	CMyDrawCurveDlg m_crvOriginAml;//lzh:原始增容曲线的对象
	CMyDrawCurveDlg m_crvPeakMelt;
	CMyDrawCurveDlg m_crvOriginMelt;
	CMyDrawCurveDlg m_crvStd; 

	CMyLstCtrl m_lstAnaResult;

	CMySamPalteDlg m_samPlateDlg;

	CMiddleDlg* m_pParent;

	vector<BOOL> m_vecPlateSelectState;	//记录所有孔位的选中状态
	vector<int> m_vecPlateSelectedSite;	//记录孔板选中孔位下标

	CMyDrawCurveDlg* m_pCurSelCrvToShow;	//当前要显示曲线控件

	CLegendInfo m_segLgdInfo;	//样本信息控件图例

	CStdProc m_stdProc;

private:
	BOOL m_bCtrlCreated;
	int m_iPreCbSelIndex;	//记录曲线类型下拉选择窗口上一个选择项

	CRect m_rcThresholdText;	//记录“阈值”文本的显示区域

	CRectManage m_rcManage;
};


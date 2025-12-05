#pragma once

// CMiddleDlg dialog

#include "MyButton.h"
#include "BaseSetDlg.h"
#include "ProcSetDlg.h"
#include "SamSetDlg.h"
#include "RunMonitorDlg.h"
#include "DataAnaDlg.h"
#include "ExpRunCtrl.h"
#include "RectManage.h"
#include "ExperimentData.h"
#include "LinkList.h"
#include "libxl.h"
#include "EditModeType.h"
#include "RunProgressRecord.h"
#include "SamInfoExportMode.h"
#include "FuncBtDlg.h"

typedef enum Son_Page_Type
{
	ESPT_BASEINFODLG = 1,ESPT_PROCSET,ESPT_SAMSET,ESPT_RUNMONITOR,ESPT_DATAANALYSE
}ESON_PAGE_TYPE;

class CMiddleDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMiddleDlg)

public:
	CMiddleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMiddleDlg();

// Dialog Data
	enum {IDD = IDD_MIDDLEDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual afx_msg BOOL OnEraseBkgnd(CDC* pDC);
			afx_msg void OnSize(UINT nType, int cx, int cy);
			afx_msg void OnTimer(UINT_PTR nIDEvent);

	virtual BOOL OnInitDialog();
	virtual	void CreateCtrl();
			void InitCtrl();
	virtual	void SetCtrlPos();
	virtual	void ShowCtrl();
			void ShowBt();
	virtual	void InitSonDlg();
			void InitBaseSetDlg();
			void InitSelectPrjDlg();
			void InitSamSetDlg();
			void InitRunMonitroDlg();
			void InitDataAnaDlg();

	virtual	void SetCtrlText();

	afx_msg void OnBnClickedSwtBaseSet();
	afx_msg void OnBnClickedSwtSelectProject();
	afx_msg void OnBnClickedSwtSamSet();
	afx_msg void OnBnClickedSwtRunMonitor();
	afx_msg void OnBnClickedSwtDataAnalyse();

	afx_msg void OnBnClickedSwtBtRunStop();

	afx_msg LRESULT OnMsgExperFluChanged(WPARAM wParam,LPARAM lParam);
	virtual afx_msg LRESULT OnMsgExperRun(WPARAM wParam,LPARAM lParam);
	virtual afx_msg LRESULT OnMsgExperFinished(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgSegmentOver(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgMeltOneTemOver(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgExperTempChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgExperStepChanged(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgRunPowerOffProtect(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnMsgExperimentDisconnect(WPARAM wParam,LPARAM lParam);

	afx_msg LRESULT OnMsgInstrumentSelected(WPARAM wParam,LPARAM lParam);

	void SetPageSelected(ESON_PAGE_TYPE eSonPageType,BOOL bInit = FALSE);

			void UpdateFuncBtMode();
			
			void SetPageId(int pageId);
			int GetPageId();
			int Save();
			BOOL SaveAsFile(BOOL bTemplate);
			int Delete();

			int ExportCompleteInfo(CString strFilePath);	//导出完整实验数据及信息,bNoData:标识是否只导出结果报告和仪器信息
			int ExportTemperature(CString strFilePath = _T(""));	//导出温度

			int ExportPS96ResultSheet(libxl::Book* pBook);
			int ExprotPS96OldResultSheet(libxl::Book* pBook);
			int ExportResultSheet(libxl::Book* pBook);

			int ExportInstrumentInfoSheet(libxl::Book* pBook);
			int ExportTubeInfoSheet(libxl::Book* pBook,ESAMINFOEXPORTMODE eSamExportMode);
			int ExportAmlDataSheet(libxl::Book* pBook,ESAMINFOEXPORTMODE eSamExportMode);

			int Analyse();
			int SetCrossParam();
			int SetFilterParam();
			int SetBaseSet();
			int SetMotChannelAdjust();
			int SetMeltAnalyseParam();	//编辑熔曲分析参数信息

			int SaveProject(int iIndex,BOOL bNew);	//项目已被保存，更新到实验页面
			int DeleteProject(int iIndex);	//删除一个项目
			void SetDefaultGeneName();

			void SynchronizeExperimentSet();	//同步实验设置信息
			void SynchronizeSampleSet();
			void SynchronizeExperimentSetOnReAnalyse();	//同步实验设置信息，重分析功能使用

	virtual	int Run();	//运行
	virtual	int RunPowerOffProtectExperiment();	//运行断电保护实验
	virtual	int Stop();	//停止

	virtual int RefreshExpDlgRunState(BOOL bOccupy);

			int SetInstrumentLidTem(int iLidtem);	//设置仪器EEPROM里的热盖温度
			int ReadInstrumentLidTem(double& dLidtem);	//读取仪器EEPROM里的热盖温度

			int SetInstrumentLidPreheatTem(int iLidtem);	//设置仪器EEPROM里的热盖预热温度
			int ReadInstrumentLidPreheatTem(float& fLidtem);	//读取仪器EEPROM里的热盖预热温度

			virtual void InitExperimentData(CExperimentData* pExpData);

			int Export();	//导出功能
			int ExportPicture();
			int Import();	//导入功能
			int ExportResult();
			int ExportResultReport(CString strFilename, CString strFilename2);
			int ExportResultReportToCSV(CString strFileExcel, CString strFilCSV);

			double GetAverageLidTem();
			double GetAveragePeltiaTem();
			double PeltiaTemAmend(double dTem,double dAmendScope = 3.0);
			double LidTemAmend(double dTem,double dAmendScope = 3.0);

			int InitFluBackUp();	//初始化“保存实验荧光数据”excel文件

			void ClearBackupFileParam();	//清空荧光数据备份文件（excel文件）相关参数

	virtual	int SetEditMode(EEXPEDITMODE eExpEditMode);	//设置编辑模式，无限制模式、IVD模式

			int SampleDyeNameChanged(int iChanId,CString strNewDyeName);	//样本的某个通道的染料名称手动变更了，更新“通道切换按钮标识”和“分析页面阈值对应通道标识”

			void ClearExperiment();	//清空实验信息

			void FileOpened();	//打开了一个新的实验文件或者模板

	virtual void SetTemplateListSelectedRow(int iIndex);	//设置模板文件列表选中行

	virtual	int NewTemplateAdded(int iIndex);	//新的模板文件成功添加

			BOOL GetBeExportData();	//导出实验数据时，判断是否导出荧光信号相关信息

			BOOL IsExpRunning();

			int SetGradientFuncShieldState(BOOL bData);	//设置梯度功能屏蔽状态
			int SetTemperatureRateShowState(BOOL bData);

			void SetPowerOffExpInfo(CRunProgressRecord runPrgRcd);

			int GetAllSampleType(vector<eSampleType>& vecSamType);	//按下标顺序获取孔板的样本类型信息

			int UploadInspectResultToLis();	//上传检测结果到LIS 系统

			int RefreshLanguageDisplay();

			int CycleAddSuccessful(int iSegId,int iCount);

			int RefreshExpTotalTime(int iTotalSec);

			void SetCurveHoriAxisParam();

	virtual void SetExpDataPtr(CExperimentData* pExpData);
	virtual void EmbodyBaseSetInfo();	//显示实验文件中的基本设置信息
	virtual	void RefreshExpEmbody();
	virtual	void DeleteTemplateListRow(int iIndex);

	BOOL GetBeDataAnaDlgVisible();
	int ResetTubeInfo(vector<int> &vecPlateSelectedSite,vector<CTubeInfo> &vecTubeInfo, int iFlag);	//选择项目页面样本信息改变后，同步到样本登记页面

	void SetChannelDyeBtn();
	afx_msg LRESULT OnMsgChanBtClicked(WPARAM wParam,LPARAM lParam);
	virtual void SetConnectState(BOOL bRunExp);
	void SetRunBtnState(BOOL bRunning);

	void SetInstrumentConnectState(CString strModel, CString strSN, BOOL bConnected);
	virtual void ReshowInstrumentList();
	virtual void SetSelectedInstrument(CString strModel, CString strSN);

	void ExportPS96ResultSheetFormat(libxl::Book* pBook, libxl::Sheet* pSheetId);
	void ExportPS96ResultSheetRectBorder(libxl::Book* pBook, libxl::Sheet* pSheetId, int iStartRow, int iStartCol, int iEndRow, int iEndCol);
	void ExportPS96ResultWriteStrBold(libxl::Book* pBook, libxl::Sheet* pSheetId, int iRow, int iCol, CString strText, int iFontSize=-1);
	void ExportPS96ResultWriteStrUnderline(libxl::Book* pBook, libxl::Sheet* pSheetId, int iRow, int iCol, CString strText);

private:
			int ExportMeltData(libxl::Book* pBook);	//导出熔曲数据

public: 
	int	m_iFunBtnHeight;
	CFuncBtDlg m_funcBtDlg;

	CMyButton m_swtBaseSet;
	CMyButton m_swtSelectPrj;
	CMyButton m_swtSamSet;
	CMyButton m_swtRunMonitor;
	CMyButton m_swtDataAna;

	CMyButton m_btnRunStop; 
	CMyStatic m_stcConnectState;
	CRect	m_rectConntectState;

	CMyButton* m_pCurSelSwt;
	CDialogEx* m_pCurSelDlg;


	ESON_PAGE_TYPE m_curSelSonPage;

	int m_pageId;

	CExperimentData* m_pExpData;	//创建窗口后保实验数据对象指针，方便操作

	CLinkList<double> m_lnkLidTem;	//记录热盖温度，滤波使用
	CLinkList<double> m_lnkPeltiaTem;	//记录帕尔贴温度，滤波使用

	int m_iCurCycle;
	int m_iCurSeg;
	int m_iCurStep;

	CExpRunCtrl* m_pRunCtrl; // 实验运行线程

protected:
	CBaseSetDlg m_baseSetDlg;
	CProcSetDlg m_selPrjDlg;
	CSamSetDlg m_samSetDlg;
	CRunMonitorDlg m_runMonitorDlg;
	CDataAnaDlg m_dataAnaDlg;

	BOOL	m_bExpRunning;
	BOOL	m_bCtrlCreated;

	BOOL	m_bInstruConnected;

protected:

	//保存荧光备份数据
	libxl::Book* m_pBook;
	libxl::Sheet** m_ppSheet;
	CString m_strBackupFilePath;	//保存备份文件的完整路径名

	CRunProgressRecord m_runRrgRcd;	//运行断电保护实验时，传入的断电保护实验信息
	BOOL m_bPowerOffExp;	//标识是否是断电保护实验 继续运行的实验


	int m_iColCount;
	int m_iStartCol;
	int m_iFirstPaneStart; // 第1区域行数
	int m_iFirstPaneEnd;
	int m_iSecondPaneStart; 
	int m_iSecondPaneEnd; 
	int m_iThirdPaneStart; 
	int m_iThirdPaneEnd; 
	int m_iFourthPaneStart; 
	int m_iFourthPaneEnd; 
};





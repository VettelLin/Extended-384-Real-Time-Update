#pragma once

#include "ExperimentInfo.h"
#include "ReagentItem.h"
#include "PlateTubeInfo.h"
#include "PlateTubeData.h"
#include "TemperatureData.h"
#include "DataAnalysisPCR.h"
#include "CalibrateParaTurbidity.h"
#include "DataAnalysisPara.h"
#include "ConfigureDBInfo.h"
#include "HardwareInfo.h"
#include "SamIdSiteRelation.h"
#include "ExpStateInfo.h"
#include "InstancePCR.h"
#include "AxisInfo.h"
#include "VolumeDelayParam.h"

class AFX_EXT_CLASS CExperimentData
{
public:
	CExperimentData(void);
	~CExperimentData(void);

	struct tagAddCycleInfo
	{
		int iSegmentNo;
		int iAddNum;
	};

	enum eDataStatus
	{
		STATUS_NEW = 0, // 新建实验
		STATUS_SAVE, // 来源于保存文件
		STATUS_SAVE_DATA, // 来源于保存文件，有实验数据，不能进行新实验或者修改初孔板外的其他设置
		STATUS_CHANGE, // 内容修改
		STATUS_USING, // 实验执行过程中设置，不能修改实验设置
		STATUS_PLATE, // 修改孔板设置，方便分析
	};

// Operations
public:
	void DeleteRunData(BOOL bOnlyData = TRUE);
	void DeleteMeltData(BOOL bOnlyData = TRUE);
	BOOL SaveFile(int iFile=0);
	BOOL SaveFile(CString strFilePath,BOOL bTemplate = FALSE);
	int OpenFile(CString strFilePath, BOOL bTemplate = FALSE);
	BOOL HasRunData();
	void InitDyeChannelInfo(CDyeInfoList* pListDye, CPtrArray* pArrayChannel);

	//设置样本每个通道默认颜色值
	int SetTubeDefaultTargetColor();	

	//获取通道当前颜色值
	COLORREF GetChannelColor(int iChanId);

	//设置样本每个通道的默认染料名
	int SetTubeDefaultDyeName();

	//获取每个通道的默认染料名，应该跟仪器型号关联，CExperimentData又包含了仪器型号信息
	vector<CString>& GetDefaultDyeName();

	//保存、读取步骤信息
	BOOL WriteSegmentInfo(CFile* pFile);
	BOOL ReadSegmentInfo(CFile* pFile,WORD wVersion);

	// 保存、读取染料通道信息
	BOOL WriteDyeInfo(CFile* pFile);
	BOOL ReadDyeInfo(CFile* pFile,WORD wVersion);

	//保存、读取图形控件的X/Y最大最小值
	BOOL WriteDefaultAxisInfo(CFile* pFile);
	BOOL ReadDefaultAxisInfo(CFile* pFile,WORD wVersion);

	// 试剂项目相关参数设置
	void PasteReagentItemInfo(CReagentItem* pInputItem);
	// 根据项目参数初始化选择通道
	void InitDyeTargetInfo(int iTubeID);

	// 温度相关函数
	int GetTempChannelCount();
	void SetTempChannelCount(int iCount);
	void AddTempData(DWORD dwTime,int nTempCount,UINT* pInputTemp,BOOL bMeltStepRunning); // 增加一次温度值
	UINT GetCurTemp(CTemperatureData::eTempChannel nTempChannel, int iChannelNo=0); // 取得最近PCR仪温度
	DWORD GetCurTempCollectTime();
	void SetLidHeater(BOOL bUseLid);
	void SetTempTarget(float fTemp1, float fTemp2, float fTemp3) { return m_pTempData->SetChannelTarget(fTemp1, fTemp2, fTemp3); } 
	int GetTempDataCount();
	void GetTempTimeData(int iDataCount, double* pdValueTime);
	void GetTempChannelData(CTemperatureData::eTempChannel nTempChannel, int iChannelNo, int iDataCount, double* pdValueTemp);

	CTemperatureData* GetTemperatureData();

	// 添加扩增段和熔解段的原始数据
	void AddAmplifyCycleX(CUIntArray* pArrayCycle);
	int AddAmplifyFluOriginalData(CUIntArray& aryOriData,int chanId);
	void AddMeltCycleX(CUIntArray* pArrayCycle);
	int AddMeltFluOriginalData(CUIntArray& aryOriData,int chanId,double dTem);
	void SetScanStartPos(eScanStartPos nPos);

	// 取荧光数据，扩增段和熔解段
	int GetAmplifyDataNum();
	UINT GetCurrentAmplifyCycleX(int iTubeID);
	void GetAmplifyCycleX(int iTubeID, int iCycleCount, double* pdValueX);
	void GetAmplifyChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdValueY);
	double GetAmplifyChannelCycleFluY(int iTubeID, eDataType nType, int iChannel, int iCycleId);	//获取特定通道单个孔位在某个循环的荧光信号值
	
	//获取熔曲数据点个数
	int GetMeltDataNum();
	void GetMeltCycleX(int iTubeID, int iCycleCount, double* pdValueX);
	double GetMeltCycleX(int iTubeID, int iCycleId);	//获取特定通道单个孔位熔曲数据在某个循环的温度值
	void GetMeltChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdValueY);
	double GetMeltChannelCycleFluY(int iTubeID, eDataType nType, int iChannel, int iCycleId);

	// 扩增段、熔解段荧光数据处理
	BOOL ProcessAmplifyFluData(int nFlag = 5);  // 处理扩增数据，进行通道荧光补偿、滤波、浊度调整
	BOOL ProcessMeltFluData(); // 处理熔解段荧光数据
	BOOL MonitorProcessMeltFluData(int iPtCount);

	void GenerateCt(BOOL bCalcDefThre,eMethodCalculateCt nMethod=METHOD_CT_USER, eDataType nDataType=FLU_NORMALIZATE);
	void GenerateOneChannelCt(eMethodCalculateCt nMethod, eDataType nDataType,int iChanId);	//计算单个通道的阈值信息

	// 配置参数
	void InitAlgorithmPara(CString strIniFile); // 初始化算法默认参数
	BOOL InitCrosstalkPara(CString strIniFile); // 从配置文件中读取默认的串扰参数
	BOOL InitAmpCrosstalkParam(float** ppFData,int iChanNum);
	BOOL InitMeltCrosstalkParam(float** ppFData,int iChanNum);
	BOOL InitFilterInfo(CString strIniFile); // 从配置文件中读取默认的滤波参数
	BOOL InitFilterInfo(vector<tagFilterItem>& vecFlt);

	// 增加循环数
	int AddCycle(int iSegment,int iCount);

	void SetTubeInfo(vector<CTubeInfo>& vecTubeInfo);
	void SetTubeInfo(vector<CTubeInfo*>& vecPTubeInfo);
	int SetTubeInfo(CTubeInfo* pTotalTubeInfo,int iTubeNum);

	void GetTubeInfo(vector<CTubeInfo>& vecTubeInfo);
	void GetAllTubeInfoPtr(vector<CTubeInfo*>& vecPTubeInfo);
	CPtrArray* GetTubeInfoArrayPtr();
	CTubeInfo* GetTubeInfoAt(int iIndex);	//获取第(iIndex + 1)个样本信息指针

	// 实验运行前判断数据参数设置是否设置和合理，否则初始化为默认值
	BOOL CheckParaBeforeRun(CString strStartupPath);

	int GetFirstEndCollectCycleNum();	//获取流程信息中第一个终点采集步骤所在程序段的循环数
	int GetFirstMeltCollectStartAndEndTemperature(double& dTemStart,double& dTemEnd);	//获取流程信息中第一个熔曲采集步骤的起始温度和终止温度

	void SetMonitorChannelBtSelectState(vector<int> vecData);
	vector<int> GetMonitorChannelBtSelectState();	

	void SetDataAnaChannelBtSelectState(vector<int> vecData);
	vector<int> GetDataAnaChannelBtSelectState();

	void SetMonitorChannelBtSelectState(int iChanId,int iSelState);	//设置监控页面单个通道切换按钮选中状态，iChanId从0开始
	int GetMonitorChannelBtSelectState(int iChanId);	

	void SetDataAnaChannelBtSelectState(int iChanId,int iSelState);	//设置分析页面单个通道切换按钮选中状态，iChanId从0开始
	int GetDataAnaChannelBtSelectState(int iChanId);

	void SetMonitorChannelBtAllSelected();	//设置监控页面通道选择按钮为全选状态
	void SetDataAnaChannelBtAllSelected();	//设置分析页面通道选择按钮为全选状态

	void ResetMonitorChannelBtSelectState();	//清空监控页面通道选择按钮的选中状态
	void ResetDataAnaChannelBtSelectState();	//清空分析页面通道选择按钮的选中状态

	void SetChannelDyeName(vector<CString>& vecChanDyeName);	//设置通道切换按钮的染料名称标识
	void SetChannelDyeNameIfEmpty(vector<CString>& vecChanDyeName);
	vector<CString> GetChannelDyeName();
	BOOL GetBeValidChannelDyeName();
	CString GetOneChannelDyeName(int iChanId);
	void SetOneChannelDyeName(int iChanId,CString strChanDyeName);
	int GetChannelDyeNameFromTubeInfo();	//从样本信息中获取通道切换按钮的染料名称标识

	int GetRound(double r);	//四舍五入功能

	// 数据导入导出功能
	int InputSampleSetInfo(vector<eSampleType>* pVecSamType = NULL); // 导入样本设置信息
	BOOL ExportSampleSetInfo(int iFlag = 0,int iIdVersion = 0); // 导出样本设置信息

	int GetFilterCount();	//获取滤波参数个数
	tagFilterItem* GetFilterBy(int iIndex);	//获取第iIndex + 1个滤波参数

	eSplineThresholdType GetSplineThresholdType();

	int GetRowCount();	//获取孔板行数
	int GetColCount();	//获取孔板列数

	BOOL CheckSegmentExistDataCollect();
	BOOL IsExistAmpSegmentLessThanCycleNum(int iCycleNum);

	CItemChannelInfo* GetReagentChanInfo(int iChanId);	//获取项目中的通道设置信息

	CString GetProjectName();	//获取项目名称

	void SetHardwareInfo(CHardwareInfo& info);
	CHardwareInfo GetHardwareInfo();

	CString GetStartTimeString();
	CString GetEndTimeString();
	CString GetEndTimeYMDString();
	CString GetTotalTimeString();
	CString GetExperName();
	CString GetExperSaveFolder();

	BOOL GetBePS96ExperFile();	//判断是否是PS96的实验文件
	BOOL GetBeXINYIM16ExperFile();	//判断是否是新羿M16的实验文件
	BOOL GetBeNQ16ExperFile();	//判断是否是NQ16的实验文件
	BOOL GetBeN96ExperFile();	//判断是否是N96的实验文件

	BOOL GetBeUseAvarageTemperature();	//判断“是否使用多通道平均温度作为仪器温度”

	BOOL GetBeNewExperFile();	//判断是否是新建的实验文件

	void SetNormalizeSelectLogState(BOOL bSelLogData);	//设置归一化数据对数模式状态信息
	BOOL GetNormalizeSelectLogState();

	void SetDeltaRnSelectLogState(BOOL bSelLogData);
	BOOL GetDeltaRnSelectLogState();

	void SetFilterSelectLogState(BOOL bSelLogData);
	BOOL GetFilterSelectLogState();

	int AutoTransformThresholdValue(int iDataType);	//对数模式选项改变后，自动转换归一化或者DeltaRn的阈值信息,iDataType,数据类型，0--归一化数据，1--DeltaRn数据

	int ReadOriginalDataFromExcel(CString strFileName,vector<double>* ppVecData);	//从Excel文件读取原始数据	

	int SetTubeData(vector<double>* pVecData,int iTubeNum,int iChanNum);	//设置样本数据信息

	int SetTubeInfoToBeAllProcessed();	//设置样本的默认信息（设置为未知品，通道全选），用于“从外部加载荧光数据后，能立即看到数据曲线”；

	void DeleteRunSegment();
	void DeleteTubeInfo();

	void GetSampleIdSiteIndexRelation(BOOL bHori);	//根据样本ID对样本孔位进行分类

	int GetResultFileDyeTargetTitle(vector<CString>& vecTitle);	//获取PS96导出结果时 Ct值所在列的标题（基因名组合-染料名称）

	int GetChannelIsSelectedByAtLeastOneTube(vector<BOOL>& vecChanSelState);	//获取是否有样本孔位选中某个通道

	BOOL IsSampleIdSet();	//获取是否有样本孔位设置了样本ID

	BOOL IsAllNoneSampleType();	//获取是否全部样本的样本类型都为空

	BOOL IsSampleIdSetAtExceptNoneSampleType();	//获取是否有样本孔位设置了样本ID，NONE样本类型除外

	int SetChannelCrossTalkParam(double* pCrosstalkPara,int iChanCount);

	CExpStateInfo GetExperimentStateInfo();	//获取实验状态信息

	double GetExperimentProgerss();	//获取实验进度信息

	BOOL GetGradientFuncShieldState();	//获取“是否屏蔽梯度功能”

	float GetFirstMeltStepStartTem();	//获取第一个熔曲步骤起始温度
	float GetFirstMeltStepEndTem();	//获取第一个熔曲步骤目标温度

	void InitAmplifyParamByTubeTarget();

protected:
	void Initialize();
	void DeleteAll();
	void DeleteDyePara();
	void DeleteCrosstalkPara();
	void DeleteFilterPara();

	int GenerateAutoSplineThresholdForChannel(eDataType nDataType,BOOL bAutoThreCopyToHandThre = FALSE);

	BOOL WriteLog(CFile* pFile);
	BOOL ReadLog(CFile* pFile,WORD wVersion);

// Attributes
public:
	CExperimentInfo* GetExperimentInfo() { return m_pExpInfo; }
	CReagentItem* GetReagentItem() { return m_pReagentItem; }
	CPlateTubeInfo* GetPlateInfo() { return m_pPlateInfo; }

	void SetSegment(CLinkList<CSegItemInfo>& lnkSegItem);
	CLinkList<CSegItemInfo>& GetSegment();

	void SetReachLidTemp(BOOL bReach) { m_bReachLidTemp = bReach; }
	void SetNotReachTempOnce(int iCount) { m_iNotReachTempCount = iCount;}

	int GetChannelCount() { return m_iChannelCount; }
	void SetChannelCount(int iCount, BOOL bOpenFile=FALSE); // 设置扫描通道数

	int SetTubeChannelSelectState(int iChanId,BOOL bSelected);	//设置所有样本的特定通道选择状态,20220518,孙文郎

	int GetDyeChannelCount() { return m_arrayDye.GetCount(); }
	tagDyeChannelInfo* GetDyeChannelInfo(int iIndex);

	float** GetCrosstalkPara() { return m_ppFCrosstalkPara; }
	float** GetMeltCrosstalkPara() { return m_ppFMeltCrosstalk; }

	void SetCalibrateMethod(eCalibrateMethod nMethod);
	BOOL GetBeUseTurbidityCalibration();	//判断是否使用浊度校准

	CCalibrateParaTurbidity* GetTurbidityPara() { return m_pTurbidityPara; }
	BOOL SaveCalibratePara();
	void SetCalibrateChannel(int iChannel);
	int GetCalibrateChannel();	//获取校准通道号，添加于20221019，孙文郎，不存在校准通道则返回-1

	eDataStatus GetStatus() { return m_nStatus; }
	void SetStatus(eDataStatus nStatus);

	int GetTubeCount();

	void SetAnalysisPara(CDataAnalysisPara *pAnaParam);
	CDataAnalysisPara* GetAnalysisPara() { return m_pAnalysisPara; }

	tagAnaAmplifyPara* GetAmplifyParaBy(int nIndex); 
	tagAnaMeltCurvePara* GetMeltAnaParamAt(int iIndex);

	void SetFileName(CString strFileName){m_strFileName = strFileName;}
	CString GetFileName(){return m_strFileName;}

	void SetFilePath(CString strFilePath){m_strFileOpenPath = strFilePath;}
	CString GetFilePath(){return m_strFileOpenPath;}

	CString GetBackupFileCreateTime(){return m_strBackupFileCreateTime;}

	int SetExpFileSaveDirectory(CString strDir);

	void CreateBackupFilePath(CString strStarupPath);	//创建备份实验文件
	
	void SetBeTemplateFile(BOOL bTemplateFile);
	BOOL GetBeTemplateFile();

	void SetIVDFileRstSaveFolder(CString strData);	
	CString GetIVDFileRstSaveFolder();

	void SetIVDFileRstSaveName(CString strData);	
	CString GetIVDFileRstSaveName();

	CString GetIVDResultFileSavePath();	//PS96IVD模式下，获取自动保存结果报告的路径名
	CString GetIVDResultFileSavePath2();

	void SetFileOpenPath(CString strData);	//设置文件打开路径
	CString GetFileOpenPath();

	void SetDefaultAxisCount(int iCount);	//设置默认坐标轴信息个数
	int GetDefaultAxisCount();
	int SetDefaultAxisInfoAt(int iIndex,CAxisInfo& axisInfo);
	CAxisInfo GetDefaultAxisInfoAt(int iIndex);

	int GetPreStepDelay(int iSeg,int iStep);	// 取得指定程序段和步骤的前面一步的等待时间
	float GetPreStepTemperature(int iSeg,int iStep);	// 取得指定程序段和步骤的前面一步的等待时间

	double GetStepVolumeDelaySec(double dStepTem);	//96孔QPCR获取和体积关联的额外延时时间（非熔曲步骤），单个步骤
	double GetTotalVolumeDelaySec(BOOL bIgnoreInstruType);	//96孔QPCR获取和体积关联的延时时间（非熔曲步骤），总时间

	void SetUserName(CString strData);
	CString GetUserName();

	void SetCYSampleCategory(CString strData);
	CString GetCYSampleCategory();

	void SetCYSampleCount(CString strData);
	CString GetCYSampleCount();

	void SetCYSampleState(CString strData);
	CString GetCYSampleState();

	void SetCYProductBatchNumber(CString strData);
	CString GetCYProductBatchNumber();

	void SetCYTestItem(CString strData);
	CString GetCYTestItem();

	void SetCYComment(CString strData);
	CString GetCYComment();

	void SetCYReportNo(CString strData);
	CString GetCYReportNo();

	void SetCYCollectDate(CString strData);
	CString GetCYCollectDate();

	void SetTester(CString strData);
	CString GetTester();

	void SetReviewer(CString strData);
	CString GetReviewer();

	// 语言切换时对程序段名进行修改
	void CorrectSegmentNameByLanguage(CString strSource, CString strTarget);

	void AddLog(CString strLog) { m_arrayLog.Add(strLog); }
	CString GetAbnormalLog();

	void SetTemplateNane(CString strName) { m_strTemplateName = strName; }
	CString GetTempateName() { return m_strTemplateName; }

	void SetExpRunning(BOOL bRunning) { m_bExpRunning = bRunning; }

	void SetReconnectState(int  iState) { m_iReconnectState = iState; }
	int GetReconnectState() { return m_iReconnectState; }
	void ConnInterruptOnce();
	int GetConnInterruptCount() { return m_iConnInterruptCount; }

	void SetHasZeroValues(BOOL bHas) { m_bHasZeroValues = bHas; } 

protected:
	WORD	m_wFileFlag; // 文件标识码
	WORD	m_wFileVersion; // 文件版本
	WORD	m_wSoftFileMaxVer;	//软件支持的最高版本文件

	CDataAnalysisPCR*	m_pAlgorithm; // 算法指针

	CExperimentInfo*	m_pExpInfo; // 实验基本信息
	CReagentItem*		m_pReagentItem; // 试剂项目
	CPlateTubeInfo*		m_pPlateInfo; // 孔板设置信息

	CLinkList<CSegItemInfo> m_lnkSegment;	//实验步骤信息

	int			m_iChannelCount; // 仪器通道数
	CPtrArray	m_arrayDye; // 仪器可用的通道染料 tagDyeChannelInfo
	float**		m_ppFCrosstalkPara; // 交叉干扰参数，扩增分析使用
	float**		m_ppFMeltCrosstalk; // 交叉干扰参数，熔曲分析使用
	CPtrArray	m_arrayFilter; // 滤波参数, tagFilterItem

	eCalibrateMethod	m_nCalibrateMethod; // 校准方法
	CCalibrateParaTurbidity* m_pTurbidityPara; // 校准方法，浊度校准

	CDataAnalysisPara*	m_pAnalysisPara; // 分析参数

	CTemperatureData*	m_pTempData; // 温度数据

	CPlateAmplifyTubeData*	m_pAmplifyData; // 荧光扩增数据
	CPlateMeltTubeData*		m_pMeltData; // 荧光熔解数据

	CPtrArray	m_arrayAddCycle; // 增加的循环数

	BOOL m_bReachLidTemp; // 热盖是否达到预设温度
	int	m_iNotReachTempCount; // 控温没有达到次数，超过规定控温时间（如60秒）

	eDataStatus	m_nStatus; // 数据状态

	vector<int> m_vecMonitorChanSelState;	//记录监控页面通道选择状态
	vector<int> m_vecDataAnaChanSelState;	//记录分析页面通道选择状态

	vector<CString> m_vecChanDyeName;	//通道切换按钮的染料名称标识（用于通道切换按钮显示）
	vector<CString> m_vecInstruDefDyeName;	//仪器每个通道默认的染料名称，暂时先不保存在实验文件中，20220624

	CString m_strFileOpenPath;	//记录文件打开时的完整路径名
	CString m_strFileName;	//记录文件打开时的文件名称
	CString m_strBackupFilePath; // 备份文件名

	CString m_strBackupFileCreateTime;	//备份文件创建时间，无需保存
	BOOL m_bTemplateFile;	//标识打开的文件是否是模板文件，无需保存 
		
	CHardwareInfo m_hardwareInfo;	//实验文件保存的硬件信息

	BOOL m_bNormalizeSelLog;	//标识归一化数据是否使用对数坐标（曲线绘制控件的纵坐标），20220616，孙文郎
	BOOL m_bDeltaRnSelLog;	//标识DeltaRn数据是否使用对数坐标（曲线绘制控件的纵坐标），20220616，孙文郎
	BOOL m_bFilterSelLog;	//标识滤波后的原始数据是否使用对数坐标（曲线绘制控件的纵坐标），20220616，孙文郎

	CString m_strIVDFileRstSaveFolder;	//PS96IVD模式下，记录保存实验文件和结果报告的文件夹，不需要保存到实验文件中，20220627，孙文郎
	CString m_strIVDFileRstSaveName;	//PS96IVD模式下，记录保存实验文件和结果报告的名称，不需要保存到实验文件中，20220627，孙文郎

	CLinkList<CAxisInfo> m_lnkDefAxisInfo;	//实验结束后，保存图形控件的X/Y最大最小值

	CVolumeDelayParam m_volumeDelayParam;	//记录与体积相关的额外延时参数信息，N96非熔曲步骤使用，可保存到文件中

	CString m_strUserName;

	//康研葆使用的公用样本信息，添加于20221025
	CString m_strCYSamCategory;
	CString m_strCYSamCount;
	CString m_strCYSamState;
	CString m_strCYProductBN;
	CString m_strCYTestItem;
	CString m_strCYComment;
	CString m_strCYCollectDate;

	//实验员和审核员，20221027
	CString m_strTester;	//实验员，不使用
	CString m_strReviewer;	//审核员，不使用

	CStringArray m_arrayLog; // 实验错误或提示信息，保存在数据文件中

	CString m_strTemplateName;

	int	m_iReconnectState; // 实验过程中仪器重联状态 0:正常或用户中止 1:断开后重联成功 2:断开后实验时间到
	int	m_iConnInterruptCount; // 连接断开次数
	BOOL m_bHasZeroValues;

public:
	CTime m_tmStart;	//记录实验开始时的系统时间，无需保存
	CTime m_tmEnd;	//记录实验结束时的系统时间，无需保存

	CSamIdSiteRelation m_samIdSiteRelation;	//导出结果报告时使用，不需要保存到实验文件中
	CExpStateInfo m_ExpStateInfo;	//记录实验状态信息，不需要保存到实验文件中
	CInstancePCR* m_pInstance;	//记录实验运行过程中的仪器对象

	double m_dCurTem;	//记录实验过程中的实时温度
	BOOL	m_bExpRunning;
};



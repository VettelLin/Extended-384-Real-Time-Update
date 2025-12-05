#pragma once

#include "InstancePCR.h"
#include "ExperimentData.h"
#include "ExpRunPara.h"
#include "ExpTempDataThread.h"
#include "ExpDeviceStatusThread.h"
#include "LanguageInfo.h"

// 实验过程控制线程
class CExpRunCtrl
{
public:
	CExpRunCtrl(CInstancePCR* pInstance, CWnd* pMsgWnd);
	~CExpRunCtrl(void);

// Operations
public:
	BOOL Run(CExperimentData* pExpData, CExpRunPara* pRunPara);
	void RunProc();

	int GetCurCycle(){return m_pRunPara->GetCurCycle();}
	int GetCurSegment(){return m_pRunPara->GetCurSegment();}
	int GetCurStep(){return m_pRunPara->GetCurStep();}

	void SetPowerOffExpInfo(BOOL bPowerOffExp,int iStartSeg,int iStartStep,int iStartCyle);
	void ResetPowerOffExpInfo();

protected:
	virtual BOOL Initialize();
	virtual void AfterRunning();
	virtual BOOL PrepareRunning();
	virtual void RefreshStatus();
	virtual BOOL RunLidHeader();
	virtual BOOL SetLaserStatus();
	virtual BOOL ChannelSignalCorrection();
	virtual void HandleDeviceStatus();

	void CreateAndSendCycle(long lStartTime, long lEndTime, int iCycleCount);
	BOOL RunNoneStep(CStepItemInfo* pStep, float fPreTemp);
	BOOL RunMeltStep(CStepItemInfo* pStep,float fPreTemp,int iPreDelay);
	BOOL RunAmplifyStep(int iCycleC, CStepItemInfo* pStep, float fPreTemp);
	BOOL ReadScanDataOnce(ESCANDATATYPE eScanDataType);

	BOOL SetMultiTemp(float fTemp1, float fTemp2, float fTemp3, float fRate, BYTE cMode);
	void CompareTempAndWriteLog(int iCycle, CStepItemInfo* pStep);
	void AddInterruptLog(int iSegNo, int iStepNo, int iCycleNo);

	BOOL ReconnectDeviceToRun();
	int CalculateCurrentCycle(int iCycle, CSegItemInfo* pSegment, int iDelaySecond);

private:
	void RunStepDelay(int iWaitSecond);
	int RunWaitReachTemp(CStepItemInfo* pStep);
	int RunWaitReachTemp(float fAimTem);
	BOOL CoolAtEnd();	//实验结束后，自动进行冷却降温

// Attributes
public:
	void SetInstancePCR(CInstancePCR* pInstance) { m_pInstancePCR = pInstance; }
	CInstancePCR* GetInstancePCR() {return m_pInstancePCR;}
	void AskStop() { m_bAskStop = TRUE; }
	void SetLanguage(CLanguageInfo* pInfo) { m_pLanguage = pInfo; }
	CString GetRunLog();

protected:
	CInstancePCR*	m_pInstancePCR; // PCR实例

	CExperimentData* m_pExpData; // 实验数据
	CExpRunPara*	m_pRunPara; // 运行参数

	CWnd*		m_pMsgWnd; // 接受消息窗口
	CWinThread* m_pRunThread; // 运行主线程指针
	CExpTempDataThread* m_pTempThread; // 读取温度线程指针
	CExpDeviceStatusThread* m_pStatusThread; // 设备状态监控线程

	eExpRunStatus	m_nRunStatus; // 运行状态
	BOOL			m_bAskStop; // 要求结束
	CString			m_strErrorInfo; // 错误信息描述，保存到日志中
	long			m_lScanStartTime; // 开始扫描时的时刻，恒温PCR使用

	int				m_iNotReachTempCount; // 控温没有达到次数，超过规定控温时间（如60秒）
	BOOL			m_bNotReachMeltStartTemp; // 没有达到熔解段开始温度
	BOOL	m_bFirstAmplifyCycle; // 扩增循环第1个循环

	double m_dMeltTemBeforeScan;	//保存熔曲数据扫描前的温度
	double m_dMeltTemAfterScan;	//保存熔曲数据扫描前的温度

	BOOL m_bPowerOffExp;	//标识是否是断电保护后重新运行的实验
	int m_iStartSeg;	//断电保护实验 断电时运行的程序段ID
	int m_iStartStep;	//断电保护实验 断电时运行的步骤ID
	int m_iStartCyle;	//断电保护实验 断电时运行的循环数ID

	CStringArray m_arrayInfo; 
	BOOL	m_bNewSegmentCycle; // 是否是新的程序段开始，方便恒温仪控温使用
	BOOL	m_bFirstTempErr;
	CLanguageInfo* m_pLanguage;

private:
	BOOL m_bNoStepBeforeMeltStep;	//运行断电保护流程时，如果熔曲步骤是该程序段的第一个步骤，则 运行该步骤前需要稳定初始温度
	float m_fMeltStepStartTem;	//记录熔曲步骤运行前的初始温度（运行断电保护流程时使用）
};


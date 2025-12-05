#pragma once

#include "DeclaresPCR.h"
#include "ConfigureDBInfo.h"

// 实验运行过程中的参数，包括配置参数和过程参数
class AFX_EXT_CLASS CExpRunPara
{
public:
	CExpRunPara(void);
	~CExpRunPara(void);

// Operations
public:
	unsigned long long GetStartTime(eExpStageType nType);
	void SetStartTime(eExpStageType nType, unsigned long long lTime);
	int GetCurrentStageDelay();

	void SetRunInfo(CDBRunInfo* pRunInfo);
	void ExpRun();
	void SetCurSegment(int iSegment);
	void SetCurCycle(int iCycle);
	void SetCurStep(int iStep);
	void ExpStop(CDBRunInfo::eStopFlagType nType);
	int GetCurSegment() { return m_pDBRunInfo->GetCurSegment(); }
	int GetCurCycle() { return m_pDBRunInfo->GetCurCycle(); }
	int GetCurStep() { return m_pDBRunInfo->GetCurStep(); }

	void LockInstance();
	void UnlockInstance();

protected:

// Attributes
public:
	float GetLidLowerTemp() { return m_fLidLowerTemp; }
	void SetLidLowerTemp(float fTemp) { m_fLidLowerTemp = fTemp;}
	float GetPeltierLowerTemp() { return m_fPeltierLowerTemp; }
	void SetPeltierLowerTemp(float fTemp) { m_fPeltierLowerTemp = fTemp; }

	int GetLidGetTempDelay() { return m_iLidGetTempDelay; }
 	void SetLidGetTempDelay(int iDelay) { m_iLidGetTempDelay = iDelay; }
	int GetAmplifyTempDelay() { return m_iAmplifyTempDelay; }
	void SetAmplifyTempDelay(int iDelay) { m_iAmplifyTempDelay = iDelay; }
	int GetMeltTempDelay() { return m_iMeltTempDelay; }
	void SetMeltTempDelay(int iDelay) { m_iMeltTempDelay = iDelay; }
	BOOL GetEndAutoCooling()  { return m_bEndAutoCoolling; }
	void SetEndAutoCooling(BOOL bEnable) { m_bEndAutoCoolling = bEnable; }

	eExpStageType GetCurStage() { return m_nStageType; }
	void SetCurStage(eExpStageType nType) { m_nStageType = nType; }

	float GetCurTemp() { return m_fCurTemp; }
	void SetCurTemp(float fTemp, BOOL bReach) { m_fCurTemp = fTemp; m_bCurReachTemp = bReach; }
	BOOL GetCurReachTemp() { return m_bCurReachTemp; }
	float GetLidTemp() { return m_fLidTemp; }
	void SetLidTemp(float fTemp, BOOL bReach) { m_fLidTemp = fTemp; m_bLidReachTemp = bReach; }
	BOOL GetLidReachTemp() { return m_bLidReachTemp; }

	void SetScanGetDataMs(clock_t ckMs);
	clock_t GetScanGetDataMs();
	int GetScanDataSecond();

	void SetMeltStepRunningState(BOOL bMeltStepRunning);
	BOOL IsMeltStepRunning();

	void SetCommunicationState(BOOL bValue);
	BOOL GetCommunicationState();

protected:
	CRITICAL_SECTION m_csUseInstance; 

	float		m_fLidLowerTemp; // 热盖最低温度
	float		m_fPeltierLowerTemp; // 帕尔贴最低保持温度

	// 控制参数
	int		m_iLidGetTempDelay; // 热盖加热过程中读取温度间隔，单位：毫秒
	int		m_iAmplifyTempDelay; // 扩增过程中读取温度间隔，单位：毫秒
	int		m_iMeltTempDelay; // 熔解过程中读取温度间隔，单位：毫秒
	BOOL	m_bEndAutoCoolling; // 实验结束是否自动降温

	// 过程参数
	eExpStageType	m_nStageType; // 当前实验段类型
	unsigned long long m_lTempStartTime; // 取温度数据开始时间
	unsigned long long m_lAmplifyStartTime; // 扩增段开始时间
	unsigned long long m_lMeltStartTime; // 熔解段开始时间
	unsigned long long m_lSegmentStartTime; // 每个程序段循环开始时间，方便计算整个实验所需时间，可以根据一个循环的时间计算全部循环时间

	CDBRunInfo* m_pDBRunInfo; // 数据库中需要保存的运行信息
	BOOL	m_bExpInterrupt; // 实验是否中断

	float	m_fCurTemp; // 取温度线程得到的最近温度值，表示整个PCR仪温度
	BOOL	m_bCurReachTemp; // 最近取温度时下位机返回，表示温度是否达到
	float	m_fLidTemp; // 热盖温度
	BOOL	m_bLidReachTemp; // 热盖温度是否到达目标值

	clock_t m_ckScanGetDataMs;	//记录扫描以及上传数据占用的时间，单位为毫秒

	BOOL m_bMeltStepRunning;	//判断是否正在运行熔曲步骤

private:
	BOOL m_bCommuNormal;	//标识通信状态是否正常，防止通信异常时，主线程处于延时等待过程中无法及时退出
	HANDLE m_hSyncCommuState;	//同步m_bCommuNormal访问
};


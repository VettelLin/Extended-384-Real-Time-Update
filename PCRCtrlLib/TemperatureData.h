#pragma once

#include <vector>
using namespace std;

// 实验过程采集到的温度数据
class AFX_EXT_CLASS CTemperatureData
{
public:
	CTemperatureData(void);
	~CTemperatureData(void);

	enum eTempChannel
	{
		TEMP_CH_PET = 0, // 帕尔贴第1个通道
		TEMP_CH_LID, //热盖
	};
	BOOL AllocateMemory(int iChannelCount,int iLidChannelCount = 1);
	void AddOneTempData(DWORD dwTime,int nTempCount,UINT* pInputTemp,BOOL bMeltStepRunning = FALSE);

	BOOL WriteFile(CFile* pFile);
	BOOL ReadFile(CFile* pFile, WORD wVersion);

	UINT GetCurTemp(eTempChannel nTempChannel,int iChannelNo = 0);
	DWORD GetCurTempCollectTime();
	BOOL HasData();

	void SetChannelTarget(float fTemp1, float fTemp2, float fTemp3);

	vector<UINT>* GetTemDataPtr();

	int GetDataCount();
	void GetTimeData(int iDataCount, double* pdValueTime);
	void GetChannelData(CTemperatureData::eTempChannel nTempChannel, int iChannelNo, int iDataCount, double* pdValueTemp);

	double GetOneTimeData(int iIndex);
	double GetOneTempData(int iChannel, int iIndex);

protected:
	void DeleteAll();

// Attributes
public:
	void SetUseLidHeater(BOOL bUseLid) { m_bUseLid = bUseLid; }
	void SetLidChannelCount(int iCount) { m_iLidChannelCount = iCount; }
	int GetLidChannelCount() { return m_iLidChannelCount; } 
	void SetChannelCount(int iCount) { m_iChannelCount = iCount; }
	int GetChannelCount() { return m_iChannelCount; }

protected:
	BOOL	m_bUseLid; // 是否使用热盖
	int		m_iLidChannelCount; // 热盖通道数
	int		m_iChannelCount; // 温度有几种
	vector<DWORD>*	m_pTimeData; // 采集温度时的时间
	vector<UINT>*	m_pTempData; // 采集温度，是种类阵列数组，单位0.01度

	// 温度数据修正参数
	int		m_iUpDownFlag;
	UINT	m_nTargetChannel1; // 通道1目标值
	UINT	m_nTargetChannel2; // 通道2目标值
	UINT	m_nTargetChannel3; // 通道3目标值
	BOOL	m_bSteadyPeriod; // 是否稳定期

	UINT	m_iChan1PreTem; // 通道1上一个目标值
	UINT	m_iChan2PreTem; // 通道2上一个目标值
	UINT	m_iChan3PreTem; // 通道3上一个目标值
};


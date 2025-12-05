#pragma once

#include "TubeData.h"


// 所有反应管数据

class AFX_EXT_CLASS CPlateTubeData
{
public:
	CPlateTubeData(void);
	~CPlateTubeData(void);

// Operations
public:
	virtual BOOL AllocateMemory(int iTubeCount, int iChannelCount) = 0;
	void DeleteAll();
	void AddCycleX(CUIntArray* pArrayCycle); // 增加全部管的当前循环数或时间
	int AddOriginalData(CUIntArray& aryOriData,int iChannelID);

	void GetCycleX(int iTubeID, int iCycleCount, double* pdValueX, BOOL bConstantPCR);
	double GetCycleX(int iTubeID, int iCycleId, BOOL bConstantPCR);
	UINT GetCurrentCycleX(int iTubeID);

	void GetChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdYValue);
	double GetChannelCycleFluY(int iTubeID, eDataType nType, int iChannel, int iCycleId);	//获取特定通道单个孔位在某个循环的荧光信号值
	void SetChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdYValue);

	BOOL WriteFile(CFile* pFile);
	BOOL ReadFile(CFile* pFile, WORD wVersion);

	// 数据处理
	void FluChannelCrosstalkAdjust(CTubeInfo* pTubeInfo, float** pCrosstalkMatrix);

	//获取数据点个数
	int GetDataNum();
	double GetChannelMaxValueY(eDataType nDataType,int iTubeID,int iChannel);

// Attributes
public:
	void SetScanStartPos(eScanStartPos nPos) { m_nStartPos = nPos; }
	int GetChannelCount() { return m_iChannelCount; }
	BOOL HasData();

protected:
	eScanStartPos m_nStartPos; // 扫描开始位置
	int		m_iTubeCount;	//仪器总的试管个数
	int		m_iChannelCount; //仪器总的检测光路通道个数
	CPtrArray	m_arrayTubeData; // 所有数据
};

// 所有反应管扩增数据
class AFX_EXT_CLASS CPlateAmplifyTubeData : public CPlateTubeData
{
public:
	CPlateAmplifyTubeData(void) {}
	~CPlateAmplifyTubeData(void) {}

public:
	virtual BOOL AllocateMemory(int iTubeCount, int iChannelCount);

	void FluDigitalFilter(CTubeInfo* pTubeInfo, CPtrArray* pFilterItem, CDataAnalysisPCR* pAlgorithm, int iFlag=0);
	void FluoAdjustBaseline(CTubeInfo* pTubeInfo, CDataAnalysisPara* pAnalysisPara, CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity);
	void FluTurbidityAdjust(CTubeInfo* pTubeInfo, CCalibrateParaTurbidity* pTurbidity, CDataAnalysisPCR* pAlgorithm);
	void FluNormalizeAndDeltaRn(CTubeInfo* pTubeInfo, CDataAnalysisPara* pAnalysisPara, CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity);

	double GenerateCtByTubeID(eDataType nDataType, int iTubeID, eMethodCalculateCt nMethod, tagAnaAmplifyPara* pAmplifyPara, CDataAnalysisPCR* pAlgorithm);
	double GenerateAutoSplineThreshold(eDataType nDataType,int iTubeID,int iChannel,CDataAnalysisPCR* pAlgorithm);

	int SetTubeData(vector<double>* pVecData,int iTubeNum,int iChanNum);	//设置样本数据，iTubeNum 和 iChanNum是数据源对应的样本管数和通道数

protected:
	// 滤波参数
};

// 所有反应管熔解数据
class AFX_EXT_CLASS CPlateMeltTubeData : public CPlateTubeData
{
public:
	CPlateMeltTubeData(void) {}
	~CPlateMeltTubeData(void) {}

public:
	virtual BOOL AllocateMemory(int iTubeCount, int iChannelCount);

	void FluDigitalFilter(CTubeInfo* pTubeInfo, CPtrArray* pFilterItem, CDataAnalysisPCR* pAlgorithm, int iFlag=0);

	int AddOriginalData(CUIntArray& aryOriData,int iChannelID,double dTem);
};


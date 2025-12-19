#pragma once

#include <vector>
#include "TubeInfo.h"
#include "DataAnalysisPCR.h"
#include "CalibrateParaTurbidity.h"
#include "DataAnalysisPara.h"

using namespace std;

//一个反应管的数据，只有原始数据和处理后数据
class AFX_EXT_CLASS CTubeData
{
public:
	CTubeData(void);
	virtual ~CTubeData(void);

	virtual BOOL AllocateMemory(int nChannelCount);
			void DeleteAll();
			void AddCycleX(UINT nCycle);
	virtual void AddOriginalData(int nChannelNo, UINT nYValue);
	virtual void AddOriginalData(int nChannelNo, UINT nYValue,double dTem) = 0;
			BOOL HasData();

			int GetPointCout(){	return m_pFluOriginal[0].size(); }
	virtual void GetCycleX(int nCycleCount, double* pdValueX, BOOL bConstantPCR);
	virtual double GetCycleX(int iCycleId, BOOL bConstantPCR);
			UINT GetCurrentCycleX();
			void GetChannelFluY(eDataType nType, int nChannel, int nCycleCount, double* pdYValue);
			void SetChannelFluY(eDataType nType, int nChannel, int nCycleCount, double* pdYValue);

			//获取孔位特定通道某个循环的信号值
			double GetChannelCycleFluY(eDataType nType, int nChannel, int nCycleId );

	virtual	BOOL WriteFile(CFile* pFile);
	virtual BOOL ReadFile(CFile* pFile,WORD wVersion);

			// 数据处理
			void FluChannelCrosstalkAdjust(CTubeInfo* pTubeInfo, float** pCrosstalkMatrix);

			//获取数据点个数
			int GetDataNum(int iChannelNo=0);
			double GetMaxValueY(eDataType nDataType, int iChannel);

protected:
	virtual BOOL BeAllAllocated();	//是否都成功申请了内存空间
	virtual double GetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo);
	virtual void SetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo, double dYValue);

	void FluCopyTo(int nChannelCount, int nCycleCount, vector<UINT>* pBase, vector<double>* pTarget);
	void FluCopyTo(int nChannelCount, int nCycleCount, vector<double>* pBase, vector<double>* pTarget);
	void GetChannelFlu(eDataType Type, int nChannel, int nCycleCount, double* pdXValue, double* pdYValue);
	void SetChannelFlu(eDataType Type, int nChannel, int nCycleCount, double* pdYValue);
	void FluDigitalFilterBy(CTubeInfo* pTubeInfo,CPtrArray* pFilterItem,int iFlag,CDataAnalysisPCR* pAlgorithm, int iDivideFlag);
	void FluDigitalFilterByChannel(int iChannelNo,CPtrArray* pFilterItem,int iFlag,CDataAnalysisPCR* pAlgorithm, int iDivideFlag);

	double ConvertToLog10(double dInput);

protected:
	int		m_nChannelCount;
	vector<UINT>	m_viValueX; // 对应循环数或者时间等，如果时间则按照毫秒保存
	vector<UINT>*	m_pFluOriginal;
	vector<double>* m_pFluCrossTalk;
	vector<double>* m_pFluFilterMedian;
	vector<double>* m_pFluBaselineAdjust;
	vector<double>* m_pFluFilter;
	vector<double>* m_pFluTuibitidy; // 使用浊度调整

	CArray<BOOL, BOOL> m_arrayAutoAdjust; // 自动调整荧光数据

};

//扩增段数据，包括原始数据和处理后数据
class AFX_EXT_CLASS CTubeAmplifyData : public CTubeData
{
public:
	CTubeAmplifyData(void);
	~CTubeAmplifyData(void);

public:
	virtual BOOL AllocateMemory(int nChannelCount);
			void DeleteAll();
	virtual void AddOriginalData(int nChannelNo, UINT nYValue);
	virtual	void AddOriginalData(int nChannelNo,UINT nYValue,double dTem) { ASSERT(FALSE); }

			void FluDigitalFilter(CTubeInfo* pTubeInfo,CPtrArray* pFilterItem,CDataAnalysisPCR* pAlgorithm, int iDivideFlag);
			void FluoAdjustBaseline(CTubeInfo* pTubeInfo, CDataAnalysisPara* pAnalysisPara,CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity);

			void FluTurbidityAdjust(CTubeInfo* pTubeInfo, CCalibrateParaTurbidity* pTurbidity,CDataAnalysisPCR* pAlgorithm);

			void FluNormalizeAndDeltaRn(CTubeInfo* pTubeInfo,CDataAnalysisPara* pAnalysisPara,CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity);

			double GenerateCt(eDataType nType, eMethodCalculateCt nMethod, tagAnaAmplifyPara* pAmplifyPara, CDataAnalysisPCR* pAlgorithm);
			double GenerateAutoSplineThreshold(eDataType nType,int iChannel,CDataAnalysisPCR* pAlgorithm);

protected:
	virtual BOOL BeAllAllocated();
	virtual double GetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo);
	virtual void SetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo, double dYValue);
			void CalcFluDataByCtAndMOT(int nChannelNo, int nStartNo, int nCycleCount, double* pdYValue, double dChangedRatio);
			void CalcAllCtPosBySndDerivative(CTubeInfo* pTubeInfo, int iChannelMOT, CDataAnalysisPCR* pAlgorithm);
			int CalcFluoDescendPosByCtAndMOT(CTubeInfo* pTubeInfo, int iCycleOfMOT, CDataAnalysisPCR* pAlgorithm);

			double GenerateCtOfSpline(int iCycleCount, double *pdInputX, double *pdInputY, tagAnaAmplifyPara* pAmplifyPara, CDataAnalysisPCR* pAlgorithm,eDataType nType);
			int CalcCycleOfMaxMOTRatio(int iCycleCount, CCalibrateParaTurbidity* pTurbidity, CDataAnalysisPCR* pAlgorithm);


protected:
	vector<double>* m_pFluNormalize;
	vector<double>* m_pFluDeltaRn;
};

// 熔解段数据，包括原始数据和处理后数据
class AFX_EXT_CLASS CTubeMeltData : public CTubeData
{
public:
	CTubeMeltData(void);
	~CTubeMeltData(void);

public:
	virtual BOOL AllocateMemory(int nChannelCount);
	virtual BOOL BeAllAllocated();
			void DeleteAll();
	virtual double GetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo);
	virtual void SetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo, double dYValue);
	virtual void GetCycleX(int nCycleCount,double* pdValueX,BOOL bConstantPCR);
	virtual double GetCycleX(int iCycleId,BOOL bConstantPCR);

	virtual	BOOL WriteFile(CFile* pFile);
	virtual BOOL ReadFile(CFile* pFile,WORD wVersion);

			void AddOriginalData(int nChannelNo,UINT nYValue,double dTem);

			void FluDigitalFilter(CTubeInfo* pTubeInfo, CPtrArray* pFilterItem, CDataAnalysisPCR* pAlgorithm, int iDivideFlag);

protected:
	vector<double>* m_pTem;
	vector<double>* m_pNormalize;	//归一化处理之后的数据，预留
	vector<double>* m_pFstNegReciprocal;	//计算一阶负倒数之后的数据
	vector<double>* m_pGainData;	//增益处理之后的数据,预留
};

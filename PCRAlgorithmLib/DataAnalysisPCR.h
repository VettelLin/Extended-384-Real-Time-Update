#pragma once

#include "DataAlgorithmPCR.h"
#include "PeakAmlCheckParam.h"

class AFX_EXT_CLASS CDataAnalysisPCR : public CDataAlgorithmPCR
{
public:
	CDataAnalysisPCR(void);
	~CDataAnalysisPCR(void);

// 保存与分析相关的参数
public:
	void InitPara(CString strStartPath);
	BOOL CanNormalize(int iCycleCount);

	BOOL IsPeakAml(double *pDX,double *pDY,int nDataNum,CPeakAmlCheckParam& peakAmlChkParam, double &dOutCt);

	void AdjustFluValueByBaseline(int nPtCount, double *pInputX, double *pInputY, int iStart, int iEnd);
	void CalcNormalize(int nPtCount, double *pInputY, double *pOutputY, double dAverage);
	void CalcDeltaRn(int nPtCount, double *pInputY, double *pOutputY, double dAverage);
	void CalcFluTurbidityAdjust(int iCtMOT, int iCtPos, double dAdjustRatio, int nPtCount, double* pInputX, double *pInputY, double *pOutputY);
	BOOL AbleToCalculateCt(int nPtCount, double *pInputX, double *pInputY, BOOL bNormalized);
	void NewAutoFindCrest(int nCrestNum,int nPtNum,double *dXValue,double *dYValue,double *dReturn,int* pDCrestIndex,double dThrePercent = 0.2,double dHandSetThre = 0,BOOL bPosFstDerivative = FALSE);

	int FindStartPlateauCycle(int iCycleCount, double *pdInputX, double *pdInputY, double dCt);

	void CalcBaselinePosOfNoCt(int nPtCount, double *pInputX, double *pInputY, int &iStart, int &iEnd);
	int CalcLegalLineFitParam(int nPtCount, double *pInputX, double *pInputY, double *dReturn, int &iMinPtCount, double dMinR);
	BOOL FindPeakAmlBySlope(int nPtCount, double *pInputX, double *pInputY, int iWndWidth=5);

protected:
	void CalcFluDataByCtAndMOT(int nStartNo, int nCycleCount, double* pdValueY, double dChangedRatio, double* pdOutputY);
	double CalculateSD(int iDataCount, double* pInputData);
	BOOL MeetForeAvgAndEndAvgCondition(int iDataCount, double* pInputData, CPeakAmlCheckParam& peakAmlChkParam);

protected:
	tagFunAbleToCalcuParamInfo m_tagAbleToCalcuParam; ///< 判定曲线是否满足计算的相关参数信息 
	tagFunAmpNormalizedAnaParamInfo m_tagAmpNormalizedAnaParam; ///< 扩增曲线归一化处理相关参数信息
};


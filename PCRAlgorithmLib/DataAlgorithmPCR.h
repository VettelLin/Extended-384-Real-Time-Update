#pragma once
#include <afxtempl.h>
#include <vector>
#include "DataAlgorithm.h"


//归一化分析参数结构(熔曲)
typedef struct structNormalizeParaminfo
{
	bool	bAble;			//可否进行归一化
	int		nMinPos;		//归一化最小值位置
	int     nMaxPos;        //归一化最大值位置 
	float   fMinTemp;
	float   fMaxTemp;
	structNormalizeParaminfo()
	{
		bAble = false;
		nMinPos = -1;
		nMaxPos = -1;
		fMinTemp = 0;
		fMaxTemp = 0;
	}

}NORMAPARAMINFO, *PNORMAPARAMINFO;

typedef struct tagFunAbleToCalcuParamInfo
{
	 bool bNormalization;
	 int  nCurveType;
	 double  dAmpNoNormalizeStd;
	 double  dAmpNoNormalizeRatio;
	 double  dAmpNormalizeStd;
	 double  dAmpNormalizeRatio;

	 double  dFDNoNormalizeVal;
	 double  dFDNoNormalizeRatio;
	 double  dFDNormalizeVal;
	 double  dFDNormalizeRatio;

	 double  dMeltNoNormalizeStd;
	 double  dMeltNoNormalizeRatio;
	 double  dMeltNormalizeStd;
	 double  dMeltNormalizeRatio;

	 int bkCalcDotNum;
	 double dCrestBkRatio;

	 tagFunAbleToCalcuParamInfo()
	 {
		 bNormalization = false;
		 nCurveType = 0;//0，扩增曲线；1，普通熔解曲线；2，蛋白熔曲
		 dAmpNoNormalizeStd = 45;
		 dAmpNoNormalizeRatio = 1.1;
		 dAmpNormalizeStd = 0.007;
		 dAmpNormalizeRatio = 1.1;

		 dFDNoNormalizeVal = 40;
		 dFDNoNormalizeRatio = 2;
		 dFDNormalizeVal  = 0.01;
		 dFDNormalizeRatio = 2;

		 dMeltNoNormalizeStd = 45;
		 dMeltNoNormalizeRatio = 1.1;
		 dMeltNormalizeStd = 0.01;
		 dMeltNormalizeRatio = 1.1;

		 bkCalcDotNum = 3;
		 dCrestBkRatio = 0.001;		//原值：0.002，修改于20200608，和IsPeakAml保持一致,孙文郎
	 }
}FUNABLETOCALCUPARAMINFO, *PFUNABLETOCALCUPARAMINFO;

typedef struct tagFunAmpNormalizedAnaParamInfo
{
	bool bAbleToCalcu;
	int nAvgNum;  
	int nNormalizeStartPreXPos;
	int nFiltNum;       

	bool bBaseCorrection; 
	double  dBCK;
	double  dBCR2;    
	int     nBCXPosThre;
	int     nBCStart;     
	int     nBCEndPreXPos; 
	int     nBCEndPreNumPos;

	bool bMinus1;  

	bool bBaseSuppression;
	double  dBSRatioAllData;  
	double  dBSRatioBaseLine;

	tagFunAmpNormalizedAnaParamInfo()
	{
		bAbleToCalcu = false;
		nAvgNum = 3;  
		nNormalizeStartPreXPos = 5;
		nFiltNum = 4;       
		bBaseCorrection = true; 
		dBCK = 0xFFFF;
		dBCR2 = 0.5;    
		nBCXPosThre = 10;
		nBCStart = 1;     
		nBCEndPreXPos = 3; 
		nBCEndPreNumPos = 0;
		bMinus1 = false;  
		bBaseSuppression = true;
		dBSRatioAllData =0.5;  
		dBSRatioBaseLine=0.5;
	}
}FUNAMPNORMALIZEDANAPARAMINFO,*PFUNAMPNORMALIZEDANAPARAMINFO;


class AFX_EXT_CLASS CDataAlgorithmPCR : public CDataAlgorithm
{
public:
	CDataAlgorithmPCR(void);
	~CDataAlgorithmPCR(void);

	// 滤波类型
	enum eDigitalFilterType
	{
		FILTERTYPE_CFMEAN3 = 0,            ///< 中点滑动平均3点
		FILTERTYPE_CFMEAN5 = 1,			   ///< 中点滑动平均5点	
		FILTERTYPE_CFMEAN7 = 2,			   ///< 中点滑动平均7点
		FILTERTYPE_CFMEAN9 = 14,
		FILTERTYPE_CFMEAN11 = 15,          
		FILTERTYPE_CFMEAN13 = 16,
		FILTERTYPE_CFMEAN15 = 17,
		FILTERTYPE_CFMEAN17 = 18,
		FILTERTYPE_CFMEAN19 = 19,
		FILTERTYPE_CFMEAN21 = 20,
		FILTERTYPE_CFMEAN23 = 21,
		FILTERTYPE_CFMEAN25 = 22,
		FILTERTYPE_CFMEAN27 = 23,
		FILTERTYPE_CFMEAN29 = 24,
		FILTERTYPE_CFMEAN31 = 25,

		FILTERTYPE_MEDIAN = 26,

		FILTERTYPE_QuadraticSmooth_5 = 3,	///< 五点两次
		FILTERTYPE_CubicSmooth_5 = 4,		///< 五点三次
		FILTERTYPE_QuadraticSmooth_7 = 5,	///< 七点两次
		FILTERTYPE_CubicSmooth_7 = 6,		///< 七点三次

		FILTERTYPE_DTMEAN3 = 7,				///< 前置滑动平均3点
		FILTERTYPE_DTMEAN4 = 8,				///< 前置滑动平均4点
		FILTERTYPE_DTMEAN5 = 9,				///< 前置滑动平均5点
		FILTERTYPE_DTMEAN6 = 10,				///< 前置滑动平均6点
		FILTERTYPE_DTMEAN7 = 11,				///< 前置滑动平均7点

		FILTERTYPE_TriangularSmooths_29 = 12,///< 29点三角滤波
		FILTERTYPE_GaussianSmooths_31 = 13  ///< 31点伪高斯
	};


public:
	double FindCrestWithParabola(double *x, double *y, int nNum, int nWndWidth);

	//计算一阶/二阶导数	
	void CalDerivative(double *x, double *y, int nNum,int nDev,double *dReturn);
	void NewCalDerivative(double *x, double *y, int nNum,int nDev,double *dReturn);

	//线性拟合
	void LinearFit(double *x, double *y,int nNum, double *dReturn);

	//数字滤波	
	void DigitalFilter(double *x, double *dReturn, int nNum, eDigitalFilterType eType = FILTERTYPE_CubicSmooth_5,int nWnd = 3);

	//插值计算某一点值
	void Spline(double *x, double *y, int nNum, double xCur,double &yCur);
	void SplineEx(double *x, double *y, int nNum, double xCur,double &yCur);

	//数组中的元素头尾两端依次对调
	void Reverse_array(double *x,int nNum, double *dReturn);
	void findPeak(const std::vector<int>& v, std::vector<int>& peakPositions);
	double GetDoublePrecision(double dValue,int nPres);

/*----------------------用户调用的函数--------------------------------------------------*/
	void KdsptForwardMBackN(int n, double *pY, double *pReturn,int forwardM,int backwardN);	//数字滤波，前M点后N点,共M + N + 1点,五点中心滤波尾端使用特殊不收缩算法

	void KdsptForwardMBackN_New(int n, double *pY, double *pReturn,int forwardM,int backwardN);	//数字滤波，前M点后N点,共M + N + 1点,五点中心滤波尾端使用逐渐收缩算法

	void KdsptForwardMBackN_NoShrink(int n, double *pY, double *pReturn,int forwardM,int backwardN);	//数字滤波，前M点后N点,共M + N + 1点，首尾不收缩

	void GenStdCurveByStd(int nCurveNum,int nPtNum, int nStar, int nEnd, int nMultiple,double *dX, double *dY,double *dCt,double *dLinePara,bool bStdCurve,bool bBaseLine);
	void FirstNegDerivative(int nCurveNum, int nPtNum, double *dX, double *dY, double *dXReturn, double *dYReturn);
	void DataDigitalFilter(int nCurveNum, int nPtNum, double *dX, double *dY, double *dYReturn);
	void DataSmooth(int nCurveNum, int nPtNum, int nWidth, double *dY, double *dYReturn);
	void AdjustBaseLine(int nNum, int nAvgNum, double *pInput, double *pOutput);
	void NewAdjustBaseLine(int nNum, int start,int end, double *pInput, double *pOutput);
	void AdjustBaseLine(int nNum, int nAvgNum, double *pdx,double *pdyInput, double *pdyOutput,bool bNormalization = false);
	void NewAdjustBaseLine(int nNum,int forewardNum,int calcDotNum,double *pdx,double *pdyInput, double *pdyOutput,bool bNormalization  = false );
	void CalculatedDeltaRn(int nNum, int nStart,int nEnd, double *pInput, double *pOutput);
	void SortDouble(int nNum, double *pInput, double *pOutput);
	void SortInt(int nNum, int *pInput, int *pOutput);
    void AutoFindCrest(int nCrestNum,int nPtNum, double *dXValue, double *dYValue, double *dReturn,double dThrePercent = 0.2);
	BOOL AbleToCalculate(double *dx, double *dy, int nNum,bool bNormalization = false,bool bCt = true);
	BOOL AbleToCalculateEx(double *dx, double *dy, int nNum,bool bNormalization = false,bool bCt = true);
	BOOL AbleToCalculate(double *dx, double *dy, int nNum,tagFunAbleToCalcuParamInfo paraminfo);

	BOOL NewAbleToCalculate(double *dx, double *dy, int nNum,tagFunAbleToCalcuParamInfo paraminfo,int newStart = 1,int newForwardNum = 3,float newStdNum = 3);

	void NormalizedAnalysis(int nNum,int nAvgNum,int nNormType,bool bMinus1, double *pInput, double *pOutput);
	void NormalizedAnalysisBySndDerivative(int nNum, int nAvgNum, double *pdx,double *pdyInput, double *pdyOutput,int nFiltNum = 4,bool bMinus1 = false,bool bkSuppression = false,bool baseCorrection = false);
	void NormalizedAnalysisBySndDerivative(int nNum, double *pdx,double *pdyInput, double *pdyOutput,tagFunAmpNormalizedAnaParamInfo paraminfo);

	void NormalizedAnalysisFrom0To1BySndDerivative(int nNum,int nmaxpos,int nminpos,double dminTemp,double dmaxTemp,double *pdx,double *pdyInput, double *pdyOutput);
	void FindMinPosAndMaxPosByNormalizeFrom0To1ByFirstDerivative(int nNum,double *pdx,double *pdyInput,int &nMin,int &nMax,int nFiltNum = 4, double dThrePercent = 0.2);
	void DeltaRnAnalysisBySndDerivative(int nNum, double *pdx,double *pdyInput, double *pdyOutput,tagFunAmpNormalizedAnaParamInfo paraminfo);
	double Median(double* x,int nNum);

	int CalcCycleOfMaxMOTRatioBy(int nCycleCount, double* pdXValue, double* pdYValueMOT, double dMinRatio);
	void CalMeltFirstDerivative(double *x, double *y, int nNum,BOOL bBackMinusFront,int stepNum,double *dReturn);

	void CalcAutoBaselineByMinStd(int nPtCount, double *pInputY, int &iOutStart, int &iOutEnd);
	double CalcAutoBaselinePos(int nPtCount, double *pInputX, double *pInputY, int &iOutStart, int &iOutEnd);
	void CalcBaselinePosByCt(int nPosMax, int &iOutStart, int &iOutEnd);
	void CalcAutoBaselinePosBySndDrvCt(double dSndDrvCt, int nPtCount, double *pInputX, double *pInputY, int &iOutStart, int &iOutEnd);
	double CalcBaselineAverage(int nPtCount, double *pInputX, double *pInputY, int iStart, int iEnd);

	void CalcFirstDerivative(int iPtCount, double *pdInputY, double* pdOutputY);
	void CalcSecondDerivative(int iPtCount, double *pdInputY, double* pdOutputY);
	double FindCtPosWithParabola(int iPtCount, double* pdInputX, double *pdInputY, double* pdFirstDer, double* pdSecondDer);
	double CalcCtPosBySndDerivative(int iPtCount, double* pdInputX, double* pdInputY);

	double GenerateCtOfSndDerivative(int iCycleCount, double *pdInputX, double *pdInputY, int iFilterCount=4);
	double GenerateCtOfSpline(int iCycleCount, double *pdInputX, double *pdInputY, double dThreshold);

	void CalcBaselinePosByLineFit(int nPtCount, double *pInputX, double *pInputY, int iEndPos, double dMinR, int &iOutStart, int &iOutEnd);
	void CalcBaselinePosByDeviations(int nPtCount, double *pInputX, double *pInputY, int iEndPos, double dMinR, int &iOutStart, int &iOutEnd);

	int FindCtPosBySlope(int nPtCount, double *pInputX, double *pInputY, int iWndWidth=5);
	BOOL IsLegalCtByFluo(int nPtCount, double *pInputX, double *pInputY, int iCtPos);
	double CalcCtPos(int iPtCount, double* pdInputX, double* pdInputY);

	void OutputPointerData(int iDataCount, double* pOutData);

protected:
	float m_dd2MaxXThre;	//二阶导最大值对应X轴位置小于dd2MaxXThreshold * 循环数(默认值为35)时，比较二阶导最大值是否大于某个数，如果大于，则认为是扩增，反之认为不是扩增
	float m_dd2MaxYThre;	//二阶导最大值阈值

};

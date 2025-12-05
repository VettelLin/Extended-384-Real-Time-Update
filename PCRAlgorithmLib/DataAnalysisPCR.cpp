#include "StdAfx.h"
#include "DataAnalysisPCR.h"

CDataAnalysisPCR::CDataAnalysisPCR(void)
{
	m_tagAbleToCalcuParam.dAmpNoNormalizeStd = 45.0;
	m_tagAbleToCalcuParam.dAmpNoNormalizeRatio = 1.03;
	m_tagAbleToCalcuParam.dAmpNormalizeStd = 0.007;
	m_tagAbleToCalcuParam.dAmpNormalizeRatio = 1.03;
	m_tagAbleToCalcuParam.dFDNoNormalizeVal = 10;
	m_tagAbleToCalcuParam.dFDNoNormalizeRatio = 1.1;
	m_tagAbleToCalcuParam.dFDNormalizeVal = 0.01;
	m_tagAbleToCalcuParam.dFDNormalizeRatio = 1.1;
	m_tagAbleToCalcuParam.dMeltNoNormalizeStd = 45;
	m_tagAbleToCalcuParam.dMeltNoNormalizeRatio = 1.1;
	m_tagAbleToCalcuParam.dMeltNormalizeStd = 0.01;
	m_tagAbleToCalcuParam.dMeltNormalizeRatio = 1.1;

	m_tagAbleToCalcuParam.bkCalcDotNum = 3;
	m_tagAbleToCalcuParam.dCrestBkRatio = 0.002;
	m_tagAmpNormalizedAnaParam.nAvgNum = 3;
	m_tagAmpNormalizedAnaParam.nNormalizeStartPreXPos = 8;

	m_tagAmpNormalizedAnaParam.bBaseCorrection = 1;
	m_tagAmpNormalizedAnaParam.dBCK = 65535;
	m_tagAmpNormalizedAnaParam.dBCR2 = 0.1;
	m_tagAmpNormalizedAnaParam.nBCXPosThre = 10;
	m_tagAmpNormalizedAnaParam.nBCStart = 1;
	m_tagAmpNormalizedAnaParam.nBCEndPreXPos = 3;
	m_tagAmpNormalizedAnaParam.nBCEndPreNumPos = 3;
	m_tagAmpNormalizedAnaParam.bMinus1 = true;
	m_tagAmpNormalizedAnaParam.bBaseSuppression = 1;
	m_tagAmpNormalizedAnaParam.dBSRatioAllData = 0.5;
	m_tagAmpNormalizedAnaParam.dBSRatioBaseLine = 0.5;
}


CDataAnalysisPCR::~CDataAnalysisPCR(void)
{
}

void CDataAnalysisPCR::InitPara(CString strStartPath)
{
	CString strFilePath = strStartPath + _T("\\system\\AlgorithmPara.ini");
	TCHAR buf[128];

	//判定曲线是否满足计算的相关参数信息
	CString sSection=_T(""),sKeyName=_T("");
	sSection=_T("AbleToCalcu");

	sKeyName=_T("AmpNoNormalizeStd");
	GetPrivateProfileString(sSection, sKeyName,_T("-1"), buf, sizeof(buf), strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dAmpNoNormalizeStd = _ttof(buf);
	}
	sKeyName=_T("AmpNoNormalizeRatio");
	GetPrivateProfileString(sSection, sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dAmpNoNormalizeRatio = _ttof(buf);
	}
	sKeyName=_T("AmpNormalizeStd");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dAmpNormalizeStd = _ttof(buf);
	}
	sKeyName=_T("AmpNormalizeRatio");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dAmpNormalizeRatio = _ttof(buf);
	}
	//一阶导数最大值Max有效阈值及最大值/中位值 比率的有效阈值
	sKeyName=_T("FDNoNormalizeVal");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dFDNoNormalizeVal = _ttof(buf);
	}
	sKeyName=_T("FDNoNormalizeRatio");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dFDNoNormalizeRatio = _ttof(buf);
	}
	sKeyName=_T("FDNormalizeVal");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dFDNormalizeVal = _ttof(buf);
	}
	sKeyName=_T("FDNormalizeRatio");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dFDNormalizeRatio = _ttof(buf);
	}

	sKeyName=_T("MeltNoNormalizeStd");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dMeltNoNormalizeStd = _ttof(buf);
	}
	sKeyName=_T("MeltNoNormalizeRatio");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dMeltNoNormalizeRatio = _ttof(buf);
	}
	sKeyName=_T("MeltNormalizeStd");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dMeltNormalizeStd = _ttof(buf);
	}
	sKeyName=_T("MeltNormalizeRatio");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAbleToCalcuParam.dMeltNormalizeRatio = _ttof(buf);
	}

	sSection = _T("DD2CrestBackgroundRatioInfo");
	sKeyName=_T("BkCalculateDotNum");
	GetPrivateProfileString(sSection,sKeyName,_T("3"),buf,sizeof(buf),strFilePath);
	m_tagAbleToCalcuParam.bkCalcDotNum = _ttoi(buf);
	if(m_tagAbleToCalcuParam.bkCalcDotNum <= 0)
	{
		m_tagAbleToCalcuParam.bkCalcDotNum = 3;
	}

	sKeyName=_T("CrestBackgroundRatio");
	GetPrivateProfileString(sSection,sKeyName,_T("0.002"),buf,sizeof(buf),strFilePath);
	m_tagAbleToCalcuParam.dCrestBkRatio = _ttof(buf);


	//扩增曲线归一化处理相关参数信息
	sSection=_T("AmpNormalizedAna");

	sKeyName=_T("AvgNum");
	GetPrivateProfileString(sSection,sKeyName,_T("3"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.nAvgNum = _ttoi(buf);
	}
	sKeyName=_T("NormalizeStartPreXPos");
	GetPrivateProfileString(sSection,sKeyName,_T("8"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.nNormalizeStartPreXPos = _ttoi(buf);
	}
	sKeyName=_T("BaseCorrection");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.bBaseCorrection = _ttoi(buf);
	}
	sKeyName=_T("BCK");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.dBCK = _ttof(buf);
	}
	sKeyName=_T("BCR2");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.dBCR2 = _ttof(buf);
	}
	sKeyName=_T("BCXPosThre");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.nBCXPosThre = _ttoi(buf);
	}
	sKeyName=_T("BCStart");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.nBCStart = _ttoi(buf);
	}
	sKeyName=_T("BCEndPreXPos");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.nBCEndPreXPos = _ttoi(buf);
	}
	sKeyName=_T("BCEndPreNumPos");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.nBCEndPreNumPos = _ttoi(buf);
	}

	sKeyName=_T("Minus1");
	GetPrivateProfileString(sSection,sKeyName,_T("1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.bMinus1 = _ttoi(buf);
	}

	sKeyName=_T("BaseSuppression");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.bBaseSuppression = _ttoi(buf);
	}
	sKeyName=_T("BSRatioAllData");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.dBSRatioAllData = _ttof(buf);
	}
	sKeyName=_T("BSRatioBaseLine");
	GetPrivateProfileString(sSection,sKeyName,_T("-1"),buf,sizeof(buf),strFilePath);
	if(_ttoi(buf) != -1)
	{
		m_tagAmpNormalizedAnaParam.dBSRatioBaseLine = _ttof(buf);
	}
}

// 判断能否归一化，nAvgNum一般为3
BOOL CDataAnalysisPCR::CanNormalize(int iCycleCount)
{
	return m_tagAmpNormalizedAnaParam.nAvgNum > 0 && m_tagAmpNormalizedAnaParam.nAvgNum < iCycleCount;
}

BOOL CDataAnalysisPCR::IsPeakAml(double *pDX,double *pDY,int nDataNum,CPeakAmlCheckParam& peakAmlChkParam, double &dOutCt)
{
	double max = abs(pDY[0]);
	for(int i = 1;i < nDataNum;++i)
	{
		max = max > abs(pDY[i]) ? max : abs(pDY[i]);
	}	
	if (0 == max)
	{
		return FALSE;
	}

	double dSD = CalculateSD(nDataNum, pDY);
	if (dSD <= peakAmlChkParam.m_dStdThreshold)
		return FALSE;

	BOOL bMeetAvg = MeetForeAvgAndEndAvgCondition(nDataNum, pDY, peakAmlChkParam);

	BOOL bFindNormalD2Max = FALSE;

	double* pDTemp = new double[nDataNum];
	memcpy(pDTemp,pDY,nDataNum * sizeof(double));
	double* pDReturn = new double[nDataNum];
	::memset(pDReturn,0,nDataNum);

	double* pDFiltered = new double [nDataNum];
	::memset(pDFiltered,0,nDataNum);

	DigitalFilter(pDTemp, pDFiltered, nDataNum,CDataAlgorithmPCR::FILTERTYPE_MEDIAN, 5);
	memcpy(pDTemp,pDFiltered,sizeof(double) * nDataNum);

	for (int i=0; i<3; ++i)
	{
		KdsptForwardMBackN_New(nDataNum, pDTemp, pDFiltered, 2, 2);
		memcpy(pDTemp,pDFiltered,sizeof(double) * nDataNum);
	}
	//OutputPointerData(nDataNum, pDTemp);

	//计算二阶导数
	//NewCalDerivative(pDX, pDFiltered,nDataNum,2, pDReturn); 	
	CalcSecondDerivative(nDataNum, pDFiltered, pDReturn);
	//OutputPointerData(nDataNum, pDReturn);

	//计算二阶导数最大值
	double dmax = pDReturn[0]; 
	double dmin = pDReturn[0];
	double dXPosMax = CalcCtPosBySndDerivative(nDataNum, pDX, pDFiltered);
	dOutCt = dXPosMax;

	int nxpos_max = (int)(dXPosMax + 0.5);
	dmax = pDReturn[nxpos_max - 1];

	int blDotNum = nxpos_max - peakAmlChkParam.m_forwardNum - peakAmlChkParam.m_start + 1;
	if(blDotNum > 1)
	{
		double dBLAvg = 0,dBLSD = 0;
		//计算变异系数
		for(int i = 0; i < blDotNum;++i)
		{
			dBLAvg += pDReturn[i];
		}
		dBLAvg /= blDotNum;
		for(int i= 0;i < nxpos_max - 3;++i)
		{
			dBLSD += (pDReturn[i] - dBLAvg) * (pDReturn[i] - dBLAvg);
		}
		dBLSD = sqrt(dBLSD / (blDotNum - 1));

		if(dmax > (dBLAvg + peakAmlChkParam.m_sdMultiples * dBLSD))
		{
			bFindNormalD2Max = TRUE;
		}
		else
		{
			bFindNormalD2Max = FALSE;
		}
	}
	else
	{
		bFindNormalD2Max = FALSE;
	}	

	//计算二阶导最大值（从离散数据中寻找）和背景值（取滤波后数据前N个点）的比值
	double dCrestBkRatio = 0;
	double dBkSum = 0,dBkAvg = 0;
	for(int i = 0;i < peakAmlChkParam.m_bkCalcDotNum;++i)
	{
		dBkSum += pDY[i];
	}
	if(peakAmlChkParam.m_bkCalcDotNum > 0)
	{
		dBkAvg = dBkSum / peakAmlChkParam.m_bkCalcDotNum;
	}

	if(0 != dBkAvg)
	{
		dCrestBkRatio = abs(dmax / dBkAvg);
	}

	BOOL bFindPeak = FALSE;
	if (bFindNormalD2Max && dCrestBkRatio > peakAmlChkParam.m_dCrestBkRatio)
	{
		if (bMeetAvg)
		{
			bFindPeak = TRUE;
		}
		else if (nxpos_max > 1)
		{
			int iCurStart, iCurEnd;
			memcpy(pDFiltered,pDTemp,sizeof(double) * nDataNum);

			CalcAutoBaselinePosBySndDrvCt(dXPosMax, nDataNum, pDX, pDFiltered,iCurStart,iCurEnd);
			if (iCurEnd - iCurStart < dXPosMax / 2)
			{
				iCurStart = iCurEnd - int(dXPosMax / 2);
				if (iCurStart < 1)
				{
					iCurStart = 1;
				}
			}

			memcpy(pDFiltered,pDTemp,sizeof(double) * nDataNum);
			AdjustFluValueByBaseline(nDataNum, pDX, pDFiltered,iCurStart,iCurEnd);
			bFindPeak = MeetForeAvgAndEndAvgCondition(nDataNum, pDFiltered, peakAmlChkParam);
		}
		else
			bFindPeak = FALSE; 
	}
	else
	{
		bFindPeak = FALSE; 
	}

	delete [] pDTemp;
	delete [] pDFiltered;
	delete [] pDReturn;	

	if (!bFindPeak && dXPosMax > 3)
	{
		bFindPeak = FindPeakAmlBySlope(nDataNum, pDX, pDY);
	}

	return bFindPeak;
}

// 满足标准方差条件
double CDataAnalysisPCR::CalculateSD(int iDataCount, double* pInputData)
{
	double dAvg = 0,dSD = 0;
	for(int i = 0;i < iDataCount;++i)
	{
		dAvg += pInputData[i];
	}

	dAvg /= iDataCount;
	for(int i = 0;i < iDataCount;++i)
		dSD += (pInputData[i] - dAvg) * (pInputData[i] - dAvg);
	dSD = sqrt(dSD / (iDataCount - 1));
	double cv = 0;
	if(0 != dAvg)
	{
		cv = dSD / dAvg;
	}
	else
	{
		cv = 0;
	}

	return dSD;
}
// 是否满足前后均值限制条件
BOOL CDataAnalysisPCR::MeetForeAvgAndEndAvgCondition(int iDataCount, double* pInputData, CPeakAmlCheckParam& peakAmlChkParam)
{
	double dForeAvg = 0,dEndAvg = 0;
	int nForeID = 0, nEndID = 0;
	nForeID = (int)(iDataCount / 3.0 + 0.5); 
	nEndID =(int)(iDataCount * 8 / 9 + 0.5);

	for(int i = 0;i < nForeID;++i)
	{
		dForeAvg += pInputData[i];
	}
	if(nEndID >= 10)
	{
		nEndID = iDataCount - 3;
	}
	for(int i = nEndID;i < iDataCount;++i)
	{
		dEndAvg += pInputData[i];
	}

	dForeAvg = nForeID > 0 ? dForeAvg / (nForeID) : 0;
	dEndAvg = nEndID > 0 ? dEndAvg / (iDataCount - nEndID) : 0;

	return dForeAvg * peakAmlChkParam.m_dRatioThreshold < dEndAvg;
}

// 归一化处理
void CDataAnalysisPCR::CalcNormalize(int nPtCount, double *pInputY, double *pOutputY, double dAverage)
{
	if(0 == dAverage)
	{
		for(int i = 0;i < nPtCount;++i)
		{
			pOutputY[i] = pInputY[i];
		}	
		return;
	}

	if(m_tagAmpNormalizedAnaParam.bMinus1)	//减1操作,便于对数曲线表现
	{
		for(int i=0; i<nPtCount; i++)
		{
			pOutputY[i] = pInputY[i] / dAverage - 1;
		}	
	}
	else
	{
		for(int i=0; i<nPtCount; i++)
		{
			pOutputY[i] = pInputY[i] / dAverage;
		}
	}
}

//计算△Rn
void CDataAnalysisPCR::CalcDeltaRn(int nPtCount, double *pInputY, double *pOutputY, double dAverage)
{
	for(int i=0; i<nPtCount; i++)
	{
		pOutputY[i] = pInputY[i] - dAverage;
	}
}

// 根据基线区间拟合直线斜率对荧光值进行修正
void CDataAnalysisPCR::AdjustFluValueByBaseline(int nPtCount, double *pInputX, double *pInputY, int iStart, int iEnd)
{
	if (iStart > nPtCount)
	{
		iStart = nPtCount;
	}
	if (iEnd >nPtCount)
	{
		iEnd = nPtCount;
	}
	int iStep = iEnd - iStart +1;

	if (iStep >= 2) // 基线区间包括2个点，进行线性拟合调整
	{
		double dCoef[3]={0};
		double* dTemp = new double[iStep];
		memcpy(dTemp, pInputY + (iStart - 1), sizeof(double) * iStep);
		LinearFit(pInputX, dTemp, iStep, dCoef);

		if (fabs(dCoef[1]) < 100 || (iEnd-iStart > nPtCount/3 || iEnd-iStart > 10))
		{
			for (int i = 0;i < nPtCount;++i)
			{
				pInputY[i] -= (pInputX[i] * dCoef[1]);
			}
		}

		if(NULL != dTemp)
		{
			delete [] dTemp;
			dTemp = NULL;
		}
	}
	return;
}

void CDataAnalysisPCR::CalcFluTurbidityAdjust(int iCtMOT, int iCtPos, double dAdjustRatio, int nPtCount, double* pInputX, double *pInputY, double *pOutputY)
{
	int nStartNo = iCtMOT -1;
	if (nStartNo < 0)
	{
		nStartNo = 0;
	}

	if (nStartNo < 3) // 只有3个点不需要进行线性拟合
	{
		if (iCtPos < 1) // 没有Ct
		{
			double dStartBase = pInputY[nStartNo];
			for (int j=0; j<nPtCount; j++)
			{
				if (j > nStartNo)
				{
					pOutputY[j] = dStartBase + (pInputY[j] - dStartBase) * dAdjustRatio;
				}
				else
				{
					pOutputY[j] = pInputY[j];
				}
			}
		}
		else
		{
			CalcFluDataByCtAndMOT(nStartNo, nPtCount, pInputY, dAdjustRatio, pOutputY);
		}
	}
	else 
	{
		int iMinPtCount = 4;// 4-6个点进行线性拟合，保证R大于9，不然较少拟合点数，最少4点
		double dMinR = 0.93;
		if (nStartNo < 8)
		{
			dMinR = 0.95;
		}
		else
		{
			iMinPtCount = 6;
			dMinR = 0.97;
		}

		double dCoef[3]={0};
		double* pdTmepX = new double[nStartNo+1];
		memcpy(pdTmepX, pInputX, sizeof(double) * (nStartNo+1));
		double* pdTempY = new double[nStartNo+1];
		memcpy(pdTempY, pInputY, sizeof(double) * (nStartNo+1));
		// LinearFit(pInputX, dTemp, nStartNo+1, dCoef);
		int iTempStart = CalcLegalLineFitParam(nStartNo+1, pdTmepX, pdTempY, dCoef, iMinPtCount, dMinR);

		double dMaxDecend = 0;
		if (dCoef[1] < 0)
		{
			for (int i=iMinPtCount+iTempStart; i<nPtCount; ++i)
			{
				double dTempY = pInputX[i] * dCoef[1] + dCoef[0];
				if (dTempY > pInputY[i])
				{
					if (dMaxDecend < dTempY - pInputY[i])
						dMaxDecend = dTempY - pInputY[i];
				}

				pInputY[i] = pInputY[i] + dMaxDecend;
			}
		}

		if (pdTmepX != NULL)
		{
			delete pdTmepX;
			pdTmepX = NULL;
		}
		if (pdTempY != NULL)
		{
			delete pdTempY;
			pdTempY = NULL;
		}
	}

	memcpy(pOutputY, pInputY, sizeof(double)*nPtCount);
	CalcFluDataByCtAndMOT(nStartNo, nPtCount, pInputY, dAdjustRatio, pOutputY);
}


// 根据给定比率进行荧光数据浊度调整
void CDataAnalysisPCR::CalcFluDataByCtAndMOT(int nStartNo, int nCycleCount, double* pdValueY, double dChangedRatio, double* pdOutputY)
{
	double dStartBase = pdValueY[nStartNo];
	for (int j=nStartNo+1; j<nCycleCount; j++)
	{
		if (pdValueY[j] > dStartBase)
		{
			dStartBase = pdValueY[j];
		}
		else
		{
			double dCalcValue = dStartBase + (pdValueY[j] - dStartBase) * dChangedRatio;
			pdOutputY[j] = dCalcValue;
		}
	}
}

BOOL CDataAnalysisPCR::AbleToCalculateCt(int nPtCount, double *pInputX, double *pInputY, BOOL bNormalized)
{
	m_tagAbleToCalcuParam.bNormalization = bNormalized;
	return NewAbleToCalculate(pInputX, pInputY, nPtCount, m_tagAbleToCalcuParam, m_tagAmpNormalizedAnaParam.nBCStart, m_tagAmpNormalizedAnaParam.nBCEndPreXPos, 3);
}

void CDataAnalysisPCR::NewAutoFindCrest(int nCrestNum,int nPtNum, double *dXValue, double *dYValue, double *dReturn,int* pDCrestIndex,double dThrePercent /* = 0.2 */,double dHandSetThre /*= 0*/,BOOL bPosFstDerivative /*= FALSE*/)
{
	CArray<int,int>  arrayTemp;
	arrayTemp.RemoveAll();
	double temp[3] = {0};
	for(int i=1; i<nPtNum-2; i++)
	{
		for(int j=0; j< 3; temp[j++] = 0);
		temp[0] = dYValue[i]- dYValue[i-1];
		temp[1] = dYValue[i]- dYValue[i+1];
		temp[2] = dYValue[i]- dYValue[i+2];

		if( temp[0] >0 && temp[1] >0  && temp[2]>0)
			arrayTemp.Add(i);
	}
	int nSize = (int) arrayTemp.GetSize();
	double *dTemp = new double[nSize];
	double *dOut = new double[nSize];
	for(int i=0; i< nSize; i++)
		dTemp[i] = dYValue[arrayTemp.GetAt(i)];
	SortDouble(nSize,dTemp,dOut);
	//进一步处理，以最高峰值的20%(缺省)作为阈值线，高于阈值线的即为有效峰
	float fThre = 0;
	if (nSize >= 1)
	{
		fThre = dOut[nSize -1] * dThrePercent;
	}

	fThre = dHandSetThre;

	//如果找到多个峰，过滤掉峰值比较小的数,20200225,孙文郎,20211111修改为过滤掉峰值绝对值较小的数
	//使用峰值的绝对值和峰高阈值作比较
	for (int i = 0; i < nSize; i++)
	{
		if (fabs(dOut[i]) < fThre)
			dOut[i] = -1;
	}
	int count = 0;
	for(int i=nSize-1; i>= nSize-nCrestNum && i >= 0; i--)
	{
		bool bfind = false;
		for(int j=0; j< nSize; j++)
		{
			if( dYValue[arrayTemp.GetAt(j)] == dOut[i])
			{
				bfind = true;
				pDCrestIndex[count] = arrayTemp.GetAt(j);
				dReturn[count++] = dXValue[arrayTemp.GetAt(j)];
				if(count >= nCrestNum)
				{
					goto SAMCRESTEXIST;	//两个峰值相同时，容易触发这个问题
				}
			}
		}
		if (!bfind)
		{
			pDCrestIndex[count] = 0; 
			dReturn[count++] = -1;
		}
	}

#ifdef _DEBUG
	if(count > 1)
	{
		int mm = 0;
		++mm;
	}
#endif

SAMCRESTEXIST:
	if(nCrestNum > nSize)
	{
		for(int i=nSize; i<nCrestNum; i++)
		{
			dReturn[i] = -1;
			pDCrestIndex[i] = -1;
		}			
	}

	delete [] dTemp;
	delete [] dOut;
}


int CDataAnalysisPCR::FindStartPlateauCycle(int iCycleCount, double *pdInputX, double *pdInputY, double dCt)
{
	int iReturn = -1;
	int iStartCycle = int (dCt);
	double dStartBase = pdInputY[iStartCycle];
	
	for (int i=iStartCycle+1; i<iCycleCount; i++)
	{
		if (pdInputY[i] > dStartBase)
		{
			dStartBase = pdInputY[i];
		}
		else // 开始下降
		{
			iReturn = i;
			//for (int j=i; j>=1; j--)
			//{
			//	if ((pdInputY[j] - pdInputY[j-1]) > 50)
			//		break;
			//	else
			//		iReturn = j-1;
			//}
			break;
		}
	}

	return iReturn;
}

// 去除开始和结束位置的变动较大的点
void CDataAnalysisPCR::CalcBaselinePosOfNoCt(int nPtCount, double *pInputX, double *pInputY, int &iOutStart, int &iOutEnd)
{
	//if (nPtCount <= 10)
	{
		double dSum = 0;
		double dAverage = 0;
		for (int i=nPtCount/2; i<nPtCount; i++)
		{
			dSum += pInputY[i];
		}

		iOutStart = 1;
		iOutEnd = nPtCount;
		dAverage = dSum / nPtCount / 2;
		for (int i=1; i<nPtCount; i++)
		{
			if (fabs(pInputY[i] - pInputY[i-1]) > dAverage * 0.1)
			{
				iOutStart = i;
			}
			else
				break;
		}

		for (int i=nPtCount-1; i>iOutStart; i--)
		{
			if (fabs(pInputY[i] - pInputY[i-1]) > dAverage * 0.1)
			{
				iOutEnd = i-1;
			}
			else
				break;

		}

		if (iOutStart > nPtCount / 2 || iOutEnd < nPtCount / 2 || iOutEnd - iOutStart < nPtCount / 2)
		{
			iOutStart = 1;
			iOutEnd = nPtCount;
		}
	}

	//else
	//{
	//	double dCurCoef[3] = {0};
	//	int iWndWidth = nPtCount / 4;
	//	double* pdTempX = new double[iWndWidth];
	//	double* pdTempY = new double[iWndWidth];
	//	double fFirstSlope = 0;

	//	BOOL bFindPlateau = FALSE;
	//	BOOL bFindFirstSlope = FALSE;
	//	for (int i=0; i<=nPtCount-iWndWidth; i++)
	//	{
	//		for (int j=0; j<iWndWidth; j++)
	//		{
	//			pdTempX[j] = pInputX[i+j];
	//			pdTempY[j] = pInputY[i+j];
	//		}
	//		LinearFit(pdTempX, pdTempY, iWndWidth, dCurCoef);
	//		if (!bFindFirstSlope)
	//		{
	//			if (dCurCoef[1] < 50)
	//			{
	//				fFirstSlope = dCurCoef[1];
	//				iOutStart = i+1;
	//				bFindFirstSlope = TRUE;
	//				bFindPlateau = TRUE;
	//			}
	//		}
	//		else
	//		{
	//			if (fabs(dCurCoef[1]) < 25)
	//			{
	//				iOutEnd = i+iWndWidth+1;
	//				continue;
	//			}

	//			if (fabs(dCurCoef[1]-fFirstSlope) > fabs(fFirstSlope) * 0.5)
	//			{
	//				iOutEnd = i+iWndWidth+1;
	//				break;
	//			}
	//		}
	//	}

	//	if (!bFindPlateau)
	//	{
	//		int iStart1, iEnd1, iStart2, iEnd2;
	//		fFirstSlope = 0;
	//		BOOL bFirstPlateau = FALSE;
	//		bFindFirstSlope = FALSE;
	//		BOOL bFindSecondSlope = FALSE;

	//		for (int i=0; i<=nPtCount-iWndWidth; i++)
	//		{
	//			for (int j=0; j<iWndWidth; j++)
	//			{
	//				pdTempX[j] = pInputX[i+j];
	//				pdTempY[j] = pInputY[i+j];
	//			}
	//			LinearFit(pdTempX, pdTempY, iWndWidth, dCurCoef);
	//			if (!bFirstPlateau)
	//			{
	//				if (!bFindFirstSlope)
	//				{
	//					fFirstSlope = dCurCoef[1];
	//					iStart1 = i+1;
	//					iEnd1 = iStart1 + iWndWidth -1;
	//					bFindFirstSlope = TRUE;
	//				}
	//				else
	//				{
	//					if (fabs(dCurCoef[1]-fFirstSlope) > fabs(fFirstSlope) * 0.5)
	//					{
	//						iEnd1 = i + iWndWidth;
	//						bFirstPlateau = TRUE;
	//					}
	//				}
	//			}
	//			else
	//			{
	//				if (!bFindSecondSlope)
	//				{
	//					fFirstSlope = dCurCoef[1];
	//					iStart2 = i+1;
	//					iEnd2 = iStart2 + iWndWidth -1;
	//					bFindSecondSlope = TRUE;
	//				}
	//				else
	//				{
	//					if (fabs(dCurCoef[1]-fFirstSlope) > fabs(fFirstSlope) * 0.5)
	//					{
	//						iEnd2 = i + iWndWidth;
	//						break;
	//					}
	//				}
	//			}
	//		}

	//		if (iEnd1-iStart1 > nPtCount / 2 || iEnd1-iStart1 > iEnd2 - iStart2)
	//		{
	//			iOutStart = iStart1;
	//			iOutEnd = iEnd1;
	//		}
	//		else
	//		{
	//			iOutStart = iStart2;
	//			iOutEnd = iEnd2;
	//		}
	//	}
	//}
}


int CDataAnalysisPCR::CalcLegalLineFitParam(int nPtCount, double *pInputX, double *pInputY, double *dReturn, int &iMinPtCount, double dMinR)
{
	double* pdTempX = new double[nPtCount];
	double* pdTempY = new double[nPtCount];

	double dMaxRParam[3] = {0};
	memset(dMaxRParam, 0, sizeof(double) * 3);
	int iStartPos = 0;
	BOOL bFindLegal = FALSE;

	int iTempCount = iMinPtCount;
	int iTempStart = nPtCount - iTempCount;

	double dTempCoef[3] = {0};
	while (TRUE)
	{
		for(int i=0; i<iTempCount; i++)
		{
			pdTempX[i] = pInputX[i+iTempStart];
			pdTempY[i] = pInputY[i+iTempStart];
		}

		LinearFit(pdTempX, pdTempY, iTempCount, dTempCoef);

		if (bFindLegal)
		{
			if (dTempCoef[2]*dTempCoef[2] > dMinR)
			{
				memcpy(dMaxRParam, dTempCoef, sizeof(double) * 3);
				iStartPos = iTempStart;
				iMinPtCount = iTempCount;
				iTempCount += 1;
			}
		}
		else
		{
			if (dTempCoef[2]*dTempCoef[2] > dMinR)
			{
				memcpy(dMaxRParam, dTempCoef, sizeof(double) * 3);
				iStartPos = iTempStart;
				bFindLegal = TRUE;
				iMinPtCount = iTempCount;
				iTempCount += 1;
			}
			else if (dTempCoef[2]*dTempCoef[2] > dMaxRParam[2])
			{
				memcpy(dMaxRParam, dTempCoef, sizeof(double) * 3);
				iStartPos = iTempStart;
			}
		}

		iTempStart -= 1;
		if (iTempStart < 0)
			break;
	}

	if (pdTempX != NULL)
	{
		delete pdTempX;
		pdTempX = NULL;
	}
	if (pdTempY != NULL)
	{
		delete pdTempY;
		pdTempY = NULL;
	}

	memcpy(dReturn, dMaxRParam, sizeof(double) * 3);

	return iStartPos;
}

BOOL CDataAnalysisPCR::FindPeakAmlBySlope(int nPtCount, double *pInputX, double *pInputY, int iWndWidth)
{
	BOOL bFindPeak = FALSE;

	double dPreCoef[3] = {0};
	double dCurCoef[3] = {0};
	double* pdTempX = new double[iWndWidth];
	double* pdTempY = new double[iWndWidth];

	for (int i=3; i<nPtCount-iWndWidth+1; i++)
	{
		BOOL bContinueRise = TRUE;
		for (int j=0; j<iWndWidth-1; j++)
		{
			if (pInputY[j+i+1] < pInputY[j+i])
			{
				bContinueRise = FALSE;
				break;
			}
		}

		if (bContinueRise) // 计算前后点的斜率
		{
			int iTempCount = iWndWidth;
			for(int ii=0; ii<iTempCount; ii++)
			{
				pdTempX[ii] = pInputX[ii+i];
				pdTempY[ii] = pInputY[ii+i];
			}
			LinearFit(pdTempX, pdTempY, iTempCount, dCurCoef);
			if (dCurCoef[1] < 80)
				continue;

			if (i < iWndWidth)
			{
				iTempCount = i+1;
			}

			for(int ii=0; ii<iTempCount; ii++)
			{
				pdTempX[ii] = pInputX[i-iTempCount+1+ii];
				pdTempY[ii] = pInputY[i-iTempCount+1+ii];
			}
			LinearFit(pdTempX, pdTempY, iTempCount, dPreCoef);


			if (dCurCoef[1] - dPreCoef[1] > 50)
			{
				bFindPeak = TRUE;
				break;
			}
		}
	}

	delete pdTempX;
	delete pdTempY;

	return bFindPeak;
}

#include "StdAfx.h"
#include "TubeData.h"
#include "DeclaresPCR.h"
#include <cmath>
#include "PublicInfo.h"

// Safe access helpers to avoid vector out-of-range assertions
static inline double getValueSafe(const std::vector<double>& values, int index)
{
	if (index < 0)
		return 0.0;
	if (index >= (int)values.size())
		return 0.0;
	return values[index];
}

static inline double getValueSafe(const std::vector<UINT>& values, int index)
{
	if (index < 0)
		return 0.0;
	if (index >= (int)values.size())
		return 0.0;
	return static_cast<double>(values[index]);
}

CTubeData::CTubeData(void)
{
	m_nChannelCount = 0;
	m_pFluOriginal = NULL;
	m_pFluCrossTalk = NULL;
	m_pFluFilterMedian = NULL;
	m_pFluBaselineAdjust = NULL;
	m_pFluFilter = NULL;
	m_pFluTuibitidy = NULL;
}

CTubeData::~CTubeData(void)
{
	DeleteAll();
}

BOOL CTubeData::AllocateMemory(int nChannelCount)
{
	DeleteAll();

	m_nChannelCount = nChannelCount;
	m_viValueX.clear();
	m_pFluOriginal = new vector<UINT>[m_nChannelCount];
	m_pFluCrossTalk = new vector<double>[m_nChannelCount];
	m_pFluFilterMedian = new vector<double>[m_nChannelCount];
	m_pFluBaselineAdjust = new vector<double>[m_nChannelCount];
	m_pFluFilter = new vector<double>[m_nChannelCount];
	m_pFluTuibitidy = new vector<double>[m_nChannelCount];

	m_arrayAutoAdjust.RemoveAll();
	for (int i=0; i<nChannelCount; i++)
	{
		m_arrayAutoAdjust.Add(FALSE);
	}

	return BeAllAllocated();
}

void CTubeData::DeleteAll()
{
	m_nChannelCount = 0;
	m_viValueX.clear();
	if(NULL != m_pFluOriginal)
	{
		delete [] m_pFluOriginal;
		m_pFluOriginal = NULL;
	}
	if(NULL != m_pFluCrossTalk)
	{
		delete [] m_pFluCrossTalk;
		m_pFluCrossTalk = NULL;
	}
	if(NULL != m_pFluFilterMedian)
	{
		delete [] m_pFluFilterMedian;
		m_pFluFilterMedian = NULL;
	}
	if(NULL != m_pFluBaselineAdjust)
	{
		delete [] m_pFluBaselineAdjust;
		m_pFluBaselineAdjust = NULL;
	}
	if(NULL != m_pFluFilter)
	{
		delete [] m_pFluFilter;
		m_pFluFilter = NULL;
	}
	if(NULL != m_pFluTuibitidy)
	{
		delete [] m_pFluTuibitidy;
		m_pFluTuibitidy = NULL;
	}

	return;
}

BOOL CTubeData::BeAllAllocated()
{
	if(NULL == m_pFluOriginal)
	{
		return FALSE;
	}
	if(NULL == m_pFluCrossTalk)
	{
		return FALSE;
	}
	if(NULL == m_pFluFilterMedian)
	{
		return FALSE;
	}
	if(NULL == m_pFluBaselineAdjust)
	{
		return FALSE;
	}
	if(NULL == m_pFluFilter)
	{
		return FALSE;
	}
	if(NULL == m_pFluTuibitidy)
	{
		return FALSE;
	}

	return TRUE;
}

void CTubeData::AddCycleX(UINT nCycle)
{
	m_viValueX.push_back(nCycle);
}

void CTubeData::AddOriginalData(int nChannelNo, UINT nYValue)
{
	m_pFluOriginal[nChannelNo].push_back(nYValue);
	m_pFluCrossTalk[nChannelNo].push_back(nYValue);
	m_pFluFilterMedian[nChannelNo].push_back(nYValue);
	m_pFluBaselineAdjust[nChannelNo].push_back(nYValue);
	m_pFluFilter[nChannelNo].push_back(nYValue);
	m_pFluTuibitidy[nChannelNo].push_back(nYValue);
}

double CTubeData::GetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo)
{
	double dReturn = 0;
	if (nChannelNo < 0 || nChannelNo >= m_nChannelCount)
	{
		return 0.0;
	}
	switch(nType)
	{
	case FLU_ORIGINAL:
		dReturn = getValueSafe(m_pFluOriginal[nChannelNo], nPointNo);
		break;
	case FLU_CROSSTALK:
		dReturn = getValueSafe(m_pFluCrossTalk[nChannelNo], nPointNo);
		break;
	case FLU_FILTER_MEDIAN:
		dReturn = getValueSafe(m_pFluFilterMedian[nChannelNo], nPointNo);
		break;
	case FLU_BASELINE_ADJUST:
		dReturn = getValueSafe(m_pFluBaselineAdjust[nChannelNo], nPointNo);
		break;
	case FLU_FILTER:
		dReturn = getValueSafe(m_pFluFilter[nChannelNo], nPointNo);
		break;
	case FLU_TURBIDITY:
		dReturn = getValueSafe(m_pFluTuibitidy[nChannelNo], nPointNo);
		break;
	case LOG_FLU_TURBIDITY:
		dReturn = getValueSafe(m_pFluTuibitidy[nChannelNo], nPointNo);
		dReturn = ConvertToLog10(dReturn);
		break;

	default:
		break;
	}

	return dReturn;
}

void CTubeData::SetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo, double dYValue)
{
	switch(nType)
	{
	case FLU_ORIGINAL:
		m_pFluOriginal[nChannelNo][nPointNo] = (UINT)dYValue; 
		break;
	case FLU_CROSSTALK:
		m_pFluCrossTalk[nChannelNo][nPointNo] = dYValue; 
		break;
	case FLU_FILTER_MEDIAN:
		m_pFluFilterMedian[nChannelNo][nPointNo] = dYValue; 
		break;
	case FLU_BASELINE_ADJUST:
		m_pFluBaselineAdjust[nChannelNo][nPointNo] = dYValue; 
		break;
	case FLU_FILTER:
		m_pFluFilter[nChannelNo][nPointNo] = dYValue; 
		break;
	case FLU_TURBIDITY:
		m_pFluTuibitidy[nChannelNo][nPointNo] = dYValue; 
		break;
	default:
		break;
	}
}

void CTubeData::GetCycleX(int nCycleCount, double* pdValueX, BOOL bConstantPCR)
{
	for(int i=0; i<nCycleCount; i++)
	{
		if (bConstantPCR)
		{
			pdValueX[i] = m_viValueX[i] / 1000.0; // Ĭ�ϰ�����ʾ
		}
		else
		{
			pdValueX[i] = m_viValueX[i];
		}
	}
}

double CTubeData::GetCycleX(int iCycleId, BOOL bConstantPCR)
{
	double dValueX = 0;
	if(iCycleId >= m_viValueX.size())
	{
		return dValueX;
	}

	if (bConstantPCR)
	{
		dValueX = m_viValueX[iCycleId] / 1000.0; // Ĭ�ϰ�����ʾ
	}
	else
	{
		dValueX = m_viValueX[iCycleId];
	}
	return dValueX;
}

UINT CTubeData::GetCurrentCycleX()
{
	int iCycle = m_viValueX.size() -1;
	return m_viValueX[iCycle];
}


void CTubeData::GetChannelFluY(eDataType nType, int nChannel, int nCycleCount, double* pdYValue)
{
	// Clamp to the actual data length to prevent out-of-range
	int available = 0;
	if (nChannel >= 0 && nChannel < m_nChannelCount)
	{
		available = (int)m_pFluOriginal[nChannel].size();
	}
	int count = nCycleCount < available ? nCycleCount : available;
	for(int i=0; i<count; i++)
	{
		pdYValue[i] = GetFluYValueBy(nType, nChannel, i);
	}
	for (int i=count; i<nCycleCount; ++i)
	{
        // Pad remainder with 0 to keep caller's buffer consistent
		pdYValue[i] = 0.0;
	}
}

void CTubeData::SetChannelFluY(eDataType nType, int nChannel, int nCycleCount, double* pdYValue)
{
	for(int i=0; i<nCycleCount; i++)
	{
		SetFluYValueBy(nType, nChannel, i, pdYValue[i]);
	}
}

double CTubeData::GetChannelCycleFluY(eDataType nType, int nChannel, int nCycleId)
{
	return GetFluYValueBy(nType, nChannel,nCycleId);
}

void CTubeData::FluCopyTo(int nChannelCount, int nCycleCount, vector<UINT>* pBase, vector<double>* pTarget)
{
	for (int i=0; i<nChannelCount; i++)
	{
		for (int j=0; j<nCycleCount; j++)
		{
			pTarget[i][j] = pBase[i][j];
		}
	}
}

void CTubeData::FluCopyTo(int nChannelCount, int nCycleCount, vector<double>* pBase, vector<double>* pTarget)
{
	for (int i=0; i<nChannelCount; i++)
	{
		for (int j=0; j<nCycleCount; j++)
		{
			pTarget[i][j] = pBase[i][j];
		}
	}
}

BOOL CTubeData::WriteFile(CFile* pFile)
{
	int nCount = m_viValueX.size();
	pFile->Write((const char*)&nCount, sizeof(int));
	pFile->Write((const char*)m_viValueX.data(), sizeof(UINT)*nCount);

	for(int i=0; i<m_nChannelCount; i++)
	{
		nCount = m_pFluOriginal[i].size();
		pFile->Write((const char*)&nCount, sizeof(int));
		pFile->Write((const char*)m_pFluOriginal[i].data(), sizeof(UINT)*nCount);
	}

	return TRUE;
}

BOOL CTubeData::ReadFile(CFile* pFile,WORD wVersion)
{
	int nCount = 0;
	pFile->Read(&nCount, sizeof(int));
	UINT* pValueX = new UINT[nCount];
	pFile->Read(pValueX, sizeof(UINT)*nCount);
	for (int i=0; i<nCount; i++)
	{
		m_viValueX.push_back(pValueX[i]);
	}

	UINT* pOrigData = NULL;
	for (int k=0; k<m_nChannelCount; k++)
	{
		pFile->Read(&nCount, sizeof(int));
		if (pOrigData == NULL)
		{
			pOrigData = new UINT[nCount];
		}
		pFile->Read(pOrigData, sizeof(UINT)*nCount);
		for (int x=0; x<nCount; x++)
		{
			AddOriginalData(k, pOrigData[x]);
		}
	}

	SAFE_DELETE(pValueX);
	SAFE_DELETE(pOrigData);

	return TRUE;
}

// ���и�ͨ���������ݴ���
void CTubeData::FluChannelCrosstalkAdjust(CTubeInfo* pTubeInfo, float** pCrosstalkMatrix)
{
	int nCycleCount = GetPointCout();

	for (int i=0; i<nCycleCount; i++)
	{
		int iTargetCount = pTubeInfo->GetTargetCount();
		for(int j=0; j<iTargetCount; j++)
		{
			CDyeTargetInfo* pTargetCol = pTubeInfo->GetTargetInfo(j);
			if (!pTargetCol->GetBeSelected())
				continue;

			int nCol = pTargetCol->GetChannel() -1;
			m_pFluCrossTalk[nCol][i] = m_pFluOriginal[nCol][i];

			for (int x=0; x<iTargetCount; x++)
			{
				CDyeTargetInfo* pTargetRow = pTubeInfo->GetTargetInfo(x);
				if (!pTargetCol->GetBeSelected())
					continue;

				int nRow = pTargetRow->GetChannel() -1;
				if (nCol == nRow)
					continue;

				float fRatio = pCrosstalkMatrix[nRow][nCol];
				double value1 = m_pFluCrossTalk[nCol][i];
				double value2 = m_pFluOriginal[nRow][i];
				double value3 = m_pFluCrossTalk[nCol][i] - m_pFluOriginal[nRow][i] * fRatio;
				m_pFluCrossTalk[nCol][i] -= m_pFluOriginal[nRow][i] * fRatio;

				//��ֹ����ϵ���ϴ󣬵��²���������������20220413��������
				if(m_pFluCrossTalk[nCol][i] < 0)
				{
					m_pFluCrossTalk[nCol][i] = 10;
				}
			}
		}
	}
}

int CTubeData::GetDataNum(int iChannelNo)
{
	if(m_nChannelCount < 1)
	{
		return 0;
	}

	if(NULL == m_pFluOriginal)
	{
		return 0;
	}

	return m_pFluOriginal[iChannelNo].size();
}

double CTubeData::GetMaxValueY(eDataType nDataType, int iChannel)
{
	int nCycleCount = GetPointCout();
	if(nCycleCount < 1)
		return 0;

	CPeakAmlCheckParam peakAmlChkParam;

	double *pdTempX = new double[nCycleCount];
	double *pdTempY = new double[nCycleCount];
	if(NULL == pdTempX || NULL == pdTempY)
		goto GAST_END;

	GetChannelFlu(nDataType, iChannel - 1,nCycleCount, pdTempX,pdTempY);

	double dReturn = -1000;
	for(int k = 2;k < nCycleCount-1;++k)
	{
		if(pdTempY[k] > dReturn)
		{
			dReturn = pdTempY[k];
		}
	}

GAST_END:
	if(NULL != pdTempX)
	{
		delete [] pdTempX;
	}
	if(NULL != pdTempY)
	{
		delete [] pdTempY;
	}
	return dReturn;
}

// �����˲��������������˲�
void CTubeData::FluDigitalFilterBy(CTubeInfo* pTubeInfo,CPtrArray* pFilterItem,int iFlag,CDataAnalysisPCR* pAlgorithm, int iDivideFlag)
{
	int iCalibrateChannel = pTubeInfo->GetCalibrateChannel();
	int iTargetCount = pTubeInfo->GetTargetCount();
	for(int i = 0;i < iTargetCount;++i)
	{
		CDyeTargetInfo* pTarget = pTubeInfo->GetTargetInfo(i);
		if (pTarget->GetBeSelected() && pTarget->GetChannel() != iCalibrateChannel)
		{
			int nChannelNo = pTarget->GetChannel() -1;
			FluDigitalFilterByChannel(nChannelNo,pFilterItem,iFlag,pAlgorithm, iDivideFlag);
		}
	}

	//�ɰ��PS96������IVDģʽ�£�Ĭ��У׼ͨ��δѡ�У����ܴ��ڵ�6ͨ�����˲�һ�ε����
	if(iCalibrateChannel > 0)
	{
		FluDigitalFilterByChannel(iCalibrateChannel-1,pFilterItem,iFlag,pAlgorithm, iDivideFlag);
	}

	return;
}

// ָ��ͨ���˲�
void CTubeData::FluDigitalFilterByChannel(int iChannelNo,CPtrArray* pFilterItem,int iFlag,CDataAnalysisPCR* pAlgorithm, int iDivideFlag)
{
	int nCycleCount = GetPointCout();
	if(nCycleCount < 2)
	{
		return;
	}

	int iFilterCount = pFilterItem->GetCount();
	int iByteNum = sizeof(double) * nCycleCount;

	double *pDTemp = new double[nCycleCount];
	if(NULL == pDTemp)
	{
		goto FDFBC_END;
	}
	double *pDReturn = new double[nCycleCount];
	if(NULL == pDReturn)
	{
		goto FDFBC_END;
	}

	if (iDivideFlag == 2)
	{
		GetChannelFlu(FLU_TURBIDITY,iChannelNo,nCycleCount,NULL,pDReturn);
	}
	else
		GetChannelFlu(FLU_CROSSTALK,iChannelNo,nCycleCount,NULL,pDReturn);

	memcpy(pDTemp,pDReturn,iByteNum);

	BOOL bFiltering = FALSE;
	for(int j = 0;j < iFilterCount;++j)
	{
		tagFilterItem* pItem = (tagFilterItem*)pFilterItem->GetAt(j);
		if(FILTER_NONE == pItem->nType)
		{
			continue;
		}

		if(pItem->iFilterUser == iFlag)
		{
			for(int m = 0;m < pItem->iTimes;++m)
			{
				switch(pItem->nType)
				{
				case FILTER_MEDIAN://��λֵ�˲�
					if (iDivideFlag != 2)
					{
						bFiltering = TRUE;
						pAlgorithm->DigitalFilter(pDTemp,pDReturn,nCycleCount,CDataAlgorithmPCR::FILTERTYPE_MEDIAN, pItem->iPara1);
					}
					break;
				case FILTER_FM_BN: // ��ֵ�˲�
					if (iDivideFlag != 1)
						pAlgorithm->KdsptForwardMBackN_New(nCycleCount,pDTemp,pDReturn,pItem->iPara1,pItem->iPara2);
					break;
				default:
					ASSERT(FALSE);
					break;
				}
				memcpy(pDTemp,pDReturn,iByteNum); 
			}
		}
	}


	// ������ʼ��������ݣ���֤ƫ���С
	if (bFiltering)
	{
		if (nCycleCount > 3)
		{
			pDReturn[0] = pDReturn[2] + (pDReturn[0]-pDReturn[2]) * 0.1;
			pDReturn[1] = pDReturn[2] + (pDReturn[1]-pDReturn[2]) * 0.1;
		}
	}

	if (iDivideFlag == 1)
	{
		SetChannelFlu(FLU_FILTER_MEDIAN,iChannelNo,nCycleCount,pDReturn);
		SetChannelFlu(FLU_FILTER,iChannelNo,nCycleCount,pDReturn);
		SetChannelFlu(FLU_TURBIDITY,iChannelNo,nCycleCount,pDReturn);
		SetChannelFlu(FLU_BASELINE_ADJUST,iChannelNo,nCycleCount,pDReturn);
	}
	else
	{
		SetChannelFlu(FLU_FILTER,iChannelNo,nCycleCount,pDReturn);
	}

FDFBC_END:
	if(NULL != pDTemp)
	{
		delete [] pDTemp;
	}
	if(NULL != pDReturn)
	{
		delete [] pDReturn;
	}

	return;
}

void CTubeData::GetChannelFlu(eDataType Type, int nChannel, int nCycleCount, double* pdXValue, double* pdYValue)
{
	// Clamp to actual data length
	int available = 0;
	if (nChannel >= 0 && nChannel < m_nChannelCount)
	{
		available = (int)m_pFluOriginal[nChannel].size();
	}
	int count = nCycleCount < available ? nCycleCount : available;
	for(int i=0; i<count; i++)
	{
		if (pdXValue != NULL)
		{
			pdXValue[i] = i + 1;
		}
		pdYValue[i] = GetFluYValueBy(Type, nChannel, i);
	}
	for (int i=count; i<nCycleCount; ++i)
	{
		if (pdXValue != NULL)
		{
			pdXValue[i] = i + 1;
		}
		pdYValue[i] = 0.0;
	}
}

void CTubeData::SetChannelFlu(eDataType Type, int nChannel, int nCycleCount, double* pdYValue)
{
	for(int i=0; i<nCycleCount; i++)
	{
		SetFluYValueBy(Type, nChannel, i, pdYValue[i]);
	}
}

BOOL CTubeData::HasData()
{
	if (m_pFluOriginal != NULL)
	{
		if (m_pFluOriginal[0].size() > 0)
			return TRUE;
	}

	return FALSE;
}

double CTubeData::ConvertToLog10(double dInput)
{
	if(dInput < 0)
	{
		return log10(-dInput);
	}
	else if(dInput > 0)
	{
		return log10(dInput);
	}
	else 
		return 0;
}


//*****************************************************************************************************************
// ������ӫ������
CTubeAmplifyData::CTubeAmplifyData(void)
{
	m_pFluNormalize = NULL;
	m_pFluDeltaRn = NULL;
}

CTubeAmplifyData::~CTubeAmplifyData(void)
{
	DeleteAll();
}

BOOL CTubeAmplifyData::AllocateMemory(int nChannelCount)
{
	CTubeData::AllocateMemory(nChannelCount);

	DeleteAll();
	m_nChannelCount = nChannelCount;

	m_pFluNormalize = new vector<double>[m_nChannelCount];
	m_pFluDeltaRn = new vector<double>[m_nChannelCount];

	return BeAllAllocated();
}

void CTubeAmplifyData::DeleteAll()
{
	m_nChannelCount = 0;

	if(NULL != m_pFluNormalize)
	{
		delete [] m_pFluNormalize;
		m_pFluNormalize = NULL;
	}

	if(NULL != m_pFluDeltaRn)
	{
		delete [] m_pFluDeltaRn;
		m_pFluDeltaRn = NULL;
	}

	return;
}

void CTubeAmplifyData::AddOriginalData(int nChannelNo, UINT nYValue)
{
	CPublicInfo* pInfo = CPublicInfo::GetInstance();
	if (pInfo->GetFluoDataAutoAdjust())
	{
		BOOL bAutoAdjust = m_arrayAutoAdjust.GetAt(nChannelNo);
		if (bAutoAdjust)
		{
			if (nYValue < pInfo->GetFluoDataAdjustLimited())
			{
				nYValue += pInfo->GetFluoDataMaxLimited();
			}
		}
		else
		{
			int iDafaCount = GetDataNum(nChannelNo);
			if (nYValue == pInfo->GetFluoDataMaxLimited())
			{
				m_arrayAutoAdjust.SetAt(nChannelNo, TRUE);
			}
			else if (iDafaCount > 0)
			{
				UINT nLastValue = GetFluYValueBy(FLU_ORIGINAL, nChannelNo, iDafaCount-1);
				if (nYValue+40000 < nLastValue)
				{
					m_arrayAutoAdjust.SetAt(nChannelNo, TRUE);
					nYValue += pInfo->GetFluoDataMaxLimited();
				}
			}
		}
	}

	CTubeData::AddOriginalData(nChannelNo, nYValue);
	m_pFluNormalize[nChannelNo].push_back(nYValue);
	m_pFluDeltaRn[nChannelNo].push_back(nYValue);

	return;
}


double CTubeAmplifyData::GetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo)
{
	double dReturn = 0;
	if (nChannelNo < 0 || nChannelNo >= m_nChannelCount)
	{
		return 0.0;
	}
	switch(nType)
	{
	case FLU_NORMALIZATE:
		dReturn = getValueSafe(m_pFluNormalize[nChannelNo], nPointNo);
		break;
	case FLU_DELTARN:
		dReturn = getValueSafe(m_pFluDeltaRn[nChannelNo], nPointNo);
		break;

	case LOG_FLU_TURBIDITY:
		dReturn = getValueSafe(m_pFluTuibitidy[nChannelNo], nPointNo);
		dReturn = ConvertToLog10(dReturn);
		break;
	case LOG_FLU_NORMALIZATE:
		dReturn = getValueSafe(m_pFluNormalize[nChannelNo], nPointNo);
		dReturn = ConvertToLog10(dReturn);
		break;
	case LOG_FLU_DELTARN:
		dReturn = getValueSafe(m_pFluDeltaRn[nChannelNo], nPointNo);
		dReturn = ConvertToLog10(dReturn);
		break;
	default:
		dReturn = CTubeData::GetFluYValueBy(nType, nChannelNo, nPointNo);
		break;
	}
	return dReturn;
}

void CTubeAmplifyData::SetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo, double dYValue)
{
	switch(nType)
	{
	case FLU_NORMALIZATE:
		m_pFluNormalize[nChannelNo][nPointNo] = dYValue; 
		break;
	case FLU_DELTARN:
		m_pFluDeltaRn[nChannelNo][nPointNo] = dYValue; 
		break;
	default:
		CTubeData::SetFluYValueBy(nType, nChannelNo, nPointNo, dYValue);
		break;
	}
}

void CTubeAmplifyData::FluDigitalFilter(CTubeInfo* pTubeInfo,CPtrArray* pFilterItem,CDataAnalysisPCR* pAlgorithm, int iDivideFlag)
{
	FluDigitalFilterBy(pTubeInfo,pFilterItem,1,pAlgorithm, iDivideFlag);
}

// ����б�ʺͻ������ݵ���
void CTubeAmplifyData::FluoAdjustBaseline(CTubeInfo* pTubeInfo, CDataAnalysisPara* pAnalysisPara,CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity)
{
	int nCycleCount = GetPointCout();
	FluCopyTo(m_nChannelCount, nCycleCount, m_pFluFilterMedian, m_pFluBaselineAdjust);
	if(nCycleCount < 8)
	{
		return;
	}

	double *pDTempX = new double[nCycleCount];
	if(NULL == pDTempX)
	{
		goto FNAD_END;
	}
	double *pDTempY = new double[nCycleCount];
	if(NULL == pDTempY)
	{
		goto FNAD_END;
	}
	double *pDReturn = new double[nCycleCount];
	if(NULL == pDReturn)
	{
		goto FNAD_END;
	}

	int iCycleOfMaxMOTRatio = CalcCycleOfMaxMOTRatio(nCycleCount, pTurbidity, pAlgorithm);
	if (iCycleOfMaxMOTRatio < 5)
		iCycleOfMaxMOTRatio = nCycleCount;

	int iDyeCount = pTubeInfo->GetTargetCount();
	for(int i = 0;i < iDyeCount;++i)
	{
		CDyeTargetInfo* pDyeInfo = pTubeInfo->GetTargetInfo(i);
		if(NULL == pDyeInfo || pDyeInfo->GetChannel() == pTurbidity->GetChannelMOT() || !pDyeInfo->GetBeSelected())
			continue;

		int iChannelNo = pDyeInfo->GetChannel() - 1;
		GetChannelFlu(FLU_FILTER_MEDIAN, iChannelNo, nCycleCount, pDTempX, pDTempY);

		if(pTurbidity->UseCalibrate(pDyeInfo->GetChannel()))
		{
			int iCurStart = 3;
			int iCurEnd = 8;
			BOOL bHasCt = FALSE;
			BOOL bChangeBase = FALSE;

			tagAnaAmplifyPara* pAmplifyPara = pAnalysisPara->GetAmplifyParaBy(pDyeInfo->GetTargetName(),pDyeInfo->GetDyeName(),pDyeInfo->GetChannel());
			if(NULL == pAmplifyPara)
			{
				pAmplifyPara = pAnalysisPara->GetAmplifyParaBy(iChannelNo);
			}
			if(NULL != pAmplifyPara)
			{
				CPeakAmlCheckParam peakAmlChkParam;
				double dSndDrvCt = 0;
				bHasCt = pAlgorithm->IsPeakAml(pDTempX,pDTempY,nCycleCount, peakAmlChkParam, dSndDrvCt);
				pDyeInfo->SetHasSndDrvCt(bHasCt);
				pDyeInfo->SetSndDrvCt(dSndDrvCt);

				if(pAmplifyPara->bAutoBaseline)
				{
					bChangeBase = TRUE;
					// ��Ҫ���������˲�
					pAlgorithm->DigitalFilter(pDTempY, pDReturn, nCycleCount,CDataAlgorithmPCR::FILTERTYPE_MEDIAN, 5);
					memcpy(pDTempY, pDReturn, sizeof(double) * nCycleCount);
					for (int i=0; i<3; ++i)
					{
						pAlgorithm->KdsptForwardMBackN_New(nCycleCount, pDTempY, pDReturn, 2, 2);
						memcpy(pDTempY, pDReturn,sizeof(double) * nCycleCount);
					}

					if(bHasCt)
					{
						pAlgorithm->CalcAutoBaselinePosBySndDrvCt(dSndDrvCt, nCycleCount,pDTempX,pDTempY,iCurStart,iCurEnd);
						if (iCurEnd - iCurStart < dSndDrvCt / 2)
						{
							iCurStart = iCurEnd - int(dSndDrvCt / 2);
							if (iCurStart < 1)
							{
								iCurStart = 1;
							}
						}

						if (iCurEnd > iCycleOfMaxMOTRatio)
							iCurEnd = iCycleOfMaxMOTRatio;
					}
					else
					{
						iCurStart = 1;
						iCurEnd = nCycleCount;
						pAlgorithm->CalcBaselinePosOfNoCt(nCycleCount, pDTempX, pDTempY, iCurStart, iCurEnd);
						if (nCycleCount > 20 && iCurEnd - iCurStart > nCycleCount*2/3)
						{
							iCurStart = iCurEnd - nCycleCount*2/3;
						}
					}
				}
				else
				{
					iCurStart = pAmplifyPara->iBaselineStart;
					iCurEnd = pAmplifyPara->iBaselineEnd;
				}
			}

			// ���ݻ�������������߽���
			if (bChangeBase)
			{
				GetChannelFlu(FLU_FILTER_MEDIAN, iChannelNo, nCycleCount, pDTempX, pDTempY);
			}
			pAlgorithm->AdjustFluValueByBaseline(nCycleCount,pDTempX,pDTempY,iCurStart,iCurEnd);
			pDyeInfo->SetBaselineStart(iCurStart);
			pDyeInfo->SetBaselineEnd(iCurEnd);
		}

		SetChannelFlu(FLU_BASELINE_ADJUST,iChannelNo,nCycleCount,pDTempY);
		SetChannelFlu(FLU_FILTER,iChannelNo,nCycleCount,pDTempY);
	}

FNAD_END:
	if(NULL != pDTempX)
	{
		delete [] pDTempX;
	}
	if(NULL != pDTempY)
	{
		delete [] pDTempY;
	}
	if(NULL != pDReturn)
	{
		delete [] pDReturn;
	}

	return;

}

int CTubeAmplifyData::CalcCycleOfMaxMOTRatio(int iCycleCount, CCalibrateParaTurbidity* pTurbidity, CDataAnalysisPCR* pAlgorithm)
{
	double *pdXValue = new double[iCycleCount];
	double* pdYValue = new double[iCycleCount];
	GetChannelFlu(FLU_TURBIDITY, pTurbidity->GetChannelMOT() - 1, iCycleCount, pdXValue, pdYValue);
	int iCycleMOT = pAlgorithm->CalcCycleOfMaxMOTRatioBy(iCycleCount, pdXValue, pdYValue, pTurbidity->GetMinRatio());

	delete pdXValue;
	delete pdYValue;

	return iCycleMOT;
}


void CTubeAmplifyData::FluTurbidityAdjust(CTubeInfo* pTubeInfo,CCalibrateParaTurbidity* pTurbidity,CDataAnalysisPCR* pAlgorithm)
{
	int nCycleCount = GetPointCout();
	if(NULL == pTurbidity || nCycleCount < 8)
	{
		return;
	}

	int nChannelMOT = pTurbidity->GetChannelMOT() - 1;
	if(nChannelMOT >= m_nChannelCount || nChannelMOT < 0)
	{
		return;
	}

	double *pdXValue = new double[nCycleCount];
	if(NULL == pdXValue)
	{
		goto FTA_END;
	}
	double *pdYValue = new double[nCycleCount];
	if(NULL == pdYValue)
	{
		goto FTA_END;
	}

	// �����˲����ȫ��ͨ��Ctֵ�����ҳ�������ƽ̨�ڿ�ʼλ��
	CalcAllCtPosBySndDerivative(pTubeInfo, pTurbidity->GetChannelMOT(), pAlgorithm);
	int iStartDescendPos = CalcFluoDescendPosByCtAndMOT(pTubeInfo, nCycleCount, pAlgorithm);

	double *pdReturnY = new double[nCycleCount];
	if(NULL == pdReturnY)
	{
		goto FTA_END;
	}

	int iDyeCount = pTubeInfo->GetTargetCount();
	for(int i = 0;i < iDyeCount;++i)
	{
		CDyeTargetInfo* pDyeInfo = pTubeInfo->GetTargetInfo(i);
		if(!pDyeInfo->GetBeSelected())
		{
			continue;
		}
		if(!pTurbidity->UseCalibrate(pDyeInfo->GetChannel()))
		{
			continue;
		}

		int iChannelNo = pDyeInfo->GetChannel() - 1;
		GetChannelFlu(FLU_BASELINE_ADJUST,iChannelNo,nCycleCount,pdXValue,pdYValue);

		//�����Ƕȵ���ֵ1
		double dAjustRatio = pTurbidity->GetAdjustRatio(pDyeInfo->GetDyeName());
		pAlgorithm->CalcFluTurbidityAdjust(iStartDescendPos, int(pDyeInfo->GetSndDrvCt()), dAjustRatio,nCycleCount,pdXValue,pdYValue,pdReturnY);
		SetChannelFlu(FLU_TURBIDITY,iChannelNo,nCycleCount,pdReturnY);
	}


	if(NULL != pdReturnY)
	{
		delete [] pdReturnY;
	}

FTA_END:
	if(NULL != pdXValue)
	{
		delete [] pdXValue;
	}
	if(NULL != pdYValue)
	{
		delete [] pdYValue;
	}
}

void CTubeAmplifyData::CalcAllCtPosBySndDerivative(CTubeInfo* pTubeInfo, int iChannelMOT, CDataAnalysisPCR* pAlgorithm)
{
	int nCycleCount = GetPointCout();
	double *pdXValue = new double[nCycleCount];
	double* pdYValue = new double[nCycleCount];
	if(NULL == pdXValue || pdYValue == NULL)
	{
		SAFE_DELETE(pdXValue);
		SAFE_DELETE(pdYValue);
		return;
	}

	int iDyeCount = pTubeInfo->GetTargetCount();
	for(int i = 0;i < iDyeCount;++i)
	{
		CDyeTargetInfo* pDyeInfo = pTubeInfo->GetTargetInfo(i);
		if(!pDyeInfo->GetBeSelected())
			continue;
		if(pDyeInfo->GetChannel() == iChannelMOT)
			continue;

		int iChannelNo = pDyeInfo->GetChannel() - 1;
		GetChannelFlu(FLU_BASELINE_ADJUST, iChannelNo, nCycleCount, pdXValue, pdYValue);

		//double dCt = pAlgorithm->CalcCtPos(nCycleCount, pdXValue, pdYValue);
		//pDyeInfo->SetSndDrvCt(dCt);
		double dCt = pDyeInfo->GetSndDrvCt();

		int iCycleStart = -1;
		if (dCt > 0)
		{
			iCycleStart = pAlgorithm->FindStartPlateauCycle(nCycleCount, pdXValue, pdYValue, dCt);
		}
		pDyeInfo->SetCycleOfPlateau(iCycleStart);
	}

	delete pdXValue;
	delete pdYValue;
}

int CTubeAmplifyData::CalcFluoDescendPosByCtAndMOT(CTubeInfo* pTubeInfo, int iCycleOfMOT, CDataAnalysisPCR* pAlgorithm)
{
	int iStartDecend = iCycleOfMOT;

	int iDyeCount = pTubeInfo->GetTargetCount();
	for(int i = 0;i < iDyeCount;++i)
	{
		CDyeTargetInfo* pDyeInfo = pTubeInfo->GetTargetInfo(i);
		if (pDyeInfo->GetCycleOfPlateau() <= 0)
			continue;

		if (iStartDecend > pDyeInfo->GetCycleOfPlateau())
			iStartDecend = pDyeInfo->GetCycleOfPlateau();
	}

	return iStartDecend;
}


void CTubeAmplifyData::FluNormalizeAndDeltaRn(CTubeInfo* pTubeInfo,CDataAnalysisPara* pAnalysisPara,CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity)
{
	int nCycleCount = GetPointCout();
	if(nCycleCount < 1)
	{
		return;
	}

	int iDyeCount = pTubeInfo->GetTargetCount();
	double *pDTempX = new double[nCycleCount];
	if(NULL == pDTempX)
	{
		goto FNAD_END;
	}
	double *pDTempY = new double[nCycleCount];
	if(NULL == pDTempY)
	{
		goto FNAD_END;
	}
	double *pDReturn = new double[nCycleCount];
	if(NULL == pDReturn)
	{
		goto FNAD_END;
	}

	for(int i = 0;i < iDyeCount;++i)
	{
		CDyeTargetInfo* pDyeInfo = pTubeInfo->GetTargetInfo(i);
		if(NULL == pDyeInfo || !pDyeInfo->GetBeSelected() || pDyeInfo->GetChannel() == 6)
		{
			continue;
		}

		int iChannelNo = pDyeInfo->GetChannel() - 1;
		GetChannelFlu(FLU_FILTER,iChannelNo,nCycleCount,pDTempX,pDTempY);

		if(nCycleCount > 3 && pAlgorithm->CanNormalize(nCycleCount))
		{
			int iCurStart = 3;
			int iCurEnd = 8;
			BOOL bHasCt = FALSE;

			tagAnaAmplifyPara* pAmplifyPara = pAnalysisPara->GetAmplifyParaBy(pDyeInfo->GetTargetName(),pDyeInfo->GetDyeName(),pDyeInfo->GetChannel());
			if(NULL == pAmplifyPara)
			{
				pAmplifyPara = pAnalysisPara->GetAmplifyParaBy(iChannelNo);
			}
			if(NULL != pAmplifyPara)
			{
				if(pAmplifyPara->bAutoBaseline)
				{
					double dSndDrvCt = pDyeInfo->GetSndDrvCt();
					bHasCt = pDyeInfo->HasSndDrvCt();
					if (bHasCt)
					{
						pAlgorithm->CalcAutoBaselinePosBySndDrvCt(dSndDrvCt, nCycleCount,pDTempX,pDTempY,iCurStart,iCurEnd);
						//ASSERT(iCurStart == pDyeInfo->GetBaselineStart());
						//ASSERT(iCurEnd == pDyeInfo->GetBaselineEnd());
					}
					else
					{
						iCurStart = 1;
						iCurEnd = nCycleCount;
						pAlgorithm->CalcBaselinePosOfNoCt(nCycleCount, pDTempX, pDTempY, iCurStart, iCurEnd);
						if (nCycleCount > 20 && iCurEnd - iCurStart > nCycleCount*2/3)
						{
							iCurStart = iCurEnd - nCycleCount*2/3;
						}

						//ASSERT(iCurStart == pDyeInfo->GetBaselineStart());
						//ASSERT(iCurEnd == pDyeInfo->GetBaselineEnd());
					}
				}
				else
				{
					iCurStart = pAmplifyPara->iBaselineStart;
					iCurEnd = pAmplifyPara->iBaselineEnd;
				}

			}

			if (pTurbidity->UseCalibrate(pDyeInfo->GetChannel()))
			{
				// �������߼�����ǰ������
				for (int x=iCurEnd; x>0; x--)
				{
					if (x>= nCycleCount)
					{
						continue;
					}
					pDTempY[x-1] = pDTempY[iCurEnd-1] + (pDTempY[x-1]-pDTempY[iCurEnd-1]) * 0.1;
				}

				if (!bHasCt)
				{
					for (int x=iCurEnd; x<nCycleCount; x++)
					{
						pDTempY[x] = pDTempY[iCurEnd-1] + (pDTempY[x]-pDTempY[iCurEnd-1]) * 0.1;
					}
				}
			}
			SetChannelFlu(FLU_FILTER,iChannelNo,nCycleCount,pDTempY);

			//pAlgorithm->AdjustFluValueByBaseline(nCycleCount,pDTempX,pDTempY,iCurStart,iCurEnd);
			double dAverage = pAlgorithm->CalcBaselineAverage(nCycleCount,pDTempX,pDTempY,iCurStart,iCurEnd);

			//��һ������1
			pAlgorithm->CalcNormalize(nCycleCount,pDTempY,pDReturn,dAverage);
			SetChannelFlu(FLU_NORMALIZATE,iChannelNo,nCycleCount,pDReturn);

			//DeltaRn����1
			pAlgorithm->CalcDeltaRn(nCycleCount, pDTempY, pDReturn, dAverage);
			// ����Atila���ޱҪ�󣬽�ƽ̨�ڵ����ݶ�����
			for (int i=0; i<iCurEnd; i++)
			{
				pDReturn[i] = 0;
			}
			SetChannelFlu(FLU_DELTARN, iChannelNo, nCycleCount, pDReturn);
		}
		else
		{
			//�����һ������
			SetChannelFlu(FLU_NORMALIZATE,iChannelNo,nCycleCount,pDTempY);
			//����DeltaRn����
			SetChannelFlu(FLU_DELTARN,iChannelNo,nCycleCount,pDTempY);
		}
	}

FNAD_END:
	if(NULL != pDTempX)
	{
		delete [] pDTempX;
	}
	if(NULL != pDTempY)
	{
		delete [] pDTempY;
	}
	if(NULL != pDReturn)
	{
		delete [] pDReturn;
	}

	return;
}

void CTubeAmplifyData::CalcFluDataByCtAndMOT(int nChannelNo, int nStartNo, int nCycleCount, double* pdYValue, double dChangedRatio)
{
	double dStartBase = pdYValue[nStartNo];
	double dCalcValue;

	for (int j=nStartNo+1; j<nCycleCount; j++)
	{
		if (pdYValue[j] > dStartBase)
		{
			dStartBase = pdYValue[j];
		}
		else
		{
			dCalcValue = dStartBase + (pdYValue[j] - dStartBase) * dChangedRatio;
			m_pFluTuibitidy[nChannelNo][j] = dCalcValue;
		}
	}
}

// ����ĳ��ָ���Ct
double CTubeAmplifyData::GenerateCt(eDataType nType, eMethodCalculateCt nMethod, tagAnaAmplifyPara* pAmplifyPara, CDataAnalysisPCR* pAlgorithm)
{
	double dReturn = 0.0;
	eMethodCalculateCt nCurMethod = nMethod;
	if (nCurMethod == METHOD_CT_USER)
	{
		nCurMethod = pAmplifyPara->nMethodCt;
	}

	int nCycleCount = GetPointCout();
	double *pdTempX = new double[nCycleCount];
	double *pdTempY = new double[nCycleCount];
	GetChannelFlu(nType, pAmplifyPara->iChannelNo-1, nCycleCount, pdTempX, pdTempY);

	if(METHOD_CT_SNDDEV == nCurMethod)	// ���׵�����
	{
		BOOL bAble = pAlgorithm->AbleToCalculateCt(nCycleCount, pdTempX, pdTempY, nType == FLU_NORMALIZATE);
		if(bAble)
		{
			dReturn = pAlgorithm->GenerateCtOfSndDerivative(nCycleCount, pdTempX, pdTempY);
		}
	}
	else if(METHOD_CT_SPLINE == nCurMethod)	// ��ֵ��
	{
		dReturn = GenerateCtOfSpline(nCycleCount, pdTempX, pdTempY, pAmplifyPara, pAlgorithm,nType);
	}
	else
	{
		ASSERT(FALSE);
	}

	delete [] pdTempX;
	delete [] pdTempY;

	if(_isnan(dReturn))
	{
		dReturn = nCycleCount;
	}

	//������쳣ֵ��Ctֵ����Ϊ���ݵ���
	if(dReturn < 1 || dReturn > nCycleCount)
	{
		dReturn = nCycleCount;
	}

	return dReturn;
}

double CTubeAmplifyData::GenerateCtOfSpline(int iCycleCount, double *pdInputX, double *pdInputY, tagAnaAmplifyPara* pAmplifyPara, CDataAnalysisPCR* pAlgorithm,eDataType nType)
{
	//ʹ�ò�ֵ������� ��ֵ�����ݵ����������ޡ�����֮�⣬�������CtֵΪ ���ݵ������N/A��
	double dThreshold = pAmplifyPara->dCalcNormalizeThreshold;
	if(FLU_DELTARN == nType)
	{
		dThreshold = pAmplifyPara->dCalcDeltaRnThreshold;
	}

	double dReturn = iCycleCount;
	dReturn = pAlgorithm->GenerateCtOfSpline(iCycleCount, pdInputX, pdInputY, dThreshold);
	if(dReturn > iCycleCount || dReturn < 1)
	{
		dReturn = iCycleCount;
	}

	return dReturn;
}

// Ϊ�Զ���������Ct
double CTubeAmplifyData::GenerateAutoSplineThreshold(eDataType nType,int iChannel,CDataAnalysisPCR* pAlgorithm)
{
	double dReturn = 0;

	int nCycleCount = GetPointCout();
	if(nCycleCount < 1)
	{
		return dReturn;
	}

	CPeakAmlCheckParam peakAmlChkParam;

	double *pdTempX = new double[nCycleCount];
	if(NULL == pdTempX)
	{
		goto GAST_END;
	}
	double *pdTempY = new double[nCycleCount];
	if(NULL == pdTempY)
	{
		goto GAST_END;
	}

	//GetChannelFlu(nType,iChannel - 1,nCycleCount,pdTempX,pdTempY);
	GetChannelFlu(FLU_TURBIDITY,iChannel - 1,nCycleCount,pdTempX,pdTempY);
	double dOrgCt = 0;
	if(pAlgorithm->IsPeakAml(pdTempX,pdTempY,nCycleCount,peakAmlChkParam, dOrgCt))
	{
		double dReturnCt = pAlgorithm->GenerateCtOfSndDerivative(nCycleCount,pdTempX,pdTempY);
		if (dReturnCt > 1 && dReturnCt <= nCycleCount) // �ҵ�Ct
		{
			GetChannelFlu(nType,iChannel-1, nCycleCount, pdTempX, pdTempY);
			pAlgorithm->Spline(pdTempX,pdTempY,nCycleCount,dReturnCt,dReturn);
		}
	}

GAST_END:
	if(NULL != pdTempX)
	{
		delete [] pdTempX;
	}
	if(NULL != pdTempY)
	{
		delete [] pdTempY;
	}
	return dReturn;
}

BOOL CTubeAmplifyData::BeAllAllocated()
{
	BOOL bReturn = CTubeData::BeAllAllocated();
	if (!bReturn)
	{
		return FALSE;
	}
	if(NULL == m_pFluNormalize)
	{
		return FALSE;
	}
	if(NULL == m_pFluDeltaRn)
	{
		return FALSE;
	}

	//if(NULL == m_pLogFluNormalize)
	//{
	//	return FALSE;
	//}
	//if(NULL == m_pLogFluDeltaRn)
	//{
	//	return FALSE;
	//}

	//if(NULL == m_pFluAfterCorrect)
	//{
	//	return FALSE;
	//}

	//if(NULL == m_pFluNormNoCP)
	//{
	//	return FALSE;
	//}
	//if(NULL == m_pFluRnNoCP)
	//{
	//	return FALSE;
	//}

	return TRUE;
}

//*****************************************************************************************************************
// �۽��ӫ������
CTubeMeltData::CTubeMeltData(void)
{
	m_pTem = NULL;
	m_pNormalize = NULL;
	m_pFstNegReciprocal = NULL;
	m_pGainData = NULL;
}

CTubeMeltData::~CTubeMeltData(void)
{
	DeleteAll();
}

BOOL CTubeMeltData::AllocateMemory(int nChannelCount)
{
	CTubeData::AllocateMemory(nChannelCount);

	DeleteAll();
	m_nChannelCount = nChannelCount;
	
	m_pTem = new vector<double>;
	m_pNormalize = new vector<double>[m_nChannelCount];

	m_pFstNegReciprocal = new vector<double>[m_nChannelCount];
	m_pGainData = new vector<double>[m_nChannelCount];

	return BeAllAllocated();
}

BOOL CTubeMeltData::BeAllAllocated()
{
	BOOL bReturn = CTubeData::BeAllAllocated();
	if (!bReturn)
	{
		return FALSE;
	}
	if(NULL == m_pTem)
	{
		return FALSE;
	}
	if(NULL == m_pNormalize)
	{
		return FALSE;
	}

	if(NULL == m_pFstNegReciprocal)
	{
		return FALSE;
	}
	if(NULL == m_pGainData)
	{
		return FALSE;
	}

	return TRUE;
}

void CTubeMeltData::DeleteAll()
{
	m_nChannelCount = 0; 

	if(NULL != m_pTem)
	{
		delete m_pTem;
		m_pTem = NULL;
	}

	if(NULL != m_pNormalize)
	{
		delete [] m_pNormalize;
		m_pNormalize = NULL;
	}

	if(NULL != m_pFstNegReciprocal)
	{
		delete [] m_pFstNegReciprocal;
		m_pFstNegReciprocal = NULL;
	}

	if(NULL != m_pGainData)
	{
		delete [] m_pGainData;
		m_pGainData = NULL;
	}
	return;
}


void CTubeMeltData::AddOriginalData(int nChannelNo,UINT nYValue,double dTem)
{
	if(nChannelNo < 0 || nChannelNo >= m_nChannelCount)
	{
		return;
	}

	CPublicInfo* pInfo = CPublicInfo::GetInstance();
	if (pInfo->GetFluoDataAutoAdjust())
	{
		BOOL bAutoAdjust = m_arrayAutoAdjust.GetAt(nChannelNo);
		if (bAutoAdjust)
		{
			if (nYValue < pInfo->GetFluoDataAdjustLimited())
			{
				nYValue += pInfo->GetFluoDataMaxLimited();
			}
		}
		else
		{
			if (nYValue == pInfo->GetFluoDataMaxLimited())
			{
				m_arrayAutoAdjust.SetAt(nChannelNo, TRUE);
			}
		}
	}

	CTubeData::AddOriginalData(nChannelNo, nYValue);
	if(0 == nChannelNo)	//��ֹ�¶����ݱ�����
	{
		(*m_pTem).push_back(dTem);
	}
	
	m_pNormalize[nChannelNo].push_back(nYValue);
	m_pFstNegReciprocal[nChannelNo].push_back(nYValue);
	m_pGainData[nChannelNo].push_back(nYValue);

	return;
}

double CTubeMeltData::GetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo)
{
	double dReturn = 0;
	if (nChannelNo < 0 || nChannelNo >= m_nChannelCount)
	{
		return 0.0;
	}
	switch(nType)
	{
	case MELT_NORMALIZE:
		dReturn = getValueSafe(m_pNormalize[nChannelNo], nPointNo);
		break;
	case MELT_FSTNEGRECIPROCAL:
		dReturn = getValueSafe(m_pFstNegReciprocal[nChannelNo], nPointNo);
		break;
	case MELT_GAINDATA:
		dReturn = getValueSafe(m_pGainData[nChannelNo], nPointNo);
		break;
	default:
		dReturn = CTubeData::GetFluYValueBy(nType, nChannelNo, nPointNo);
		break;
	}
	return dReturn;
}

void CTubeMeltData::SetFluYValueBy(eDataType nType, int nChannelNo, int nPointNo, double dYValue)
{
	switch(nType)
	{
	case MELT_NORMALIZE:
		m_pNormalize[nChannelNo][nPointNo] = dYValue;
		break;
	case MELT_FSTNEGRECIPROCAL:
		m_pFstNegReciprocal[nChannelNo][nPointNo] = dYValue;
		break;
	case MELT_GAINDATA:
		m_pGainData[nChannelNo][nPointNo] = dYValue;
		break;
	default:
		CTubeData::SetFluYValueBy(nType, nChannelNo, nPointNo, dYValue);
		break;
	}
}

void CTubeMeltData::GetCycleX(int nCycleCount,double* pdValueX,BOOL bConstantPCR)
{
	if(NULL == pdValueX || NULL == m_pTem)
	{
		return;
	}
	if(nCycleCount < 1 || nCycleCount > (*m_pTem).size())
	{
		return;
	}

	for(int i = 0;i < nCycleCount;++i)
	{
		pdValueX[i] = (*m_pTem)[i];
	}
	return;
}

double CTubeMeltData::GetCycleX(int iCycleId, BOOL bConstantPCR)
{
	if(NULL == m_pTem)
	{
		return 0;
	}
	if(iCycleId < 0 || iCycleId >= (*m_pTem).size())
	{
		return 0;
	}

	return (*m_pTem)[iCycleId];
}

BOOL CTubeMeltData::WriteFile(CFile* pFile)
{
	if(NULL == pFile || NULL == m_pTem)
	{
		return FALSE;
	}

	CTubeData::WriteFile(pFile);

	int iCount = (*m_pTem).size();
	pFile->Write((const char*)&iCount,sizeof(int));
	pFile->Write((const char*)(*m_pTem).data(),sizeof(double) * iCount);

	return TRUE;
}

BOOL CTubeMeltData::ReadFile(CFile* pFile,WORD wVersion)
{
	if(NULL == pFile|| NULL == m_pTem)
	{
		return FALSE;
	}

	if(!CTubeData::ReadFile(pFile,wVersion))
	{
		return FALSE;
	}

	if(wVersion >= wPCRVersion109)
	{
		for(int i = 0;i < m_nChannelCount;++i)
		{
			for(int j = 0;j < m_pFluOriginal[i].size();++j)
			{
				m_pNormalize[i].push_back(m_pFluOriginal[i][j]);
				m_pFstNegReciprocal[i].push_back(m_pFluOriginal[i][j]);
				m_pGainData[i].push_back(m_pFluOriginal[i][j]);
			}
		}

		int iCount = 0;
		pFile->Read(&iCount,sizeof(int));
		if(iCount > 0)
		{
			double* pDTem = new double[iCount];
			if(NULL == pDTem)
			{
				return FALSE;
			}
			pFile->Read(pDTem,sizeof(double) * iCount);

			for (int i = 0;i < iCount;++i)
			{
				(*m_pTem).push_back(pDTem[i]);
			}

			delete [] pDTem;
			pDTem = NULL;
		}
	}

	return TRUE;
}

void CTubeMeltData::FluDigitalFilter(CTubeInfo* pTubeInfo, CPtrArray* pFilterItem, CDataAnalysisPCR* pAlgorithm, int iDivideFlag)
{
	FluDigitalFilterBy(pTubeInfo, pFilterItem, 2, pAlgorithm, iDivideFlag);
}

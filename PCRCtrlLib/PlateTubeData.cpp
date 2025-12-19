#include "StdAfx.h"
#include "PlateTubeData.h"
#include "DeclaresPCR.h"

CPlateTubeData::CPlateTubeData(void)
{
	m_nStartPos = SCAN_START_LT;
	m_iTubeCount = 0;
	m_iChannelCount = 0;
	m_arrayTubeData.RemoveAll();
}

CPlateTubeData::~CPlateTubeData(void)
{
	DeleteAll();
}

void CPlateTubeData::DeleteAll()
{
	m_iTubeCount = 0;
	m_iChannelCount = 0;

	int nTubeCount = m_arrayTubeData.GetCount();
	for (int i=0; i<nTubeCount; i++)
	{
		CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(i);
		SAFE_DELETE(pTubeData);
	}

	m_arrayTubeData.RemoveAll();
}

BOOL CPlateTubeData::HasData()
{
	if (m_arrayTubeData.GetCount() > 0)
	{
		CTubeData* pData = (CTubeData*)m_arrayTubeData.GetAt(0);
		if (pData->HasData())
			return TRUE;

	}
	return FALSE;
}


// ����ȫ���ܵĵ�ǰѭ������ʱ��
void CPlateTubeData::AddCycleX(CUIntArray * pArrayCycle)
{
	int usefulDataNum = (m_iTubeCount > pArrayCycle->GetSize() ? pArrayCycle->GetSize() : m_iTubeCount);

	for (int i = 0; i < usefulDataNum; i++)
	{
		int nTubeNo = i;
		if (m_nStartPos == SCAN_START_RT)
		{
			//lzh:��12�޸ĳ�6
			//int nRow = i / 12;	//��Ҫ�����й���������ʹ�ù̶�ֵ��20220609��������
			//int nCol = i % 12;
			//nTubeNo = (nRow+1)*12 - nCol -1;
			int nRow = i / 6;
			int nCol = i % 6;
			nTubeNo = (nRow + 1) * 6 - nCol - 1;
		}
		CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(nTubeNo);
		pTubeData->AddCycleX(pArrayCycle->GetAt(i));
	}
}

int CPlateTubeData::AddOriginalData(CUIntArray& aryOriData, int iChannelID)
{
	int usefulDataNum = (m_iTubeCount > aryOriData.GetSize() ? aryOriData.GetSize() : m_iTubeCount);

	for(int i=0; i<usefulDataNum; i++)
	{
		int nTubeNo = i;
		if (m_nStartPos == SCAN_START_RT)
		{
			//lzh��12�޸ĳ�6
			//int nRow = i / 12;
			//int nCol = i % 12;
			//nTubeNo = (nRow+1)*12 - nCol -1;
			int nRow = i / 6;
			int nCol = i % 6;
			nTubeNo = (nRow + 1) * 6 - nCol - 1;
		}
		CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(nTubeNo);
		pTubeData->AddOriginalData(iChannelID-1, aryOriData.GetAt(i));
	}

	return 0;
}

BOOL CPlateTubeData::WriteFile(CFile* pFile)
{
	BOOL bHaveData = m_arrayTubeData.GetCount() > 0;
	pFile->Write((const char*)&bHaveData, sizeof(BOOL));

	if(bHaveData)
	{
		pFile->Write((const char*)&m_iTubeCount, sizeof(int));
		pFile->Write((const char*)&m_iChannelCount, sizeof(int));
		for(int i=0; i<m_iTubeCount; i++)
		{
			CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(i);
			pTubeData->WriteFile(pFile);
		}
	}

	return TRUE;
}

BOOL CPlateTubeData::ReadFile(CFile* pFile, WORD wVersion)
{
	DeleteAll();

	BOOL bHaveData = FALSE;
	pFile->Read(&bHaveData, sizeof(BOOL));

	if (bHaveData)
	{
		int nTubeCount = 0;
		int nChannelCount = 0;
		pFile->Read(&nTubeCount, sizeof(int));
		pFile->Read(&nChannelCount, sizeof(int));
		AllocateMemory(nTubeCount, nChannelCount);

		for (int i=0; i<nTubeCount; i++)
		{
			CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(i);
			pTubeData->ReadFile(pFile,wVersion);
		}
	}

	return TRUE;
}

void CPlateTubeData::GetCycleX(int iTubeID, int iCycleCount, double* pdValueX, BOOL bConstantPCR)
{
	int index = iTubeID - 1;
	if (index < 0 || index >= m_arrayTubeData.GetCount())
	{
		// Out of range: return zeros to keep caller stable
		for (int i = 0; i < iCycleCount; ++i)
		{
			pdValueX[i] = 0.0;
		}
		return;
	}
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(index);
	if (pTubeData != NULL)
	{
		pTubeData->GetCycleX(iCycleCount, pdValueX, bConstantPCR);
	}
}

double CPlateTubeData::GetCycleX(int iTubeID, int iCycleId, BOOL bConstantPCR)
{
	int index = iTubeID - 1;
	if (index < 0 || index >= m_arrayTubeData.GetCount())
	{
		return 0.0;
	}
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(index);
	return (pTubeData != NULL) ? pTubeData->GetCycleX(iCycleId, bConstantPCR) : 0.0;
}

UINT CPlateTubeData::GetCurrentCycleX(int iTubeID)
{
	int index = iTubeID - 1;
	if (index < 0 || index >= m_arrayTubeData.GetCount())
	{
		return 0;
	}
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(index);
	return (pTubeData != NULL) ? pTubeData->GetCurrentCycleX() : 0;
}

void CPlateTubeData::GetChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdYValue)
{
	int index = iTubeID - 1;
	if (index < 0 || index >= m_arrayTubeData.GetCount())
	{
		for (int i = 0; i < iCycleCount; ++i) { pdYValue[i] = 0.0; }
		return;
	}
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(index);
	if (pTubeData != NULL)
	{
		pTubeData->GetChannelFluY(nType, iChannel, iCycleCount, pdYValue);
	}
	else
	{
		for (int i = 0; i < iCycleCount; ++i) { pdYValue[i] = 0.0; }
	}
}

double CPlateTubeData::GetChannelCycleFluY(int iTubeID, eDataType nType, int iChannel, int iCycleId)
{
	int index = iTubeID - 1;
	if (index < 0 || index >= m_arrayTubeData.GetCount())
	{
		return 0.0;
	}
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(index);
	return (pTubeData != NULL) ? pTubeData->GetChannelCycleFluY(nType, iChannel, iCycleId) : 0.0;
}

void CPlateTubeData::SetChannelFluY(int iTubeID, eDataType nType, int iChannel, int iCycleCount, double* pdYValue)
{
	int index = iTubeID - 1;
	if (index < 0 || index >= m_arrayTubeData.GetCount())
	{
		return;
	}
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(index);
	if (pTubeData != NULL)
	{
		pTubeData->SetChannelFluY(nType, iChannel, iCycleCount, pdYValue);
	}
}

void CPlateTubeData::FluChannelCrosstalkAdjust(CTubeInfo* pTubeInfo, float** pCrosstalkMatrix)
{
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(pTubeInfo->GetTubeID()-1);
	pTubeData->FluChannelCrosstalkAdjust(pTubeInfo, pCrosstalkMatrix);
}

int CPlateTubeData::GetDataNum()
{
	CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(0);
	ASSERT(pTubeData != NULL);
	return pTubeData->GetDataNum();
}

double CPlateTubeData::GetChannelMaxValueY(eDataType nDataType,int iTubeID,int iChannel)
{
	CTubeAmplifyData* pTubeData = (CTubeAmplifyData*)m_arrayTubeData.GetAt(iTubeID - 1);
	double dReturn = pTubeData->GetMaxValueY(nDataType, iChannel);
	return dReturn;
}

//*****************************************************************************************************************
// �װ�������ӫ������
BOOL CPlateAmplifyTubeData::AllocateMemory(int iTubeCount, int iChannelCount)
{
	DeleteAll();
	ASSERT(iTubeCount > 0);
	ASSERT(iChannelCount > 0);

	m_iTubeCount = iTubeCount;
	m_iChannelCount = iChannelCount;

	for(int i=0; i<m_iTubeCount; i++)
	{
		CTubeAmplifyData* pTubeData = new CTubeAmplifyData;
		pTubeData->AllocateMemory(m_iChannelCount);
		m_arrayTubeData.Add(pTubeData);
	}

	return TRUE;
}

void CPlateAmplifyTubeData::FluDigitalFilter(CTubeInfo* pTubeInfo, CPtrArray* pFilterItem, CDataAnalysisPCR* pAlgorithm, int iFlag)
{
	CTubeAmplifyData* pTubeData = (CTubeAmplifyData*)m_arrayTubeData.GetAt(pTubeInfo->GetTubeID()-1);
	pTubeData->FluDigitalFilter(pTubeInfo, pFilterItem, pAlgorithm, iFlag);
}

void CPlateAmplifyTubeData::FluoAdjustBaseline(CTubeInfo* pTubeInfo, CDataAnalysisPara* pAnalysisPara, CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity)
{
	CTubeAmplifyData* pTubeData = (CTubeAmplifyData*)m_arrayTubeData.GetAt(pTubeInfo->GetTubeID()-1);
	pTubeData->FluoAdjustBaseline(pTubeInfo, pAnalysisPara, pAlgorithm, pTurbidity);
}

void CPlateAmplifyTubeData::FluTurbidityAdjust(CTubeInfo* pTubeInfo, CCalibrateParaTurbidity* pTurbidity, CDataAnalysisPCR* pAlgorithm)
{
	CTubeAmplifyData* pTubeData = (CTubeAmplifyData*)m_arrayTubeData.GetAt(pTubeInfo->GetTubeID()-1);
	pTubeData->FluTurbidityAdjust(pTubeInfo, pTurbidity, pAlgorithm);
}

void CPlateAmplifyTubeData::FluNormalizeAndDeltaRn(CTubeInfo* pTubeInfo, CDataAnalysisPara* pAnalysisPara, CDataAnalysisPCR* pAlgorithm, CCalibrateParaTurbidity* pTurbidity)
{
	CTubeAmplifyData* pTubeData = (CTubeAmplifyData*)m_arrayTubeData.GetAt(pTubeInfo->GetTubeID()-1);
	pTubeData->FluNormalizeAndDeltaRn(pTubeInfo, pAnalysisPara, pAlgorithm, pTurbidity);
}

// ����Ct����ز���
double CPlateAmplifyTubeData::GenerateCtByTubeID(eDataType nDataType, int iTubeID, eMethodCalculateCt nMethod, tagAnaAmplifyPara* pAmplifyPara, CDataAnalysisPCR* pAlgorithm)
{
	CTubeAmplifyData* pTubeData = (CTubeAmplifyData*)m_arrayTubeData.GetAt(iTubeID - 1);
	double dReturn = pTubeData->GenerateCt(nDataType, nMethod, pAmplifyPara, pAlgorithm);
	return dReturn;
}

double CPlateAmplifyTubeData::GenerateAutoSplineThreshold(eDataType nDataType,int iTubeID,int iChannel,CDataAnalysisPCR* pAlgorithm)
{
	CTubeAmplifyData* pTubeData = (CTubeAmplifyData*)m_arrayTubeData.GetAt(iTubeID - 1);
	double dReturn = pTubeData->GenerateAutoSplineThreshold(nDataType,iChannel,pAlgorithm);
	return dReturn;
}

int CPlateAmplifyTubeData::SetTubeData(vector<double>* pVecData,int iTubeNum,int iChanNum)
{
	if(NULL == pVecData)
	{
		return -1;
	}
	if(iTubeNum < 1)
	{
		return -2;
	}
	if(iChanNum < 1)
	{
		return -3;
	}
	if(m_iChannelCount < 1)
	{
		return -4;
	}
	if(m_iTubeCount < 1)
	{
		return -5;
	}

	int iUsedChanNum = m_iChannelCount;
	if(m_iChannelCount > iChanNum)
	{
		iUsedChanNum = iChanNum;
	}

	int iUsedTubeNum = m_iTubeCount;
	if(m_iTubeCount > iTubeNum)
	{
		iUsedTubeNum = iTubeNum;
	}

	CTubeData* pTubeData = NULL;
	for(int i = 0;i < iUsedTubeNum;++i)
	{
		pTubeData = (CTubeData*)m_arrayTubeData.GetAt(i);
		if(NULL == pTubeData)
		{
			continue;
		}
		for(int j = 0;j < iUsedChanNum;++j)
		{
			vector<double>& vecData = pVecData[j * iUsedTubeNum + i];
			for(int k = 0;k < vecData.size();++k)
			{
				pTubeData->AddOriginalData(j,vecData[k]);
			}
		}
	}

	return 0;
}


//*****************************************************************************************************************
// �װ��۽��ӫ������
BOOL CPlateMeltTubeData::AllocateMemory(int iTubeCount, int iChannelCount)
{
	DeleteAll();
	ASSERT(iTubeCount > 0);
	ASSERT(iChannelCount > 0);

	m_iTubeCount = iTubeCount;
	m_iChannelCount = iChannelCount;

	for(int i=0; i<m_iTubeCount; i++)
	{
		CTubeMeltData* pTubeData = new CTubeMeltData;
		pTubeData->AllocateMemory(m_iChannelCount);
		m_arrayTubeData.Add(pTubeData);
	}

	return TRUE;
}

void CPlateMeltTubeData::FluDigitalFilter(CTubeInfo* pTubeInfo, CPtrArray* pFilterItem, CDataAnalysisPCR* pAlgorithm, int iFlag)
{
	CTubeMeltData* pTubeData = (CTubeMeltData*)m_arrayTubeData.GetAt(pTubeInfo->GetTubeID()-1);
	pTubeData->FluDigitalFilter(pTubeInfo, pFilterItem, pAlgorithm, iFlag);
}

int CPlateMeltTubeData::AddOriginalData(CUIntArray& aryOriData,int iChannelID,double dTem)
{
	int iUsefulDataNum = (m_iTubeCount > aryOriData.GetSize() ? aryOriData.GetSize() : m_iTubeCount);

	for(int i = 0;i < iUsefulDataNum;++i)
	{
		int nTubeNo = i;
		if (m_nStartPos == SCAN_START_RT)
		{
			int nRow = i / 12;
			int nCol = i % 12;
			nTubeNo = (nRow+1)*12 - nCol -1;
		}

		CTubeData* pTubeData = (CTubeData*)m_arrayTubeData.GetAt(nTubeNo);
		if(NULL == pTubeData)
		{
			continue;
		}
		pTubeData->AddOriginalData(iChannelID - 1,aryOriData.GetAt(i),dTem);
	}

	return 0;
}

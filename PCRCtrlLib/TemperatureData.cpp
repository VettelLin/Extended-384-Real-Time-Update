#include "StdAfx.h"
#include "TemperatureData.h"
#include "DeclaresPCR.h"

CTemperatureData::CTemperatureData(void)
{
	m_bUseLid = FALSE;
	m_iLidChannelCount = 1;
	m_iChannelCount = 0;
	m_pTimeData = NULL;
	m_pTempData = NULL;

	m_iUpDownFlag = -1;
	m_nTargetChannel1 = 45;
	m_nTargetChannel2 = 0;
	m_nTargetChannel3 = 0;
	m_bSteadyPeriod = FALSE;

	m_iChan1PreTem = 0;
	m_iChan2PreTem = 0;
	m_iChan3PreTem = 0;
}

CTemperatureData::~CTemperatureData(void)
{
	DeleteAll();
}

void CTemperatureData::DeleteAll()
{
	m_iChannelCount = 0;
	SAFE_DELETE(m_pTimeData);

	if (m_pTempData != NULL)
	{
		delete [] m_pTempData;
		m_pTempData = NULL;
	}

	m_iUpDownFlag = -1;

	m_nTargetChannel1 = 45;
	m_nTargetChannel2 = 0;
	m_nTargetChannel3 = 0;
	m_bSteadyPeriod = FALSE;

	m_iChan1PreTem = 0;
	m_iChan2PreTem = 0;
	m_iChan3PreTem = 0;
}

BOOL CTemperatureData::AllocateMemory(int iChannelCount, int iLidChannelCount /*= 1*/)
{
	DeleteAll();

	m_iChannelCount = iChannelCount;
	m_iLidChannelCount = iLidChannelCount;
	m_pTimeData = new vector<DWORD>;
	m_pTempData = new vector<UINT>[iChannelCount+iLidChannelCount]; // 保证可以保存热盖温度
	return TRUE;
}

void CTemperatureData::AddOneTempData(DWORD dwTime,int nTempCount,UINT* pInputTemp,BOOL bMeltStepRunning /*= FALSE*/)
{
	m_pTimeData->push_back(dwTime);
	for (int i = 0;i < nTempCount;++i)
	{
		if(0 == i && m_nTargetChannel1 > 2000 && !bMeltStepRunning) // 值修正温度通道1的显示数据
		{
			UINT nTemp = pInputTemp[i];
			if (m_iUpDownFlag == 0) // 升温
			{
				if (nTemp > m_nTargetChannel1) // 过温
				{
					/*float fHigh = (nTemp - m_nTargetChannel1) * 0.1f;
					nTemp = m_nTargetChannel1 + UINT(fHigh);*/

					nTemp = m_nTargetChannel1;
					m_bSteadyPeriod = TRUE;
				}
				else if(nTemp < m_iChan1PreTem)
				{
					nTemp = m_iChan1PreTem;
				}
				else if (m_bSteadyPeriod)
				{
					float fHigh = (m_nTargetChannel1 - nTemp) * 0.1f;
					nTemp = m_nTargetChannel1 - UINT(fHigh);
				}
			}
			else if (m_iUpDownFlag == 1) // 降温
			{
				if (nTemp < m_nTargetChannel1) // 低温
				{
					/*float fHigh = (m_nTargetChannel1 - nTemp) * 0.1f;
					nTemp = m_nTargetChannel1 - UINT(fHigh);*/

					nTemp = m_nTargetChannel1;
					m_bSteadyPeriod = TRUE;
				}
				else if(nTemp > m_iChan1PreTem)
				{
					nTemp = m_iChan1PreTem;
				}
				else if (m_bSteadyPeriod)
				{
					float fHigh = (nTemp - m_nTargetChannel1) * 0.1f;
					nTemp = m_nTargetChannel1 + UINT(fHigh);
				}
			}

			if(i < m_iChannelCount + m_iLidChannelCount)
			{
				m_pTempData[i].push_back(nTemp);
			}
		}
		else
		{
			if(i < m_iChannelCount + m_iLidChannelCount)
			{
				m_pTempData[i].push_back(pInputTemp[i]);
			}
		}
	}
}

BOOL CTemperatureData::WriteFile(CFile* pFile)
{
	pFile->Write((const char*)&m_iChannelCount, sizeof(int));
	pFile->Write((const char*)&m_bUseLid, sizeof(BOOL));
	pFile->Write((const char*)&m_iLidChannelCount, sizeof(int));


	if (m_pTimeData != NULL)
	{
		int iDataCount = m_pTimeData->size();
		pFile->Write((const char*)&iDataCount, sizeof(int));
		pFile->Write((const char*)m_pTimeData->data(), sizeof(DWORD) * iDataCount);
	}

	if (m_pTempData != NULL)
	{
		for (int i=0; i<m_iChannelCount+m_iLidChannelCount; i++)
		{
			int iDataCount = m_pTempData[i].size();
			pFile->Write((const char*)&iDataCount, sizeof(int));
			pFile->Write((const char*)m_pTempData[i].data(), sizeof(UINT) * iDataCount);
		}
	}

	return TRUE;
}

BOOL CTemperatureData::ReadFile(CFile* pFile, WORD wVersion)
{
	DeleteAll();

	int iKindCount = 0;
	if (wVersion <= wPCRVersion106)
	{
		pFile->Read(&iKindCount, sizeof(int));
		if (iKindCount == 0)
			return TRUE;

		AllocateMemory(iKindCount - 1);	//20220624之前为iKindCount，此时打开旧有的工程文件，再次运行实验会报错（因为温度通道数不对），改为iKindCount - 1能够保持对旧有工程文件的兼容
	}
	else
	{
		pFile->Read(&m_iChannelCount,sizeof(int));
		pFile->Read(&m_bUseLid,sizeof(BOOL));
		pFile->Read(&m_iLidChannelCount,sizeof(int));
		AllocateMemory(m_iChannelCount,m_iLidChannelCount);
		iKindCount = m_iChannelCount + m_iLidChannelCount;
	}

	int iTimeCount = 0;
	pFile->Read(&iTimeCount, sizeof(int));
	DWORD* pTempTime = new DWORD[iTimeCount];
	pFile->Read(pTempTime, sizeof(DWORD)*iTimeCount);
	for (int i=0; i<iTimeCount; i++)
	{
		m_pTimeData->push_back(pTempTime[i]);
	}

	int iTempCount = 0;
	UINT* pTempData = NULL;
	for (int i=0; i<iKindCount; i++)
	{
		pFile->Read(&iTempCount, sizeof(int));

		if (pTempData == NULL)
		{
			pTempData = new UINT[iTempCount];
		}
		pFile->Read(pTempData, sizeof(UINT)*iTempCount);
		for (int k=0; k<iTempCount; k++)
		{
			m_pTempData[i].push_back(pTempData[k]);
		}
	}

	SAFE_DELETE(pTempTime);
	SAFE_DELETE(pTempData);

	return TRUE;
}

UINT CTemperatureData::GetCurTemp(eTempChannel nTempChannel,int iChannelNo /*= 0*/)
{
	UINT nTemp = 0;
	int iMaxIndex = m_pTempData[0].size() - 1;
	int iCurChannel = iChannelNo; 

	if (nTempChannel == TEMP_CH_LID) // 热盖
	{
		ASSERT(m_bUseLid);
		iCurChannel = m_iChannelCount + iChannelNo;
	}

	if (iCurChannel < m_iLidChannelCount + m_iChannelCount)
	{
		if(iMaxIndex < m_pTempData[iCurChannel].size())	//防止“仪器温度通道数和实验文件里的通道数不一致导致崩溃”，20220707，孙文郎
		{
			nTemp = m_pTempData[iCurChannel][iMaxIndex];
		}
		else
		{
			if(iCurChannel - 1 >= 0 && iMaxIndex < m_pTempData[iCurChannel - 1].size())
			{
				nTemp = m_pTempData[iCurChannel - 1][iMaxIndex];
			}
		}
	}

	return nTemp;
}

DWORD CTemperatureData::GetCurTempCollectTime()
{
	int iMaxIndex = m_pTempData[0].size() - 1;
	if(iMaxIndex < 0)
	{
		return 0;
	}
	
	return (*m_pTimeData)[iMaxIndex];
}

BOOL CTemperatureData::HasData() 
{ 
	if (m_pTimeData != NULL)
	{
		if (m_pTimeData->size() > 0)
			return TRUE;
	}
	
	if (m_pTempData != NULL)
	{
		if (m_pTempData->size() > 0)
			return TRUE;
	}

	return FALSE;
} 

// 设置控温过程中的目标温度
void CTemperatureData::SetChannelTarget(float fTemp1, float fTemp2, float fTemp3)
{
	UINT nChannelTemp1 = UINT(fTemp1 * RESOLUTION_TEMP);

	if (m_iUpDownFlag == -1)
	{
		m_iUpDownFlag = 0; // 升温
	}
	else
	{
		if (nChannelTemp1 > m_nTargetChannel1)
		{
			m_iUpDownFlag = 0; // 升温
		}
		else
		{
			m_iUpDownFlag = 1; // 降温
		}
	}

	m_iChan1PreTem = m_nTargetChannel1;
	m_iChan2PreTem = m_nTargetChannel2;
	m_iChan3PreTem = m_nTargetChannel3;

	m_bSteadyPeriod = FALSE;
	m_nTargetChannel1 = UINT(fTemp1 * RESOLUTION_TEMP);
	m_nTargetChannel2 = UINT(fTemp2 * RESOLUTION_TEMP);
	m_nTargetChannel3 = UINT(fTemp3 * RESOLUTION_TEMP);
}

vector<UINT>* CTemperatureData::GetTemDataPtr()
{
	return m_pTempData;
}


int CTemperatureData::GetDataCount()
{
	return m_pTempData[0].size();
}


void CTemperatureData::GetTimeData(int iDataCount, double* pdValueTime)
{
	for (int i=0; i<iDataCount; i++)
	{
		pdValueTime[i] = (*m_pTimeData)[i];
	}
}

void CTemperatureData::GetChannelData(CTemperatureData::eTempChannel nTempChannel, int iChannelNo, int iDataCount, double* pdValueTemp)
{
	UINT nTemp = 0;
	int iCurChannel = iChannelNo; 

	if (nTempChannel == TEMP_CH_LID) // 热盖
	{
		ASSERT(m_bUseLid);
		iCurChannel = m_iChannelCount + iChannelNo;
	}

	if (iCurChannel < m_iLidChannelCount + m_iChannelCount)
	{
		for (int i=0; i<iDataCount; i++)
		{
			pdValueTemp[i] = m_pTempData[iCurChannel][i] / RESOLUTION_TEMP;
		}
	}
}

double CTemperatureData::GetOneTimeData(int iIndex)
{
	return (*m_pTimeData)[iIndex] / 1000.0;
}

double CTemperatureData::GetOneTempData(int iChannel, int iIndex)
{
	return double(m_pTempData[iChannel][iIndex]) / RESOLUTION_TEMP;
}

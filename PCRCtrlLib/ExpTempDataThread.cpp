#include "StdAfx.h"
#include "ExpTempDataThread.h"
#include "DeclaresPCR.h"

IMPLEMENT_DYNCREATE(CExpTempDataThread, CWinThread)

CExpTempDataThread::CExpTempDataThread()
{
	m_pInstance = NULL;
	m_pExpData = NULL;
	m_pRunPara = NULL;
	m_pMsgWnd = NULL;

	m_nRunStatus = STATUS_RUNNING;
	m_nDelay = 100;
	m_bAskStop = FALSE;

	m_nTempChannelCount = 1;
	m_dwStartTime = 0;
}

CExpTempDataThread::~CExpTempDataThread(void)
{
}

BOOL CExpTempDataThread::InitInstance()
{
	return TRUE;
}

int CExpTempDataThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

void CExpTempDataThread::InitParameter()
{
	ASSERT(m_pInstance != NULL);
	ASSERT(m_pExpData != NULL);
	ASSERT(m_pRunPara != NULL);
	ASSERT(m_pMsgWnd != NULL);

	m_nTempChannelCount = m_pInstance->GetTempChannelCount();
	m_bUseLidHeater = m_pExpData->GetExperimentInfo()->GetLidHeader();
}

int CExpTempDataThread::Run()
{
	InitParameter();
	
	//获取是否使用平均温度作为仪器温度输出
	BOOL bUseAvgTem = m_pExpData->GetBeUseAvarageTemperature();

	int iTempCount = m_nTempChannelCount;
	if(m_bUseLidHeater)
	{
		++iTempCount;
	}
	UINT* pTempData = new UINT[iTempCount];
	CTime timeStart = CTime::GetCurrentTime();

	clock_t ckStart = clock();
	BOOL bFstCollect = TRUE;
	float fTemInstrument = 0;	//仪器整体温度
	BOOL bReturn = TRUE;

	while(STATUS_RUNNING == m_nRunStatus)
	{
		if(m_bAskStop)
		{
			m_nRunStatus = STATUS_ASK_STOP;
			break;
		}

		unsigned long long startTime = ::GetTickCount64();

		m_pRunPara->LockInstance();

		//获取第1路帕尔贴温度值
		float fCurTemp = 0.0f;
		BOOL bCurToTarget = FALSE;
		if (m_pInstance->GetInstanceStatus() != INSTANCE_ERROR)
		{
			bReturn = m_pInstance->ReadPCRTemp(fCurTemp,bCurToTarget);
			if (bReturn)
			{
				pTempData[0] = UINT(fCurTemp * RESOLUTION_TEMP + 0.5);
				fTemInstrument = fCurTemp;
			}
		}

		//if(!bReturn)
		//{
		//	m_nRunStatus = STATUS_STOP_ERROR;
		//	m_pRunPara->UnlockInstance();
		//	break;
		//}
		//pTempData[0] = UINT(fCurTemp * RESOLUTION_TEMP + 0.5);
		//fTemInstrument = fCurTemp;

		//获取热盖温度值
		if(m_bUseLidHeater && m_pInstance->GetInstanceStatus() != INSTANCE_ERROR)
		{
			float fLidTemp = 0.0f;
			BOOL bLidToTarget = FALSE; 
			bReturn = m_pInstance->ReadChannelTemp(TEMP_CHANNEL_LID,fLidTemp,bLidToTarget);
			if (bReturn)
			{
				pTempData[iTempCount - 1] = UINT(fLidTemp * RESOLUTION_TEMP + 0.5);
				m_pRunPara->SetLidTemp(fLidTemp,bLidToTarget);
			}

			//if(!bReturn)
			//{
			//	m_nRunStatus = STATUS_STOP_ERROR;
			//	m_pRunPara->UnlockInstance();
			//	break;
			//}
			//pTempData[iTempCount - 1] = UINT(fLidTemp * RESOLUTION_TEMP + 0.5);
			//m_pRunPara->SetLidTemp(fLidTemp,bLidToTarget);
		}


#ifdef _DEBUG
		//m_pInstance->ReadAllTem();	//M16使用
#endif


		//获取第2路帕尔贴温度值
		if(2 == m_nTempChannelCount && m_pInstance->GetInstanceStatus() != INSTANCE_ERROR)
		{
			float fTempCh2 = 0.0f;
			BOOL bCh2ToTarget = FALSE; 
			bReturn = m_pInstance->ReadChannelTemp(TEMP_CHANNEL_PET2,fTempCh2,bCh2ToTarget);
			if (bReturn)
			{
				pTempData[1] = UINT(fTempCh2 * RESOLUTION_TEMP + 0.5);
				fCurTemp += fTempCh2;
			}

			//if(!bReturn)
			//{
			//	m_nRunStatus = STATUS_STOP_ERROR;
			//	m_pRunPara->UnlockInstance();
			//	break;
			//}
			//pTempData[1] = UINT(fTempCh2 * RESOLUTION_TEMP + 0.5);
			//fCurTemp += fTempCh2;
		}

		//获取第3路帕尔贴温度值
		if(3 == m_nTempChannelCount && m_pInstance->GetInstanceStatus() != INSTANCE_ERROR)
		{
			float fTempCh3 = 0.0f;
			BOOL bCh3ToTarget = FALSE; 
			bReturn = m_pInstance->ReadChannelTemp(TEMP_CHANNEL_PET3,fTempCh3,bCh3ToTarget);
			if (bReturn)
			{
				pTempData[2] = UINT(fTempCh3 * RESOLUTION_TEMP + 0.5);
				fCurTemp += fTempCh3;
			}

			//if(!bReturn)
			//{
			//	m_nRunStatus = STATUS_STOP_ERROR;
			//	m_pRunPara->UnlockInstance();
			//	break;
			//}
			//pTempData[2] = UINT(fTempCh3 * RESOLUTION_TEMP + 0.5);
			//fCurTemp += fTempCh3;
		}
		
		if (m_pInstance->GetInstanceStatus() != INSTANCE_ERROR)
		{
			if(bUseAvgTem)
			{
				//目前只有PS96需要计算温度均值，20220624
				m_pRunPara->SetCurTemp(fCurTemp / m_nTempChannelCount,bCurToTarget);
			}
			else
			{
				m_pRunPara->SetCurTemp(fTemInstrument,bCurToTarget);
			}
		}
		
		m_pRunPara->UnlockInstance();

		// 保存温度数据
		CTime timeTemp = CTime::GetCurrentTime();
		CTimeSpan timespan = timeTemp - timeStart;
		DWORD dwCurrentTime = (DWORD)timespan.GetTotalSeconds();

		//时间使用毫秒，20220329，孙文郎
		if (m_pInstance->GetInstanceStatus() != INSTANCE_ERROR)
		{
			if(bFstCollect)
			{
				ckStart = clock();
				m_pExpData->AddTempData(0+m_dwStartTime,iTempCount,pTempData,m_pRunPara->IsMeltStepRunning());
				bFstCollect = FALSE;
			}
			else
			{
				m_pExpData->AddTempData((clock() - ckStart)+m_dwStartTime, iTempCount, pTempData,m_pRunPara->IsMeltStepRunning());
			}
			// 刷新界面数据
			m_pMsgWnd->PostMessage(WN_EXPERIMENT_TEMP_CHANGED);
		}

		// 等待
		unsigned long long currentTime = ::GetTickCount64();
		int iDelay = m_pRunPara->GetCurrentStageDelay();
		int iSleepTime = int(iDelay - (currentTime - startTime));
		if(iSleepTime > 0)
		{
			::Sleep(iSleepTime);   
		}
	}

	if(STATUS_STOP_ERROR == m_nRunStatus)
	{
		m_pRunPara->SetCommunicationState(FALSE);

		// 写日志
	}

	if(NULL != pTempData)
	{
		delete [] pTempData;
		pTempData = NULL;
	}

	return 0;
}




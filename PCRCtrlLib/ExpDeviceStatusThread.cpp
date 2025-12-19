#include "StdAfx.h"
#include "ExpDeviceStatusThread.h"

IMPLEMENT_DYNCREATE(CExpDeviceStatusThread, CWinThread)

CExpDeviceStatusThread::CExpDeviceStatusThread()
{
	m_pInstance = NULL;
	m_pRunPara = NULL;
	m_nRunStatus = STATUS_RUNNING;
	m_nDelay = 200;
	m_bAskStop = FALSE;
}


CExpDeviceStatusThread::~CExpDeviceStatusThread(void)
{
}


BOOL CExpDeviceStatusThread::InitInstance()
{
	return TRUE;
}

int CExpDeviceStatusThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

void CExpDeviceStatusThread::InitParameter()
{
	ASSERT(m_pInstance != NULL);
	ASSERT(m_pRunPara != NULL);
}

int CExpDeviceStatusThread::Run()
{
	InitParameter();

	int iErrTimes = 0;
	while(STATUS_RUNNING == m_nRunStatus)
	{
		if(m_bAskStop)
		{
			m_nRunStatus = STATUS_ASK_STOP;
			break;
		}

		m_pRunPara->LockInstance();

		if (m_pInstance->GetInstanceStatus() != INSTANCE_ERROR)
		{
			BOOL bReturn = m_pInstance->ReadDeviceStatus();
			if(bReturn)
			{
				iErrTimes = 0;
			}
			else
			{
				++iErrTimes;
				//if(iErrTimes >= 3)
				//{
				//	m_pRunPara->UnlockInstance();
				//	m_nRunStatus = STATUS_STOP_ERROR;
				//	break;
				//}
			}
		}
		m_pRunPara->UnlockInstance();

		::Sleep(m_nDelay);
	}

	if(STATUS_STOP_ERROR == m_nRunStatus)
	{
		m_pRunPara->SetCommunicationState(FALSE);

		// –¥»’÷æ
	}

	return 0;
}

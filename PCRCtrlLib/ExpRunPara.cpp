#include "StdAfx.h"
#include "ExpRunPara.h"
#include "PublicFun.h"
#include "ConfigureDB.h"

CExpRunPara::CExpRunPara(void)
{
	m_fLidLowerTemp = 30.0f;
	m_fPeltierLowerTemp = 15.0f;

	m_iLidGetTempDelay = 500;
	m_iAmplifyTempDelay = 800;
	m_iMeltTempDelay = 800;
	m_bEndAutoCoolling = TRUE;

	m_nStageType = STAGE_LID_HEATER;
	m_lTempStartTime = 0;
	m_lAmplifyStartTime = 0;
	m_lMeltStartTime = 0;
	m_lSegmentStartTime = 0;

	m_bExpInterrupt = FALSE;
	m_pDBRunInfo = new CDBRunInfo;

	m_fCurTemp = 15.0f;
	m_bCurReachTemp = FALSE;
	m_fLidTemp = 30.0f;
	m_bLidReachTemp = FALSE;

	m_ckScanGetDataMs = 0;
	m_bMeltStepRunning = FALSE;

	InitializeCriticalSection(&m_csUseInstance);

	m_bCommuNormal = TRUE;
	m_hSyncCommuState = CreateEvent(NULL,FALSE,TRUE,NULL);
}

CExpRunPara::~CExpRunPara(void)
{
	DeleteCriticalSection(&m_csUseInstance);
	SAFE_DELETE(m_pDBRunInfo);

	if(NULL != m_hSyncCommuState)
	{
		CloseHandle(m_hSyncCommuState);
		m_hSyncCommuState = NULL;
	}
}

void CExpRunPara::LockInstance()
{
	EnterCriticalSection(&m_csUseInstance);
}

void CExpRunPara::UnlockInstance()
{
	LeaveCriticalSection(&m_csUseInstance);
}

void CExpRunPara::SetRunInfo(CDBRunInfo* pRunInfo)
{
	if (pRunInfo->GetStopFlag() == 1) // 非法中止过
	{
		m_bExpInterrupt = TRUE;
		m_pDBRunInfo->SetIndex(pRunInfo->GetIndex());
		m_pDBRunInfo->SetInstrument(pRunInfo->GetInstruemnt());
		m_pDBRunInfo->SetFileName(pRunInfo->GetFileName());
		m_pDBRunInfo->SetUser(pRunInfo->GetUser());
		m_pDBRunInfo->SetBeginTime(pRunInfo->GetBeginTime());
		m_pDBRunInfo->SetEndTime(pRunInfo->GetEndTime());
		m_pDBRunInfo->SetStopFlag(pRunInfo->GetStopFlag());
		m_pDBRunInfo->SetCurSegment(pRunInfo->GetCurSegment());
		m_pDBRunInfo->SetCurCycle(pRunInfo->GetCurCycle());
		m_pDBRunInfo->SetCurStep(pRunInfo->GetCurStep());
	}
}

// 实验运行，新实验插入一条记录到数据库中
void CExpRunPara::ExpRun()
{
	ASSERT(m_pDBRunInfo != NULL);
	if (!m_bExpInterrupt)
	{
		m_pDBRunInfo->SetBeginTime(CPublicFun::GetCurrentDateTime(FALSE));
		CConfigureDB::GetInstance()->InsertRunRecord(m_pDBRunInfo);
	}
}

void CExpRunPara::SetCurSegment(int iSegment)
{
	m_lSegmentStartTime = GetTickCount64();
	m_pDBRunInfo->SetCurSegment(iSegment);
}

void CExpRunPara::SetCurCycle(int iCycle)
{
	m_pDBRunInfo->SetCurCycle(iCycle);
}

void CExpRunPara::SetCurStep(int iStep)
{
	m_pDBRunInfo->SetCurStep(iStep);
	// 保存到数据库中
	CConfigureDB::GetInstance()->UpdateRunRecord(m_pDBRunInfo, FALSE);
}

void CExpRunPara::ExpStop(CDBRunInfo::eStopFlagType nType)
{
	m_pDBRunInfo->SetEndTime(CPublicFun::GetCurrentDateTime(FALSE));
	m_pDBRunInfo->SetStopFlag(nType);
	CConfigureDB::GetInstance()->UpdateRunRecord(m_pDBRunInfo, TRUE);
}

unsigned long long CExpRunPara::GetStartTime(eExpStageType nType)
{
	unsigned long long lTemp = 0;
	switch (nType)
	{
	case STAGE_LID_HEATER:
	case STAGE_HOLD:
		lTemp = m_lTempStartTime;
		break;
	case STAGE_AMPLIFY:
		lTemp = m_lAmplifyStartTime;
		break;
	case STAGE_MELT:
		lTemp = m_lMeltStartTime;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return lTemp;
}

void CExpRunPara::SetStartTime(eExpStageType nType, unsigned long long lTime)
{
	switch (nType)
	{
	case STAGE_LID_HEATER: // 如果已经设置开始时间，则不修改
	case STAGE_HOLD: 
		if (m_lTempStartTime == 0)
		{
			m_lTempStartTime = lTime;
		}
		break;
	case STAGE_AMPLIFY:
		m_lAmplifyStartTime = lTime;
		break;
	case STAGE_MELT:
		m_lMeltStartTime = lTime;
		break;
	default:
		ASSERT(FALSE);
		break;
	}
}

int CExpRunPara::GetCurrentStageDelay()
{
	int iDelay = 0;
	switch(m_nStageType)
	{
	case STAGE_HOLD:
	case STAGE_LID_HEATER:
		iDelay = m_iLidGetTempDelay;
		break;
	case STAGE_AMPLIFY:
		iDelay = m_iAmplifyTempDelay;
		break;
	case STAGE_MELT:
		iDelay = m_iMeltTempDelay;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return iDelay;
}

void CExpRunPara::SetScanGetDataMs(clock_t ckMs)
{
	m_ckScanGetDataMs = ckMs;
}

clock_t CExpRunPara::GetScanGetDataMs()
{
	return m_ckScanGetDataMs;
}

int CExpRunPara::GetScanDataSecond()
{
	return m_ckScanGetDataMs / 1000.0;
}

void CExpRunPara::SetMeltStepRunningState(BOOL bMeltStepRunning)
{
	m_bMeltStepRunning = bMeltStepRunning;
	return;
}

BOOL CExpRunPara::IsMeltStepRunning()
{
	return m_bMeltStepRunning;
}

void CExpRunPara::SetCommunicationState(BOOL bValue)
{
	//慎用INFINITE
	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncCommuState,200000))
	{
		return;
	}
	m_bCommuNormal = bValue;
	SetEvent(m_hSyncCommuState);
	return;
}

BOOL CExpRunPara::GetCommunicationState()
{
	//慎用INFINITE
	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncCommuState,200000))
	{
		return TRUE;
	}
	SetEvent(m_hSyncCommuState);
	return m_bCommuNormal;
}



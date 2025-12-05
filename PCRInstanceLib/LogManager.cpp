#include "StdAfx.h"
#include "LogManager.h"
#include "PublicFun.h"

CLogManager* CLogManager::m_pInstance = NULL;

CLogManager::CLogManager(void)
{
	m_iSaveMode = 0;
	m_pLogDB = NULL;
	m_pLogRunFile = NULL;
	m_pLogErrorFile = NULL;
	m_pLogCommFile = NULL;

	m_arrayInstrument.RemoveAll();
	m_arrayErrorIndex.RemoveAll();
	m_arrayWarmIndex.RemoveAll();

	InitializeCriticalSection(&m_csUseDB);
}

CLogManager::~CLogManager(void)
{
	SAFE_DELETE(m_pLogDB);

	SAFE_DELETE(m_pLogRunFile);
	SAFE_DELETE(m_pLogErrorFile);
	SAFE_DELETE(m_pLogCommFile);

	m_arrayInstrument.RemoveAll();
	m_arrayErrorIndex.RemoveAll();

	for (int i=0; i<m_arrayWarmIndex.GetCount(); i++)
	{
		CLongArray* pArrayLong = (CLongArray*)m_arrayWarmIndex.GetAt(i);
		SAFE_DELETE(pArrayLong);
	}
	m_arrayWarmIndex.RemoveAll();

	DeleteCriticalSection(&m_csUseDB);
}

CLogManager* CLogManager::GetInstance()
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CLogManager();
	}
	return m_pInstance;
}

void CLogManager::DestroyInstance()
{
	if (NULL != m_pInstance)
	{
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

BOOL CLogManager::Initialize(CString strStartupPath)
{
	BOOL bReturn = FALSE;

	if (m_iSaveMode == 0)
	{
		m_pLogRunFile = new CLogRunFile(strStartupPath);
		m_pLogErrorFile = new CLogErrorFile(strStartupPath);
		m_pLogCommFile = new CLogCommFile(strStartupPath);
	}
	else
	{
		m_pLogDB = new CLogDataDB;
		if (m_pLogDB != NULL)
		{
			m_pLogDB->SetDBPath(strStartupPath);
			bReturn = m_pLogDB->Open();
		}
	}

	return bReturn;
}

// 保存运行日志，是否需要错误日志中的序号
BOOL CLogManager::WriteRunLog(CLogRunInfo* pLogRun, int nUseErrIndex)
{
	BOOL bReturn = TRUE;

	if (nUseErrIndex == 1) // 使用警告日志序号
	{
		int nInstrumentNo = FindInstruemntIndexOfArray(pLogRun->GetInstrument());
		ASSERT(nInstrumentNo != -1);

		CString strIndex;
		strIndex.Empty();
		CLongArray* pWarmLog = (CLongArray*)m_arrayWarmIndex.GetAt(nInstrumentNo);
		CString strTemp;
		for (int i=0; i<pWarmLog->GetCount(); i++)
		{
			strTemp.Format(_T("%ld"), pWarmLog->GetAt(i));
			strIndex += strTemp;
			if(i < pWarmLog->GetCount() -1)
			{
				strIndex += _T(";");
			}
		}
		pWarmLog->RemoveAll();

		pLogRun->SetErrorIndex(strIndex);
	}
	else if (nUseErrIndex == 2) // 使用错误日志序号
	{
		int nInstrumentNo = FindInstruemntIndexOfArray(pLogRun->GetInstrument());
		ASSERT(nInstrumentNo != -1);

		CString strIndex;
		strIndex.Format(_T("%ld"), m_arrayErrorIndex.GetAt(nInstrumentNo));
		m_arrayErrorIndex.SetAt(nInstrumentNo, 0);
		pLogRun->SetErrorIndex(strIndex);
	}
	
	if (pLogRun->GetDate().IsEmpty())
	{
		pLogRun->SetDate(CPublicFun::GetCurrentDateTime(FALSE));
	}

	// 保存文件或数据库中
	if (m_iSaveMode == 0)
	{
		m_pLogRunFile->WriteLog(pLogRun);
	}
	else
	{
		EnterCriticalSection(&m_csUseDB);
		bReturn = m_pLogDB->InsertRunRecord(pLogRun);
		LeaveCriticalSection(&m_csUseDB);
	}

	return bReturn;
}

// 保存错误日志，是否需要记录插入日志的序号，方便在运行日志中记录原因
BOOL CLogManager::WriteErrorLog(CLogErrorInfo* pLogError, BOOL bRecordIndex)
{
	BOOL bReturn = TRUE;
	if (m_iSaveMode == 0)
	{
		m_pLogErrorFile->WriteLog(pLogError);

		int nInstrumentNo = FindInstruemntIndexOfArray(pLogError->GetInstrument());
		if (nInstrumentNo == -1)
		{
			nInstrumentNo = m_arrayInstrument.Add(pLogError->GetInstrument());
			m_arrayErrorIndex.Add(0);
			CLongArray* pWarmLog = new CLongArray;
			m_arrayWarmIndex.Add(pWarmLog);
		}

		ASSERT(nInstrumentNo != -1);
		if (pLogError->GetLogType() == LOG_TYPE_WARM)
		{
			CLongArray* pWarmLog = (CLongArray*)m_arrayWarmIndex.GetAt(nInstrumentNo);
			pWarmLog->Add(0);

			// 如果警告超出规定数则增加一条运行记录
			if (pWarmLog->GetCount() > 4)
			{
				CLogRunInfo* pLogRun = new CLogRunInfo;

				pLogRun->SetDate(CPublicFun::GetCurrentDateTime(FALSE));
				pLogRun->SetUser(pLogError->GetUser());
				pLogRun->SetInstrument(pLogError->GetInstrument());
				pLogRun->SetLogType(LOG_TYPE_WARM);

				CString strTemp;
				strTemp.Format(_T("The command sent to the %s had more than %d failures"), pLogError->GetInstrument(), pWarmLog->GetCount());
				pLogRun->SetContent(strTemp);
				bReturn = WriteRunLog(pLogRun, 1);
				SAFE_DELETE(pLogRun);
			}
		}
		else if (pLogError->GetLogType() == LOG_TYPE_ERROR)
		{
			m_arrayErrorIndex.SetAt(nInstrumentNo, 0);
		}
	}
	else
	{
		EnterCriticalSection(&m_csUseDB);

		bReturn = m_pLogDB->InsertErrorRecord(pLogError);
		if (bRecordIndex)
		{
			long lIndex = m_pLogDB->FindErrorLogMaxIndex();

			int nInstrumentNo = FindInstruemntIndexOfArray(pLogError->GetInstrument());
			if (nInstrumentNo == -1)
			{
				nInstrumentNo = m_arrayInstrument.Add(pLogError->GetInstrument());
				m_arrayErrorIndex.Add(0);
				CLongArray* pWarmLog = new CLongArray;
				m_arrayWarmIndex.Add(pWarmLog);
			}

			ASSERT(nInstrumentNo != -1);
			if (pLogError->GetLogType() == LOG_TYPE_WARM)
			{
				CLongArray* pWarmLog = (CLongArray*)m_arrayWarmIndex.GetAt(nInstrumentNo);
				pWarmLog->Add(lIndex);

				// 如果警告超出规定数则增加一条运行记录
				if (pWarmLog->GetCount() > 4)
				{
					CLogRunInfo* pLogRun = new CLogRunInfo;

					pLogRun->SetDate(CPublicFun::GetCurrentDateTime(FALSE));
					pLogRun->SetUser(pLogError->GetUser());
					pLogRun->SetInstrument(pLogError->GetInstrument());
					pLogRun->SetLogType(LOG_TYPE_WARM);

					CString strTemp;
					strTemp.Format(_T("The command sent to the %s had more than %d failures"), pLogError->GetInstrument(), pWarmLog->GetCount());
					pLogRun->SetContent(strTemp);
					bReturn = WriteRunLog(pLogRun, 1);
					SAFE_DELETE(pLogRun);
				}
			}
			else if (pLogError->GetLogType() == LOG_TYPE_ERROR)
			{
				m_arrayErrorIndex.SetAt(nInstrumentNo, lIndex);
			}
		}

		LeaveCriticalSection(&m_csUseDB);
	}

	return bReturn;
}

BOOL CLogManager::WriteErrorLog(CString strError)
{
	if (m_iSaveMode == 0)
	{
		m_pLogErrorFile->WriteLog(strError);
		return TRUE;
	}
	else
	{
		ASSERT(FALSE);
		return FALSE;
	}
}

BOOL CLogManager::WriteCommLog(CLogCommInfo* pLogComm)
{
	BOOL bReturn = TRUE;
	if (m_iSaveMode == 0)
	{
		m_pLogCommFile->WriteLog(pLogComm);
	}
	else
	{
		EnterCriticalSection(&m_csUseDB);
		bReturn = m_pLogDB->InsertCommRecord(pLogComm);
		LeaveCriticalSection(&m_csUseDB);
	}
	return bReturn;
}

int CLogManager::FindInstruemntIndexOfArray(CString strInstrument)
{
	int nInstrumentNo = -1;
	for (int x=0; x<m_arrayInstrument.GetCount(); x++)
	{
		if (m_arrayInstrument.GetAt(x) == strInstrument)
		{
			nInstrumentNo = x;
			break;
		}
	}

	return nInstrumentNo;
}

#include "StdAfx.h"
#include "PowerOffProtect.h"

CPowerOffProtect::CPowerOffProtect(void)
{
	m_bProtectWork = FALSE;
	m_iFileTag = 0X6969;
	m_hSyncLnk = CreateEvent(NULL,FALSE,TRUE,NULL);
	if(NULL != m_hSyncLnk)
	{
		m_bProtectWork = TRUE;
	}
	m_strFileDir.Empty();
	m_strFileName = _T("POPROTECT.po");
	m_strFilePath.Empty();
}

CPowerOffProtect::~CPowerOffProtect(void)
{
	if(NULL != m_hSyncLnk)
	{
		CloseHandle(m_hSyncLnk);
		m_hSyncLnk = NULL;
	}

	if(m_fs.is_open())
	{
		m_fs.close();
	}
}

int CPowerOffProtect::InitPowerOffProtect(CString strPath)
{
	if(strPath.IsEmpty())
	{
		return -1;
	}

	m_strFileDir = strPath;
	m_strFilePath = m_strFileDir + m_strFileName;

	if(ReadFile() < 0)
	{
		return -3;
	}

	return 0;
}

int CPowerOffProtect::SetRunProgress(int iPageId,int iSegId,int iStepId,int iCycleId)
{
	if(!m_bProtectWork)
	{
		return -1;
	}

	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncLnk,INFINITE))
	{
		return -2;
	}

	BOOL bFind = FALSE;
	Node<CRunProgressRecord>* pNode = m_lnkRunPrgRcd.GetHead();
	while(NULL != pNode)
	{
		if(iPageId == pNode->data.m_iPageId)
		{
			pNode->data.m_iSegId = iSegId;
			pNode->data.m_iStepId = iStepId;
			pNode->data.m_iCycleId = iCycleId;
			SaveFile();
			bFind = TRUE;
			break;
		}
		pNode = pNode->pNext;
	}

	SetEvent(m_hSyncLnk);

	if(!bFind)
	{
		return -3;	
	}
	
	return 0;
}

int CPowerOffProtect::SetRunProgress(int iPageId,float fMeltTem)
{
	if(!m_bProtectWork)
	{
		return -1;
	}

	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncLnk,INFINITE))
	{
		return -2;
	}

	BOOL bFind = FALSE;
	Node<CRunProgressRecord>* pNode = m_lnkRunPrgRcd.GetHead();
	while(NULL != pNode)
	{
		if(iPageId == pNode->data.m_iPageId)
		{
			pNode->data.m_fPowerOffTem = fMeltTem;
			SaveFile();
			bFind = TRUE;
			break;
		}
		pNode = pNode->pNext;
	}

	SetEvent(m_hSyncLnk);

	if(!bFind)
	{
		return -3;	
	}

	return 0;
}

int CPowerOffProtect::AddRunProgress(CRunProgressRecord& runPrgRcd)
{
	if(!m_bProtectWork)
	{
		return -1;
	}

	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncLnk,INFINITE))
	{
		return -2;
	}

	m_lnkRunPrgRcd.AddTail(runPrgRcd);
	SaveFile();

	SetEvent(m_hSyncLnk);

	return 0;
}

int CPowerOffProtect::DeleteRunProgress(int iPageId)
{
	if(!m_bProtectWork)
	{
		return -1;
	}

	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncLnk,INFINITE))
	{
		return -2;
	}

	BOOL bFind = FALSE;
	Node<CRunProgressRecord>* pNode = m_lnkRunPrgRcd.GetHead();
	while(NULL != pNode)
	{
		if(iPageId == pNode->data.m_iPageId)
		{
			m_lnkRunPrgRcd.DeleteNode(pNode);
			SaveFile();
			bFind = TRUE;
			break;
		}
		pNode = pNode->pNext;
	}

	SetEvent(m_hSyncLnk);

	if(!bFind)
	{
		return 1;	
	}

	return 0;
}

int CPowerOffProtect::DeleteOldProgress(int iLnkIndex)
{
	if(!m_bProtectWork)
	{
		return -1;
	}

	if(iLnkIndex < 0)
	{
		return -2;
	}

	if(iLnkIndex >= m_lnkRunPrgRcd.GetSize())
	{
		return -3;
	}

	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncLnk,INFINITE))
	{
		return -4;
	}

	Node<CRunProgressRecord>* pNode = m_lnkRunPrgRcd.GetAt(iLnkIndex);
	if(NULL != pNode)
	{
		m_lnkRunPrgRcd.DeleteNode(pNode);
		SaveFile();
	}

	SetEvent(m_hSyncLnk);

	return 0;
}

int CPowerOffProtect::SearchPowerOffProtect(CString strSN)
{
	int iIndex = -1;
	if(WAIT_OBJECT_0 != WaitForSingleObject(m_hSyncLnk,INFINITE))
	{
		return iIndex;
	}

	Node<CRunProgressRecord>* pNode = m_lnkRunPrgRcd.GetHead();
	while(NULL != pNode)
	{
		++iIndex;
		if(strSN == pNode->data.m_strSN)
		{
			goto END;
		}
		pNode = pNode->pNext;
	}

END:
	SetEvent(m_hSyncLnk);

	return iIndex;
}

int CPowerOffProtect::GetRunProgressRecord(int iIndex,CRunProgressRecord& runPrjRcd)
{
	if(iIndex < 0)
	{
		return -1;
	}
	if(iIndex >= m_lnkRunPrgRcd.GetSize())
	{
		return -2;
	}
	runPrjRcd = *m_lnkRunPrgRcd[iIndex];

	return 0;
}

int CPowerOffProtect::ReadFile()
{
	ifstream ifs(m_strFilePath);
	if(!ifs.is_open())
	{
		return -1;
	}

	int iRet = 0;

	int iFileTag = 0;
	ifs.read((char*)&iFileTag,sizeof(int));
	if(iFileTag != m_iFileTag)
	{
		iRet = -2;
		goto END;
	}

	m_lnkRunPrgRcd.Clear();

	int iRcdNum = 0;
	ifs.read((char*)&iRcdNum,sizeof(int));
	if(iRcdNum < 1)
	{
		iRet = 1;
		goto END;
	}

	for(int i = 0;i < iRcdNum;++i)
	{
		CRunProgressRecord runPrgRcd;
		ifs.read((char*)&runPrgRcd.m_iSegId,sizeof(int));
		ifs.read((char*)&runPrgRcd.m_iStepId,sizeof(int));
		ifs.read((char*)&runPrgRcd.m_iCycleId,sizeof(int));

		ifs.read((char*)&runPrgRcd.m_ttStart,sizeof(time_t));
		ifs.read((char*)&runPrgRcd.m_fMeltStartTem,sizeof(float));
		ifs.read((char*)&runPrgRcd.m_fMeltEndTem,sizeof(float));
		ifs.read((char*)&runPrgRcd.m_fPowerOffTem,sizeof(float));

		ReadString(ifs,runPrgRcd.m_strFilePath);
		ReadString(ifs,runPrgRcd.m_strSN);

		m_lnkRunPrgRcd.AddTail(runPrgRcd);
	}

END:
	ifs.close();
	return iRet;
}

int CPowerOffProtect::SaveFile()
{
	if(!m_bProtectWork)
	{
		return -1;
	}

	m_fs.open(m_strFilePath,ios::out | ios::trunc | ios::binary);
	if(!m_fs.is_open())
	{
		return -2;
	}

	m_fs.write((const char*)&m_iFileTag,sizeof(int));

	int iRcdNum = m_lnkRunPrgRcd.GetSize();
	m_fs.write((const char*)&iRcdNum,sizeof(int));
	
	int iStrLen = 0;
	Node<CRunProgressRecord>* pNode = m_lnkRunPrgRcd.GetHead();
	while(NULL != pNode)
	{
		m_fs.write((const char*)&pNode->data.m_iSegId,sizeof(int));
		m_fs.write((const char*)&pNode->data.m_iStepId,sizeof(int));
		m_fs.write((const char*)&pNode->data.m_iCycleId,sizeof(int));
		
		m_fs.write((const char*)&pNode->data.m_ttStart,sizeof(time_t));
		m_fs.write((const char*)&pNode->data.m_fMeltStartTem,sizeof(float));
		m_fs.write((const char*)&pNode->data.m_fMeltEndTem,sizeof(float));
		m_fs.write((const char*)&pNode->data.m_fPowerOffTem,sizeof(float));

		WriteString(pNode->data.m_strFilePath);
		WriteString(pNode->data.m_strSN);

		pNode = pNode->pNext;
	}

	m_fs.close();

	return 0;
}

int CPowerOffProtect::WriteString(CString& strText)
{
	if(!m_fs.is_open())
	{
		return -1;
	}

	int iStrLen = 0;
	iStrLen = strText.GetLength();
	m_fs.write((const char *)&iStrLen,sizeof(int));
	if(iStrLen > 0)
	{
		m_fs.write((const char *)(strText.GetBuffer()),sizeof(TCHAR) * iStrLen);
	}

	return 0;
}

int CPowerOffProtect::ReadString(ifstream& ifs,CString& strText)
{
	if(!ifs.is_open())
	{
		return -1;
	}

	int iStrLen = 0;
	TCHAR lpTStr[1024] = {0};

	ifs.read((char *)&iStrLen,sizeof(int));
	if(iStrLen > 0)
	{
		ifs.read((char*)lpTStr,sizeof(TCHAR) * iStrLen);
		lpTStr[iStrLen] = 0;
		strText = lpTStr;
	}

	return 0;
}



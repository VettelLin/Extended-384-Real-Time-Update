#include "StdAfx.h"
#include "LogFile.h"
#include "locale.h"

CLogFile::CLogFile(CString strStartupPath)
{
	strStartupPath += _T("LogFile");
	m_strFileName.Empty();
	m_strSpace = _T("    ");

	CreateLogPath(strStartupPath);

	InitializeCriticalSection(&m_CriticalSection);
}


CLogFile::~CLogFile(void)
{
	DeleteCriticalSection(&m_CriticalSection);
}

BOOL CLogFile::CreateLogPath(CString strFilePath)
{
	if(!PathFileExists(strFilePath))
	{
		BOOL bRtn = -1;
		bRtn = CreateDirectory(strFilePath,NULL);
		if (0 == bRtn)
		{
			return FALSE;
		}
	}

	CString strDate = GetCurrentDate();
	m_strFilePath = strFilePath + _T("\\") + strDate;

	//获取存储路径
	if(!PathFileExists(m_strFilePath))
	{
		BOOL bRtn = -1;
		bRtn = CreateDirectory(m_strFilePath,NULL);
		if (0 == bRtn)
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CLogFile::CreateLogFile()
{
	CFileFind filefind;
	BOOL bFind = filefind.FindFile(m_strFileName);
	if(!bFind)
	{
		HANDLE hcreatefile = CreateFile(m_strFileName, GENERIC_READ|GENERIC_WRITE,NULL, NULL, OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL);
		if (hcreatefile== INVALID_HANDLE_VALUE)
		{
			hcreatefile = NULL;
			MessageBoxA(NULL, "Can not open the file", "warning", MB_OK);
			return FALSE;
		}
		else
		{
			CloseHandle(hcreatefile);
			return TRUE;
		}
	}

	return TRUE;
}

BOOL CLogFile::WriteString(CString strLog)
{	
	BOOL bReturn = FALSE;
	EnterCriticalSection(&m_CriticalSection);

	CStdioFile recordFile;
	BOOL bSuc = recordFile.Open(m_strFileName, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite);
	if (!bSuc)
	{
		return FALSE;
	}
		
	char* old_locale = _strdup( setlocale(LC_CTYPE,NULL) );
	setlocale( LC_CTYPE, "chs" );
	recordFile.SeekToEnd();

	strLog += "\n";
	recordFile.WriteString(strLog);

	setlocale( LC_CTYPE,old_locale );
	free(old_locale);

	recordFile.Close();

	OutputDebugString(strLog);

	LeaveCriticalSection(&m_CriticalSection);

	return bReturn;
}


CString CLogFile::GetCurrentDate()
{
	CString strDate;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strDate.Format(_T("%4d-%02d-%02d"),st.wYear, st.wMonth, st.wDay);
	return strDate;
}


CString CLogFile::GetCurrentTime()
{
	CString strTime;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strTime.Format(_T("%4d-%02d-%02d-%02d-%02d-%02d"),st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	return strTime;
}


//*****************************************************************************************************************
// 运行日志文件

CLogRunFile::CLogRunFile(CString strStartupPath) : CLogFile(strStartupPath)
{
	m_strFileName = m_strFilePath + _T("\\PCRRun.log");
}

void CLogRunFile::WriteLog(CLogRunInfo* pLogInfo)
{
	CString strLog, strTemp;
	strLog = pLogInfo->GetDate() + m_strSpace + pLogInfo->GetContent();
	strTemp.Format(_T("%d"), pLogInfo->GetLogType());
	strLog += m_strSpace + strTemp;
	strLog += m_strSpace + pLogInfo->GetUser() + m_strSpace + pLogInfo->GetInstrument();

	WriteString(strLog);
}

//*****************************************************************************************************************
// 错误日志文件

CLogErrorFile::CLogErrorFile(CString strStartupPath) : CLogFile(strStartupPath)
{
	m_strFileName = m_strFilePath + _T("\\PCRError.log");
}

void CLogErrorFile::WriteLog(CLogErrorInfo* pLogInfo)
{
	CString strLog, strTemp;
	strLog = pLogInfo->GetDate() + m_strSpace + pLogInfo->GetError();
	strLog += m_strSpace + pLogInfo->GetErrPara1() + m_strSpace + pLogInfo->GetErrPara2();
	strTemp.Format(_T("%d"), pLogInfo->GetLogType());
	strLog += m_strSpace + strTemp;
	strLog += m_strSpace + pLogInfo->GetUser() + m_strSpace + pLogInfo->GetInstrument();

	WriteString(strLog);
}

void CLogErrorFile::WriteLog(CString strError)
{
	WriteString(strError);
}


//*****************************************************************************************************************
// 通讯日志文件

CLogCommFile::CLogCommFile(CString strStartupPath) : CLogFile(strStartupPath)
{
	m_strFileName = m_strFilePath + _T("\\Comm") + GetCurrentTime() + _T(".log");
}

void CLogCommFile::WriteLog(CLogCommInfo* pLogInfo)
{
	CString strLog, strTemp;
	CString strTime = pLogInfo->GetDate();
	strLog = strTime.Right(strTime.GetLength()-11);
	if (pLogInfo->GetCommType() == CLogCommInfo::COMM_TYPE_SEND)
	{
		strLog += m_strSpace + _T("Send:    ");
	}
	else
	{
		strLog += m_strSpace + _T("Receive:");
	}

	strLog += m_strSpace + pLogInfo->GetContent();

	strTemp.Format(_T("%d"), pLogInfo->GetResult());
	strLog += m_strSpace + strTemp;

	strTemp.Format(_T("%d"), pLogInfo->GetSendCount());
	strLog += m_strSpace + strTemp;

	strLog += m_strSpace + pLogInfo->GetComment();

	WriteString(strLog);
}



#pragma once

#include "LogDataInfo.h"

class CLogFile
{
public:
	CLogFile(CString strStartupPath);
	~CLogFile(void);

protected:
	BOOL CreateLogPath(CString strFilePath);
	BOOL CreateLogFile();
	BOOL WriteString(CString strLog);
	CString GetCurrentDate();
	CString GetCurrentTime();

protected:
	CRITICAL_SECTION m_CriticalSection;

	CString m_strFilePath;
	CString m_strFileName;
	CString m_strSpace;
};

class CLogRunFile : public CLogFile
{
public:
	CLogRunFile(CString strStartupPath);
	~CLogRunFile(void) {}

public:
	void WriteLog(CLogRunInfo* pLogInfo);
};

class CLogErrorFile : public CLogFile
{
public:
	CLogErrorFile(CString strStartupPath);
	~CLogErrorFile(void) {}

public:
	void WriteLog(CLogErrorInfo* pLogInfo);
	void WriteLog(CString strError);
};

class CLogCommFile : public CLogFile
{
public:
	CLogCommFile(CString strStartupPath);
	~CLogCommFile(void) {}

public:
	void WriteLog(CLogCommInfo* pLogInfo);
};
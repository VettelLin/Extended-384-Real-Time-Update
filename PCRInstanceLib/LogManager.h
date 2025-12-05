#pragma once

#include "LogDataDB.h"
#include "DeclaresPCR.h"
#include "LogFile.h"

class AFX_EXT_CLASS CLogManager
{
private:
	CLogManager(void);
	virtual ~CLogManager(void);

public:
	static CLogManager* GetInstance();
	static void DestroyInstance();
	BOOL Initialize(CString strStartupPath);

	BOOL WriteRunLog(CLogRunInfo* pLogRun, int nUseErrIndex=0);
	BOOL WriteErrorLog(CLogErrorInfo* pLogError, BOOL bRecordIndex=FALSE);
	BOOL WriteErrorLog(CString strError);
	BOOL WriteCommLog(CLogCommInfo* pLogComm);

protected:
	int FindInstruemntIndexOfArray(CString strInstrument);

private:
	static CLogManager* m_pInstance;

	int	m_iSaveMode; // 保存模式，0：保存到文本文件 1：保存到数据库
	CLogDataDB*		m_pLogDB;

	CLogRunFile*	m_pLogRunFile; // 运行文件日志
	CLogErrorFile*	m_pLogErrorFile; // 错误文件日志
	CLogCommFile*	m_pLogCommFile; // 通讯文件日志

	CRITICAL_SECTION m_csUseDB;

	CStringArray	m_arrayInstrument; // 仪器序号
	CLongArray		m_arrayErrorIndex; // 错误日志序号，有则在运行日志中发出错误提示，外部反馈提示
	CPtrArray		m_arrayWarmIndex; // 记录通讯警告日志序号，如果超过5次则在运行日志中发出警告
};


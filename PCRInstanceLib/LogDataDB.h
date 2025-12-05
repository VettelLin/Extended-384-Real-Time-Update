#pragma once

#include "LogDataInfo.h"
#include <afxdao.h>
#pragma warning(disable: 4995 311 244)

class AFX_EXT_CLASS CLogDataDB
{
public:
	CLogDataDB(void);
	~CLogDataDB(void);
	enum eTableRunrField
	{
		RUN_INDEX = 0,
		RUN_DATE,
		RUN_CONTENT,
		RUN_USER,
		RUN_INSTRUMENT,
		RUN_LOG_TYPE,
		RUN_ERR_INDEX,
		RUN_STATUS,
	};

	enum eTableErrorField
	{
		ERROR_INDEX = 0,
		ERROR_DATE,
		ERROR_CONTENT,
		ERROR_PARA1,
		ERROR_PARA2,
		ERROR_USER,
		ERROR_INSTRUMENT,
		ERROR_LOG_TYPE,
	};

	enum eTableCommField
	{
		COMM_INDEX = 0,
		COMM_LOG_DATE,
		COMM_INSTRUMENT,
		COMM_COMMENT,
		COMM_TYPE,
		COMM_RESULT,
		COMM_SEND_COUNT,
		COMM_CONTENT,
	};

// Operations
public:
	void SetDBPath(CString strPath) { m_strFileName = strPath + m_strFileName; }
	BOOL Open();

	BOOL InsertRunRecord(CLogRunInfo* pLog);
	BOOL SearchRunLog(CLogRunInfoList* pList, CString strUser, CString strInstrument, CString strDateBegin, CString strDateEnd);

	BOOL InsertErrorRecord(CLogErrorInfo* pLog);
	BOOL SearchErrorLog(CLogErrorInfoList* pList, CString strUser, CString strInstrument, CString strDateBegin, CString strDateEnd);
	BOOL SearchErrorLog(CLogErrorInfoList* pList, CString strErrIndex);
	long FindErrorLogMaxIndex();

	BOOL InsertCommRecord(CLogCommInfo* pLog);
	BOOL SearchCommLog(CLogCommInfoList* pList, CString strInstrument);

protected:
	BOOL SearchErrorLogBySQL(CString strSQL, CLogErrorInfoList* pList);

// Attributes
protected:
	CDaoDatabase* m_pDaoDB;
	CString m_strFileName;

	// 运行日志信息表
	CString m_strRunTableName;
	CStringArray m_arrayRunTableField;

	// 错误日志信息表
	CString m_strErrorTableName;
	CStringArray m_arrayErrorTableField;

	// 通讯日志信息表
	CString m_strCommTableName;
	CStringArray m_arrayCommTableField;
};


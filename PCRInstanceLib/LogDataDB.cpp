#include "StdAfx.h"
#include "LogDataDB.h"
#include "PublicFun.h"

const UINT TABLE_RUN_FIELD_COUNT = 8;
const UINT TABLE_ERROR_FIELD_COUNT = 8;
const UINT TABLE_COMM_FIELD_COUNT = 8;

CLogDataDB::CLogDataDB(void)
{
	m_pDaoDB = new CDaoDatabase;
	m_strFileName = _T("DB\\LogData.mdb");

	m_strRunTableName = _T("RunInfo");
	m_arrayRunTableField.Add(_T("Index"));
	m_arrayRunTableField.Add(_T("LogDate"));
	m_arrayRunTableField.Add(_T("Content"));
	m_arrayRunTableField.Add(_T("LogUser"));
	m_arrayRunTableField.Add(_T("InstrumentSN"));
	m_arrayRunTableField.Add(_T("LogType"));
	m_arrayRunTableField.Add(_T("ErrorIndex"));
	m_arrayRunTableField.Add(_T("Status"));
	ASSERT(m_arrayRunTableField.GetCount() == TABLE_RUN_FIELD_COUNT);

	m_strErrorTableName = _T("ErrorInfo");
	m_arrayErrorTableField.Add(_T("Index"));
	m_arrayErrorTableField.Add(_T("LogDate"));
	m_arrayErrorTableField.Add(_T("Content"));
	m_arrayErrorTableField.Add(_T("ErrorPara1"));
	m_arrayErrorTableField.Add(_T("ErrorPara2"));
	m_arrayErrorTableField.Add(_T("LogUser"));
	m_arrayErrorTableField.Add(_T("InstrumentSN"));
	m_arrayErrorTableField.Add(_T("LogType"));
	ASSERT(m_arrayErrorTableField.GetCount() == TABLE_ERROR_FIELD_COUNT);

	m_strCommTableName = _T("CommInfo");
	m_arrayCommTableField.Add(_T("Index"));
	m_arrayCommTableField.Add(_T("LogDate"));
	m_arrayCommTableField.Add(_T("InstrumentSN"));
	m_arrayCommTableField.Add(_T("Comment"));
	m_arrayCommTableField.Add(_T("CommType"));
	m_arrayCommTableField.Add(_T("Result"));
	m_arrayCommTableField.Add(_T("SendNum"));
	m_arrayCommTableField.Add(_T("Content"));
	ASSERT(m_arrayCommTableField.GetCount() == TABLE_COMM_FIELD_COUNT);
}


CLogDataDB::~CLogDataDB(void)
{
	if(m_pDaoDB->IsOpen())
	{
		m_pDaoDB->Close();
	}
	delete m_pDaoDB;
}

BOOL CLogDataDB::Open()
{
	CFileFind finder;
	BOOL bReturn = finder.FindFile(m_strFileName);
	if (bReturn)
	{
		if (!m_pDaoDB->IsOpen())
		{
			m_pDaoDB->Open(m_strFileName);
		}
		return m_pDaoDB->IsOpen();
	}

	return bReturn;
}

//*****************************************************************************************************************
// 运行日志信息表
BOOL CLogDataDB::InsertRunRecord(CLogRunInfo* pLog)
{
	CDaoDatabase* pDB = m_pDaoDB;
	CString strSQL;

	CString strRecord, strValue, strTemp;
	strRecord.Format(_T("%s"), m_strRunTableName);
	strRecord += _T("(");
	for(int i = 1; i<TABLE_RUN_FIELD_COUNT; i++)
	{
		strTemp.Format(_T("%s"), m_arrayRunTableField[i]);
		strRecord += strTemp;
		if(i < TABLE_RUN_FIELD_COUNT -1)
		{
			strRecord += _T(",");
		}
	}
	strRecord += _T(")");
	strValue = _T("(");

	for(int i = 1; i<TABLE_RUN_FIELD_COUNT; i++)
	{
		switch(i)
		{
		case RUN_DATE:
			strTemp.Format(_T("'%s'"), pLog->GetDate());
			break;
		case RUN_CONTENT:
			strTemp.Format(_T("'%s'"), pLog->GetContent());
			break;
		case RUN_USER:
			strTemp.Format(_T("'%s'"), pLog->GetUser());
			break;
		case RUN_INSTRUMENT:
			strTemp.Format(_T("'%s'"), pLog->GetInstrument());
			break;
		case RUN_LOG_TYPE:
			strTemp.Format(_T("%d"), pLog->GetLogType());
			break;
		case RUN_ERR_INDEX:
			strTemp.Format(_T("'%s'"), pLog->GetErrorIndex());
			break;
		case RUN_STATUS:
			strTemp.Format(_T("%d"), pLog->GetStatus());
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		strValue += strTemp;
		if(i < TABLE_RUN_FIELD_COUNT -1)
		{
			strValue += _T(",");
		}
	}
	strValue += _T(")");
	strSQL.Format(_T("INSERT INTO %s VALUES %s"), strRecord, strValue);
	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	return TRUE;
}

BOOL CLogDataDB::SearchRunLog(CLogRunInfoList* pList, CString strUser, CString strInstrument, CString strDateBegin, CString strDateEnd)
{
	CString strSQL;

	if (strDateBegin.IsEmpty() || strDateEnd.IsEmpty())
	{
		strSQL.Format(_T("SELECT * FROM %s ORDER BY %s"), m_strRunTableName, m_arrayRunTableField[RUN_INDEX]);
	}
	else
	{
		strSQL.Format(_T("SELECT * FROM %s WHERE %s > '%s' AND %s < '%s' ORDER BY %s"), m_strRunTableName, 
			m_arrayRunTableField[RUN_DATE], strDateBegin, m_arrayRunTableField[RUN_DATE], strDateEnd, m_arrayRunTableField[RUN_INDEX]);
	}

	CDaoRecordset rs(m_pDaoDB);
	try
	{
		rs.Open(dbOpenDynaset, strSQL);
		COleVariant var;
		if(rs.IsOpen())
		{
			int Cnt = rs.GetFieldCount();
			while(!rs.IsEOF())
			{
				CLogRunInfo* pLog = new CLogRunInfo;
				for(int i = 0; i<Cnt; i++)
				{
					var = rs.GetFieldValue(i);
					if (var.vt == VT_NULL)
						continue;
					switch(i)
					{
					case RUN_INDEX:
						pLog->SetIndex(V_I4(&var));
						break;
					case RUN_DATE:
						pLog->SetDate(V_BSTRT(&var));
						break;
					case RUN_CONTENT:
						pLog->SetContent(V_BSTRT(&var));
						break;
					case RUN_USER:
						pLog->SetUser(V_BSTRT(&var));
						break;
					case RUN_INSTRUMENT:
						pLog->SetInstrument(V_BSTRT(&var));
						break;
					case RUN_LOG_TYPE:
						pLog->SetLogType((eLogType)V_I1(&var));
						break;
					case RUN_ERR_INDEX:
						pLog->SetErrorIndex(V_BSTRT(&var));
						break;
					case RUN_STATUS:
						pLog->SetStatus(V_I1(&var));
						break;
					default:
						ASSERT(FALSE);
						break;
					}
				}
				pList->AddTail(pLog);
				rs.MoveNext();
			}
			rs.Close();
		}
	}
	catch (CDaoException* e) 
	{
		e->ReportError ();
		e->Delete ();
	}

	return TRUE;
}

//*****************************************************************************************************************
// 错误日志信息表

BOOL CLogDataDB::InsertErrorRecord(CLogErrorInfo* pLog)
{
	CDaoDatabase* pDB = m_pDaoDB;
	CString strSQL;

	CString strRecord, strValue, strTemp;
	strRecord.Format(_T("%s"), m_strErrorTableName);
	strRecord += _T("(");
	for(int i=1; i<TABLE_ERROR_FIELD_COUNT; i++)
	{
		strTemp.Format(_T("%s"), m_arrayErrorTableField[i]);
		strRecord += strTemp;
		if(i < TABLE_ERROR_FIELD_COUNT -1)
		{
			strRecord += _T(",");
		}
	}
	strRecord += _T(")");
	strValue = _T("(");

	for(int i = 1; i<TABLE_ERROR_FIELD_COUNT; i++)
	{
		switch(i)
		{
		case ERROR_DATE:
			strTemp.Format(_T("'%s'"), pLog->GetDate());
			break;
		case ERROR_CONTENT:
			strTemp.Format(_T("'%s'"), pLog->GetError());
			break;
		case ERROR_PARA1:
			strTemp.Format(_T("'%s'"), pLog->GetErrPara1());
			break;
		case ERROR_PARA2:
			strTemp.Format(_T("'%s'"), pLog->GetErrPara2());
			break;
		case ERROR_USER:
			strTemp.Format(_T("'%s'"), pLog->GetUser());
			break;
		case ERROR_INSTRUMENT:
			strTemp.Format(_T("'%s'"), pLog->GetInstrument());
			break;
		case ERROR_LOG_TYPE:
			strTemp.Format(_T("%d"), pLog->GetLogType());
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		strValue += strTemp;
		if(i < TABLE_ERROR_FIELD_COUNT -1)
		{
			strValue += _T(",");
		}
	}
	strValue += _T(")");
	strSQL.Format(_T("INSERT INTO %s VALUES %s"), strRecord, strValue);
	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	return TRUE;
}

BOOL CLogDataDB::SearchErrorLog(CLogErrorInfoList* pList, CString strUser, CString strInstrument, CString strDateBegin, CString strDateEnd)
{
	CString strSQL;

	if (strDateBegin.IsEmpty() || strDateEnd.IsEmpty())
	{
		if (strUser.IsEmpty())
		{
			if (strInstrument.IsEmpty())
			{
				strSQL.Format(_T("SELECT * FROM %s ORDER BY %s"), m_strErrorTableName, m_arrayErrorTableField[ERROR_INDEX]);
			}
			else
			{
				strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s' ORDER BY %s"), m_strErrorTableName
					, m_arrayErrorTableField[ERROR_INSTRUMENT], strInstrument, m_arrayErrorTableField[ERROR_INDEX]);
			}
		}
		else
		{
			if (strInstrument.IsEmpty())
			{
				strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s' ORDER BY %s"), m_strErrorTableName
					, m_arrayErrorTableField[ERROR_USER], strUser, m_arrayErrorTableField[ERROR_INDEX]);
			}
			else
			{
				strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s' AND %s = '%s' ORDER BY %s"), m_strErrorTableName
					, m_arrayErrorTableField[ERROR_INSTRUMENT], strInstrument, m_arrayErrorTableField[ERROR_USER], strUser, m_arrayErrorTableField[ERROR_INDEX]);
			}
		}
	}
	else
	{
		if (strUser.IsEmpty())
		{
			if (strInstrument.IsEmpty())
			{
				strSQL.Format(_T("SELECT * FROM %s WHERE %s > '%s' AND %s < '%s' ORDER BY %s"), m_strErrorTableName
					, m_arrayErrorTableField[ERROR_DATE], strDateBegin, m_arrayErrorTableField[ERROR_DATE], strDateEnd, m_arrayErrorTableField[ERROR_INDEX]);
			}
			else
			{
				strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s' AND %s > '%s' AND %s < '%s' ORDER BY %s"), m_strErrorTableName
					, m_arrayErrorTableField[ERROR_INSTRUMENT], strInstrument
					, m_arrayErrorTableField[ERROR_DATE], strDateBegin, m_arrayErrorTableField[ERROR_DATE], strDateEnd, m_arrayErrorTableField[ERROR_INDEX]);
			}
		}
		else
		{
			if (strInstrument.IsEmpty())
			{
				strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s' AND %s > '%s' AND %s < '%s' ORDER BY %s"), m_strErrorTableName
					, m_arrayErrorTableField[ERROR_USER], strUser
					, m_arrayErrorTableField[ERROR_DATE], strDateBegin, m_arrayErrorTableField[ERROR_DATE], strDateEnd, m_arrayErrorTableField[ERROR_INDEX]);
			}
			else
			{
				strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s' AND %s = '%s' AND %s > '%s' AND %s < '%s' ORDER BY %s"), m_strErrorTableName
					, m_arrayErrorTableField[ERROR_USER], strUser, m_arrayErrorTableField[ERROR_INSTRUMENT], strInstrument
					, m_arrayErrorTableField[ERROR_DATE], strDateBegin, m_arrayErrorTableField[ERROR_DATE], strDateEnd, m_arrayErrorTableField[ERROR_INDEX]);
			}
		}
	}

	return SearchErrorLogBySQL(strSQL, pList);
}

BOOL CLogDataDB::SearchErrorLog(CLogErrorInfoList* pList, CString strErrIndex)
{
	ASSERT(!strErrIndex.IsEmpty());
	CStringArray arrayIndex;
	CPublicFun::SplitCombineStringByFlag(strErrIndex, &arrayIndex);
	CString strTemp;
	CString strIndexIn;
	strIndexIn.Empty();
	for(int i=1; i<arrayIndex.GetCount(); i++)
	{
		strTemp.Format(_T("%s"), arrayIndex[i]);
		strIndexIn += strTemp;
		if(i < arrayIndex.GetCount() -1)
		{
			strIndexIn += _T(",");
		}
	}

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE %s IN (%s) ORDER BY %s"), m_strErrorTableName
		, m_arrayErrorTableField[ERROR_INDEX], strIndexIn, m_arrayErrorTableField[ERROR_INDEX]);

	return SearchErrorLogBySQL(strSQL, pList);
}

long CLogDataDB::FindErrorLogMaxIndex()
{
	long nMaxIndex = -1;
	CString strSQL;
	strSQL.Format(_T("SELECT MAX(%s) FROM %s"), m_arrayErrorTableField[ERROR_INDEX], m_strErrorTableName);
	CDaoRecordset rs(m_pDaoDB);
	rs.Open(dbOpenDynaset, strSQL);
	if(rs.IsOpen() && !rs.IsEOF())
	{
		nMaxIndex = rs.GetFieldValue(0).lVal;
	}
	rs.Close();

	return nMaxIndex;
}

BOOL CLogDataDB::SearchErrorLogBySQL(CString strSQL, CLogErrorInfoList* pList)
{
	CDaoRecordset rs(m_pDaoDB);
	try
	{
		rs.Open(dbOpenDynaset, strSQL);
		COleVariant var;
		if(rs.IsOpen())
		{
			int Cnt = rs.GetFieldCount();
			while(!rs.IsEOF())
			{
				CLogErrorInfo* pLog = new CLogErrorInfo;
				for(int i = 0; i<Cnt; i++)
				{
					var = rs.GetFieldValue(i);
					if (var.vt == VT_NULL)
						continue;
					switch(i)
					{
					case ERROR_INDEX:
						pLog->SetIndex(V_I4(&var));
						break;
					case ERROR_DATE:
						pLog->SetDate(V_BSTRT(&var));
						break;
					case ERROR_CONTENT:
						pLog->SetError(V_BSTRT(&var));
						break;
					case ERROR_PARA1:
						pLog->SetErrPara1(V_BSTRT(&var));
						break;
					case ERROR_PARA2:
						pLog->SetErrPara2(V_BSTRT(&var));
						break;
					case ERROR_USER:
						pLog->SetUser(V_BSTRT(&var));
						break;
					case ERROR_INSTRUMENT:
						pLog->SetInstrument(V_BSTRT(&var));
						break;
					case ERROR_LOG_TYPE:
						pLog->SetLogType((eLogType)V_I1(&var));
						break;
					default:
						ASSERT(FALSE);
						break;
					}
				}
				pList->AddTail(pLog);
				rs.MoveNext();
			}
			rs.Close();
		}
	}
	catch (CDaoException* e) 
	{
		e->ReportError ();
		e->Delete ();
	}

	return TRUE;
}

//*****************************************************************************************************************
// 通讯日志信息表

BOOL CLogDataDB::InsertCommRecord(CLogCommInfo* pLog)
{
	CDaoDatabase* pDB = m_pDaoDB;
	CString strSQL;

	CString strRecord, strValue, strTemp;
	strRecord.Format(_T("%s"), m_strCommTableName);
	strRecord += _T("(");
	for(int i = 1; i<TABLE_COMM_FIELD_COUNT; i++)
	{
		strTemp.Format(_T("%s"), m_arrayCommTableField[i]);
		strRecord += strTemp;
		if(i < TABLE_COMM_FIELD_COUNT -1)
		{
			strRecord += _T(",");
		}
	}
	strRecord += _T(")");
	strValue = _T("(");

	for(int i = 1; i<TABLE_COMM_FIELD_COUNT; i++)
	{
		switch(i)
		{
		case COMM_LOG_DATE:
			strTemp.Format(_T("'%s'"), pLog->GetDate());
			break;
		case COMM_INSTRUMENT:
			strTemp.Format(_T("'%s'"), pLog->GetInstrument());
			break;
		case COMM_COMMENT:
			strTemp.Format(_T("'%s'"), pLog->GetComment());
			break;
		case COMM_TYPE:
			strTemp.Format(_T("%d"), pLog->GetCommType());
			break;
		case COMM_RESULT:
			strTemp.Format(_T("%d"), pLog->GetResult());
			break;
		case COMM_SEND_COUNT:
			strTemp.Format(_T("%d"), pLog->GetSendCount());
			break;
		case COMM_CONTENT:
			strTemp.Format(_T("'%s'"), pLog->GetContent());
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		strValue += strTemp;
		if(i < TABLE_COMM_FIELD_COUNT -1)
		{
			strValue += _T(",");
		}
	}
	strValue += _T(")");
	strSQL.Format(_T("INSERT INTO %s VALUES %s"), strRecord, strValue);
	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	return TRUE;
}

BOOL CLogDataDB::SearchCommLog(CLogCommInfoList* pList, CString strInstrument)
{
	CString strSQL;

	if (strInstrument.IsEmpty())
	{
		strSQL.Format(_T("SELECT * FROM %s ORDER BY %s"), m_strCommTableName, m_arrayCommTableField[COMM_INDEX]);
	}
	else
	{
		strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s' ORDER BY %s"), m_strCommTableName
			, m_arrayCommTableField[COMM_INSTRUMENT], strInstrument, m_arrayCommTableField[COMM_INDEX]);
	}

	CDaoRecordset rs(m_pDaoDB);
	try
	{
		rs.Open(dbOpenDynaset, strSQL);
		COleVariant var;
		if(rs.IsOpen())
		{
			int Cnt = rs.GetFieldCount();
			while(!rs.IsEOF())
			{
				CLogCommInfo* pLog = new CLogCommInfo;
				for(int i=1; i<Cnt; i++)
				{
					var = rs.GetFieldValue(i);
					if (var.vt == VT_NULL)
						continue;
					switch(i)
					{
					case COMM_LOG_DATE:
						pLog->SetDate(V_BSTRT(&var));
						break;
					case COMM_INSTRUMENT:
						pLog->SetInstrument(V_BSTRT(&var));
						break;
					case COMM_COMMENT:
						pLog->SetComment(V_BSTRT(&var));
						break;
					case COMM_TYPE:
						pLog->SetCommType((CLogCommInfo::eCommType)V_I1(&var));
						break;
					case COMM_RESULT:
						pLog->SetResult(V_I1(&var));
						break;
					case COMM_SEND_COUNT:
						pLog->SetSendCount(V_I1(&var));
						break;
					case COMM_CONTENT:
						pLog->SetContent(V_BSTRT(&var));
						break;
					default:
						ASSERT(FALSE);
						break;
					}
				}
				pList->AddTail(pLog);
				rs.MoveNext();
			}
			rs.Close();
		}
	}
	catch (CDaoException* e) 
	{
		e->ReportError ();
		e->Delete ();
	}

	return TRUE;
}

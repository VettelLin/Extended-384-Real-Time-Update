#include "StdAfx.h"
#include "ConfigureDB.h"


CConfigureDB* CConfigureDB::m_pInstance = NULL;

const UINT TABLE_USER_FIELD_COUNT = 7;
const UINT TABLE_DYE_FIELD_COUNT = 12;
const UINT TABLE_RUN_FIELD_COUNT = 10;

CConfigureDB::CConfigureDB(void)
{
	m_pDaoDB = new CDaoDatabase;
	m_strFileName = _T("DB\\Configure.mdb");
	InitializeCriticalSection(&m_csUseDB);

	m_strUserTableName = _T("UserInfo");
	m_arrayUserTableField.Add(_T("Index"));
	m_arrayUserTableField.Add(_T("UserName"));
	m_arrayUserTableField.Add(_T("Password"));
	m_arrayUserTableField.Add(_T("RoleType"));
	m_arrayUserTableField.Add(_T("Status"));
	m_arrayUserTableField.Add(_T("Mnemonic"));
	m_arrayUserTableField.Add(_T("CreateTime"));
	ASSERT(m_arrayUserTableField.GetCount() == TABLE_USER_FIELD_COUNT);

	m_strDyeTableName = _T("DyeInfo");
	m_arrayDyeTableField.Add(_T("Index"));
	m_arrayDyeTableField.Add(_T("DyeName"));
	m_arrayDyeTableField.Add(_T("ExciteWavelength"));
	m_arrayDyeTableField.Add(_T("ExcitePosition"));
	m_arrayDyeTableField.Add(_T("EmitWavelength"));
	m_arrayDyeTableField.Add(_T("EmitPosition"));
	m_arrayDyeTableField.Add(_T("Target"));
	m_arrayDyeTableField.Add(_T("Quencher"));
	m_arrayDyeTableField.Add(_T("TargetColor"));
	m_arrayDyeTableField.Add(_T("Default"));
	m_arrayDyeTableField.Add(_T("InstrumentSN"));
	m_arrayDyeTableField.Add(_T("UserName"));
	ASSERT(m_arrayDyeTableField.GetCount() == TABLE_DYE_FIELD_COUNT);

	m_strRunTableName = _T("RunInfo");
	m_arrayRunTableField.Add(_T("Index"));
	m_arrayRunTableField.Add(_T("InstrumentSN"));
	m_arrayRunTableField.Add(_T("FileName"));
	m_arrayRunTableField.Add(_T("UserName"));
	m_arrayRunTableField.Add(_T("BeginDate"));
	m_arrayRunTableField.Add(_T("EndDate"));
	m_arrayRunTableField.Add(_T("StopFlag"));
	m_arrayRunTableField.Add(_T("CurSegment"));
	m_arrayRunTableField.Add(_T("CurCycle"));
	m_arrayRunTableField.Add(_T("CurStep"));
	ASSERT(m_arrayRunTableField.GetCount() == TABLE_RUN_FIELD_COUNT);
}

CConfigureDB::~CConfigureDB(void)
{
	if(m_pDaoDB->IsOpen())
	{
		m_pDaoDB->Close();
	}
	delete m_pDaoDB;

	DeleteCriticalSection(&m_csUseDB);
}

CConfigureDB* CConfigureDB::GetInstance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new CConfigureDB;
	}

	return m_pInstance;
}

void CConfigureDB::DestroyInstance()
{
	if(m_pInstance != NULL)
	{
		m_pInstance->Close();
		delete m_pInstance;
	}

	m_pInstance = NULL;
}

BOOL CConfigureDB::Open(CString strPath)
{
	m_strFileName = strPath + m_strFileName;

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

void CConfigureDB::Close()
{
	if(m_pDaoDB->IsOpen())
	{
		m_pDaoDB->Close();
	}
}

//*****************************************************************************************************************
// 用户表
BOOL CConfigureDB::FindUserRecord(CString strName)
{
	BOOL bFind = FALSE;
	CDaoRecordset rs(m_pDaoDB);
	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s WHERE %s = '%s'"), m_strUserTableName, m_arrayUserTableField[USER_NAME], strName);
	try
	{
		rs.Open(dbOpenDynaset , strSQL);
		if(rs.IsOpen())
		{
			int iCount = rs.GetRecordCount();
			if(iCount > 0)
			{
				bFind = TRUE;;
			}
			rs.Close();
		}
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	return bFind;
}

BOOL CConfigureDB::SearchUser(CUserInfoList* pList)
{
	EnterCriticalSection(&m_csUseDB);

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s Where %s <> 1 ORDER BY %s"), m_strUserTableName, m_arrayUserTableField[USER_STATUS], m_arrayUserTableField[USER_INDEX]);

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
				CDBUserInfo *pUser = new CDBUserInfo;
				for(int i = 0; i<Cnt; i++)
				{
					var = rs.GetFieldValue(i);
					if (var.vt == VT_NULL)
						continue;
					switch(i)
					{
					case USER_INDEX:
						pUser->SetIndex(V_I4(&var));
						break;
					case USER_NAME:
						pUser->SetName(V_BSTRT(&var));
						break;
					case USER_PASSWORD:
						pUser->SetPassword(V_BSTRT(&var));
						break;
					case USER_ROLE_TYPE:
						pUser->SetPermissionType((EPERMISSIONTYPE)V_I1(&var));
						break;
					case USER_STATUS:
						pUser->SetStatus(V_I1(&var));
						break;
					case USER_MNEMONIC:
						pUser->SetMnemonic(V_BSTRT(&var));
						break;
					case USER_CREATETM:
						pUser->SetCreateTime(V_BSTRT(&var));
						break;

					default:
						ASSERT(FALSE);
						break;
					}
				}
				pList->AddTail(pUser);
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

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::SearchUser(CString strName,CDBUserInfo& userInfo)
{
	EnterCriticalSection(&m_csUseDB);

	CString strSQL;
	strSQL.Format(_T("SELECT * FROM %s Where %s = '%s'"), m_strUserTableName, m_arrayUserTableField[USER_NAME],strName);

	BOOL bFind = FALSE;

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
				for(int i = 0; i<Cnt; i++)
				{
					var = rs.GetFieldValue(i);
					if (var.vt == VT_NULL)
						continue;
					switch(i)
					{
					case USER_INDEX:
						userInfo.SetIndex(V_I4(&var));
						break;
					case USER_NAME:
						userInfo.SetName(V_BSTRT(&var));
						break;
					case USER_PASSWORD:
						userInfo.SetPassword(V_BSTRT(&var));
						break;
					case USER_ROLE_TYPE:
						userInfo.SetPermissionType((EPERMISSIONTYPE)V_I1(&var));
						break;
					case USER_STATUS:
						userInfo.SetStatus(V_I1(&var));
						break;
					case USER_MNEMONIC:
						userInfo.SetMnemonic(V_BSTRT(&var));
						break;
					case USER_CREATETM:
						userInfo.SetCreateTime(V_BSTRT(&var));
						break;

					default:
						ASSERT(FALSE);
						break;
					}
				}

				rs.MoveNext();

				bFind = TRUE;
			}
			rs.Close();
		}
	}
	catch (CDaoException* e) 
	{
		e->ReportError ();
		e->Delete ();
	}

	LeaveCriticalSection(&m_csUseDB);
	return bFind;
}

BOOL CConfigureDB::InsertUserRecord(CDBUserInfo* pUser)
{
	EnterCriticalSection(&m_csUseDB);

	CDaoDatabase* pDB = m_pDaoDB;
	CString strSQL;

	CString strRecord, strValue, strTemp;
	strRecord.Format(_T("%s"), m_strUserTableName);
	strRecord += _T("(");
	for(int i = 1; i<TABLE_USER_FIELD_COUNT; i++)
	{
		strTemp.Format(_T("%s"), m_arrayUserTableField[i]);
		strRecord += strTemp;
		if(i <TABLE_USER_FIELD_COUNT -1)
			strRecord += _T(",");
	}
	strRecord += _T(")");
	strValue = _T("(");

	for(int i = 1; i<TABLE_USER_FIELD_COUNT; i++)
	{
		switch(i)
		{
		case USER_NAME:
			strTemp.Format(_T("'%s'"),pUser->GetName());
			break;
		case USER_PASSWORD:
			strTemp.Format(_T("'%s'"),pUser->GetPassword());
			break;
		case USER_ROLE_TYPE:
			strTemp.Format(_T("%d"),pUser->GetPermissionType());
			break;
		case USER_STATUS:
			strTemp.Format(_T("%d"),pUser->GetStatus());
			break;
		case USER_MNEMONIC:
			strTemp.Format(_T("'%s'"),pUser->GetMnemonic());
			break;
		case USER_CREATETM:
			strTemp.Format(_T("'%s'"),pUser->GetCreateTime());
			break;

		default:
			ASSERT(FALSE);
			break;
		}
		strValue += strTemp;
		if(i < TABLE_USER_FIELD_COUNT -1)
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

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::UpdateUserRecord(CDBUserInfo* pUser)
{
	if(!FindUserRecord(pUser->GetName()))
		return FALSE;

	EnterCriticalSection(&m_csUseDB);
	CDaoDatabase* pDB = m_pDaoDB;
	CString strSQL;
	CString strValue, strTemp;

	strValue.Empty();
	for(int i = 1; i<TABLE_USER_FIELD_COUNT; i++)
	{
		switch(i)
		{
		case USER_NAME:
			strTemp.Format(_T("%s = '%s'"), m_arrayUserTableField[USER_NAME], pUser->GetName());
			break;
		case USER_PASSWORD:
			strTemp.Format(_T("%s = '%s'"), m_arrayUserTableField[USER_PASSWORD], pUser->GetPassword());
			break;
		case USER_ROLE_TYPE:
			strTemp.Format(_T("%s = %d"), m_arrayUserTableField[USER_ROLE_TYPE], pUser->GetPermissionType());
			break;
		case USER_STATUS:
			strTemp.Format(_T("%s = %d"), m_arrayUserTableField[USER_STATUS], pUser->GetStatus());
			break;
		case USER_MNEMONIC:
			strTemp.Format(_T("%s = '%s'"), m_arrayUserTableField[USER_MNEMONIC], pUser->GetMnemonic());
			break;
		case USER_CREATETM:
			strTemp.Format(_T("%s = '%s'"), m_arrayUserTableField[USER_CREATETM], pUser->GetCreateTime());
			break;

		default:
			ASSERT(FALSE);
			break;
		}

		if (strTemp.IsEmpty())
			continue;

		strValue += strTemp;
		if(i < TABLE_USER_FIELD_COUNT -1)
		{
			strValue += _T(",");
		}
	}

	TRACE(strValue +_T("\n"));
	strSQL.Format(_T("UPDATE %s SET %s WHERE %s = '%s'"), m_strUserTableName, strValue, m_arrayUserTableField[USER_NAME], pUser->GetName());

	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::DeleteUserRecord(CDBUserInfo* pUser)
{
	EnterCriticalSection(&m_csUseDB);
	CString strSQL;
	strSQL.Format(_T("DELETE FROM %s WHERE %s = '%s'"), m_strUserTableName, m_arrayUserTableField[USER_NAME], pUser->GetName());

	CDaoDatabase* pDB = m_pDaoDB;
	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

//*****************************************************************************************************************
// 染料表
BOOL CConfigureDB::FindDyeRecord(int nIndex)
{
	BOOL bFind = FALSE;
	CDaoRecordset rs(m_pDaoDB);
	CString strSQL;
	strSQL.Format(_T("SELECT COUNT(*) FROM %s WHERE %s = %d"), m_strDyeTableName, m_arrayDyeTableField[DYE_INDEX], nIndex);
	try
	{
		rs.Open(dbOpenDynaset , strSQL);
		if(rs.IsOpen())
		{
			if (rs.GetRecordCount() > 0)
			{
				bFind = TRUE;;
			}
			rs.Close();
		}
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	return bFind;
}
BOOL CConfigureDB::SearchDye(CDyeInfoList* pList, CString strUser, CString strInstrument)
{
	EnterCriticalSection(&m_csUseDB);
	CString strSQL;

	if (strInstrument.IsEmpty())
	{
		strSQL.Format(_T("SELECT * FROM %s Where %s = '%s' ORDER BY %s"), m_strDyeTableName, m_arrayDyeTableField[DYE_USERNAME], strUser, m_arrayDyeTableField[USER_INDEX]);
	}
	else
	{
		strSQL.Format(_T("SELECT * FROM %s Where %s = '%s' AND %s = '%s' ORDER BY %s"), m_strDyeTableName, m_arrayDyeTableField[DYE_USERNAME], strUser, 
			m_arrayDyeTableField[DYE_INSTRUMENT], strInstrument, m_arrayDyeTableField[USER_INDEX]);
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
				CDBDyeInfo *pDye = new CDBDyeInfo;
				for(int i = 0; i<Cnt; i++)
				{
					var = rs.GetFieldValue(i);
					if (var.vt == VT_NULL)
						continue;
					switch(i)
					{
					case DYE_INDEX:
						pDye->SetIndex(V_I4(&var));
						break;
					case DYE_NAME:
						pDye->SetDye(V_BSTRT(&var));
						break;
					case DYE_EXWAVELENGTH:
						pDye->SetExWavelength(V_I2(&var));
						break;
					case DYE_EXCHANNEL:
						pDye->SetExChannel(V_I1(&var));
						break;
					case DYE_EMWAVELENGTH:
						pDye->SetEmWavelength(V_I2(&var));
						break;
					case DYE_EMCHANNEL:
						pDye->SetEmChannel(V_I1(&var));
						break;
					case DYE_TARGET:
						pDye->SetTarget(V_BSTRT(&var));
						break;
					case DYE_QUENCHER:
						pDye->SetQuencher(V_BSTRT(&var));
						break;
					case DYE_COLOR:
						pDye->SetColor(V_BSTRT(&var));
						break;
					case DYE_DEFAULT:
						pDye->SetDefault(V_UI1(&var));
						break;
					case DYE_INSTRUMENT:
						pDye->SetInstrument(V_BSTRT(&var));
						break;
					case DYE_USERNAME:
						pDye->SetUser(V_BSTRT(&var));
						break;

					default:
						ASSERT(FALSE);
						break;
					}
				}
				pList->AddTail(pDye);

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

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::InsertDyeRecord(CDBDyeInfo* pDye)
{
	EnterCriticalSection(&m_csUseDB);
	CDaoDatabase* pDB = m_pDaoDB;
	CString strSQL;

	CString strRecord, strValue, strTemp;
	strRecord.Format(_T("%s"), m_strDyeTableName);
	strRecord += _T("(");
	for(int i=1; i<TABLE_DYE_FIELD_COUNT; i++)
	{
		strTemp.Format(_T("%s"), m_arrayDyeTableField[i]);
		strRecord += strTemp;
		if(i <TABLE_DYE_FIELD_COUNT -1)
			strRecord += _T(",");
	}
	strRecord += _T(")");
	strValue = _T("(");

	for(int i=1; i<TABLE_DYE_FIELD_COUNT; i++)
	{
		switch(i)
		{
		case DYE_NAME:
			strTemp.Format(_T("'%s'"), pDye->GetDye());
			break;
		case DYE_EXWAVELENGTH:
			strTemp.Format(_T("%d"), pDye->GetExWavelength());
			break;
		case DYE_EXCHANNEL:
			strTemp.Format(_T("%d"), pDye->GetExChannel());
			break;
		case DYE_EMWAVELENGTH:
			strTemp.Format(_T("%d"), pDye->GetEmWavelength());
			break;
		case DYE_EMCHANNEL:
			strTemp.Format(_T("%d"), pDye->GetEmChannel());
			break;
		case DYE_TARGET:
			strTemp.Format(_T("'%s'"), pDye->GetTarget());
			break;
		case DYE_QUENCHER:
			strTemp.Format(_T("'%s'"), pDye->GetQuencher());
			break;
		case DYE_COLOR:
			strTemp.Format(_T("'%s'"), pDye->GetColor());
			break;
		case DYE_DEFAULT:
			strTemp.Format(_T("%d"), pDye->GetDefault());
			break;
		case DYE_INSTRUMENT:
			strTemp.Format(_T("'%s'"), pDye->GetInstruemnt());
			break;
		case DYE_USERNAME:
			strTemp.Format(_T("'%s'"), pDye->GetUser());
			break;
		default:
			ASSERT(FALSE);
			break;
		}
		strValue += strTemp;
		if(i < TABLE_DYE_FIELD_COUNT -1)
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

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::UpdateDyeRecord(CDBDyeInfo* pDye)
{
	if(!FindDyeRecord(pDye->GetIndex()))
		return FALSE;

	EnterCriticalSection(&m_csUseDB);
	CDaoDatabase* pDB = m_pDaoDB;
	CString strSQL;
	CString strValue, strTemp;

	strValue.Empty();
	for(int i=2; i<TABLE_DYE_FIELD_COUNT; i++)
	{
		switch(i)
		{
		case DYE_EXWAVELENGTH:
			strTemp.Format(_T("%s = %d"), m_arrayDyeTableField[DYE_EXWAVELENGTH], pDye->GetExWavelength());
			break;
		case DYE_EXCHANNEL:
			strTemp.Format(_T("%s = %d"), m_arrayDyeTableField[DYE_EXCHANNEL], pDye->GetExChannel());
			break;
		case DYE_EMWAVELENGTH:
			strTemp.Format(_T("%s = %d"), m_arrayDyeTableField[DYE_EMWAVELENGTH], pDye->GetEmWavelength());
			break;
		case DYE_EMCHANNEL:
			strTemp.Format(_T("%s = %d"), m_arrayDyeTableField[DYE_EMCHANNEL], pDye->GetEmChannel());
			break;
		case DYE_TARGET:
			strTemp.Format(_T("%s = '%s'"), m_arrayDyeTableField[DYE_TARGET], pDye->GetTarget());
			break;
		case DYE_QUENCHER:
			strTemp.Format(_T("%s = '%s'"), m_arrayDyeTableField[DYE_QUENCHER], pDye->GetQuencher());
			break;
		case DYE_COLOR:
			strTemp.Format(_T("%s = '%s'"), m_arrayDyeTableField[DYE_COLOR], pDye->GetColor());
			break;
		case DYE_DEFAULT:
			strTemp.Format(_T("%s = %d"), m_arrayDyeTableField[DYE_DEFAULT], pDye->GetDefault());
			break;
		case DYE_INSTRUMENT:
			strTemp.Format(_T("%s = '%s'"), m_arrayDyeTableField[DYE_INSTRUMENT], pDye->GetInstruemnt());
			break;
		case DYE_USERNAME:
			strTemp.Format(_T("%s = '%s'"), m_arrayDyeTableField[DYE_USERNAME], pDye->GetUser());
			break;
		default:
			ASSERT(FALSE);
			break;
		}

		if (strTemp.IsEmpty())
			continue;

		strValue += strTemp;
		if(i < TABLE_DYE_FIELD_COUNT -1)
		{
			strValue += _T(",");
		}
	}

	TRACE(strValue +_T("\n"));
	strSQL.Format(_T("UPDATE %s SET %s WHERE %s = %d"), m_strDyeTableName, strValue, m_arrayDyeTableField[DYE_INDEX], pDye->GetIndex());

	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::DeleteDyeRecord(CDBDyeInfo* pDye)
{
	EnterCriticalSection(&m_csUseDB);
	CString strSQL;
	strSQL.Format(_T("DELETE FROM %s WHERE %s=%d"), m_strDyeTableName, m_arrayDyeTableField[DYE_INDEX], pDye->GetIndex());

	CDaoDatabase* pDB = m_pDaoDB;
	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;

}


//*****************************************************************************************************************
// 运行信息表
BOOL CConfigureDB::FindRunRecord(int nIndex)
{
	BOOL bFind = FALSE;
	CDaoRecordset rs(m_pDaoDB);
	CString strSQL;
	strSQL.Format(_T("SELECT COUNT(*) FROM %s WHERE %s = %d"), m_strRunTableName, m_arrayRunTableField[DYE_INDEX], nIndex);
	try
	{
		rs.Open(dbOpenDynaset , strSQL);
		if(rs.IsOpen())
		{
			if (rs.GetRecordCount() > 0)
			{
				bFind = TRUE;;
			}
			rs.Close();
		}
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	return bFind;
}
BOOL CConfigureDB::SearchRun(CRunInfoList* pList, CString strUser, CString strInstrument, int iStopFlag)
{
	EnterCriticalSection(&m_csUseDB);
	CString strSQL;
	if (iStopFlag == -1)
	{
		strSQL.Format(_T("SELECT * FROM %s Where %s = '%s' AND %s = '%s' ORDER BY %s"), m_strRunTableName
			, m_arrayRunTableField[RUN_INSTRUMENT], m_arrayRunTableField[RUN_USERNAME]
			, strInstrument, strUser, m_arrayRunTableField[RUN_INDEX]);
	}
	else
	{
		strSQL.Format(_T("SELECT * FROM %s Where %s = '%s' AND %s = '%s' AND %s = %d ORDER BY %s"), m_strRunTableName
			, m_arrayRunTableField[RUN_INSTRUMENT], m_arrayRunTableField[RUN_USERNAME], m_arrayRunTableField[RUN_STOP_FLAG]
			, strInstrument, strUser, iStopFlag, m_arrayRunTableField[RUN_INDEX]);
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
				CDBRunInfo *pRunInfo = new CDBRunInfo;
				for(int i = 0; i<Cnt; i++)
				{
					var = rs.GetFieldValue(i);
					if (var.vt == VT_NULL)
						continue;
					switch(i)
					{
					case RUN_INDEX:
						pRunInfo->SetIndex(V_I4(&var));
						break;
					case RUN_INSTRUMENT:
						pRunInfo->SetInstrument(V_BSTRT(&var));
						break;
					case RUN_FILENAME:
						pRunInfo->SetFileName(V_BSTRT(&var));
						break;
					case RUN_USERNAME:
						pRunInfo->SetUser(V_BSTRT(&var));
						break;
					case RUN_BEGIN_DATE:
						pRunInfo->SetBeginTime(V_BSTRT(&var));
						break;
					case RUN_END_DATE:
						pRunInfo->SetEndTime(V_BSTRT(&var));
						break;
					case RUN_STOP_FLAG:
						pRunInfo->SetStopFlag(V_I2(&var));
						break;
					case RUN_CUR_SEGMENT:
						pRunInfo->SetCurSegment(V_I2(&var));
						break;
					case RUN_CUR_CYCLE:
						pRunInfo->SetCurCycle(V_I2(&var));
						break;
					case RUN_CUR_STEP:
						pRunInfo->SetCurStep(V_I2(&var));
						break;

					default:
						ASSERT(FALSE);
						break;
					}
				}
				pList->AddTail(pRunInfo);
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

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::InsertRunRecord(CDBRunInfo* pRunInfo)
{
	//EnterCriticalSection(&m_csUseDB);
	//CDaoDatabase* pDB = m_pDaoDB;
	//CString strSQL;

	//CString strRecord, strValue, strTemp;
	//strRecord.Format(_T("%s"), m_strRunTableName);
	//strRecord += _T("(");
	//for(int i=1; i<TABLE_RUN_FIELD_COUNT; i++)
	//{
	//	strTemp.Format(_T("%s"), m_arrayRunTableField[i]);
	//	strRecord += strTemp;
	//	if(i <TABLE_RUN_FIELD_COUNT -1)
	//		strRecord += _T(",");
	//}
	//strRecord += _T(")");
	//strValue = _T("(");

	//for(int i=1; i<TABLE_RUN_FIELD_COUNT; i++)
	//{
	//	switch(i)
	//	{
	//	case RUN_INSTRUMENT:
	//		strTemp.Format(_T("'%s'"), pRunInfo->GetInstruemnt());
	//		break;
	//	case RUN_FILENAME:
	//		strTemp.Format(_T("'%s'"), pRunInfo->GetFileName());
	//		break;
	//	case RUN_USERNAME:
	//		strTemp.Format(_T("'%s'"), pRunInfo->GetUser());
	//		break;
	//	case RUN_BEGIN_DATE:
	//		strTemp.Format(_T("'%s'"), pRunInfo->GetBeginTime());
	//		break;
	//	case RUN_END_DATE:
	//		strTemp.Format(_T("'%s'"), pRunInfo->GetEndTime());
	//		break;
	//	case RUN_STOP_FLAG:
	//		strTemp.Format(_T("%d"), pRunInfo->GetStopFlag());
	//		break;
	//	case RUN_CUR_SEGMENT:
	//		strTemp.Format(_T("%d"), pRunInfo->GetCurSegment());
	//		break;
	//	case RUN_CUR_CYCLE:
	//		strTemp.Format(_T("%d"), pRunInfo->GetCurCycle());
	//		break;
	//	case RUN_CUR_STEP:
	//		strTemp.Format(_T("%d"), pRunInfo->GetCurStep());
	//		break;
	//	default:
	//		ASSERT(FALSE);
	//		break;
	//	}
	//	strValue += strTemp;
	//	if(i < TABLE_RUN_FIELD_COUNT -1)
	//	{
	//		strValue += _T(",");
	//	}
	//}
	//strValue += _T(")");
	//strSQL.Format(_T("INSERT INTO %s VALUES %s"), strRecord, strValue);
	//try
	//{
	//	pDB->Execute(strSQL);
	//}
	//catch (CDaoException* e) 
	//{
	//	e->ReportError();
	//	e->Delete();
	//}

	//// 找到插入记录的序号
	//long lMaxIndex = -1;
	//strSQL.Format(_T("SELECT MAX(%s) FROM %s"), m_arrayRunTableField[RUN_INDEX], m_strRunTableName);
	//CDaoRecordset rs(m_pDaoDB);
	//rs.Open(dbOpenDynaset, strSQL);
	//if(rs.IsOpen() && !rs.IsEOF())
	//{
	//	lMaxIndex = rs.GetFieldValue(0).lVal;
	//}
	//rs.Close();

	//pRunInfo->SetIndex(lMaxIndex);

	//LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::UpdateRunRecord(CDBRunInfo* pRunInfo, BOOL bStop)
{
	//if(!FindRunRecord(pRunInfo->GetIndex()))
	//	return FALSE;

	//EnterCriticalSection(&m_csUseDB);
	//CDaoDatabase* pDB = m_pDaoDB;
	//CString strSQL;
	//CString strValue, strTemp;

	//strValue.Empty();
	//for(int i=5; i<TABLE_RUN_FIELD_COUNT; i++)
	//{
	//	switch(i)
	//	{
	//	case RUN_END_DATE:
	//		if (bStop)
	//		{
	//			strTemp.Format(_T("%s = '%s'"), m_arrayRunTableField[RUN_END_DATE], pRunInfo->GetEndTime());
	//		}
	//		break;
	//	case RUN_STOP_FLAG:
	//		if (bStop)
	//		{
	//			strTemp.Format(_T("%s = %d"), m_arrayRunTableField[RUN_STOP_FLAG], pRunInfo->GetStopFlag());
	//		}
	//		break;
	//	case RUN_CUR_SEGMENT:
	//		if (!bStop)
	//		{
	//			strTemp.Format(_T("%s = %d"), m_arrayRunTableField[RUN_CUR_SEGMENT], pRunInfo->GetCurSegment());
	//		}
	//		break;
	//	case RUN_CUR_CYCLE:
	//		if (!bStop)
	//		{
	//			strTemp.Format(_T("%s = %d"), m_arrayRunTableField[RUN_CUR_CYCLE], pRunInfo->GetCurCycle());
	//		}
	//		break;
	//	case RUN_CUR_STEP:
	//		if (!bStop)
	//		{
	//			strTemp.Format(_T("%s = %d"), m_arrayRunTableField[RUN_CUR_STEP], pRunInfo->GetCurStep());
	//		}
	//		break;
	//	default:
	//		ASSERT(FALSE);
	//		break;
	//	}

	//	if (strTemp.IsEmpty())
	//		continue;

	//	strValue += strTemp;
	//	if(i < TABLE_RUN_FIELD_COUNT -1)
	//	{
	//		strValue += _T(",");
	//	}
	//}

	//TRACE(strValue +_T("\n"));
	//strSQL.Format(_T("UPDATE %s SET %s WHERE %s = %d"), m_strRunTableName, strValue, m_arrayRunTableField[RUN_INDEX], pRunInfo->GetIndex());

	//try
	//{
	//	pDB->Execute(strSQL);
	//}
	//catch (CDaoException* e) 
	//{
	//	e->ReportError();
	//	e->Delete();
	//}

	//LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

BOOL CConfigureDB::DeleteRunRecord(CDBRunInfo* pRunInfo)
{
	EnterCriticalSection(&m_csUseDB);
	CString strSQL;
	strSQL.Format(_T("DELETE FROM %s WHERE %s=%d"), m_strRunTableName, m_arrayRunTableField[RUN_INDEX], pRunInfo->GetIndex());

	CDaoDatabase* pDB = m_pDaoDB;
	try
	{
		pDB->Execute(strSQL);
	}
	catch (CDaoException* e) 
	{
		e->ReportError();
		e->Delete();
	}

	LeaveCriticalSection(&m_csUseDB);
	return TRUE;
}

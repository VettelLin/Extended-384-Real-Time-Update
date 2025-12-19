#pragma once

#include "ConfigureDBInfo.h"
#include <afxdao.h>
#pragma warning(disable: 4995 311 244)

class AFX_EXT_CLASS CConfigureDB
{
public:
	CConfigureDB(void);
	~CConfigureDB(void);

	enum eTableUserField
	{
		USER_INDEX = 0,
		USER_NAME,
		USER_PASSWORD,
		USER_ROLE_TYPE,
		USER_STATUS,
		USER_MNEMONIC,
		USER_CREATETM
	};

	enum eTableDyeField
	{
		DYE_INDEX = 0,
		DYE_NAME,
		DYE_EXWAVELENGTH,
		DYE_EXCHANNEL,
		DYE_EMWAVELENGTH,
		DYE_EMCHANNEL,
		DYE_TARGET,
		DYE_QUENCHER,
		DYE_COLOR,
		DYE_DEFAULT,
		DYE_INSTRUMENT,
		DYE_USERNAME,
	};

	enum eTableRunField
	{
		RUN_INDEX = 0,
		RUN_INSTRUMENT,
		RUN_FILENAME,
		RUN_USERNAME,
		RUN_BEGIN_DATE,
		RUN_END_DATE,
		RUN_STOP_FLAG,
		RUN_CUR_SEGMENT,
		RUN_CUR_CYCLE,
		RUN_CUR_STEP,
	};

// Operations
public:
	static CConfigureDB* GetInstance();
	static void DestroyInstance();
	BOOL Open(CString strPath);

	// 操作用户表
	BOOL FindUserRecord(CString strName);
	BOOL SearchUser(CUserInfoList* pList);
	BOOL SearchUser(CString strName,CDBUserInfo& userInfo);
	BOOL InsertUserRecord(CDBUserInfo* pUser);
	BOOL UpdateUserRecord(CDBUserInfo* pUser);
	BOOL DeleteUserRecord(CDBUserInfo* pUser);

	// 操作染料表
	BOOL FindDyeRecord(int nIndex);
	BOOL SearchDye(CDyeInfoList* pList, CString strUser, CString strInstrument=_T(""));
	BOOL InsertDyeRecord(CDBDyeInfo* pDye);
	BOOL UpdateDyeRecord(CDBDyeInfo* pDye);
	BOOL DeleteDyeRecord(CDBDyeInfo* pDye);

	// 操作运行信息表
	BOOL FindRunRecord(int nIndex);
	BOOL SearchRun(CRunInfoList* pList, CString strUser, CString strInstrument, int iStopFlag=-1);
	BOOL InsertRunRecord(CDBRunInfo* pRunInfo);
	BOOL UpdateRunRecord(CDBRunInfo* pRunInfo, BOOL bStop=TRUE);
	BOOL DeleteRunRecord(CDBRunInfo* pRunInfo);

protected:
	void Close();

// Attributes
public:
	CDaoDatabase* GetDatabase(){return m_pDaoDB;}
	BOOL IsOpen(){return m_pDaoDB->IsOpen();}

protected:
	static CConfigureDB* m_pInstance;
	CDaoDatabase* m_pDaoDB;
	CString m_strFileName;
	CRITICAL_SECTION m_csUseDB;

	// 用户信息表
	CString m_strUserTableName;
	CStringArray m_arrayUserTableField;

	// 染料信息表，包括所在通道、目标等
	CString m_strDyeTableName;
	CStringArray m_arrayDyeTableField;

	// 运行信息表
	CString m_strRunTableName;
	CStringArray m_arrayRunTableField;
};


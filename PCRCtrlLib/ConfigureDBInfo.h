#pragma once

#include "DeclaresPCR.h"


// 数据库中用户信息
class AFX_EXT_CLASS CDBUserInfo
{
public:
	CDBUserInfo() {
		m_iIndex = -1;
		m_strName.Empty();
		m_strPassword.Empty();
		m_strMnemonic.Empty();
		m_strCreateTm.Empty();
		m_ePermissionType = EPT_ORDINARY;
		m_iStatus = 0;
	}
	virtual ~CDBUserInfo() {}

	CDBUserInfo(const CDBUserInfo& info){
		m_iIndex = info.m_iIndex;
		m_strName = info.m_strName;
		m_strPassword = info.m_strPassword;
		m_strMnemonic = info.m_strMnemonic;
		m_strCreateTm = info.m_strCreateTm;
		m_ePermissionType = info.m_ePermissionType;
		m_iStatus = info.m_iStatus;
	}

	void operator=(const CDBUserInfo& info){
		m_iIndex = info.m_iIndex;
		m_strName = info.m_strName;
		m_strPassword = info.m_strPassword;
		m_strMnemonic = info.m_strMnemonic;
		m_strCreateTm = info.m_strCreateTm;
		m_ePermissionType = info.m_ePermissionType;
		m_iStatus = info.m_iStatus;
	}

	BOOL operator==(const CDBUserInfo& info){
		return m_strName == info.m_strName &&
		m_strPassword == info.m_strPassword &&
		m_strMnemonic == info.m_strMnemonic &&
		m_ePermissionType == info.m_ePermissionType;
	}

	// Attributes
public:
	int GetIndex()  const { return m_iIndex; }
	void SetIndex(int iIndex) { m_iIndex = iIndex; }

	CString GetName() const  { return m_strName; }
	void SetName(CString strName){m_strName = strName;}

	CString GetPassword() const  { return m_strPassword; }
	void SetPassword(CString strPassword){m_strPassword = strPassword;}

	CString GetMnemonic() const  { return m_strMnemonic; }
	void SetMnemonic(CString strData){m_strMnemonic = strData;}

	CString GetCreateTime() const  { return m_strCreateTm; }
	void SetCreateTime(CString strData){m_strCreateTm = strData;}

	EPERMISSIONTYPE GetPermissionType() { return m_ePermissionType; }
	void SetPermissionType(EPERMISSIONTYPE eType) { m_ePermissionType = eType; }

	int GetStatus() { return m_iStatus; }
	void SetStatus(int iStatus) { m_iStatus = iStatus; }

	BOOL GetBeAdminPermission() { return EPT_ADMIN == m_ePermissionType; }
	BOOL GetBeOrdinaryPermission() { return EPT_ORDINARY == m_ePermissionType || EPT_IVD == m_ePermissionType; }
	BOOL GetBeIVDUser() { return EPT_IVD == m_ePermissionType; }

protected:
	int	m_iIndex;
	CString	m_strName; //用户名
	CString	m_strPassword; //密码
	CString	m_strMnemonic;	//助记符
	CString m_strCreateTm;	//用户创建时间
	EPERMISSIONTYPE	m_ePermissionType; //用户权限类型
	int	m_iStatus; //用户创建类型，0：手动创建用户，1：软件默认用户
};

typedef CTypedPtrList<CPtrList, CDBUserInfo*> CUserInfoList;


// 数据库中染料相关信息
class AFX_EXT_CLASS CDBDyeInfo
{
public:
	CDBDyeInfo() {
		m_nIndex = -1;
		m_strDye.Empty();
		m_nExWavelength = 0;
		m_nExChannel = 0;
		m_nEmWavelength = 0;
		m_nEmChannel = 0;
		m_strTarget.Empty();
		m_strQuencher.Empty();
		m_bDefault = FALSE;
		m_strInstrument.Empty();
		m_strUser.Empty();
	}
	virtual ~CDBDyeInfo() {}

// Attributes
public:
	int GetIndex()  const { return m_nIndex; }
	void SetIndex(int nIndex) { m_nIndex = nIndex; }

	CString GetDye() const  { return m_strDye; }
	void SetDye(CString strDye){m_strDye = strDye;}
	int GetExWavelength() { return m_nExWavelength; }
	void SetExWavelength(int nLength) { m_nExWavelength = nLength; }
	int GetExChannel()  { return m_nExChannel; }
	void SetExChannel(int nChannel) { m_nExChannel = nChannel; }
	int GetEmWavelength() { return m_nEmWavelength; }
	void SetEmWavelength(int nLength) { m_nEmWavelength = nLength; }
	int GetEmChannel()  { return m_nEmChannel; }
	void SetEmChannel(int nChannel) { m_nEmChannel = nChannel; }

	CString GetTarget() { return m_strTarget; }
	void SetTarget(CString strTarget) { m_strTarget = strTarget; }
	CString GetQuencher() { return m_strQuencher; }
	void SetQuencher(CString strQuencher) { m_strQuencher = strQuencher; }
	CString GetColor() { return m_strColor; } 
	void SetColor(CString strColor) { m_strColor = strColor; }

	BOOL GetDefault() { return m_bDefault; }
	void SetDefault(BOOL bDefault) { m_bDefault = bDefault; }

	CString GetInstruemnt() { return m_strInstrument; }
	void SetInstrument(CString strInstrument) { m_strInstrument = strInstrument; }
	CString GetUser() { return m_strUser; }
	void SetUser(CString strUser) { m_strUser = strUser; }

protected:
	int			m_nIndex;
	CString		m_strDye; // 染料名
	int			m_nExWavelength; // 激发光波长
	int			m_nExChannel; // 激发通道
	int			m_nEmWavelength; // 发射光波长
	int			m_nEmChannel; // 发射通道
	CString		m_strTarget; // 目标
	CString		m_strQuencher; // 淬灭者
	CString		m_strColor; // 目标显示颜色
	BOOL		m_bDefault; //是否通道中默认
	CString		m_strInstrument; // 设备ID
	CString		m_strUser; // 用户名
};

typedef CTypedPtrList<CPtrList, CDBDyeInfo*> CDyeInfoList;

// 数据库中实验执行状态信息，方便中止实验接着进行
class AFX_EXT_CLASS CDBRunInfo
{
public:
	CDBRunInfo() {
		m_lIndex = -1;
		m_strInstrument.Empty();
		m_strFileName.Empty();
		m_strUser.Empty();
		m_strBeginTime.Empty();
		m_strEndTime.Empty();
		m_iStopFlag = STOP_FLAG_RUNNING;
		m_iCurSegment = 0;
		m_iCurCycle = 0;
		m_iCurStep = 0;
	}
	virtual ~CDBRunInfo() {}

	enum eStopFlagType
	{
		STOP_FLAG_NORMAL = 0, // 正常结束
		STOP_FLAG_RUNNING = 1, // 正在运行
		STOP_FLAG_USER = 2, // 用户中止
		STOP_FLAG_ERROR = 3, // 错误中止
	};

// Attributes
public:
	long GetIndex()  const { return m_lIndex; }
	void SetIndex(long lIndex) { m_lIndex = lIndex; }

	CString GetInstruemnt() { return m_strInstrument; }
	void SetInstrument(CString strInstrument) { m_strInstrument = strInstrument; }
	CString GetFileName() { return m_strFileName; }
	void SetFileName(CString strFile) { m_strFileName = strFile; }
	CString GetUser() { return m_strUser; }
	void SetUser(CString strUser) { m_strUser = strUser; }

	CString GetBeginTime() { return m_strBeginTime; }
	void SetBeginTime(CString strTime) { m_strBeginTime = strTime; }
	CString GetEndTime() { return m_strEndTime; }
	void SetEndTime(CString strTime) { m_strEndTime = strTime; }

	int GetStopFlag() { return m_iStopFlag; }
	void SetStopFlag(int iFlag) { m_iStopFlag = iFlag; }
	int GetCurSegment() { return m_iCurSegment; }
	void SetCurSegment(int iSegment) { m_iCurSegment = iSegment; }
	int GetCurCycle() { return m_iCurCycle; }
	void SetCurCycle(int iCycle) { m_iCurCycle = iCycle; }
	int GetCurStep() { return m_iCurStep; }
	void SetCurStep(int iStep) { m_iCurStep = iStep; }

protected:
	long		m_lIndex;
	CString		m_strInstrument; // 设备ID
	CString		m_strFileName; // 文件名
	CString		m_strUser; // 用户名
	CString		m_strBeginTime; // 实验开始时间
	CString		m_strEndTime; // 实验结束时间
	int			m_iStopFlag; // 运行状态
	int			m_iCurSegment;
	int			m_iCurCycle;
	int			m_iCurStep;
};

typedef CTypedPtrList<CPtrList, CDBRunInfo*> CRunInfoList;

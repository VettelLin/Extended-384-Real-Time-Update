#pragma once

#include "InstancePCR.h"
#include "ExpRunPara.h"

class AFX_EXT_CLASS CExpDeviceStatusThread :	public CWinThread
{
	DECLARE_DYNCREATE(CExpDeviceStatusThread)

public:
	CExpDeviceStatusThread();
	virtual ~CExpDeviceStatusThread(void);

// Operations
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual int Run();

protected:
	void InitParameter();

// Attributes
public:
	void SetInstance(CInstancePCR* pInstance) { m_pInstance = pInstance; }
	void SetAskStop(BOOL bStop) { m_bAskStop = bStop; }
	void SetExpRunPara(CExpRunPara* pRunPara) { m_pRunPara = pRunPara; }
	void SetRunStatus(eExpRunStatus nStatus) { m_nRunStatus = nStatus; }

protected:
	CInstancePCR* m_pInstance;
	CExpRunPara* m_pRunPara; // 运行状态参数
	eExpRunStatus m_nRunStatus;

	BOOL m_bAskStop; // 外部中止
	int m_nDelay; // 每次取温度间隔，毫秒
};


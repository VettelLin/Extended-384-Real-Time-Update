#pragma once

#include "InstancePCR.h"
#include "ExperimentData.h"
#include "ExpRunPara.h"


// PCR实验中读取温度数据线程
class AFX_EXT_CLASS CExpTempDataThread : public CWinThread
{
	DECLARE_DYNCREATE(CExpTempDataThread)

public:
	CExpTempDataThread();
	virtual ~CExpTempDataThread(void);

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
	void SetExperimentData(CExperimentData* pExpData) { m_pExpData = pExpData; }
	void SetExpRunPara(CExpRunPara* pRunPara) { m_pRunPara = pRunPara; }
	void SetMsgWnd(CWnd* pWnd) { m_pMsgWnd = pWnd; }
	void SetAskStop(BOOL bStop) { m_bAskStop = bStop; }
	void SetRunStatus(eExpRunStatus nStatus) { m_nRunStatus = nStatus; }

	void SetStartTime(DWORD dwStartTime) { m_dwStartTime = dwStartTime; }

protected:
	CInstancePCR* m_pInstance;
	
	CExperimentData* m_pExpData;// 保存数据对象指针
	CExpRunPara*	m_pRunPara; // 运行状态参数
	CWnd*	m_pMsgWnd; // 接受消息窗口
	
	eExpRunStatus	m_nRunStatus;
	BOOL			m_bAskStop; // 外部中止
	int				m_nDelay; // 每次取温度间隔，毫秒
	int				m_nTempChannelCount; // 加热模块温度通道数，不包括热盖个数
	BOOL			m_bUseLidHeater; // 使用热盖

	DWORD	m_dwStartTime;
};


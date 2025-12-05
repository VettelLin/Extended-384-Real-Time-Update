#pragma once

#include "LinkList.h"
#include "DeclaresPCR.h"

class AFX_EXT_CLASS CHardwareInfo
{
public:
	CHardwareInfo(void);
	~CHardwareInfo(void);

	CHardwareInfo(const CHardwareInfo& info);
	void operator=(const CHardwareInfo& info);

	BOOL WriteFile(CFile* pFile,BOOL bTemplate);
	BOOL ReadFile(CFile* pFile,WORD wVersion);

	BOOL WriteScanChannelInfoToFile(CFile* pFile,BOOL bTemplate);
	BOOL ReadScanChannelInfoFromFile(CFile* pFile,WORD wVersion,BOOL bTemplate);

	void ResetHardwareInfo();	//重置（清空）硬件信息

	BOOL GetBeEmptyHardware();	//判断是否是空的硬件信息，一般Demo模式会导致空的硬件信息

public:
	CString	m_strModel; // PCR仪型号
	CString	m_strSN; // 产品序列号
	CString	m_strCode; // PCR仪版本(类型)
	CString	m_strVersion; // 硬件版本
	CString	m_strSoftwareVer; // 嵌入式软件版本

	CLinkList<tagScanChannelInfo> m_lnkScanChanInfo;
};


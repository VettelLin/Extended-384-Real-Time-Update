#pragma once

//康研葆使用的相关信息

class CCYInfo
{
public:
	CCYInfo(void);
	~CCYInfo(void);

	CCYInfo(const CCYInfo& info);
	void operator=(const CCYInfo& info);

public:
	CString m_strClient;	//委托单位
	CString m_strCpAdr;	//委托单位地址
	CString m_strRptSoftDir;	//报告生成程序目录
};


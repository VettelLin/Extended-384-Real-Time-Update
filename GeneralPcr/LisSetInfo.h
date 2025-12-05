#pragma once

typedef enum CheckWay{
	ECK_NONE,ECK_PARITYCHECK
}ECHEKCWAY;

class CLisSetInfo
{
public:
	CLisSetInfo(void);
	~CLisSetInfo(void);

	CLisSetInfo(const CLisSetInfo& lisSetInfo);
	void operator=(const CLisSetInfo& lisSetInfo);

public:
	BOOL m_bOpenLis;
	double m_dRecvWaitMaxSec;
	int m_maxTransferTimes;

	BOOL m_bUseCom;
	CString m_strComId;
	int m_dataNum;
	double m_dStopBit;
	int m_baudRate;

	CString m_strIpAddress;
	CString m_strPort;

	ECHEKCWAY m_eCheckWay;
};


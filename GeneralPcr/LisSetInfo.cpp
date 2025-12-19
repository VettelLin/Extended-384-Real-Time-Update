#include "StdAfx.h"
#include "LisSetInfo.h"


CLisSetInfo::CLisSetInfo(void)
{
	m_bOpenLis = FALSE;
	m_dRecvWaitMaxSec = 5;
	m_maxTransferTimes = 3;

	m_bUseCom = TRUE;
	m_strComId = _T("COM1");
	m_dataNum = 8;
	m_dStopBit = 1;
	m_baudRate = 9600;
	m_strIpAddress = _T("127.0.0.1");
	m_strPort = _T("21110");
}

CLisSetInfo::~CLisSetInfo(void)
{

}

CLisSetInfo::CLisSetInfo(const CLisSetInfo& lisSetInfo)
{
	m_bOpenLis = lisSetInfo.m_bOpenLis;
	m_dRecvWaitMaxSec = lisSetInfo.m_dRecvWaitMaxSec;
	m_maxTransferTimes = lisSetInfo.m_maxTransferTimes;
	m_bUseCom = lisSetInfo.m_bUseCom;
	m_strComId = lisSetInfo.m_strComId;
	m_dataNum = lisSetInfo.m_dataNum;
	m_dStopBit = lisSetInfo.m_dStopBit;
	m_baudRate = lisSetInfo.m_baudRate;
	m_strIpAddress = lisSetInfo.m_strIpAddress;
	m_strPort = lisSetInfo.m_strPort;
}

void CLisSetInfo::operator=(const CLisSetInfo& lisSetInfo)
{
	m_bOpenLis = lisSetInfo.m_bOpenLis;
	m_dRecvWaitMaxSec = lisSetInfo.m_dRecvWaitMaxSec;
	m_maxTransferTimes = lisSetInfo.m_maxTransferTimes;
	m_bUseCom = lisSetInfo.m_bUseCom;
	m_strComId = lisSetInfo.m_strComId;
	m_dataNum = lisSetInfo.m_dataNum;
	m_dStopBit = lisSetInfo.m_dStopBit;
	m_baudRate = lisSetInfo.m_baudRate;
	m_strIpAddress = lisSetInfo.m_strIpAddress;
	m_strPort = lisSetInfo.m_strPort;
}




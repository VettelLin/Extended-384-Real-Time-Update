#include "StdAfx.h"
#include "CYInfo.h"


CCYInfo::CCYInfo(void)
{
	m_strClient.Empty();
	m_strCpAdr.Empty();
	m_strRptSoftDir.Empty();
}

CCYInfo::~CCYInfo(void)
{

}

CCYInfo::CCYInfo(const CCYInfo& info)
{
	m_strClient = info.m_strClient;
	m_strCpAdr = info.m_strCpAdr;
	m_strRptSoftDir = info.m_strRptSoftDir;
}

void CCYInfo::operator=(const CCYInfo& info)
{
	m_strClient = info.m_strClient;
	m_strCpAdr = m_strCpAdr;
	m_strRptSoftDir = info.m_strRptSoftDir;
}




#include "StdAfx.h"
#include "PeltierCheckParam.h"


CPeltierCheckParam::CPeltierCheckParam(void)
{
	m_bCheck = TRUE;
	m_dTemThre = 50;
	m_dRiseTemError = 5;
	m_dDropTemError = 5;
	m_dTemRiseDropRange = 20;
	m_fRiseTimeSec = 16;	
	m_fDropTimeSec = 16;	
}

CPeltierCheckParam::~CPeltierCheckParam(void)
{

}

CPeltierCheckParam::CPeltierCheckParam(const CPeltierCheckParam& info)
{
	m_bCheck = info.m_bCheck;
	m_dTemThre = info.m_dTemThre;
	m_dRiseTemError = info.m_dRiseTemError;
	m_dDropTemError = info.m_dDropTemError;
	m_dTemRiseDropRange = info.m_dTemRiseDropRange;
	m_fRiseTimeSec = info.m_fRiseTimeSec;	
	m_fDropTimeSec = info.m_fDropTimeSec;	
}

void CPeltierCheckParam::operator=(const CPeltierCheckParam& info)
{
	m_bCheck = info.m_bCheck;
	m_dTemThre = info.m_dTemThre;
	m_dRiseTemError = info.m_dRiseTemError;
	m_dDropTemError = info.m_dDropTemError;
	m_dTemRiseDropRange = info.m_dTemRiseDropRange;
	m_fRiseTimeSec = info.m_fRiseTimeSec;	
	m_fDropTimeSec = info.m_fDropTimeSec;	
}

BOOL CPeltierCheckParam::IsCheck()
{
	return m_bCheck;
}

double CPeltierCheckParam::GetTemperatureThreshold()
{
	return m_dTemThre;
}

double CPeltierCheckParam::GetRiseTemError()
{
	return m_dRiseTemError;
}

double CPeltierCheckParam::GetDropTemError()
{
	return m_dDropTemError;
}

double CPeltierCheckParam::GetTemRiseDropRange()
{
	return m_dTemRiseDropRange;
}

float CPeltierCheckParam::GetRiseTimeSec()
{
	return m_fRiseTimeSec;
}

float CPeltierCheckParam::GetDropTimeSec()
{
	return m_fDropTimeSec;
}






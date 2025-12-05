#include "StdAfx.h"
#include "ChanMeltAnaParamChangeInfo.h"

CChanMeltAnaParamChangeInfo::CChanMeltAnaParamChangeInfo(void)
{
	m_bMinTemChanged = FALSE;
	m_bMaxTemChanged = FALSE;
	m_bPeakThreChanged = FALSE;
}

CChanMeltAnaParamChangeInfo::~CChanMeltAnaParamChangeInfo(void)
{

}

CChanMeltAnaParamChangeInfo::CChanMeltAnaParamChangeInfo(const CChanMeltAnaParamChangeInfo& info)
{
	m_bMinTemChanged = info.m_bMinTemChanged;
	m_bMaxTemChanged = info.m_bMaxTemChanged;
	m_bPeakThreChanged = info.m_bPeakThreChanged;
}

void CChanMeltAnaParamChangeInfo::operator=(const CChanMeltAnaParamChangeInfo& info)
{
	m_bMinTemChanged = info.m_bMinTemChanged;
	m_bMaxTemChanged = info.m_bMaxTemChanged;
	m_bPeakThreChanged = info.m_bPeakThreChanged;
}

BOOL CChanMeltAnaParamChangeInfo::IsAnaParamChanged()
{
	return m_bMinTemChanged || m_bMaxTemChanged || m_bPeakThreChanged;
}


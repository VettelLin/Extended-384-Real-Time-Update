#include "StdAfx.h"
#include "MeltAnaParamChangeInfo.h"

CMeltAnaParamChangeInfo::CMeltAnaParamChangeInfo(void)
{
	m_iChanNum = 0;
}

CMeltAnaParamChangeInfo::~CMeltAnaParamChangeInfo(void)
{

}

CMeltAnaParamChangeInfo::CMeltAnaParamChangeInfo(const CMeltAnaParamChangeInfo& info)
{
	m_iChanNum = info.m_iChanNum;
	m_vecChanMeltChangeInfo = info.m_vecChanMeltChangeInfo;
}

void CMeltAnaParamChangeInfo::operator=(const CMeltAnaParamChangeInfo& info)
{
	m_iChanNum = info.m_iChanNum;
	m_vecChanMeltChangeInfo = info.m_vecChanMeltChangeInfo;
}

int CMeltAnaParamChangeInfo::SetChannelNum(int iChanNum)
{ 
	if(iChanNum < 0)
	{
		return -1;
	}

	if(m_iChanNum == iChanNum)
	{
		return 1;
	}

	m_iChanNum = iChanNum;
	m_vecChanMeltChangeInfo.clear();

	for(int i = 0;i < m_iChanNum;++i)
	{
		CChanMeltAnaParamChangeInfo temp;
		m_vecChanMeltChangeInfo.push_back(temp);
	}

	return 0;
}

BOOL CMeltAnaParamChangeInfo::IsAnaParamChanged()
{
	if(m_iChanNum < 1)
	{
		return FALSE;
	}

	for(int i = 0;i < m_iChanNum;++i)
	{
		if(m_vecChanMeltChangeInfo[i].IsAnaParamChanged())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CMeltAnaParamChangeInfo::IsChannelAnaParamChanged(int iChanId)
{
	if(iChanId >= m_iChanNum || iChanId < 0)
	{
		return FALSE;
	}

	return m_vecChanMeltChangeInfo[iChanId].IsAnaParamChanged();
}


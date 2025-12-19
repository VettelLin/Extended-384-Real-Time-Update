#include "StdAfx.h"
#include "SamNameAutoSetInfo.h"


CSamNameAutoSetInfo::CSamNameAutoSetInfo(void)
{
	m_strPrjName = _T("");
	m_strSamIdPrefix = _T("");
	m_startValue = 1;
	m_interval = 1;
	m_bitNum = 1; 
	m_eSamSortWay = ESSW_HORI;
}

CSamNameAutoSetInfo::~CSamNameAutoSetInfo(void)
{

}

CSamNameAutoSetInfo::CSamNameAutoSetInfo(const CSamNameAutoSetInfo& samIdAutoSetInfo)
{
	m_strPrjName = samIdAutoSetInfo.m_strPrjName;
	m_strSamIdPrefix = samIdAutoSetInfo.m_strSamIdPrefix;
	m_startValue = samIdAutoSetInfo.m_startValue;
	m_interval = samIdAutoSetInfo.m_interval;
	m_bitNum = samIdAutoSetInfo.m_bitNum;
	m_eSamSortWay = samIdAutoSetInfo.m_eSamSortWay;
}

void CSamNameAutoSetInfo::operator=(const CSamNameAutoSetInfo& samIdAutoSetInfo)
{
	m_strPrjName = samIdAutoSetInfo.m_strPrjName;
	m_strSamIdPrefix = samIdAutoSetInfo.m_strSamIdPrefix;
	m_startValue = samIdAutoSetInfo.m_startValue;
	m_interval = samIdAutoSetInfo.m_interval;
	m_bitNum = samIdAutoSetInfo.m_bitNum;
	m_eSamSortWay = samIdAutoSetInfo.m_eSamSortWay;
}

#include "StdAfx.h"
#include "AxisMaxMinInfo.h"


CAxisMaxMinInfo::CAxisMaxMinInfo(void)
{
	m_dXMax = 0;
	m_dXMin = 40;
	m_dYMax = 0;
	m_dYMin = 1;
}


CAxisMaxMinInfo::~CAxisMaxMinInfo(void)
{

}

CAxisMaxMinInfo::CAxisMaxMinInfo(const CAxisMaxMinInfo& axisMaxMinInfo)
{
	m_dXMax = axisMaxMinInfo.m_dXMax;
	m_dXMin = axisMaxMinInfo.m_dXMin;
	m_dYMax = axisMaxMinInfo.m_dYMax;
	m_dYMin = axisMaxMinInfo.m_dYMin;
}

void CAxisMaxMinInfo::operator=(const CAxisMaxMinInfo& axisMaxMinInfo)
{
	m_dXMax = axisMaxMinInfo.m_dXMax;
	m_dXMin = axisMaxMinInfo.m_dXMin;
	m_dYMax = axisMaxMinInfo.m_dYMax;
	m_dYMin = axisMaxMinInfo.m_dYMin;
}

#include "StdAfx.h"

#include<math.h>
#include "AxisInfo.h"

CAxisInfo::CAxisInfo(void)
{
	m_dXMin = 0;
	m_dXMax = 1;
	m_dYMin = 0;
	m_dYMax = 1;
}

CAxisInfo::~CAxisInfo(void)
{
	
}

CAxisInfo::CAxisInfo(const CAxisInfo& axisInfo)
{
	m_dXMin = axisInfo.m_dXMin;
	m_dXMax = axisInfo.m_dXMax;
	m_dYMin = axisInfo.m_dYMin;
	m_dYMax = axisInfo.m_dYMax;
}

void CAxisInfo::operator=(const CAxisInfo& axisInfo)
{
	m_dXMin = axisInfo.m_dXMin;
	m_dXMax = axisInfo.m_dXMax;
	m_dYMin = axisInfo.m_dYMin;
	m_dYMax = axisInfo.m_dYMax;
}

BOOL CAxisInfo::operator==(const CAxisInfo& axisInfo)
{
	return m_dXMin == axisInfo.m_dXMin &&
		   m_dXMax == axisInfo.m_dXMax && 
		   m_dYMin == axisInfo.m_dYMin && 
		   m_dYMax == axisInfo.m_dYMax;
}

CAxisInfo CAxisInfo::CalculateLogValaue()
{
	CAxisInfo axisInfo;
	if(m_dXMin <= 0)
	{
		axisInfo.m_dXMin = 0;
	}
	else
	{
		axisInfo.m_dXMin = log10(m_dXMin);
	}
	
	if(m_dXMax <= 0)
	{
		axisInfo.m_dXMax = axisInfo.m_dXMin + 1;
	}
	else
	{
		axisInfo.m_dXMax = log10(m_dXMax);
	}

	if(m_dYMin <= 0)
	{
		axisInfo.m_dYMin = 0;
	}
	else
	{
		axisInfo.m_dYMin = log10(m_dYMin);
	}
	
	if(m_dYMax <= 0)
	{
		axisInfo.m_dYMax = axisInfo.m_dYMin + 1;
	}
	else
	{
		axisInfo.m_dYMax = log10(m_dYMax);
	}
	
	return axisInfo;
}

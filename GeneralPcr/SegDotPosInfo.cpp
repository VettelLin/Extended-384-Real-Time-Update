#include "StdAfx.h"
#include "SegDotPosInfo.h"


CSegDotPosInfo::CSegDotPosInfo(void)
{
	m_seg = -1;
	m_step = -1; 
	m_eDotPosType = EDP_NONE;
}


CSegDotPosInfo::~CSegDotPosInfo(void)
{
}

CSegDotPosInfo::CSegDotPosInfo(const CSegDotPosInfo& segDotPosInfo)
{
	m_seg = segDotPosInfo.m_seg;
	m_step = segDotPosInfo.m_step;
	m_eDotPosType = segDotPosInfo.m_eDotPosType;
	m_rcItem = segDotPosInfo.m_rcItem;
	m_pt = segDotPosInfo.m_pt;
}

void CSegDotPosInfo::operator=(const CSegDotPosInfo& segDotPosInfo)
{
	m_seg = segDotPosInfo.m_seg;
	m_step = segDotPosInfo.m_step;
	m_eDotPosType = segDotPosInfo.m_eDotPosType;
	m_rcItem = segDotPosInfo.m_rcItem;
	m_pt = segDotPosInfo.m_pt;
}

BOOL CSegDotPosInfo::BeValidStepEmptyPos()
{
	return (-1 != m_seg && -1 != m_step && EDP_NONE == m_eDotPosType);
}

BOOL CSegDotPosInfo::BeValidSegmentEmptyPos()
{
	return (-1 != m_seg && -1 == m_step && EDP_SEGEMPTY == m_eDotPosType);
}

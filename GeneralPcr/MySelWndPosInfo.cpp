#include "StdAfx.h"
#include "MySelWndPosInfo.h"

CMySelWndDotPosInfo::CMySelWndDotPosInfo(void)
{
	m_eDotPosType = EMSWDPT_NONE;
	m_rowId = -1;
}

CMySelWndDotPosInfo::~CMySelWndDotPosInfo(void)
{

}

CMySelWndDotPosInfo::CMySelWndDotPosInfo(const CMySelWndDotPosInfo& dotPosInfo)
{
	m_eDotPosType = dotPosInfo.m_eDotPosType;
	m_rowId = dotPosInfo.m_rowId;
	m_pt = dotPosInfo.m_pt;
}

void CMySelWndDotPosInfo::operator=(const CMySelWndDotPosInfo& dotPosInfo)
{
	m_eDotPosType = dotPosInfo.m_eDotPosType;
	m_rowId = dotPosInfo.m_rowId;
	m_pt = dotPosInfo.m_pt;
}

BOOL CMySelWndDotPosInfo::operator==(const CMySelWndDotPosInfo& dotPosInfo)
{
	return m_eDotPosType == dotPosInfo.m_eDotPosType && 
			m_rowId == dotPosInfo.m_rowId;
}

BOOL CMySelWndDotPosInfo::BeSameRow(CMySelWndDotPosInfo& dotPosInfo)
{
	if(EMSWDPT_NONE == dotPosInfo.m_eDotPosType)
	{
		return FALSE;
	}
	if(-1 == m_rowId)
	{
		return FALSE;
	}
	return m_rowId == dotPosInfo.m_rowId;
}

void CMySelWndDotPosInfo::Clear()
{
	m_rowId = -1;
	m_pt.x = 0;
	m_pt.y = 0;
}
#include "StdAfx.h"
#include "DotPosInfo.h"


CDotPosInfo::CDotPosInfo(void)
{
	m_eDotPosType = EDT_NONE;
	m_rowId = -1;
	m_colId = -1;
}

CDotPosInfo::~CDotPosInfo(void)
{

}

CDotPosInfo::CDotPosInfo(const CDotPosInfo& dotPosInfo)
{
	m_eDotPosType = dotPosInfo.m_eDotPosType;
	m_rowId = dotPosInfo.m_rowId;
	m_colId = dotPosInfo.m_colId;
	m_pt = dotPosInfo.m_pt;
}

void CDotPosInfo::operator=(const CDotPosInfo& dotPosInfo)
{
	m_eDotPosType = dotPosInfo.m_eDotPosType;
	m_rowId = dotPosInfo.m_rowId;
	m_colId = dotPosInfo.m_colId;
	m_pt = dotPosInfo.m_pt;
}

BOOL CDotPosInfo::operator==(const CDotPosInfo& dotPosInfo)
{
	return m_eDotPosType == dotPosInfo.m_eDotPosType && 
			m_rowId == dotPosInfo.m_rowId && 
			m_colId == dotPosInfo.m_colId;
}

BOOL CDotPosInfo::BeSameRowCol(CDotPosInfo& dotPosInfo)
{
	if(EDT_NONE == dotPosInfo.m_eDotPosType)
	{
		return FALSE;
	}
	if(-1 == m_rowId && -1 == m_colId)
	{
		return FALSE;
	}

	return m_rowId == dotPosInfo.m_rowId &&
		m_colId == dotPosInfo.m_colId;
}

BOOL CDotPosInfo::BeSameRow(CDotPosInfo& dotPosInfo)
{
	if(EDT_NONE == dotPosInfo.m_eDotPosType)
	{
		return FALSE;
	}
	if(-1 == m_rowId)
	{
		return FALSE;
	}
	return m_rowId == dotPosInfo.m_rowId;
}

BOOL CDotPosInfo::BeSameCol(CDotPosInfo& dotPosInfo)
{
	if(EDT_NONE == dotPosInfo.m_eDotPosType)
	{
		return FALSE;
	}
	if(-1 == m_colId)
	{
		return FALSE;
	}
	return m_colId == dotPosInfo.m_colId;
}

BOOL CDotPosInfo::BeValidCellPos()
{
	return EDT_BODY == m_eDotPosType && -1 != m_rowId && -1 != m_colId;
}

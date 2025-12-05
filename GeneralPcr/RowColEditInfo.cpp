#include "StdAfx.h"
#include "RowColEditInfo.h"


CRowColEditInfo::CRowColEditInfo(void)
{
	m_rowId = -1;
	m_colId = -1;
	m_strText = _T("");
	m_strNewText = _T("");
}

CRowColEditInfo::~CRowColEditInfo(void)
{

}

CRowColEditInfo::CRowColEditInfo(const CRowColEditInfo& rowColEditInfo)
{
	m_rowId = rowColEditInfo.m_rowId;
	m_colId = rowColEditInfo.m_colId;
	m_strText = rowColEditInfo.m_strText;
	m_strNewText = rowColEditInfo.m_strNewText;
}

void CRowColEditInfo::operator=(const CRowColEditInfo& rowColEditInfo)
{
	m_rowId = rowColEditInfo.m_rowId;
	m_colId = rowColEditInfo.m_colId;
	m_strText = rowColEditInfo.m_strText;
	m_strNewText = rowColEditInfo.m_strNewText;
}

BOOL CRowColEditInfo::operator==(const CRowColEditInfo& rowColEditInfo)
{
	return m_rowId == rowColEditInfo.m_rowId && 
	m_colId == rowColEditInfo.m_colId;
}

BOOL CRowColEditInfo::BeValidRowCol()
{
	if(m_rowId < 0)
	{
		return FALSE;
	}
	if(m_colId < 0)
	{
		return FALSE;
	}

	return TRUE;
}

void CRowColEditInfo::Clear()
{
	m_rowId = -1;
	m_colId = -1;
}



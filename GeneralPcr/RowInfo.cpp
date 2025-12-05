#include "StdAfx.h"
#include "RowInfo.h"


CRowInfo::CRowInfo(void)
{
	m_rowH = 28;
	m_bSelected = FALSE;
	m_bDisabled = FALSE;
}


CRowInfo::~CRowInfo(void)
{
}

CRowInfo::CRowInfo(const CRowInfo& info)
{
	m_rowH = info.m_rowH;
	m_bSelected = info.m_bSelected;
	m_bDisabled = info.m_bDisabled;
}

void CRowInfo::operator=(const CRowInfo& info)
{
	m_rowH = info.m_rowH;
	m_bSelected = info.m_bSelected;
	m_bDisabled = info.m_bDisabled;
}


#include "StdAfx.h"
#include "TubeChanEditInfo.h"


CTubeChanEditInfo::CTubeChanEditInfo(void)
{
	m_iRowId = -1;
	m_iColId = -1;
	m_bChecked = FALSE;
	m_strData = _T("");
	m_clrData = 255;
	m_dData = 0.0;
	m_iDpd = 4;
}

CTubeChanEditInfo::~CTubeChanEditInfo(void)
{
}

CTubeChanEditInfo::CTubeChanEditInfo(const CTubeChanEditInfo& info)
{
	m_iRowId = info.m_iRowId;
	m_iColId = info.m_iColId;
	m_bChecked = info.m_bChecked;
	m_dData = info.m_dData;
	m_strData = info.m_strData;
	m_clrData = info.m_clrData;
	m_iDpd = info.m_iDpd;
	return;
}

void CTubeChanEditInfo::operator=(const CTubeChanEditInfo& info)
{
	m_iRowId = info.m_iRowId;
	m_iColId = info.m_iColId;
	m_dData = info.m_dData;
	m_bChecked = info.m_bChecked;
	m_strData = info.m_strData;
	m_clrData = info.m_clrData;
	m_iDpd = info.m_iDpd;
	return;
}

BOOL CTubeChanEditInfo::GetBeAllTubeSameSet()
{
	//return (2 == m_iColId || 4 == m_iColId);
	return 2 == m_iColId;
}

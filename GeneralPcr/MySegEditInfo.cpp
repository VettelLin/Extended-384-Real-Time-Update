#include "StdAfx.h"
#include "MySegEditInfo.h"


CMySegEditInfo::CMySegEditInfo(void)
{
	m_strSegName = _T("");
	m_cycleNum = 0;
	m_bMeltSeg = FALSE;
}

CMySegEditInfo::~CMySegEditInfo(void)
{

}

CMySegEditInfo::CMySegEditInfo(const CMySegEditInfo& segEditInfo)
{
	m_strSegName = segEditInfo.m_strSegName;
	m_cycleNum = segEditInfo.m_cycleNum;
	m_bMeltSeg = segEditInfo.m_bMeltSeg;
}

void CMySegEditInfo::operator=(const CMySegEditInfo& segEditInfo)
{
	m_strSegName = segEditInfo.m_strSegName;
	m_cycleNum = segEditInfo.m_cycleNum;
	m_bMeltSeg = segEditInfo.m_bMeltSeg;
}




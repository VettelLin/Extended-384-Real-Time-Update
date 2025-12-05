#include "StdAfx.h"
#include "SingleLgdInfo.h"


CSingleLgdInfo::CSingleLgdInfo(void)
{
	m_clrFrame = Color(0XFF,0XFF,0XFF);
	m_clrFill = 0;
	m_pxFrameW = 1;
	m_strTx = _T("");
}

CSingleLgdInfo::~CSingleLgdInfo(void)
{
}

CSingleLgdInfo::CSingleLgdInfo(const CSingleLgdInfo& info)
{
	m_clrFrame = info.m_clrFrame;
	m_clrFill = info.m_clrFill;
	m_pxFrameW = info.m_pxFrameW;
	m_strTx = info.m_strTx;
}

void CSingleLgdInfo::operator=(const CSingleLgdInfo& info)
{
	m_clrFrame = info.m_clrFrame;
	m_clrFill = info.m_clrFill;
	m_pxFrameW = info.m_pxFrameW;
	m_strTx = info.m_strTx;
}


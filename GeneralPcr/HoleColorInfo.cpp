#include "StdAfx.h"
#include "HoleColorInfo.h"


CHoleColorInfo::CHoleColorInfo(void)
{
	m_clrFrame = Color(0X0,0X0,0X0);
	m_clrFill = Color(255 / 5,0X0,0X0,0X0);
	m_clrTxUnSelected = RGB(0,0,0);
	m_clrTxSelected = RGB(0,0,0);
}

CHoleColorInfo::~CHoleColorInfo(void)
{
	
}

CHoleColorInfo::CHoleColorInfo(const CHoleColorInfo& info)
{
	m_clrFrame = info.m_clrFrame;
	m_clrFill = info.m_clrFill;	
	m_clrTxUnSelected = info.m_clrTxUnSelected;
	m_clrTxSelected = info.m_clrTxSelected;
}

void CHoleColorInfo::operator=(const CHoleColorInfo& info)
{
	m_clrFrame = info.m_clrFrame;
	m_clrFill = info.m_clrFill;
	m_clrTxUnSelected = info.m_clrTxUnSelected;
	m_clrTxSelected = info.m_clrTxSelected;
}



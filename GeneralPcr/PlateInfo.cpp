#include "StdAfx.h"
#include "PlateInfo.h"


CPlateInfo::CPlateInfo(void)
{
	m_bSelected = FALSE;
}


CPlateInfo::~CPlateInfo(void)
{
	
}

CPlateInfo::CPlateInfo(const CPlateInfo& info)
{
	m_bSelected = info.m_bSelected;
}

void CPlateInfo::operator=(const CPlateInfo& info)
{
	m_bSelected = info.m_bSelected;
}

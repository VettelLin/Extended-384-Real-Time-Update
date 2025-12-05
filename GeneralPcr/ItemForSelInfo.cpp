#include "StdAfx.h"
#include "ItemForSelInfo.h"


CItemForSelInfo::CItemForSelInfo(void)
{
	m_strText = _T("");

	m_clrNormal = RGB(255,255,255);
	m_clrHover = RGB(255,255,255);
	m_clrSelected = RGB(229,242,255);
	m_clrDisabled = RGB(246,246,246);

	m_iItemH = 28;
	m_eSelState = ESS_NORMAL;
}

CItemForSelInfo::~CItemForSelInfo(void)
{

}

CItemForSelInfo::CItemForSelInfo(const CItemForSelInfo& info)
{
	m_strText = info.m_strText;
	m_clrNormal = info.m_clrNormal;
	m_clrHover = info.m_clrHover;
	m_clrSelected = info.m_clrSelected;
	m_clrDisabled = info.m_clrDisabled;
	m_iItemH = info.m_iItemH;
	m_eSelState = info.m_eSelState;
}

void CItemForSelInfo::operator=(const CItemForSelInfo& info)
{
	m_strText = info.m_strText;
	m_clrNormal = info.m_clrNormal;
	m_clrHover = info.m_clrHover;
	m_clrSelected = info.m_clrSelected;
	m_clrDisabled = info.m_clrDisabled;
	m_iItemH = info.m_iItemH;
	m_eSelState = info.m_eSelState;
}

BOOL CItemForSelInfo::GetBeDisabled()
{
	return ESS_DISABLE == m_eSelState;
}

BOOL CItemForSelInfo::GetBeSelected()
{
	return ESS_SELECTED == m_eSelState;
}

int CItemForSelInfo::SetSelectState(ESELECTSTATE eSelState)
{
	m_eSelState = eSelState;
	return 0;
}

ESELECTSTATE CItemForSelInfo::GetSelectState()
{
	return m_eSelState;
}

int CItemForSelInfo::MouseOut()
{
	if(ESS_DISABLE == m_eSelState)
	{
		return 1;
	}

	if(ESS_SELECTED == m_eSelState)
	{
		return 2 ;
	}

	if(ESS_HOVER == m_eSelState)
	{
		m_eSelState = ESS_NORMAL;
		return 3;
	}
	
	if(ESS_NORMAL == m_eSelState)
	{
		return 4;
	}

	return 0;
}

int CItemForSelInfo::MouseIn()
{
	if(ESS_DISABLE == m_eSelState)
	{
		return 1;
	}

	if(ESS_SELECTED == m_eSelState)
	{
		return 2 ;
	}

	if(ESS_HOVER == m_eSelState)
	{
		return 3;
	}

	if(ESS_NORMAL == m_eSelState)
	{
		m_eSelState = ESS_HOVER;
		return 4;
	}

	return 0;
}

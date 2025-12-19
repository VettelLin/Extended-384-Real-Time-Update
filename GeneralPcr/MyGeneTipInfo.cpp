#include "StdAfx.h"
#include "MyGeneTipInfo.h"


CMyGeneTipInfo::CMyGeneTipInfo(void)
{
	m_pParent = NULL;
	m_strTipInfo = _T("");
	m_eGeneTipType = EGTT_NORMAL;
}

CMyGeneTipInfo::~CMyGeneTipInfo(void)
{

}

CMyGeneTipInfo::CMyGeneTipInfo(const CMyGeneTipInfo& geneTipInfo)
{
	m_pParent = geneTipInfo.m_pParent;
	m_strTipInfo = geneTipInfo.m_strTipInfo;
	m_eGeneTipType = geneTipInfo.m_eGeneTipType;
}

void CMyGeneTipInfo::operator=(const CMyGeneTipInfo& geneTipInfo)
{
	m_pParent = geneTipInfo.m_pParent;
	m_strTipInfo = geneTipInfo.m_strTipInfo;
	m_eGeneTipType = geneTipInfo.m_eGeneTipType;
}




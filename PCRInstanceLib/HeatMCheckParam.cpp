#include "StdAfx.h"
#include "HeatMCheckParam.h"


CHeatMCheckParam::CHeatMCheckParam(void)
{
	m_strChkRst = _T("");
}

CHeatMCheckParam::~CHeatMCheckParam(void)
{

}

CHeatMCheckParam::CHeatMCheckParam(const CHeatMCheckParam& info)
{
	m_peltier1ChkParam = info.m_peltier1ChkParam;
	m_peltier2ChkParam = info.m_peltier2ChkParam;
	m_peltier3ChkParam = info.m_peltier3ChkParam;
	m_strChkRst = info.m_strChkRst;
}

void CHeatMCheckParam::operator=(const CHeatMCheckParam& info)
{
	m_peltier1ChkParam = info.m_peltier1ChkParam;
	m_peltier2ChkParam = info.m_peltier2ChkParam;
	m_peltier3ChkParam = info.m_peltier3ChkParam;
	m_strChkRst = info.m_strChkRst;
}

CPeltierCheckParam* CHeatMCheckParam::GetPeltierCheckParam(int iPeltierId)
{
	if(iPeltierId < 0 || iPeltierId > 2)
	{
		return NULL;
	}
	
	if(0 == iPeltierId)
	{
		return &m_peltier1ChkParam;
	}
	else if(1 == iPeltierId)
	{
		return &m_peltier2ChkParam;
	}
	else if(2 == iPeltierId)
	{
		return &m_peltier3ChkParam;
	}
}

BOOL CHeatMCheckParam::IsNeedCheck()
{
	return m_peltier1ChkParam.IsCheck() || m_peltier2ChkParam.IsCheck() || m_peltier3ChkParam.IsCheck();
}

void CHeatMCheckParam::ResetCheckResult()
{
	m_strChkRst.Empty();
}

void CHeatMCheckParam::AddCheckResult(CString strData)
{
	m_strChkRst += _T("\r\n");
	m_strChkRst += strData;
	return;
}

CString CHeatMCheckParam::GetCheckResult()
{
	return m_strChkRst;
}

BOOL CHeatMCheckParam::IsSuccessful()
{
	return m_strChkRst.IsEmpty();
}





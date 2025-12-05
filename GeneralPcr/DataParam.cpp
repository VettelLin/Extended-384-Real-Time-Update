#include "StdAfx.h"
#include "DataParam.h"


CDataParam::CDataParam(void)
{
	m_eDataType = EDT2_NONE;
	m_iDpd = 0;
	m_bUseDataFilter = FALSE;
}

CDataParam::CDataParam(EDATATYPE eDtType,int iDpd,BOOL bData)
{
	m_eDataType = eDtType;
	m_iDpd = iDpd;
	m_bUseDataFilter = bData;
}

CDataParam::~CDataParam(void)
{

}

CDataParam::CDataParam(const CDataParam& info)
{
	m_eDataType = info.m_eDataType;
	m_iDpd = info.m_iDpd;
	m_bUseDataFilter = info.m_bUseDataFilter;
}

void CDataParam::operator=(const CDataParam& info)
{
	m_eDataType = info.m_eDataType;
	m_iDpd = info.m_iDpd;
	m_bUseDataFilter = info.m_bUseDataFilter;
}

int CDataParam::DataFilter(CString& strData)
{
	if(!m_bUseDataFilter)
	{
		return 1;
	}

	if(EDT2_DOUBLE != m_eDataType)
	{
		return -1;
	}

	int iDPd = 0;
	int iDotIndex = strData.Find(_T('.'));
	if(-1 != iDotIndex)
	{
		iDPd = strData.GetLength() - iDotIndex - 1;
	}

	if(iDPd == m_iDpd)
	{
		return 2;
	}

	double dData = _ttof(strData);
	CString strFormat = _T("");
	strFormat.Format(_T("%%.%dlf"),m_iDpd);
	strData.Format(strFormat,dData);

	return 0;
}

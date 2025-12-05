#include "StdAfx.h"
#include "Ps96IvdSet.h"

CPs96IvdSet::CPs96IvdSet(void)
{
	m_eRstExportOrder = EREO_VERT;
	m_strHospitalName.Empty();
}

CPs96IvdSet::~CPs96IvdSet(void)
{

}

CPs96IvdSet::CPs96IvdSet(const CPs96IvdSet& info)
{
	m_eRstExportOrder = info.m_eRstExportOrder;
	m_strHospitalName = info.m_strHospitalName;

	return;
}

void CPs96IvdSet::operator=(const CPs96IvdSet& info)
{
	m_eRstExportOrder = info.m_eRstExportOrder;
	m_strHospitalName = info.m_strHospitalName;

	return;
}

BOOL CPs96IvdSet::operator==(const CPs96IvdSet& info)
{
	return m_eRstExportOrder == info.m_eRstExportOrder && 
	m_strHospitalName == info.m_strHospitalName;
}

BOOL CPs96IvdSet::operator!=(const CPs96IvdSet& info)
{
	return m_eRstExportOrder != info.m_eRstExportOrder || 
		   m_strHospitalName != info.m_strHospitalName;
}

BOOL CPs96IvdSet::IsExportResultHorizontal()
{
	return EREO_HORI == m_eRstExportOrder;
}

void CPs96IvdSet::SetResultExportOrder(ERSTEXPORTORDER eData)
{
	m_eRstExportOrder = eData;
	return;
}

void CPs96IvdSet::SetResultExportOrder(BOOL bHori)
{
	if(bHori)
	{
		m_eRstExportOrder = EREO_HORI;
	}
	else
	{
		m_eRstExportOrder = EREO_VERT;
	}
	return;
}

ERSTEXPORTORDER CPs96IvdSet::GetResultExportOrder()
{
	return m_eRstExportOrder;
}

void CPs96IvdSet::SetHospitalName(CString strData)
{
	m_strHospitalName = strData;
	return;
}

CString CPs96IvdSet::GetHospitalName()
{
	return m_strHospitalName;
}

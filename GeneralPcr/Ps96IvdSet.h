#pragma once

typedef enum RstExportOrder{
	EREO_HORI = 1,EREO_VERT
}ERSTEXPORTORDER;

class CPs96IvdSet
{
public:
	CPs96IvdSet(void);
	~CPs96IvdSet(void);

	CPs96IvdSet(const CPs96IvdSet& info);
	void operator=(const CPs96IvdSet& info);
	BOOL operator==(const CPs96IvdSet& info);
	BOOL operator!=(const CPs96IvdSet& info);

	BOOL IsExportResultHorizontal();
	
	void SetResultExportOrder(ERSTEXPORTORDER eData);
	void SetResultExportOrder(BOOL bHori);
	ERSTEXPORTORDER GetResultExportOrder();

	void SetHospitalName(CString strData);
	CString GetHospitalName();

public:
	ERSTEXPORTORDER m_eRstExportOrder;
	CString m_strHospitalName;
};


#pragma once

#include "PeltierCheckParam.h"

class AFX_EXT_CLASS CHeatMCheckParam
{
public:
	CHeatMCheckParam(void);
	~CHeatMCheckParam(void);

	CHeatMCheckParam(const CHeatMCheckParam& info);
	void operator=(const CHeatMCheckParam& info);

	virtual CPeltierCheckParam* GetPeltierCheckParam(int iPeltierId);
	virtual	BOOL IsNeedCheck();
			
			void ResetCheckResult();
			void AddCheckResult(CString strData);
			CString GetCheckResult();	
			BOOL IsSuccessful();

protected:
	CPeltierCheckParam m_peltier1ChkParam;
	CPeltierCheckParam m_peltier2ChkParam;
	CPeltierCheckParam m_peltier3ChkParam;

	CString m_strChkRst;
};


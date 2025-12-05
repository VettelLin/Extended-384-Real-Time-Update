#pragma once

#include "SelfDefType.h"

class CInterfaceC
{
public:
	CInterfaceC(void);
	~CInterfaceC(void);

	virtual void SetPlateSelectedSiteSamInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData = 0,double dData = 0) = 0;
};


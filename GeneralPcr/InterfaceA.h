#pragma once

#include "TubeChanEditInfo.h"
#include "SelfDefType.h"

class CInterfaceA
{
public:
	CInterfaceA(void);
	~CInterfaceA(void);

	virtual void SetPlateSelectedSiteSamInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/) = 0;
	virtual int SetPlateSelectedSiteSamInfo(CTubeChanEditInfo tubeChanEditInfo) = 0;
};


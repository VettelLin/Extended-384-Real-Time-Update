#pragma once

class CInterfaceB
{
public:
	CInterfaceB(void);
	~CInterfaceB(void);

	virtual void SetNewPrjBaseInfo(EITEMINFOTYPE eItemInfoType,CString strData,int intData /*= 0*/,double dData /*= 0*/) = 0;
};


#pragma once
#include "InstancePCR.h"

class AFX_EXT_CLASS CInstancePS96 : public CInstancePCR
{
public:
	CInstancePS96(CSerialPortPCR* pComPort, CString strVersion);
	~CInstancePS96(void);

// Operations
public:
	virtual BOOL Initialize();
	virtual int GetInitChanDyeName(vector<CString>& vecChanDyeName);	//新建实验时，初始化每个通道的染料名称标识
	virtual int CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume);	//估算实验总时间

	virtual int GetLidHeatNormalSecond();
	virtual int GetLidState(int& iStatus);
	virtual BOOL WriteLidPreheatTemperaturePLCUsed(float fLidTem);
	virtual BOOL ReadLidPreheatTemperaturePLCUsed(float& fLidPreheatTem);

protected:
	virtual void InitStructurePara();

	BOOL FluoSelfcheck();
};


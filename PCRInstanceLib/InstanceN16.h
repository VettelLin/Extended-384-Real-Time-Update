#pragma once

#include "InstancePCR.h"

class CInstanceN16: public CInstancePCR
{
public:
	CInstanceN16(CSerialPortPCR* pComPort, CString strVersion);
	~CInstanceN16(void);

// Operations
public:
	virtual BOOL Initialize();
	virtual int CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume);	//估算实验总时间

	virtual int GetLidHeatNormalSecond();

	virtual BOOL WriteLidPreheatTemperaturePLCUsed(float fLidTem);
	virtual BOOL ReadLidPreheatTemperaturePLCUsed(float& fLidPreheatTem);

protected:
	virtual void InitDevicePara();
	virtual void InitStructurePara();

	virtual BOOL ScanOnce(BOOL *pBeAskTop);	// 完成一次扫描全部扫描动作，支持快速停止扫描
	virtual BOOL ReadAllFLUData(CUIntArray* pAryData,int iChanNum);
};


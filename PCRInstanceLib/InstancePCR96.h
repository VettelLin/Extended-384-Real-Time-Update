#pragma once

#include "InstancePCR.h"

class CInstancePCR96 :	public CInstancePCR
{
public:
	CInstancePCR96(CSerialPortPCR* pComPort, CString strVersion);
	~CInstancePCR96(void);

// Operations
public:
	virtual BOOL ReadDeviceParamsFromEPROM();
	virtual BOOL ReadStructureParamsFromEPROM();

	virtual BOOL Initialize();

	virtual int CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume);

	virtual BOOL ReadLidTemperatureFromEEPROMPLCUsed();
	virtual BOOL ReadLidTemperatureFromEEPROMPLCUsed(double& dLidTem);
	virtual BOOL WriteLidTemperaturePLCUsed(float fLidTem);

	virtual int GetLidHeatNormalSecond();

protected:
	virtual void InitDevicePara();
	virtual void InitStructurePara();

	virtual BOOL ReadAllFLUData(CUIntArray* pAryData,int iChanNum);
};


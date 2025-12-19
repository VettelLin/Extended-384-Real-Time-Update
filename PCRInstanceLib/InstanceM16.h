#pragma once

#include "InstancePCR.h"

//0XD60，热盖1默认目标温度，短整型
//0XD64，热盖2默认目标温度，短整型

class CInstanceM16: public CInstancePCR
{
public:
	CInstanceM16(CSerialPortPCR* pComPort, CString strVersion);
	~CInstanceM16(void);

// Operations
public:
	virtual BOOL Initialize();

	virtual BOOL ReadLidTemperatureFromEEPROMPLCUsed();	
	virtual BOOL ReadLidTemperatureFromEEPROMPLCUsed(double& dLidTem);
	virtual BOOL WriteLidTemperaturePLCUsed(float fLidTem);
	virtual BOOL WriteLidPreheatTemperaturePLCUsed(float fLidTem);
	virtual BOOL ReadLidPreheatTemperaturePLCUsed(float& fLidPreheatTem);

	virtual int CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume);	//估算实验总时间

	virtual int GetLidHeatNormalSecond();

protected:
	virtual void InitDevicePara();
	virtual void InitStructurePara();

	virtual BOOL ScanOnce(BOOL *pBeAskTop);	// 完成一次扫描全部扫描动作，支持快速停止扫描
	virtual BOOL ReadAllFLUData(CUIntArray* pAryData,int iChanNum);
};


#include "StdAfx.h"
#include "InstancePCR96.h"
#include "Define.h"

CInstancePCR96::CInstancePCR96(CSerialPortPCR* pComPort, CString strVersion) : CInstancePCR(pComPort, strVersion)
{
}

CInstancePCR96::~CInstancePCR96(void)
{

}

BOOL CInstancePCR96::ReadDeviceParamsFromEPROM()
{
	return CInstancePCR::ReadDeviceParamsFromEPROM();
}

BOOL CInstancePCR96::ReadStructureParamsFromEPROM()
{
	if(!CInstancePCR::ReadStructureParamsFromEPROM())
	{
		return FALSE;
	}

	char recv[8] = {0};	
	USHORT* pUshort = (USHORT*)recv;
	float* pFloat = (float*)recv;

	//读取MPPC增益值
	BOOL bReturn = ReadEPROM(ADDR_MPPCGAIN_HIGH,2,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_iMppcGainH = *pUshort;

	bReturn = ReadEPROM(ADDR_MPPCGAIN_MIDDLE,2,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_iMppcGainM = *pUshort;

	bReturn = ReadEPROM(ADDR_MPPCGAIN_LOW,2,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_iMppcGainL = *pUshort;

	//读取PWM K/B值
	bReturn = ReadEPROM(PWMK1,4,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_fPwmK1 = *pFloat;

	bReturn = ReadEPROM(PWMB1,4,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_fPwmB1 = *pFloat;
	 
	bReturn = ReadEPROM(PWMK2,4,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_fPwmK2 = *pFloat;

	bReturn = ReadEPROM(PWMB2,4,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_fPwmB2 = *pFloat;

	bReturn = ReadEPROM(PWMK3,4,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_fPwmK3 = *pFloat;

	bReturn = ReadEPROM(PWMB3,4,(BYTE*)recv);
	if(!bReturn) 
	{
		return FALSE;
	}
	m_fPwmB3 = *pFloat;

	return TRUE;
}

BOOL CInstancePCR96::Initialize()
{
	InitDevicePara();
	InitDeviceStatus();
	InitStructurePara();

	BOOL bReturn = Hello();
	if (!bReturn)
	{
		return bReturn;
	}

	//读取设备参数
	bReturn = ReadDeviceParamsFromEPROM();

	//N16,96QPCR使用
	char* pChannel1 = "T505,ET470/10,ET525/10";
	memcpy(m_sDevicePara.cKeyFilter1, pChannel1, strlen(pChannel1));
	pChannel1 = "T650,ET620/16,ET680/20";
	memcpy(m_sDevicePara.cKeyFilter2, pChannel1, strlen(pChannel1));
	pChannel1 = "T550,ET515/20,ET575/20";
	memcpy(m_sDevicePara.cKeyFilter3, pChannel1, strlen(pChannel1));
	pChannel1 = "T585,ET560/20,ET620/20";	
	memcpy(m_sDevicePara.cKeyFilter4, pChannel1, strlen(pChannel1));

	char* pTemp = "11110000";
	memcpy(m_sDevicePara.cFilterConfig, pTemp,8);

	if(bReturn)
	{
		bReturn = ReadStructureParamsFromEPROM();
	}

	//m_sStructurePara.cLidTempTop = 100;	//N96默认热盖温度为100

	InitFilterPara();

	return bReturn;
}

int CInstancePCR96::CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume)
{
	//使用临时的 整板扫描时间,96孔QPCR默认扫描时间是10.5秒
	double dScanTimeSec = QPCR_SCANSEC;

	int iExpTotalSec = 0;

	//计算热盖升温时间
	if(bUseLid)
	{
		iExpTotalSec += QPCR_LIDHEATSEC;
	}

	//PS96只有升温，升温速率默认为0.4℃/S，96孔QPCR升降温速率使用2.5℃/S，N16使用5.5℃/S
	float fUpRate = 2.5;
	float fDownRate = 2.5;
	float fPreTem = 0,fPreTem2 = 0,fTemMinus = 0,fTemMinus2 = 0,fExistSec = 0,fExistSec2 = 0;

	Node<CSegItemInfo>* pSegNode =lnkSegItem.GetHead();
	Node<CStepItemInfo>* pStepNode = NULL;
	int iSegIdnex = 0,iStepIndex = 0;

	while(NULL != pSegNode)
	{
		CSegItemInfo& segItem = pSegNode->data;
		pStepNode = pSegNode->data.m_lnkStepItemInfo.GetHead();
		iStepIndex = 0;

		while(NULL != pStepNode)
		{
			CStepItemInfo& stepItem = pStepNode->data;

			if(CHECK_ALL_POINT == stepItem.m_eChkType)
			{
				if(0 == iStepIndex)
				{
					if(0 == iSegIdnex)
					{
						fPreTem = 50;
					}
					else
					{
						fPreTem = pSegNode->pPrevious->data.m_lnkStepItemInfo.GetTail()->data.m_fAimTem;
					}
				}
				else
				{
					fPreTem = pStepNode->pPrevious->data.m_fAimTem;
				}

				fTemMinus = stepItem.m_fAimTem - fPreTem;
				if(fTemMinus > 0 && stepItem.m_fIncrement > 0)
				{
					fExistSec = (WORD)((stepItem.m_fAimTem - fPreTem) / stepItem.m_fIncrement * (6 + dScanTimeSec) + 0.5);
				}

				float ctrlSec = 0;
				ctrlSec = fExistSec + m_iMsPwmInitWait / 1000.0 * 2 + 10;

				iExpTotalSec += ctrlSec; 
				iExpTotalSec += stepItem.m_iDelay;
				iExpTotalSec += (6 + dScanTimeSec) * 7;	//起始温度点多余扫描过程
			}
			else
			{
				if(0 == iStepIndex)
				{
					if(0 == iSegIdnex)
					{
						fPreTem = 36;
					}
					else
					{
						fPreTem = pSegNode->pPrevious->data.m_lnkStepItemInfo.GetTail()->data.m_fAimTem;
					}
					fPreTem2 = segItem.m_lnkStepItemInfo.GetTail()->data.m_fAimTem;

					fTemMinus = stepItem.m_fAimTem - fPreTem;
					fTemMinus2 = stepItem.m_fAimTem - fPreTem2;
					if(fTemMinus > 0)
					{
						fExistSec = fTemMinus / fUpRate;
					}
					else
					{
						fExistSec = -fTemMinus / fDownRate;
					}
					if(fTemMinus2 > 0)
					{
						fExistSec2 = fTemMinus2 / fUpRate;
					}
					else
					{
						fExistSec2 = -fTemMinus2 / fDownRate;
					}
					iExpTotalSec += (fExistSec + (segItem.m_iCycleNum - 1) * fExistSec2 + stepItem.m_iDelay * segItem.m_iCycleNum);
				}
				else
				{
					fPreTem = pStepNode->pPrevious->data.m_fAimTem;
					fTemMinus = stepItem.m_fAimTem - fPreTem;
					if(fTemMinus > 0)
					{
						fExistSec = fTemMinus / fUpRate;
					}
					else
					{
						fExistSec = -fTemMinus / fDownRate;
					}
					iExpTotalSec += ((fExistSec + stepItem.m_iDelay) * segItem.m_iCycleNum);
				}

				if(CHECK_END_POINT == stepItem.m_eChkType)
				{
					iExpTotalSec += dScanTimeSec * segItem.m_iCycleNum;
				}

				//非熔曲步骤实际到温需要5秒左右,添加于20221221
				iExpTotalSec += 5 * segItem.m_iCycleNum;
			}

			++iStepIndex;
			pStepNode = pStepNode->pNext;
		}

		++iSegIdnex;
		pSegNode = pSegNode->pNext;
	}

	//计算结尾仪器温度大于40℃自动降温时间
	pSegNode = lnkSegItem.GetTail(); 
	if(NULL != pSegNode)
	{
		 pStepNode = pSegNode->data.m_lnkStepItemInfo.GetTail();
		 if(NULL != pStepNode && pStepNode->data.m_fAimTem > m_fEndCoolTem)
		 {
			 iExpTotalSec += (pStepNode->data.m_fAimTem - m_fEndCoolTem) / fDownRate;
			 iExpTotalSec += m_iMsSlpAfterCool / 1000.0;
		 }
	}

	iExpTotalSec += 20;	//20200609，平衡总时间

	return iExpTotalSec;
}

BOOL CInstancePCR96::ReadLidTemperatureFromEEPROMPLCUsed()
{
	BYTE* pReceive = new BYTE[2];
	if(NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive,0,sizeof(BYTE) * 2);
	if(ReadEPROM(0X2027,1,pReceive))
	{
		if(pReceive[0] > LIDTEMMAX || pReceive[0] < LIDTEMMIN)
		{
			if(!SetLidTem(N96DEFAULT_LIDTEM))
			{
				delete [] pReceive;
				return FALSE;
			}
		}
		else
		{
			m_sStructurePara.cLidTempTop = pReceive[0];
		}
	}
	else
	{
		delete [] pReceive;
		return FALSE;
	}

	delete [] pReceive;

	return TRUE;
}

BOOL CInstancePCR96::ReadLidTemperatureFromEEPROMPLCUsed(double& dLidTem)
{
	BYTE* pReceive = new BYTE[2];
	if(NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive,0,sizeof(BYTE) * 2);
	if(ReadEPROM(0X2027,1,pReceive))
	{
		if(pReceive[0] > LIDTEMMAX || pReceive[0] < LIDTEMMIN)
		{
			if(!SetLidTem(N96DEFAULT_LIDTEM))
			{
				delete [] pReceive;
				return FALSE;
			}
		}
		else
		{
			m_sStructurePara.cLidTempTop = pReceive[0];
		}
	}
	else
	{
		delete [] pReceive;
		return FALSE;
	}

	dLidTem = m_sStructurePara.cLidTempTop;

	delete [] pReceive;

	return TRUE;
}

BOOL CInstancePCR96::WriteLidTemperaturePLCUsed(float fLidTem)
{
	int iLidTem = fLidTem;

	if(iLidTem <= 0)
	{
		return FALSE;
	}

	if(!SetLidTem(iLidTem))
	{
		return FALSE;
	}

	return TRUE;
}

int CInstancePCR96::GetLidHeatNormalSecond()
{
	return QPCR_LIDHEATSEC;
}

void CInstancePCR96::InitDevicePara()
{ 
	CInstancePCR::InitDevicePara();

	char* pChannel1 = "T505,ET480/10,ET518/10";
	memcpy(m_sDevicePara.cKeyFilter1, pChannel1, strlen(pChannel1));
	pChannel1 = "T650,ET630/16,ET670/20";
	memcpy(m_sDevicePara.cKeyFilter2, pChannel1, strlen(pChannel1));
	pChannel1 = "T550,ET523/20,ET564/20";
	memcpy(m_sDevicePara.cKeyFilter3, pChannel1, strlen(pChannel1));
	pChannel1 = "T505,ET480/10,ET518/10";
	memcpy(m_sDevicePara.cKeyFilter4, pChannel1, strlen(pChannel1));
	pChannel1 = "T585,ET571/20,ET610/20";
	memcpy(m_sDevicePara.cKeyFilter5, pChannel1, strlen(pChannel1));

	pChannel1 = "11101000";
	memcpy(m_sDevicePara.cFilterConfig, pChannel1, strlen(pChannel1));
}

void CInstancePCR96::InitStructurePara()
{
	CInstancePCR::InitStructurePara();

	m_sStructurePara.cAutoCooling = 1;
	m_sStructurePara.cUseBox = 1;
	m_sStructurePara.cScanPathCount = 2;
}

//
//BOOL CInstancePCR96::ReadDeviceParamsFromEPROM()
//{
//	BOOL bReturn = FALSE;
//	UINT nEachLen = 249;
//	int  iReadNum = SIZE_DEVICE_PARAM / nEachLen;
//	UINT nRemain = SIZE_DEVICE_PARAM % nEachLen;
//	UINT nAddress = 0;
//	BYTE* pReceive = new BYTE[SIZE_DEVICE_PARAM];
//	memset(pReceive, 0, sizeof(BYTE)*SIZE_DEVICE_PARAM);
//
//	for (int i=0; i<iReadNum; i++)
//	{
//		bReturn = ReadEPROM(nAddress, nEachLen, &pReceive[i*nEachLen]);
//		if (!bReturn) 
//		{
//			delete [] pReceive;
//			pReceive = nullptr;
//			return bReturn;
//		}
//		nAddress += nEachLen;
//	}
//
//	if (nRemain > 0)
//	{
//		bReturn = ReadEPROM(nAddress, nRemain, &pReceive[nEachLen*iReadNum]);
//	}
//
//	if (bReturn)
//	{
//		tagDeviceParams sDeviceParams;
//		memcpy(&sDeviceParams, pReceive, SIZE_DEVICE_PARAM);
//
//		// 对应赋值
//		m_sDevicePara.wXMotorBackPulse = sDeviceParams.wXOffSet;
//		m_sDevicePara.wXMotorMaxFreq = sDeviceParams.wXMaxFreq;
//		m_sDevicePara.wXMotorFreq0 = sDeviceParams.wXFreq0;
//		m_sDevicePara.wXMotorScanFreq = sDeviceParams.wXScanFreq;
//		m_sDevicePara.wXMotorHomeFreq = sDeviceParams.wXHFreq;
//		m_sDevicePara.wXMotorAccPulse = sDeviceParams.wXAccPls;
//		m_sDevicePara.wXMotorMaxPls = sDeviceParams.wXMaxPls;
//		m_sDevicePara.wXMotorScanPulse = sDeviceParams.wXScanPls;
//
//		m_sDevicePara.wYMotorBackPulse = sDeviceParams.wYOffSet;
//		m_sDevicePara.wYMotorMaxFreq = sDeviceParams.wYMaxFreq;
//		m_sDevicePara.wYMotorFreq0 = sDeviceParams.wYFreq0;
//		m_sDevicePara.wYMotorScanFreq = sDeviceParams.wYScanFreq;
//		m_sDevicePara.wYMotorHomeFreq = sDeviceParams.wYHFreq;
//		m_sDevicePara.wYMotorAccPulse = sDeviceParams.wYAccPls;
//		m_sDevicePara.wYMotorMaxPls = sDeviceParams.wYMaxPls;
//		m_sDevicePara.wYMotorScanPulse = sDeviceParams.wYScanPls;
//
//	}
//
//	delete [] pReceive;
//	pReceive = nullptr;
//	return bReturn;
//}
//
//BOOL CInstancePCR96::ReadStructureParamsFromEPROM()
//{
//
//	return TRUE;
//}

BOOL CInstancePCR96::ReadAllFLUData(CUIntArray* pAryData,int iChanNum)
{
	if(NULL == pAryData)
	{
		return FALSE;
	}
	if(iChanNum != GetScanChannelCount())
	{
		return FALSE;
	}

	BOOL bReturn = FALSE;

	int iRealChannel = 0;
	for (int ichannel = 1;ichannel <= iChanNum;++ichannel)
	{
		if(4 == ichannel)
		{
			iRealChannel = 5;
		}
		else
		{
			iRealChannel = ichannel;
		}

		bReturn = ReadFLUData(pAryData[ichannel - 1],iRealChannel);
		if(!bReturn)
		{
			break;
		}
	}

	return bReturn;
}



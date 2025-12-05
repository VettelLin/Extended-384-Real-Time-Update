#include "StdAfx.h"
#include "InstanceM16.h"
#include "ErrorCodeDefine.h"

CInstanceM16::CInstanceM16(CSerialPortPCR* pComPort, CString strVersion) : CInstancePCR(pComPort, strVersion)
{
}


CInstanceM16::~CInstanceM16(void)
{
}

BOOL CInstanceM16::Initialize()
{
	InitDevicePara();
	InitDeviceStatus();
	InitStructurePara();

	BOOL bReturn = Hello();
	if (!bReturn)
	{
		return bReturn;
	}

	// 读取设备参数
	bReturn = ReadDeviceParamsFromEPROM();

	//N16,M16,96QPCR使用
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

	//临时兼容新羿M16
	m_sDevicePara.wPMTGainCh1 = 3031;

	if(bReturn)
	{
		bReturn = ReadStructureParamsFromEPROM();
	}

	InitFilterPara();

	return bReturn;
}

BOOL CInstanceM16::ReadLidTemperatureFromEEPROMPLCUsed()
{
	int iDataLen = 2;

	BYTE* pReceive = new BYTE[iDataLen];
	if(NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive,0,sizeof(BYTE) * iDataLen);
	if(ReadEPROM(0XD60,iDataLen,pReceive))
	{
		SetLidTem(*((unsigned short*)pReceive));
	}
	else
	{
		delete [] pReceive;
		return FALSE;
	}

	if(!ReadEPROM(0XD64,iDataLen,pReceive))
	{
		delete [] pReceive;
		return FALSE;
	}
	
	delete [] pReceive;

	return TRUE;
}

BOOL CInstanceM16::ReadLidTemperatureFromEEPROMPLCUsed(double& dLidTem)
{
	int iDataLen = 2;

	BYTE* pReceive = new BYTE[iDataLen];
	if(NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive,0,sizeof(BYTE) * iDataLen);

	if(!ReadEPROM(0XD64,iDataLen,pReceive))
	{
		delete [] pReceive;
		return FALSE;
	}

	if(ReadEPROM(0XD60,iDataLen,pReceive))
	{
		//默认显示第一个热盖的温度
		SetLidTem(*((unsigned short*)pReceive));
	}
	else
	{
		delete [] pReceive;
		return FALSE;
	}
	
	dLidTem = *((unsigned short*)pReceive);

	delete [] pReceive;

	return TRUE;
}

BOOL CInstanceM16::WriteLidTemperaturePLCUsed(float fLidTem)
{
	int iLidTem = fLidTem;

	if(iLidTem <= 0)
	{
		return FALSE;
	}

	int iDataLen = 2;

	if(!WriteEPROM(0XD60,iLidTem,iDataLen))
	{
		return FALSE;
	}

	if(!WriteEPROM(0XD64,iLidTem,iDataLen))
	{
		return FALSE;
	}

	if(!SetLidTem(iLidTem))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CInstanceM16::WriteLidPreheatTemperaturePLCUsed(float fLidTem)
{
	int iLidTem = fLidTem;

	if(iLidTem <= 0)
	{
		return FALSE;
	}

	if(!WriteEPROM(0XD68,iLidTem,2))
	{
		return FALSE;
	}

	if(!WriteEPROM(0XD6C,iLidTem,2))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CInstanceM16::ReadLidPreheatTemperaturePLCUsed(float& fLidPreheatTem)
{
	BYTE* pReceive = new BYTE[2];
	if(NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive,0,sizeof(BYTE) * 2);

	if(!ReadEPROM(0XD68,2,pReceive))
	{
		delete [] pReceive;
		return FALSE;
	}

	fLidPreheatTem = *((USHORT*)pReceive);

	delete [] pReceive;

	return TRUE;
}

int CInstanceM16::CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume)
{
	//使用临时的 整板扫描时间,M16默认扫描时间是2.6秒
	double dScanTimeSec = XINYIM16_SCANSEC;

	int iExpTotalSec = 0;

	//计算热盖升温时间
	if(bUseLid)
	{
		iExpTotalSec += XINYIM16_LIDHEATSEC;
	}

	//PS96只有升温，升温速率默认为0.4℃/S，96孔QPCR升降温速率使用2.5℃/S，M16使用5.5℃/S
	float fUpRate = 5.5;
	float fDownRate = 5.5;
	float fPreTem = 0,fPreTem2 = 0,fTemMinus = 0,fTemMinus2 = 0,fExistSec = 0,fExistSec2 = 0;

	Node<CSegItemInfo>* pSegNode = lnkSegItem.GetHead();
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
					fExistSec = (WORD)((stepItem.m_fAimTem - fPreTem) / stepItem.m_fIncrement * dScanTimeSec + 0.5);
				}

				iExpTotalSec += fExistSec; 
				iExpTotalSec += stepItem.m_iDelay;
				iExpTotalSec += dScanTimeSec * 4;	//起始温度点多余扫描过程
				iExpTotalSec += 20;	//熔曲扫描前稳定时间
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

	iExpTotalSec += 10;	//20200609，平衡总时间

	return iExpTotalSec;
}

int CInstanceM16::GetLidHeatNormalSecond()
{
	return XINYIM16_LIDHEATSEC;
}

void CInstanceM16::InitDevicePara()
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

void CInstanceM16::InitStructurePara()
{
	CInstancePCR::InitStructurePara();

	m_sStructurePara.cPlateRowCount = 2;
	m_sStructurePara.cPlateColCount = 8;
	m_sStructurePara.cTempChannelCount = 7;
}

// 完成一次扫描全部扫描动作，支持快速停止扫描
BOOL CInstanceM16::ScanOnce(BOOL *pBeAskTop)
{
	BOOL bReturn = FALSE;
	BYTE cDirection = 1;
	int iXMotScanDistance = m_sDevicePara.wXMotorScanPulse;
	
	//判断用户是否点击了停止扫描
	if(NULL != pBeAskTop && *pBeAskTop)
	{
		return TRUE;
	}

	//发送采样一行指令
	bReturn = MotorScanRow(iXMotScanDistance,cDirection);
	if (!bReturn)
	{
		//执行容错,缺省2次
		int iFoleNum = 0;
		do 
		{
			bReturn = MotorReset(CInstancePCR::UNIT_MOTOR_X);
			if (!bReturn) 
				break;

			bReturn = MotorScanRow(iXMotScanDistance, cDirection);
			if (bReturn)
			{
				break;
			}
			else
			{
				iFoleNum++;
			}
		} while (iFoleNum < 2);
	}

	if(bReturn)
	{
		bReturn = MotorReset(CInstancePCR::UNIT_MOTOR_X);
	}

	return  bReturn;
}

BOOL CInstanceM16::ReadAllFLUData(CUIntArray* pAryData,int iChanNum)
{
	if(NULL == pAryData)
	{
		return FALSE;
	}
	if(iChanNum != GetScanChannelCount())
	{
		return FALSE;
	}

	CUIntArray aryTemp;

	BOOL bReturn = FALSE;
	bReturn = ReadFLUData(aryTemp,1);
	if(!bReturn)
	{
		return FALSE;
	}

	int iDataNum = aryTemp.GetSize(); 
	if(iDataNum < 1)
	{
		return FALSE;
	}
	if(0 != (iDataNum % iChanNum))
	{
		return FALSE;
	}

	int iOneChanDataNum = iDataNum / iChanNum;
	for (int ichannel = 1;ichannel <= iChanNum;++ichannel)
	{
		for(int i = 0;i < iOneChanDataNum;++i)
		{
			pAryData[ichannel - 1].Add(aryTemp.GetAt(i + iOneChanDataNum * (ichannel - 1)));
		}
	}

	return TRUE;
}





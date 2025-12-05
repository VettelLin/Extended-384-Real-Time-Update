#include "StdAfx.h"
#include "InstancePS96.h"
#include "ErrorCodeDefine.h"

CInstancePS96::CInstancePS96(CSerialPortPCR* pComPort, CString strVersion) : CInstancePCR(pComPort, strVersion)
{
	
}

CInstancePS96::~CInstancePS96(void)
{

}

// 初始化仪器结构参数
void CInstancePS96::InitStructurePara()
{
	CInstancePCR::InitStructurePara();

	m_sStructurePara.cTempMode = TEMP_MODE_CONSTANT; // 恒温
	m_sStructurePara.cScanOrigin = 1; // 从右上开始扫描
	m_sStructurePara.cSupportMelt = 0; // 不支持熔曲
	m_sStructurePara.wScanOnceTime = 1200;
	m_sStructurePara.cCalibratePara = 1; // 使用浊度校准

	m_sStructurePara.cTempCtrlMaxTime = 120;	//PS96升温20℃（非热盖）用时大约75秒
	m_sStructurePara.cTempLegalGap = 20;	//PS96下位机返回的到温范围为正负1℃，这里默认值设置为正负2℃

	m_sStructurePara.cLidTempTop = 90;
}
  
BOOL CInstancePS96::Initialize()
{
	BOOL bReturn = CInstancePCR::Initialize();
	if (bReturn)
	{
		int iHigherCount = 0;
		int iLowerCount = 0;
		BOOL bReachTemp = FALSE;
		int iChannelCount = 6;
		float* pChTemp = new float[iChannelCount];
		if (ReadAllTemp(iChannelCount, pChTemp, bReachTemp))
		{
			for (int i=0; i<iChannelCount; i++)
			{
				if (pChTemp[i] > 85)
					iHigherCount ++;
				else if (pChTemp[i] < 5)
					iLowerCount ++;
			}
		}

		if (iHigherCount >= 2)
			m_eInstanceStatus = INSTANCE_TEMP_HIGH;
		else if(iLowerCount >= 2)
			m_eInstanceStatus = INSTANCE_TEMP_LOW;

		delete pChTemp;

		bReturn = FluoSelfcheck();
	}

	return bReturn;
}

int CInstancePS96::GetInitChanDyeName(vector<CString>& vecChanDyeName)
{
	vecChanDyeName.clear();
	int iChanCount = GetScanChannelCount();
	if(iChanCount < 1)
	{
		return -1;
	}
	
	for(int i = 0;i < iChanCount;++i)
	{
		if(0 == i)
		{
			vecChanDyeName.push_back(_T("FAM"));
		}
		else if(1 == i)
		{
			vecChanDyeName.push_back(_T("Cy5")); 
		}
		else if(2 == i)
		{
			vecChanDyeName.push_back(_T("VIC"));
		}
		else if(3 == i)
		{
			vecChanDyeName.push_back(_T("Cy5.5"));
		}
		else if(4 == i)
		{
			vecChanDyeName.push_back(_T("ROX")); 
		}
		else if(5 == i)
		{
			vecChanDyeName.push_back(_T("MOT")); 
		}
		else
		{
			CString strTemp(_T(""));
			strTemp.Format(_T("%d"),i + 1);
			vecChanDyeName.push_back(strTemp);
		}
	}

	return 0;
}

int CInstancePS96::CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume)
{
	//使用临时的 整板扫描时间,96孔QPCR默认扫描时间是10.5秒
	double dScanTimeSec = PS96_SCANSEC;
	int iExpTotalSec = 0;

	//计算热盖升温时间
	if(bUseLid)
	{
		iExpTotalSec += PS96_LIDHEATSEC;
	}

	//PS96只有升温，升温速率默认为0.4℃/S，96孔QPCR升降温速率使用2.5℃/S，N16使用5.5℃/S
	float iUpRate = 0.4f;
	float iDownRate = 0.4f;
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
						fPreTem = 36;
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
				if(fTemMinus > 0)
				{
					fExistSec = (WORD)((stepItem.m_fAimTem - fPreTem) / stepItem.m_fIncrement * (6 + dScanTimeSec) + 0.5);
				}

				float fRate = stepItem.m_fRate / 0.1f + 0.05f;
				float ctrlSec = fExistSec * 1.3f + stepItem.m_uiWaitTimeBelowStartTem * 2;

				iExpTotalSec += ctrlSec; 
				iExpTotalSec += stepItem.m_iDelay;
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
						fExistSec = fTemMinus / iUpRate;
					}
					else
					{
						fExistSec = -fTemMinus / iDownRate;
					}
					if(fTemMinus2 > 0)
					{
						fExistSec2 = fTemMinus2 / iUpRate;
					}
					else
					{
						fExistSec2 = -fTemMinus2 / iDownRate;
					}
					iExpTotalSec += (fExistSec + (segItem.m_iCycleNum - 1) * fExistSec2 + stepItem.m_iDelay * segItem.m_iCycleNum);
				}
				else
				{
					fPreTem = pStepNode->pPrevious->data.m_fAimTem;
					fTemMinus = stepItem.m_fAimTem - fPreTem;
					if(fTemMinus > 0)
					{
						fExistSec = fTemMinus / iUpRate;
					}
					else
					{
						fExistSec = -fTemMinus / iDownRate;
					}
					iExpTotalSec += ((fExistSec + stepItem.m_iDelay) * segItem.m_iCycleNum);
				}
			}

			++iStepIndex;
			pStepNode = pStepNode->pNext;
		}

		++iSegIdnex;
		pSegNode = pSegNode->pNext;
	}

	iExpTotalSec += 20;	//20200609，ps96计算出的总时间偏低

	return iExpTotalSec;
}

int CInstancePS96::GetLidHeatNormalSecond()
{
	return PS96_LIDHEATSEC;
}

int CInstancePS96::GetLidState(int& iStatus)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN6,PCR_LID_STATUS,0X0D);
	int iReceiveFrameLen = 0,iRet = 0;
	BYTE* pStatus = NULL;

	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand,CMD_SEND_LEN6,&pStatus,iReceiveFrameLen);
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		iStatus = pStatus[4];
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_LED_SET_FAIL;
			iRet = -1;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_LED_SET_TIMEOUT;
			iRet = -2;
		}
		else
		{
			iRet = -3;
		}
		m_iErrorPara1 = UINT_STATE_LED;
	}

	delete [] pCommand;
	delete [] pStatus;

	return iRet;
}


BOOL CInstancePS96::WriteLidPreheatTemperaturePLCUsed(float fLidTem)
{
	int iLidTem = fLidTem;

	if(iLidTem <= 0)
	{
		return FALSE;
	}

	if(!WriteEPROM(0X540,iLidTem,2))
	{
		return FALSE;
	}

	if(!WriteEPROM(0X544,iLidTem,2))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CInstancePS96::ReadLidPreheatTemperaturePLCUsed(float& fLidPreheatTem)
{
	BYTE* pReceive = new BYTE[2];
	if(NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive,0,sizeof(BYTE) * 2);

	if(!ReadEPROM(0X540,2,pReceive))
	{
		delete [] pReceive;
		return FALSE;
	}

	fLidPreheatTem = *((USHORT*)pReceive);

	delete [] pReceive;

	return TRUE;
}

// 荧光值自检
BOOL CInstancePS96::FluoSelfcheck()
{
	BOOL bReturn = FALSE;
	int iScanChannelCount = 5;
	UINT* pChannelFluo = new UINT[iScanChannelCount];

	BYTE* pReceive = new BYTE[10];
	memset(pReceive, 0, sizeof(BYTE)*10);
	for (int i=0; i<iScanChannelCount; i++)
	{
		UINT nAddress = 0X1300 + 2*16*i;
		bReturn = ReadEPROM(nAddress, 2, pReceive);
		if(bReturn)
		{
			pChannelFluo[i] = UINT(*pReceive);
		}
		else
		{
			break;
		}
	}

	delete pReceive;

	if (bReturn)
	{
		bReturn = SetPMTGain(GetPMTGain());
		if (!bReturn)
			goto END;

		bReturn = SetLaserStatus(2, 255);
		if (!bReturn)
			goto END;



		bReturn = ScanOnce();
		if (bReturn)
		{
			CUIntArray* pAryFlu = new CUIntArray[iScanChannelCount+1];

			bReturn = ReadAllFLUData(pAryFlu, iScanChannelCount+1);
			if(!bReturn)
			{
				delete [] pAryFlu;
				goto END;
			}

			int iMaxCount = 0;
			int iMinCount = 0;
			// 保存数据
			for(int i=0; i<iScanChannelCount; i++)
			{
				int iDataCount = pAryFlu[i].GetCount();
				iMaxCount = 0;
				iMinCount = 0;
				for (int j=0; j<iDataCount; j++)
				{
					UINT iFluoValue = pAryFlu[i].GetAt(j);
					if (pAryFlu[i].GetAt(j) > 55000)
						iMaxCount ++;
					if (pAryFlu[i].GetAt(j) < pChannelFluo[i])
						iMinCount ++;
				}

				if (iMaxCount > iDataCount/2)
				{
					m_eInstanceStatus = INSTANCE_FLUO_HIGH;
					break;
				}
				if (iMinCount > iDataCount/2)
				{
					m_eInstanceStatus = INSTANCE_FLUO_LOW;
					break;
				}
			}

			delete [] pAryFlu;
		}
	}

END:
	delete pChannelFluo;
	return bReturn;
}

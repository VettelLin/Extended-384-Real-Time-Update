#include "StdAfx.h"
#include "InstancePCR.h"
#include "ErrorCodeDefine.h"
#include "LogManager.h"
#include "LanguageInfo.h"
//#include <windows.h>

CInstancePCR::CInstancePCR(CSerialPortPCR* pComPort, CString strVersion)
{
	m_pComPort = (CSerialPortPCR*)pComPort;
	m_strModel.Empty();
	m_strSN.Empty();
	m_strVersion = strVersion;
	m_strCode.Empty();
	m_strSoftwareVer.Empty();
	m_dwYAccumulatedPulse = 0;

	m_uiErrorCode = 0;
	m_iErrorPara1 = -1;
	m_iErrorPara2 = -1;

	memset(&m_sHardwareStatus, 0, sizeof(tagHardwareStatus));

	m_iMppcGainH = 4742;
	m_iMppcGainM = 4125;
	m_iMppcGainL = 3637;

	m_eInstanceStatus = INSTANCE_READY;

	m_fPwmK1 = 1;
	m_fPwmB1 = 0;
	m_fPwmK2 = 1;
	m_fPwmB2 = 0;
	m_fPwmK3 = 1;
	m_fPwmB3 = 0;

	m_fEndCoolTem = 40;
	m_iMsSlpAfterCool = 10000;

	m_iMsPwmInitWait = 80000;
	m_iPreMeltMinSlpSec = 30;
}

CInstancePCR::~CInstancePCR(void)
{
	DeleteAll();
}

void CInstancePCR::DeleteAll()
{
	if (m_pComPort != NULL)
	{
		m_pComPort->CloseCom();
	}
	SAFE_DELETE(m_pComPort);

	DeleteFilterPara();
}

unsigned int CInstancePCR::GetErrorCode(int& iErrPara1, int& iErrPara2)
{
	iErrPara1 = m_iErrorPara1;
	iErrPara2 = m_iErrorPara2;
	return m_uiErrorCode;
}

BOOL CInstancePCR::Initialize()
{
	InitDevicePara();
	InitDeviceStatus();
	InitStructurePara();

	BOOL bReturn = Hello();
	if (!bReturn)
		return bReturn;

	// ��ȡ�豸����
	bReturn = ReadDeviceParamsFromEPROM();

	if (bReturn)
	{
		bReturn = ReadStructureParamsFromEPROM();
	}

	InitFilterPara();

	return bReturn;
}

void CInstancePCR::InitDevicePara()
{
	memset(m_sDevicePara.cDeviceModel, 0, 16);
	memset(m_sDevicePara.cDeviceSN, 0, 16);
	memset(m_sDevicePara.cDeviceVersion, 0, 16);
	memset(m_sDevicePara.cFirmware, 0, 16);
	memset(m_sDevicePara.cReserved1, 0, 160);

	char* pChannel1 = "T505,ET470/10,ET520/10";
	memset(m_sDevicePara.cKeyFilter1, 0, 32);
	memcpy(m_sDevicePara.cKeyFilter1, pChannel1, strlen(pChannel1));
	pChannel1 = "T650,ET630/16,ET670/20";
	memset(m_sDevicePara.cKeyFilter2, 0, 32);
	memcpy(m_sDevicePara.cKeyFilter2, pChannel1, strlen(pChannel1));
	pChannel1 = "T550,ET520/20,ET560/20";
	memset(m_sDevicePara.cKeyFilter3, 0, 32);
	memcpy(m_sDevicePara.cKeyFilter3, pChannel1, strlen(pChannel1));
	pChannel1 = "T700,ET670/20,ET740/20";
	memset(m_sDevicePara.cKeyFilter4, 0, 32);
	memcpy(m_sDevicePara.cKeyFilter4, pChannel1, strlen(pChannel1));
	pChannel1 = "T585,ET570/20,ET610/20";
	memset(m_sDevicePara.cKeyFilter5, 0, 32);
	memcpy(m_sDevicePara.cKeyFilter5, pChannel1, strlen(pChannel1));

	memset(m_sDevicePara.cKeyFilter6, 0, 32);
	memset(m_sDevicePara.cKeyFilter7, 0, 32);
	memset(m_sDevicePara.cKeyFilter8, 0, 32);

	pChannel1 = "11111400";
	memset(m_sDevicePara.cFilterConfig, 0, 8);
	memcpy(m_sDevicePara.cFilterConfig, pChannel1, strlen(pChannel1));

	memset(m_sDevicePara.cReserved2, 0, 24);

	m_sDevicePara.wXMotorBackPulse = 0;
	m_sDevicePara.wXMotorMaxFreq = 33000;
	m_sDevicePara.wXMotorFreq0 = 2000;
	m_sDevicePara.wXMotorScanFreq = 11000;
	m_sDevicePara.wXMotorHomeFreq = 10000;
	m_sDevicePara.wXMotorAccPulse = 400;
	m_sDevicePara.wXMotorMaxPls = 10000;
	m_sDevicePara.wXMotorScanPulse = 9500;
	m_sDevicePara.wXMotorOutDir = 0;
	m_sDevicePara.wXMotorBackFreq = 1200;

	m_sDevicePara.wYMotorBackPulse = 4600;
	m_sDevicePara.wYMotorMaxFreq = 33000;
	m_sDevicePara.wYMotorFreq0 = 2000;
	m_sDevicePara.wYMotorScanFreq = 16000;
	m_sDevicePara.wYMotorHomeFreq = 20000;
	m_sDevicePara.wYMotorAccPulse = 2000;
	m_sDevicePara.wYMotorMaxPls = 50000;
	m_sDevicePara.wYMotorScanPulse = 5905;
	m_sDevicePara.wYMotorOutDir = 0;
	m_sDevicePara.wYMotorBackFreq = 3000;
	m_sDevicePara.wYMotorMoveToFreq = 16000;

	memset(m_sDevicePara.cReserved3, 0, 214);
	memset(m_sDevicePara.cReserved4, 0, 512);
	memset(m_sDevicePara.cReserved5, 0, 256);
	memset(m_sDevicePara.cReserved6, 0, 256);

	m_sDevicePara.wPMTGainCh1 = 2300;
	memset(m_sDevicePara.cReserved7, 0, 94);

	m_sDevicePara.wCrtCoef1To1 = 0;
	m_sDevicePara.wCrtCoef2To1 = 0;
	m_sDevicePara.wCrtCoef3To1 = 0;
	m_sDevicePara.wCrtCoef4To1 = 0;
	m_sDevicePara.wCrtCoef5To1 = 0;
	m_sDevicePara.wCrtCoef6To1 = 0;
	m_sDevicePara.wCrtCoef7To1 = 0;
	m_sDevicePara.wCrtCoef8To1 = 0;

	m_sDevicePara.wCrtCoef1To2 = 0;
	m_sDevicePara.wCrtCoef2To2 = 0;
	m_sDevicePara.wCrtCoef3To2 = 0;
	m_sDevicePara.wCrtCoef4To2 = 0;
	m_sDevicePara.wCrtCoef5To2 = 0;
	m_sDevicePara.wCrtCoef6To2 = 0;
	m_sDevicePara.wCrtCoef7To2 = 0;
	m_sDevicePara.wCrtCoef8To2 = 0;

	m_sDevicePara.wCrtCoef1To3 = 0;
	m_sDevicePara.wCrtCoef2To3 = 0;
	m_sDevicePara.wCrtCoef3To3 = 0;
	m_sDevicePara.wCrtCoef4To3 = 0;
	m_sDevicePara.wCrtCoef5To3 = 0;
	m_sDevicePara.wCrtCoef6To3 = 0;
	m_sDevicePara.wCrtCoef7To3 = 0;
	m_sDevicePara.wCrtCoef8To3 = 0;

	m_sDevicePara.wCrtCoef1To4 = 0;
	m_sDevicePara.wCrtCoef2To4 = 0;
	m_sDevicePara.wCrtCoef3To4 = 0;
	m_sDevicePara.wCrtCoef4To4 = 0;
	m_sDevicePara.wCrtCoef5To4 = 0;
	m_sDevicePara.wCrtCoef6To4 = 0;
	m_sDevicePara.wCrtCoef7To4 = 0;
	m_sDevicePara.wCrtCoef8To4 = 0;

	m_sDevicePara.wCrtCoef1To5 = 0;
	m_sDevicePara.wCrtCoef2To5 = 0;
	m_sDevicePara.wCrtCoef3To5 = 0;
	m_sDevicePara.wCrtCoef4To5 = 0;
	m_sDevicePara.wCrtCoef5To5 = 0;
	m_sDevicePara.wCrtCoef6To5 = 0;
	m_sDevicePara.wCrtCoef7To5 = 0;
	m_sDevicePara.wCrtCoef8To5 = 0;

	m_sDevicePara.wCrtCoef1To6 = 0;
	m_sDevicePara.wCrtCoef2To6 = 0;
	m_sDevicePara.wCrtCoef3To6 = 0;
	m_sDevicePara.wCrtCoef4To6 = 0;
	m_sDevicePara.wCrtCoef5To6 = 0;
	m_sDevicePara.wCrtCoef6To6 = 0;
	m_sDevicePara.wCrtCoef7To6 = 0;
	m_sDevicePara.wCrtCoef8To6 = 0;

	memset(m_sDevicePara.cReserved8, 0, 64);
	memset(m_sDevicePara.cReserved9, 0, 512);
}

void CInstancePCR::InitDeviceStatus()
{
	m_sDeviceStatus.cXMot = 0; // X��״̬��0-������1-����
	m_sDeviceStatus.cYMot = 0; // Y��״̬��0-������1-����
	m_sDeviceStatus.cZMot = 0;		// Z��״̬��0-������1-����
	m_sDeviceStatus.cUMot = 0;		// U��״̬��0-������1-����
	m_sDeviceStatus.cMotSense = 0;	// ��翪�أ�0-������1-����
	m_sDeviceStatus.cEeprom = 0;	// EEPROM״̬��0-������1-����
	m_sDeviceStatus.cPMT = 0;		// PMT״̬��0-������1-����

	m_sDeviceStatus.cPETUnit1 = 0;	// ������1״̬��0-������1-����
	m_sDeviceStatus.cPETUnit2 = 0;	// ������2״̬��0-������1-����
	m_sDeviceStatus.cPETUnit3 = 0;	// ������3״̬��0-������1-����
	m_sDeviceStatus.cCap = 0;		// �ȸ�״̬��0-������1-����
	m_sDeviceStatus.cFanState = 0;	// ɢ��������״̬��0-������1-����
	m_sDeviceStatus.cAuxerState = 0; // ��������״̬��0-������1-����	
	m_sDeviceStatus.cA4940Rst = 0;	// ���ʰ�״̬��0-������1-����
	m_sDeviceStatus.cSysBox = 0;	// �����ְ�ť״̬��0-������1-�а���

	m_sDeviceStatus.cLid = 0; // ����״̬ 0���ر� 1����
}

// ��ʼ�������ṹ����
void CInstancePCR::InitStructurePara()
{
	m_sStructurePara.cFrameLength = 1;
	m_sStructurePara.cUseBox = 0;
	m_sStructurePara.wReserved1 = 0;
	m_sStructurePara.wReserved2 = 0;

	m_sStructurePara.cTempMode = TEMP_MODE_UPDOWN;
	m_sStructurePara.cTempCtrl = TEMP_CTRL_TUBE;
	m_sStructurePara.cTempModuleCount = 1;
	m_sStructurePara.cAutoCooling = 0;
	m_sStructurePara.cReserved3 = 0;

	m_sStructurePara.cPlateType = 0;
	m_sStructurePara.cPlateCount = 1;
	m_sStructurePara.cPlateSortType = 0;
	//Zhihao Lin
	m_sStructurePara.cPlateRowCount = 8;
	m_sStructurePara.cPlateColCount = 32;
	//xzq
	//m_sStructurePara.cPlateRowCount = 8;
	//m_sStructurePara.cPlateColCount = 12;
	//m_sStructurePara.cPlateRowCount = 3;
	//m_sStructurePara.cPlateColCount = 6;
	m_sStructurePara.cReserved4 = 0;

	m_sStructurePara.cScanMode = 0;
	m_sStructurePara.cScanOrigin = 0;
	m_sStructurePara.cScanPathCount = 1;
	m_sStructurePara.cReserved5 = 0;

	m_sStructurePara.cSupportMelt = 1;
	m_sStructurePara.cSupportMultiMelt = 0;
	m_sStructurePara.cMeltPreDelay = 60;
	m_sStructurePara.cMeltBeginTemp = 60;
	m_sStructurePara.cMeltBeginPreTemp = 3;
	m_sStructurePara.cMeltMinIncrement = 50;
	m_sStructurePara.cMeltStepDelay = 60;
	m_sStructurePara.cReserved6 = 0;

	m_sStructurePara.cTempChannelCount = 3;
	m_sStructurePara.wTempChannelFlag = 0;
	m_sStructurePara.cTempCtrlTop = 90;
	m_sStructurePara.cTempCtrlLower = 30;
	m_sStructurePara.cTempCtrlMaxTime = 60;
	m_sStructurePara.cTempLegalGap = 50;
	m_sStructurePara.cTempPeltierLower = 15;
	m_sStructurePara.cLidHeader = 1;
	m_sStructurePara.cLidTempTop = 100;
	m_sStructurePara.cLidTempLower = 30;
	m_sStructurePara.cLidTempMaxTime = 10;
	m_sStructurePara.cLidTempLegalGap = 50;
	m_sStructurePara.wReserved7 = 0;

	m_sStructurePara.wScanOnceTime = 1000;
	m_sStructurePara.cScanChangeRowTime = 0;
	m_sStructurePara.cUseCrosstailk = 2;
	m_sStructurePara.cCalibratePara = 0;
	m_sStructurePara.cReserved8 = 0;
	m_sStructurePara.wReserved9 = 0;

	m_sStructurePara.cUseFilter = 0;
	m_sStructurePara.cFilterAmplifyMethod = 1;
	m_sStructurePara.cFilterAmplifyShowMethod = 1;
	m_sStructurePara.cFilterMeltMethod = 2;

	memset(m_sStructurePara.cReserved10, 0, 454);
}

void CInstancePCR::InitFilterPara()
{
	DeleteFilterPara();

	char* pFilterConfig = m_sDevicePara.cFilterConfig;
	int iLength = 8;
	for (int i = 0; i < iLength; i++)
	{
		char cType[2];
		memset(cType, 0, 2);
		cType[0] = pFilterConfig[i];
		InitOneChannelFilter(i + 1, ::atoi(cType));
	}
}

void CInstancePCR::InitOneChannelFilter(int iChannel, int iByte)
{
	char* pChannelFlag = NULL;

	switch (iChannel)
	{
	case 1:
		pChannelFlag = m_sDevicePara.cKeyFilter1;
		break;
	case 2:
		pChannelFlag = m_sDevicePara.cKeyFilter2;
		break;
	case 3:
		pChannelFlag = m_sDevicePara.cKeyFilter3;
		break;
	case 4:
		pChannelFlag = m_sDevicePara.cKeyFilter4;
		break;
	case 5:
		pChannelFlag = m_sDevicePara.cKeyFilter5;
		break;
	case 6:
		pChannelFlag = m_sDevicePara.cKeyFilter6;
		break;
	case 7:
		pChannelFlag = m_sDevicePara.cKeyFilter7;
		break;
	case 8:
		pChannelFlag = m_sDevicePara.cKeyFilter8;
		break;
	default:
		break;
	}

	if (pChannelFlag == NULL)
		return;

	if (iByte == 1) // MPPC
	{
		BOOL bFstLine = TRUE;

		int iComma1 = -1;
		int iComma2 = -1;
		int iLine1 = -1;
		int iLine2 = -1;
		for (int i = 0; i < 32; i++)
		{
			if (pChannelFlag[i] == ',')
			{
				if (iComma1 == -1)
				{
					iComma1 = i;
				}
				else if (iComma2 == -1)
				{
					iComma2 = i;
				}
			}
			else if (pChannelFlag[i] == '/')
			{
				if (iLine1 == -1)
				{
					iLine1 = i;
				}
				else if (iLine2 == -1)
				{
					iLine2 = i;
				}

				/*if(!bFstLine)
				{
				if (iLine1 == -1)
				{
				iLine1 = i;
				}
				else if (iLine2 == -1)
				{
				iLine2 = i;
				}
				}
				else
				{
				bFstLine = FALSE;
				}*/
			}
		}

		if (iComma1 == -1 || iComma2 == -1 || iLine1 == -1 || iLine2 == -1)
		{
			ASSERT(FALSE);
			return;
		}

		char cExWavelength[8];
		memset(cExWavelength, 0, 8);
		memcpy(cExWavelength, pChannelFlag + iComma1 + 1, iLine1 - iComma1 - 1);
		char cExWaveRange[5];
		memset(cExWaveRange, 0, 5);
		memcpy(cExWaveRange, pChannelFlag + iLine1 + 1, iComma2 - iLine1 - 1);
		char cEmWavelength[8];
		memset(cEmWavelength, 0, 8);
		memcpy(cEmWavelength, pChannelFlag + iComma2 + 1, iLine2 - iComma2 - 1);
		char cEmWaveRange[5];
		memset(cEmWaveRange, 0, 5);
		memcpy(cEmWaveRange, pChannelFlag + iLine2 + 1, 2);

		int iExFirst = -1;
		int iEmFirst = -1;
		for (int j = 0; j < 8; j++)
		{
			if (cExWavelength[j] >= '0' && cExWavelength[j] <= '9')
			{
				if (iExFirst == -1)
					iExFirst = j;
			}

			if (cEmWavelength[j] >= '0' && cEmWavelength[j] <= '9')
			{
				if (iEmFirst == -1)
					iEmFirst = j;
			}
		}

		if (iExFirst != -1)
		{
			memcpy(cExWavelength, cExWavelength + iExFirst, 8 - iExFirst);
		}
		if (iEmFirst != -1)
		{
			memcpy(cEmWavelength, cEmWavelength + iEmFirst, 8 - iEmFirst);
		}

		tagScanChannelInfo* pFilter = new tagScanChannelInfo;
		pFilter->iType = iByte;
		pFilter->iExPosition = iChannel;
		pFilter->iExWaveLen = ::atoi(cExWavelength);
		pFilter->iExRange = ::atoi(cExWaveRange);
		pFilter->iEmPosition = iChannel;
		pFilter->iEmWaveLen = ::atoi(cEmWavelength);
		pFilter->iEmRange = ::atoi(cEmWaveRange);
		m_arrayFilter.Add(pFilter);
	}
	else if (iByte == 4) // MOTУ׼
	{
		tagScanChannelInfo* pFilter = new tagScanChannelInfo;
		pFilter->iType = iByte;
		pFilter->iExPosition = iChannel;
		pFilter->iExWaveLen = 0;
		pFilter->iExRange = 0;
		pFilter->iEmPosition = iChannel;
		pFilter->iEmWaveLen = 0;
		pFilter->iEmRange = 0;
		m_arrayFilter.Add(pFilter);
	}
}

void CInstancePCR::DeleteFilterPara()
{
	int iCount = m_arrayFilter.GetCount();
	for (int i = 0; i < iCount; i++)
	{
		tagScanChannelInfo* pFilter = (tagScanChannelInfo*)m_arrayFilter.GetAt(i);
		SAFE_DELETE(pFilter);
	}
	m_arrayFilter.RemoveAll();
}


// ���ɷ����������ָ���5/6/7�������û�в�����ֻ��һ������
BYTE* CInstancePCR::NewSendCommand(int iCmdLength, BYTE cCommand, WORD wPara)
{
	BYTE* pCmd = new BYTE[iCmdLength];
	memset(pCmd, 0, iCmdLength);
	pCmd[0] = PCR_DNSEND;//֡ͷ
	pCmd[1] = cCommand; //������
	pCmd[3] = iCmdLength; //����
	if (iCmdLength == CMD_SEND_LEN7)
	{
		pCmd[4] = (BYTE)((wPara & 0xFF00) >> 8);
		pCmd[5] = (BYTE)(wPara & 0xFF);
	}
	else if (iCmdLength == CMD_SEND_LEN6)
	{
		pCmd[4] = (BYTE)(wPara & 0xFF);;
	}
	pCmd[iCmdLength - 1] = m_pComPort->GetByteSum(pCmd, iCmdLength - 1);
	return pCmd;
}

// ���ɷ����������ָ���5/6/7����������������ֽڲ���
BYTE* CInstancePCR::NewSendCommand(int iCmdLength, BYTE cCommand, BYTE cPara1, BYTE cPara2)
{
	BYTE* pCmd = new BYTE[iCmdLength];
	memset(pCmd, 0, iCmdLength);
	pCmd[0] = PCR_DNSEND;//֡ͷ
	pCmd[1] = cCommand; //������
	pCmd[3] = iCmdLength; //����
	pCmd[4] = cPara1;
	pCmd[5] = cPara2;
	pCmd[iCmdLength - 1] = m_pComPort->GetByteSum(pCmd, iCmdLength - 1);
	return pCmd;
}

// ���ɷ����������ָ���9���������ǰʹ����ȡEEPROM����ʱ��������ַ��ȡ���ݳ��ȣ��ֽ�����
BYTE* CInstancePCR::NewSendCommand(int iCmdLength, BYTE cCommand, UINT nAddress, BYTE cGReceiveLen)
{
	BYTE* pCmd = new BYTE[iCmdLength];
	memset(pCmd, 0, iCmdLength);
	pCmd[0] = PCR_DNSEND;//֡ͷ
	pCmd[1] = cCommand; //������
	pCmd[3] = iCmdLength; //����
	pCmd[4] = (BYTE)((nAddress & 0xFF0000) >> 16);
	pCmd[5] = (BYTE)((nAddress & 0xFF00) >> 8);
	pCmd[6] = (BYTE)(nAddress & 0xFF);
	pCmd[7] = cGReceiveLen;
	pCmd[iCmdLength - 1] = m_pComPort->GetByteSum(pCmd, iCmdLength - 1);//У���
	return pCmd;
}
// �������� 10 �ֽڵ����������ֽ� + ���� + �ٶ� + У���
BYTE* CInstancePCR::NewSendCommand(int iCmdLength, BYTE cCommand, BYTE cFlag, WORD wPulse, WORD wSpeed)
{
	BYTE* pCmd = new BYTE[iCmdLength];
	memset(pCmd, 0, iCmdLength);

	pCmd[0] = PCR_DNSEND;     // ֡ͷ 0x5E
	pCmd[1] = cCommand;       // ������ 0x07
	pCmd[2] = 0x00;           // ����λ
	pCmd[3] = iCmdLength;     // ���ݳ���

	pCmd[4] = cFlag;          // ����/��־λ���� 0x15, 0x24, 0x28 �ȣ�

	pCmd[5] = (BYTE)((wPulse >> 8) & 0xFF); // ���� ���ֽ�
	pCmd[6] = (BYTE)(wPulse & 0xFF);        // ���� ���ֽ�

	pCmd[7] = (BYTE)((wSpeed >> 8) & 0xFF); // �ٶ� ���ֽ�
	pCmd[8] = (BYTE)(wSpeed & 0xFF);        // �ٶ� ���ֽ�

	// У��ͷŵ����һ�ֽڣ�ȡǰ iCmdLength - 1 ���ֽڵĺ�
	pCmd[iCmdLength - 1] = m_pComPort->GetByteSum(pCmd, iCmdLength - 1);

	return pCmd;
}



BYTE* CInstancePCR::NewSendCommand(int iCmdLength, BYTE cCommand, WORD wPara1, WORD wPara2, WORD wPara3, WORD wPara4, BYTE cMode)
{
	BYTE* pCmd = new BYTE[iCmdLength];
	memset(pCmd, 0, iCmdLength);
	pCmd[0] = PCR_DNSEND;//֡ͷ
	pCmd[1] = cCommand; //������
	pCmd[3] = iCmdLength; //����
	pCmd[4] = (BYTE)((wPara1 & 0xFF00) >> 8);
	pCmd[5] = (BYTE)(wPara1 & 0xFF);
	pCmd[6] = (BYTE)((wPara2 & 0xFF00) >> 8);
	pCmd[7] = (BYTE)(wPara2 & 0xFF);
	pCmd[8] = (BYTE)((wPara3 & 0xFF00) >> 8);
	pCmd[9] = (BYTE)(wPara3 & 0xFF);
	pCmd[10] = (BYTE)((wPara4 & 0xFF00) >> 8);
	pCmd[11] = (BYTE)(wPara4 & 0xFF);
	pCmd[12] = cMode;
	pCmd[iCmdLength - 1] = m_pComPort->GetByteSum(pCmd, iCmdLength - 1);//У���
	return pCmd;
}

// ����Hello����
BOOL CInstancePCR::Hello()
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN5, PCR_HELLO, 0);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN5, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 0;
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_HELLO_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_REPEAT_OUT)
		{
			m_uiErrorCode = ERR_HELLO_TIMEOUTL;
		}
	}

	delete pCommand;
	if (pStatus) delete[] pStatus;


	return bReturn;
}

//������λ
BOOL CInstancePCR::Home()
{
	BOOL bReturn = MotorReset(PCR_XMOT_RESET);
	if (!bReturn)
	{
		return bReturn;
	}

	bReturn = MotorReset(PCR_YMOT_RESET);
	return bReturn;
}

// �Ƿ����
BOOL CInstancePCR::IsInBox(eSysBoxStatus& outStatus)
{
	outStatus = BOXSTATE_UNKNOW;
	// �ȶ�ȡӲ���豸״̬
	BOOL bReturn = ReadHardwareStatus();
	if (bReturn)
	{
		outStatus = (eSysBoxStatus)(m_sHardwareStatus.nSysBox);
	}

	return bReturn;
}

// ���ͽ�������
BOOL CInstancePCR::InBox()
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN5, PCR_IN_BOX, 0);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	m_pComPort->SetLongReceiveTime(TRUE);
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN5, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 0;
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_BOX_IN_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_REPEAT_OUT)
		{
			m_uiErrorCode = ERR_BOX_IN_TIMEOUT;
		}
	}

	delete pCommand;
	if (pStatus) delete[] pStatus;

	return bReturn;
}

// ���ͳ�������
BOOL CInstancePCR::OutBox()
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN5, PCR_OUT_BOX, 0);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	m_pComPort->SetLongReceiveTime(TRUE);
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN5, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 0;
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_BOX_OUT_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_REPEAT_OUT)
		{
			m_uiErrorCode = ERR_BOX_OUT_FAIL;
		}
	}

	delete pCommand;
	if (pStatus) delete[] pStatus;

	return bReturn;
}

BOOL CInstancePCR::MotorReset(eUnitPCR nMotorFlag)
{
	int iMoveFlag = -1;
	switch (nMotorFlag)
	{
	case UNIT_MOTOR_X:
		iMoveFlag = PCR_XMOT_RESET;
		break;
	case UNIT_MOTOR_Y:
		iMoveFlag = PCR_YMOT_RESET;
		break;
	case UNIT_MOTOR_Z:
		iMoveFlag = PCR_ZMOT_RESET;
		break;
	case UNIT_MOTOR_U:
		iMoveFlag = PCR_UMOT_RESET;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return MotorReset(iMoveFlag);
}
BOOL CInstancePCR::MotorMove(eUnitPCR nMotorFlag, WORD nPulse, eMotorMoveFlag nMoveFlag)
{
	int iMoveFlag = -1;
	WORD nSpeed = 0;
	switch (nMotorFlag)
	{
	case UNIT_MOTOR_X:
		nSpeed = m_sDevicePara.wXMotorScanFreq;
		if (nMoveFlag == MOTOR_MOVE_TO)
		{
			iMoveFlag = PCR_XMOT_MOVETO;
		}
		else if (nMoveFlag == MOTOR_FORWARD)
		{
			iMoveFlag = PCR_XMOT_FORWARD;
		}
		else if (nMoveFlag == MOTOR_REVERSE)
		{
			iMoveFlag = PCR_XMOT_REVERSE;
		}
		else
		{
			ASSERT(FALSE);
		}
		break;
	case UNIT_MOTOR_Y:
		nSpeed = m_sDevicePara.wYMotorScanFreq;
		if (nMoveFlag == MOTOR_MOVE_TO)
		{
			iMoveFlag = PCR_YMOT_MOVETO;
		}
		else if (nMoveFlag == MOTOR_FORWARD)
		{
			iMoveFlag = PCR_YMOT_FORWARD;
		}
		else if (nMoveFlag == MOTOR_REVERSE)
		{
			iMoveFlag = PCR_YMOT_REVERSE;
		}
		else
		{
			ASSERT(FALSE);
		}
		break;
	case UNIT_MOTOR_Z:
		ASSERT(FALSE);
		break;
	case UNIT_MOTOR_U:
		ASSERT(FALSE);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return MotorMove(iMoveFlag, nPulse, nSpeed);
}

BOOL CInstancePCR::MotorReset(BYTE cMotorFlag)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN6, PCR_MOTOR, cMotorFlag);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	m_pComPort->SetLongReceiveTime(TRUE);
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN6, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_MOTOR_RESET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_MOTOR_RESET_TIMEOUT;
		}

		m_iErrorPara1 = cMotorFlag;
	}
	m_pComPort->SetLongReceiveTime(FALSE);

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}
	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

BOOL CInstancePCR::MotorMove(BYTE cMotorFlag, WORD wPulse, WORD wSpeed)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN10, PCR_MOTOR, cMotorFlag, wPulse, wSpeed);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	m_pComPort->SetLongReceiveTime(TRUE);
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN10, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_MOTOR_MOVE_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_MOTOR_MOVE_TIMEOUT;
		}

		m_iErrorPara1 = cMotorFlag;
		m_iErrorPara2 = wPulse;
	}
	m_pComPort->SetLongReceiveTime(FALSE);

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}
	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

// Ĭ��X�����ɨ��һ��
BOOL CInstancePCR::MotorScanRow(WORD wPulse, BYTE cDirection)
{
	int iCmdLength = CMD_SEND_LEN11;
	WORD wSpeed = m_sDevicePara.wXMotorScanFreq;

	BYTE* pCommand = new BYTE[iCmdLength];
	memset(pCommand, 0, iCmdLength);
	pCommand[0] = PCR_DNSEND;//֡ͷ
	pCommand[1] = PCR_MOTOR; //������
	pCommand[3] = iCmdLength; //����
	pCommand[4] = PCR_XMOT_SCAN;
	pCommand[5] = (BYTE)((wPulse & 0xFF00) >> 8);
	pCommand[6] = (BYTE)(wPulse & 0xFF);
	pCommand[7] = cDirection;
	pCommand[8] = (BYTE)((wSpeed & 0xFF00) >> 8);
	pCommand[9] = (BYTE)(wSpeed & 0xFF);
	pCommand[iCmdLength - 1] = m_pComPort->GetByteSum(pCommand, iCmdLength - 1);//У���

	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, iCmdLength, &pStatus, iReceiveFrameLen, 1);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_MOTOR_MOVE_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_MOTOR_MOVE_TIMEOUT;
		}

		m_iErrorPara1 = PCR_XMOT_SCAN;
		m_iErrorPara2 = wPulse;
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}
	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;

}


BOOL CInstancePCR::SendYMotorAbsoluteMove4Byte(DWORD dwPulse, WORD wSpeed)
{
#define PCR_YMOT_ABS4BYTE  0x28
	const int iCmdLength = 12;
	BYTE* pCommand = new BYTE[iCmdLength];
	memset(pCommand, 0, iCmdLength);

	pCommand[0] = PCR_DNSEND;     // Frame Header: 0x5E
	pCommand[1] = PCR_MOTOR;      // Command ID:    0x07
	pCommand[3] = iCmdLength;     // Frame Length:  0x0C
	pCommand[4] = PCR_YMOT_ABS4BYTE; // Motor case:  0x28

	// 4-byte Pulse value (big-endian)
	pCommand[5] = (BYTE)((dwPulse >> 24) & 0xFF);
	pCommand[6] = (BYTE)((dwPulse >> 16) & 0xFF);
	pCommand[7] = (BYTE)((dwPulse >> 8) & 0xFF);
	pCommand[8] = (BYTE)(dwPulse & 0xFF);

	// 2-byte Speed value (big-endian)
	pCommand[9] = (BYTE)((wSpeed >> 8) & 0xFF);
	pCommand[10] = (BYTE)(wSpeed & 0xFF);

	// Calculate checksum
	BYTE bCheckSum = m_pComPort->GetByteSum(pCommand, iCmdLength - 1);
	pCommand[iCmdLength - 1] = bCheckSum;

	// Send command
	BYTE* pStatus = NULL;
	int iReceiveFrameLen = 0;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, iCmdLength, &pStatus, iReceiveFrameLen, 1);

	BOOL bSuccess = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bSuccess = TRUE;
	}
	else
	{
		m_uiErrorCode = (iReturn == CSerialPortPCR::RETURN_FAIL) ? ERR_MOTOR_MOVE_FAIL : ERR_MOTOR_MOVE_TIMEOUT;
		m_iErrorPara1 = PCR_YMOT_ABS4BYTE;
		m_iErrorPara2 = static_cast<UINT>(dwPulse);
	}

	delete[] pCommand;
	if (pStatus) delete[] pStatus;

	return bSuccess;
}



// 简化：无参版本调用可中断版本
BOOL CInstancePCR::ScanOnce()
{
	BOOL bAskStop = FALSE;
	return ScanOnce(&bAskStop);
}





// ʵ����
// 在 CInstancePCR 类里新增成员：
// DWORD m_dwYAccumulatedPulse = 0;

BOOL CInstancePCR::ScanOnce(BOOL* pBeAskTop)
{
	// Scan pattern: 4 blocks of 12×8 (384 wells total)
	// Each block: 8 X-scans + 7 Y-moves (snake pattern)
	// After each block (except last): Y moves extra 10mm alone (coarse move)
	// IMPORTANT CHANGE:
	//  - Do NOT auto-home Y at end
	//  - Keep Y accumulated position across calls (m_dwYAccumulatedPulse)
	//  - Only home when asked to stop (pBeAskTop) or when you explicitly decide to end experiment

	BOOL bReturn = TRUE;

	int  iLineDistance = m_sDevicePara.wYMotorScanPulse;     // Pulse per grid
	int  iXMotScanDistance = m_sDevicePara.wXMotorScanPulse;
	WORD wSpeed = m_sDevicePara.wYMotorScanFreq;

	// 计算“10mm 粗移”的脉冲数：已知1格=9mm -> iLineDistance 脉冲
	int i10mmPulse = static_cast<int>(static_cast<double>(iLineDistance) * 10.0 / 9.0 + 0.5);

	const int BLOCKS = 4;
	const int X_SCANS_PER_BLOCK = 8;   // 7 with Y + 1 X-only
	const int Y_MOVES_PER_BLOCK = 7;

	BOOL bForward = TRUE;

#ifdef _DEBUG
	CString strDebug;
#endif

	// 对齐本次扫描到第0行（与UI首行一致）；避免一开始跳到很大绝对位置
	m_dwYAccumulatedPulse = 0;
	MotorMove(CInstancePCR::UNIT_MOTOR_Y, 0, CInstancePCR::MOTOR_MOVE_TO);
	DWORD dwAccumulatedPulse = 0;

	for (int block = 0; block < BLOCKS; ++block)
	{
		if (pBeAskTop && *pBeAskTop)
		{
			bReturn = FALSE;
			break;
		}

		// 每个 block 内部从 forward 开始（保持你原来的策略）
		bForward = TRUE;

		// 8 次 X 扫描；前 7 次之后每次都要 Y+1格；第 8 次只扫X不动Y
		for (int stepInBlock = 0; stepInBlock < X_SCANS_PER_BLOCK; ++stepInBlock)
		{
			if (pBeAskTop && *pBeAskTop)
			{
				bReturn = FALSE;
				break;
			}

			// 1) X 扫描（蛇形）
			if (!MotorScanRow(iXMotScanDistance, bForward ? 1 : 0))
			{
				bReturn = FALSE;
				break;
			}

#ifdef _DEBUG
			strDebug.Format(_T("X %s %d"), bForward ? _T("forward") : _T("reverse"), iXMotScanDistance);
			OutputDebugString(strDebug);
#endif

			// 2) 前 7 次：Y 走一格（绝对位置累加）
			if (stepInBlock < Y_MOVES_PER_BLOCK)
			{
				dwAccumulatedPulse += static_cast<DWORD>(iLineDistance);

				if (!SendYMotorAbsoluteMove4Byte(dwAccumulatedPulse, wSpeed))
				{
					bReturn = FALSE;
					break;
				}

#ifdef _DEBUG
				strDebug.Format(_T("Y +%d -> %lu"), iLineDistance, dwAccumulatedPulse);
				OutputDebugString(strDebug);
#endif
			}
			else
			{
				// 第 8 次：X-only（不动Y）
			}

			// 下一次 X 扫描反向
			bForward = !bForward;
		}

		if (!bReturn) break;

		// 3) block 扫完以后（除最后一个 block），Y 额外走 10mm（分块粗移）
		if (block < BLOCKS - 1)
		{
			dwAccumulatedPulse += static_cast<DWORD>(i10mmPulse);

			if (!SendYMotorAbsoluteMove4Byte(dwAccumulatedPulse, wSpeed))
			{
				bReturn = FALSE;
				break;
			}

#ifdef _DEBUG
			OutputDebugString(_T("Y +10mm (block gap move)"));
#endif
		}
	}

	// ✅ 关键：把累计位置写回成员变量，让下一次 ScanOnce 继续往下
	m_dwYAccumulatedPulse = dwAccumulatedPulse;

	// ✅ 不再“函数结束自动回原点”
	// 你如果需要“真正结束实验/用户点停止”才回原点：
	if (pBeAskTop && *pBeAskTop)
	{
#ifdef _DEBUG
		OutputDebugString(_T("STOP asked -> Y GO HOME"));
#endif
		MotorMove(CInstancePCR::UNIT_MOTOR_Y, 0, CInstancePCR::MOTOR_MOVE_TO);
	}

	return bReturn;
}











BOOL CInstancePCR::SetStateLED(BYTE cMode)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN6, PCR_STALED, cMode);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	m_pComPort->SetLongReceiveTime(TRUE);
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN6, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_LED_SET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_LED_SET_TIMEOUT;
		}
		m_iErrorPara1 = UINT_STATE_LED;
	}
	m_pComPort->SetLongReceiveTime(FALSE);

	delete pCommand;
	if (pStatus) delete[] pStatus;


	return bReturn;
}

// ��������PMT���Ƶ�ѹ����
BOOL CInstancePCR::SetPMTGain(WORD wGainPara)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN7, PCR_PMT_GAIN, wGainPara);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN7, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_LED_SET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_LED_SET_TIMEOUT;
		}

		m_iErrorPara1 = UINT_STATE_LED;
	}

	delete pCommand;
if (pStatus) delete[] pStatus;


	return bReturn;
}

// PCR ����ģʽ
BOOL CInstancePCR::SetTempCtrlMode(eTempCtrlMode nCtrlMode)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN6, PCR_TEMP_MODE, nCtrlMode);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;

	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN6, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_GAIN_SET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_GAIN_SET_TIMEOUT;
		}
	}

	delete pCommand;
	if (pStatus) delete[] pStatus;


	return bReturn;
}

void CInstancePCR::SetDeviceParams(BYTE* pPara, int Len)
{
	memcpy(&m_sDevicePara, pPara, Len);
}

WORD CInstancePCR::GetPMTGain()
{
	WORD wPMTGain = m_sDevicePara.wPMTGainCh1;
	if (wPMTGain == 0)
	{
		//ASSERT(FALSE);
		wPMTGain = 2300;
	}

	return wPMTGain;
}

// ȡ��ϵͳ�����Ԥ���¶Ȳ���
float CInstancePCR::GetSysTemp(eSysTempType nType)
{
	float fTemp = 0.0f;
	switch (nType)
	{
	case SYS_TEMP_CTRL_TOP:
		fTemp = m_sStructurePara.cTempCtrlTop;
		break;
	case SYS_TEMP_CTRL_LOWER:
		fTemp = m_sStructurePara.cTempCtrlLower;
		break;
	case SYS_TEMP_LID_TOP:
		fTemp = m_sStructurePara.cLidTempTop;
		break;
	case SYS_TEMP_LID_LOWER:
		fTemp = m_sStructurePara.cLidTempLower;
		break;
	case SYS_TEMP_PELTIER_LOWER:
		fTemp = m_sStructurePara.cTempPeltierLower;
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return fTemp;
}


// �����ȸ��¶�
BOOL CInstancePCR::SetLidHeaterTemp(float fTemp)
{
	WORD wTemp = (WORD)(fTemp * RESOLUTION_TEMP + 0.5);
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN7, PCR_LID_TEMP, wTemp);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN7, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = TEMP_CHANNEL_LID; // ��ʾ�ȸ�
		m_iErrorPara2 = wTemp; // �¶�
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_LASER_SET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_TEMP_SET_TIMEOUT;
		}
	}

	delete pCommand;
	if (pStatus) delete[] pStatus;


	return bReturn;
}

// ȡ����������ʱ����Ŀ���¶ȵĺ���ƫ��
// ��λ���������¶ȵ���ʶλ����1ʱ�� ʵ���¶�ֵ��Ŀ���¶�ֵ֮���������ĺ���ƫ�Χ
float CInstancePCR::GetTempLegalGap()
{
	float fReturn = m_sStructurePara.cTempLegalGap;
	fReturn /= 10;
	return fReturn;
}

float CInstancePCR::GetLidTempLegalGap()
{
	float fReturn = m_sStructurePara.cLidTempLegalGap;
	fReturn /= 10;
	return fReturn;
}

// �ȸ������ʱ�䣬��λ���룬Ĭ��15����
int CInstancePCR::GetMaxLidHeatSecond()
{
	int iMaxTime = m_sStructurePara.cLidTempMaxTime * 1;
	return iMaxTime;
}

// ȡ�������ʱ�䣬Ĭ��60��
int CInstancePCR::GetMaxTempCtrlSecond()
{
	int iMaxTime = m_sStructurePara.cTempCtrlMaxTime;
	return iMaxTime;
}

int CInstancePCR::GetLidHeatNormalSecond()
{
	if (0 == m_sStructurePara.cLidHeader)
	{
		return 0;
	}
	return 0;
}

int CInstancePCR::GetLidState(int& iStatus)
{
	return -1;
}

// ȡ���м�������ͨ�����������ȸ�
int CInstancePCR::GetTempChannelCount()
{
	int iCount = m_sStructurePara.cTempChannelCount;
	if (m_sStructurePara.cLidHeader == 1)
	{
		iCount--;
	}
	return iCount;
}

int CInstancePCR::GetMeltPreDelay()
{
	int iMaxTime = m_sStructurePara.cMeltPreDelay;
	return iMaxTime;
}

// �۽ⵥ����С����
float CInstancePCR::GetMeltMinIncrement()
{
	float fIncrement = m_sStructurePara.cMeltMinIncrement;
	fIncrement /= 100;
	return fIncrement;
}



// ɨ��ָ��ͨ��һ����Ҫʱ�䣬��λ
int CInstancePCR::GetScanOnceTime(BOOL bIsSecond)
{
	int iMaxTime = m_sStructurePara.wScanOnceTime;
	if (bIsSecond)
	{
		iMaxTime /= 100;
	}
	return iMaxTime;
}

// ���һ��ɨ��X�����д���
int CInstancePCR::GetScanRowCount()
{
	int iCount = 0;

	if (m_sStructurePara.cScanPathCount == 1)
	{
		iCount = m_sStructurePara.cPlateRowCount;
	}
	else if (m_sStructurePara.cScanPathCount == 2)
	{
		iCount = m_sStructurePara.cPlateRowCount + 2;
	}
	else
	{
		ASSERT(FALSE);
	}

	return iCount;
}

// ȡ��У׼ͨ��
int CInstancePCR::GetCalibrateChannel()
{
	int iChannel = -1;
	for (int i = 0; i < m_arrayFilter.GetCount(); i++)
	{
		tagScanChannelInfo* pChannel = (tagScanChannelInfo*)m_arrayFilter.GetAt(i);
		if (pChannel->iType == 4)
		{
			iChannel = pChannel->iEmPosition;
			break;
		}
	}

	return iChannel;
}

// ֻ����ɨ���·ͨ���������ش��ڵ�ͨ���������м䲻��ͨ��Ҳ�Ǵ���
int CInstancePCR::GetScanChannelCount()
{
	int iMaxChannel = -1;
	for (int i = 0; i < m_arrayFilter.GetCount(); i++)
	{
		tagScanChannelInfo* pChannel = (tagScanChannelInfo*)m_arrayFilter.GetAt(i);
		if (pChannel->iType != 4)
		{
			if (pChannel->iExPosition > iMaxChannel)
			{
				iMaxChannel = pChannel->iExPosition;
			}
		}
	}

	if (m_arrayFilter.GetCount() > iMaxChannel)
	{
		return m_arrayFilter.GetCount();
	}
	else
	{
		return iMaxChannel;
	}
}

BOOL CInstancePCR::SetMultiTemp(float fTemp1, float fTemp2, float fTemp3, float fRate, BYTE cMode, BOOL bSelfCheck /*= FALSE*/)
{
	WORD wTemp1 = (int)(fTemp1 * RESOLUTION_TEMP + 0.5);
	WORD wTemp2 = (int)(fTemp2 * RESOLUTION_TEMP + 0.5);
	WORD wTemp3 = (int)(fTemp3 * RESOLUTION_TEMP + 0.5);
	WORD wRate = (int)(fRate * RESOLUTION_TEMP + 0.5);

	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN14, PCR_SET_TEMP, wTemp1, wTemp2, wTemp3, wRate, cMode);

	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN14, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else if (bSelfCheck && NULL != pStatus && iReceiveFrameLen > 2 && (0x01 == pStatus[iReceiveFrameLen - 2]))
	{
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 7; // ��ʾ3���¶�ͨ��
		m_iErrorPara2 = wTemp1; // �¶�
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_LASER_SET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_TEMP_SET_TIMEOUT;
		}
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}
	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

BOOL CInstancePCR::SetPWMTemp(float fTemp1, float fTemp2, float fTemp3, WORD Pwm1, WORD Pwm2, WORD Pwm3)
{
	WORD wTemp1 = (int)(fTemp1 * RESOLUTION_TEMP + 0.5);
	WORD wTemp2 = (int)(fTemp2 * RESOLUTION_TEMP + 0.5);
	WORD wTemp3 = (int)(fTemp3 * RESOLUTION_TEMP + 0.5);

	BYTE* pCmd = new BYTE[17];
	if (NULL == pCmd)
	{
		return FALSE;
	}
	memset(pCmd, 0, 17);

	pCmd[0] = PCR_DNSEND;
	pCmd[1] = PCR_SETPWM;
	pCmd[3] = 17;

	//�¶�ֵ
	pCmd[4] = (BYTE)(wTemp1 >> 8);
	pCmd[5] = (BYTE)(wTemp1 & 0xFF);
	pCmd[6] = (BYTE)(wTemp2 >> 8);
	pCmd[7] = (BYTE)(wTemp2 & 0xFF);
	pCmd[8] = (BYTE)(wTemp3 >> 8);
	pCmd[9] = (BYTE)(wTemp3 & 0xFF);

	//PWMֵ
	pCmd[10] = (BYTE)(Pwm1 >> 8);
	pCmd[11] = (BYTE)(Pwm1 & 0xFF);
	pCmd[12] = (BYTE)(Pwm2 >> 8);
	pCmd[13] = (BYTE)(Pwm2 & 0xFF);
	pCmd[14] = (BYTE)(Pwm3 >> 8);
	pCmd[15] = (BYTE)(Pwm3 & 0xFF);

	//У���
	for (int i = 0; i < 16; ++i)
	{
		pCmd[16] += pCmd[i];
	}

	int iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCmd, 17, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 7; // ��ʾ3���¶�ͨ��
		m_iErrorPara2 = wTemp1; // �¶�
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_LASER_SET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_TEMP_SET_TIMEOUT;
		}
	}

	delete[] pCmd;
	if (NULL != pStatus)
	{
		if (pStatus) delete[] pStatus;

	}

	return bReturn;
}

BOOL CInstancePCR::SetLaserStatus(BYTE cType, BYTE cStatus)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN7, PCR_LASER_CTRL, cType, cStatus);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN7, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = cType;
		m_iErrorPara2 = cStatus;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_LASER_SET_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_LASER_SET_TIMEOUT;
		}
	}

	delete pCommand;
	if (pStatus) delete[] pStatus;

	return bReturn;
}

// ��ȡָ��ͨ���¶�
BOOL CInstancePCR::ReadChannelTemp(BYTE cChannel, float& fTempCh, BOOL& bToTarget)
{
	if (cChannel == TEMP_CHANNEL_ENVIRONMENT)
	{
		m_pComPort->SetLongSendTime(TRUE);
	}

	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN6, PCR_READ_TEMP, cChannel);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN6, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		ASSERT(iReceiveFrameLen == 9);

		int iTemp = 0;
		iTemp = pStatus[iReceiveFrameLen - 5];
		iTemp = iTemp << 8;
		iTemp &= 0xFF00;
		iTemp |= pStatus[iReceiveFrameLen - 4];
		fTempCh = ((float)iTemp) / RESOLUTION_TEMP;
		bToTarget = (BOOL)pStatus[iReceiveFrameLen - 3];

		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = cChannel; // ��ʾͨ��
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_TEMP_READ_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_TEMP_READ_TIMEOUT;
		}
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}
	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	if (cChannel == TEMP_CHANNEL_ENVIRONMENT)
	{
		m_pComPort->SetLongSendTime(FALSE);
	}

	return bReturn;
}

// ��ȡPCR���¶�
BOOL CInstancePCR::ReadPCRTemp(float& fTempPCR, BOOL& bToTarget)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN6, PCR_READ_PCRTEMP, 0);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN6, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		ASSERT(iReceiveFrameLen == 9);

		int iTemp = 0;
		iTemp = pStatus[iReceiveFrameLen - 5];
		iTemp = iTemp << 8;
		iTemp &= 0xFF00;
		iTemp |= pStatus[iReceiveFrameLen - 4];
		fTempPCR = ((float)iTemp) / RESOLUTION_TEMP;
		bToTarget = (BOOL)pStatus[iReceiveFrameLen - 3];

		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 0; // ��Чֵ
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_PCR_TEMP_READ_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_PCR_TEMP_READ_TIMEOUT;
		}
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}

	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

BOOL CInstancePCR::ReadFLUData(CUIntArray& arrayData, BYTE cChannel, BYTE cFrmIndex)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN7, PCR_READ_FLUDATA, cChannel, cFrmIndex);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN7, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		//xzq:������ʱ�������
		UINT uintFludata[200];
		int iIndex = 0;
		for (int i = 4; i + 1 < iReceiveFrameLen - 2; i += 2)
		{
			UINT nFludata = 0;
			nFludata = pStatus[i + 1];
			nFludata = nFludata << 8;
			nFludata &= 0xFF00;
			nFludata |= pStatus[i];
			//xzq:ע�͵����ȸ�ֵ����ʱ������Ȼ����������֯����
			//arrayData.Add(nFludata);
			uintFludata[iIndex] = nFludata;
			iIndex++;
		}
		//xzq:��96�ܵ�����ת����18�ܵ����ݣ���96�ܵ�A,C,E�ţ�1��3��5��7��9��11,
		for (int j = 0; j < 18; j++)
		{
		
			UINT* nFludata;
			switch (j)
			{
			case 0:
				arrayData.Add(uintFludata[0]);
				break;
			case 1:
				arrayData.Add(uintFludata[1]);
				break;
			case 2:
				arrayData.Add(uintFludata[2]);
				break;
			case 3:
				arrayData.Add(uintFludata[3]);
				break;
			case 4:
				arrayData.Add(uintFludata[4]);
				break;
			case 5:
				arrayData.Add(uintFludata[5]);
				break;
			case 6:
				arrayData.Add(uintFludata[6]);
				break;
			case 7:
				arrayData.Add(uintFludata[7]);
				break;
			case 8:
				arrayData.Add(uintFludata[8]);
				break;
			case 9:
				arrayData.Add(uintFludata[9]);
				break;
			case 10:
				arrayData.Add(uintFludata[10]);
				break;
			case 11:
				arrayData.Add(uintFludata[11]);
				break;
			case 12:
				arrayData.Add(uintFludata[12]);
				break;
			case 13:
				arrayData.Add(uintFludata[13]);
				break;
			case 14:
				arrayData.Add(uintFludata[14]);
				break;
			case 15:
				arrayData.Add(uintFludata[15]);
				break;
			case 16:
				arrayData.Add(uintFludata[16]);
				break;
			case 17:
				arrayData.Add(uintFludata[17]);
				break;
			}
		}

		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = cChannel;
		m_iErrorPara2 = cFrmIndex;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_PCR_FLU_READ_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_PCR_FLU_READ_TIMEOUT;
		}
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}

	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

BOOL CInstancePCR::ReadAllFLUData(CUIntArray* pAryData, int iChanNum)
{
	if (NULL == pAryData)
	{
		return FALSE;
	}
	if (iChanNum != GetScanChannelCount())
	{
		return FALSE;
	}

	BOOL bReturn = FALSE;

	for (int ichannel = 1; ichannel <= iChanNum; ++ichannel)
	{
		bReturn = ReadFLUData(pAryData[ichannel - 1], ichannel);
		if (!bReturn)
		{
			break;
		}
	}

	return bReturn;
}

// ��ȡ�豸״̬����ʵ�������ʹ��
BOOL CInstancePCR::ReadDeviceStatus()
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN5, PCR_DEVICE_STATUS, 0);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN5, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		SetDeviceStatusBy(iReceiveFrameLen - 6, pStatus + 4);
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 0; // ��Чֵ
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_PCR_STATUS_READ_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_PCR_STATUS_READ_TIMEOUT;
		}
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}

	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

// ���ݷ��ز���������״̬���б���
void CInstancePCR::SetDeviceStatusBy(int iLength, BYTE* pStatus)
{
	m_sDeviceStatus.cXMot = (pStatus[0] & (1 << 1)) >> 1;
	m_sDeviceStatus.cYMot = (pStatus[0] & (1 << 2)) >> 2;
	m_sDeviceStatus.cZMot = (pStatus[0] & (1 << 3)) >> 3;
	m_sDeviceStatus.cUMot = (pStatus[0] & (1 << 4)) >> 4;
	m_sDeviceStatus.cMotSense = (pStatus[0] & (1 << 5)) >> 5;
	m_sDeviceStatus.cEeprom = (pStatus[0] & (1 << 6)) >> 6;
	m_sDeviceStatus.cPMT = (pStatus[0] & (1 << 7)) >> 7;

	m_sDeviceStatus.cPETUnit1 = (pStatus[1] & (1));
	m_sDeviceStatus.cPETUnit2 = (pStatus[1] & (1 << 1)) >> 1;
	m_sDeviceStatus.cPETUnit3 = (pStatus[1] & (1 << 2)) >> 2;
	m_sDeviceStatus.cCap = (pStatus[1] & (1 << 3)) >> 3;
	m_sDeviceStatus.cFanState = (pStatus[1] & (1 << 4)) >> 4;
	m_sDeviceStatus.cAuxerState = (pStatus[1] & (1 << 5)) >> 5;
	m_sDeviceStatus.cA4940Rst = (pStatus[1] & (1 << 6)) >> 6;
	m_sDeviceStatus.cSysBox = (pStatus[1] & (1 << 7)) >> 7;

	// Э���޸�������2���ֽ���Ϊ���ز���
	if (iLength > 2)
	{
		m_sDeviceStatus.cLid = (pStatus[2] & (1));
	}
}


// ��ȡ�豸״̬����ȡ��λ�����е�״̬����������Ӳ���豸�ı�־������ȣ���������״̬69�ֽڣ�
// ��������ָ��������в��������쳣ʱ���� 
BOOL CInstancePCR::ReadHardwareStatus()
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN5, PCR_HARDWARE_STATUS, 0);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN5, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		SetHardwareStatus(pStatus + 4);
		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 0; // ��Чֵ
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_PCR_STATUS_READ_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_PCR_STATUS_READ_TIMEOUT;
		}
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}
	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

void CInstancePCR::SetHardwareStatus(BYTE* state)
{
	m_sHardwareStatus.nDevRun = state[0];    ///< 
	m_sHardwareStatus.nXMotState = state[1];  ///< 
	m_sHardwareStatus.wXMotPos = state[2]; ///< 
	m_sHardwareStatus.wXMotPos = WORD(m_sHardwareStatus.wXMotPos << 8) + state[3];
	m_sHardwareStatus.nXMotDirection = state[4]; ///< 

	m_sHardwareStatus.nYMotState = state[5];  ///< 
	m_sHardwareStatus.wYMotPos = state[6]; ///< 
	m_sHardwareStatus.wYMotPos = WORD(m_sHardwareStatus.wXMotPos << 8) + state[7];
	m_sHardwareStatus.nYMotDirection = state[8]; ///< 

	m_sHardwareStatus.nZMotState = state[9];  ///< 
	m_sHardwareStatus.wZMotPos = state[10]; ///< 
	m_sHardwareStatus.wZMotPos = WORD(m_sHardwareStatus.wXMotPos << 8) + state[11];
	m_sHardwareStatus.nZMotDirection = state[12]; ///< 

	m_sHardwareStatus.nUMotState = state[13];  ///< 
	m_sHardwareStatus.wUMotPos = state[14]; ///< 
	m_sHardwareStatus.wUMotPos = WORD(m_sHardwareStatus.wXMotPos << 8) + state[15];
	m_sHardwareStatus.nUMotDirection = state[16]; ///< 

	m_sHardwareStatus.nMotSensor = state[17];  ///< 
	m_sHardwareStatus.nLED = state[18];        ///< 
	m_sHardwareStatus.nPmtAnalogState = state[19];      ///< 
	m_sHardwareStatus.nEeprom = state[20];       ///< 
	m_sHardwareStatus.nSMBUart = state[21];       ///< 

	m_sHardwareStatus.wPMT7512Out = state[22];   ///< 
	m_sHardwareStatus.wPMT7512Out = WORD(m_sHardwareStatus.wPMT7512Out << 8) + state[23];   ///< 

	m_sHardwareStatus.wSMBBackground = state[24];         ///<
	m_sHardwareStatus.wSMBBackground = WORD(m_sHardwareStatus.wSMBBackground << 8) + state[25];         ///<

	m_sHardwareStatus.nSysBox = state[26];///< 

	m_sHardwareStatus.wScannertTemp = state[27];    ///< 
	m_sHardwareStatus.wScannertTemp = WORD(m_sHardwareStatus.wScannertTemp << 8) + state[28];    ///< 

	m_sHardwareStatus.nSMBParameterInit = state[29];    ///< 
	m_sHardwareStatus.nWKBParaInit = state[30];    ///<
	m_sHardwareStatus.nSysStateLed = state[31]; ///< 
	m_sHardwareStatus.nCurtTempCtrlMode_TubeBlockMode = state[32];   ///< 

	m_sHardwareStatus.nPETUnit1State = state[33];     ///< 

	m_sHardwareStatus.wPETUnit1Set = state[34];  ///< 
	m_sHardwareStatus.wPETUnit1Set = WORD(m_sHardwareStatus.wPETUnit1Set << 8) + state[35];  ///< 

	m_sHardwareStatus.wPETUnit1tTemp = state[36];  ///<
	m_sHardwareStatus.wPETUnit1tTemp = WORD(m_sHardwareStatus.wPETUnit1tTemp << 8) + state[37];  ///<

	m_sHardwareStatus.nPETUnit2State = state[38];     ///< 

	m_sHardwareStatus.wPETUnit2Set = state[39];  ///< 
	m_sHardwareStatus.wPETUnit2Set = WORD(m_sHardwareStatus.wPETUnit2Set << 8) + state[40];  ///<

	m_sHardwareStatus.wPETUnit2tTemp = state[41];  ///<
	m_sHardwareStatus.wPETUnit2tTemp = WORD(m_sHardwareStatus.wPETUnit2tTemp << 8) + state[42];  ///<

	m_sHardwareStatus.nPETUnit3State = state[43];     ///< 

	m_sHardwareStatus.wPETUnit3Set = state[44];  ///< 
	m_sHardwareStatus.wPETUnit3Set = WORD(m_sHardwareStatus.wPETUnit3Set << 8) + state[45];  ///< 

	m_sHardwareStatus.wPETUnit3tTemp = state[46];  ///<
	m_sHardwareStatus.wPETUnit3tTemp = WORD(m_sHardwareStatus.wPETUnit3tTemp << 8) + state[47];  ///<

	m_sHardwareStatus.nCapState = state[48];  ///<

	m_sHardwareStatus.wCapSet = state[49];  ///<
	m_sHardwareStatus.wCapSet = WORD(m_sHardwareStatus.wCapSet << 8) + state[50];  ///<

	m_sHardwareStatus.wCaptTemp = state[51];  ///<
	m_sHardwareStatus.wCaptTemp = WORD(m_sHardwareStatus.wCaptTemp << 8) + state[52];  ///<

	m_sHardwareStatus.nAuxerState = state[53];  ///<

	m_sHardwareStatus.wAuxerSet = state[54];  ///<
	m_sHardwareStatus.wAuxerSet = WORD(m_sHardwareStatus.wAuxerSet << 8) + state[55];  ///<

	m_sHardwareStatus.wFanTemp = state[56];  ///<
	m_sHardwareStatus.wFanTemp = WORD(m_sHardwareStatus.wFanTemp << 8) + state[57];  ///<

	m_sHardwareStatus.nFanState = state[58];  ///<

	m_sHardwareStatus.wFanSet = state[59];  ///<
	m_sHardwareStatus.wFanSet = WORD(m_sHardwareStatus.wFanSet << 8) + state[60];  ///<

	m_sHardwareStatus.wSysFanSet = state[61];  ///<
	m_sHardwareStatus.wSysFanSet = WORD(m_sHardwareStatus.wSysFanSet << 8) + state[62];  ///<

	m_sHardwareStatus.wSysFantTemp = state[63];  ///<
	m_sHardwareStatus.wSysFantTemp = WORD(m_sHardwareStatus.wSysFantTemp << 8) + state[64];  ///<

	m_sHardwareStatus.nA4940Rst = state[65];  ///<

	m_sHardwareStatus.wWKBBackground = state[66];  ///<
	m_sHardwareStatus.wWKBBackground = WORD(m_sHardwareStatus.wWKBBackground << 8) + state[67];  ///<

	m_sHardwareStatus.nAuxerHState = state[68];  ///<

	m_sHardwareStatus.wAuxerHSet = state[69];  ///<
	m_sHardwareStatus.wAuxerHSet = WORD(m_sHardwareStatus.wAuxerHSet << 8) + state[70];  ///<

	m_sHardwareStatus.nTransportlockState = state[71];  ///<

	m_sHardwareStatus.wPET1PWMOut = state[72];  ///<
	m_sHardwareStatus.wPET1PWMOut = WORD(m_sHardwareStatus.wPET1PWMOut << 8) + state[73];  ///<

	m_sHardwareStatus.wPET2PWMOut = state[74];  ///<
	m_sHardwareStatus.wPET2PWMOut = WORD(m_sHardwareStatus.wPET2PWMOut << 8) + state[75];  ///<

	m_sHardwareStatus.wPET3PWMOut = state[76];  ///<
	m_sHardwareStatus.wPET3PWMOut = WORD(m_sHardwareStatus.wPET3PWMOut << 8) + state[77];  ///<
}

// ���Ӳ��״̬��Ϣ��������־��
void CInstancePCR::OutputHardwareStatusToErrorLog()
{
	CString str;
	str.Format(_T("ErrorCode(Hex) :%02X,\r\n%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,\r\n%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,\r\n%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,\r\n%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,\r\n%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,%02X,"),
		m_sHardwareStatus.nDevRun, m_sHardwareStatus.nXMotState, m_sHardwareStatus.wXMotPos,
		m_sHardwareStatus.nXMotDirection, m_sHardwareStatus.nYMotState, m_sHardwareStatus.wYMotPos, m_sHardwareStatus.nYMotDirection, m_sHardwareStatus.nZMotState,
		m_sHardwareStatus.wZMotPos, m_sHardwareStatus.nZMotDirection, m_sHardwareStatus.nUMotState, m_sHardwareStatus.wUMotPos, m_sHardwareStatus.nUMotDirection,
		m_sHardwareStatus.nMotSensor, m_sHardwareStatus.nLED, m_sHardwareStatus.nPmtAnalogState, m_sHardwareStatus.nEeprom, m_sHardwareStatus.nSMBUart,
		m_sHardwareStatus.wPMT7512Out, m_sHardwareStatus.wSMBBackground, m_sHardwareStatus.nSysBox, m_sHardwareStatus.wScannertTemp, m_sHardwareStatus.nSMBParameterInit,
		m_sHardwareStatus.nWKBParaInit, m_sHardwareStatus.nSysStateLed, m_sHardwareStatus.nCurtTempCtrlMode_TubeBlockMode, m_sHardwareStatus.nPETUnit1State, m_sHardwareStatus.wPETUnit1Set,
		m_sHardwareStatus.wPETUnit1tTemp, m_sHardwareStatus.nPETUnit2State, m_sHardwareStatus.wPETUnit2Set, m_sHardwareStatus.wPETUnit2tTemp, m_sHardwareStatus.nPETUnit3State,
		m_sHardwareStatus.wPETUnit3Set, m_sHardwareStatus.wPETUnit3tTemp, m_sHardwareStatus.nCapState, m_sHardwareStatus.wCapSet, m_sHardwareStatus.wCaptTemp,
		m_sHardwareStatus.nAuxerState, m_sHardwareStatus.wAuxerSet, m_sHardwareStatus.wFanTemp, m_sHardwareStatus.nFanState, m_sHardwareStatus.wFanSet,
		m_sHardwareStatus.wSysFanSet, m_sHardwareStatus.wSysFantTemp, m_sHardwareStatus.nA4940Rst, m_sHardwareStatus.wWKBBackground, m_sHardwareStatus.nAuxerHState,
		m_sHardwareStatus.wAuxerSet, m_sHardwareStatus.nTransportlockState, m_sHardwareStatus.wPET1PWMOut, m_sHardwareStatus.wPET2PWMOut, m_sHardwareStatus.wPET3PWMOut);

	CLogManager* pLogManager = CLogManager::GetInstance();
	pLogManager->WriteErrorLog(str);
}

BOOL CInstancePCR::ReadLidTemperatureFromEEPROMPLCUsed()
{
	BYTE* pReceive = new BYTE[2];
	if (NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive, 0, sizeof(BYTE) * 2);
	if (ReadEPROM(0X550, 2, pReceive))
	{
		SetLidTem(*((USHORT*)pReceive));
	}
	else
	{
		delete[] pReceive;
		return FALSE;
	}

	delete[] pReceive;

	return TRUE;
}

BOOL CInstancePCR::ReadLidTemperatureFromEEPROMPLCUsed(double& dLidTem)
{
	BYTE* pReceive = new BYTE[2];
	if (NULL == pReceive)
	{
		return FALSE;
	}
	memset(pReceive, 0, sizeof(BYTE) * 2);
	if (ReadEPROM(0X550, 2, pReceive))
	{
		SetLidTem(*((USHORT*)pReceive));
	}
	else
	{
		delete[] pReceive;
		return FALSE;
	}

	dLidTem = *((USHORT*)pReceive);

	delete[] pReceive;

	return TRUE;
}

BOOL CInstancePCR::WriteLidTemperaturePLCUsed(float fLidTem)
{
	int iLidTem = fLidTem;

	if (iLidTem <= 0)
	{
		return FALSE;
	}

	if (!WriteEPROM(0X550, iLidTem, 2))
	{
		return FALSE;
	}

	if (!SetLidTem(iLidTem))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CInstancePCR::WriteLidPreheatTemperaturePLCUsed(float fLidTem)
{
	return FALSE;
}

BOOL CInstancePCR::ReadLidPreheatTemperaturePLCUsed(float& fLidPreheatTem)
{
	return FALSE;
}

int CInstancePCR::GetInitChanDyeName(vector<CString>& vecChanDyeName)
{
	vecChanDyeName.clear();
	int iChanCount = GetScanChannelCount();
	if (iChanCount < 1)
	{
		return -1;
	}

	for (int i = 0; i < iChanCount; ++i)
	{
		if (0 == i)
		{
			vecChanDyeName.push_back(_T("FAM"));
		}
		else if (1 == i)
		{
			vecChanDyeName.push_back(_T("Cy5"));
		}
		else if (2 == i)
		{
			vecChanDyeName.push_back(_T("VIC"));
		}
		else if (3 == i)
		{
			vecChanDyeName.push_back(_T("ROX"));
		}
		else
		{
			CString strTemp(_T(""));
			strTemp.Format(_T("%d"), i + 1);
			vecChanDyeName.push_back(strTemp);
		}
	}

	return 0;
}

int CInstancePCR::CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem, BOOL bUseLid, int iVolume)
{
	return 0;
}

BOOL CInstancePCR::IsDemo()
{
	return FALSE;
}

int CInstancePCR::GetMppcGainHigh()
{
	return m_iMppcGainH;
}

int CInstancePCR::GetMppcGainMiddle()
{
	return m_iMppcGainM;
}

int CInstancePCR::GetMppcGainLow()
{
	return m_iMppcGainL;
}

void CInstancePCR::SetInstanceStatus(eInstanceStatus eStatus)
{
	m_eInstanceStatus = eStatus;
}

eInstanceStatus CInstancePCR::GetInstanceStatus()
{
	return m_eInstanceStatus;
}

BOOL CInstancePCR::IsInstanceIdle()
{
	return INSTANCE_READY == m_eInstanceStatus;
}

float CInstancePCR::GetPwmK1()
{
	return m_fPwmK1;
}

float CInstancePCR::GetPwmB1()
{
	return m_fPwmB1;
}

float CInstancePCR::GetPwmK2()
{
	return m_fPwmK2;
}

float CInstancePCR::GetPwmB2()
{
	return m_fPwmB2;
}

float CInstancePCR::GetPwmK3()
{
	return m_fPwmK3;
}

float CInstancePCR::GetPwmB3()
{
	return m_fPwmB3;
}

void CInstancePCR::SetEndCoolTemperature(float fTem)
{
	m_fEndCoolTem = fTem;
}

float CInstancePCR::GetEndCoolTemperature()
{
	return m_fEndCoolTem;
}

void CInstancePCR::SetSleepMsAfterCool(int iMs)
{
	if (iMs < 0)
	{
		iMs = 0;
	}
	m_iMsSlpAfterCool = iMs;
}

int CInstancePCR::GetSleepMsAfterCool()
{
	return m_iMsSlpAfterCool;
}

void CInstancePCR::SetPWMInitWaitMs(int iMs)
{
	if (iMs < 0)
	{
		iMs = 0;
	}
	m_iMsPwmInitWait = iMs;
}

int CInstancePCR::GetPWMInitWaitMs()
{
	return m_iMsPwmInitWait;
}

void CInstancePCR::SaveParaForDemo(CString strIniFile)
{
	CString strValue;
	CString strAppName = _T("Instrument");

	::WritePrivateProfileString(strAppName, _T("PCRModel"), m_strModel, strIniFile);
	::WritePrivateProfileString(strAppName, _T("PCRVersion"), m_strVersion, strIniFile);

	strValue.Format(_T("%d"), m_sStructurePara.cTempMode);
	::WritePrivateProfileString(strAppName, _T("TempMode"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_sStructurePara.cTempCtrl);
	::WritePrivateProfileString(strAppName, _T("TempCtrlMode"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_sStructurePara.cLidHeader);
	::WritePrivateProfileString(strAppName, _T("HasLid"), strValue, strIniFile);

	strValue.Format(_T("%d"), m_sStructurePara.cPlateType);
	::WritePrivateProfileString(strAppName, _T("PlateType"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_sStructurePara.cPlateCount);
	::WritePrivateProfileString(strAppName, _T("PlateCount"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_sStructurePara.cPlateSortType);
	::WritePrivateProfileString(strAppName, _T("SortType"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_sStructurePara.cPlateRowCount);
	::WritePrivateProfileString(strAppName, _T("RowCount"), strValue, strIniFile);
	strValue.Format(_T("%d"), m_sStructurePara.cPlateColCount);
	::WritePrivateProfileString(strAppName, _T("ColCount"), strValue, strIniFile);

}

tagScanChannelInfo* CInstancePCR::GetScanChannelInfo(int iIndex)
{
	if (iIndex >= 0 && iIndex < m_arrayFilter.GetCount())
	{
		tagScanChannelInfo* pChannel = (tagScanChannelInfo*)m_arrayFilter.GetAt(iIndex);
		return pChannel;
	}
	return NULL;
}

int CInstancePCR::GetScanChannelInfo(CLinkList<tagScanChannelInfo>& lnkScanChanInfo)
{
	lnkScanChanInfo.Clear();
	int iFltNum = m_arrayFilter.GetSize();
	tagScanChannelInfo* pScanChanInfo = NULL;
	for (int i = 0; i < iFltNum; ++i)
	{
		pScanChanInfo = (tagScanChannelInfo*)m_arrayFilter.GetAt(i);
		if (NULL == pScanChanInfo)
		{
			continue;
		}

		lnkScanChanInfo.AddTail(*pScanChanInfo);
	}

	return 0;
}

eScanStartPos CInstancePCR::GetScanStartPos()
{
	eScanStartPos	nScanStartPos = SCAN_START_LT;
	if (m_sStructurePara.cScanOrigin == 1)
	{
		nScanStartPos = SCAN_START_RT;
	}

	return nScanStartPos;
}

void CInstancePCR::GetPlateInfo(tagPlateModeInfo* pInfo)
{
	pInfo->nPlateType = (ePlateModeType)m_sStructurePara.cPlateType;
	pInfo->iPlateCount = m_sStructurePara.cPlateCount;
	pInfo->nSortMode = (ePlateSortMode)m_sStructurePara.cPlateSortType;
	pInfo->iHoriCount = m_sStructurePara.cPlateColCount;
	pInfo->iVertCount = m_sStructurePara.cPlateRowCount;
}

ePCRTempMode CInstancePCR::GetTempMode()
{
	return ePCRTempMode(m_sStructurePara.cTempMode);
}

eTempCtrlMode CInstancePCR::GetTempCtrlMode()
{
	return eTempCtrlMode(m_sStructurePara.cTempCtrl);
}

// �Ƿ��н�����
BOOL CInstancePCR::HasBox()
{
	return BOOL(m_sStructurePara.cUseBox);
}

// �Ƿ����ȸ�
BOOL CInstancePCR::HasLid()
{
	return BOOL(m_sStructurePara.cLidHeader);
}

int CInstancePCR::GetPreMeltMinSleepSec()
{
	return m_iPreMeltMinSlpSec;
}

int CInstancePCR::SportModuleCheck()
{
	int iErrId = 0;

	BOOL bReturn = MotorReset(PCR_XMOT_RESET);
	if (!bReturn)
	{
		iErrId = -1;
		goto COMMU_FAILED;
	}

	bReturn = MotorReset(PCR_YMOT_RESET);
	if (!bReturn)
	{
		iErrId = -2;
		goto COMMU_FAILED;
	}

	return 0;

COMMU_FAILED:
	SetInstanceStatus(INSTANCE_SELFCHECK_FAIL);
	return iErrId;
}

int CInstancePCR::SensorModuleCheck()
{
	int iErrId = 0;
	float fTem = 0;
	BOOL bRet = FALSE, bTarget = FALSE;
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();
	CString strTemp(_T("")), strTemFormat(_T("%.2lf")), strUnit(_T(""));
	strUnit = pLangInfo->GetText(327);

	m_sensorMChkParam.ResetCheckResult();

	//�����¶ȴ�����
	if (m_sensorMChkParam.IsCheckEnvirTsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_ENVIRONMENT, fTem, bTarget))
		{
			iErrId = -1;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetEnvirTsmRefH())
		{
			strTemp = pLangInfo->GetText(319);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetEnvirTsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetEnvirTsmRefL())
		{
			strTemp = pLangInfo->GetText(319);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetEnvirTsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	//�ȳ��¶ȴ�����
	if (m_sensorMChkParam.IsCheckHeatSinkTsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_HEATSINK, fTem, bTarget))
		{
			iErrId = -2;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetHeatSinkTsmRefH())
		{
			strTemp = pLangInfo->GetText(320);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetHeatSinkTsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetHeatSinkTsmRefL())
		{
			strTemp = pLangInfo->GetText(320);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetHeatSinkTsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	//�ȸ��¶ȴ�����
	if (m_sensorMChkParam.IsCheckLidTsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_LID, fTem, bTarget))
		{
			iErrId = -3;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetHeatSinkTsmRefH())
		{
			strTemp = pLangInfo->GetText(321);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetLidTsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetHeatSinkTsmRefL())
		{
			strTemp = pLangInfo->GetText(321);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetLidTsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	//������1�¶ȴ�����
	if (m_sensorMChkParam.IsCheckPelt1Tsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_PET1, fTem, bTarget))
		{
			iErrId = -4;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetPelt1TsmRefH())
		{
			strTemp = pLangInfo->GetText(322);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPelt1TsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetPelt1TsmRefL())
		{
			strTemp = pLangInfo->GetText(322);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPelt1TsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	//������2�¶ȴ�����
	if (m_sensorMChkParam.IsCheckPelt2Tsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_PET2, fTem, bTarget))
		{
			iErrId = -5;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetPelt2TsmRefH())
		{
			strTemp = pLangInfo->GetText(323);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPelt2TsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetPelt2TsmRefL())
		{
			strTemp = pLangInfo->GetText(323);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPelt2TsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	//������3�¶ȴ�����
	if (m_sensorMChkParam.IsCheckPelt3Tsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_PET3, fTem, bTarget))
		{
			iErrId = -6;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetPelt3TsmRefH())
		{
			strTemp = pLangInfo->GetText(324);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPelt3TsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetPelt3TsmRefL())
		{
			strTemp = pLangInfo->GetText(324);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPelt3TsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	//��Դ�¶ȴ�����
	if (m_sensorMChkParam.IsCheckPowerTsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_POWER, fTem, bTarget))
		{
			iErrId = -7;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetPowerTsmRefH())
		{
			strTemp = pLangInfo->GetText(325);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPowerTsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetPowerTsmRefL())
		{
			strTemp = pLangInfo->GetText(325);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetPowerTsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	//ɨ���¶ȴ�����
	if (m_sensorMChkParam.IsCheckScanTsm())
	{
		if (!ReadChannelTemp(TEMP_CHANNEL_SCAN, fTem, bTarget))
		{
			iErrId = -8;
			goto COMMU_FAILED;
		}
		if (fTem > m_sensorMChkParam.GetScanTsmRefH())
		{
			strTemp = pLangInfo->GetText(326);
			strTemp += pLangInfo->GetText(317);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetScanTsmRefH());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
		else if (fTem < m_sensorMChkParam.GetScanTsmRefL())
		{
			strTemp = pLangInfo->GetText(326);
			strTemp += pLangInfo->GetText(318);
			strTemp.AppendFormat(strTemFormat, m_sensorMChkParam.GetScanTsmRefL());
			m_sensorMChkParam.AddCheckResult(strTemp);
		}
	}

	if (!m_sensorMChkParam.IsSuccessful())
	{
		return 1;
	}

	return 0;

COMMU_FAILED:
	SetInstanceStatus(INSTANCE_SELFCHECK_FAIL);
	return iErrId;
}

int CInstancePCR::HeatModeduleCheck()
{
	if (!m_heatModuleChkParam.IsNeedCheck())
	{
		return 1;
	}

	int iErrId = 0;
	BYTE byChanId = 5;
	BOOL bTarget = FALSE;
	int iSlpSec = 0, iSlpSec2 = 0;
	double dTemError = 0, dTemError2 = 0;
	float fTem = 0, fSetTem = 0, fOriginTem = 0, fLastTem = 0, fTemPeltierNotWork = 200, fTemPeltier1 = 0, fTemPeltier2 = 0, fTemPeltier3 = 0, fTemPublic = 0;
	CString strFormat(_T(",%.2lf-%.2lf,")), strTemp(_T(""));
	CPeltierCheckParam* pPeltierChkParam = NULL;
	CLanguageInfo* pLangInfo = CLanguageInfo::GetInstance();

	m_heatModuleChkParam.ResetCheckResult();

	//�����������¼��
	for (int i = 0; i < 3; ++i)
	{
		pPeltierChkParam = m_heatModuleChkParam.GetPeltierCheckParam(0);
		if (NULL != pPeltierChkParam && pPeltierChkParam->IsCheck())
		{
			//��ȡ��ǰ�¶�
			if (!ReadChannelTemp(++byChanId, fTem, bTarget))
			{
				iErrId = -1;
				goto COMMU_FAILED;
			}
			fOriginTem = fTem;

			//��ȡ���ºͽ��²���
			if (fTem > pPeltierChkParam->GetTemperatureThreshold())
			{
				iSlpSec = pPeltierChkParam->GetDropTimeSec();
				dTemError = pPeltierChkParam->GetDropTemError();
				fSetTem = fTem - pPeltierChkParam->GetTemRiseDropRange();
				iSlpSec2 = pPeltierChkParam->GetRiseTimeSec();
				dTemError2 = pPeltierChkParam->GetRiseTemError();
			}
			else
			{
				iSlpSec = pPeltierChkParam->GetRiseTimeSec();
				dTemError = pPeltierChkParam->GetRiseTemError();
				fSetTem = fTem + pPeltierChkParam->GetTemRiseDropRange();
				iSlpSec2 = pPeltierChkParam->GetDropTimeSec();
				dTemError2 = pPeltierChkParam->GetDropTemError();
			}

			if (0 == i)
			{
				fTemPeltier1 = fSetTem;
				fTemPeltier2 = fTemPeltierNotWork;
				fTemPeltier3 = fTemPeltierNotWork;
			}
			else if (1 == i)
			{
				fTemPeltier1 = fTemPeltierNotWork;
				fTemPeltier2 = fSetTem;
				fTemPeltier3 = fTemPeltierNotWork;
			}
			else if (2 == i)
			{
				fTemPeltier1 = fTemPeltierNotWork;
				fTemPeltier2 = fTemPeltierNotWork;
				fTemPeltier3 = fSetTem;
			}

			//����Ŀ���¶ȣ����»�������
			if (!SetMultiTemp(fTemPeltier1, fTemPeltier2, fTemPeltier3, 0, TEMP_CTRL_NORMAL, TRUE))
			{
				iErrId = -2;
				goto COMMU_FAILED;
			}

			//��ʱ�ȴ�
			/*for(int j = 0;j < iSlpSec;++j)
			{
				if(!ReadChannelTemp(byChanId,fTemPublic,bTarget))
				{
					iErrId = -10;
					goto COMMU_FAILED;
				}

				if(abs(fTemPublic - fSetTem) <= dTemError)
				{
					break;
				}

				Sleep(1);
			}*/

			//�ٴλ�ȡ��ǰ�¶�
			if (!ReadChannelTemp(byChanId, fTem, bTarget))
			{
				iErrId = -3;
				goto COMMU_FAILED;
			}
			fLastTem = fTem;

			/*if(!SetMultiTemp(655.35,655.35,655.35,0,TEMP_CTRL_NORMAL))
			{
			iErrId = -2;
			goto COMMU_FAILED;
			}*/

			//�ж��ض�ʱ�����Ƿ񵽴�Ŀ���¶��ض���Χ��
			if (abs(fTem - fSetTem) > dTemError)
			{
				if (0 == i)
				{
					strTemp = pLangInfo->GetText(322);
				}
				else if (1 == i)
				{
					strTemp = pLangInfo->GetText(323);
				}
				else if (2 == i)
				{
					strTemp = pLangInfo->GetText(324);
				}
				strTemp.AppendFormat(strFormat, fOriginTem, fSetTem);
				strTemp += pLangInfo->GetText(329);
				m_heatModuleChkParam.AddCheckResult(strTemp);
			}


			if (0 == i)
			{
				fTemPeltier1 = fOriginTem;
				fTemPeltier2 = fTemPeltierNotWork;
				fTemPeltier3 = fTemPeltierNotWork;
			}
			else if (1 == i)
			{
				fTemPeltier1 = fTemPeltierNotWork;
				fTemPeltier2 = fOriginTem;
				fTemPeltier3 = fTemPeltierNotWork;
			}
			else if (2 == i)
			{
				fTemPeltier1 = fTemPeltierNotWork;
				fTemPeltier2 = fTemPeltierNotWork;
				fTemPeltier3 = fOriginTem;
			}

			//�ٴ�����Ŀ���¶ȣ����»��߽���
			if (!SetMultiTemp(fTemPeltier1, fTemPeltier2, fTemPeltier3, 0, TEMP_CTRL_NORMAL, TRUE))
			{
				iErrId = -4;
				goto COMMU_FAILED;
			}

			//��ʱ�ȴ�
			/*for(int j = 0;j < iSlpSec2;++j)
			{
				if(!ReadChannelTemp(byChanId,fTemPublic,bTarget))
				{
					iErrId = -11;
					goto COMMU_FAILED;
				}

				if(abs(fTemPublic - fOriginTem) <= dTemError)
				{
					break;
				}

				Sleep(1);
			}*/

			//�ٴλ�ȡ��ǰ�¶�
			if (!ReadChannelTemp(byChanId, fTem, bTarget))
			{
				iErrId = -5;
				goto COMMU_FAILED;
			}

			//�ж��ض�ʱ�����Ƿ񵽴�Ŀ���¶��ض���Χ��
			/*if(abs(fTem - fOriginTem) > dTemError2)
			{
				if(0 == i)
				{
					strTemp = pLangInfo->GetText(322);
				}
				else if(1 == i)
				{
					strTemp = pLangInfo->GetText(323);
				}
				else if(2 == i)
				{
					strTemp = pLangInfo->GetText(324);
				}
				strTemp.AppendFormat(strFormat,fLastTem,fOriginTem);
				strTemp += pLangInfo->GetText(329);
				m_heatModuleChkParam.AddCheckResult(strTemp);
			}*/
		}
	}

	//�Լ����ʱ����ȡ��ǰ�¶��ж��費��Ҫ����
	if (!ReadPCRTemp(fTem, bTarget))
	{
		iErrId = -6;
		goto COMMU_FAILED;
	}
	if (fTem > 45)
	{
		if (!SetMultiTemp(40, 40, 40, 4, 0))
		{
			iErrId = -7;
			goto COMMU_FAILED;
		}
		Sleep(10);
	}

	//�ͷ��������¿�
	if (!SetMultiTemp(PCR_TEMP_RELEASE, PCR_TEMP_RELEASE, PCR_TEMP_RELEASE, 0, TEMP_CTRL_NORMAL))
	{
		iErrId = -8;
		goto COMMU_FAILED;
	}

#ifdef _DEBUG
	/*if(!ReadHardwareStatus())
	{
	iErrId = -9;
	goto COMMU_FAILED;
	}*/
#endif

	if (!m_heatModuleChkParam.IsSuccessful())
	{
		return 2;
	}

	return 0;

COMMU_FAILED:
	SetInstanceStatus(INSTANCE_SELFCHECK_FAIL);
	return iErrId;
}

int CInstancePCR::OpticsModuleCheck()
{
	int iErrId = 0;

	//������LED��
	if (!SetLaserStatus(2, 0XFF))
	{
		iErrId = -1;
		goto COMMU_FAILED;
	}

	Sleep(30);

	//�ر�����LED��
	if (!SetLaserStatus(2, 0))
	{
		iErrId = -2;
		goto COMMU_FAILED;
	}

	return 0;

COMMU_FAILED:
	SetInstanceStatus(INSTANCE_SELFCHECK_FAIL);
	return iErrId;
}

CString CInstancePCR::GetCheckResultString()
{
	CString strTemp(_T(""));

	if (!m_sensorMChkParam.IsSuccessful())
	{
		strTemp += m_sensorMChkParam.GetCheckResult();
		strTemp += _T("\n");
	}

	if (!m_heatModuleChkParam.IsSuccessful())
	{
		strTemp += m_heatModuleChkParam.GetCheckResult();
		strTemp += _T("\n");
	}

	return strTemp;
}

BOOL CInstancePCR::ReadDeviceParamsFromEPROM()
{
	BYTE* pReceive = new BYTE[SIZE_DEVICE_PARAM];
	memset(pReceive, 0, sizeof(BYTE) * SIZE_DEVICE_PARAM);

	UINT nAddress = 0;

#ifdef _DEBUG
	clock_t ckStart = clock();
#endif

	BOOL bReturn = ReadLongEPROM(nAddress, SIZE_DEVICE_PARAM, pReceive);

#ifdef _DEBUG
	clock_t ckEnd = clock() - ckStart;
#endif

	if (bReturn)
	{
		memcpy(&m_sDevicePara, pReceive, SIZE_DEVICE_PARAM);
	}

	delete[] pReceive;
	pReceive = nullptr;
	return bReturn;
}

BOOL CInstancePCR::ReadStructureParamsFromEPROM()
{
	UINT nLength = 0x200;
	UINT nAddress = 0x2000;
	BYTE* pReceive = new BYTE[nLength];
	memset(pReceive, 0, sizeof(BYTE) * nLength);

	BOOL bReturn = ReadLongEPROM(nAddress, nLength, pReceive);
	if (bReturn)
	{
		BYTE cUsePara = BOOL(*pReceive);
		if (cUsePara == 1)
		{
			memcpy(&m_sStructurePara, pReceive + 1, nLength - 1);
		}
		else
		{
			//��ȡ�ȸ��¶�
			nAddress = 0X2027;
			if (ReadEPROM(nAddress, 1, pReceive)) // 0X550
			{
				m_sStructurePara.cLidTempTop = *pReceive;
				if (m_sStructurePara.cLidTempTop == 255 || m_sStructurePara.cLidTempTop == 0)
				{
					m_sStructurePara.cLidTempTop = 90;
				}
				bReturn = TRUE;
			}
		}
	}

	//��ȡ����ʵ��ʹ�õ����ȸ��¶ȣ�EEPROM������д�뵽0X2027��ַ
	if (bReturn)
	{
		bReturn = ReadLidTemperatureFromEEPROMPLCUsed();
	}

	delete[] pReceive;
	pReceive = NULL;

	return bReturn;
}

// ��ȡָ����ַ�ͳ��ȵ�EEPROM����
BOOL CInstancePCR::ReadEPROM(UINT nAddress, BYTE cGetLength, BYTE* pReceive)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN9, PCR_RDEE, nAddress, cGetLength);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN9, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		memcpy(pReceive, &pStatus[4], iReceiveFrameLen - 6);
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_READ_EEPROM_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_READ_EEPROM_TIMEOUT;
		}

		m_iErrorPara1 = nAddress;
		m_iErrorPara1 = cGetLength;
	}

	delete pCommand;
	if (pStatus) delete[] pStatus;

	return bReturn;
}

BOOL CInstancePCR::WriteEPROM(UINT nAddress, int iData, int iDataLen)
{
	if (iDataLen < 1)
	{
		return FALSE;
	}

	int iCmdLen = 8 + iDataLen;
	BYTE* pCommand = new BYTE[iCmdLen];
	if (NULL == pCommand)
	{
		return FALSE;
	}
	memset(pCommand, 0, iCmdLen);

	pCommand[0] = PCR_DNSEND;//֡ͷ
	pCommand[1] = PCR_WDEE; //������
	pCommand[3] = iCmdLen; //����
	pCommand[4] = (BYTE)((nAddress & 0xFF0000) >> 16);
	pCommand[5] = (BYTE)((nAddress & 0xFF00) >> 8);
	pCommand[6] = (BYTE)(nAddress & 0xFF);

	for (int i = 0; i < iDataLen; ++i)
	{
		pCommand[7 + i] = BYTE((iData >> (i * 8)) & 0XFF);
	}
	pCommand[iCmdLen - 1] = m_pComPort->GetByteSum(pCommand, iCmdLen - 1);//У���

	int iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, iCmdLen, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		bReturn = TRUE;
	}
	else
	{
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_READ_EEPROM_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_READ_EEPROM_TIMEOUT;
		}

		m_iErrorPara1 = nAddress;
		m_iErrorPara1 = iDataLen;
	}

	delete[] pCommand;
	delete[] pStatus;

	return bReturn;
}

int CInstancePCR::GetLidTem()
{
	return m_sStructurePara.cLidTempTop;
}

BOOL CInstancePCR::SetLidTem(int iLidTem)
{
	m_sStructurePara.cLidTempTop = BYTE(iLidTem);
	BOOL bReturn = WriteEPROM(0X2027, BYTE(iLidTem), 1);

	return bReturn;
}

// ��ȡ����һ�δ��䳤�ȵ�����
BOOL CInstancePCR::ReadLongEPROM(UINT nAddress, UINT nGetLength, BYTE* pReceive)
{
	BOOL bReturn = FALSE;
	UINT nEachLen = 249;
	int  iReadNum = nGetLength / nEachLen;
	UINT nRemain = nGetLength % nEachLen;

	for (int i = 0; i < iReadNum; i++)
	{
		bReturn = ReadEPROM(nAddress, nEachLen, &pReceive[i * nEachLen]);
		if (!bReturn)
		{
			delete[] pReceive;
			pReceive = nullptr;
			return bReturn;
		}
		nAddress += nEachLen;
	}

	if (nRemain > 0)
	{
		bReturn = ReadEPROM(nAddress, nRemain, &pReceive[nEachLen * iReadNum]);
	}

	return bReturn;
}

BOOL CInstancePCR::Reinitialize()
{
	BOOL bReturn = m_pComPort->Reset();
	if (bReturn)
	{
		bReturn = Hello();
	}
	return bReturn;
}


BOOL CInstancePCR::ReadAllTemp(int iTempCount, float* pTemp, BOOL& bToTarget)
{
	BYTE* pCommand = NewSendCommand(CMD_SEND_LEN5, PCR_READ_SIX_TEMP, 0);
	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = m_pComPort->SendCommandRepeatedly(pCommand, CMD_SEND_LEN5, &pStatus, iReceiveFrameLen);
	BOOL bReturn = FALSE;
	if (CSerialPortPCR::RETURN_OK == iReturn)
	{
		ASSERT(19 == iReceiveFrameLen);

		int Temp = 0;
		BOOL bTargetVal = FALSE;

		Temp = pStatus[iReceiveFrameLen - 15];
		Temp = Temp << 8;
		Temp &= 0xFF00;
		Temp |= pStatus[iReceiveFrameLen - 14];
		pTemp[0] = (float)(Temp / RESOLUTION_TEMP);

		Temp = pStatus[iReceiveFrameLen - 13];
		Temp = Temp << 8;
		Temp &= 0xFF00;
		Temp |= pStatus[iReceiveFrameLen - 12];
		pTemp[1] = (float)(Temp / RESOLUTION_TEMP);

		Temp = pStatus[iReceiveFrameLen - 11];
		Temp = Temp << 8;
		Temp &= 0xFF00;
		Temp |= pStatus[iReceiveFrameLen - 10];
		pTemp[2] = (float)(Temp / RESOLUTION_TEMP);

		Temp = pStatus[iReceiveFrameLen - 9];
		Temp = Temp << 8;
		Temp &= 0xFF00;
		Temp |= pStatus[iReceiveFrameLen - 8];
		pTemp[3] = (float)(Temp / RESOLUTION_TEMP);

		Temp = pStatus[iReceiveFrameLen - 7];
		Temp = Temp << 8;
		Temp &= 0xFF00;
		Temp |= pStatus[iReceiveFrameLen - 6];
		pTemp[4] = (float)(Temp / RESOLUTION_TEMP);

		Temp = pStatus[iReceiveFrameLen - 5];
		Temp = Temp << 8;
		Temp &= 0xFF00;
		Temp |= pStatus[iReceiveFrameLen - 4];
		pTemp[5] = (float)(Temp / RESOLUTION_TEMP);

		bTargetVal = pStatus[iReceiveFrameLen - 3];

		bReturn = TRUE;
	}
	else
	{
		m_iErrorPara1 = 0; // ��Чֵ
		m_iErrorPara2 = 0;
		if (iReturn == CSerialPortPCR::RETURN_FAIL)
		{
			m_uiErrorCode = ERR_PCR_TEMP_READ_FAIL;
		}
		else if (iReturn == CSerialPortPCR::RETURN_TIMEOUT)
		{
			m_uiErrorCode = ERR_PCR_TEMP_READ_TIMEOUT;
		}
	}

	if (NULL != pCommand)
	{
		delete[] pCommand;
	}

	if (NULL != pStatus)
	{
		delete[] pStatus;
	}

	return bReturn;
}

BOOL CInstancePCR::CanRunExp()
{
	return m_eInstanceStatus != INSTANCE_TEMP_LOW && m_eInstanceStatus != INSTANCE_TEMP_HIGH
		&& m_eInstanceStatus != INSTANCE_FLUO_LOW && m_eInstanceStatus != INSTANCE_FLUO_HIGH;
}

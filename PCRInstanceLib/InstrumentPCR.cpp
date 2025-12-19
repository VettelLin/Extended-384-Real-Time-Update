#include "StdAfx.h"
#include "InstrumentPCR.h"
#include "InstancePS96.h"
#include "ErrorCodeDefine.h"
#include "InstanceDemo.h"
#include "LogManager.h"
#include "InstancePCR96.h"
#include "InstanceN16.h"
#include "InstanceM16.h"
#include "Define.h"

CInstrumentPCR* CInstrumentPCR::m_pInstrumentPCR = NULL;

CInstrumentPCR::CInstrumentPCR(void)
{
	m_strStartPath.Empty();
	m_uiErrorCode = 0;
	m_iErrorPara1 = 0; 
	m_iErrorPara2 = 0;

	m_iBandRate = 9600;
	m_iResetTime = 15;
	m_iSTimeOutBase = 500;
}

CInstrumentPCR::~CInstrumentPCR(void)
{
	Uninitialize();
}

CInstrumentPCR* CInstrumentPCR::GetInstance(void)
{
	if (m_pInstrumentPCR == NULL)
	{
		m_pInstrumentPCR = new CInstrumentPCR;
	}
	return m_pInstrumentPCR;
}

void CInstrumentPCR::CleanUp()
{
	SAFE_DELETE(m_pInstrumentPCR);
	m_pInstrumentPCR = NULL;
}

BOOL CInstrumentPCR::Initialize(CString strStartPath, BOOL bRecognizeAll)
{
	if (!strStartPath.IsEmpty())
	{
		m_strStartPath = strStartPath;
	}
	if (m_strStartPath.IsEmpty())
		return FALSE;

	BOOL bReturn = Reinitialize(bRecognizeAll);
	return bReturn;
}

// 重新连接串口查找连接的PCR设备
BOOL CInstrumentPCR::Reinitialize(BOOL bRecognizeAll)
{
	BOOL bHasDemo = FALSE;
	BOOL bReturn = FALSE;
	if (bRecognizeAll)
	{
		Uninitialize();
	}
	else
	{
		int nInstanceCount = m_arrayInstancePCR.GetCount();
		for (int i=nInstanceCount-1; i>=0; i--)
		{
			tagInstancePara* pInstancePara = (tagInstancePara*)m_arrayInstancePCR.GetAt(i);
			if (pInstancePara == NULL)
			{
				m_arrayInstancePCR.RemoveAt(i);
				continue;
			}

			if (pInstancePara->pInstance->IsDemo())
			{
				bHasDemo = TRUE;
				continue;
			}

			if (pInstancePara->pInstance->GetInstanceStatus() == INSTANCE_USING)
				continue;

			SAFE_DELETE(pInstancePara->pInstance);
			delete pInstancePara;
			m_arrayInstancePCR.RemoveAt(i);
		}
	}

	CStringArray arrayComPort;
	GetComPort(arrayComPort);

	int iComCount = arrayComPort.GetCount();
	if (iComCount == 0)
	{
		m_uiErrorCode = ERR_COM_INIT_FAIL;
		return FALSE;
	}

	CSerialPortPCR* pComPort = NULL;
	tagInstancePara* pInstancePara = NULL;

	BOOL bGetModel = FALSE;
	CString strModel, strVersion;
	for (int i=0; i<iComCount; i++)
	{
		CString strPort = arrayComPort.GetAt(i);
		if (!bRecognizeAll)
		{
			if (PortHasUsed(strPort))
			{
				continue;
			}
		}

		if (pComPort == NULL)
		{
			pComPort = new CSerialPortPCR;
		}

		//打开串口
		if (!pComPort->OpenCom(strPort)) 
		{
			continue ;
		}
		//设置串口
		if (!pComPort->SettingCom(m_iBandRate)) 
		{
			continue ;
		}
		
		if (pInstancePara == NULL)
		{
			pInstancePara = new tagInstancePara;
		}
		
		pInstancePara->strPort = strPort;
		// 读取仪器类型参数
		bReturn = ReadModelFromEEPROM(pComPort,pInstancePara);
		if (bReturn)
		{
			CInstancePCR* pInstance = CreateInstancePCR(pComPort, pInstancePara);
			if (pInstance != NULL)
			{
				pInstancePara->pInstance = pInstance;
				m_arrayInstancePCR.Add(pInstancePara);
				pComPort = NULL;
				pInstancePara = NULL;
			}
		}
		else
		{
			pComPort->CloseCom();
			continue;
		}
	}

	SAFE_DELETE(pComPort);
	SAFE_DELETE(pInstancePara);

	if (!bReturn && GetInstanceCount() > 0)
	{
		bReturn = TRUE;
	}

	if (bHasDemo)
	{
		if (GetInstanceCount() > 1)
		{
			int nInstanceCount = m_arrayInstancePCR.GetCount();
			for (int i=nInstanceCount-1; i>=0; i--)
			{
				tagInstancePara* pInstancePara = (tagInstancePara*)m_arrayInstancePCR.GetAt(i);
				if (pInstancePara->pInstance->IsDemo())
				{
					SAFE_DELETE(pInstancePara->pInstance);
					delete pInstancePara;
					m_arrayInstancePCR.RemoveAt(i);
				}
			}
		}
		else if (GetInstanceCount() == 1)
		{
			bReturn = FALSE;
		}
	}

	return bReturn;
}

CInstancePCR* CInstrumentPCR::CreateInstancePCR(CSerialPortPCR* pComPort, tagInstancePara* pInputPara)
{
	CInstancePCR* pInstance = NULL;
	if (pInputPara->strCode.IsEmpty())
	{
		if (-1 != pInputPara->strModel.Find(PCR_MODEL_IAMP_PS96))
		{
			pInstance = new CInstancePS96(pComPort, pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
		}
	}
	else
	{	
		if(_T("N16") == pInputPara->strModel.Left(3) || _T("nQ16") == pInputPara->strModel.Left(4)) // BioWe N16
		{
			pInputPara->strCode = PCR_CODE_N16_STANDARD;
			pInstance = new CInstanceN16(pComPort, pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
		}
		else if (pInputPara->strCode == PCR_CODE_IAMP_PS96)
		{
			pInstance = new CInstancePS96(pComPort, pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
		}
		else if (pInputPara->strCode == PCR_CODE_QPCR_STANDARD) // BioWe QPCR
		{
			pInstance = new CInstancePCR96(pComPort, pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
		}
		else if (pInputPara->strCode == PCR_CODE_N16_STANDARD) // BioWe N16
		{
			pInstance = new CInstanceN16(pComPort, pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
		}
		else if (pInputPara->strCode == PCR_CODE_M16_XINYI) // XinYi M16
		{
			pInstance = new CInstanceM16(pComPort, pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
		}
		else
		{ 
#ifdef PS96INSTRUMENT
			//默认支持的型号，可设置，当前是 新羿M16
			pInputPara->strCode = PCR_CODE_IAMP_PS96;
			pInstance = new CInstancePS96(pComPort,pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
#else

#ifdef N96INSTRUMENT
			//默认支持的型号，可设置，当前是 新羿M16
			pInputPara->strCode = PCR_CODE_QPCR_STANDARD;
			pInstance = new CInstancePCR96(pComPort,pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
#else
			//默认支持的型号，可设置，当前是 新羿M16
			pInputPara->strCode = PCR_CODE_M16_XINYI;
			pInstance = new CInstanceM16(pComPort,pInputPara->strVersion);
			pInstance->SetStartPath(m_strStartPath);
			pInstance->Initialize();
#endif
			
#endif 
		}
	}

	if (pInstance != NULL)
	{
		pInstance->SetModel(pInputPara->strModel);
		pInstance->SetSN(pInputPara->strSN);
		pInstance->SetCode(pInputPara->strCode);
		pInstance->SetSoftwareVersion(pInputPara->strSoftware);
	}

	return  pInstance;
}

void CInstrumentPCR::Uninitialize()
{
	int nInstanceCount = m_arrayInstancePCR.GetCount();
	for (int i=0; i<nInstanceCount; i++)
	{
		tagInstancePara* pInstancePara = (tagInstancePara*)m_arrayInstancePCR.GetAt(i);
		if (pInstancePara != NULL)
		{
			SAFE_DELETE(pInstancePara->pInstance);
			delete pInstancePara;
			pInstancePara = NULL;
		}
	}

	m_arrayInstancePCR.RemoveAll();
}

UINT CInstrumentPCR::GetErrorCode(int &iErrPara1, int &iErrPara2)
{
	iErrPara1 = m_iErrorPara1;
	iErrPara2 = m_iErrorPara2;
	return m_uiErrorCode;
}

BOOL CInstrumentPCR::ReadModelFromEEPROM(CSerialPortPCR* pComPort, tagInstancePara* pInsPara)
{
	BYTE cGetLength = 16;
	BYTE* pReceiveData = NULL;
	
	// 读取设备型号编码和硬件版本号
	UINT nAddress = 0x20;
	BOOL bReturn = ReadEPROM(pComPort, nAddress, cGetLength, &pReceiveData);
	if(!bReturn)
	{
		SAFE_DELETE(pReceiveData);
		return FALSE;
	}
	SaveCodeToPara(pInsPara, pReceiveData+4, 0);
	SAFE_DELETE(pReceiveData);

	// 读取仪器型号
	nAddress = 0x00;
	bReturn = ReadEPROM(pComPort, nAddress, cGetLength, &pReceiveData);
	if (!bReturn)
	{
		SAFE_DELETE(pReceiveData);
		return FALSE;
	}
	SaveCodeToPara(pInsPara, pReceiveData+4, 1);
	SAFE_DELETE(pReceiveData);

	// 读取SN
	nAddress = 0x10;
	cGetLength = 16;
	bReturn = ReadEPROM(pComPort, nAddress, cGetLength, &pReceiveData);
	if (bReturn)
	{
		SaveCodeToPara(pInsPara, pReceiveData+4, 2);
	}
	SAFE_DELETE(pReceiveData);

	return bReturn;
}

void CInstrumentPCR::SaveCodeToPara(tagInstancePara* pInsPara, BYTE* pReceiveData, int iFlag)
{
	char cModel[17];
	memset(cModel, 0, 17);
	memcpy(cModel, pReceiveData, 16);

	if (iFlag == 0)
	{
		pInsPara->strCode = cModel;
		pInsPara->strCode.TrimRight();
		if(!pInsPara->strCode.IsEmpty())
		{
			CString strCode = pInsPara->strCode.Left(4);
			pInsPara->strVersion = pInsPara->strCode.Mid(4, 6);
			pInsPara->strSoftware = pInsPara->strCode.Mid(10, 6);
			pInsPara->strCode = strCode;
		}
	}
	else if (iFlag == 1)
	{
		pInsPara->strModel = cModel;
		pInsPara->strModel.TrimRight();
	}
	else if (iFlag == 2)
	{
		pInsPara->strSN = cModel;
		pInsPara->strSN.TrimRight();
	}
}


BOOL CInstrumentPCR::ReadEPROM(CSerialPortPCR* pComPort, UINT nAddress, BYTE &cGetLength,BYTE** pReceive)
{
	BOOL bReturn = FALSE;
	int nCmdLength = CMD_SEND_LEN9;
	BYTE* cCmd = new BYTE[nCmdLength];
	memset(cCmd, 0, nCmdLength);
	cCmd[0] = PCR_DNSEND;
	cCmd[1] = PCR_RDEE;	
	cCmd[3] = nCmdLength;
	cCmd[4] = (BYTE)((nAddress & 0xFF0000)>> 16);//地址
	cCmd[5] = (BYTE)((nAddress & 0xFF00)>> 8);
	cCmd[6] = (BYTE)(nAddress & 0xFF);
	cCmd[7] =  cGetLength;//读取字节数
	cCmd[nCmdLength-1] = pComPort->GetByteSum(cCmd, nCmdLength-1);

	int   iReceiveFrameLen = 0;
	int iReturn = pComPort->SendCommandRepeatedly(cCmd, nCmdLength, pReceive, iReceiveFrameLen, 1);
	if (iReturn == CSerialPortPCR::RETURN_OK)
	{
		cGetLength = iReceiveFrameLen;
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

	delete [] cCmd;

	return bReturn;
}


//程序启动时或者用户选择时获取全部可用串口
void CInstrumentPCR::GetComPort(CStringArray& ComArray)
{
	ComArray.RemoveAll();
	HANDLE  hCom;
	CString str;
	for (int i = 1;i <= 256;i++)		//此程序支持256个串口
	{
		if (i<=9)
		{
			str.Format(_T("COM%d"),i);
		}
		else
		{
			str.Format(_T("\\\\.\\COM%d"),i);
		}
		hCom = CreateFile(str, 0, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
		if(INVALID_HANDLE_VALUE != hCom )
		{//能打开该串口，则添加该串口
			CloseHandle(hCom);
			ComArray.Add(str);
		}
	}
}

CInstancePCR* CInstrumentPCR::GetInstancePCR(CString strModel, CString strSN)
{
	CInstancePCR* pInstance = NULL;

	if (strModel.IsEmpty() || strSN.IsEmpty())
	{
		if(m_arrayInstancePCR.GetSize() > 0)
		{
			pInstance = ((tagInstancePara*)m_arrayInstancePCR.GetAt(0))->pInstance;
		}
	}
	else
	{
		int iCount = m_arrayInstancePCR.GetCount();
		for (int i=0; i<iCount; i++)
		{
			tagInstancePara* pPara = (tagInstancePara*)m_arrayInstancePCR.GetAt(i);
			if (pPara == NULL)
			{
				ASSERT(FALSE);
				continue;
			}
			if (pPara->strModel == strModel && pPara->strSN == strSN)
			{
				pInstance = pPara->pInstance;
				break;
			}
		}
	}
	
	return pInstance;
}

CInstancePCR* CInstrumentPCR::GetInstancePCR(int iIndex)
{
	CInstancePCR* pInstance = NULL;
	if (iIndex >=0 && iIndex < GetInstanceCount())
	{
		tagInstancePara* pPara = (tagInstancePara*)m_arrayInstancePCR.GetAt(iIndex);
		pInstance = pPara->pInstance;
	}

	return pInstance;
}


// 返回可以使用的PCR实例，如果没有可用的，则返回正在使用的第一个实例
CInstancePCR* CInstrumentPCR::GetAvailableInstance()
{
	CInstancePCR* pRunningInstance = NULL;
	CInstancePCR* pAvailableInstance = NULL;

	for (int i=0; i<m_arrayInstancePCR.GetCount(); i++)
	{
		tagInstancePara* pPara = (tagInstancePara*)m_arrayInstancePCR.GetAt(i);
		if (pPara != NULL)
		{
			if (pPara->pInstance->GetInstanceStatus()== INSTANCE_USING && pRunningInstance == NULL)
			{
				pRunningInstance = pPara->pInstance;
			}
			else if (pPara->pInstance->GetInstanceStatus() != INSTANCE_USING && pAvailableInstance == NULL)
			{
				pAvailableInstance = pPara->pInstance;
				break;
			}
		}
	}

	if (pAvailableInstance == NULL)
	{
		pAvailableInstance = pRunningInstance;
	}

	return pAvailableInstance;
}

// 创建虚拟演示仪器对象
BOOL CInstrumentPCR::CreateInstanceDemo(CString strUser)
{
	CInstancePCR* pInstance = new CInstanceDemo(NULL, _T(""));
	if (pInstance != NULL)
	{
		pInstance->SetStartPath(m_strStartPath);
		pInstance->Initialize();
		((CInstanceDemo*)pInstance)->ReadParaFromDemo(strUser);

		tagInstancePara* pInstancePara = new tagInstancePara;
		pInstancePara->pInstance = pInstance;
		m_arrayInstancePCR.Add(pInstancePara);
		return TRUE;
	}

	return FALSE;
}

// 保存
void CInstrumentPCR::SaveInstanceParaForDemo(CString strIniFile)
{
	if (m_arrayInstancePCR.GetCount() > 0)
	{
		tagInstancePara* pInstancePara = (tagInstancePara*)m_arrayInstancePCR.GetAt(0);
		pInstancePara->pInstance->SaveParaForDemo(strIniFile);
	}
}

BOOL CInstrumentPCR::IsExistIdleInstrument()
{
	CInstancePCR* pInstance = NULL;
	for(int i = 0;i < m_arrayInstancePCR.GetCount();++i)
	{
		tagInstancePara* pPara = (tagInstancePara*)m_arrayInstancePCR.GetAt(i);
		if(NULL == pPara)
		{
			continue;
		}

		pInstance = pPara->pInstance;
		if(NULL == pInstance)
		{
			continue;
		}

		if(!pInstance->IsDemo() && pInstance->IsInstanceIdle())
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CInstrumentPCR::SendHello(CSerialPortPCR* pComPort)
{
	int nCmdLength = CMD_SEND_LEN5;
	BYTE* cCmd = new BYTE[nCmdLength];
	memset(cCmd, 0, nCmdLength);
	cCmd[0] = PCR_DNSEND;
	cCmd[1] = PCR_HELLO;	
	cCmd[3] = nCmdLength;
	cCmd[nCmdLength-1] = pComPort->GetByteSum(cCmd, nCmdLength-1);

	int   iReceiveFrameLen = 0;
	BYTE* pStatus = NULL;
	int iReturn = pComPort->SendCommandRepeatedly(cCmd, CMD_SEND_LEN5, &pStatus, iReceiveFrameLen);
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
			m_uiErrorCode = ERR_HELLO_FAIL;
		}
	}

	delete [] cCmd;
	delete [] pStatus;

	return bReturn;
}

BOOL CInstrumentPCR::PortHasUsed(CString strPort)
{
	BOOL bUsed = FALSE;
	int iCount = m_arrayInstancePCR.GetCount();
	for (int i=0; i<iCount; i++)
	{
		tagInstancePara* pParam = (tagInstancePara*)m_arrayInstancePCR.GetAt(i);
		if (pParam->strPort == strPort)
		{
			bUsed = TRUE;
			break;
		}
	}

	return bUsed;
}

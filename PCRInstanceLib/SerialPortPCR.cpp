#include "StdAfx.h"
#include "SerialPortPCR.h"
#include "LogManager.h "
#include "DeclaresPCR.h"
#include "PublicFun.h"

CSerialPortPCR::CSerialPortPCR(void):
	m_ucToleranceMaxVal(1)
	, m_nBaudRate(9600)
{
	m_iSendTimeout = SENDTIMEOUT;
	m_iReceiveTimeout = RECEIVETIMEOUT;
	m_bLongSendTime = FALSE;
	m_bLongReceiveTime = FALSE;

	m_pLogError = new CLogErrorInfo;
	m_pLogComm = new CLogCommInfo;

	m_CommWrite.hEvent = NULL;	
	m_CommRead.hEvent = NULL;	
	m_hWriteEvent = NULL;	
	m_hReadEvent = NULL;
	m_hCommDev = NULL;	
	m_strPort = _T("com1");

	m_bNormalCom = TRUE;
	InitializeCriticalSection(&m_csProtocol);
}


CSerialPortPCR::~CSerialPortPCR(void)
{
	DeleteCriticalSection(&m_csProtocol);
	CloseCom();
	SAFE_DELETE(m_pLogError);
	SAFE_DELETE(m_pLogComm);

}

void CSerialPortPCR::SetUserName(CString strName) 
{ 
	ASSERT(m_pLogError != NULL);
	m_pLogError->SetUser(strName);
}

void CSerialPortPCR::SetInstrumentSN(CString strSN) 
{ 
	ASSERT(m_pLogError != NULL);
	ASSERT(m_pLogComm != NULL);
	m_pLogError->SetInstrument(strSN);
	m_pLogComm->SetInstrument(strSN);
}

// 为日志设置发送指令的说明
void CSerialPortPCR::SetSendComment(CString strSend)
{
	if (m_pLogError != NULL)
	{
		m_pLogError->SetError(strSend);
	}

	if (m_pLogComm != NULL)
	{
		m_pLogComm->SetComment(strSend);
	}
}

BYTE CSerialPortPCR::GetByteSum(BYTE* buf, int length)
{
	BYTE  ret = 0;
	for (int i=0; i<length; i++) 
	{
		ret += buf[i];
	}
	return ret;
}

int CSerialPortPCR::SendCommandRepeatedly(BYTE* pCommand, int nCmdLength, BYTE** ppStatus, int& iStatusLength, int iFaultToleranceCount /*= 3*/)
{
	EnterCriticalSection(&m_csProtocol);

	//if(!m_bNormalCom)
	//{
	//	LeaveCriticalSection(&m_csProtocol);
	//	return RETURN_TIMEOUT;	//20220822添加测试代码
	//}

	int iReturn = RETURN_OK;
	int iSendNum = 0;
	int iSendTimeout = m_iSendTimeout;
	if (m_bLongSendTime)
	{
		iSendTimeout *= 5;
	}
	int iReceiveTimeout = m_iReceiveTimeout;
	if (m_bLongReceiveTime || PCR_MOTOR == pCommand[1])
	{
		iReceiveTimeout *= 5;
		m_bLongReceiveTime = FALSE;
	}

	for (int i = 0; i < iFaultToleranceCount; ++i)
	{
		++iSendNum;
		// 保存通讯记录
		SaveSendCommandLog(pCommand, nCmdLength, iReturn, iSendNum);

		if (SendCommand(pCommand, nCmdLength, iSendTimeout))
		{
			if (NULL != *ppStatus)	//防止重发时发生内存泄漏
			{
				delete[] * ppStatus;
				*ppStatus = NULL;
			}

			if (0 == ReadCom(ppStatus, iStatusLength, iReceiveTimeout))
			{
				if ((pCommand[1] | PCR_0X80) == (*ppStatus)[1])
				{
					if (RETURN_OK == (eCmdReturn)(*ppStatus)[iStatusLength - 2])
					{
						iReturn = RETURN_OK;
						i = iFaultToleranceCount;
					}
					else
					{
						iReturn = RETURN_FAIL;
					}
				}
				//lzh:接收下位机返回的报文
				SaveReceiveCommandLog(*ppStatus, iStatusLength);
			}
			else
			{
				iReturn = RETURN_TIMEOUT;
			}
		}
		else
		{
			iReturn = RETURN_SEND_FAIL;
		}
	}

	if (RETURN_OK != iReturn)
	{
		m_bNormalCom = FALSE;
	}

	// 根据情况保存日志
	HandleCommandReturn(iReturn, pCommand, nCmdLength, iSendNum);

	LeaveCriticalSection(&m_csProtocol);

	return iReturn;
}

void CSerialPortPCR::SaveSendCommandLog(BYTE* pCommand, int nCmdLength, int iResult, int SendCount)
{
	ASSERT(m_pLogComm != NULL);
	m_pLogComm->SetDate(CPublicFun::GetCurrentDateTime());
	m_pLogComm->SetCommType(CLogCommInfo::COMM_TYPE_SEND);
	m_pLogComm->SetResult(iResult);
	m_pLogComm->SetSendCount(SendCount);
	m_pLogComm->SetContent(ConvertToString(pCommand, nCmdLength));
	CLogManager::GetInstance()->WriteCommLog(m_pLogComm);
}

CString CSerialPortPCR::ConvertToString(BYTE* pCommand, int nCmdLength)
{
	CString strLog;
	CString string;
	strLog.Empty();
	for(int i=0; i<nCmdLength; i++)
	{
		string.Format(_T("%02X "),pCommand[i]);
		strLog+=string;
	}

	return strLog;
}

void CSerialPortPCR::SaveReceiveCommandLog(BYTE* pReceive,int nLength)
{
	ASSERT(m_pLogComm != NULL);
	m_pLogComm->SetDate(CPublicFun::GetCurrentDateTime());
	m_pLogComm->SetCommType(CLogCommInfo::COMM_TYPE_RECEIVE);
	m_pLogComm->SetResult(0);
	m_pLogComm->SetSendCount(0);
	m_pLogComm->SetDate(CPublicFun::GetCurrentDateTime(TRUE));
	m_pLogComm->SetContent(ConvertToString(pReceive, nLength));
	CLogManager::GetInstance()->WriteCommLog(m_pLogComm);
}

// 根据命令发送接收情况进行相关日志记录，超过发送1次都要记录，如果超过规定发送次数则认为错误
void CSerialPortPCR::HandleCommandReturn(int nReturn, BYTE* pCommand, int nCmdLength, int nSendCount)
{
	if (nSendCount > 1)
	{
		if (nReturn == RETURN_OK)
		{
			m_pLogError->SetLogType(LOG_TYPE_WARM);
		}
		else
		{
			m_pLogError->SetLogType(LOG_TYPE_ERROR);
		}

		m_pLogError->SetDate(CPublicFun::GetCurrentDateTime(FALSE));
		m_pLogError->SetErrPara1(ConvertToString(pCommand, nCmdLength));
		CString strTemp;
		strTemp.Format(_T(" repeatly to %d times"), nSendCount);
		m_pLogError->SetErrorAppend(strTemp);
		CLogManager::GetInstance()->WriteErrorLog(m_pLogError, TRUE);
	}
}

BOOL CSerialPortPCR::OpenCom(int nPort)
{
	CString str;

	if (nPort < 1 || nPort > 10)
	{
		return FALSE;
	}
	str.Format(_T("com%d"), nPort);

	return OpenCom(str);
}

BOOL CSerialPortPCR::OpenCom(CString strPort)
{
	m_strPort = strPort;

	//打开串口句柄
	m_hCommDev = CreateFile(strPort, GENERIC_WRITE|GENERIC_READ, 0, NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	NULL);

	if(INVALID_HANDLE_VALUE == m_hCommDev)
	{
		m_hCommDev = NULL;
		return FALSE;
	}

	//创建读句柄
	m_hReadEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(NULL == m_hReadEvent)
	{
		return FALSE;
	}
	//创建写句柄
	m_hWriteEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
	if(NULL == m_hWriteEvent)
	{
		return FALSE;
	}

	//清空内存变量
	memset(&m_CommRead,0,sizeof(OVERLAPPED));
	memset(&m_CommWrite,0,sizeof(OVERLAPPED));

	//给重叠区设置句柄
	m_CommRead.hEvent = m_hReadEvent;
	m_CommWrite.hEvent = m_hWriteEvent;

	return TRUE;
}

/**
 * 关闭串口
 */
void CSerialPortPCR::CloseCom()
{
#ifndef BIO_DEMO
	//给重叠区句柄置空
	m_CommWrite.hEvent = NULL;
	m_CommRead.hEvent = NULL;

	//关闭句柄
	if(NULL != m_hReadEvent)
	{
		CloseHandle(m_hReadEvent);
		m_hReadEvent = NULL;
	}
	if(NULL != m_hWriteEvent)
	{
		CloseHandle(m_hWriteEvent);
		m_hWriteEvent = NULL;
	}
	if(NULL != m_hCommDev)
	{
		CloseHandle(m_hCommDev);
		m_hCommDev = NULL;
	}
#endif
}

/**
 * 设置串口
 *
 * \return 
 */
BOOL CSerialPortPCR::SettingCom(int nBaudRate)
{
	//设置DCB块
	DCB dcb;
	//BuildCommDCB("baud=1200 parity=N data=8 stop=1",&dcb);
	if(NULL == m_hCommDev || INVALID_HANDLE_VALUE == m_hCommDev)
	{
		return FALSE;
	}

	m_nBaudRate = nBaudRate;
	//取出系统默认参数
	GetCommState(m_hCommDev,&dcb);

	//设置相关参数
	dcb.BaudRate = nBaudRate;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = ONESTOPBIT;

	// set the timeout values
	COMMTIMEOUTS commTimeouts;
	GetCommTimeouts(m_hCommDev,&commTimeouts);

	commTimeouts.ReadIntervalTimeout = 1000;
	commTimeouts.ReadTotalTimeoutMultiplier = 1000;
	commTimeouts.ReadTotalTimeoutConstant = 1000;
	commTimeouts.WriteTotalTimeoutMultiplier = 1000;
	commTimeouts.WriteTotalTimeoutConstant = 1000;

	SetCommTimeouts(m_hCommDev, &commTimeouts);

	//将数值放回系统
	BOOL bRet = SetCommState(m_hCommDev,&dcb);

	//设置串口接收模式
	if(!SetCommMask(m_hCommDev,EV_RXCHAR))
	{
		return FALSE;
	}

	//设置引脚线参数
	PurgeComm(m_hCommDev, PURGE_RXCLEAR);

	return bRet;
}

/**
 * 读取固定长度数据
 *
 * \param buf 
 * \param dwLength 
 * \return 
 */
DWORD CSerialPortPCR::ReadCommFixed(BYTE* buf,DWORD dwLength)
{
    DWORD   length = 0;
	COMSTAT ComStat;
	DWORD   dwErrorFlags;
	
	for (int iCycle = 0;iCycle < m_ucToleranceMaxVal;iCycle++)
	{
		ClearCommError(m_hCommDev,&dwErrorFlags,&ComStat);
		length = min(dwLength,ComStat.cbInQue);
		BOOL bRet = ReadFile(m_hCommDev,buf,length,&length,&m_CommRead);

		if((!bRet) && (ERROR_IO_PENDING == GetLastError()))
		{
			if(WAIT_OBJECT_0 == WaitForSingleObject(m_CommRead.hEvent,100))
			{
				if(GetOverlappedResult(m_hCommDev,&m_CommRead,(DWORD*)&length,TRUE))
				{
					bRet = TRUE;
				}
			}
			else
			{
				length = 0;
			}
		}

		if (bRet)
		{
			break;
		}
	}

	return length;
}

/**
 * 下发数据
 *
 * \param *pchBuf    ---要发送的内容的首地址
 * \param iLen       ---要发送的内容的长度
 * \param iTime      ---定时时间
 *
 * \return 
 *    TRUE           ---发送成功
 *    FALSE          ---发送失败
 */
BOOL CSerialPortPCR::SendCommand(BYTE *pchBuf,int iLen,int iTime)
{
	if(NULL == m_hCommDev || INVALID_HANDLE_VALUE == m_hCommDev)
	{
		return FALSE;
	}

	DWORD dwWriteLen;
	m_CommWrite.Offset = 0;
	m_CommWrite.OffsetHigh = 0;

	BOOL bRet = FALSE;
	for (int iCycle = 0;iCycle < m_ucToleranceMaxVal;iCycle++)
	{
		bRet = WriteFile(m_hCommDev,pchBuf,iLen,&dwWriteLen,&m_CommWrite);

		if((!bRet) && (ERROR_IO_PENDING == GetLastError()))
		{
			if(WAIT_OBJECT_0 == WaitForSingleObject(m_CommWrite.hEvent,iTime))
			{
				bRet = TRUE;
			}
		}
		
		if (TRUE != bRet)
		{
			bRet = Reset();
			if (!bRet)
			{
				iCycle = m_ucToleranceMaxVal;
			}
		}
		else
		{
			break;
		}
	}

	return bRet;
}

int CSerialPortPCR::ReadCom(BYTE** ppBuf,int& iFrameLen,int iTimeOut)
{
	OVERLAPPED osRead;  
	memset(&osRead,0,sizeof(OVERLAPPED));
	osRead.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	COMSTAT ComStat;
	DWORD dwErrorFlags;
	int len = 0;
	
	SetCommMask(m_hCommDev,EV_RXCHAR);  
	time_t currentMs = clock();	

	while((clock() - currentMs) < iTimeOut)
	{	
		ClearCommError(m_hCommDev,&dwErrorFlags,&ComStat);
		len = ComStat.cbInQue;
		if(len >= 4)
		{
			break;
		}
		Sleep(20);
	}
	if(!len)
	{
		return -1;
	}

	BYTE tempData[4] = {0};
	DWORD dwRevLength = 0;
	BOOL bRcStat = ReadFile(m_hCommDev,tempData,4, &dwRevLength,&osRead);
	DWORD retValue = 0X22222222;  
	if(!bRcStat)
	{
		if(ERROR_IO_PENDING == GetLastError())
		{
			retValue = WaitForSingleObject(osRead.hEvent,1000);  //读总超时1000ms
		}	
	}

	if(bRcStat || WAIT_OBJECT_0 == retValue)
	{
		int iGetLength = tempData[3];

		*ppBuf = new BYTE[iGetLength];
		memset(*ppBuf, 0, iGetLength);

		currentMs = clock();

		while((clock() - currentMs) < 6000)
		{	
			ClearCommError(m_hCommDev,&dwErrorFlags,&ComStat);
			if(iGetLength - 4 == ComStat.cbInQue)
			{
				break;
			}
			Sleep(20);
		}

		if(iGetLength - 4 != ComStat.cbInQue)
		{
			return -4;
		}

		OVERLAPPED osRead2;  
		memset(&osRead2,0,sizeof(OVERLAPPED));
		osRead2.hEvent = CreateEvent(NULL,TRUE,FALSE,NULL);
		memcpy(*ppBuf,tempData,4);
		bRcStat = ReadFile(m_hCommDev,*ppBuf + 4, iGetLength - 4,NULL,&osRead2);
		retValue = 0X22222222;  
		if(!bRcStat)
		{
			if(GetLastError() == ERROR_IO_PENDING)
			{
				retValue = WaitForSingleObject(osRead2.hEvent,5000);  //读总超时5000ms
			}	
		}

		if(bRcStat || WAIT_OBJECT_0 == retValue)
		{
			iFrameLen = iGetLength;
			return 0;
		}
		else
		{
			return -5;   //接收失败
		}
	}
	else
	{
		return -2;   //读取数据失败
	}
}

BOOL CSerialPortPCR::Reset()
{
	CloseCom();
	Sleep(20);

	BOOL bReturn = OpenCom(m_strPort);
	if (bReturn)
	{
		bReturn = SettingCom(m_nBaudRate);
	}

	return bReturn;
}



#pragma once
#include "LogDataInfo.h"

const int FRAMELENPARAM_INDEX = 3; ///帧长参数序号(从0开始)
const int  MAX_BLOCK = 1024; //缓冲区长度
const int  SENDTIMEOUT = 1000; //超时时间 
const int  RECEIVETIMEOUT = 3000; //接收超时

const BYTE PCR_UPSEND = 0x5e;
const BYTE PCR_DNSEND = 0x5e;
const BYTE PCR_0X80   = 0x80;

class AFX_EXT_CLASS CSerialPortPCR
{
public:
	CSerialPortPCR(void);
	~CSerialPortPCR(void);

	enum eCmdReturn{
		RETURN_OK = 0,       // 应答OK
		RETURN_FAIL = 1,     // 应答失败
		RETURN_TIMEOUT = 2, // 超时
		RETURN_REPEAT_OUT = 3, // 超出规定次数失败
		RETURN_CKERROR = 4, // 收到数据校验错误
		RETURN_SEND_FAIL = 5, // 发送失败
	};

public:
	BYTE GetByteSum(BYTE* buf, int length);

	int SendCommandRepeatedly(BYTE* pCommand,int nCmdLength,BYTE** ppStatus,int &iStatusLength,int iFaultToleranceCount = 3);

	void SetUserName(CString strName);
	void SetInstrumentSN(CString strSN);
	void SetSendComment(CString strSend);

	CString GetComPort() { return m_strPort;}

	BOOL OpenCom(int nPort);        
	BOOL OpenCom(CString strPort);    
	void CloseCom();      
	BOOL SettingCom(int nBaudRate);	///< 设置串口参数 
	DWORD ReadCommFixed(BYTE* buf,DWORD dwLength);	///< 读取固定长度数据
	BOOL SendCommand(BYTE* pchBuf,int iLen,int iTime);	///< 下发数据
	int ReadCom(BYTE** ppBuf,int& iFrameLen,int iTimeOut); ///< 读取变长数据

	BOOL Reset();

protected:
	void SaveSendCommandLog(BYTE* pCommand, int nCmdLength, int iResult, int SendCount);
	void SaveReceiveCommandLog(BYTE* pReceive,int nLength);
	void HandleCommandReturn(int nReturn, BYTE* pCommand, int nCmdLength, int nSendCount);

private:
	CString ConvertToString(BYTE* pCommand, int nCmdLength);
	
public:
	void SetSendTimeout(int iTimeout) { m_iSendTimeout = iTimeout; }
	void SetReceiveTimeout(int iTimeout) { m_iReceiveTimeout = iTimeout; }
	void SetLongSendTime(BOOL bLong) { m_bLongSendTime = bLong; }
	void SetLongReceiveTime(BOOL bLong) { m_bLongReceiveTime = bLong; }

protected:
	CRITICAL_SECTION m_csProtocol;
	int m_iSendTimeout; // 发送命令超时时间
	int m_iReceiveTimeout; // 结束数据超时时间
	BOOL m_bLongSendTime; // 是否使用长时间发送
	BOOL m_bLongReceiveTime; // 是否使用长时间接受

	CLogErrorInfo*	m_pLogError; // 错误或警告日志
	CLogCommInfo*	m_pLogComm; // 串口通讯记录日志

	CString m_strPort; //设备端口号 “COM N”
	HANDLE   m_hCommDev;	       ///< 设备句柄
	HANDLE   m_hWriteEvent;	       ///< 写事件句柄
	HANDLE   m_hReadEvent;         ///< 读事件句柄

	OVERLAPPED  m_CommWrite;	   ///<写重叠区
	OVERLAPPED  m_CommRead;	       ///<读重叠区

	unsigned char m_ucToleranceMaxVal;
	int m_nBaudRate;
	

private:
	BOOL m_bNormalCom;	//标识端口是否正常工作，20220822，孙文郎
};


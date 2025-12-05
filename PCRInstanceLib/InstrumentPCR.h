#pragma once

#include "InstancePCR.h"
#include "SerialPortPCR.h"

class AFX_EXT_CLASS CInstrumentPCR
{
public:
	CInstrumentPCR(void);
	~CInstrumentPCR(void);

	struct tagInstancePara
	{
		CString		strPort;
		CInstancePCR*	pInstance; // 设备指针
		CString		strModel; // PCR仪型号
		CString		strSN; // 产品序列号
		CString		strCode; // PCR仪版本(类型)
		CString		strVersion; // 硬件版本
		CString		strSoftware; // 嵌入式软件版本
		tagInstancePara()
		{
			strPort.Empty();
			pInstance = NULL;
			strModel.Empty();
			strSN.Empty();
			strCode.Empty();
			strVersion.Empty();
			strSoftware.Empty();
		}
	};

public:
	static CInstrumentPCR* GetInstance(void);
	static void CleanUp();

	BOOL Initialize(CString strStartPath, BOOL bRecognizeAll=TRUE);
	void Uninitialize();
	UINT GetErrorCode(int &iErrPara1, int &iErrPara2);
	BOOL Reinitialize(BOOL bRecognizeAll);

	int GetInstanceCount() { return m_arrayInstancePCR.GetCount(); }
	CInstancePCR* GetInstancePCR(CString strModel=_T(""), CString strSN=_T(""));
	CInstancePCR* GetInstancePCR(int iIndex);
	CInstancePCR* GetAvailableInstance();

	BOOL CreateInstanceDemo(CString strUser); // 创建虚拟仪器
	void SaveInstanceParaForDemo(CString strIniFile);

	BOOL IsExistIdleInstrument();

protected:
	void GetComPort(CStringArray& ComArray);
	void InitCommPort_AutoRecognition(BOOL bAppOrSer); // 初始化通讯口 自动识别
	BOOL ReadModelFromEEPROM(CSerialPortPCR* pComPort, tagInstancePara* pInsPara);
	BOOL ReadEPROM(CSerialPortPCR* pComPort, UINT nAddress,BYTE &cLen,BYTE** pReceive);
	CInstancePCR* CreateInstancePCR(CSerialPortPCR* pComPort, tagInstancePara* pInputPara);
	BOOL SendHello(CSerialPortPCR* pComPort);

	void SaveCodeToPara(tagInstancePara* pInsPara, BYTE* pReceiveData, int iFlag);

	BOOL PortHasUsed(CString strPort);

protected:
	static CInstrumentPCR*	m_pInstrumentPCR;

	CString m_strStartPath;
	UINT m_uiErrorCode; // 错误码，方便外部判读错误类型
	int	m_iErrorPara1; // 错误参数 1/2 方便表明错误的详细信息
	int m_iErrorPara2;

	int m_iBandRate;
	int m_iResetTime;
	int m_iSTimeOutBase;
	CPtrArray	m_arrayInstancePCR;// 全部链接的PCR仪 CInstancePCR
};


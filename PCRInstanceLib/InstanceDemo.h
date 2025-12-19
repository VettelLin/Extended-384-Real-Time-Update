#pragma once

#include "InstancePCR.h"

class AFX_EXT_CLASS CInstanceDemo : public CInstancePCR
{
public:
	CInstanceDemo(CSerialPortPCR* pComPort, CString strVersion);
	~CInstanceDemo(void);

// Operations
public:	
	virtual BOOL Initialize();
	virtual BOOL Hello();
	virtual BOOL ReadDeviceParamsFromEPROM();
	virtual BOOL ReadStructureParamsFromEPROM();
	virtual BOOL Home();
	virtual BOOL MotorScanRow(WORD wPulse, BYTE cDirection); // 默认X电机

	virtual BOOL SetStateLED(BYTE cMode);
	virtual BOOL SetPMTGain(WORD wGainPara);
	virtual BOOL SetTempCtrlMode(eTempCtrlMode nCtrlMode);// 控温方式？是否控温
	virtual BOOL SetLidHeaterTemp(float fTemp); // 设置热盖温度
	virtual BOOL SetMultiTemp(float fTemp1, float fTemp2, float fTemp3, float fRate, BYTE cMode);
	virtual BOOL SetLaserStatus(BYTE cType, BYTE cStatus);

	virtual BOOL ScanOnce(BOOL *pBeAskTop);
	virtual BOOL ReadChannelTemp(BYTE cChannel, float &fTempCh, BOOL& bToTarget); // 读取指定通道温度
	virtual BOOL ReadPCRTemp(float &fTempPCR, BOOL& bToTarget); // 读取PCR仪温度
	virtual BOOL ReadFLUData(CUIntArray& arrayData, BYTE cChannel, BYTE cFrmIndex=1);

	virtual BOOL ReadDeviceStatus();

	//添加于20220415，孙文郎
	virtual BOOL ReadEPROM(UINT nAddress, BYTE cGetLength,BYTE* pReceive);
	virtual BOOL WriteEPROM(UINT nAddress,int iData,int iDataLen);

	virtual int GetInitChanDyeName(vector<CString>& vecChanDyeName);	//新建实验时，初始化每个通道的染料名称标识

	virtual BOOL IsDemo();	//判断是否是Demo

			void ReadParaFromDemo(CString strIniFile);

protected:
	virtual void InitStructurePara();
	// 模拟数据
	void InitFluData(); // 初始化各通道荧光数据
	void InitOneDyeFluData(CUIntArray* pArrayFlu, CString strIniFile);
	UINT GetFluDataBy(int iTubeIndex, CUIntArray* pArrayFlu);

public:
	virtual CString GetModel();

	void InitDemoData(int iTubeCount);
	virtual int GetMaxLidHeatSecond(); // 取得热盖加热最长时间

protected:
	CUIntArray	m_arrayTemp; // 温度数据
	int		m_iCurTempIndex; // 当前温度位置

	CUIntArray	m_arrayFluFam; // 通道1
	CUIntArray  m_arrayFluCy5; // 通道2
	CUIntArray	m_arrayFluHex; // 通道3
	CUIntArray	m_arrayFluCy55; // 通道4
	CUIntArray	m_arrayFluRox; // 通道5
	CUIntArray	m_arrayFluCy3; // 通道6

	int		m_iTubeCount; // 孔板管数
	int		m_iCurCycleNo; // 当前循环数
	int		m_iFirstChannelNo;

	int*	m_pCurFluIndex; // 孔板各管位对应荧光数据开始位置
};


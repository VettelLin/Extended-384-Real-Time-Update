#pragma once

#include "SerialPortPCR.h"
#include "DeclaresPCR.h"
#include "LinkList.h"
#include "SegItemInfo.h"
#include "SensorMCheckParam.h"
#include "HeatMCheckParam.h"

#include <vector>

using namespace std;

class AFX_EXT_CLASS CInstancePCR
{
public:
	CInstancePCR(CSerialPortPCR* pComPort, CString strVersion);
	virtual ~CInstancePCR(void);
private:
	DWORD m_dwYAccumulatedPulse;   // Y轴累计绝对脉冲（跨 ScanOnce 持续）

public:
	// 仪器模块说明，外部调用使用
	enum eUnitPCR{
		UNIT_MOTOR_X = 0, // X轴电机
		UNIT_MOTOR_Y, // Y轴电机
		UNIT_MOTOR_Z, // Z轴压紧电机
		UNIT_MOTOR_U, // U轴进出仓电机

		UINT_STATE_LED, // 状态灯
	};
	// 电机移动类型
	enum eMotorMoveFlag{
		MOTOR_MOVE_TO = 0, // 移动到
		MOTOR_FORWARD, // 正向移动
		MOTOR_REVERSE, // 反向移动
	};
	// 系统设置温度参数类型
	enum eSysTempType
	{
		SYS_TEMP_CTRL_TOP = 0, // 系统控温最高温度
		SYS_TEMP_CTRL_LOWER, // 系统控温最低温度
		SYS_TEMP_LID_TOP, // 热盖最高温度
		SYS_TEMP_LID_LOWER, // 热盖最低温度
		SYS_TEMP_PELTIER_LOWER, // 帕尔贴最低保持温度
	};

// Operations
public:
	BOOL Reinitialize();
	virtual BOOL Initialize();
	virtual BOOL Hello();
			void InitDevicePara(CString strFilename);
	virtual BOOL ReadEPROM(UINT nAddress, BYTE cGetLength,BYTE* pReceive);
	virtual BOOL WriteEPROM(UINT nAddress,int iData,int iDataLen);

	virtual BOOL Home();
	virtual BOOL IsInBox(eSysBoxStatus &outStatus); // 是否进仓
	virtual BOOL InBox();
	virtual BOOL OutBox();
	virtual BOOL MotorReset(eUnitPCR nMotorFlag);
	virtual BOOL MotorMove(eUnitPCR nMotorFlag, WORD wPulse, eMotorMoveFlag nMoveFlag);
	virtual BOOL MotorScanRow(WORD wPulse, BYTE cDirection); // 默认X电机
	virtual BOOL ScanOnce();

	virtual BOOL ScanOnce(BOOL *pBeAskTop);	// 完成一次扫描全部扫描动作，支持快速停止扫描，20220330，孙文郎

	virtual BOOL SetStateLED(BYTE cMode);
	virtual BOOL SetPMTGain(WORD wGainPara);
	virtual BOOL SetTempCtrlMode(eTempCtrlMode nCtrlMode);// 控温方式？是否控温
	virtual BOOL SetLidHeaterTemp(float fTemp); // 设置热盖温度，实验运行时给下位机下发热盖温度
	virtual BOOL SetMultiTemp(float fTemp1, float fTemp2, float fTemp3, float fRate, BYTE cMode,BOOL bSelfCheck = FALSE);	//设置温度，PID模式
	virtual BOOL SetPWMTemp(float fTemp1,float fTemp2,float fTemp3,WORD Pwm1,WORD Pwm2,WORD Pwm3);	//设置温度，PID模式
	virtual BOOL SetLaserStatus(BYTE cType, BYTE cStatus);

	virtual BOOL ReadChannelTemp(BYTE cChannel, float &fTempCh, BOOL& bToTarget); // 读取指定通道温度
	virtual BOOL ReadPCRTemp(float &fTempPCR, BOOL& bToTarget); // 读取PCR仪温度
	virtual BOOL ReadAllTemp(int iTempCount, float* pTemp, BOOL& bToTarget); //读取仪器Block模块所有温度

	virtual BOOL ReadFLUData(CUIntArray& arrayData, BYTE cChannel, BYTE cFrmIndex=1);
	virtual BOOL ReadAllFLUData(CUIntArray* pAryData,int iChanNum);	//封装了多通道数据读取过程
	virtual BOOL SendYMotorAbsoluteMove4Byte(DWORD dwPulse, WORD wSpeed);
	virtual BOOL ReadDeviceStatus(); // 读取设备状态，在实验过程中使用
	virtual BOOL ReadHardwareStatus(); // 读取全部硬件状态，查找错误是使用

			void SaveParaForDemo(CString strIniFile); // 虚拟演示程序使用
			void OutputHardwareStatusToErrorLog(); // 输出硬件状态信息到错误日志中

	virtual BOOL ReadLidTemperatureFromEEPROMPLCUsed();	
	virtual BOOL ReadLidTemperatureFromEEPROMPLCUsed(double& dLidTem);
	virtual BOOL WriteLidTemperaturePLCUsed(float fLidTem);
	virtual BOOL WriteLidPreheatTemperaturePLCUsed(float fLidTem);
	virtual BOOL ReadLidPreheatTemperaturePLCUsed(float& fLidPreheatTem);

	virtual int GetInitChanDyeName(vector<CString>& vecChanDyeName);	//新建实验时，初始化每个通道的染料名称标识

	virtual int CalculateExperimentTotalTimeSec(CLinkList<CSegItemInfo>& lnkSegItem,BOOL bUseLid,int iVolume);	//估算实验总时间

	virtual BOOL IsDemo();	//判断是否是Demo

			int GetMppcGainHigh();
			int GetMppcGainMiddle();
			int GetMppcGainLow();

			void SetInstanceStatus(eInstanceStatus eStatus);
			eInstanceStatus GetInstanceStatus();
			BOOL IsInstanceIdle();

			float GetPwmK1();
			float GetPwmB1();
			float GetPwmK2();
			float GetPwmB2();
			float GetPwmK3();
			float GetPwmB3();

			void SetEndCoolTemperature(float fTem);
			float GetEndCoolTemperature();
			void SetSleepMsAfterCool(int iMs);
			int GetSleepMsAfterCool();

			void SetPWMInitWaitMs(int iMs);
			int GetPWMInitWaitMs();

protected:
			void DeleteAll();
	virtual void InitDevicePara();
			void InitDeviceStatus();
	virtual void InitStructurePara();
			void InitOneChannelFilter(int iChannel, int iByte);
			void InitFilterPara();
			void DeleteFilterPara();

	virtual BOOL ReadDeviceParamsFromEPROM();
	virtual BOOL ReadStructureParamsFromEPROM();
	
			BOOL ReadLongEPROM(UINT nAddress, UINT nGetLength,BYTE* pReceive);

			void SetDeviceStatusBy(int iLength, BYTE* pStatus);
			void SetHardwareStatus(BYTE* state);

			BYTE* NewSendCommand(int iCmdLength, BYTE cCommand, WORD wPara);
			BYTE* NewSendCommand(int iCmdLength, BYTE cCommand, BYTE cPara1, BYTE cPara2);
			BYTE* NewSendCommand(int iCmdLength, BYTE cCommand, UINT nAddress, BYTE cGReceiveLen);
			BYTE* NewSendCommand(int iCmdLength, BYTE cCommand, BYTE cFlag, WORD wPulse, WORD wSpeed);
			BYTE* NewSendCommand(int iCmdLength, BYTE cCommand, WORD wPara1, WORD wPara2, WORD wPara3, WORD wPara4, BYTE cMode);

			BOOL MotorReset(BYTE cMotorFlag);
			BOOL MotorMove(BYTE  cMotorFlag, WORD wPulse, WORD wSpeed);

			void SetDeviceParams(BYTE* pPara,int iLen);

// Attributes
public:
	void SetModel(CString strModel) { m_strModel = strModel; }
	virtual CString GetModel() { return m_strModel; }

	void SetSN(CString strSN) { m_strSN=strSN; }
	CString GetSN() { return m_strSN; }

	CString GetVersion() { return m_strVersion; }

	void SetCode(CString strCode) { m_strCode = strCode; } 
	CString GetCode() { return m_strCode; }

	void SetSoftwareVersion(CString strSoftwareVer) { m_strSoftwareVer = strSoftwareVer; }
	CString GetSoftwareVersion(){ return m_strSoftwareVer; }

	BOOL GetBe96SiteQPCR() { return PCR_CODE_QPCR_STANDARD == m_strCode; }	
	BOOL GetBePS96PCR() { return PCR_CODE_IAMP_PS96 == m_strCode; }
	BOOL GetBeN96PCR() { return PCR_CODE_QPCR_STANDARD == m_strCode; }

	void SetStartPath(CString strPath) { m_strStartPath = strPath; }
	UINT GetErrorCode(int &iErrPara1, int &iErrPara2);

	void GetPlateInfo(tagPlateModeInfo* pInfo);
	ePCRTempMode GetTempMode(); // 控温模式
	eTempCtrlMode GetTempCtrlMode(); // 控温方式

	WORD GetPMTGain();

	float GetTempLegalGap(); // 取得默认到达温度的偏差范围
	float GetLidTempLegalGap();	//获取热盖温度的 到温偏差范围

	virtual int GetMaxLidHeatSecond(); // 取得热盖加热最长时间
			int GetMaxTempCtrlSecond(); // 取得加热模块最长控温时间

	virtual int GetLidHeatNormalSecond();	//获取热盖正常工作时的升温时间

	virtual int GetLidState(int& iStatus);	//获取PS96热盖状态

	float GetSysTemp(eSysTempType eType); // 取得指定类型的温度
	int GetTempChannelCount(); // 取得温度通道数
	
	int GetMeltPreDelay(); // 熔解荧光扫描开始前等待时间
	float GetMeltMinIncrement(); // 熔解单步最小增量
	int GetMeltStepDelay(); // 熔解单步等待时间
	int GetScanOnceTime(BOOL bIsSecond=TRUE); // 扫描指定通道一次需要时间
	int GetScanRowCount(); // 扫描行数即X电机移动次数
	eCalibrateMethod GetCalibrateMethod() { return (eCalibrateMethod)m_sStructurePara.cCalibratePara; }

	tagDeviceStatus* GetDeviceStatus() { return &m_sDeviceStatus; }

	int GetCalibrateChannel();
	int GetScanChannelCount(); // 只返回扫描光路通道数
	tagScanChannelInfo* GetScanChannelInfo(int iIndex); // 所有光路
	int GetScanChannelInfo(CLinkList<tagScanChannelInfo>& lnkScanChanInfo);
	CPtrArray* GetScanChannelInfoArray() { return & m_arrayFilter; }
	eScanStartPos GetScanStartPos();

	int GetLidTem();
	BOOL SetLidTem(int iLidTem);
	
	BOOL HasBox();// 是否有进出仓
	BOOL HasLid(); // 是否有热盖

    int GetPreMeltMinSleepSec();

	virtual int SportModuleCheck();
	virtual int SensorModuleCheck();
	virtual int HeatModeduleCheck();
	virtual int OpticsModuleCheck();

	virtual CString GetCheckResultString();

	BOOL CanRunExp();

protected:
	CSerialPortPCR*	m_pComPort;
	CString		m_strModel; // PCR仪型号
	CString		m_strSN; // PCR序列号
	CString		m_strCode;	//PCR仪类型
	CString		m_strVersion; // 硬件版本
	CString		m_strSoftwareVer; // 嵌入式软件版本

	CString m_strStartPath;
	UINT m_uiErrorCode; // 错误码，方便外部判读错误类型
	int	m_iErrorPara1; // 错误参数 1/2 方便表明错误的详细信息
	int m_iErrorPara2;

	CPtrArray		m_arrayFilter; // 通道滤色片信息 tagScanChannelInfo
	tagDeviceParams		m_sDevicePara;  // 全部设备参数信息，按一个字节排列
	tagStructureParams	m_sStructurePara;  // PCR仪结构参数
	tagDeviceStatus		m_sDeviceStatus; // 设备状态，监控状态
	tagHardwareStatus	m_sHardwareStatus; // 硬件设备状态

	int m_iMppcGainH;	//MPPC增益高值，仪器各通道统一的增益高值，默认值为4742，EEPROM地址:0X850,2Byte，主要用于96孔QPCR
	int m_iMppcGainM;	//MPPC增益中值，仪器各通道统一的增益中值，默认值为4125，EEPROM地址:0X852,2Byte，主要用于96孔QPCR
	int m_iMppcGainL;	//MPPC增益低值，仪器各通道统一的增益低值，默认值为3637，EEPROM地址:0X854,2Byte，主要用于96孔QPCR

	eInstanceStatus m_eInstanceStatus;	//记录仪器的工作状态

	float m_fPwmK1;	//PWM K B 值
	float m_fPwmB1;
	float m_fPwmK2;
	float m_fPwmB2;
	float m_fPwmK3;
	float m_fPwmB3;

	float m_fEndCoolTem;	//实验结束时，仪器需要自动到达的温度(低于该温度时，不做处理，防止开仓时爆管)
	int m_iMsSlpAfterCool;	//实验结束时，仪器自动到达冷却温度后，延时时间，单位为毫秒

	int m_iMsPwmInitWait;	//PWM设置初始温度时，需等待（PWM稳定）的毫秒数

	int m_iPreMeltMinSlpSec;	//熔曲开始前，起始温度需要保持的最小时间，N16熔曲使用

	CSensorMCheckParam m_sensorMChkParam;
	CHeatMCheckParam m_heatModuleChkParam;
};



#pragma once

#include "DeclaresPCR.h"

// 校准参数：浊度校准
class AFX_EXT_CLASS CCalibrateParaTurbidity
{
public:
	CCalibrateParaTurbidity(void);
	~CCalibrateParaTurbidity(void);


	struct tagTurbidityPara
	{
		CString strDyeName; // 染料名称
		int		iChannel; // 通道
		BOOL bUseMOT; // 是否使用修正通道数据
		int iAheadCycle; // 前移多少循环开始
		float fRatio;  // 作为显示调整系数使用

		tagTurbidityPara()
		{
			strDyeName.Empty();
			iChannel = -1;
			bUseMOT = FALSE;
			iAheadCycle = 5;
			fRatio = 0;
		}
	};

public:
	BOOL ReadPara(CString strFile);
	BOOL SavePara(CString strFile);

	BOOL WriteFile(CFile* pFile);
	BOOL ReadFile(CFile* pFile, WORD wVersion);

	int GetParaCount() { return m_arrayDyePara.GetCount(); }
	CString GetDye(int iIndex);

	void ResetChannel(CPtrArray* pArrayDyeChannel);

protected:
	void DeleteAll();
	tagTurbidityPara* GetParaBy(CString strDye);

// Attributes
public:
	int GetChannelMOT() { return m_iChannelMOT; }
	void SetChannelMOT(int iChannel) { m_iChannelMOT = iChannel; }

	float GetMinRatio() { return m_fMinRatio; }
	void SetMinRatio(float fRatio) { m_fMinRatio = fRatio; }

	int GetChannel(CString strDye);

	BOOL UseCalibrate(CString strDye);
	BOOL UseCalibrate(int iChannel);
	void EnableUseCalibrate(CString strDye, BOOL bEnable);
	int GetAheadCycle(CString strDye);
	void SetAheadCycle(CString strDye, int iCycyle);
	float GetAdjustRatio(CString strDye);
	void SetAdjustRatio(CString strDye, float fRatio);

protected:
	int		m_iChannelMOT; // 浊度校准通道
	float	m_fMinRatio; //MOT通道荧光数据归一化后一阶导数认为是出现浊度变化的最小值
	CPtrArray	m_arrayDyePara; // 染料调整参数
};


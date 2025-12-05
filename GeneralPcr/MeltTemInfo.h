#pragma once
class AFX_EXT_CLASS CMeltTemInfo
{
public:
	CMeltTemInfo(void);
	~CMeltTemInfo(void);

	CMeltTemInfo(const CMeltTemInfo& meltTemInfo);
	void operator=(const CMeltTemInfo& meltTemInfo);

public:
	double m_dTargetTem;///< 目标温度值
	double m_dPeltier1Tem;    ///< 第1路温度数据
	double m_dPeltier2Tem;	 ///< 第2路温度数据
	double m_dPeltier3Tem;	 ///< 第3路温度数据

	double m_dPeltier1StartTem; ///< 第1路温度整板扫描起点温度值
	double m_dPeltier1EndTem;   ///< 第1路温度整板扫描终点温度值
	double m_dPeltier2StartTem; ///< 第2路温度整板扫描起点温度值
	double m_dPeltier2EndTem;   ///< 第2路温度整板扫描终点温度值
	double m_dPeltier3StartTem; ///< 第3路温度整板扫描起点温度值
	double m_dPeltier3EndTem;   ///< 第3路温度整板扫描终点温度值
};


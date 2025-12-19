#pragma once

class CRunProgressRecord
{
public:
	CRunProgressRecord(void);
	~CRunProgressRecord(void);

	CRunProgressRecord(const CRunProgressRecord& info);
	void operator=(const CRunProgressRecord& info);

public:
	int m_iPageId;	//页面ID
	int m_iSegId;
	int m_iStepId;
	int m_iCycleId;
	CString m_strFilePath;	//实验文件保存路径
	CString m_strSN;	//仪器序列号

	time_t m_ttStart;	//实验开始时间
	float m_fMeltStartTem;	//熔曲步骤起始温度
	float m_fMeltEndTem;	//熔曲步骤目标温度
	float m_fPowerOffTem;	//记录已经完成数据保存的熔曲温度点
};


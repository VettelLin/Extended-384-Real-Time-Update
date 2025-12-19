#pragma once
class CMyStepEditInfo
{
public:
	CMyStepEditInfo(void);
	~CMyStepEditInfo(void);

	CMyStepEditInfo(const CMyStepEditInfo& stepEditInfo);
	void operator=(const CMyStepEditInfo& stepEditInfo);

public:
	CString m_strSegName;
	CString m_strStepName;
	float m_fAimTem;
	BOOL m_bSelGradient;
	float m_fGradientMaxTem;
	int m_holdSec;
	int m_iCollect;	//是否采集荧光
	float m_fRate;	//升降温速率
	float m_fIncrement;	//熔曲荧光采集温度间隔

	BOOL m_bCollectExistExceptThisStep;	//步骤之外是否已经存在采集步骤（扩增）
	BOOL m_bCollectAllExistExceptThisStep;	//步骤之外是否已经存在全点采集步骤（熔曲）

	BOOL m_bMeltSeg;	//标识步骤所在的程序段是否是熔曲程序段
};


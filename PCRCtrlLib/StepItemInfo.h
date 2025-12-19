#pragma once

#include "DeclaresPCR.h"

class AFX_EXT_CLASS CStepItemInfo
{
public:
	CStepItemInfo(void);
	~CStepItemInfo(void);

	CStepItemInfo(const CStepItemInfo& stepItemInfo);
	void operator=(const CStepItemInfo& stepItemInfo);

	
	void GetStepGradientInfo(float *ColTemp,float &DeltaT,float &fbackcali_AimT,float &fbackcali_DeltaT);

	BOOL GetBeCollectAll();	//判断是否是 熔曲采集 步骤
	BOOL GetBeCollect();	//判断是否是 扩增采集 步骤
	BOOL GetBeNoneCollect();	//判断是否是 不采集 步骤

	void SetCollectAll();	//设置是否是 熔曲采集 步骤
	void SetCollect();	//设置是否是 扩增采集 步骤
	void SetNoneCollect();	//设置是否是 不采集 步骤

	BOOL WriteFile(CFile* pFile);
	BOOL ReadFile(CFile* pFile, WORD wVersion);

public:
	BOOL    m_bSel;  ///< 表示当前是否处于选中状态
	BOOL    m_bSelGrad; ///< 表示是否选中梯度模式
	float   m_fAimTem;   ///< 表示目标温度
	float   m_fGradUpTem;    ///< 表示梯度温度上限	
	int     m_iDelay;  ///< 表示延迟时间
	float   m_fRate;   ///< 表示温度上升的速度
	eStepCheckType m_eChkType;  ///< 数据检测的方式,1--不检测，2--末点检测，3--全点检测
	float   m_fIncrement; ///< Melt温度增量
	unsigned int     m_uiHoldTimeSec; ///< Melt温度增量对应的Hold时间
	float   m_fRangeBelowStartTem; ///< Melt 控制流程，初始降温范围
	unsigned int	m_uiWaitTimeBelowStartTem; ///< Melt 控制流程，低于起始温度的等待时间 单位（s）

	int m_stepW;	//步骤项宽度

	CRect m_rcTem;
	CRect m_rcLine;
	CRect m_rcMinSec;
	CRect m_rcCkbtCollectFlu;

	CString m_strStepName;
};


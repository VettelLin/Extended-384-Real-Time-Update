#pragma once

#include "DeclaresPCR.h"
#include "ReagentItem.h"

class AFX_EXT_CLASS CDyeTargetInfo
{
public:
	CDyeTargetInfo(void);
	~CDyeTargetInfo(void);

	enum eCalculateValueType
	{
		VALUE_CT = 0, // 插值法Ct
		VALUE_REPEAT_CT, // 复管Ct
		VALUE_SET_QUANTITY, // 设置浓度
		VALUE_CALC_QUANTITY, // 计算浓度拷贝数
		VALUE_REPEAT_QUANTITY, // 复管浓度
		VALUE_MEAN_CT, // 平均Ct
		VALUE_CT_SD, // Ct平均方差
		VALUE_TM	//Tm值
	};

	CDyeTargetInfo(const CDyeTargetInfo& dyeTargetInfo);
	void operator=(const CDyeTargetInfo& dyeTargetInfo);

// Operations
public:
	BOOL WriteFile(CFile* pFile,WORD wVersion);
	BOOL ReadFile(CFile* pFile,WORD wVersion);
	void SetTm(int nCount, double* pdTm, double* pdRm);
	
protected:
	void DeleteAll();

// Attributes
public:
	CString GetDyeName() { return m_strDye; }
	void SetDyeName(CString string) { m_strDye = string; }
	CString GetTargetName() { return m_strTarget; }
	void SetTargetName(CString string) { m_strTarget = string; }
	CString GetQuencher() { return m_strQuencher; }
	void SetQuencher(CString string) { m_strQuencher = string; }
	int GetChannel() { return m_iChannel; }
	void SetChannel(int iChannel) { m_iChannel = iChannel; }
	COLORREF GetTargetColor() {return m_clrTarget; }
	void SetTargetColor(COLORREF color) { m_clrTarget = color; }

	BOOL GetAutoBaseline() { return m_bAutoBaseline; }
	void SetAutoBaseline(BOOL bAuto) { m_bAutoBaseline = bAuto; }
	int GetBaselineStart() { return m_iBaselineStart; }
	void SetBaselineStart(int iStart) { m_iBaselineStart = iStart; }
	int GetBaselineEnd() { return m_iBaselineEnd; }
	void SetBaselineEnd(int iEnd) { m_iBaselineEnd = iEnd; }
	BOOL GetAutoThreshold() { return m_bAutoThreshold; }
	void SetAutoThreshold(BOOL bAuto) { m_bAutoThreshold = bAuto; }
	double GetThresholdValue() { return m_dThreshold; }
	void SetThresholdValue(double dValue) { m_dThreshold = dValue; }

	double GetCalcValue(eCalculateValueType nType);
	void SetCalcValue(eCalculateValueType nType, double dValue);

	void SetTmCount(int nCount);
	int GetTmCount() { return m_iTmCount; }

	void SetBeSelected(BOOL bSelected);
	BOOL GetBeSelected();

	void SetQuantity(double dQuantity,int iDpd);
	double GetQuantity();
	int GetQuanIDPD();

	void SetBaselineAverage(double dAverage) { m_dBaselineAverage = dAverage; }
	double GetBaselineAverage() { return m_dBaselineAverage; }
	BOOL HasSndDrvCt() { return m_bHasSndDrvCt; }
	void SetHasSndDrvCt(BOOL bHas) { m_bHasSndDrvCt = bHas; }
	double GetSndDrvCt() { return m_dSndDrvCt; }
	void SetSndDrvCt(double dCt) { m_dSndDrvCt = dCt; }

	double GetCt(){return m_dCt;}
	double GetTm1();
	CString GetTm1String();

	void SetAnalyseResult(EANALYSERESULT eAnaRst){ m_eAnaRst = eAnaRst; }
	EANALYSERESULT GetAnalyseResult(){ return m_eAnaRst; }

	BOOL GetBePositiveAnaResult() { return EAR_POSITVE == m_eAnaRst; }
	BOOL GetBeNegativeAnaResult() { return EAR_NEGATIVE == m_eAnaRst; }
	BOOL GetBeUnhandledAnaResult() { return EAR_UNHANDLED == m_eAnaRst; }

	void SetCycleOfPlateau(int iCycle) { m_iCycleOfPlateau = iCycle; }
	int GetCycleOfPlateau() { return m_iCycleOfPlateau;}

protected:
	BOOL		m_bSelected;	//标识染料是否选中
	CString		m_strDye; // 染料名称，即Reporter
	CString		m_strTarget; // 目标
	CString		m_strQuencher; // 淬灭者
	int			m_iChannel; // 通道，从1开始
	double		m_dQuantity; // 浓度
	int			m_iQuanIdpd;	//设定浓度的小数点位数
	COLORREF	m_clrTarget; // 指标曲线颜色

	BOOL	m_bAutoBaseline;	//是否选择自动基线
	int		m_iBaselineStart;	//基线起点
	int		m_iBaselineEnd;	//基线终点
	BOOL	m_bAutoThreshold;	//是否选择自动阈值
	double	m_dThreshold; // 手动阈值

	// 扩增曲线Ct及相关结果
	double	m_dBaselineAverage; // 基线范围内荧光值均值
	BOOL	m_bHasSndDrvCt; // 是否存在二阶导数Ct
	double	m_dSndDrvCt; // 最大二阶导数计算Ct

	double	m_dCt; // Ct
	double	m_dCalcQuantity; // 重新计算后的拷贝数
	double	m_dRepeatCt; // 复管Ct
	double	m_dRepeatQuantity; // 复管浓度数

	// 熔解段参数
	int			m_iTmCount; // 熔解段Tm数量
	double*		m_pMeltTm; // 熔解段Tm值
	double*		m_pMeltRm; // 熔解段最高峰Rm值

	EANALYSERESULT m_eAnaRst;	//阴阳性判定结果

	int	m_iCycleOfPlateau; // 平台期开始的循环
};


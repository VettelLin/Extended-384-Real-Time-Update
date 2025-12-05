#pragma once
class CMaxMinInfo
{
public:
	CMaxMinInfo(void);
	~CMaxMinInfo(void);

	CMaxMinInfo(const CMaxMinInfo& minMaxInfo);
	void operator=(const CMaxMinInfo& minMaxInfo);

	void SetBeOnlyIntergerNumber(BOOL beOnlyIntergerNumber);
	BOOL GetBeOnlyIntergerNumber();

	void SetBeOnlyFloatNumber(BOOL beOnlyFloatNumber);
	BOOL GetBeOnlyFloatNumber();

	void SetBeSetMaxValue(BOOL bSetMaxValue);
	BOOL GetBeSetMaxValue();

	void SetMaxValue(double dMaxValue);
	double GetMaxValue();

	void SetBeMaxEqualSignWork(double bMaxEqualSignWork);
	BOOL GetBeMaxEqualSignWork();

	void SetBeSetMinValue(BOOL bSetMinValue);
	BOOL GetBeSetMinValue();

	void SetMinValue(double dMinValue);
	double GetMinValue();

	void SetBeMinEqualSignWork(double bMinEqualSignWork);
	BOOL GetBeMinEqualSignWork();

	void SetBeAllowNegtiveNumber(BOOL bAllowNegtiveNumber);
	BOOL GetBeAllowNegtiveNumber();

	void SetBeMaxMinSimultaneousEffect(BOOL bMaxMinSimultaneousEffect);
	BOOL GetBeMaxMinSimultaneousEffect();

	void SetBeLessThanMinSetMin(BOOL bLessThanMinSetMin);
	BOOL GetBeLessThanMinSetMin();

	void ClearMaxMinLimit();

private:
	BOOL   m_bOnlyIntergerNumber;	//是否只能输入整数数字
	BOOL   m_bOnlyFloatNumber;	//是否只能输入浮点数（兼容double类型）
	BOOL   m_bSetMaxValue;	//是否设置最大值
	double m_dMaxValue;	//最大值，结合m_bSetMaxValue使用
	BOOL   m_bMaxEqualSignWork;	//标识“<= 中的 = 是否起作用”
	BOOL   m_bSetMinValue;	//是否设置最小值
	double m_dMinValue;	//最小值，结合m_bSetMinValue使用
	BOOL   m_bAllowNegtiveNumber;	//标识“是否允许负数输入"
	BOOL   m_bMinEqualSignWork;	//标识“>= 中的 = 是否起作用”

	BOOL m_bMaxMinSimultaneousEffect;	//标识“最大最小值限制是否同时起作用（最大最小值限制同时存在时，默认最大值生效）”

	BOOL m_bLessThanMinSetMin;	//有最小值限制的项，如果用户输入的数值小于最小值 或者 输入内容为空，软件直接设置为最小值
};


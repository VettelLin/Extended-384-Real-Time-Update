#pragma once
class __declspec(dllexport) CPeakAmlCheckParam
{
public:
	CPeakAmlCheckParam(void);
	~CPeakAmlCheckParam(void);

	CPeakAmlCheckParam(const CPeakAmlCheckParam& param);
	void operator=(const CPeakAmlCheckParam& param);
	BOOL operator==(const CPeakAmlCheckParam& param);

public:
	double m_dStdThreshold;	/*!< 曲线标准差阈值 */
	double m_dRatioThreshold;	/*!< 曲线尾端信号值除以首端信号值 阈值 */

	double m_dCrestBkRatio;	/*!< 曲线二阶导数最大值除以基线值计算出比值系数R,该参数为R的阈值 */
	int m_bkCalcDotNum;	/*!< 曲线二阶导数最大值除以基线值计算出比值系数R,选取前bkCalcDotNum个数据点的Y值均值作为基线值 */

	int m_start;	/*!<比较二阶导数最大值和基线段二阶倒数数值，基线段起点 */
	int m_forwardNum;	/*!<比较二阶导数最大值和基线段二阶倒数数值，二阶导数最大值位置前移newForwardNum个点作为基线段终点 */
	float m_sdMultiples;	/*!<比较二阶导数最大值和基线段二阶倒数数值，前者是否达到后者均值加上newStdNum倍SD */
};


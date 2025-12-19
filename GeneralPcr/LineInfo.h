#pragma once

#include <vector>
#include "MyPoint.h"
#include "LinkList.h"
#include "AlgorithmSet.h"
#include "LinearFitData.h"

using namespace std;

class CLineInfo
{
public:
	CLineInfo(void);
	~CLineInfo(void);

	CLineInfo(const CLineInfo& lineInfo);
	void operator=(const CLineInfo& lineInfo);

	void AddDot(CMyPoint& pt);

	int SetLinearFitData(LINEARFITDATA* pLinearFitData);
	int SetLinearFitData(CLinearFitData* pLinearFitData);

	void SetExpressionShowState(BOOL bShowExpression);
	BOOL GetExpressionShowState();
	BOOL BeHaveFitData();
	BOOL GetDotXYMinMax(double& minX,double& maxX,double& minY,double& maxY);	//获取表达式拟合前数据点最大值最小值
	BOOL GetLineXYMinMax(double& minX,double& maxX,double& minY,double& maxY);	//获取表达式拟合后线段数据点最大值最小值

	void SetChanId(int iChanId);
	int GetChanId();

	void SetDyeName(CString strData);
	CString GetDyeName();

	BOOL GetGeneralDotMinMax(double& minX,double& maxX,double& minY,double& maxY);

	COLORREF GetLineClr(ELINECLRMODE eGetLineClr);

	void SetBeXAxisMillisecondTime(BOOL bXAxisMsTime);
	BOOL GetBeXAxisMillisecondTime();

	CString GetEfficiencyString();	//获取字符串形式的扩增效率(默认按照百分比格式输出，保留两位小数)

	CString GetExpressionString();	//获取表达式信息

	BOOL IsExpressionVisible();	//获取“关系式是否可视”

public:
	int m_tubeId;
	int m_rowId;
	int m_colId;
	int m_chanId;
	BOOL m_bShow;
	CString m_strDyeName;	//染料名称
	CString m_strTaget;	//基因名称
	CString m_strHoleTag;	//孔位标识"A1"

	CLinkList<CMyPoint> m_lnkDots;
	COLORREF m_lineClr;

	static COLORREF m_staChanClr[6];	//通道默认颜色

	//记录标曲
	LINEARFITDATA m_linearFitData;
	CMyPoint m_linearMinPt;
	CMyPoint m_linearMaxPt;
	BOOL m_bShowExpression;	//是否显示曲线关系式

	double m_dEfficiency;	//扩增效率

protected:
	BOOL m_bXAxisMsTime;	//横坐标是否是毫秒时间
};


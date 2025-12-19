#pragma once

#include <GdiPlus.h>

using namespace Gdiplus;

class CThreLineInfo
{
public:
	CThreLineInfo(void);
	~CThreLineInfo(void);

	CThreLineInfo(const CThreLineInfo& threLineInfo);
	void operator=(const CThreLineInfo& threLineInfo);

	void SetThreLineColor(Color clrThreLine);
	Color GetThreLineColor();
	void SetBeShow(BOOL bShow);
	BOOL GetBeShow();
	void SetBeFirstDrawThreLline(BOOL bFstDrawThreLine);
	BOOL GetBeFirstDrawThreLline();
	void SetThreLineValue(double threLineValue);
	double GetThreLineValue();

public:
	int m_threLinePixelH;	//阈值线对应的像素高度
	int m_threLineNormalWidth;	//阈值线普通状态宽度
	int m_threLineHighLightWidth;	//阈值线高亮状态宽度
	int m_threLineDiscernHalfH;	//阈值线识别（扩展半径，线宽 + 扩展半径 * 2）
	
	Color m_clrThreLine;
	CRect m_rcThreLine;

	BOOL m_bThreLineSel;
	BOOL m_bShow;
	BOOL m_bFstDrawThreLine;

	double m_dThre;	//手动阈值
	double m_dThreAuto;	//自动阈值

	BOOL m_bAutoThre;
};


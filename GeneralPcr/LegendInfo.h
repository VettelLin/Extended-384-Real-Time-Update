#pragma once

#include "SingleLgdInfo.h"
#include "FontSet.h"
#include "LinkList.h"
#include <GdiPlus.h>
using namespace Gdiplus;

class CLegendInfo: public CFontSet
{
public:
	CLegendInfo(void);
	~CLegendInfo(void);

	CLegendInfo(const CLegendInfo& info);
	void operator=(const CLegendInfo& info);

			void AddSingleLegendInfo(CSingleLgdInfo& singleLgdInfo);

			int GetSingleLegendCount();
			CSingleLgdInfo* GetSingleLegendInfo(int iIndex);

	static  void ResetSegLegendText(CLegendInfo* pLgdInfo);

public:
	int m_iLeftIndent;	//图例起始位置左侧缩进
	int m_iRoundRadius;	//圆形半径
	int m_iInrIconToTx;	//图形到文本之间的间隔
	int m_iInrTxToIcon;	//文本到图形之间的间隔
	int m_iOneCharW;	//单个字符占用的宽度（平均值）

	Color m_clrFrame;	//边框颜色
	Color m_clrBk;	//背景颜色
	Color m_clrBkEnd;	//背景颜色(渐变色终点颜色)

private:
	CLinkList<CSingleLgdInfo> m_lnkSingleLgdInfo;
};


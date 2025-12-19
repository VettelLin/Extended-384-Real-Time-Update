#pragma once

#include "MaxMinInfo.h"
#include "LinkList.h"
#include "ArrowInfo.h"
#include "DataParam.h"

class CTitleInfo
{
public:
	CTitleInfo(void);
	~CTitleInfo(void);

	CTitleInfo(const CTitleInfo& info);
	void operator=(const CTitleInfo& info);

	void SetBeEditable(BOOL bEditable);
	BOOL GetBeEditable();

	void SetBeDisabled(BOOL bDisabled);
	BOOL GetBeDisabled();

	void SetDataParam(CDataParam dtParam);
	CDataParam GetDataParam();	
	int DataFilter(CString& strData);

	void SetMultiDensityState(BOOL bData);
	BOOL GetMultiDensityState();

	void SetTextAlignFormat2(DWORD dwData);
	DWORD GetTextAlignFormat2();

public:
	CString m_strTitle;	//列标题
	BOOL m_bSelected;	//标识是否被选中
	COLORREF m_clrBkSlected;	//选中效果背景色
	COLORREF m_clrBkUnselected;	//未选中效果背景色
	int m_colWidth;	//列宽
	DWORD m_dwTextAlign;	//文本对齐方式
	DWORD m_dwTxAlign2;	//文本对齐方式2

	BOOL m_bCheckFunc;	//标识是否是可勾选列

	BOOL m_bColorFunc;	//标识是否是显示颜色块
	BOOL m_bClrEditale;	//标识颜色块的颜色是否可修改

	BOOL m_bCbtable;	//标识列是否可使用下拉框编辑
	BOOL m_bCbEditable;	//标识下拉框是否可手动输入
	BOOL m_bTellParentCellChanged;	//是否通知父窗口单元格信息改变，CcellInfo暂不保存该信息,20220302

	CMaxMinInfo m_maxMinInfo;	//记录列共有的最大最小值信息

	CArrowInfo m_arrowInfo;	//记录下拉框箭头参数信息
	CLinkList<CString> m_lnkCbStr;	//记录下拉框包含的所有选择项
	int m_iCbTxLeftIndent;	//下拉框文本输出左侧缩进
	int m_iCbTxRightIndent;	//下拉框文本输出右侧缩进

protected:
	BOOL m_bEditable;	//标识列是否可使用编辑框编辑
	BOOL m_bDisabled;	//标识列是否是禁用状态

	CDataParam m_dtParam; 

	BOOL m_bMultiDensity;	//标识“是否在同一列显示多个浓度”
};


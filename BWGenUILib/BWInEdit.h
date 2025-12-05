#pragma once

#include <Float.h>

// CBWInEdit

class AFX_EXT_CLASS CBWInEdit : public CEdit
{
	DECLARE_DYNAMIC(CBWInEdit)

public:
	CBWInEdit(int m_iItem, int iSubItem, CString strText);
	virtual ~CBWInEdit();

	void EnableNumEdit(double dMinValue=-DBL_MAX, double dMaxValue=DBL_MAX);
	void SetPrecision(UINT nInPrecision){m_nPrecision=nInPrecision;}
	void SetUserFont(LOGFONT lf);
	BOOL IsTabDown(){return m_bTabDown;}
	void SetFlag(UINT nFlag) { m_nFlag = nFlag; }
	CString GetInitText() { return m_strInitText; }

protected:
	int m_iItem; // 数据所在行数，可以表示其它含义
	int m_iSubItem;// 每行数据列数，可以表示其它含义
	UINT m_nFlag; // 附加标记
	CString m_strInitText;// 编辑的初始字符串

	BOOL m_bIsNumEdit;  // 表明是否是数字编辑
	double m_dMinValue; // 最小值
	double m_dMaxValue; // 最大值
	UINT m_nPrecision; // 浮点数的精度

	CFont* m_pFont; 
	BOOL m_bTabDown;
	BOOL m_bEscPressed; ///< 表明是否Esc键被输入


protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};



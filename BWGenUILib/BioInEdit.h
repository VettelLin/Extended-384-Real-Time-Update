#pragma once

#include <Float.h>

// CBioInEdit

/**
 * \ingroup ScanSolution
 *
 * 完成在列表控件或树型控件中的文本编辑，发送消息到父类，父类只能是ListCtrl或TreeCtrl。
 * 如果是数字编辑，只能处理整数
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 2005-01-21
 *
 * \author 车从俊
 *
 * \par license
 * 本产品代码归北京博奥生物芯片有限责任公司所有。
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CBioInEdit : public CEdit
{
	DECLARE_DYNAMIC(CBioInEdit)

public:
	CBioInEdit(int nItem, int nSubItem, CString strText);
	CBioInEdit(HTREEITEM hItem, int nSubItem, CString strText);

	virtual ~CBioInEdit();

	CString GetInitText();

	void EnableNumEdit(double dMinValue=-DBL_MAX, double dMaxValue=DBL_MAX);
	void EnableMinus(){m_bIsMinus=TRUE;}
	void SetPrecision(UINT nInPrecision){m_nPrecision=nInPrecision;}

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	void SetUserFont(LOGFONT lf);
	
	BOOL IsTabDown(){return m_bTabDown;}

protected:
// Attributes

	/** \brief 父窗口类型集合
	*
	* 表示父窗口的类型，当前只完成ListCtrl、TreeCtrl
	*/
	enum eParentType
	{		
		TYPE_LISTCTRL = 0, ///< 列表控件
		TYPE_TREECTRL = 1  ///< 树行控件
	};
	eParentType m_nParentType;///< 父窗口的类型，需要据此判断发送的消息类型

	
	int m_nItem; ///< 当父窗口是ListCtrl时的行数
	int m_nSubItem;///< 每行数据列数

	CString m_strInitText;///< 编辑的初始字符串
	HTREEITEM m_hItem;///< 当父窗口是TreeCtrl时的项句柄

	BOOL m_bIsNumEdit;  ///< 表明是否是数字编辑
	BOOL m_bEscPressed; ///< 表明是否Esc键被输入
	BOOL m_bIsMinus; ///< 是否支持负数，默认为正数
	double m_dMinValue; ///< 最小值
	double m_dMaxValue; ///< 最大值
	UINT m_nPrecision; ///< 浮点数的精度

	CFont* m_pFont; 
	
	BOOL m_bTabDown;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};



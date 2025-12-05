#pragma once

const UINT CHILD_ITEM_KEYDOW = WM_USER + 2101; ///< 提示父类窗口处理特殊的按键事件

// CBioInComboBox

/**
 * \ingroup ScanSolution
 *
 * 完成在列表控件或树型控件中的数据项选择，发送消息到父类，父类只能是ListCtrl或TreeCtrl。
 *
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
class AFX_EXT_CLASS CBioInComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CBioInComboBox)

public:
	CBioInComboBox(int nItem, int nSubItem, CString strText, CStringArray* pArrayColText);
	CBioInComboBox(HTREEITEM hItem, int nSubItem, CString strText, CStringArray* pArrayColText);
	virtual ~CBioInComboBox();
	CString GetInitText();

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
	eParentType m_nParentType; ///< 父窗口的类型，需要据此判断发送的消息类型

	int m_nItem; ///< 当父窗口是ListCtrl时的行数
	int m_nSubItem; ///< 每行数据列数
	CString m_strInitText; ///< 编辑的初始字符串
	CStringArray* m_pArrayColText; ///< 当前可供选择数据项的集合
	HTREEITEM m_hItem; ///< 当父窗口是TreeCtrl时的项句柄

	BOOL m_bEscPressed; ///< 表明是否Esc键被输入

	CFont* m_pFont; 
	BOOL m_bTabDown;

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};



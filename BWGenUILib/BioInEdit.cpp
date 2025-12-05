// BioInEdit.cpp : implementation file
//

#include "stdafx.h"
#include "BioInEdit.h"
#include ".\bioinedit.h"


// CBioInEdit

IMPLEMENT_DYNAMIC(CBioInEdit, CEdit)

/**
 * 对象构造函数，对父窗口是ListCtrl适用
 * \param nItem 列表控件的行数
 * \param nSubItem  项的列数
 * \param strText 初始文本
 * \return 
 */
CBioInEdit::CBioInEdit(int nItem, int nSubItem, CString strText)
{
	m_nParentType = TYPE_LISTCTRL;
	m_nItem = nItem;
	m_nSubItem = nSubItem;
	m_strInitText = strText;

	m_bIsMinus = FALSE;
	m_bEscPressed = FALSE;
	m_bIsNumEdit = FALSE;

	m_dMinValue = -DBL_MAX;
	m_nPrecision = 0;
	m_dMaxValue = DBL_MAX;

	m_bTabDown = FALSE;

	m_pFont = NULL;
}


/**
 * 对象构造函数，对父窗口是TreeCtrl适用
 * \param hItem 树型控件的项句柄
 * \param nSubItem 项的列数
 * \param strText  初始文本
 * \return 
 */
CBioInEdit::CBioInEdit(HTREEITEM hItem, int nSubItem, CString strText)
{
	m_nParentType = TYPE_TREECTRL;
	m_hItem = hItem;
	m_nSubItem = nSubItem;
	m_strInitText = strText;

	m_bEscPressed = FALSE;
	m_bIsNumEdit = FALSE;
	m_pFont = NULL;
	m_bTabDown = FALSE;
}

/**
 * 对象析构函数，无操作
 * \return 
 */
CBioInEdit::~CBioInEdit()
{
	if (m_pFont != NULL)
	{
		m_pFont->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CBioInEdit, CEdit)
	ON_WM_CREATE()
	ON_WM_CHAR()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CBioInEdit message handlers

/**
 * 对象创建，设置焦点，保存需要属性
 * \param lpCreateStruct 被创建窗口的结构信息
 * \return 0：继续创建，-1：消灭
 */
int CBioInEdit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	// Set the proper font
	if (m_pFont == NULL)
	{
		SetFont(GetParent()->GetFont());
	}else
	{
		SetFont(m_pFont);
	}

	SetWindowText(m_strInitText);
	SetFocus();
	SetSel(0, -1 );

	return 0;
}

/**
 * \brief 在字母输入时处理特殊输入
 *
 * 可以根据编辑要求的具体类型，根据不同输入的要求对输入进行过滤，如只能输入
 * 数字时，对字母输入进行忽略。
 * \param nChar  输入键的字母代码
 * \param nRepCnt 重复次数
 * \param nFlags 标记
 */
void CBioInEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if( nChar == VK_ESCAPE || nChar == VK_RETURN)
	{
		if( nChar == VK_ESCAPE )
			m_bEscPressed = TRUE;

		GetParent()->SetFocus();
		return;
	}

	if (m_bIsNumEdit)
	{
		if (nChar == 46/*处理小数点*/)
		{
			if (m_nPrecision != 0)
				CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else if (m_bIsMinus && nChar == 45/*处理负号*/) 
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		else if (isdigit(nChar) || nChar == VK_BACK || nChar == VK_DELETE || nChar == VK_SPACE)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
		}
		return;
	}

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

/**
 * 在失去焦点时向父窗口发送修改数据消息
 * \param pNewWnd 得到焦点的窗口指针
 */
void CBioInEdit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CString strText;
	GetWindowText(strText);

	strText.TrimLeft();
	strText.TrimRight();
	if (m_bIsNumEdit)
	{	
		double dValue = _tstof(strText);
		if (dValue < m_dMinValue)
			dValue = m_dMinValue;
		else if (dValue > m_dMaxValue)
			dValue = m_dMaxValue;
		CString strFormat;
		strFormat.Format(_T("%%.%df"), m_nPrecision);
		strText.Format(strFormat, dValue);
	}

	if (m_nParentType == TYPE_LISTCTRL)
	{
		LV_DISPINFO dispinfo;
		dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		dispinfo.hdr.idFrom = GetDlgCtrlID();
		dispinfo.hdr.code = LVN_ENDLABELEDIT;
		
		dispinfo.item.mask = LVIF_TEXT;
		dispinfo.item.iItem = m_nItem;
		dispinfo.item.iSubItem = m_nSubItem;
		dispinfo.item.pszText = m_bEscPressed ? LPTSTR((LPCTSTR)m_strInitText) : LPTSTR((LPCTSTR)strText);
		dispinfo.item.cchTextMax = m_bEscPressed ? m_strInitText.GetLength() : strText.GetLength();
		
		GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
	}
	else 
	{
		NMTVDISPINFO  dispinfo;
		dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		dispinfo.hdr.idFrom = GetDlgCtrlID();
		dispinfo.hdr.code = TVN_ENDLABELEDIT;
		
		dispinfo.item.mask = LVIF_TEXT;
		dispinfo.item.hItem = m_hItem;
		dispinfo.item.pszText = m_bEscPressed ?  LPTSTR((LPCTSTR)m_strInitText) : LPTSTR((LPCTSTR)strText);
		dispinfo.item.cchTextMax = m_bEscPressed ? m_strInitText.GetLength() : strText.GetLength();
		dispinfo.item.lParam = m_nSubItem; // 列序号
		
		GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
	}
}


/**
 * 设置是否是数字编辑
 * \param bFlag 标记，真为数字编辑
 */
void CBioInEdit::EnableNumEdit(double dMinValue, double dMaxValue)
{
	m_bIsNumEdit = TRUE;
	m_dMinValue = dMinValue;
	m_dMaxValue = dMaxValue;
}

/**
 * 取得编辑框初始文本
 * \return 编辑框初始文本
 */
CString CBioInEdit::GetInitText()
{
	return m_strInitText;
}

void CBioInEdit::SetUserFont(LOGFONT lf)
{
	if (m_pFont == NULL)
	{
		m_pFont = new CFont();
	}
	else
	{
		m_pFont->DeleteObject();
	}

	m_pFont->CreateFontIndirect(&lf);
}

void CBioInEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB)
	{
		m_bTabDown = TRUE;
		GetParent()->SetFocus();
		return;
	}

	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

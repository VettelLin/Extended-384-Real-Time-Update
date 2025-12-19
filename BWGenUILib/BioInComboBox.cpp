// BioInComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "BioInComboBox.h"
#include ".\bioincombobox.h"


// CBioInComboBox

IMPLEMENT_DYNAMIC(CBioInComboBox, CComboBox)

/**
 * 对象构造函数，对父窗口是ListCtrl适用
 * \param nItem 列表控件的行数
 * \param nSubItem 项的列数
 * \param strText 初始文本
 * \param pArrayColText 可供选择的数据项数组
 * \return 
 */
CBioInComboBox::CBioInComboBox(int nItem, int nSubItem, CString strText, CStringArray* pArrayColText)
{
	m_nParentType = TYPE_LISTCTRL;
	m_nItem = nItem;
	m_nSubItem = nSubItem;
	m_strInitText = strText;
	m_pArrayColText = pArrayColText;

	m_bEscPressed = FALSE;
	m_pFont = NULL;
	m_bTabDown = FALSE;
}

/**
 * 对象构造函数，对父窗口是TreeCtrl适用
 * \param hItem 树型控件的项句柄
 * \param nSubItem 项的列数
 * \param strText 初始文本
 * \param pArrayColText 可供选择的数据项数组
 * \return 
 */
CBioInComboBox::CBioInComboBox(HTREEITEM hItem, int nSubItem, CString strText, CStringArray* pArrayColText)
{
	m_nParentType = TYPE_TREECTRL;
	m_hItem = hItem;
	m_nSubItem = nSubItem;
	m_strInitText = strText;
	m_pArrayColText = pArrayColText;

	m_bEscPressed = FALSE;

	m_pFont = NULL;
	m_bTabDown = FALSE;
}

/** 
 * 对象析构函数，无操作
 * \return 
 */
CBioInComboBox::~CBioInComboBox()
{
	if (m_pFont != NULL)
	{
		m_pFont->DeleteObject();
		delete m_pFont;
		m_pFont = NULL;
	}
}


BEGIN_MESSAGE_MAP(CBioInComboBox, CComboBox)
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CBioInComboBox message handlers


/**
 * 对象创建，设置焦点，保存需要属性
 * \param lpCreateStruct 被创建窗口的结构信息
 * \return 0：继续创建，-1：消灭
 */
int CBioInComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pFont == NULL)
	{
		SetFont(GetParent()->GetFont());
	}else
	{
		SetFont(m_pFont);
	}

	for (int i=0; i<m_pArrayColText->GetSize(); i++)
	{
		AddString(m_pArrayColText->GetAt(i));
	}

	SelectString(-1, m_strInitText);
	SetFocus();

	return 0;
}

/**
 * 在失去焦点时向父窗口发送修改数据消息
 * \param pNewWnd 得到焦点的窗口指针
 */
void CBioInComboBox::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

	CString strText;
	int nSel = GetCurSel();
	if (nSel == -1)
		strText.Empty();
	else
        GetLBText(nSel, strText);
	
	// Send Notification to parent
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
		
		GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
	}
	else 
	{
		NMTVDISPINFO  dispinfo;
		dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
		dispinfo.hdr.idFrom = GetDlgCtrlID();
		dispinfo.hdr.code = TVN_ENDLABELEDIT;
		
		dispinfo.item.mask = LVIF_TEXT;
		dispinfo.item.hItem = m_hItem;
		dispinfo.item.pszText = m_bEscPressed ? LPTSTR((LPCTSTR)m_strInitText) : LPTSTR((LPCTSTR)strText);
		dispinfo.item.cchTextMax = m_bEscPressed ? m_strInitText.GetLength() : strText.GetLength();
		dispinfo.item.lParam = m_nSubItem; // 列序号
		
		GetParent()->SendMessage( WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo );
	}
}

/**
 * 取得选择框的初始选择文本
 * \return 选择框初始文本
 */
CString CBioInComboBox::GetInitText()
{
	return m_strInitText;
}

void CBioInComboBox::SetUserFont(LOGFONT lf)
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

void CBioInComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_TAB)
	{
		m_bTabDown = TRUE;
		GetParent()->SetFocus();
		return;
	}
	else if (nChar == VK_RETURN)
	{
		GetParent()->SetFocus();
		return;
	}

	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

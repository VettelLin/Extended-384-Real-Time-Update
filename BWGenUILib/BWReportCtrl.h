#pragma once

#include <afxtempl.h>
#include "BioInEdit.h"
#include "BioInComboBox.h"
#include "BWColorWnd.h"


// CBWReportCtrl
const CString CLASSNAME_REPORTCTRL =  _T("BWReportCtrl");
const UINT BWREPORT_SEL_CHANGE = WM_USER + 2001; // 行选择改变，第1个参数是选择行数，第2个参数保存行号的UINT*指针
//const UINT REPORT_ITEM_CHANGE = WM_USER + 2002; // 项数据改变

/**
 * \ingroup PublicLib
 * 列表类，类似于MFC的CListCtrl，提供排序功能，可以按行、列设置颜色
 */
class AFX_EXT_CLASS CBWReportCtrl : public CWnd
{
	DECLARE_DYNAMIC(CBWReportCtrl)

public:
	CBWReportCtrl();
	virtual ~CBWReportCtrl();

	enum eColType
	{
		DATA_INTEGER = 0,
		DATA_FLOAT = 1,
		DATA_TEXT = 2
	};


	// 一个数据项的类
	class CReportItem : public CObject
	{
	public:
		CReportItem()
		{
			m_nFormat = DT_VCENTER | DT_SINGLELINE | DT_LEFT;
			m_strText.Empty();
			m_clrBkg = RGB(255, 255, 255);
			m_clrText = RGB(0, 0, 0);
			m_bEnable = TRUE;
			m_clrFill = RGB(228, 228, 228);
			m_iStatus = 0;
		}

		virtual ~CReportItem(){};

		// Attributes
		UINT		m_nFormat; // 文本格式
		CString		m_strText; // 文本数据
		COLORREF	m_clrBkg; // 背景色
		COLORREF	m_clrText; // 文本颜色
		BOOL	m_bEnable;
		COLORREF	m_clrFill; // 特殊状态时的填充色
		int		m_iStatus; // 状态，特殊栏使用，如CHECK
	};

	// 一个数据行的类，包括多个数据项
	class CReportRowItem
	{
	public:
		CReportRowItem()
		{
			m_clrBkg = RGB(255, 255, 255);
			m_dwParam = 0;
			m_bSelected = FALSE;
		}
		virtual ~CReportRowItem()
		{
			for (int j=0; j<m_arrayItems.GetSize(); j++)
			{
				CReportItem* pItemData = m_arrayItems.GetAt(j);
				if (pItemData != NULL)
				{
					delete pItemData;
				}
			}
			m_arrayItems.RemoveAll();
		}

		int AddChild(CReportItem* pItem){return (int)m_arrayItems.Add(pItem);}
		CReportItem* GetChildAt(int nIndex){return m_arrayItems.GetAt(nIndex);}
		int	GetChildCount(){return (int)m_arrayItems.GetSize();}

		CTypedPtrArray<CPtrArray, CReportItem*>  m_arrayItems;
		// Attributes
		COLORREF	m_clrBkg; // 背景色
		DWORD		m_dwParam;
		BOOL		m_bSelected; // 选中
	};

	typedef CTypedPtrArray<CPtrArray, CReportRowItem*> CReportDataArray;

	enum eSortType
	{
		SORT_NO = 0,
		SORT_ASCEND = 1,
		SORT_DESCEND = 2
	};
	
	struct tagReportColData
	{
		BOOL	bResize; // 宽度可改变
		UINT	nType; // HDI_BITMAP, HDI_TEXT
		CString	strText;
		int		iWidth;
		int		iImage; // 图像列表的序号
		UINT	nFormat; // DT_VCENTER | DT_SINGLELINE | DT_LEFT;
		BOOL	bClicked; // 是否已被按下
		eSortType	nSortType; // 是否排序及排序类型
		DWORD	dwParam;
		tagReportColData()
		{
			bResize = FALSE;
			nType = HDI_TEXT;
			strText.Empty();
			iWidth = 0;
			iImage = -1;
			nFormat = DT_VCENTER | DT_SINGLELINE | DT_LEFT;
			bClicked = FALSE;
			nSortType = SORT_NO;
			dwParam = 0;
		}
	};


	/** \brief 子项编辑时对象类型集合
	* 表示子窗口的类型，当前只完成文本编辑、数字编辑和列表选择
	*/
	enum eListItemType
	{
		LIST_ITEM_ENABLE = 0, // 有效
		LIST_ITEM_DISABLE, // 无效
		LIST_ITEM_EDIT, // 文本编辑
		LIST_ITEM_NUMEDIT, // 数字编辑
		LIST_ITEM_COMBO, // 列表选择， CBS_DROPDOWNLIST
		LIST_ITEM_CHECK, // 复选框
		LIST_ITEM_COLOR, // 颜色框
		LIST_ITEM_DELETE, // 删除框
	};

	// 列表子项编辑时对象类型信息集合，表明特殊子项的处理信息
	struct sListColAttribute
	{
		int nCol; // 列表的列数
		int nRow; // 行
		eListItemType nType; //  子项创建对象类型，只能时eListItemType中的一种
		int nMin;
		int nMax;
		int iDataCount; // 当子项对象为ComboBox时可供选择的数据项总数
		DWORD dwParam;// 当子项对象为ComboBox时可供选择的数据项集合
		sListColAttribute(){
			nCol = -1;
			nRow = -1;
			nType = LIST_ITEM_EDIT;
			nMin = 0;
			nMax = 0;
			iDataCount = 0;
			dwParam = 0;
		};
	};

	// 子项对象信息数组
	CArray<sListColAttribute,sListColAttribute&> m_arrayColAttr;

// Operations
public:
	void RemoveAll(BOOL bHasCol=TRUE, BOOL bHasAttri=TRUE);
	// 设定标题栏数据
	int AddColumn(CString strText, int nWidth);
	int AddColumn(CString strText, int nWidth, UINT nType, UINT nFormat, int iImage=-1);
	void SetColumnFormat(int nIndex, UINT nFormat);
	UINT GetColumnFormat(int nIndex);
	void SetColumnType(int nIndex, int nType);
	void SetColumnResize(int nIndex, BOOL bEnable);
	void SetColumnText(int nIndex, CString strText);
	void SetColumnData(int nIndex, DWORD dwParam);
	DWORD GetColumnData(int iIndex);

	// 设定行数据
	int AddRow(CString strText);
	int InsertRow(int nRow, CString strText);
	void RemoveRow(int nRow);
	void SetItemFormat(int nRow, int nSubItem, UINT nFormat = DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	CString GetItemText(int nRow, int nCol);
	void SetItemText(int nRow, int nSubItem, CString strText);
	void SetItemBkgColor(int nRow, int nSubItem, COLORREF clrBkg);
	void SetItemTextColor(int nRow, int nSubItem, COLORREF clrText);
	void SetItemEnable(int nRow, int nSubItem, BOOL bEnable);
	void SetItemFillColor(int nRow, int nSubItem, COLORREF clrFill);
	COLORREF GetItemFillColor(int iRow, int iSubItem);
	void SetItemStatus(int nRow, int nSubItem, int iStatus);
	int GetItemStatus(int iRow, int iSubItem);
	
	int GetColCount(){return m_iColCount;}
	int GetRowCount(){return m_iRowCount;}

	void SetRowColor(int nRow, COLORREF clrBkg);

    int  GetColumnWidth(int iCol) const;
	CString GetColumnName(int iCol) const;

	// 处理列表状态
	void EnableSort(BOOL bEnable){m_bEnableSort=bEnable;}
	void EnableEdit(BOOL bEnable){m_bEnableEdit=bEnable;}
	void EnableMultiSelect(BOOL bEnable) {m_bEnableMultiSelected=bEnable;}
	virtual BOOL CanEdit(int nRow, int nCol);

	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	// 设置列表项为ComboBox 控件
	void SetColumnComboBox(int nCol, int nRow, CStringArray &arrayColText);
	// 设置列表项为数字编辑控件
	void SetColumnNumEdit(int nCol, int nRow=-1, int iPrecision=0, int nMin=0, int nMax=INT_MAX);
	// 设置列表项为不可编辑
	void SetColumnDisable(int nCol, int nRow=-1);
	void SetColumnCheckBox(int iCol, int iRow=-1);
	void SetColumnColor(int iCol, int iRow=-1, int iColorCount=0, COLORREF* pInputColor=NULL);
	void SetColumnDelete(int iCol, int iRow=-1);

	int GetSelectedRow();
	int GetSelectedItems(UINT* &pOutItems);
	void SelectAll();
	void SetSelectedRow(int nRowNo);
	void SetSelectedItems(int nCount, UINT* pOutItems);
	
	// 保存一行数据的参数
	BOOL SetRowData(int nRow, DWORD dwData);
	DWORD GetRowData(int nRow);

	// 打印
	void Print(CDC* pDC, const RECT& rect, float f);
	void PrintItem(CDC* pDC, const RECT& rect, CReportItem* pItemData, BOOL IsFirst = FALSE);
	void PrintHeadItem(CDC* pDC, const RECT& rect, CString strText, UINT format, BOOL IsFirst = FALSE);
	void PrintLine(CDC* pDC, CPoint ptStart, CPoint ptEnd, COLORREF clrPen = RGB(0,0,0));

protected:
	BOOL RegisterWindowClass();
	
	// 绘制标题栏
	virtual void DrawHeader(CDC* pDC, CRect rectHeader, UINT nStartCol);
	virtual void DrawColItem(CDC* pDC, CRect rectItem, tagReportColData* pColData, BOOL bIsFirst=FALSE);
	// 绘制行数据
	virtual void DrawRow(CDC* pDC, CRect rectRow, CReportRowItem* pRowData, UINT nStartCol, BOOL bIsLocked=FALSE, BOOL bIsSelected=FALSE);
	virtual void DrawRowItem(CDC* pDC, int iRow, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected);
	virtual void DrawItemFrame(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected, BOOL bDisable=FALSE);
	virtual void DrawTextItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked=FALSE, BOOL bIsSelected=FALSE, BOOL bDisable=FALSE);
	virtual void DrawCheckBoxItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected);
	virtual void DrawColorItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected);
	virtual void DrawDeleteItem(CDC* pDC, CRect rectItem, CReportItem* pItemData, BOOL bIsLocked, BOOL bIsSelected);
	virtual void DrawText(CDC* pDC, CRect rect, CString strText, UINT nFormat=DT_VCENTER | DT_SINGLELINE | DT_LEFT);
	virtual void DrawSortIcon(CDC* pDC, CRect rectIcon, UINT nSortType);
	CString GetTextWithDots(CDC* pDC, CString strText , int nWidth);
	
	// 公用绘制
	void DrawLine(CDC* pDC, CPoint ptStart, CPoint ptEnd, COLORREF clrPen=RGB(0,0,0));

	void ResetScrollBars();
	void RecalculateColWidth(CRect recClient);

	int GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	UINT GetMouseScrollLines();
	CSize GetTopleftNonFixedIndex() const;

    int  GetFixedRowHeight() const;
    int  GetFixedColumnWidth() const;
    long GetVirtualWidth() const;
    long GetVirtualHeight() const;
 	
	// 标题栏宽度修改
	BOOL PtInColSizingRect(CPoint point);
	CRect GetColSizingRect(long lOffset);
	void DrawSplittingLine(long lHorzPos);

	// 行选择
	long GetRowIndexBy(CPoint point);
	int GetColIndexBy(CPoint point);
	// 列选择
	void SetColSortState(int nCol, BOOL bIsAdd=FALSE);
	void SetColClickedState(int nCol, BOOL bIsClicked);

	// 排序
	void SortRows(int nSortCol);
	void SortRowsByRange(int nSortCol, int nBeginRow, int nEndRow, UINT nSortType, BOOL bIsIncludeEnd=FALSE);
	void MergePass(CReportDataArray* pData1, CReportDataArray* pData2, int s, int n, UINT nSortCol, UINT nSortType);
	void Merge(CReportDataArray* pData1, CReportDataArray* pData2, int l, int m, int r, UINT nSortCol, UINT nSortType);
	int CompareReportItem(CReportDataArray* pData, UINT nSortCol, UINT nIndex1, UINT nIndex2);

	void GetOtherSortIndex(CUIntArray* pSortIndexArray, int nSortCol);
	UINT GetNextDifferentDataIndex(UINT nBeginRow, CUIntArray* pSortIndexArray);
	BOOL IsTwoItemDataSameOfIndex(CReportRowItem* pRowData1, CReportRowItem* pRowData2, CUIntArray* pSortIndex);

	// 处理列表编辑 －－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－
	void HandleEditSubitem(int nIndex, int nCol, int nType, sListColAttribute* pColAttr);// 对Edit 进行编辑	
	void HandleComboBoxSubitem(int nIndex, int nCol, sListColAttribute* pColAttr);// 对ComboBox 进行选择
	void HandleCheckBoxSubitem(int nIndex, int nCol, sListColAttribute* pColAttr);// 对CheckBox 进行选择
	void HandleColorSubitem(int nIndex, int nCol, sListColAttribute* pColAttr);// 设置颜色
	void HandleDeleteSubitem(int nIndex, int nCol, sListColAttribute* pColAttr);// 删除
 
	// 取得给定行数和列数的列表项的区域
	CRect GetSubItemRect(int nIndex, int nCol);
	virtual int GetItemTypeBy(int nCol, int nRow, sListColAttribute* &pColAttr);
	int FindItemBy(int nType, int nCol, int nRow);
	int FindItemBy(int nCol, int nRow);


	BOOL IsRowSelected(UINT nRow);
	void HandleRowSelected(UINT nFlags, int nNewRow);

	virtual void SendMessageOfSelectChanged();
	virtual void DeleteSelectedRows();

	CReportItem* GetRowItem(int iRow, int iCol);

// Attributes
public:
	void SetColumnColor(COLORREF clrHeader){m_clrHeader = clrHeader;}
	void EnableAutoFilling(BOOL bEnable) { m_bAutoFilling = bEnable; }
	int  GetRowHeight(int iRow) const;
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
	void SetMsgID(UINT nID) { m_nMsgID=nID; }
	void SetColLockedCount(UINT nCount){m_iFixedColCount=nCount;}
	void SetRowLockedCount(UINT nCount){m_iFixedRowCount=nCount;}
	void SetBkgColor(COLORREF clrBkg){m_clrBkg = clrBkg;}

	void SetOddEvenColor(COLORREF clrOdd, COLORREF clrEven);
	void SetSelectedColor(COLORREF clrSelected) { m_clrSelected = clrSelected; }

	void SetHeaderHeight(int nHeaderHeight) { m_iHeaderHeight = nHeaderHeight; }
	void SetRowHeight(int nRowHeight) { m_iRowHeight = nRowHeight; }

protected:
	UINT	m_nMsgID; // 发送消息时的ID

	// 标题栏属性
	BOOL	m_bHasHeader; // 是否有标题栏
	BOOL	m_bAutoFilling; // 水平方向自动充满
	int		m_iHeaderHeight; // 标题栏的高度
	int		m_iMinColWidth; // 最小的栏宽度
	CPtrArray m_arrayColData; // 标题栏的数据
	COLORREF m_clrHeader;
	CFont	m_fontHeader; // 标题栏字体
	int		m_iColCount; // 列数
	
	// 报表属性
	int		m_iRowHeight; // 数据行的高度
	int		m_iFixedColCount; // 默认从左开始锁定
	int		m_iFixedRowCount; // 默认从上开始锁定
	COLORREF m_clrBkg; // 背景色
	CFont	m_fontItem; // 内容字体

	BOOL	m_bOddEvenColor;	// 是否使用奇偶色
	COLORREF m_clrOdd;	// 奇数行背景色
	COLORREF m_clrEven;	// 偶数行背景色
	COLORREF m_clrSelected; // 选中行的背景色

	// 报表数据
	CUIntArray m_arrayRowHeight; ///< 行的高度数组
    CReportDataArray m_arrayAllData; ///< 行数据数组
	int		m_iRowCount; ///< 行数

	CSize	m_sizeVirtual; ///< 实际report的滚动范围
	
	BOOL	m_bEnableMultiSelected; // 是否支持多选
	int		m_iLastSelectedRow; // 最后选择行，方便行多选时使用

	BOOL	m_bIsColClicked; // 标题栏被按下
	int		m_iColClickedIndex; 

	// 修改标题栏宽度
	BOOL	m_bIsColSizing; //  是否正在修改栏宽度
	int		m_iSizingColIndex; // 修改栏宽度时的栏序号
	long	m_lSizingColLeft; // 修改栏的最左边Client值
	long	m_lDrawingPos;

	// 当前状态
	BOOL	m_bEnableSort; //  是否支持排序
	BOOL	m_bEnableEdit; //  是否支持编辑等修改内容操作

	CBioInEdit*	m_pInEdit; // 当前编辑对象指针，当子项进行文本（数字）编辑时使用
	CBioInComboBox* m_pInComboBox;// 当前编辑对象指针，当子项进行数据选择时使用
	CBWColorWnd*	m_pColorWnd; // 颜色选择窗口
	int	m_iColorWndWidth;
	int	m_iColorWndHeight;

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage( MSG* pMsg );
};



#pragma once

/**
 * 处理控件位置大小改变的容器。
 * 特别注意，添加时按先上后下，先左后右的顺序添加，如果存在水平、垂直方向完全改变的控件，应该在最后添加
 */
class AFX_EXT_CLASS CResizeContainer : public CObject
{
public:
	CResizeContainer();
	virtual ~CResizeContainer();

	enum eResizeType
	{
		LEFT_SIZE_NO, // 靠左，不改变大小
		LEFT_SIZE_H, // 靠左，水平放缩
		LEFT_SIZE_V, // 靠左，上下放缩
		LEFT_SIZE_HV, // 靠左，水平上下放缩

		RIGHT_SIZE_NO, // 靠右，不改变大小
		RIGHT_SIZE_H, // 靠右，水平放缩
		RIGHT_SIZE_V, // 靠右，上下放缩
		RIGHT_SIZE_HV, // 靠右，水平上下放缩
	};

	enum eResizeFillType
	{
		FILL_NO_RESIZE, // 不改变大小的填充
		FILL_RESIZE_H, // 水平充满
		FILL_RESIZE_V, // 垂直充满
		//FILL_RESIZE_M, // 中间充满，三个以上一组控件
		FILL_RESIZE_LEFT, // 左充满
	};

	enum eDockType
	{
		DOCK_TOP = 0,
		DOCK_BOTTOM,
	};

	// CResizeContainer command target

	class CResizeItem : public CObject
	{
	public:
		CResizeItem(CWnd* pWnd, CRect rect, eResizeType nType, int iIndent=0);
		virtual ~CResizeItem();

	// Operations
	public:
		virtual void SetRect(CRect rect);
		CRect GetRect() { return m_rect; }
		virtual BOOL FindAndSetShow(CWnd* pInputWnd, BOOL bShow);
		BOOL GetShow() { return m_bShow; }
		void SetNewRow(BOOL bNew) { m_bNewRow = bNew; }
		BOOL GetNewRow() { return m_bNewRow; }
		eResizeType GetResizeType() { return m_nResizeType; }

	protected:
		CRect GetPosition(CRect rect);
		void MoveWindow(CWnd* pWnd, CRect rect);

	protected:
		CWnd*	m_pResizeWnd; // 需要改变大小的窗口
		CRect	m_rect; // 相对于父窗口的窗口位置大小
		eResizeType	m_nResizeType;
		int		m_iIndent; // 相对于停靠位置点的缩进距离
		BOOL	m_bNewRow; // 会否新起一行
		BOOL	m_bShow;
	};

	class CResizeGroup : public CResizeItem
	{
	public:
		CResizeGroup(CRect rect, eResizeType nType, eResizeFillType nFillType, int iIndent=0);
		virtual ~CResizeGroup();

	public:
		void SetChildren(CPtrArray* pChildren);
		virtual void SetRect(CRect rect);
		virtual BOOL FindAndSetShow(CWnd* pInputWnd, BOOL bShow);

	protected:
		eResizeFillType	m_nFillType;
		CPtrArray	m_arrayChildren;
		UINT	m_nSeparator; // 分割区域大小
	};

public:
	void SetParent(CWnd* pWnd) { m_pParentWnd=pWnd; }

	void SetDockType(eDockType nType) { m_nDockType = nType; }
	void SetMargin(CSize sizeLT, CSize sizeRB) { m_sizeMarginLT = sizeLT; m_sizeMarginRB = sizeRB; }
	void SetRowSpace(int iSpace) { m_iRowSpace = iSpace; }

	void SetRect(CRect rect);
	int AddChild(CWnd* pChild, eResizeType nType, int iIndent, BOOL bNewRow=TRUE);
	int AddGroupChild(CPtrArray* pChildArray, eResizeType nType, eResizeFillType nFillType, int iIndent, BOOL bNewRow=TRUE);
	CResizeItem* GetAt(int nIndex) { return (CResizeItem*)m_arrayChild.GetAt(nIndex);}

	void SetWndShow(CWnd* pInputWnd, BOOL bShow);

protected:
	CRect GetRowRect(int iStartIndex);
	int CalculateRowSpace(CRect rectArea);

protected:
	CWnd*	m_pParentWnd; // 父类窗口
	CPtrArray	m_arrayChild; // 对象指针

	eDockType m_nDockType; 
	CSize	m_sizeMarginLT; // 左上边空
	CSize	m_sizeMarginRB; // 右下边空
	int		m_iRowSpace; // 行间空白
	CSize	m_sizeResizeMax; // 窗口缩放的水平垂直最大值
};
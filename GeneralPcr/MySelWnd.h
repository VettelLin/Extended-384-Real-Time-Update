#pragma once

#include "FontSet.h"
#include "LinkList.h"
#include "ItemForSelInfo.h"
#include "MyScrollInfo.h"
#include "MySelWndPosInfo.h"

#define WM_ROWSELECTCHANGE	WM_USER + 6023	//列表框选中行改变，通知父窗口

class CMySelWnd : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMySelWnd)

public:
	CMySelWnd();
	virtual ~CMySelWnd();

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnShowWindow(BOOL bShow,UINT nStatus);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

			int DrawBody(CDC* pDC);
			int DrawVertScroll(CDC* pDC);

			void CalcScrollPos();
			int CalcRowToShow();
			int CalcRowToShow(int newStartRow);
			int CalcTotalRowHeight();
			int GetHeight(int rowStart,int rowEnd);
			int GetRowStartDrawInMemoryVertPixel(int rowId);
			void ProcessVertSrcoll(UINT nFlags, CPoint point);
			int ScrollRow(int pxVertShift);
			int ScrollRowToFixPos(int pxFixPos);
			CMySelWndDotPosInfo CalcPtPos(CPoint pt);

			int SetRowSingleSelected(int rowId,BOOL bRedraw = FALSE);

			int GetRowString(int rowId,CString& strText);
			int SetItemText(int iIndex,CString strText);
			int InsertRow(int rowId,CString strText,BOOL bRedraw = TRUE); //在rowId之前插入
			int InsertRow(CString strText,BOOL bRedraw = TRUE);
			int DeleteRow(int rowId,BOOL bRedraw = TRUE);
			int DeleteAll(BOOL bRedraw = TRUE);

			int SetSelectedRow(CString strSelectedRow,BOOL bRedraw = FALSE);	//单选
			int SetRowSelected(CString strSelectedRow,BOOL bRedraw = FALSE);	//不影响其它行选中状态

			int GetFirstSelectedRow(CString strSelectedRow);

			void SetBeAllowSameItem(BOOL bAllowSamItem);
			BOOL GetBeAllowSameItem();

			int GetItemCount();	//获取供选择的所有项的个数

private:
			int InitVertScroll();

public:
	int m_pxFrameW;
	
	CLinkList<CItemForSelInfo> m_lnkItemForSelInfo;
	CLinkList<CMySelWndDotPosInfo> m_lnkHistoryDotPosInfo;

	CMyScrollInfo m_vertScroll;

	CMySelWndDotPosInfo m_curDotPosInfo;
	CMySelWndDotPosInfo m_preDotPosInfo;

	CRect m_rcWnd;
	CRect m_rcBody;

	int m_rowNum;
	int m_cdcH;	//记录显示行的总高度（包括 顶部、底部缩进像素） CDC Height
	int m_totalRowH;	//所d有行的高度之和
	int m_rowIdStartShow;
	int m_rowIdEndShow;
	int m_iRowTopIndent;	//起始显示行顶部缩进
	int m_iRowBottomIndent;	//终止显示列底部缩进

	BOOL m_bTellParentRowSelChange;

	CWnd* m_pParent;

	int m_iLeftIndent;	//输出文字左侧缩进
	int m_iRightIndent;	//输出文字右侧缩进
	int m_iDefRowH;	//默认行高

private:
	BOOL m_bMouseFstInCtrl;	//标识鼠标是否初次在控件中
	BOOL m_bAllowSameItem;	//是否允许选项内容重复
};



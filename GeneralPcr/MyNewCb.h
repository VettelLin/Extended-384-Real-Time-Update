#pragma once
#include "MyNewEdit.h"
#include "ArrowInfo.h"
#include "MySelWnd.h"

typedef enum CbSelWndState{
	ECSWS_SHOW,ECSWS_HIDE
}ECBSELWNDSTATE;

class CMyNewCb :public CMyNewEdit
{
	DECLARE_DYNAMIC(CMyNewCb)

public:
	CMyNewCb(CWnd* pParent = NULL);
	~CMyNewCb(void);

	DECLARE_MESSAGE_MAP()

public:
	virtual afx_msg void OnPaint();
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual	int LBtDownSpecialProc();

			int GetString(int iIndex,CString& strText);
			int SetItemText(int iIndex,CString strText);
			int InsertString(int iIndex,CString strText,BOOL bRedraw = FALSE);
			int InsertString(CString strText,BOOL bRedraw = FALSE);
			int DeleteString(int iIndex,BOOL bRedraw = FALSE);
			int Clear(BOOL bRedraw = FALSE);

	afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam);	
	afx_msg LRESULT OnMsgHideSonCtrl(WPARAM wParam,LPARAM lParam);	
	afx_msg void OnKillFocus(CWnd* pNewWnd);

			int CalcSelWndHeight();
	virtual int SetText(CString strText,BOOL bRedraw = TRUE,BOOL bForbidTellParent = FALSE);

			int SetCurSel(int iIndex,BOOL bRedraw = TRUE,BOOL bForbidTellParent = FALSE);
			int GetCurSel();

			int GetItemCount();	//获取供选择的所有项的个数

			int SetDropDown();	//设置下拉状态
			int SetDropUp();	//取消下拉状态

			int RefreshText(int iIndex,BOOL bRedraw = FALSE);	//语言设置改变后，刷新文本信息

public:
	CMySelWnd m_selWnd;
	CArrowInfo m_arrowInfo;

	ECBSELWNDSTATE m_eCbSelWndState;

private:
	int m_iSelWndH;	//item选择窗口高度
	BOOL m_bFstDown;	//标识是否是第一次弹出下拉选择窗口
};


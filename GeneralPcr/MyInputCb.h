#pragma once
#include "MyNewEdit.h"
#include "ArrowInfo.h"
#include "MyCbSelWnd.h"

typedef enum InputCbSelWndState{
	EICSWS_SHOW,EICSWS_HIDE
}EINPUTCBSELWNDSTATE;

class CMyInputCb :public CMyNewEdit
{
	DECLARE_DYNAMIC(CMyInputCb)

public:
	CMyInputCb(CWnd* pParent = NULL);
	~CMyInputCb(void);

	DECLARE_MESSAGE_MAP()

public:
	virtual afx_msg void OnPaint();
	virtual afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual	int LBtDownSpecialProc();

			int GetString(int iIndex,CString& strText);
			int InsertString(int iIndex,CString strText,BOOL bRedraw = FALSE);
			int DeleteString(int iIndex,BOOL bRedraw = FALSE);
			int Clear(BOOL bRedraw = FALSE);

	afx_msg LRESULT OnMsgRowSelectedChange(WPARAM wParam,LPARAM lParam);	
	afx_msg LRESULT OnMsgHideSonCtrl(WPARAM wParam,LPARAM lParam);	
	afx_msg void OnKillFocus(CWnd* pNewWnd);

			int CalcSelWndHeight();
	virtual int SetText(CString strText,BOOL bRedraw = TRUE,BOOL bForbidTellParent = FALSE);

			int SetCurSel(int iIndex,BOOL bRedraw = TRUE,BOOL bForbidTellParent = FALSE);
			int GetCurSel();

			int SetDropDown();	//设置下拉状态
			int SetDropUp();	//取消下拉状态

			afx_msg LRESULT OnMsgDeleteRowString(WPARAM wParam,LPARAM lParam);
			afx_msg LRESULT OnMsgResetWindowSize(WPARAM wParam,LPARAM lParam);
			void EnableRowDelete(BOOL bEnable);
public:
	CMyCbSelWnd m_selWnd;
	CArrowInfo m_arrowInfo;

	EINPUTCBSELWNDSTATE m_eCbSelWndState;

private:
	int m_iSelWndH;	//item选择窗口高度
	BOOL m_bFstDown;	//标识是否是第一次弹出下拉选择窗口
};


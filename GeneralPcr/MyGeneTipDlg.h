
#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyCheckButton.h"
#include "RectManage.h"
#include "resource.h"

class CMyGeneTipDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CMyGeneTipDlg)

public:
	CMyGeneTipDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMyGeneTipDlg();

	enum {IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateCkBt();
			void CreateSt();
			void CreateEd();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitCkBt();
			void InitSt();
			void InitEd();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

			void SetNoteInfo(CString strNoteInfo);
			CString GetNoteInfo();

			void SetGeneTipType(EGENETIPTYPE eGeneType);
			EGENETIPTYPE GetGeneTipType();

			void SetNoteInfoStyle(DWORD style);
			DWORD GetNoteInfoStyle();

			BOOL SetRoundRect(int x = 5,int y = 5);

			void CheckNoteInfoSize();

public:
	CMyButton m_btOk;
	CMyButton m_btCancel;

	CMyStatic m_stNoteInfo;
	CMyStatic m_stIcon;

	int m_retCode;	//弹窗返回值

private:
	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	CString m_strNoteInfo;	//提示信息

	int   m_noteFontSize;//单位为磅
	DWORD m_dwNoteStyle;
	int m_width;
	int m_height;
	int m_iCapHeight;

	EGENETIPTYPE m_eGeneTipType;

	CRectManage m_rcManage;
};

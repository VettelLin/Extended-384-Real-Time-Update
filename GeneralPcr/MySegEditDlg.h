
#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyNewEdit.h"
#include "MyCheckButton.h"
#include "MySegEditInfo.h"
#include "RectManage.h"
#include "resource.h"

class CMySegEditDlg : public CDialogEx,public CFontSet,public CRectManage
{
	DECLARE_DYNAMIC(CMySegEditDlg)

public:
	CMySegEditDlg(CMySegEditInfo segEditInfo,CWnd* pParent = NULL);   // standard constructor
	~CMySegEditDlg();

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

	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			BOOL SetRoundRect(int x = 5,int y = 5);

public:
	CMyButton m_btSave;
	CMyButton m_btCancel;

	CMyNewEdit m_edSegName;
	CMyNewEdit m_edCycleNum;
	
	CMySegEditInfo m_segEditInfo;

private:
	int m_iRA;	//圆角数值
	int m_iFrameW;	//边框宽度

	int m_iWndW;	//窗口宽度
	int m_iWndH;	//窗口高度
	int m_iCapH;	//标题栏高度
};

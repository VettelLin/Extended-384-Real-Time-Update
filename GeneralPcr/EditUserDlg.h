
#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "MyCheckButton.h"
#include "ConfigureDBInfo.h"
#include "RectManage.h"
#include "Resource.h"

class CEditUserDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CEditUserDlg)

public:
	CEditUserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditUserDlg();

	enum { IDD = IDD_POPUPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
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

			int InitOneEdit(CMyNewEdit* pEdit);

			BOOL IsUserInfoChanged();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCkbtGeneralUser();
	afx_msg void OnBnClickedCkbtAdminUser();

public:
	CMyButton m_btSave;
	CMyButton m_btCancel;

	CMyNewEdit m_edUserName;	//用户名
	CMyNewEdit m_edMnemonic;	//助记符
	CMyNewEdit m_edCode;	//密码

	CMyCheckButton m_ckbtGeneralUser;	//普通用户
	CMyCheckButton m_ckbtAdminUser;	//管理员

	CDBUserInfo m_userInfo;
	CDBUserInfo m_newUserInfo;

private:
	int m_iWndW;
	int m_iWndH;
	int m_iCapH;	//标题高度

	int m_iRoundAngle;	//圆角数值
	int m_iFrameW;	//边框宽度

	CRectManage m_rcManage;
	CMyCheckButton* m_pCkbtCurUserType;
};

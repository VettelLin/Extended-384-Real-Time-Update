
#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyLstCtrl.h"
#include "RectManage.h"

class CUserManageSetDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CUserManageSetDlg)

public:
	CUserManageSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUserManageSetDlg();

	enum { IDD = IDD_MIDDLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType,int cx,int cy);

	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void CreateEd();
			void CreateLst();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void InitEd();
			void InitLst();
			int InitUserInfo();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			void AddUser();
			void EditUser();
			void DeleteUser();

			void ResetLstColumnText();

			int RefreshLanguageDisplay();

public:
	CMyLstCtrl m_lstUserInfo;

private:
	BOOL m_bCtrlCreated;

	CRectManage m_rcManage;
};

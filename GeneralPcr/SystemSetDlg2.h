
#pragma once

#include "MyButton.h"
#include "MyLstCtrl.h"
#include "RectManage.h"

class CSystemSetDlg : public CDialogEx,public CFontSet,public CRectManage
{
	DECLARE_DYNAMIC(CSystemSetDlg)

public:
	CSystemSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSystemSetDlg();

	enum { IDD = IDD_POPUPDLG };

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
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			void ResetLstColumnText();

			int RefreshLanguageDisplay();

public:
	CMyLstCtrl m_lstUserInfo;

private:
	BOOL m_bCtrlCreated;
};


#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "LisSetDlg.h"
#include "UserManageSetDlg.h"
#include "Ps96SetDlg.h"
#include "CYNormSetDlg.h"
#include "SystemSetDlg.h"
#include "RectManage.h"

typedef enum _SETPAGETYPE
{
	LISSETDLG = 1,USERMANAGESETDLG,PS96SETDLG,SYSTEMSETDLG
}SETPAGETYPE;

class CSetDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CSetDlg)

public:
	CSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetDlg();

	enum { IDD = IDD_MIDDLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
			void CreateBt();
			void InitDlg();

			void IntFunBtn();
			void InitBt();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

			void SetPageSelected(SETPAGETYPE eSetPageType);
			void UpdateFuncBtMode();

			afx_msg void OnBnClickedSwtLisSet();
			afx_msg void OnBnClickedSwtUserManageSet();
			afx_msg void OnBnClickedSwtPs96Set();
			afx_msg void OnBnClickedSwtSystemSet();

			afx_msg void OnBnClickedAdd();
			afx_msg void OnBnClickedEdit();	
			afx_msg void OnBnClickedDelete();

			afx_msg void OnSize(UINT nType, int cx, int cy);

					int RefreshLanguageDisplay();

public:
	int	m_iFunBtnHeight;
	CMyButton m_btnAdd;	//ÐÂÔö
	CMyButton m_btnEdit;	//±à¼­
	CMyButton m_btnDelete;
	

	CMyButton* m_pCurSelSwt;
	CDialogEx* m_pCurSelDlg;

	CMyButton m_swtLisSet;
	CMyButton m_swtUserManageSet;
	CMyButton m_swtInstru1Set;	
	CMyButton m_swtSysSet;

	CLisSetDlg m_lisSetDlg;
	CUserManageSetDlg m_userManageSetDlg;

#ifdef CAN_YOUNG
	CCYNormSetDlg m_ps96SetDlg;
#else
	CPs96SetDlg m_ps96SetDlg;
#endif

private:
	BOOL m_bCtrlCreated;
};




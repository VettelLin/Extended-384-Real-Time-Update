#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "MyCheckButton.h"
#include "MyStatic.h"
#include "MyNewCb.h"
#include "ConfigureDB.h"
#include "RectManage.h"

// CUserLoginDlg dialog

class CUserLoginDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CUserLoginDlg)

public:
	CUserLoginDlg(CWnd* pParent = NULL,CUserInfoList* pListUser = NULL);   // standard constructor
	virtual ~CUserLoginDlg();

// Dialog Data
	enum {IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();

			void InitDlg();

			void CreateCtrl();
			void InitCtrl();
			void SetCtrlPos();
			void ShowCtrl();

			void CreateBt();
			void CreateSt();
			void CreateCb();
			void CreateEd();
			void CreateCkbt();

			void InitBt();
			void InitSt();
			void InitCb();
			void InitEd();
			void InitCkbt();

			void SetCtrlText();

	afx_msg void OnBnClickedOk();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedCancel();

			void InitFont1(int lfHeight);
			void SetMyFont1(CString fontName,int nHeight,int nWeight = FW_NORMAL,FONT_UNIT_TYPE fontUnitType = TYPE_POUND); //nHeightµ¥Î»Îª °õ£¬¼´1/72Ó¢´ç;

			int CheckUserInfo(CString strName,CString strCode);

public:
	CMyButton m_btOk;
	CMyButton m_btCancel;

	CMyNewCb m_cbUserName;
	CMyNewEdit m_edCode;

	CDBUserInfo m_userInfo;

protected:
	CUserInfoList* m_pListUser;

private:
	Image m_imgBk;

	CFont m_fontSet1; 
	LOGFONT m_logFont1;

	CRectManage m_rcManage;
};
 



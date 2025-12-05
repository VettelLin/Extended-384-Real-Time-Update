
#pragma once

#include "MyButton.h"
#include "MyNewEdit.h"
#include "ReportTemplateData.h"
#include "afxwin.h"

class CPs96AddTemplateDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CPs96AddTemplateDlg)

public:
	CPs96AddTemplateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPs96AddTemplateDlg();

	enum { IDD = IDD_PS96_ADD_TEMPLATE_DLG};

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
			void CreateEd();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitEd();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();

	afx_msg void OnBnClickedCkbtExportRstVert();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();

			BOOL SetRoundRect(int x = 5,int y = 5);

			void InitEditCtrlParam(CMyNewEdit* pEdit);

private:

public:
	CMyButton m_btSave;
	CMyButton m_btCancel;

	CMyNewEdit m_edTemplateName;
	CMyNewEdit m_edLabName;
	//CMyNewEdit m_edLabAddress;
	CEdit	m_edLabAddress;
	CMyNewEdit m_edContactUrl;
	CMyNewEdit m_edContactEmail;
	CMyNewEdit m_edContactPhone;
	CMyNewEdit m_edTestName;
	CEdit	m_edDisclaimer;
	//CEdit	m_editDisclaimer;

	CString		m_strName;
	CString		m_strLabName;
	CString		m_strLabAdress;
	CString		m_strContactUrl;
	CString		m_strContactEmail;
	CString		m_strContactPhone;
	CString		m_strTestName;
	CString		m_strDisclaimer;

	BOOL m_bAddTemplage;
	BOOL m_bChanged;

	CReportTemplateData* m_pReportData;

private:
	int m_iAngleRadius;	//圆角半径
	int m_iFrameWidth;	//边框宽度

	int m_iWndWidth;	//窗口宽度
	int m_iWndHeight;	//窗口高度
	int m_iCapHeight;	//标题栏高度

public:
};





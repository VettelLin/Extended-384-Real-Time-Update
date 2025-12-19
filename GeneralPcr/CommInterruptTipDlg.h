
#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "RectManage.h"
#include "resource.h"
#include "InstancePCR.h"
#include "ExperimentData.h"



class CCommInterruptTipDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CCommInterruptTipDlg)

public:
	CCommInterruptTipDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCommInterruptTipDlg();

	enum {IDD = IDD_POPUPDLG};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateSt();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitSt();
			void SetCtrlPos();
			void ShowCtrl();

			void SetCtrlText();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

			void SetNoteInfo(CString strNoteInfo);
			CString GetNoteInfo();

			void SetNoteInfoStyle(DWORD style);
			DWORD GetNoteInfoStyle();

			BOOL SetRoundRect(int x = 5,int y = 5);

public:
	CMyButton m_btStop;
	CMyStatic m_stIcon;
	CMyStatic m_stNoteInfo;
	CMyStatic m_stTimeInfo;

	int m_retCode;	//弹窗返回值，0:重新连接PCR正常 1:连接到时 2:中止实验
	CInstancePCR*	m_pInstancePCR; // PCR实例
	CExperimentData* m_pExpData; // 实验数据
	int m_iRemainSec;	//实验剩余时间

private:
	int m_roundValue;	//圆角数值
	int m_frameWidth;	//边框宽度

	CString m_strNoteInfo;	//提示信息
	CString m_strReconnect;

	int   m_noteFontSize;//单位为磅
	DWORD m_dwNoteStyle;
	int m_width;
	int m_height;
	int m_iCapHeight;
	CRectManage m_rcManage;

	UINT	m_nRunTimer1;
	UINT	m_nRunTimer2;
	int		m_iTimeCount;
	
	BOOL PrepareRunning();
	BOOL RunLidHeader();
};

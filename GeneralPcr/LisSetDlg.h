
#pragma once

#include "MyButton.h"
#include "MyStatic.h"
#include "MyCheckButton.h"
#include "MyNewEdit.h"
#include "MyNewCb.h"

class CLisSetDlg : public CDialogEx,public CFontSet
{
	DECLARE_DYNAMIC(CLisSetDlg)

public:
	CLisSetDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLisSetDlg();

	enum { IDD = IDD_MIDDLEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
			void CreateCtrl();
			void CreateBt();
			void CreateCkbt();
			void CreateSt();
			void CreateEd();
			void CreateCb();
			void InitDlg();
			void InitCtrl();
			void InitBt();
			void InitCkbt();
			void InitSt();
			void InitEd();
			void InitCb();
			void SetCtrlPos();
			void ShowCtrl();

	afx_msg void OnBnClickedLisConnect();
	afx_msg void OnBnClickedComMode();
	afx_msg void OnBnClickedTcpIpMode();
	
			void SaveLisSet();
			void EnableCtrl(BOOL bEnable);

			void SetComCtrlEnableState(BOOL bEnabled);
			void SetTcpCtrlEnableState(BOOL bEnabled);

public:
	CMyCheckButton m_ckbtLisConnect;	//LISÁ¬½Ó
	CMyCheckButton m_ckbtAutoUploadRst;
	CMyCheckButton m_ckbtRecvSamData;

	CMyCheckButton m_ckbtCom;
	CMyCheckButton m_ckbtTcpIp;

	CMyStatic m_stRecvWaitSec;
	CMyStatic m_stRecvWaitSecUnit;
	CMyStatic m_stMaxTransTimes;

	CMyStatic m_stComId;
	CMyStatic m_stDataByteLen;
	CMyStatic m_stStopByteLen;
	CMyStatic m_stPaud;
	CMyStatic m_stCheckWay;

	CMyStatic m_stServerIp;
	CMyStatic m_stServerPort;

	CMyNewEdit m_edRecvWaitSec;
	CMyNewEdit m_edMaxSendTimes;
	CMyNewEdit m_edServerIp;
	CMyNewEdit m_edServerPort;

	CMyNewCb m_cbComId;
	CMyNewCb m_cbDataByteLen;
	CMyNewCb m_cbStopByteLen;
	CMyNewCb m_cbPaud;
	CMyNewCb m_cbCheckWay;
};

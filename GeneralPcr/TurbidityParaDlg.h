#pragma once

#include "Resource.h"
#include "CalibrateParaTurbidity.h"

// CTurbidityParaDlg dialog

class CTurbidityParaDlg : public CDialog
{
	DECLARE_DYNAMIC(CTurbidityParaDlg)

public:
	CTurbidityParaDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTurbidityParaDlg();

// Dialog Data
	enum { IDD = IDD_TURBIDITY_PARA_DLG };

public:
	BOOL	m_bSetChanged;
	void SetTurbidityPara(CCalibrateParaTurbidity* pTuibidityPara) { m_pTuibidityPara = pTuibidityPara;}

private:
	CCalibrateParaTurbidity* m_pTuibidityPara;

	

	BOOL HasChanged();
	void EnableMOTChannlePara(int nChannel);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	BOOL m_bSaveDefault;

	CString m_strDye1;
	CString m_strChannel1;
	BOOL m_bUseMOT1;
	int m_nMotAhead1;
	float m_fMotRatio1;

	CString m_strDye2;
	CString m_strChannel2;
	BOOL m_bUseMOT2;
	int m_nMotAhead2;
	float m_fMotRatio2;

	CString m_strDye3;
	CString m_strChannel3;
	BOOL m_bUseMOT3;
	int m_nMotAhead3;
	float m_fMotRatio3;

	CString m_strDye4;
	CString m_strChannel4;
	BOOL m_bUseMOT4;
	int m_nMotAhead4;
	float m_fMotRatio4;

	CString m_strDye5;
	CString m_strChannel5;
	BOOL m_bUseMOT5;
	int m_nMotAhead5;
	float m_fMotRatio5;

	float m_fMOTMinRatio;

	afx_msg void OnBnClickedCheckUseMot1();
	afx_msg void OnBnClickedCheckUseMot2();
	afx_msg void OnBnClickedCheckUseMot3();
	afx_msg void OnBnClickedCheckUseMot4();
	afx_msg void OnBnClickedCheckUseMot5();
	afx_msg void OnBnClickedCheckCurveAdjust1();
	afx_msg void OnBnClickedCheckCurveAdjust2();
	afx_msg void OnBnClickedCheckCurveAdjust3();
	afx_msg void OnBnClickedCheckCurveAdjust4();
	afx_msg void OnBnClickedCheckCurveAdjust5();
};

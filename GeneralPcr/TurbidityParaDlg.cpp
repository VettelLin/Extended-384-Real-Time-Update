// MOTChannelParaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TurbidityParaDlg.h"
#include "afxdialogex.h"
#include "math.h"

// CTurbidityParaDlg dialog

IMPLEMENT_DYNAMIC(CTurbidityParaDlg, CDialog)

CTurbidityParaDlg::CTurbidityParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTurbidityParaDlg::IDD, pParent)
	, m_bSaveDefault(FALSE)
	, m_strChannel1(_T(""))
	, m_bUseMOT1(FALSE)
	, m_nMotAhead1(0)
	, m_fMotRatio1(0)
	, m_strChannel2(_T(""))
	, m_bUseMOT2(FALSE)
	, m_nMotAhead2(0)
	, m_fMotRatio2(0)
	, m_strChannel3(_T(""))
	, m_bUseMOT3(FALSE)
	, m_nMotAhead3(0)
	, m_fMotRatio3(0)
	, m_strChannel4(_T(""))
	, m_bUseMOT4(FALSE)
	, m_nMotAhead4(0)
	, m_fMotRatio4(0)
	, m_strChannel5(_T(""))
	, m_bUseMOT5(FALSE)
	, m_nMotAhead5(0)
	, m_fMotRatio5(0)
	, m_fMOTMinRatio(0)
{
	m_pTuibidityPara = NULL;
	m_bSetChanged = FALSE;

	m_strDye1.Empty();
	m_strDye2.Empty();
	m_strDye3.Empty();
	m_strDye4.Empty();
	m_strDye5.Empty();
}

CTurbidityParaDlg::~CTurbidityParaDlg()
{
}

void CTurbidityParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_SAVE_TO_FILE, m_bSaveDefault);
	DDX_Text(pDX, IDC_STATIC_DYE1, m_strChannel1);
	DDX_Check(pDX, IDC_CHECK_USE_MOT1, m_bUseMOT1);
	DDX_Text(pDX, IDC_EDIT_MOT_AHEAD1, m_nMotAhead1);
	DDX_Text(pDX, IDC_EDIT_MOT_RATIO1, m_fMotRatio1);
	DDX_Text(pDX, IDC_STATIC_DYE2, m_strChannel2);
	DDX_Check(pDX, IDC_CHECK_USE_MOT2, m_bUseMOT2);
	DDX_Text(pDX, IDC_EDIT_MOT_AHEAD2, m_nMotAhead2);
	DDX_Text(pDX, IDC_EDIT_MOT_RATIO2, m_fMotRatio2);
	DDX_Text(pDX, IDC_STATIC_DYE3, m_strChannel3);
	DDX_Check(pDX, IDC_CHECK_USE_MOT3, m_bUseMOT3);
	DDX_Text(pDX, IDC_EDIT_MOT_AHEAD3, m_nMotAhead3);
	DDX_Text(pDX, IDC_EDIT_MOT_RATIO3, m_fMotRatio3);
	DDX_Text(pDX, IDC_STATIC_DYE4, m_strChannel4);
	DDX_Check(pDX, IDC_CHECK_USE_MOT4, m_bUseMOT4);
	DDX_Text(pDX, IDC_EDIT_MOT_AHEAD4, m_nMotAhead4);
	DDX_Text(pDX, IDC_EDIT_MOT_RATIO4, m_fMotRatio4);
	DDX_Text(pDX, IDC_STATIC_DYE5, m_strChannel5);
	DDX_Check(pDX, IDC_CHECK_USE_MOT5, m_bUseMOT5);
	DDX_Text(pDX, IDC_EDIT_MOT_AHEAD5, m_nMotAhead5);
	DDX_Text(pDX, IDC_EDIT_MOT_RATIO5, m_fMotRatio5);
	DDX_Text(pDX, IDC_EDIT_MOT_MIN_RATIO, m_fMOTMinRatio);
}


BEGIN_MESSAGE_MAP(CTurbidityParaDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK_USE_MOT1, &CTurbidityParaDlg::OnBnClickedCheckUseMot1)
	ON_BN_CLICKED(IDC_CHECK_USE_MOT2, &CTurbidityParaDlg::OnBnClickedCheckUseMot2)
	ON_BN_CLICKED(IDC_CHECK_USE_MOT3, &CTurbidityParaDlg::OnBnClickedCheckUseMot3)
	ON_BN_CLICKED(IDC_CHECK_USE_MOT4, &CTurbidityParaDlg::OnBnClickedCheckUseMot4)
	ON_BN_CLICKED(IDC_CHECK_USE_MOT5, &CTurbidityParaDlg::OnBnClickedCheckUseMot5)
END_MESSAGE_MAP()


// CTurbidityParaDlg message handlers


BOOL CTurbidityParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	if (m_pTuibidityPara != NULL)
	{
		m_fMOTMinRatio = m_pTuibidityPara->GetMinRatio();

		int iParaCount = m_pTuibidityPara->GetParaCount();
		if (iParaCount > 0)
		{
			m_strDye1= m_pTuibidityPara->GetDye(0);
			m_strChannel1.Format(_T("%d") , m_pTuibidityPara->GetChannel(m_strDye1));
			m_bUseMOT1 = m_pTuibidityPara->UseCalibrate(m_strDye1);
			m_nMotAhead1 = m_pTuibidityPara->GetAheadCycle(m_strDye1);
			m_fMotRatio1 = m_pTuibidityPara->GetAdjustRatio(m_strDye1);
		}

		if (iParaCount > 1)
		{
			m_strDye2= m_pTuibidityPara->GetDye(1);
			m_strChannel2.Format(_T("%d") , m_pTuibidityPara->GetChannel(m_strDye2));
			m_bUseMOT2 = m_pTuibidityPara->UseCalibrate(m_strDye2);
			m_nMotAhead2 = m_pTuibidityPara->GetAheadCycle(m_strDye2);
			m_fMotRatio2 = m_pTuibidityPara->GetAdjustRatio(m_strDye2);
		}

		if (iParaCount > 2)
		{
			m_strDye3= m_pTuibidityPara->GetDye(2);
			m_strChannel3.Format(_T("%d") , m_pTuibidityPara->GetChannel(m_strDye3));
			m_bUseMOT3 = m_pTuibidityPara->UseCalibrate(m_strDye3);
			m_nMotAhead3 = m_pTuibidityPara->GetAheadCycle(m_strDye3);
			m_fMotRatio3 = m_pTuibidityPara->GetAdjustRatio(m_strDye3);
		}

		if (iParaCount > 3)
		{
			m_strDye4= m_pTuibidityPara->GetDye(3);
			m_strChannel4.Format(_T("%d") , m_pTuibidityPara->GetChannel(m_strDye4));
			m_bUseMOT4 = m_pTuibidityPara->UseCalibrate(m_strDye4);
			m_nMotAhead4 = m_pTuibidityPara->GetAheadCycle(m_strDye4);
			m_fMotRatio4 = m_pTuibidityPara->GetAdjustRatio(m_strDye4);
		}

		if (iParaCount > 4)
		{
			m_strDye5= m_pTuibidityPara->GetDye(4);
			m_strChannel5.Format(_T("%d") , m_pTuibidityPara->GetChannel(m_strDye5));
			m_bUseMOT5 = m_pTuibidityPara->UseCalibrate(m_strDye5);
			m_nMotAhead5 = m_pTuibidityPara->GetAheadCycle(m_strDye5);
			m_fMotRatio5 = m_pTuibidityPara->GetAdjustRatio(m_strDye5);
		}
	}

	for (int i=1; i<=5; i++)
	{
		EnableMOTChannlePara(i);
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTurbidityParaDlg::OnOK()
{
	UpdateData(TRUE);

	m_bSetChanged = HasChanged();
	if (m_bSetChanged)
	{
		int iParaCount = m_pTuibidityPara->GetParaCount();
		if (iParaCount > 0)
		{
			m_pTuibidityPara->EnableUseCalibrate(m_strDye1, m_bUseMOT1);
			m_pTuibidityPara->SetAheadCycle(m_strDye1, m_nMotAhead1);
			m_pTuibidityPara->SetAdjustRatio(m_strDye1, m_fMotRatio1);
		}

		if (iParaCount > 1)
		{
			m_pTuibidityPara->EnableUseCalibrate(m_strDye2, m_bUseMOT2);
			m_pTuibidityPara->SetAheadCycle(m_strDye2, m_nMotAhead2);
			m_pTuibidityPara->SetAdjustRatio(m_strDye2, m_fMotRatio2);
		}

		if (iParaCount > 2)
		{
			m_pTuibidityPara->EnableUseCalibrate(m_strDye3, m_bUseMOT3);
			m_pTuibidityPara->SetAheadCycle(m_strDye3, m_nMotAhead3);
			m_pTuibidityPara->SetAdjustRatio(m_strDye3, m_fMotRatio3);
		}

		if (iParaCount > 3)
		{
			m_pTuibidityPara->EnableUseCalibrate(m_strDye4, m_bUseMOT4);
			m_pTuibidityPara->SetAheadCycle(m_strDye4, m_nMotAhead4);
			m_pTuibidityPara->SetAdjustRatio(m_strDye4, m_fMotRatio4);
		}

		if (iParaCount > 4)
		{
			m_pTuibidityPara->EnableUseCalibrate(m_strDye5, m_bUseMOT5);
			m_pTuibidityPara->SetAheadCycle(m_strDye5, m_nMotAhead5);
			m_pTuibidityPara->SetAdjustRatio(m_strDye5, m_fMotRatio5);
		}

		m_pTuibidityPara->SetMinRatio(m_fMOTMinRatio);
	}

	CDialog::OnOK();
}


BOOL CTurbidityParaDlg::HasChanged()
{
	int iParaCount = m_pTuibidityPara->GetParaCount();
	if (iParaCount > 0)
	{
		if (m_pTuibidityPara->UseCalibrate(m_strDye1) != m_bUseMOT1)
			return TRUE;
		if (m_pTuibidityPara->GetAheadCycle(m_strDye1) != m_nMotAhead1)
			return TRUE;
		if (::fabs(m_pTuibidityPara->GetAdjustRatio(m_strDye1)- m_fMotRatio1) > 0.0001)
			return TRUE;
	}

	if (iParaCount > 1)
	{
		if (m_pTuibidityPara->UseCalibrate(m_strDye2) != m_bUseMOT2)
			return TRUE;
		if (m_pTuibidityPara->GetAheadCycle(m_strDye2) != m_nMotAhead2)
			return TRUE;
		if (::fabs(m_pTuibidityPara->GetAdjustRatio(m_strDye2)- m_fMotRatio2) > 0.0001)
			return TRUE;
	}

	if (iParaCount > 2)
	{
		if (m_pTuibidityPara->UseCalibrate(m_strDye3) != m_bUseMOT3)
			return TRUE;
		if (m_pTuibidityPara->GetAheadCycle(m_strDye3) != m_nMotAhead3)
			return TRUE;
		if (::fabs(m_pTuibidityPara->GetAdjustRatio(m_strDye3)- m_fMotRatio3) > 0.0001)
			return TRUE;
	}

	if (iParaCount > 3)
	{
		if (m_pTuibidityPara->UseCalibrate(m_strDye4) != m_bUseMOT4)
			return TRUE;
		if (m_pTuibidityPara->GetAheadCycle(m_strDye4) != m_nMotAhead4)
			return TRUE;
		if (::fabs(m_pTuibidityPara->GetAdjustRatio(m_strDye4)- m_fMotRatio4) > 0.0001)
			return TRUE;
	}

	if (iParaCount > 4)
	{
		if (m_pTuibidityPara->UseCalibrate(m_strDye5) != m_bUseMOT5)
			return TRUE;
		if (m_pTuibidityPara->GetAheadCycle(m_strDye5) != m_nMotAhead5)
			return TRUE;
		if (::fabs(m_pTuibidityPara->GetAdjustRatio(m_strDye5)- m_fMotRatio5) > 0.0001)
			return TRUE;
	}

	if(m_pTuibidityPara->GetMinRatio() != m_fMOTMinRatio)
	{
		return TRUE;
	}

	return FALSE;
}



void CTurbidityParaDlg::OnBnClickedCheckUseMot1()
{
	UpdateData(TRUE);
	EnableMOTChannlePara(1);
}


void CTurbidityParaDlg::OnBnClickedCheckUseMot2()
{
	UpdateData(TRUE);
	EnableMOTChannlePara(2);
}


void CTurbidityParaDlg::OnBnClickedCheckUseMot3()
{
	UpdateData(TRUE);
	EnableMOTChannlePara(3);
}


void CTurbidityParaDlg::OnBnClickedCheckUseMot4()
{
	UpdateData(TRUE);
	EnableMOTChannlePara(4);
}


void CTurbidityParaDlg::OnBnClickedCheckUseMot5()
{
	UpdateData(TRUE);
	EnableMOTChannlePara(5);
}


void CTurbidityParaDlg::EnableMOTChannlePara(int nChannel)
{
	CEdit* pEdit = NULL;
	if(nChannel == 1)
	{
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_AHEAD1);
		pEdit->EnableWindow(m_bUseMOT1);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_RATIO1);
		pEdit->EnableWindow(m_bUseMOT1);
	}

	if(nChannel == 2)
	{
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_AHEAD2);
		pEdit->EnableWindow(m_bUseMOT2);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_RATIO2);
		pEdit->EnableWindow(m_bUseMOT2);
	}

	if(nChannel == 3)
	{
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_AHEAD3);
		pEdit->EnableWindow(m_bUseMOT3);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_RATIO3);
		pEdit->EnableWindow(m_bUseMOT3);
	}

	if(nChannel == 4)
	{
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_AHEAD4);
		pEdit->EnableWindow(m_bUseMOT4);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_RATIO4);
		pEdit->EnableWindow(m_bUseMOT4);
	}

	if(nChannel == 5)
	{
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_AHEAD5);
		pEdit->EnableWindow(m_bUseMOT5);
		pEdit = (CEdit*)GetDlgItem(IDC_EDIT_MOT_RATIO5);
		pEdit->EnableWindow(m_bUseMOT5);
	}
}

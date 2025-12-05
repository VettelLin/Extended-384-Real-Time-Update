#pragma once
class AFX_EXT_CLASS CSensorMCheckParam
{
public:
	CSensorMCheckParam(void);
	~CSensorMCheckParam(void);

	CSensorMCheckParam(const CSensorMCheckParam& info);
	void operator=(const CSensorMCheckParam& info);

	BOOL IsCheckEnvirTsm();
	BOOL IsCheckHeatSinkTsm();
	BOOL IsCheckLidTsm();
	BOOL IsCheckPelt1Tsm();
	BOOL IsCheckPelt2Tsm();
	BOOL IsCheckPelt3Tsm();
	BOOL IsCheckPowerTsm();
	BOOL IsCheckScanTsm();

	double GetEnvirTsmRefH();
	double GetEnvirTsmRefL();

	double GetHeatSinkTsmRefH();
	double GetHeatSinkTsmRefL();

	double GetLidTsmRefH();
	double GetLidTsmRefL();

	double GetPelt1TsmRefH();
	double GetPelt1TsmRefL();

	double GetPelt2TsmRefH();
	double GetPelt2TsmRefL();

	double GetPelt3TsmRefH();
	double GetPelt3TsmRefL();

	double GetPowerTsmRefH();
	double GetPowerTsmRefL();

	double GetScanTsmRefH();
	double GetScanTsmRefL();

	void SetEnvirTsmRefH(double dData);
	void SetEnvirTsmRefL(double dData);

	void SetHeatSinkTsmRefH(double dData);
	void SetHeatSinkTsmRefL(double dData);

	void SetLidTsmRefH(double dData);
	void SetLidTsmRefL(double dData);

	void SetPelt1TsmRefH(double dData);
	void SetPelt1TsmRefL(double dData);

	void SetPelt2TsmRefH(double dData);
	void SetPelt2TsmRefL(double dData);

	void SetPelt3TsmRefH(double dData);
	void SetPelt3TsmRefL(double dData);

	void SetPowerTsmRefH(double dData);
	void SetPowerTsmRefL(double dData);

	void SetScanTsmRefH(double dData);
	void SetScanTsmRefL(double dData);

	void ResetCheckResult();
	void AddCheckResult(CString strData);
	CString GetCheckResult();
	BOOL IsSuccessful();

protected:
	BOOL m_bCheckEnvirTsm;
	BOOL m_bCheckHeatSinkTsm;
	BOOL m_bCheckLidTsm;
	BOOL m_bCheckPelt1Tsm;
	BOOL m_bCheckPelt2Tsm;
	BOOL m_bCheckPelt3Tsm;
	BOOL m_bCheckPowerTsm;
	BOOL m_bCheckScanTsm;

	double m_dEnvirTsmRefH;
	double m_dEnvirTsmRefL;

	double m_dHeatSinkTsmRefH;
	double m_dHeatSinkTsmRefL;

	double m_dLidTsmRefH;
	double m_dLidTsmRefL;

	double m_dPelt1TsmRefH;
	double m_dPelt1TsmRefL;

	double m_dPelt2TsmRefH;
	double m_dPelt2TsmRefL;

	double m_dPelt3TsmRefH;
	double m_dPelt3TsmRefL;

	double m_dPowerTsmRefH;
	double m_dPowerTsmRefL;

	double m_dScanTsmRefH;
	double m_dScanTsmRefL;

	CString m_strChkRst;
};


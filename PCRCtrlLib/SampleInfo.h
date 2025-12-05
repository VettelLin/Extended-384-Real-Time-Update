#pragma once

#include "DeclaresPCR.h"

// 样本信息，保存样本来源相关信息和检测信息
class AFX_EXT_CLASS CSampleInfo
{
public:
	CSampleInfo(void);
	~CSampleInfo(void);

	CSampleInfo(const CSampleInfo& samInfo);
	void operator=(const CSampleInfo& samInfo);
	BOOL operator==(const CSampleInfo& samInfo);

	void SetPaitentID(CString strID) { m_strPatientID = strID; }
	CString GetPatientID() { return m_strPatientID; }
	void SetClinicID(CString strID) { m_strClinicID = strID; }
	CString GetClinicID() { return m_strClinicID; }
	void SetCaseID(CString strID) { m_strCaseID = strID; }
	CString GetCaseID() { return  m_strCaseID; }

	CString GetPatientName() { return m_strPatientName; }
	void SetPatientName(CString strName) { m_strPatientName = strName; }
	ePatientSexuality GetSexuality() { return m_nPatientSex; }
	void SetSexuality(ePatientSexuality nSex) { m_nPatientSex = nSex; }

	void SetSexuality(CString strData);
	CString GetSexualityString();

	CString GetAgeString();
	int GetPaitentAge() { return m_nPatientAge; }
	void SetPaitentAge(int iAge) { m_nPatientAge = iAge; }

	ePatientAgeUnit GetAgeUnit() { return m_nAgeUnit; }
	void SetAgeUnit(ePatientAgeUnit nUnit) { m_nAgeUnit = nUnit; }
	BOOL GetBeAbnormalAge(){return m_nPatientAge <= 0;}

	CString GetAreaNo()  { return m_strAreaNo; }
	void SetAreaNo(CString strArea) { m_strAreaNo = strArea; }
	CString GetRoomNo()  { return m_strRoomNo; }
	void SetRoomNo(CString strRoom) { m_strRoomNo = strRoom; }
	CString GetBedNo()  { return m_strBedNo; }
	void SetBedNo(CString strBed) { m_strBedNo = strBed; }
	CString GetProposer()  { return m_strProposer; }
	void SetProposer(CString strName) { m_strProposer = strName; }
	CString GetDepartment()  { return m_strDepartment; }
	void SetDepartment(CString strDept) { m_strDepartment = strDept; }
	CString GetHospital()  { return m_strHospital; }
	void SetHospital(CString strHospital) { m_strHospital = strHospital; }

	CString GetCollectDate()  { return m_strCollectDate; }
	CString GetCollectYMD();
	void SetCollectDate(CString strDate) { m_strCollectDate = strDate; }

	void SetCollectDateYMD(CString strDate);
	void SetSQMCollectDateYMD(CString strDate);

	CString GetApplyDate()  { return m_strApplyDate; }
	void SetApplyDate(CString strDate) { m_strApplyDate = strDate; }

	virtual void Reset();

public:
	CString				m_strPatientID; // 唯一标识，可以使ASSIC码
	CString				m_strClinicID; // 门诊号
	CString				m_strCaseID; // 病案号 病历号
	CString				m_strPatientName; // 姓名
	ePatientSexuality	m_nPatientSex; // 性别
	int					m_nPatientAge; // 年龄
	ePatientAgeUnit		m_nAgeUnit; // 年龄单位
	CString				m_strAreaNo; // 病区
	CString				m_strRoomNo; // 病房号
	CString				m_strBedNo; // 床号
	CString				m_strProposer; // 申请人，送检医生 20250307 根据用户要求需要替换技师
	CString				m_strDepartment; // 科室
	CString				m_strHospital; // 医院
	CString				m_strCollectDate; // 采集时间
	CString				m_strApplyDate; // 申请日期 20250307 根据用户要求需要替换为复检人姓名
};


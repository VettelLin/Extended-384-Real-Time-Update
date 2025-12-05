#pragma once

typedef enum HoleDefaultDisplay{
	EHDD_PATIENTNAME,EHDD_TUBENAME,EHDD_SAMID,EHDD_SAMNAME
}EHOLEDEFAULTDISPLAY;

class CHoleSelectSet
{
public:
	CHoleSelectSet(void);
	~CHoleSelectSet(void);

	CHoleSelectSet(const CHoleSelectSet& holeSelSet);
	void operator=(const CHoleSelectSet& holeSelSet);

public:
	EHOLEDEFAULTDISPLAY m_eHoleDefDisplay;
};


#pragma once
class CSegStepRange
{
public:
	CSegStepRange(void);
	~CSegStepRange(void);

	CSegStepRange(const CSegStepRange& segStep);
	void operator=(const CSegStepRange& segStep);

	BOOL BeValidRange();
	void ResetSegStepRange();

	void SetSegmentStart(int segStart);
	int GetSegmentStart();
	void SetSegmentEnd(int segEnd);
	int GetSegmentEnd();

	void SetStepStart(int stepStart);
	int GetStepStart();
	void SetStepEnd(int stepEnd);
	int GetStepEnd();

private:
	int m_segStart;
	int m_segEnd;
	int m_stepStart;
	int m_stepEnd;
};


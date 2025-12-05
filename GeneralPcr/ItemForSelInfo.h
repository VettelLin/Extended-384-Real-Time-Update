#pragma once

typedef enum SelectState{
	ESS_NORMAL,ESS_HOVER,ESS_SELECTED,ESS_DISABLE
}ESELECTSTATE;

class CItemForSelInfo
{
public:
	CItemForSelInfo(void);
	~CItemForSelInfo(void);

	CItemForSelInfo(const CItemForSelInfo& info);
	void operator=(const CItemForSelInfo& info);

	BOOL GetBeDisabled();
	BOOL GetBeSelected();

	int SetSelectState(ESELECTSTATE eSelState);
	ESELECTSTATE GetSelectState();

	int MouseOut();
	int MouseIn();

public:
	CString m_strText;
	
	COLORREF m_clrNormal;
	COLORREF m_clrHover;
	COLORREF m_clrSelected;
	COLORREF m_clrDisabled;

	int m_iItemH;

	ESELECTSTATE m_eSelState;
};


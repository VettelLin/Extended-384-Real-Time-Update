#pragma once

#include "Fitting.h"

class AFX_EXT_CLASS CExpFitting :
    public CFitting
{
    double *m_pdXList, *m_pdYList, *m_pdCoeffExpon;
    int m_iListLength;
    BOOL LnY();//对Y列取对数
public:
    CExpFitting(void);
    ~CExpFitting(void);
    double ExpSolution(double *x, double *y, double *Result, int iListLen);//计算结果
};


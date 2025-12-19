#pragma once

#include "Fitting.h"


class AFX_EXT_CLASS CLnFitting :
    public CFitting
{
    double *m_pdXList, *m_pdYList, *m_pdCoeffExpon;
    int m_iListLength;
    BOOL LnX();// 对x列取对数
public:
    CLnFitting(void);
    ~CLnFitting(void);
    double LnSolution(double *x, double *y, double *Result, int iListLen); //计算结果
};


#pragma once

#include "Fitting.h"


class AFX_EXT_CLASS CPowerFitting :
    public CFitting
{
public:
    CPowerFitting(void);
    ~CPowerFitting(void);
private:
    double *m_pdXList;
    double *m_pdYList;
    double *m_pdCoeffExpon;
    int m_iListLength;
    BOOL Ln(double *pdData, int iDataLen);
public:
    double PowSolution(double *x, double *y, double *Result, int iListLen);
};


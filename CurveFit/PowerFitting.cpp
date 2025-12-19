#include "StdAfx.h"
#include <math.h>

#include "PowerFitting.h"


CPowerFitting::CPowerFitting(void)
    : m_pdXList(NULL)
    , m_pdYList(NULL)
    , m_pdCoeffExpon(NULL)
    , m_iListLength(0)
{
}

CPowerFitting::~CPowerFitting(void)
{
}

BOOL CPowerFitting::Ln(double *pdData, int iDataLen)
{
    for (int i = 0 ; i < iDataLen ; i++)
    {
        pdData[i] = log(pdData[i]);
    }

    return TRUE;
}

double CPowerFitting::PowSolution(double *x, double *y, double *Result, int iListLen)
{
    double fSD;
    int k;
    m_iListLength = iListLen;
    m_pdXList = new double[m_iListLength];
    if (NULL == m_pdXList)
    {
        return FALSE;
    }

    m_pdYList = new double[m_iListLength];
    if (NULL == m_pdYList)
    {
        return FALSE;
    }

    m_pdCoeffExpon = new double[2];
    if (NULL == m_pdCoeffExpon)
    {
        return FALSE;
    }

    for (k = 0; k < m_iListLength; k++)
    {
        m_pdXList[k] = x[k];
        m_pdYList[k] = y[k];
    }
    Ln(m_pdXList, m_iListLength);
    Ln(m_pdYList, m_iListLength);
    fSD = CFitting::SolutionCLEG(m_pdXList, m_pdYList, m_pdCoeffExpon, m_iListLength, 2);
    m_pdCoeffExpon[0] = exp(m_pdCoeffExpon[0]);
    memcpy(Result, m_pdCoeffExpon, 16);

    delete m_pdXList;
    delete m_pdYList;
    delete m_pdCoeffExpon;

    return fSD;
}


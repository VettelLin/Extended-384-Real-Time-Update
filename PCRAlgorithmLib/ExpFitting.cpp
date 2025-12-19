#include "StdAfx.h"
#include <math.h>

#include "ExpFitting.h"



CExpFitting::CExpFitting(void):
    m_pdXList(NULL),
    m_pdYList(NULL),
    m_pdCoeffExpon(NULL)
{
}

CExpFitting::~CExpFitting(void)
{
}

/**
 * 求Y列的ln，m_dYList的数值变为其ln值。
 */
BOOL CExpFitting::LnY()
{
    int i;
    for (i = 0; i < m_iListLength; i++)
    {
        m_pdYList[i] = log(m_pdYList[i]);
    }
    return TRUE;
}

/**
 *求指数拟和的系数和指数
 * \param *x   x列数据，double型
 * \param *y   y列数据，double型
 * \param *Result   外部获得的计算结果，Result[0]为拟和得到的系数，Result[1]为指数。
 * \param iListLen  x列和y列的长度
 * \return  各点误差的平方和
 */
double CExpFitting::ExpSolution(double *x, double *y, double *Result, int iListLen)
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
    CExpFitting::LnY();
    fSD = CFitting::SolutionCLEG(m_pdXList, m_pdYList, m_pdCoeffExpon, m_iListLength, 2);
    m_pdCoeffExpon[0] = exp(m_pdCoeffExpon[0]);
    memcpy(Result, m_pdCoeffExpon, 16);

    delete m_pdXList;
    delete m_pdYList;
    delete m_pdCoeffExpon;

    return fSD;
}


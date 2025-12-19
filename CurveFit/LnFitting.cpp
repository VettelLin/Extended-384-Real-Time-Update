#include "StdAfx.h"
#include <math.h>

#include "LnFitting.h"


CLnFitting::CLnFitting(void):
    m_pdXList(NULL),
    m_pdYList(NULL),
    m_pdCoeffExpon(NULL)
{
}

CLnFitting::~CLnFitting(void)
{
}

/**
 *  对x列取ln
 */
BOOL CLnFitting::LnX()
{
    int i;
    for (i = 0; i < m_iListLength; i++)
    {
        m_pdXList[i] = log(m_pdXList[i]);
    }
    return TRUE;
}

/**
*求对数拟和：y=klnx+b 的系数k和常数项b
* \param *x   x列数据，double型
* \param *y   y列数据，double型
* \param *Result   外部获得的计算结果，Result[0]为拟和得到的常数项b，Result[1]为系数k。
* \param iListLen  x列和y列的长度
* \return  各点误差的平方和
*/
double CLnFitting::LnSolution(double *x, double *y, double *Result, int iListLen)
{
    double fSD;
    int k;
    m_iListLength = iListLen;
    m_pdXList = new double[m_iListLength];
    m_pdYList = new double[m_iListLength];
    m_pdCoeffExpon = new double[2];

    for (k = 0; k < m_iListLength; k++)
    {
        m_pdXList[k] = x[k];
        m_pdYList[k] = y[k];
    }
    CLnFitting::LnX();
    fSD = CFitting::SolutionCLEG(m_pdXList, m_pdYList, m_pdCoeffExpon, m_iListLength, 2);
    //m_pdCoeffExpon[0] = m_pdCoeffExpon[0]/10;
    //m_pdCoeffExpon[1] = m_pdCoeffExpon[1]/10;

    memcpy(Result, m_pdCoeffExpon, 16);

    delete m_pdXList;
    delete m_pdYList;
    delete m_pdCoeffExpon;

    return fSD;
}


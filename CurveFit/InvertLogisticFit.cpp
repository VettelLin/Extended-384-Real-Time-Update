#include "StdAfx.h"
#include <math.h>

#include "InvertLogisticFit.h"
#include "Fitting.h"



CInvertLogisticFit::CInvertLogisticFit(void)
{
}

CInvertLogisticFit::~CInvertLogisticFit(void)
{
}

BOOL CInvertLogisticFit::LnXLogistY(void)
{
    int i, j;
    double min, max;
    double temp;

    min = m_dYmin - 0.1;
    max = m_dYmax + 1;

    if (m_pdLogisticY != NULL)
    {
        delete m_pdLogisticY;
        m_pdLogisticY = NULL;
    }
    m_pdLogisticY = new double [m_iEffectPoint];
    if (NULL == m_pdLogisticY)
    {
        return FALSE;
    }

    if (m_pdLnX != NULL)
    {
        delete m_pdLnX;
        m_pdLnX = NULL;
    }
    m_pdLnX = new double [m_iEffectPoint];
    if (NULL == m_pdLnX)
    {
        return FALSE;
    }

    j = 0;
    for (i = 0; i < m_iEffectPoint; i++)
    {
        temp = (max - m_pdYList[j]) / (m_pdYList[j] - min);

        m_pdLogisticY[i] = log(temp);

        if (m_pdXList[j] <= 0)
            //j++;
            //m_pdLnX[i] = log(m_dXmin/2);
        {
            m_pdLnX[i] = log(0.001);
        }
        else
        {
            m_pdLnX[i] = log(m_pdXList[j]);
        }

        j++;
    }
    return TRUE;
}

double CInvertLogisticFit::FunctionValue(double x)
{
    double temp;
    if (x <= 0)
    {
        temp = m_pdMinMaxXoP[0] + (m_pdMinMaxXoP[1] - m_pdMinMaxXoP[0]) / (1 + exp(-(m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(0.000001))));
    }
    else
    {
        temp = m_pdMinMaxXoP[0] + (m_pdMinMaxXoP[1] - m_pdMinMaxXoP[0]) / (1 + exp(-(m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(x))));
    }
    return temp;
}

double CInvertLogisticFit::PartialYmin(double x)
{
    double zero, temp;
    zero = x;
    if (zero <= 0)
    {
        zero = 0.000001;
    }
    temp = 1 - 1 / (1 + exp(-(m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(zero))));
    //temp = 1/(1+exp(-(m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(x))));
    return temp;
}

double CInvertLogisticFit::PartialYmax(double x)
{
    double zero, temp;
    zero = x;
    if (zero <= 0)
    {
        zero = 0.000001;
    }
    //temp = 1 - 1/(1+exp(-(m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(x))));
    temp = 1 / (1 + exp(-(m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(zero))));
    return temp;
}

double CInvertLogisticFit::PartialXo(double x)
{
    double zero, temp;
    zero = x;
    if (zero <= 0)
    {
        zero = 0.000001;
    }

    temp = exp(-1 * (m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(zero)));
    temp = (m_pdMinMaxXoP[1] - m_pdMinMaxXoP[0]) * temp / pow((1 + temp), 2);
    return temp;
}

double CInvertLogisticFit::PartialP(double x)
{
    double zero, temp;
    zero = x;
    if (zero <= 0)
    {
        zero = 0.000001;
    }

    temp = exp(-1 * (m_pdMinMaxXoP[2] + m_pdMinMaxXoP[3] * log(zero)));
    temp = (m_pdMinMaxXoP[1] - m_pdMinMaxXoP[0]) * temp * log(zero) / pow((1 + temp), 2);
    return temp;
}

double CInvertLogisticFit::Residual(double *pdIndex)
{
    double Ymin, Ymax, Xo, P, temp, SD;
    double ErrMedian, Threshold, TempMax;
    double *ErrSequence;
    int i, j;

    Ymin = pdIndex[0];
    Ymax = pdIndex[1];
    Xo = pdIndex[2];
    P = pdIndex[3];

    ErrMedian = 0;
    SD = 0;
    TempMax = 0;

    ErrSequence = new double [m_iListLength];

    for (i = 0; i < m_iListLength; i++)
    {
        if (m_pdXList[i] <= 0)
        {
            temp = Ymin + (Ymax - Ymin) / (1 + exp(-(Xo + P * log(0.000001))));
        }
        else
        {
            temp = Ymin + (Ymax - Ymin) / (1 + exp(-(Xo + P * log(m_pdXList[i]))));
        }
        temp -= m_pdYList[i];
        //if(i == m_iListLength - 2)
        //{
        //  temp *= 1;
        //}
        //else if(i == m_iListLength - 1)
        //{
        //  temp *= 8;
        //}
        ErrSequence[i] = abs(temp);
        SD += temp * temp;
    }

    for (i = 0; i < m_iListLength; i++)
    {
        TempMax = ErrSequence[i];
        for (j = i; j < m_iListLength; j++)
        {
            if (ErrSequence[j] > TempMax)
            {
                TempMax = ErrSequence[j];
                ErrSequence[j] = ErrSequence[i];
                ErrSequence[i] = TempMax;
            }
        }
    }

    double temp2, l;
    l = (double)m_iListLength;
    temp2 = floor(l / 2);

    i = (int)temp2;

    if (i == (l / 2))
    {
        ErrMedian = (ErrSequence[i] + ErrSequence[i - 1]) / 2;
    }
    else
    {
        ErrMedian = ErrSequence[i];
    }

    Threshold = 1.345 * 1.4826 * ErrMedian;

    //Threshold = 0.00001*ErrMedian;
    for (i = 0; i < m_iListLength; i++)
    {
        if (m_pdXList[i] <= 0)
        {
            temp = Ymin + (Ymax - Ymin) / (1 + exp(-(Xo + P * log(0.000001))));
        }
        else
        {
            temp = Ymin + (Ymax - Ymin) / (1 + exp(-(Xo + P * log(m_pdXList[i]))));
        }
        temp -= m_pdYList[i];

        for (j = 0; j < m_iListLength; j++)
        {
            if (j == i)
            {
                if (abs(temp) <= Threshold)
                {
                    m_pdWeight[i * m_iListLength + j] = 1;
                }
                else
                {
                    m_pdWeight[i * m_iListLength + j] = 1;    //Threshold/abs(temp);
                }
            }
            else
            {
                m_pdWeight[i * m_iListLength + j] = 0;
            }
        }
    }
    delete ErrSequence;
    return SD;
}

double CInvertLogisticFit::LogisticFitSol(double *x, double *y, double *MinMaxXoP, int iListLen, int InputIterNum)
{
    int Err, iIterNum;
    int i, j, k;
    double *m_pdInterceptSlope, *pdTempCMatrix, *m_pdTempIndex, dbResidual, temp;

    m_iListLength = iListLen;

    if (m_pdXList != NULL)
    {
        delete m_pdXList;
        m_pdXList = NULL;
    }
    m_pdXList = new double[m_iListLength];
    if (NULL == m_pdXList)
    {
        return FALSE;
    }

    if (m_pdYList != NULL)
    {
        delete m_pdYList;
        m_pdYList = NULL;
    }
    m_pdYList = new double[m_iListLength];
    if (NULL == m_pdYList)
    {
        return FALSE;
    }

    if (m_pdWeight != NULL)
    {
        delete m_pdWeight;
        m_pdWeight = NULL;
    }
    m_pdWeight = new double[m_iListLength * m_iListLength];
    if (NULL == m_pdWeight)
    {
        return FALSE;
    }

    if (m_pdMinMaxXoP != NULL)
    {
        delete m_pdMinMaxXoP;
        m_pdMinMaxXoP = NULL;
    }
    m_pdMinMaxXoP = new double[4];
    if (NULL == m_pdMinMaxXoP)
    {
        return FALSE;
    }

    m_pdInterceptSlope = new double[2];
    if (NULL == m_pdInterceptSlope)
    {
        return FALSE;
    }

    m_pdTempIndex = new double [4];
    if (NULL == m_pdTempIndex)
    {
        return FALSE;
    }

    InputXY(x, y);
    LnXLogistY();


    //////////////////////////////////////////////////////////////////////
    ///////////               求四参数的初值                //////////////
    //////////////////////////////////////////////////////////////////////
    if ((MinMaxXoP[0] == 0) && (MinMaxXoP[1] == 0) && (MinMaxXoP[2] == 0) && (MinMaxXoP[3] == 0))
    {
        pdTempCMatrix = new double [m_iEffectPoint * 2];
        for (i = 0; i < m_iEffectPoint; i++)
        {
            pdTempCMatrix [i * 2] = 1;
            pdTempCMatrix [i * 2 + 1] = m_pdLnX[i];
        }
        //直线拟和求Xo,p
        //CFitting fit;
        //dbResidual = fit.SolutionCLEG(pdTempCMatrix, m_pdLogisticY, m_pdInterceptSlope, m_iEffectPoint, 2);
        dbResidual = CLeastSquare::LeastSquareSolution(pdTempCMatrix, m_pdLogisticY, m_pdInterceptSlope, m_iEffectPoint, 2);
        m_pdMinMaxXoP[0] = m_dYmin - 0.1;
        m_pdMinMaxXoP[1] = m_dYmax + 1;
        m_pdMinMaxXoP[3] = -1 * m_pdInterceptSlope[1]; //将最小二乘法求得的结果换算为x0、p
        m_pdMinMaxXoP[2] = -1 * m_pdInterceptSlope[0];

        m_dSqrtR = RelativeIndex(m_pdMinMaxXoP);//计算初值得到的相关系数，生成加权因子矩阵

        if (pdTempCMatrix != NULL)
        {
            delete pdTempCMatrix;
        }

    }

    else
    {
        memcpy(m_pdMinMaxXoP, MinMaxXoP, sizeof(double) * 4);
    }

    /*m_pdMinMaxXoP[0] = -57726;
    m_pdMinMaxXoP[1] = 553359;
    m_pdMinMaxXoP[3] = 1.08; //将最小二乘法求得的结果换算为x0、p
    m_pdMinMaxXoP[2] = 1.5;*/

    ////////////////////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////
    ///////////////           迭代求解                ////////////
    //////////////////////////////////////////////////////////////
    double NewtonFactor, MaxSqrtR1, MaxSqrtR, MaxSqrtR2, BestNewtonFactor, BestNewtonFactor1, BestNewtonFactor2;
    double Lamda = 1;
    double *TempMinMaxXoP;
    temp = MaxSqrtR = 0;

    TempMinMaxXoP = new double [4];
    if (NULL == m_pdTempIndex)
    {
        return FALSE;
    }

    if (InputIterNum == 0)
    {
        iIterNum = 10000;
    }
    else
    {
        iIterNum = InputIterNum;
    }

    m_dSqrtR = RelativeIndex(m_pdMinMaxXoP);

    int m;

    NewtonFactor = 1;
    MaxSqrtR = 0;
    for (m = 0; m < iIterNum; m++)
    {
        //////下降Newton法
        MaxSqrtR = MaxSqrtR1 = MaxSqrtR2 = m_dSqrtR;
        BestNewtonFactor = BestNewtonFactor1 = BestNewtonFactor2 = 2;

        Err = FormCYMatrix();
        if (Err == FALSE)
        {
            return Err;
        }

        //CFitting fit;
        //dbResidual = fit.SolutionCLEG(m_pdPartialMatrix, m_pdYMatrix, m_pdTempIndex, m_iListLength, 4);
        dbResidual = CLeastSquare::LeastSquareSolution(m_pdPartialMatrix, m_pdYMatrix, m_pdTempIndex, m_iListLength, 4);

        NewtonFactor = 2;
        for (k = 0; k < 10; k++)
        {
            for (j = 0; j < 4; j++)
            {
                TempMinMaxXoP[j] = m_pdMinMaxXoP[j] + m_pdTempIndex[j] * NewtonFactor;
            }

            temp = RelativeIndex(TempMinMaxXoP);

            if (temp > MaxSqrtR1)
            {
                MaxSqrtR1 = temp;
                BestNewtonFactor1 = NewtonFactor;
            }

            NewtonFactor = NewtonFactor / 2;
        }

        NewtonFactor = 2;
        for (k = 0; k < 10; k++)
        {
            for (j = 0; j < 4; j++)
            {
                TempMinMaxXoP[j] = m_pdMinMaxXoP[j] + m_pdTempIndex[j] * NewtonFactor;
            }

            temp = RelativeIndex(TempMinMaxXoP);

            if (temp > MaxSqrtR2)
            {
                MaxSqrtR2 = temp;
                BestNewtonFactor2 = NewtonFactor;
            }

            NewtonFactor = NewtonFactor * 2;
        }

        if (MaxSqrtR2 > MaxSqrtR1)
        {
            MaxSqrtR = MaxSqrtR2;
            BestNewtonFactor = BestNewtonFactor2;
        }
        else
        {
            MaxSqrtR = MaxSqrtR1;
            BestNewtonFactor = BestNewtonFactor1;
        }

        if ((m != 0) && (MaxSqrtR <= m_dSqrtR))
        {
            break;
        }

        else
        {
            m_dSqrtR = MaxSqrtR;

            for (j = 0; j < 4; j++)
            {
                m_pdMinMaxXoP[j] = m_pdMinMaxXoP[j] + m_pdTempIndex[j] * BestNewtonFactor;
            }
        }

    }

    memcpy(MinMaxXoP, m_pdMinMaxXoP, 32);

    if (m_pdTempIndex != NULL)
    {
        delete m_pdTempIndex;
    }


    if (m_pdInterceptSlope != NULL)
    {
        delete m_pdInterceptSlope;
    }

    if (TempMinMaxXoP != NULL)
    {
        delete TempMinMaxXoP;
    }

    //return m_dSqrtR;
    return MaxSqrtR;
}

BOOL CInvertLogisticFit::FormCYMatrix(void)
{
    int i;
    double temp;

    if (m_pdPartialMatrix != NULL)
    {
        delete m_pdPartialMatrix;
        m_pdPartialMatrix = NULL;
    }

    m_pdPartialMatrix = new double [m_iListLength * 4];
    if (NULL == m_pdPartialMatrix)
    {
        return FALSE;
    }

    if (m_pdYMatrix != NULL)
    {
        delete m_pdYMatrix;
        m_pdYMatrix = NULL;
    }

    m_pdYMatrix = new double [m_iListLength * 4];
    if (NULL == m_pdYMatrix)
    {
        return FALSE;
    }

    for (i = 0; i < m_iListLength; i++)
    {
        temp = m_pdXList[i];
        m_pdPartialMatrix[i * 4] = PartialYmin(temp) * m_pdWeight[i * m_iListLength + i]; //乘权重因子
        m_pdPartialMatrix[i * 4 + 1] = PartialYmax(temp) * m_pdWeight[i * m_iListLength + i];
        m_pdPartialMatrix[i * 4 + 2] = PartialXo(temp) * m_pdWeight[i * m_iListLength + i];
        m_pdPartialMatrix[i * 4 + 3] = PartialP(temp) * m_pdWeight[i * m_iListLength + i];
    }

    for (i = 0; i < m_iListLength; i++)
    {
        temp = m_pdXList[i];
        temp = FunctionValue(temp);
        m_pdYMatrix[i] = m_pdYList[i] - temp;
    }

    return TRUE;
}

double CInvertLogisticFit::RelativeIndex(double *pdIndex)
{
    int l;
    double SD = 0, temp = 0, Var = 0, dRelatedCoef = 0;
    SD = Residual(pdIndex);

    temp = 0;
    for (l = 0; l < m_iListLength; l++) //求Var＝Sum(Yi-Aver(Yi))^2
    {
        temp += pow((m_pdYList[l] - m_dYMean), (double)2);
    }

    Var = temp;
    if (Var == 0)
    {
        Var = 0.0000001;
    }
    dRelatedCoef = 1 - SD / Var;

    return dRelatedCoef;
}


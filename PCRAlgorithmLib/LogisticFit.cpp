#include "StdAfx.h"
#include <math.h>

#include "LogisticFit.h"


CLogisticFit::CLogisticFit(void):
    m_pdXList(NULL),
    m_pdYList(NULL),
    m_pdLogisticY(NULL),
    m_pdLnX(NULL),
    m_pdPartialMatrix(NULL),
    m_pdYMatrix(NULL),
    m_pdMinMaxXoP(NULL),
    m_pdWeight(NULL)
    , m_dZero(0.0000000001)
{
    m_dYmin = m_dYmax = m_dXmin = 0;
    m_dSqrtR = 0;
    m_iListLength = 2;
}

CLogisticFit::~CLogisticFit(void)
{
    if (m_pdXList != NULL)
    {
        delete m_pdXList;
    }

    if (m_pdYList != NULL)
    {
        delete m_pdYList;
    }

    if (m_pdLogisticY != NULL)
    {
        delete m_pdLogisticY;
    }

    if (m_pdLnX != NULL)
    {
        delete m_pdLnX;
    }

    if (m_pdPartialMatrix != NULL)
    {
        delete m_pdPartialMatrix;
    }

    if (m_pdYMatrix != NULL)
    {
        delete m_pdYMatrix;
    }

    if (m_pdMinMaxXoP != NULL)
    {
        delete m_pdMinMaxXoP;
    }

    if (m_pdWeight != NULL)
    {
        delete m_pdWeight;
    }
}

/**
 * 输入x列，y列数据给m_pdXList，m_pdYList；同时计算y列的最大、最小值和平均值
 * \param *x 输入的x列实验数据
 * \param *y 输入的y列实验数据
 * \return
 */
BOOL CLogisticFit::InputXY(double *x, double *y)
{
    int k = 0;
    int CountLen = 0;
    double SumY, maxtemp, mintemp, Xmintemp;
    maxtemp = y[0];
    mintemp = y[0];
    Xmintemp = 9999999999;
    SumY = 0;

    for (k = 0; k < m_iListLength; k++) //读入x，y列；小于等于0的点置为0.00000001；
    {
        //求最大最小值及平均值maxtemp,mintemp
        m_pdXList[k] = x[k];
        m_pdYList[k] = y[k];
        /*  if(m_pdXList[k] <= 0)
            {
                CountLen++;
                m_pdXList[k] = 0.0001;
            }*/

        SumY = m_pdYList[k] + SumY;
        if (maxtemp < m_pdYList[k])
        {
            maxtemp = m_pdYList[k];
        }
        if (mintemp > m_pdYList[k])
        {
            mintemp = m_pdYList[k];
        }
        if ((Xmintemp > m_pdXList[k]) && (m_pdXList[k] > 0))
        {
            Xmintemp = m_pdXList[k];
        }
    }
    m_dYmax = maxtemp;
    m_dYmin = mintemp;
    m_dYMean = SumY / m_iListLength;

    m_dXmin = Xmintemp;

    //m_iEffectPoint = m_iListLength - CountLen;
    m_iEffectPoint = m_iListLength;
    return TRUE;
}

/**
 * 对x列取ln，对y列取logistic，另A0的初值为Ymin*0.5，A1的初值为Ymax*1.1
 * \return
 */
BOOL CLogisticFit::LnXLogistY()
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
        temp = (m_pdYList[j] - min) / (max - m_pdYList[j]);

        m_pdLogisticY[i] = log(temp);

        if (m_pdXList[j] <= 0)
            //j++;
            //m_pdLnX[i] = log(m_dXmin/2);
            //m_pdLnX[i] = log (0.001);
        {
            m_pdLnX[i] = log(m_dZero);
        }
        else
        {
            m_pdLnX[i] = log(m_pdXList[j]);
        }

        j++;
    }
    return TRUE;
}

double CLogisticFit::FunctionValue(double x)
{
    double temp;
    temp = m_pdMinMaxXoP[0] + (m_pdMinMaxXoP[1] - m_pdMinMaxXoP[0]) / (1 + pow((x / m_pdMinMaxXoP[2]), m_pdMinMaxXoP[3]));
    return temp;
}

double CLogisticFit::PartialYmin(double x)
{
    double temp;
    temp = 1 - 1 / (1 + pow((x / m_pdMinMaxXoP[2]), m_pdMinMaxXoP[3]));
    return temp;
}

double CLogisticFit::PartialYmax(double x)
{
    double temp;
    temp = 1 / (1 + pow((x / m_pdMinMaxXoP[2]), m_pdMinMaxXoP[3]));
    return temp;
}

double CLogisticFit::PartialXo(double x)
{
    double zero, temp;
    zero = x;
    if (zero <= 0)
    {
        zero = m_dZero;
    }

    temp = pow((zero / m_pdMinMaxXoP[2]), m_pdMinMaxXoP[3]);
    temp = m_pdMinMaxXoP[3] * temp * (m_pdMinMaxXoP[1] - m_pdMinMaxXoP[0]) / (1 + temp) / (1 + temp) / m_pdMinMaxXoP[2];
    return temp;
}

double CLogisticFit::PartialP(double x)
{
    double zero, temp;
    zero = x;
    if (zero <= 0)
    {
        zero = m_dZero;
    }

    temp = pow((zero / m_pdMinMaxXoP[2]), m_pdMinMaxXoP[3]);
    temp =  temp = log(zero / m_pdMinMaxXoP[2]) * temp * (m_pdMinMaxXoP[0] - m_pdMinMaxXoP[1]) / (1 + temp) / (1 + temp);

    return temp;
}

/**
 * 生成偏导数矩阵m_pdPartialMatrix，残差矩阵m_pdYMatrix ＝ Y－Y^
 * \return
 */
BOOL CLogisticFit::FormCYMatrix()
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

/**
 * 求残差，同时生成加权因子矩阵，加权因子为对角阵，对角线上的值为各Y值与拟合的Y值的差的绝对值。
 * \param *Index 输入四个参数值
 * \return 残差
 */
double CLogisticFit::Residual(double *Index)
{
    double Ymin, Ymax, Xo, P, temp, SD;
    double ErrMedian, Threshold, TempMax;
    double *ErrSequence;
    int i, j;

    Ymin = Index[0];
    Ymax = Index[1];
    Xo = Index[2];
    P = Index[3];

    ErrMedian = 0;
    SD = 0;
    TempMax = 0;

    ErrSequence = new double [m_iListLength];

    for (i = 0; i < m_iListLength; i++)
    {
        temp = Ymin + (Ymax - Ymin) / (1 + pow((m_pdXList[i] / Xo), P));
        temp -= m_pdYList[i];
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

        temp = Ymin + (Ymax - Ymin) / (1 + pow((m_pdXList[i] / Xo), P));
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

/**
 * 求相关系数，调用Residual求残差的同时，生成加权因子矩阵
 * \param *Index 输入四个参数值
 * \return 相关系数
 */
double CLogisticFit::RelativeIndex(double *Index)
{
    int l;
    double SD = 0, temp = 0, Var = 0, dRelatedCoef = 0;
    SD = Residual(Index);

    temp = 0;
    for (l = 0; l < m_iListLength; l++) //求Var＝Sum(Yi-Aver(Yi))^2
    {
        temp += pow((m_pdYList[l] - m_dYMean), (double)2);
    }

    Var = temp;
    if (Var == 0)
    {
        Var = m_dZero;
    }
    dRelatedCoef = 1 - SD / Var;

    return dRelatedCoef;
}

/**
 * Gauss-Newton迭代法求四参数的增量A=(P'WP)^(-1)P'WY，并计算四参数（m_pdMinMaxXoP）新的值。首先生成四参数偏导数的系数矩阵
 * \param
 * \return 1表示正确，<0表示错误，k+1表示最小二乘法中选主元法的计算中第k列（行）出现了小于ZERO的值
 */
int CLogisticFit::IterParam(double Factor, double *DeltaParam)
{
    int i, Err = 0;
    double *TempMatrix1, *TempMatrix2, *TempMatrix3, *TempMatrix4, *TempMatrix5;

    TempMatrix1 = new double [m_iListLength * 4]; //4*n
    if (NULL == TempMatrix1)
    {
        return FALSE;
    }

    TempMatrix2 = new double [16];//4*4
    if (NULL == TempMatrix2)
    {
        return FALSE;
    }

    TempMatrix3 = new double [16];//4*4
    if (NULL == TempMatrix3)
    {
        return FALSE;
    }

    TempMatrix4 = new double [m_iListLength * 4]; //4*n
    if (NULL == TempMatrix4)
    {
        return FALSE;
    }

    TempMatrix5 = new double [4]; //4*1
    if (NULL == TempMatrix5)
    {
        return FALSE;
    }

    Err = FormCYMatrix();
    if (Err == FALSE)
    {
        return Err;
    }

    //求偏导矩阵的转置，TempMatrix1 = P';
    Err = CLeastSquare::MatrixTranspose(m_pdPartialMatrix, TempMatrix1, m_iListLength, 4);
    if (Err == FALSE)
    {
        return Err;
    }
    //P'W = TempMatrix1;
    Err = CLeastSquare::MatrixMultiply(TempMatrix1, m_pdWeight, TempMatrix1, 4, m_iListLength, m_iListLength);
    if (Err == FALSE)
    {
        return Err;
    }
    //P'WP = TempMatrix2;
    Err = CLeastSquare::MatrixMultiply(TempMatrix1, m_pdPartialMatrix, TempMatrix2, 4, m_iListLength, 4);
    if (Err == FALSE)
    {
        return Err;
    }
    //(P'WP)^-1 = TempMatrix3;
    Err = CLeastSquare::MatrixInversion(TempMatrix2, TempMatrix3, 4);
    if (Err == FALSE)
    {
        return Err;
    }
    //(P'WP)^-1*P'W = TempMatrix4;
    Err = CLeastSquare::MatrixMultiply(TempMatrix3, TempMatrix1, TempMatrix4, 4, 4, m_iListLength);
    if (Err == FALSE)
    {
        return Err;
    }
    //(P'WP)^-1*P'WY
    Err = CLeastSquare::MatrixMultiply(TempMatrix4, m_pdYMatrix, TempMatrix5, 4, m_iListLength, 1);
    if (Err == FALSE)
    {
        return Err;
    }

    /*for (i = 0; i<4; i++)
        m_pdMinMaxXoP[i] += Factor * TempMatrix5[i];*/

    for (i = 0; i < 4; i++)
    {
        TempMatrix5[i] =  Factor * TempMatrix5[i];
    }

    memcpy(DeltaParam, TempMatrix5, 32);

    delete TempMatrix1;
    delete TempMatrix2;
    delete TempMatrix3;
    delete TempMatrix4;
    delete TempMatrix5;

    return 1;
}

double CLogisticFit::LogisticFitSol(double *x, double *y, double *MinMaxXoP, int iListLen, int InputIterNum)
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
        dbResidual = CLeastSquare::LeastSquareSolution(pdTempCMatrix, m_pdLogisticY, m_pdInterceptSlope, m_iEffectPoint, 2);
        m_pdMinMaxXoP[0] = m_dYmin - 0.1;
        m_pdMinMaxXoP[1] = m_dYmax + 1;
        m_pdMinMaxXoP[3] = -1 * m_pdInterceptSlope[1]; //将最小二乘法求得的结果换算为x0、p
        m_pdMinMaxXoP[2] = exp(-1 * m_pdInterceptSlope[0] / m_pdInterceptSlope[1]);

        m_dSqrtR = RelativeIndex(m_pdMinMaxXoP);//计算初值得到的相关系数，生成加权因子矩阵

        if (pdTempCMatrix != NULL)
        {
            delete pdTempCMatrix;
        }

    }

    else
    {
        memcpy(m_pdMinMaxXoP, MinMaxXoP, 32);
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


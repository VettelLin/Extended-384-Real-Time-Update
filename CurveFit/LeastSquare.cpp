#include "StdAfx.h"
#include <math.h>

#include "LeastSquare.h"



CLeastSquare::CLeastSquare(void):
    m_pdXlist(NULL),
    m_pdYlist(NULL),
    m_pCMatrix(NULL),
    m_pAMatrix(NULL)
{
    m_iCm = 6;
    m_iCn = 4;
    m_iAm = 4;
    m_iAn = 5;
    m_dXListAver = 1;
    m_dYListAver = 1;
    ZERO = 1.0E-30;
    dbRelatedCoef = 0;
}

CLeastSquare::~CLeastSquare(void)
{

    if (m_pdXlist != NULL)
    {
        delete m_pdXlist;
    }
    if (m_pdYlist != NULL)
    {
        delete m_pdYlist;
    }
    if (m_pAMatrix != NULL)
    {
        delete m_pAMatrix;
    }
    if (m_pCMatrix != NULL)
    {
        delete m_pCMatrix;
    }
}

/**
 * 选主元法把A[k][k],a[k+1][k],...a[n][k]
 * \param k: 主对角线的位置
 * \return 返回最大值
 */
double CLeastSquare::GetMainElement(int k)
{
    int i, j, Flag;
    double temp, Max;
    Flag = k;
    Max = fabs(m_pAMatrix [k * m_iAn + k]);

    for (i = +1; i < m_iAm; i++)
    {
        temp = fabs(m_pAMatrix [i * m_iAn + k]);
        if (temp <= Max)
        {
            continue;
        }

        Flag = i;
        Max = temp;
    }

    for (j = 0; j < m_iAn; j++)
    {
        temp = m_pAMatrix[k * m_iAn + j];
        m_pAMatrix[k * m_iAn + j] = m_pAMatrix[Flag * m_iAn + j];
        m_pAMatrix[Flag * m_iAn + j] = temp;
    }
    return Max;
}

/**
 * 高斯－约当法
 * \return 若主元最大值小于ZERO（1E-30），返回-11；计算正确返回1
 */
int CLeastSquare::Jordan()
{
    int i, j, k;
    double temp, test = 0;

    for (k = 0; k < m_iAm; k++)
    {
        test = GetMainElement(k);
        if (test < ZERO) //主元（每列的最大值）小于0
        {
            return -1;
        }
        temp = m_pAMatrix[k * m_iAn + k];

        for (j = k; j < m_iAn; j++)
        {
            m_pAMatrix[k * m_iAn + j] /= temp;
        }

        for (i = 0; i < m_iAm; i++)
        {
            if (i == k)
            {
                continue;
            }
            temp = m_pAMatrix[i * m_iAn + k];

            for (j = k; j < m_iAn; j++)
            {
                m_pAMatrix[i * m_iAn + j] -= temp * m_pAMatrix[k * m_iAn + j];
            }
        }
    }
    return 1;
}

/**
* 输入系数矩阵
* \param *CMatrix 外部生成的系数矩阵，将它的值付给类中的m_pCMatrix
* \return TRUE
*/
BOOL CLeastSquare::FormCMatrix(double *CMatrix)
{
    if (m_pCMatrix != NULL)
    {
        delete m_pCMatrix;
        m_pCMatrix = NULL;
    }
    m_pCMatrix = new double [m_iCm * m_iCn];
    if (NULL == m_pCMatrix)
    {
        return FALSE;
    }

    memcpy(m_pCMatrix, CMatrix, m_iCm * m_iCn * 8);
    return TRUE;
}

BOOL CLeastSquare::FormNormalEquation()
{
    int i, j, k;
    double temp;

    if (m_pAMatrix != NULL)
    {
        delete m_pAMatrix;
        m_pAMatrix = NULL;
    }
    m_pAMatrix = new double [m_iAm * m_iAn]; //为增广矩阵分配空间 M*(M+1)
    if (NULL == m_pAMatrix)
    {
        return FALSE;
    }
    ////////////////////////////////////
    for (i = 0; i < m_iCn; i++)
        for (j = i; j < m_iCn; j++) //C的转置乘C
        {
            temp = 0;
            for (k = 0; k < m_iCm; k++)
            {
                temp += m_pCMatrix[k * m_iCn + i] * m_pCMatrix[k * m_iCn + j];
            }

            m_pAMatrix[i * m_iAn + j] = temp;
            m_pAMatrix[j * m_iAn + i] = temp;
        }
    for (i = 0; i < m_iCn; i++) //C的转置乘Y
    {
        temp = 0;
        for (k = 0; k < m_iCm; k++)
        {
            temp += m_pCMatrix[k * m_iCn + i] * m_pdYlist[k];
        }

        m_pAMatrix[i * m_iAn + m_iAm] = temp;
    }
    return TRUE;
}

/**
 * 求矩阵的转置
 * \param *OriginalMatrix 原始矩阵
 * \param *TransposeMatrix 转置矩阵
 * \param RowNum 原始矩阵的行数
 * \param ListNum 原始矩阵的列数
 * \return TURE
 */
BOOL CLeastSquare::MatrixTranspose(double *OriginalMatrix, double *TransposeMatrix, int RowNum, int ListNum)
{
    int i, j, Err;
    double temp;
    m_iCm = RowNum;
    m_iCn = ListNum;

    Err = FormCMatrix(OriginalMatrix);
    if (FALSE == Err)
    {
        return Err;
    }
    for (i = 0; i < m_iCm; i++)
    {
        for (j = 0; j < m_iCn; j++)
        {
            temp = m_pCMatrix[i * m_iCn + j];
            m_pCMatrix[i * m_iCn + j] = m_pCMatrix[j * m_iCn + i];
            m_pCMatrix[j * m_iCn + i] = temp;
        }
    }
    memcpy(TransposeMatrix, m_pCMatrix, m_iCn * m_iCm * 8);
    return TRUE;
}



/**
 * 求n阶矩阵的逆
 * \param *OriginalMatrix 输入原始矩阵的值
 * \param *InverseMatrix 输出其逆矩阵
 * \param Dimension 矩阵的阶数
 * \return 返回值＝0，表示正常；大于0（＝k＋1），表示选主元计算时
 */
int CLeastSquare::MatrixInversion(double *OriginalMatrix, double *InverseMatrix, int Dimension)
{
    int i, j, ErrCode = 0;

    double *dTempMatrix;
    dTempMatrix = NULL;

    m_iAm = Dimension;
    m_iAn = 2 * Dimension;

    /*if (dTempMatrix != NULL)
    {
        delete dTempMatrix;
        dTempMatrix = NULL;
    }*/
    dTempMatrix = new double [m_iAm * m_iAm];
    if (NULL == dTempMatrix)
    {
        return -1;
    }

    memcpy(dTempMatrix, OriginalMatrix, m_iAm * m_iAm * 8); //把原始矩阵的值付给temp矩阵

    if (m_pAMatrix != NULL)
    {
        delete m_pAMatrix;
        m_pAMatrix = NULL;
    }
    m_pAMatrix = new double [m_iAm * m_iAn];
    if (NULL == m_pAMatrix)
    {
        return -1;
    }

    for (i = 0; i < m_iAm; i++) //生成2m*m阶矩阵A，左边为原始矩阵的值，右边为单位矩阵
    {
        for (j = 0; j < m_iAm; j++)
        {
            m_pAMatrix [i * m_iAn + j] = dTempMatrix [i * m_iAm + j];
            m_pAMatrix [i * m_iAn + j + m_iAm] = 0.0;
        }
        m_pAMatrix [i * m_iAn + i + m_iAm] = 1.0;
    }

    ErrCode = Jordan(); //对2m*m阶矩阵A做变换，使得其左边为单位矩阵，右边为原始矩阵的逆矩阵

    for (i = 0; i < m_iAm; i++)//将矩阵A右边的逆矩阵付给Temp矩阵，再将temp矩阵输出给InverseMatrix
    {
        for (j = 0; j < m_iAm; j++)
        {
            dTempMatrix [i * m_iAm + j] = m_pAMatrix [i * m_iAn + j + m_iAm];
        }
    }
    memcpy(InverseMatrix, dTempMatrix, m_iAm * m_iAm * 8);

    if (dTempMatrix != NULL)
    {
        delete dTempMatrix;
    }

    return ErrCode;
}

BOOL CLeastSquare::MatrixMultiply(double *MatrixA, double *MatrixB, double *MatrixMultiply, int A_RowNum, int A_ListNum, int B_ListNum)
{
    int iAm, iAn, iBm, iBn;
    int i, j, k;
    double temp, *dbMatrixA, *dbMatrixB, *dbMatrixMultiply;

    iAm = A_RowNum;
    iAn = iBm = A_ListNum;
    iBn = B_ListNum;

    dbMatrixA = new double [iAm * iAn];
    if (NULL == dbMatrixA)
    {
        return FALSE;
    }

    dbMatrixB = new double [iBm * iBn];
    if (NULL == dbMatrixB)
    {
        return FALSE;
    }

    dbMatrixMultiply = new double [iAm * iBn];
    if (NULL == dbMatrixMultiply)
    {
        return FALSE;
    }

    memcpy(dbMatrixA, MatrixA, iAm * iAn * 8);
    memcpy(dbMatrixB, MatrixB, iBm * iBn * 8);

    for (i = 0; i < iAm; i++)
    {
        for (j = 0; j < iBn; j++)
        {
            temp = 0;
            for (k = 0; k < iAn; k++)
            {
                temp += dbMatrixA [i * iAn + k] * dbMatrixB [k * iBn + j];
            }
            dbMatrixMultiply [i * iBn + j] = temp;
        }
    }
    memcpy(MatrixMultiply, dbMatrixMultiply, iAm * iBn * 8);

    delete dbMatrixA;
    delete dbMatrixB;
    delete dbMatrixMultiply;

    return TRUE;
}


double CLeastSquare::Residual()
{
    int l, m;
    double SD = 0, temp = 0;
    for (l = 0; l < m_iCm; l++) //求标准差SD
    {
        temp = 0;
        for (m = 0; m < m_iCn; m++)
        {
            temp += m_pdXlist[m] * m_pCMatrix[l * m_iCn + m];
        }
        temp -= m_pdYlist[l];
        SD += temp * temp;
    }
    return SD;
}

/**
 * 求线性相关系数
 * \return R^2
 */
double CLeastSquare::RelatedCoef()
{
    int l;
    double SD = 0, temp = 0, Var = 0, dRelatedCoef = 0;

    SD = Residual();

    temp = 0;
    for (l = 0; l < m_iCm; l++) //求Var＝Sum(Yi-Aver(Yi))^2
    {
        temp += pow((m_pdYlist[l] - m_dYListAver), (double)2);
    }

    Var = temp;
    if (Var == 0)
    {
        Var = 0.000000001;
    }
    dRelatedCoef = 1 - SD / Var;

    return dRelatedCoef;
}


/**
 * 最小二乘法解线性方程组
 * \param *CMatrix 输入系数矩阵，YListLen*N_jie维
 * \param *y 输入方程组的y值数列，YListLen维
 * \param *x 输出解得的x值数列，N_jie维
 * \param YListLen: Y列的长度
 * \param N_jie: X列的长度
 * \return 残差和 （误差的平方和）
 */
double CLeastSquare::LeastSquareSolution(double *CMatrix, double *y, double *x, int YListLen, int N_jie) //求解
{
    int k, ErrCode = 0;
    double dbResidual = 0;
    double SumX = 0, SumY = 0;

    m_iCm = YListLen;
    m_iCn = N_jie;
    m_iAm = N_jie;
    m_iAn = N_jie + 1;

    if (m_pdXlist != NULL)
    {
        delete m_pdXlist;
        m_pdXlist = NULL;
    }
    m_pdXlist = new double [N_jie];
    if (NULL == m_pdXlist)
    {
        return FALSE;
    }

    if (m_pdYlist != NULL)
    {
        delete m_pdYlist;
        m_pdYlist = NULL;
    }
    m_pdYlist = new double[YListLen];
    if (NULL == m_pdYlist)
    {
        return FALSE;
    }

    for (k = 0; k < N_jie; k++)
    {
        m_pdXlist[k] = x[k];
        SumX = SumX + x[k];
    }
    for (k = 0; k < YListLen; k++)
    {
        m_pdYlist[k] = y[k];
        SumY = SumY + y[k];
    }

    m_dXListAver = SumX / N_jie;
    m_dYListAver = SumY / YListLen;

    FormCMatrix(CMatrix);
    FormNormalEquation();
    ErrCode = Jordan();
    if (ErrCode == -1)
    {
        return -1;
    }

    for (k = 0; k < m_iAm; k++)
    {
        m_pdXlist[k] = m_pAMatrix[k * m_iAn + m_iAn - 1];
        //m_pdXlist[k] = 1;
    }

    dbResidual = Residual();
    dbRelatedCoef = RelatedCoef();

    memcpy(x, m_pdXlist, N_jie * 8);

    return dbRelatedCoef;
}


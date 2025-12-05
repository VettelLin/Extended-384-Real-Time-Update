#pragma once


class AFX_EXT_CLASS CLeastSquare
{
    double *m_pdXlist;
    double *m_pdYlist;
    int m_iCm, m_iCn, m_iAm, m_iAn;
    double ZERO;
    double m_dXListAver, m_dYListAver; //x列和y列的平均值
    double *m_pCMatrix; //系数矩阵
    double *m_pAMatrix; //C的转置乘C+C的转置乘Y，N行，M+1列
    double *m_pdFinalCoef;

    double GetMainElement(int k);
    int Jordan();  //Gauss - Jordan消去法
    BOOL FormCMatrix(double *CMatrix);
    BOOL FormNormalEquation();
    double Residual();
    double RelatedCoef();

public:
    CLeastSquare(void);
    ~CLeastSquare(void);

    double dbRelatedCoef;

    int MatrixTranspose(double *OriginalMatrix, double *TransposeMatrix, int RowNum, int ListNum);
    int MatrixInversion(double *OriginalMatrix, double *InverseMatrix, int Dimension); //矩阵求逆，OriginMatrix为原始矩阵，InverseMatrix为求得的逆矩阵
    BOOL MatrixMultiply(double *MatrixA, double *MatrixB, double *MatrixMultiply, int A_RowNum, int A_ListNum, int B_ListNum);
    double LeastSquareSolution(double *CMatrix, double *y, double *x, int Listlen, int M_jie); //求解

};


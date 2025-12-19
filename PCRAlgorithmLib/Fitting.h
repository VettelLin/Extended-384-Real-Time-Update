#pragma once

//#define iMjie 5
//#define iListLen 10



class AFX_EXT_CLASS CFitting
{
protected:
    double *m_pdXlist;
    double *m_pdYlist;
    int m_iListLen;
    int m_iMjie;
    double m_dXListAver, m_dYListAver; //x列和y列的平均值
    double *m_pCMatrix; //系数矩阵
    double *m_pAMatrix; //C的转置乘C+C的转置乘Y，N行，M+1列
    double *m_pdFinalCoef;

private:
    BOOL FormCLEG();//生成矛盾方程的系数矩阵
    BOOL FormNormalEquation();//生成正规方程的增广矩阵
    int Gauss();//Gauss消元
    //double SD();//计算标准差(残差平方和)
    double RelatedCoef();//相关系数R^2

public:
    CFitting(void);
    ~CFitting(void);
    double SolutionCLEG(double *x, double *y, double *dCoef, int len, int m); //解矛盾方程
};


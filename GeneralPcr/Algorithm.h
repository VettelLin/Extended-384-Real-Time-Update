#pragma once

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

//最小二乘法拟合相关函数定义
double Sum(vector<double> vecData);
double MultiSum(vector<double> vecX,vector<double> vecY);
double RelatePow(vector<double> vecX,int n,int ex);
double RelateMultiXY(vector<double> vecX,vector<double> vecY,int n,int ex);

//计算方程的增广矩阵
void EMatrix(vector<double> vecX,vector<double> vecY,int n,int ex,double coefficient[]);

//求解方程
void CalEquation(int exp,double coefficient[]);
double F(double c[],int l,int m);
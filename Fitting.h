//Caltextline类，用于拟合每行的文本线，并记录拟合参数。

#pragma once
#include "PageImage.h"
#include <math.h>
#define FittingN 4   //拟合的多项式的次数为FittingN-1
#define MAXPOINT 70  //每行样点个数的最大值



class Caltextline {
public:
	double Coefficient[FittingN];  //拟合系数
	int leftbound, rightbound;  //文本线左右边界
	Caltextline(); 
	void Fitting(list<int> PX, list<int> PY); //拟合函数

private:
	void f1(double *, double *, int);           //将初始x[i]的值的各幂次方存储在一个二维数组里面 
	void f2(double *, double *, double[], int);        //计算正规方程组的系数矩阵 
	void DirectLU(double a[FittingN][FittingN + 1], double[]);         //列主元LU分解
	void swap(double &, double &);          //交换两个变量的值
};
#include "stdafx.h"
#include "Fitting.h"

Caltextline::Caltextline() {
	for( int i = 0; i++; i < FittingN)
		Coefficient[i] = 0;
}

void Caltextline::Fitting(list<int> PX, list<int> PY)
{
	int ix=0, iy=0,Num;
	if (PX.size() != PY.size()) cerr << "Error!"<<endl;
	    else Num = PX.size();
	
	//将list转化为double类型数组，与算法衔接
	double *x = new double[Num];
	for (list<int>::iterator it=PX.begin(); it != PX.end(); it++) {
		*(x + ix) = *it;
		ix++;
	}
	double *y = new double[Num];
	for (list<int>::iterator it = PY.begin(); it != PY.end(); it++) {
		*(y + iy) = *it;
		iy++;
	}

	double a[FittingN][MAXPOINT];
	double b[FittingN][FittingN + 1]; //正规方程组的系数矩阵和右侧矩阵
	double c[FittingN];

	f1(a[0], x, Num);
    f2(a[0], b[0], y, Num); //计算正规方程组的系数矩阵
	DirectLU(b, c); //列主元LU分解

	//用于测试拟合系数
	/******
	cout << "-------------------------" << endl;
	cout << "拟合函数的系数分别为:\n";
	for (i = 0; i<FittingN; i++)
		cout << "a[" << i << "]=" << c[i] << endl;
	cout << "-------------------------" << endl;
	******/
	for (int i = 0; i < FittingN; i++) Coefficient[i] = c[i];
	delete[] x;
	delete[] y;
}


void Caltextline::f1(double *a, double *x, int Num)
{
	int i, j, k;
	double temp;
	for (i = 0; i<FittingN; i++)
		for (j = 0; j<Num; j++)
		{
			temp = 1;
			for (k = 0; k<i; k++)
				temp *= x[j];
			*(a + i*Num + j) = temp;
		}
}

void Caltextline::f2(double *a, double *b, double y[], int Num)
{
	int i, j, k;
	double temp2;
	for (i = 0; i<FittingN; i++)
	{
		for (j = 0; j<FittingN; j++)
		{
			temp2 = 0;
			for (k = 0; k<Num; k++)
				temp2 += *(a + i*Num + k)*(*(a + j*Num + k));
			*(b + i*(FittingN + 1) + j) = temp2;
		}
		temp2 = 0;
		for (k = 0; k<Num; k++)
		{
			temp2 += y[k] * (*(a + i*Num + k));
			*(b + i*(FittingN + 1) + FittingN) = temp2;
		}
	}
}

void Caltextline::swap(double &a, double &b)
{
	a = a + b;
	b = a - b;
	a = a - b;
}

void Caltextline::DirectLU(double a[FittingN][FittingN + 1], double x[])
{
	int i, r, k, j;
	double s[FittingN], t[FittingN];
	double max;
	for (r = 0; r<FittingN; r++)
	{
		max = 0;
		j = r;
		for (i = r; i<FittingN; i++) //求是s[i]的绝对值,选主元
		{
			s[i] = a[i][r];
			for (k = 0; k<r; k++)
				s[i] -= a[i][k] * a[k][r];
			s[i] = s[i]>0 ? s[i] : -s[i]; //s[i]取绝对值
			if (s[i]>max) {
				j = i;
				max = s[i];
			}
		}
		if (j != r) //选出的主元所在行j若不是r,则对两行相应元素进行调换
		{
			for (i = 0; i<FittingN + 1; i++)
				swap(a[r][i], a[j][i]);
		}
		for (i = r; i<FittingN + 1; i++) //记算第r行的元素
			for (k = 0; k<r; k++) {
				a[r][i] -= a[r][k] * a[k][i];
			}
		for (i = r + 1; i<FittingN; i++) //记算第r列的元素
		{
			for (k = 0; k<r; k++)
				a[i][r] -= a[i][k] * a[k][r];
			a[i][r] /= a[r][r];
		}
	}
	for (i = 0; i<FittingN; i++)
		t[i] = a[i][FittingN];
	for (i = FittingN - 1; i >= 0; i--) //利用回代法求最终解
	{
		for (r = FittingN - 1; r>i; r--)
			t[i] -= a[i][r] * x[r];
		x[i] = t[i] / a[i][i];
	}
}
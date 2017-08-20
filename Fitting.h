//Caltextline�࣬�������ÿ�е��ı��ߣ�����¼��ϲ�����

#pragma once
#include "PageImage.h"
#include <math.h>
#define FittingN 4   //��ϵĶ���ʽ�Ĵ���ΪFittingN-1
#define MAXPOINT 70  //ÿ��������������ֵ



class Caltextline {
public:
	double Coefficient[FittingN];  //���ϵ��
	int leftbound, rightbound;  //�ı������ұ߽�
	Caltextline(); 
	void Fitting(list<int> PX, list<int> PY); //��Ϻ���

private:
	void f1(double *, double *, int);           //����ʼx[i]��ֵ�ĸ��ݴη��洢��һ����ά�������� 
	void f2(double *, double *, double[], int);        //�������淽�����ϵ������ 
	void DirectLU(double a[FittingN][FittingN + 1], double[]);         //����ԪLU�ֽ�
	void swap(double &, double &);          //��������������ֵ
};
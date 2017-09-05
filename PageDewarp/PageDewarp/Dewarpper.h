#pragma once
#include "PageImage.h"
#include <numeric>
#include <opencv2/opencv.hpp>

class Dewarpper {
	/*
	Dewarpper��
	�����㷨��ʵ�֡��������̵ı����Ƕ����صķ���������������PageImage�ṩ�Ľӿڣ����Բ�����ͼƬ�����ݽṹ������δ���汾������
	���̷����¼�����1.����������õ�һϵ�����ֿ顣2.Ԥ�����ҶȻ�����ֵ����3.���������������иߡ�����ı��ߡ�ͼ���ع������㷨���ġ�
	�������̶���˽�к�����˽�б���ʵ�֡��ⲿ�ӿڽ���setImage��dewarp�������û�����ͼƬ������dewarp�õ�����������ǳ��򵥡�
	*/
public:
	Dewarpper();
	Dewarpper(const char *);
	~Dewarpper();

	int dewarp(); 
	//int setImage(const char *);  // ������ҳͼ��
	void save(const char *);  // ����ͼ��

	static enum ERROR_TYPE;

private:
	PageImage img;  // ͼƬ
	PageImage ret;
	shared_ptr<vector<cv::Rect>> *bounds;  // �����������Ľ��
	vector<cv::Rect> *LayoutRecognization();  // ��������㷨
	int preProcedure();  // Ԥ�����㷨
	int calcLineHeight();  // �����и�
	int getTextLine(const int &);  // �ı�������㷨
	int reshape(const int &);  // ���¹�ͼ�㷨
	void doSave(const PageImage *, const char *) const ;  // ִ�б���ͼ��
	list<list<double>> coefficientM;
	int lineHeight;


	inline void setElem(pair<int, int> &elem, const int &a, const int &b) { elem.first = a; elem.second = b; }
	//assist function
	bool bufcnt(bool cond,int buf, int threshold,int maxBuf = 50);
};
#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PageImage
{
	/*
	PageImage��
	��װͼƬ��ص���Ϣ�Ͳ���������ͼƬ�����ݽṹϸ�ڣ�Ϊ�����㷨�ṩ�����ͼƬ�ӿڣ�
	����������opencv����Ŀ⣬ֻ��Ҫ������������޸ļ��ɣ������㷨����������ˡ�
	1.�ṩͼƬ��Ϣ������ͼƬ�����ͼƬ����
	2.�ṩͼƬ������������ȡ���ء�ͶӰͳ�ơ��ҶȻ���������ǿ����ֵ�������͡���ֵ�����ε�
	*/
private:
	shared_ptr<Mat> img;
	string fileDirAndName;  // ��¼ͼƬ�洢λ�ú��ļ���
	string fileType; // ��¼�ļ�����
	int width, height;   // ��¼ͼƬ��ʵ�Ŀ���
	Rect bound;  // ͼƬ�Ļ�Ծ���򣬼����ڽ���������


	void setImageInfo(const string);
	void doSetBoundary(const Rect &);

public:
	PageImage(void);
	PageImage(const char *);
	~PageImage(void);


	PageImage &setImage(const Mat &);
	PageImage &setImage(const char *);
	PageImage &setBoundary(const Rect &);
	PageImage &setBoundary(int, int, int, int);
	pair<int, int> getSize() const ;
	pair<int, int> getSizeActual() const ;
	void showImage() const ;
	void showImage(const char *) const ;
	void saveImage() const ;
	void saveImage(const string&) const ;


	int getImgType() { return (*img).type();}
	//const Mat getRow(const int &row) const {return (*img).rowRange(row,row+1);}
	//const Mat getColomn(const int &col) const {return (*img).colRange(col,col+1);}
	const uchar* getRow(const int &) const ;
	uchar* getColomn(const int &) const ;

	const int getPixel(const int &row, const int &col) const {return (*img).at<uchar>(row,col);}
	PageImage &setPixel(const int &row, const int &col, const uchar &value) {(*img).at<uchar>(row,col)= value; return *this;}

	const int* vCountBlack() const ;
	const int* hCountBlack() const ;

	PageImage & grayScale();
	//void enhancement(const int *);
	PageImage & binary(const int = -1);
	PageImage & bwDilate(const pair<int, int> &, const int = 1) const ;
	PageImage & bwErode(const pair<int, int> &, const int = 1) const ;
	PageImage & bwRemap(const Mat&,const Mat &) const;
	void reshape(pair<int, int>(*)(int, int));

};
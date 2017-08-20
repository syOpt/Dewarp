#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PageImage {
	/*
	PageImage��
	��װͼƬ��ص���Ϣ�Ͳ���������ͼƬ�����ݽṹϸ�ڣ�Ϊ�����㷨�ṩ�����ͼƬ�ӿڣ�
	����������opencv����Ŀ⣬ֻ��Ҫ������������޸ļ��ɣ������㷨����������ˡ�
	1.�ṩͼƬ��Ϣ������ͼƬ�����ͼƬ����
	2.�ṩͼƬ������������ȡ���ء�ͶӰͳ�ơ��ҶȻ���������ǿ����ֵ�������͡���ֵ�����ε�
	*/
public:
	PageImage();
	PageImage(const char *);
	~PageImage();

	class Boundary {
	public:
		Boundary() {};
		Boundary(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {};
		~Boundary() {};

		int x1, y1, x2, y2;
	};

	PageImage &setImage(const Mat &);
	PageImage &setImage(const char *);
	PageImage &setBoundary(const Boundary &);
	PageImage &setBoundary(int, int, int, int);
	pair<int, int> getSize() const ;
	pair<int, int> getSizeActual() const ;
	void showImage() const ;
	void saveImage() const ;
	void saveImage(const char *) const ;

	Mat * getImg() const;
	int getImgType() { return (*img).type();}
	const uchar* getRow(const int &) const ;
	uchar* getColomn(const int &) const ;
	const uchar getPixel(const int &, const int &) const ;
	PageImage &setPixel(const int &, const int &, const uchar &);
	const int* vCountBlack() const ;
	const int* hCountBlack() const ;
	PageImage & grayScale();
	void enhancement(const int *);
	PageImage & binary(const int = -1);
	PageImage & bwDilate(const pair<int, int> &, const int = 1) const ;
	PageImage & bwErode(const pair<int, int> &, const int = 1) const ;
	PageImage & bwRemap(const Mat&,const Mat &) const;
	void reshape(pair<int, int>(*)(int, int));

private:
	Mat *img;  // ����ͼƬ
	string fileDirAndName;  // ��¼ͼƬ�洢λ�ú��ļ���
	string fileType; // ��¼�ļ�����
	int width, height;   // ��¼ͼƬ��ʵ�Ŀ���
	Boundary bound;  // ͼƬ�Ļ�Ծ���򣬼����ڽ���������

	void setFileInfo(const char *);
	void setImageInfo();
	void doSetBoundary(const Boundary &);
	void doSave(const string &) const ;
};


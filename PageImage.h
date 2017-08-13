#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PageImage {
	/*
	PageImage类
	封装图片相关的信息和操作，隐藏图片的数据结构细节，为矫正算法提供所需的图片接口，
	若后续改用opencv以外的库，只需要在这个类里作修改即可，矫正算法就无需调整了。
	1.提供图片信息，包括图片本身和图片类型
	2.提供图片操作，包括提取像素、投影统计、灰度化、画质增强、二值化、膨胀、插值、变形等
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
	Mat *img;  // 保存图片
	string fileDirAndName;  // 记录图片存储位置和文件名
	string fileType; // 记录文件类型
	int width, height;   // 记录图片真实的宽、高
	Boundary bound;  // 图片的活跃区域，即正在矫正的区域

	void setFileInfo(const char *);
	void setImageInfo();
	void doSetBoundary(const Boundary &);
	void doSave(const string &) const ;
};


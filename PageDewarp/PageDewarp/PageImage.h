#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PageImage
{
	/*
	PageImage类
	封装图片相关的信息和操作，隐藏图片的数据结构细节，为矫正算法提供所需的图片接口，
	若后续改用opencv以外的库，只需要在这个类里作修改即可，矫正算法就无需调整了。
	1.提供图片信息，包括图片本身和图片类型
	2.提供图片操作，包括提取像素、投影统计、灰度化、画质增强、二值化、膨胀、插值、变形等
	*/
private:
	shared_ptr<Mat> img;
	string fileDirAndName;  // 记录图片存储位置和文件名
	string fileType; // 记录文件类型
	int width, height;   // 记录图片真实的宽、高
	Rect bound;  // 图片的活跃区域，即正在矫正的区域


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
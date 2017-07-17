/*
PageImage类
封装图片相关的信息和操作，隐藏图片的数据结构细节，为矫正算法提供所需的图片接口，
若后续改用opencv以外的库，只需要在这个类里作修改即可，矫正算法就无需调整了。
1.提供图片信息，包括图片本身和图片类型
2.提供图片操作，包括提取像素、投影统计、灰度化、画质增强、二值化、膨胀、插值、变形等
*/

#pragma once
#include <vector>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class PageImage {
public:
	PageImage();
	PageImage(const char *);
	~PageImage();

	void setImage(const Mat &);
	void setImage(const char *);
	void setBoundary(int, int, int, int);
	pair<int, int> getSize();
	pair<int, int> getSizeActual();
	void showImage();
	void saveImage();
	void saveImage(const char *);

	const uchar* getRow(const int &);
	const uchar getPixel(const int &, const int &);
	void setPixel(const int &, const int &, const uchar &);
	const uchar* vProject();
	const uchar* hProject();
	void grayScale();
	void enhancement(int(*)(int));
	void binary(const int &);
	void dilate(const Mat *);
	void reshape(pair<int, int>(*)(int, int));

	class Boundary {
	public:
		Boundary() {};
		Boundary(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {};
		~Boundary() {};

		int x1, y1, x2, y2;
	};

private:
	Mat *img;  // 保存图片
	string fileDirAndName;  // 记录图片存储位置和文件名
	string fileType; // 记录文件类型
	int width, height;   // 记录图片真实的宽、高
	Boundary bound;  // 图片的活跃区域，即正在矫正的区域

	void setFileInfo(const char *);
	void setImageInfo();
	void doSave(const string &);
};


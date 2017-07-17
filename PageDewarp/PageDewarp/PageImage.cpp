#include "stdafx.h"
#include "PageImage.h"

PageImage::PageImage() {
	/* 默认初始化
	*/
}

PageImage::PageImage(const char *fileName) {
	/* 用图片文件名初始化PageImage
	*/
	setFileInfo(fileName);
	img = new Mat(imread(fileName)); // 从文件中读入图像
	setImageInfo();
}

PageImage::~PageImage() {
	/* 析构，销毁动态申请的img
	*/
	delete img;
}

void PageImage::setImage(const Mat &m) {
	/* 用一个Mat对象设置PageImage对象保存的图片。
	PageImage对象丢弃现有的数据，保存传入的Mat的副本，
	并初始化类内参数
	*/
	if (img != nullptr) delete img;
	setFileInfo(nullptr);
	img = new Mat(m);
	setImageInfo();
}

void PageImage::setImage(const char *fileName) {
	/* 用一个文件名设置PageImage对象保存的图片。
	PageImage对象丢弃现有的数据，以cv::Mat格式保存传入的图像文件，
	并初始化类内参数
	*/
	if (img != nullptr) delete img;
	setFileInfo(fileName);
	img = new Mat(imread(fileName));
	setImageInfo();
}

void PageImage::setBoundary(int a, int b, int c, int d) {
	/* 设置图像的活动区域
	*/ 
	bound = Boundary(a, b, c, d);
}

pair<int, int> PageImage::getSize() {
	/* 返回图像活动区域的大小
	*/
	return pair<int, int>(bound.x2 - bound.x1 + 1, bound.y2 - bound.y1 + 1);
}

pair<int, int> PageImage::getSizeActual() {
	/* 返回图像的实际大小
	*/
	return pair<int, int>(width, height);
}

void PageImage::showImage() {
	/* 显示图像
	*/
	imshow("image", *img); 
	waitKey(); //此函数等待按键，按键盘任意键就返回
}

void PageImage::saveImage() {
	/* 使用默认文件名和类型保存图像
	*/
	doSave(fileDirAndName + '.' + fileType);
}

void PageImage::saveImage(const char *f) {
	/* 指定文件名和类型保存图像
	*/
	doSave(f);
}

const uchar * PageImage::getRow(const int &r) {
	/* 返回指定行的头指针
	*/
	return img->ptr<uchar>(r);
}

const uchar PageImage::getPixel(const int &r, const int &c) {
	/* 返回指定行列的像素值
	*/
	return img->ptr<uchar>(r)[c];
}

void PageImage::setPixel(const int &r, const int &c, const uchar &value) {
	/* 设置指定行列的像素值
	*/
	img->ptr<uchar>(r)[c] = value;
}

const uchar* PageImage::vProject() {
	/* 将图像在竖直方向上投影，警告：若使用完成后不delete返回的指针将造成内存泄漏
	*/
	int sz = getSize().first;
	uchar *ret = new uchar[sz];

	for (int i = bound.y1; i <= bound.y2; ++i) {
		uchar *p = img->ptr<uchar>(i);
		int j = 0, col = j + bound.x1;
		while (j < sz) {
			ret[j++] += p[col++];
		}
	}

	return ret;
}

const uchar * PageImage::hProject() {
	/* 将图像在水平方向上投影，警告：若使用完成后不delete返回的指针将造成内存泄漏
	*/
	int sz = getSize().second;
	uchar *ret = new uchar[sz];

	int i = 0, row = i + bound.y1;
	while (i < sz) {
		uchar *p = img->ptr<uchar>(row++);
		for (int j = bound.x1; j <= bound.x2; ++j) {
			ret[i] += p[j];
		}
		++i;
	}

	return ret;
}

void PageImage::grayScale() {
	/* 将图像灰度化。使用cvtColor
	*/
	Mat *newImg = new Mat(height, width, CV_8UC1);
	cvtColor(*img, *newImg, CV_RGB2GRAY);
	delete img;
	img = newImg;
}

void PageImage::binary(const int &thre) {
	/* 将图像二值化。使用cvThreshold
	*/
	//////////////////////////////FixMe/////////////////////////////
	cvThreshold(img, img, 300, 255, CV_THRESH_OTSU | CV_THRESH_BINARY_INV);
}

void PageImage::setFileInfo(const char *fileName) {
	/* 私有函数，设置文件名、文件类型信息
	*/
	bool findDot = false;
	// 一次遍历输入的字符串，将‘.’之前的视作文件路径和文件名，之后的视为文件类型
	while (fileName++) {
		if (!findDot) {
			if (*fileName == '.') {
				findDot = true;
			} else {
				fileDirAndName += *fileName;
			}
		} else {
			fileType += *fileName;
		}
	}
	// 对非常规的输入做特殊处理
	if (fileDirAndName.empty()) {
		fileDirAndName = "unnamed";
	}
	if (fileType.empty()) {
		fileType = "unspecified_type";
	}
}

void PageImage::setImageInfo() {
	/* 私有函数，设置图像的实际长、宽和边界
	*/
	if (img->empty()) {
		throw runtime_error("Can not load image: " + fileDirAndName + "." + fileType); // 如果读入图像失败则抛出异常
	}
	width = img->cols;
	height = img->rows;
	bound = Boundary(0, 0, width - 1, height - 1);
}

void PageImage::doSave(const string &fileName) {
	/* 私有函数，保存图像到文件
	*/
	imwrite(fileName, *img);
}

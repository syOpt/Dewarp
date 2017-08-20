#include "stdafx.h"
#include "PageImage.h"

PageImage::PageImage() {
	/* 默认初始化
	*/
	img = new Mat();
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

PageImage &PageImage::setImage(const Mat &m) {
	/* 用一个Mat对象设置PageImage对象保存的图片。
	PageImage对象丢弃现有的数据，保存传入的Mat的副本，
	并初始化类内参数
	*/
	if (img != nullptr) delete img;
	setFileInfo(nullptr);
	img = new Mat(m);
	setImageInfo();
	return *this;
}

PageImage &PageImage::setImage(const char *fileName) {
	/* 用一个文件名设置PageImage对象保存的图片。
	PageImage对象丢弃现有的数据，以cv::Mat格式保存传入的图像文件，
	并初始化类内参数
	*/
	if (img != nullptr) delete img;
	setFileInfo(fileName);
	img = new Mat(imread(fileName));
	setImageInfo();
	return *this;
}

PageImage &PageImage::setBoundary(const Boundary &b) {
	/* 设置图像的活动区域
	*/
	doSetBoundary(b);
	return *this;
}

PageImage &PageImage::setBoundary(int a, int b, int c, int d) {
	/* 设置图像的活动区域
	*/ 
	doSetBoundary(Boundary(a, b, c, d));
	return *this;
}

pair<int, int> PageImage::getSize() const {
	/* 返回图像活动区域的宽、高
	*/
	return pair<int, int>(bound.x2 - bound.x1 + 1, bound.y2 - bound.y1 + 1);
}

pair<int, int> PageImage::getSizeActual() const {
	/* 返回图像的实际大小
	*/
	return pair<int, int>(width, height);
}

void PageImage::showImage() const {
	/* 显示图像
	*/
	cv::namedWindow("image", CV_WINDOW_NORMAL);  ;  
	imshow("image", *img); 
	waitKey(); //此函数等待按键，按键盘任意键就返回
}

void PageImage::saveImage() const {
	/* 使用默认文件名和类型保存图像
	*/
	doSave(fileDirAndName + '.' + fileType);
}

void PageImage::saveImage(const char *f) const {
	/* 指定文件名和类型保存图像
	*/
	doSave(f);
}

const uchar * PageImage::getRow(const int &r) const {
	/* 返回指定行的头指针
	*/
	int rt = r + bound.y1;
	if (rt < 0 || rt > bound.y2) {
		throw range_error("Row index out of range.");
	}
	return img->ptr<uchar>(rt);
}

uchar * PageImage::getColomn(const int &c) const {
	/* 返回指定列。该列为一个临时生成的动态数组，使用后若不delete会产生内存泄漏。
	数组元素是将Mat中的数据拷贝进去得到的
	*/
	int ct = c + bound.x1;
	if (ct < 0 || ct > bound.x2) {
		throw range_error("Colomn index out of range.");
	}
	int sz = getSize().second;
	uchar *ret = new uchar[sz];
	for (int i = bound.y1, j = 0; i <= bound.y2; ++i) {
		ret[j++] = img->ptr<uchar>(i)[ct];
	}
	return ret;
}

const uchar PageImage::getPixel(const int &r, const int &c) const {
	/* 返回指定行列的像素值
	*/
	int rt = r + bound.y1, ct = c + bound.x1;
	if (rt < 0 || rt >(bound.y2) || ct < 0 || ct >(bound.x2)) {
		throw range_error("Pixel index out of range.");
	}
	return img->ptr<uchar>(rt)[ct];
}

PageImage &PageImage::setPixel(const int &r, const int &c, const uchar &value) {
	/* 设置指定行列的像素值
	*/
	int rt = r + bound.y1, ct = c + bound.x1;
	if (rt < 0 || rt >(bound.y2) || ct < 0 || ct >(bound.x2)) {
		throw range_error("Pixel index out of range.");
	}
	img->ptr<uchar>(rt)[ct] = value;
	return *this;
}

const int* PageImage::vCountBlack() const {
	/* 将图像在竖直方向上投影，统计黑色像素点个数。警告：若使用完成后不delete返回的指针将造成内存泄漏。
	若图像未二值化，则结果无意义
	*/
	int sz = getSize().first;
	int *ret = new int[sz];
	for (int i = 0; i < sz; ++i) ret[i] = 0;

	for (int i = bound.y1; i <= bound.y2; ++i) {
		uchar *p = img->ptr<uchar>(i);
		int j = 0, col = j + bound.x1;
		while (j < sz) {
			if (p[col++] == 0) ++ret[j];
			j++;
		}
	}

	return ret;
}

const int * PageImage::hCountBlack() const {
	/* 将图像在水平方向上投影，统计黑色像素点个数。警告：若使用完成后不delete返回的指针将造成内存泄漏。
	*/
	int sz = getSize().second;
	int *ret = new int[sz];

	int i = 0, row = i + bound.y1;
	while (i < sz) {
		ret[i] = 0;
		uchar *p = img->ptr<uchar>(row);
		for (int j = bound.x1; j <= bound.x2; ++j) {
			if (p[j] == 0) ++ret[i];
		}
		++i;
		++row;
	}

	return ret;
}

PageImage &PageImage::grayScale() {
	/* 将全部图像（不止活动区域）灰度化。灰度化后，图像类型为CV_8UC1。使用cvtColor实现。
	*/
	cvtColor(*img, *img, CV_RGB2GRAY);
	return *this;
}

PageImage &PageImage::binary(const int thre) {
	/* 将全部图像（不止活动区域）二值化。使用cvThreshold实现。
	若不指定参数，或指定一个超出范围[0, 255]的阈值，则程序使用OTSU算法进行二值化，
	否则使用指定的阈值二值化。
	*/
	if (thre < 0 || thre > 255) {
		threshold(*img, *img, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	} else {
		threshold(*img, *img, thre, 255, CV_THRESH_BINARY);
	}
	return *this;
}

PageImage & PageImage::bwDilate(const pair<int, int> &kernel, const int iter) const {
	/* 图像膨胀。参数：矩形膨胀核（模版）的尺寸，迭代次数。返回一个新的PageImage对象的引用，该对象内的img成员保存了膨胀的结果。
	用cv::dilate实现
	*/
	Mat elem = getStructuringElement(MORPH_RECT, Size(kernel.first, kernel.second), Point(-1, -1));
	PageImage *ret = new PageImage();
	dilate(*img, *(ret->img), elem, Point(-1, -1), iter);
	ret->setImageInfo();
	ret->setBoundary(bound);
	return *ret;
}

PageImage & PageImage::bwErode(const pair<int, int> &kernel, const int iter) const {
	/* 图像腐蚀。参数：矩形腐蚀核（模版）的尺寸，迭代次数。返回一个新的PageImage对象的引用，该对象内的img成员保存了腐蚀的结果。
	用cv::erode实现
	*/
	Mat elem = getStructuringElement(MORPH_RECT, Size(kernel.first, kernel.second), Point(-1, -1));
	PageImage *ret = new PageImage();
	erode(*img, *(ret->img), elem, Point(-1, -1), iter);
	ret->setImageInfo();
	ret->setBoundary(bound);
	return *ret;
}
PageImage & PageImage::bwRemap(const Mat &map_x,const Mat &map_y) const
{
	PageImage *ret = new PageImage();
	cv::remap(*img,*(ret->img),map_x,map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0));
	ret->setImageInfo();
	ret->setBoundary(bound);
	return *ret;

}

void PageImage::setFileInfo(const char *fileName) {
	/* 私有函数，设置文件名、文件类型信息
	*/
	bool findDot = false;
	// 一次遍历输入的字符串，将‘.’之前的视作文件路径和文件名，之后的视为文件类型
	while (*fileName) {
		if (!findDot) {
			if (*fileName == '.') {
				findDot = true;
			} else {
				fileDirAndName += *fileName;
			}
		} else {
			fileType += *fileName;
		}
		++fileName;
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

void PageImage::doSetBoundary(const Boundary &b) {
	/* 私有函数，执行设置活动区域。若输入参数越界，则抛出异常。
	*/
	if (b.x1 < 0 || b.y1 < 0 || b.x2 >(width - 1) || b.y2 >(height - 1) || b.x2 < b.x1 || b.y2 < b.y1) {
		throw invalid_argument("Invalid boundary argument(s).");
	}
	bound = b;
}

void PageImage::doSave(const string &fileName) const {
	/* 私有函数，保存图像到文件
	*/
	int rows = getSize().second, cols = getSize().first;
	Mat temp(rows, cols, CV_8UC1);
	int i = 0, r = bound.y1;
	while (i < rows) {
		uchar *pOri = img->ptr<uchar>(r++);
		uchar *pNew = temp.ptr<uchar>(i++);
		int j = 0, c = bound.x1;
		while (j < cols) {
			pNew[j++] = pOri[c++];
		}
	}
	imwrite(fileName, temp);
}


Mat* PageImage::getImg() const
{
	return img;
}

#include "StdAfx.h"
#include "PageImage.h"
#include <memory>


PageImage::PageImage(void) {
	/* 默认初始化
	*/
	img = shared_ptr<Mat>(new Mat());
}

PageImage::PageImage(const char *fileName) {
	/* 用图片文件名初始化PageImage
	*/
	img = shared_ptr<Mat>(new Mat(imread(fileName,CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR ))); // 从文件中读入图像
	setImageInfo(fileName);
}


PageImage::~PageImage(void)
{
}

PageImage &PageImage::setImage(const Mat &m) {
	/* 用一个Mat对象设置PageImage对象保存的图片。
	PageImage对象丢弃现有的数据，保存传入的Mat的副本，
	并初始化类内参数
	*/

	img = shared_ptr<Mat>(new Mat(m));
	setImageInfo("NewImage.jpg");
	return *this;
}

PageImage &PageImage::setImage(const char *fileName) {
	/* 用一个文件名设置PageImage对象保存的图片。
	PageImage对象丢弃现有的数据，以cv::Mat格式保存传入的图像文件，
	并初始化类内参数
	*/

	img =  shared_ptr<Mat>(new Mat(imread(fileName,CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR )));
	setImageInfo(fileName);
	return *this;
}

PageImage &PageImage::setBoundary(const Rect &b) {
	/* 设置图像的活动区域
	*/
	doSetBoundary(b);
	return *this;
}

void PageImage::setImageInfo(const string fileName) {
	/* 
	私有函数，设置图像的实际长、宽和边界
	*/
	if (img->empty()) {
		throw runtime_error("Can not load image: " + fileDirAndName + "." + fileType); // 如果读入图像失败则抛出异常
	}

	string fileString(fileName);
	auto stringLenth = fileString.length();
	auto pos = fileString.find(".");
	if(pos == string::npos)
	{
		throw runtime_error(fileString+" is an error fileName");
	}
	fileType.assign(fileString,pos+1,stringLenth);
	fileDirAndName.assign(fileString,0,pos);
	

	width = img->cols;
	height = img->rows;
	bound = Rect(0, 0, width - 1, height - 1);
}

PageImage &PageImage::setBoundary(int a, int b, int c, int d) {
	/* 设置图像的活动区域
	*/ 
	if (a < 0 || b < 0 || c >(width - 1) || d >(height - 1) || c < a || d < b) {
		throw invalid_argument("Invalid boundary argument(s).");
	}
	doSetBoundary(Rect(a, b, c-a, d-b));
	return *this;
}

void PageImage::doSetBoundary(const Rect &b) {
	/* 私有函数，执行设置活动区域。若输入参数越界，则抛出异常。
	*/
	bound = b;
}

pair<int, int> PageImage::getSize() const {
	/* 返回图像活动区域的宽、高
	*/
	return pair<int, int>(bound.width, bound.height);
}

pair<int, int> PageImage::getSizeActual() const {
	/* 返回图像的实际大小
	*/
	return pair<int, int>(width, height);
}

void PageImage::showImage() const {
	/* 显示图像
	*/
	cv::namedWindow(fileDirAndName, CV_WINDOW_NORMAL);  ;  
	imshow(fileDirAndName, *img); 
	waitKey(0); //此函数等待按键，按键盘任意键就返回
}
void PageImage::showImage(const char*f) const {
	/* 显示图像
	*/
	cv::namedWindow(f, CV_WINDOW_NORMAL);  ;  
	imshow(f, *img); 
	waitKey(0); //此函数等待按键，按键盘任意键就返回
}

void PageImage::saveImage() const {
	/* 使用默认文件名和类型保存图像
	*/
	Mat temp;
	(*img)(bound).copyTo(temp);
	imwrite(fileDirAndName + '.' + fileType,temp);
}
void PageImage::saveImage(const string &f) const {
	/* 指定文件名和类型保存图像
	*/
	Mat temp;
	(*img)(bound).copyTo(temp);
	imwrite(f,temp);
}



const int* PageImage::vCountBlack() const {
	/* 将图像在竖直方向上投影，统计黑色像素点个数。警告：若使用完成后不delete返回的指针将造成内存泄漏。
	若图像未二值化，则结果无意义
	*/
	int sz = getSize().first;
	int *ret = new int[sz];
	for (int i = 0; i < sz; ++i) ret[i] = 0;

	for (int i = bound.y; i <= bound.y+bound.height; ++i) {
		uchar *p = img->ptr<uchar>(i);
		int j = 0, col = j + bound.x;
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

	int i = 0, row = i + bound.y;
	while (i < sz) {
		ret[i] = 0;
		uchar *p = img->ptr<uchar>(row);
		for (int j = bound.x; j <= bound.x+bound.width; ++j) {
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
	ret->setImageInfo(fileDirAndName+"_dilate."+fileType);
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
	ret->setImageInfo(fileDirAndName+"erode."+fileType);
	ret->setBoundary(bound);
	return *ret;
}
PageImage & PageImage::bwRemap(const Mat &map_x,const Mat &map_y) const
{
	PageImage *ret = new PageImage();
	cv::remap(*img,*(ret->img),map_x,map_y, INTER_LINEAR, BORDER_CONSTANT, Scalar(0,0, 0));
	ret->setImageInfo(fileDirAndName+"remap."+fileType);
	ret->setBoundary(bound);
	return *ret;

}


const uchar * PageImage::getRow(const int &r) const {
	/* 返回指定行的头指针
	*/
	int rt = r + bound.y;
	if (rt < 0 || rt > bound.y + bound.height) {
		throw range_error("Row index out of range.");
	}
	return img->ptr<uchar>(rt);
}

uchar * PageImage::getColomn(const int &c) const {
	/* 返回指定列。该列为一个临时生成的动态数组，使用后若不delete会产生内存泄漏。
	数组元素是将Mat中的数据拷贝进去得到的
	*/
	int ct = c + bound.x;
	if (ct < 0 || ct > bound.x + bound.width) {
		throw range_error("Colomn index out of range.");
	}
	int sz = getSize().second;
	uchar *ret = new uchar[sz];
	for (int i = bound.y, j = 0; i <= bound.y + bound.height; ++i) {
		ret[j++] = img->ptr<uchar>(i)[ct];
	}
	return ret;
}
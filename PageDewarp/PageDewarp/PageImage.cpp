#include "StdAfx.h"
#include "PageImage.h"


PageImage::PageImage(void) {
	/* Ĭ�ϳ�ʼ��
	*/
	img = shared_ptr<Mat>(new Mat());
}

PageImage::PageImage(const char *fileName) {
	/* ��ͼƬ�ļ�����ʼ��PageImage
	*/
	img = shared_ptr<Mat>(new Mat(imread(fileName,CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR ))); // ���ļ��ж���ͼ��
	setImageInfo(fileName);
}


PageImage::~PageImage(void)
{
}

PageImage &PageImage::setImage(const Mat &m) {
	/* ��һ��Mat��������PageImage���󱣴��ͼƬ��
	PageImage���������е����ݣ����洫���Mat�ĸ�����
	����ʼ�����ڲ���
	*/

	img = shared_ptr<Mat>(new Mat(m));
	setImageInfo("NewImage.jpg");
	return *this;
}

PageImage &PageImage::setImage(const char *fileName) {
	/* ��һ���ļ�������PageImage���󱣴��ͼƬ��
	PageImage���������е����ݣ���cv::Mat��ʽ���洫���ͼ���ļ���
	����ʼ�����ڲ���
	*/

	img =  shared_ptr<Mat>(new Mat(imread(fileName,CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR )));
	setImageInfo(fileName);
	return *this;
}

PageImage &PageImage::setBoundary(const Rect &b) {
	/* ����ͼ��Ļ����
	*/
	doSetBoundary(b);
	return *this;
}

void PageImage::setImageInfo(const string fileName) {
	/* 
	˽�к���������ͼ���ʵ�ʳ�����ͱ߽�
	*/
	if (img->empty()) {
		throw runtime_error("Can not load image: " + fileDirAndName + "." + fileType); // �������ͼ��ʧ�����׳��쳣
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
	/* ����ͼ��Ļ����
	*/ 
	if (a < 0 || b < 0 || c >(width - 1) || d >(height - 1) || c < a || d < b) {
		throw invalid_argument("Invalid boundary argument(s).");
	}
	doSetBoundary(Rect(a, b, c-a, d-b));
	return *this;
}

void PageImage::doSetBoundary(const Rect &b) {
	/* ˽�к�����ִ�����û�������������Խ�磬���׳��쳣��
	*/
	bound = b;
}

pair<int, int> PageImage::getSize() const {
	/* ����ͼ������Ŀ���
	*/
	return pair<int, int>(bound.width, bound.height);
}

pair<int, int> PageImage::getSizeActual() const {
	/* ����ͼ���ʵ�ʴ�С
	*/
	return pair<int, int>(width, height);
}

void PageImage::showImage() const {
	/* ��ʾͼ��
	*/
	cv::namedWindow(fileDirAndName, CV_WINDOW_NORMAL);  ;  
	imshow(fileDirAndName, *img); 
	waitKey(0); //�˺����ȴ�������������������ͷ���
}
void PageImage::showImage(const char*f) const {
	/* ��ʾͼ��
	*/
	cv::namedWindow(f, CV_WINDOW_NORMAL);  ;  
	imshow(f, *img); 
	waitKey(0); //�˺����ȴ�������������������ͷ���
}

void PageImage::saveImage() const {
	/* ʹ��Ĭ���ļ��������ͱ���ͼ��
	*/
	Mat temp;
	(*img)(bound).copyTo(temp);
	imwrite(fileDirAndName + '.' + fileType,temp);
}
void PageImage::saveImage(const string &f) const {
	/* ָ���ļ��������ͱ���ͼ��
	*/
	Mat temp;
	(*img)(bound).copyTo(temp);
	imwrite(f,temp);
}



const int* PageImage::vCountBlack() const {
	/* ��ͼ������ֱ������ͶӰ��ͳ�ƺ�ɫ���ص���������棺��ʹ����ɺ�delete���ص�ָ�뽫����ڴ�й©��
	��ͼ��δ��ֵ��������������
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
	/* ��ͼ����ˮƽ������ͶӰ��ͳ�ƺ�ɫ���ص���������棺��ʹ����ɺ�delete���ص�ָ�뽫����ڴ�й©��
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
	/* ��ȫ��ͼ�񣨲�ֹ����򣩻ҶȻ����ҶȻ���ͼ������ΪCV_8UC1��ʹ��cvtColorʵ�֡�
	*/
	cvtColor(*img, *img, CV_RGB2GRAY);
	return *this;
}

PageImage &PageImage::binary(const int thre) {
	/* ��ȫ��ͼ�񣨲�ֹ����򣩶�ֵ����ʹ��cvThresholdʵ�֡�
	����ָ����������ָ��һ��������Χ[0, 255]����ֵ�������ʹ��OTSU�㷨���ж�ֵ����
	����ʹ��ָ������ֵ��ֵ����
	*/
	if (thre < 0 || thre > 255) {
		threshold(*img, *img, 100, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	} else {
		threshold(*img, *img, thre, 255, CV_THRESH_BINARY);
	}
	return *this;
}

PageImage & PageImage::bwDilate(const pair<int, int> &kernel, const int iter) const {
	/* ͼ�����͡��������������ͺˣ�ģ�棩�ĳߴ磬��������������һ���µ�PageImage��������ã��ö����ڵ�img��Ա���������͵Ľ����
	��cv::dilateʵ��
	*/
	Mat elem = getStructuringElement(MORPH_RECT, Size(kernel.first, kernel.second), Point(-1, -1));
	PageImage *ret = new PageImage();
	dilate(*img, *(ret->img), elem, Point(-1, -1), iter);
	ret->setImageInfo(fileDirAndName+"_dilate."+fileType);
	ret->setBoundary(bound);
	return *ret;
}

PageImage & PageImage::bwErode(const pair<int, int> &kernel, const int iter) const {
	/* ͼ��ʴ�����������θ�ʴ�ˣ�ģ�棩�ĳߴ磬��������������һ���µ�PageImage��������ã��ö����ڵ�img��Ա�����˸�ʴ�Ľ����
	��cv::erodeʵ��
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
	/* ����ָ���е�ͷָ��
	*/
	int rt = r + bound.y;
	if (rt < 0 || rt > bound.y + bound.height) {
		throw range_error("Row index out of range.");
	}
	return img->ptr<uchar>(rt);
}

uchar * PageImage::getColomn(const int &c) const {
	/* ����ָ���С�����Ϊһ����ʱ���ɵĶ�̬���飬ʹ�ú�����delete������ڴ�й©��
	����Ԫ���ǽ�Mat�е����ݿ�����ȥ�õ���
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
#include "stdafx.h"
#include "PageImage.h"

PageImage::PageImage() {
	/* Ĭ�ϳ�ʼ��
	*/
	img = new Mat();
}

PageImage::PageImage(const char *fileName) {
	/* ��ͼƬ�ļ�����ʼ��PageImage
	*/
	setFileInfo(fileName);
	img = new Mat(imread(fileName)); // ���ļ��ж���ͼ��
	setImageInfo();
}

PageImage::~PageImage() {
	/* ���������ٶ�̬�����img
	*/
	delete img;
}

PageImage &PageImage::setImage(const Mat &m) {
	/* ��һ��Mat��������PageImage���󱣴��ͼƬ��
	PageImage���������е����ݣ����洫���Mat�ĸ�����
	����ʼ�����ڲ���
	*/
	if (img != nullptr) delete img;
	setFileInfo(nullptr);
	img = new Mat(m);
	setImageInfo();
	return *this;
}

PageImage &PageImage::setImage(const char *fileName) {
	/* ��һ���ļ�������PageImage���󱣴��ͼƬ��
	PageImage���������е����ݣ���cv::Mat��ʽ���洫���ͼ���ļ���
	����ʼ�����ڲ���
	*/
	if (img != nullptr) delete img;
	setFileInfo(fileName);
	img = new Mat(imread(fileName));
	setImageInfo();
	return *this;
}

PageImage &PageImage::setBoundary(const Boundary &b) {
	/* ����ͼ��Ļ����
	*/
	doSetBoundary(b);
	return *this;
}

PageImage &PageImage::setBoundary(int a, int b, int c, int d) {
	/* ����ͼ��Ļ����
	*/ 
	doSetBoundary(Boundary(a, b, c, d));
	return *this;
}

pair<int, int> PageImage::getSize() const {
	/* ����ͼ������Ŀ���
	*/
	return pair<int, int>(bound.x2 - bound.x1 + 1, bound.y2 - bound.y1 + 1);
}

pair<int, int> PageImage::getSizeActual() const {
	/* ����ͼ���ʵ�ʴ�С
	*/
	return pair<int, int>(width, height);
}

void PageImage::showImage() const {
	/* ��ʾͼ��
	*/
	cv::namedWindow("image", CV_WINDOW_NORMAL);  ;  
	imshow("image", *img); 
	waitKey(); //�˺����ȴ�������������������ͷ���
}

void PageImage::saveImage() const {
	/* ʹ��Ĭ���ļ��������ͱ���ͼ��
	*/
	doSave(fileDirAndName + '.' + fileType);
}

void PageImage::saveImage(const char *f) const {
	/* ָ���ļ��������ͱ���ͼ��
	*/
	doSave(f);
}

const uchar * PageImage::getRow(const int &r) const {
	/* ����ָ���е�ͷָ��
	*/
	int rt = r + bound.y1;
	if (rt < 0 || rt > bound.y2) {
		throw range_error("Row index out of range.");
	}
	return img->ptr<uchar>(rt);
}

uchar * PageImage::getColomn(const int &c) const {
	/* ����ָ���С�����Ϊһ����ʱ���ɵĶ�̬���飬ʹ�ú�����delete������ڴ�й©��
	����Ԫ���ǽ�Mat�е����ݿ�����ȥ�õ���
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
	/* ����ָ�����е�����ֵ
	*/
	int rt = r + bound.y1, ct = c + bound.x1;
	if (rt < 0 || rt >(bound.y2) || ct < 0 || ct >(bound.x2)) {
		throw range_error("Pixel index out of range.");
	}
	return img->ptr<uchar>(rt)[ct];
}

PageImage &PageImage::setPixel(const int &r, const int &c, const uchar &value) {
	/* ����ָ�����е�����ֵ
	*/
	int rt = r + bound.y1, ct = c + bound.x1;
	if (rt < 0 || rt >(bound.y2) || ct < 0 || ct >(bound.x2)) {
		throw range_error("Pixel index out of range.");
	}
	img->ptr<uchar>(rt)[ct] = value;
	return *this;
}

const int* PageImage::vCountBlack() const {
	/* ��ͼ������ֱ������ͶӰ��ͳ�ƺ�ɫ���ص���������棺��ʹ����ɺ�delete���ص�ָ�뽫����ڴ�й©��
	��ͼ��δ��ֵ��������������
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
	/* ��ͼ����ˮƽ������ͶӰ��ͳ�ƺ�ɫ���ص���������棺��ʹ����ɺ�delete���ص�ָ�뽫����ڴ�й©��
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
	ret->setImageInfo();
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
	/* ˽�к����������ļ������ļ�������Ϣ
	*/
	bool findDot = false;
	// һ�α���������ַ���������.��֮ǰ�������ļ�·�����ļ�����֮�����Ϊ�ļ�����
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
	// �Էǳ�������������⴦��
	if (fileDirAndName.empty()) {
		fileDirAndName = "unnamed";
	}
	if (fileType.empty()) {
		fileType = "unspecified_type";
	}
}

void PageImage::setImageInfo() {
	/* ˽�к���������ͼ���ʵ�ʳ�����ͱ߽�
	*/
	if (img->empty()) {
		throw runtime_error("Can not load image: " + fileDirAndName + "." + fileType); // �������ͼ��ʧ�����׳��쳣
	}
	width = img->cols;
	height = img->rows;
	bound = Boundary(0, 0, width - 1, height - 1);
}

void PageImage::doSetBoundary(const Boundary &b) {
	/* ˽�к�����ִ�����û�������������Խ�磬���׳��쳣��
	*/
	if (b.x1 < 0 || b.y1 < 0 || b.x2 >(width - 1) || b.y2 >(height - 1) || b.x2 < b.x1 || b.y2 < b.y1) {
		throw invalid_argument("Invalid boundary argument(s).");
	}
	bound = b;
}

void PageImage::doSave(const string &fileName) const {
	/* ˽�к���������ͼ���ļ�
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

#include "stdafx.h"
#include "Dewarpper.h"

enum ERROR_TYPE {SUCCESS, LAYOUTREC_FAILURE};

Dewarpper::Dewarpper() {
	/* 默认初始化
	*/
}

Dewarpper::Dewarpper(const char *fileName) : img(fileName) {
	/* 使用图片的文件名初始化
	*/
}

Dewarpper::~Dewarpper() {
	/* 析构
	*/
	if (bounds) delete bounds;
}

int Dewarpper::dewarp() {
	/* 执行图像矫正
	*/
	/* 目标是将图像先划分成若干文字块，针对每个文字块进行矫正，这样可以避免图表、分栏和标题行
	字号不同等问题的干扰。但为尽快实现一套基本能用的算法，先按文献中的方法操作，即假设书页是
	纯文本的，直接灰度、二值化->计算行高->提取文本线->图	像重构。（对靠近书脊边缘的文字，可能
	还需要进一步插值、矫正等。不过暂时也先不考虑这个问题。）
	bounds = LayoutRecognization();
	if (!bounds || (bounds->empty())) {
		return LAYOUTREC_FAILURE;
	}
	for (auto i : *bounds) {
		img.setBoundary(i);
		preProcedure();
		calcLineHeight();
		getTextLine();
		reshape();
	}
	*/
	preProcedure();
	calcLineHeight();
	getTextLine();
	reshape();
	return 0;
}

void Dewarpper::save(const char *fileName) {
	/* 使用提供的文件名保存当前图像
	*/
	img.saveImage(fileName);
}

vector<PageImage::Boundary>* Dewarpper::LayoutRecognization() {
	/* 版面识别
	*/
	vector<PageImage::Boundary> *ret = new vector<PageImage::Boundary>;
	//////////////////////////////FixMe/////////////////////////////
	return ret;
}

int Dewarpper::preProcedure() {
	/* 预处理。在这一步进行灰度、二值化。如dewarp()函数的注释所述，由于假设图像是
	纯文本的，因此无版面识别操作，需要在这预处理部分裁剪掉图像的白边
	*/
	// 图像灰度化、二值化
	img.grayScale().binary();
	// 裁剪，去除白边
	auto vprj = img.vCountBlack(), hprj = img.hCountBlack();  // 将图像向行、列方向投影，统计黑色像素点个数
	int wid = img.getSize().first, ht = img.getSize().second;  // 获得图像宽、高
	int sumblk = 0, vThre, hThre;  // sumblk记录全图黑色像素点个数，vThre、hThre分别是竖直和水平方向裁剪的阈值
	for (int i = 0; i < wid; ++i) {
		sumblk += vprj[i];
	}
	double t = 0.3;  // 0.3是经验值。由于书页不可避免的弯曲、倾斜，用该值调整阈值可以避免将有效但处于边缘位置的文字切除
	vThre = (t * sumblk) / wid;
	hThre = (t * sumblk) / ht;
	int x1 = 0, y1 = 0, x2 = wid - 1, y2 = ht - 1; 
	while (vprj[x1] < vThre) ++x1;  // 按算得的阈值寻找裁剪的位置
	while (vprj[x2] < vThre) --x2;
	while (hprj[y1] < hThre) ++y1;
	while (hprj[y2] < hThre) --y2;
	img.setBoundary(x1, y1, x2, y2);  // 设置图像的活动区域，从而起“裁剪”的效果
	delete []vprj;
	delete []hprj;
	return 0;
}

int Dewarpper::calcLineHeight() {
	return 0;
}

int Dewarpper::getTextLine() {
	return 0;
}

int Dewarpper::reshape() {
	return 0;
}


#include "stdafx.h"
#include "PageImage.h"

PageImage::PageImage() {
	/* 默认初始化
	*/
}

PageImage::PageImage(const char *fileName) {
	/* 用图片文件名初始化PageImage
	*/
	img = new Mat(imread(fileName)); // 从文件中读入图像
	if (img->empty()) {
		throw runtime_error(strcat("Can not load image: ", fileName)); // 如果读入图像失败则抛出异常
	}	
	width = img->cols;
	height = img->rows;
	bound = Boundary(0, 0, width, height);
}

PageImage::~PageImage() {
	/* 析构，销毁动态申请的img
	*/
	delete img;
}

void PageImage::showImage() {
	/* 显示图像
	*/
	imshow("image", *img); 
	waitKey(); //此函数等待按键，按键盘任意键就返回
}

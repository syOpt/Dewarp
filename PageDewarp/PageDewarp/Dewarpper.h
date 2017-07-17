/*
Dewarpper类
矫正算法的实现。整个流程的本质是对像素的分析、调整。利用PageImage提供的接口，可以不考虑图片的数据结构，便于未来版本更迭。
流程分如下几步：1.版面分析。得到一系列文字块。2.预处理。灰度化、二值化。3.矫正。包括计算行高、拟合文本线、图像重构，是算法核心。
以上流程都用私有函数、私有变量实现。外部接口仅有setImage和dewarp两个，用户输入图片，调用dewarp得到矫正结果，非常简单。
*/

#pragma once
#include "PageImage.h"

class Dewarpper {
public:
	Dewarpper();
	Dewarpper(const char *);
	~Dewarpper();

	int dewarp();  // 矫正书页
	int setImage(const char *);  // 载入书页图像
	void show();  // 显示图像
	int save(const char *);  // 保存图像

private:
	PageImage img;  // 图片
	vector<PageImage::Boundary> *bounds;  // 保存版面分析的结果
	vector<PageImage::Boundary> *LayoutRecognization();  // 版面分析算法
	void setArea(PageImage::Boundary);  // 设置矫正区域
	int preProcedure();  // 预处理算法
	int reconstruct();  // 图像重构算法
	int calcLineHeight();  // 计算行高
	int getTextLine();  // 文本线拟合算法
	int reshape();  // 重新构图算法
};


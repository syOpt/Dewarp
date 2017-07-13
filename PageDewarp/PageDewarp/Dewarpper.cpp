#include "stdafx.h"
#include "Dewarpper.h"

Dewarpper::Dewarpper() {
	/* 默认初始化
	*/
}

Dewarpper::Dewarpper(const char *fileName) : img(fileName) {
}

Dewarpper::~Dewarpper() {
	/* 析构
	*/
}

void Dewarpper::show() {
	img.showImage();
}

#pragma once
#include <stdio.h>
#include <windows.h>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class Tester {
public:
	Tester();
	~Tester();
	void TestDewarp();
	char* fileName;  //图像输入路径
	
private:
	string WChar2Ansi(LPCWSTR pwszSrc);  // LPCWSTR转string
	void readImgNamefromFile(vector <string> &imgNames);  // 利用winWIN32_FIND_DATA读取文件下的文件名

};


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
	char* fileName;  //ͼ������·��
	
private:
	string WChar2Ansi(LPCWSTR pwszSrc);  // LPCWSTRתstring
	void readImgNamefromFile(vector <string> &imgNames);  // ����winWIN32_FIND_DATA��ȡ�ļ��µ��ļ���

};


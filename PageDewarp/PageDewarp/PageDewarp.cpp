// PageDewarp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Dewarpper.h"


using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	char *fileIn = "testIn/ZWB_4464.JPG";
	char *fileOut = "testOut/CHS001.tif";
	Dewarpper dw(fileIn);
	int ret = dw.dewarp();
	if (ret) {
		cout << "Dewarp failed. Error code: " << ret << endl;
	} else {
		dw.save(fileOut);
	}
	return 0;
}

// PageDewarp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Dewarpper.h"


using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	char *fileName = "testPics/CHS001.tif";
	Dewarpper dw(fileName);
	dw.show();
	return 0;
}


// PageDewarp.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Dewarpper.h"
#include "Tester.h"


using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{
	Tester TestPic;
	TestPic.fileName = "testIn\\";
	TestPic.TestDewarp();
	system("pause");
	return 0;
}

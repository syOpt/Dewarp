#include "stdafx.h"
#include "Tester.h"
#include "Dewarpper.h"


Tester::Tester() {
	fileName = NULL;
}


Tester::~Tester() {
}

string Tester::WChar2Ansi(LPCWSTR pwszSrc) {
	int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);

	if (nLen <= 0) return std::string("");

    char* pszDst = new char[nLen];
	if (NULL == pszDst) return std::string("");

	WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
	pszDst[nLen - 1] = 0;

	string strTemp(pszDst);
	delete[] pszDst;
	
	return strTemp;
}

// 利用winWIN32_FIND_DATA读取文件下的文件名
void Tester::readImgNamefromFile(vector <string> &imgNames)
{
	// vector清零 参数设置
	imgNames.clear();
	WIN32_FIND_DATA file;
	int i = 0;
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[50];
	// 转换输入文件名
	sprintf_s(tempFilePath, "%s/*", fileName);
	// 多字节转换
	WCHAR   wstr[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// 查找该文件待操作文件的相关属性读取到WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindNextFile(handle, &file);
		FindNextFile(handle, &file);
		// 循环遍历得到文件夹的所有文件名    
		do
		{
			sprintf_s(tempFileName, "%s", fileName);
			imgNames.push_back(WChar2Ansi(file.cFileName));
			imgNames[i].insert(0, tempFileName);
			i++;
		} while (FindNextFile(handle, &file));
	}
	FindClose(handle);
}

void Tester::TestDewarp() {
	vector <string>  imgNames;
	// 获取对应文件夹下所有文件名
	readImgNamefromFile(imgNames);
	// 遍历对应文件夹下所有文件名
	int count = 1;
	for (int i = 0; i < imgNames.size(); i++)
	{
		Mat img = imread(imgNames[i]);
		if (img.data){
			cout << "----------正在矫正第" << count << "张图片:" << endl;
			count++;
			PageImage ImgTemp;
			ImgTemp.setImage(img);
			Dewarpper dw(ImgTemp);
			dw.picName = imgNames[i];
			dw.picName.erase(dw.picName.begin(), dw.picName.begin() + 7);  //删除图片名称前的路径名称，数字7为根据具体路径字符数设置
			int ret = dw.dewarp();
			if (ret)  cout << "Dewarp failed. Error code: " << ret << endl;
		}
	}
}
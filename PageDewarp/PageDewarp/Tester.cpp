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

// ����winWIN32_FIND_DATA��ȡ�ļ��µ��ļ���
void Tester::readImgNamefromFile(vector <string> &imgNames)
{
	// vector���� ��������
	imgNames.clear();
	WIN32_FIND_DATA file;
	int i = 0;
	char tempFilePath[MAX_PATH + 1];
	char tempFileName[50];
	// ת�������ļ���
	sprintf_s(tempFilePath, "%s/*", fileName);
	// ���ֽ�ת��
	WCHAR   wstr[MAX_PATH] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, tempFilePath, -1, wstr, sizeof(wstr));
	// ���Ҹ��ļ��������ļ���������Զ�ȡ��WIN32_FIND_DATA
	HANDLE handle = FindFirstFile(wstr, &file);
	if (handle != INVALID_HANDLE_VALUE)
	{
		FindNextFile(handle, &file);
		FindNextFile(handle, &file);
		// ѭ�������õ��ļ��е������ļ���    
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
	// ��ȡ��Ӧ�ļ����������ļ���
	readImgNamefromFile(imgNames);
	// ������Ӧ�ļ����������ļ���
	int count = 1;
	for (int i = 0; i < imgNames.size(); i++)
	{
		Mat img = imread(imgNames[i]);
		if (img.data){
			cout << "----------���ڽ�����" << count << "��ͼƬ:" << endl;
			count++;
			PageImage ImgTemp;
			ImgTemp.setImage(img);
			Dewarpper dw(ImgTemp);
			dw.picName = imgNames[i];
			dw.picName.erase(dw.picName.begin(), dw.picName.begin() + 7);  //ɾ��ͼƬ����ǰ��·�����ƣ�����7Ϊ���ݾ���·���ַ�������
			int ret = dw.dewarp();
			if (ret)  cout << "Dewarp failed. Error code: " << ret << endl;
		}
	}
}
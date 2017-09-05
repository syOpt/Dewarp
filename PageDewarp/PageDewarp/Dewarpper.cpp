#include "stdafx.h"
#include "Dewarpper.h"
#include "Fitting.h"
#include <numeric>
#include "math.h"

#define DEWARPPER_DEBUG true
#define DEBUG if( DEWARPPER_DEBUG)

namespace
{
	const int MAXLINE = 50;      //ÿҳ�������
	const int LINE_DETECT_BUFFER = 1;
};

Dewarpper::Dewarpper() {
	/* Ĭ�ϳ�ʼ��
	*/
}

Dewarpper::Dewarpper(const char *fileName) : img(fileName) {
	/* ʹ��ͼƬ���ļ�����ʼ��
	*/
}

Dewarpper::~Dewarpper() {
	/* ����
	*/
}

int Dewarpper::dewarp() {
	/* ִ��ͼ�����
	*/
	/* Ŀ���ǽ�ͼ���Ȼ��ֳ��������ֿ飬���ÿ�����ֿ���н������������Ա���ͼ�������ͱ�����
	�ֺŲ�ͬ������ĸ��š���Ϊ����ʵ��һ�׻������õ��㷨���Ȱ������еķ�����������������ҳ��
	���ı��ģ�ֱ�ӻҶȡ���ֵ��->�����и�->��ȡ�ı���->ͼ	���ع������Կ����鼹��Ե�����֣�����
	����Ҫ��һ����ֵ�������ȡ�������ʱҲ�Ȳ�����������⡣��
	bounds = LayoutRecognization();
	if (!bounds || (bounds->empty())) {
		return LAYOUTREC_FAILURE;
	}
	for (auto i : *bounds) {
		img.setBoundary(i);
		preProcedure();
		calcLineHeight();
		getTextLine();
		reshape();
	}
	*/
	DEBUG cout<<" BEGIN DEWARP"<<endl;
	preProcedure();
	int lineHt = calcLineHeight();
	getTextLine(lineHt);
	reshape(lineHt);
	return 0;
}

int Dewarpper::preProcedure() {
	/* Ԥ��������һ�����лҶȡ���ֵ������dewarp()������ע�����������ڼ���ͼ����
	���ı��ģ�����ް���ʶ���������Ҫ����Ԥ�����ֲü���ͼ��İױ�
	*/
	// ͼ��ҶȻ�����ֵ��
	img.grayScale().binary();
	// �ü���ȥ���ױ�
	auto vprj = img.vCountBlack(), hprj = img.hCountBlack();  // ��ͼ�����С��з���ͶӰ��ͳ�ƺ�ɫ���ص����
	int wid = img.getSize().first, ht = img.getSize().second;  // ���ͼ�����
	int sumblk = 0, vThre, hThre;  // sumblk��¼ȫͼ��ɫ���ص������vThre��hThre�ֱ�����ֱ��ˮƽ����ü�����ֵ
	for (int i = 0; i < wid; ++i) {
		sumblk += vprj[i];
	}
	double t = 0.3;  // 0.3�Ǿ���ֵ��������ҳ���ɱ������������б���ø�ֵ������ֵ���Ա��⽫��Ч�����ڱ�Եλ�õ������г�
	vThre = (t * sumblk) / wid;
	hThre = (t * sumblk) / ht;
	int x1 = 0, y1 = 0, x2 = wid - 1, y2 = ht - 1; 
	while (vprj[x1] < vThre) ++x1;  // ����õ���ֵѰ�Ҳü���λ��
	while (vprj[x2] < vThre) --x2;
	while (hprj[y1] < hThre) ++y1;
	while (hprj[y2] < hThre) --y2;
	img.setBoundary(x1, y1, x2, y2);  // ����ͼ��Ļ���򣬴Ӷ��𡰲ü�����Ч��
	delete []vprj;
	delete []hprj;
	DEBUG cout<<" FINISH PREPROCEDURE"<<endl;
	return 0;
}



int Dewarpper::calcLineHeight() {
	/* �����иߡ��������ֶ��������ģ�����Զ�ȷ���������ѵ㡣
	*/
	double avgLineHeight = 0.0;
	int elemWidth = 0, elemHeight = 0;  // �ṹԪ�ؿ���
	pair<int, int> elem;
	// �����㣬ȥ��������
	elemWidth = 4; elemHeight = 4;
	setElem(elem, elemWidth, elemHeight);
	DEBUG cout<<"  --Into calcLineHeight"<<endl;
	//PageImage ret = img.bwDilate(elem).bwErode(elem);
	ret = img.bwDilate(elem).bwErode(elem);
	DEBUG cout<<"  --Dilate and Erode OK"<<endl;
	doSave(&ret, "testOut/CHS001Open.tif");
	// ��ʴһ�Σ�����һ���иߣ����������и߱仯��С�ڽṹԪ�ظ߶Ȼ�ﵽ���õĴ�������ʱ��ֹ
	elemWidth = 20, elemHeight = 2;
	setElem(elem, elemWidth, elemHeight);
	long double sy = 0, maxIter = 8;
	bool nextIter = true;
	while (nextIter && (sy++ < maxIter)) {
		// ��ʴ
		ret = ret.bwErode(elem);
		doSave(&ret, string("testOut/CHS001Erode" + to_string(sy) + ".tif").c_str());
		// ѡȡҳ���1/3��1/2��2/3���м�����и�
		double newAvgLineHeight = 0.0;
		pair<int, int> sz = ret.getSize();
		int wid = sz.first, ht = sz.second;
		const int sampleNum = 3;
		double rfPos[sampleNum] = { 0.33, 0.5, 0.67 };
		list<int> lineHeights;  // ��¼���и�
		for (int i = 0; i < sampleNum; ++i) {
			int pos = rfPos[i] * wid;
			uchar *col = ret.getColomn(pos);
			int count = 0;  // �и�ֵ������
			for (int j = 0; j < ht; ++j) {
				if (col[j] == 0) {
					++count;  // �����ڵ㣬�ۼӼ�����
				} else {  // �����׵�
					if (count) {  // ������������ֵ������¸�ֵ��һ���иߣ������������
						lineHeights.push_back(count);
						count = 0;
					}
				}
			}
		}
		// �ɲ����õ��ĸ��и߼����ֵ
		if (!lineHeights.empty()) {
			// ���룺������иߵľ�ֵ��Ȼ��ȥ�����и���С�ھ�ֵһ���ֵ�ʹ��ھ�ֵһ����ֵ
			newAvgLineHeight = 1.0 * std::accumulate(lineHeights.cbegin(), lineHeights.cend(), 0) / lineHeights.size();
			auto i = lineHeights.begin();
			while (i != lineHeights.end()) {
				if (*i < newAvgLineHeight / 2 || *i > newAvgLineHeight * 2) {
					i = lineHeights.erase(i);
				} else {
					++i;
				}
			}
		}
		if (!lineHeights.empty()) {
			// ���¼���������ƽ���и�
			newAvgLineHeight = 1.0 * std::accumulate(lineHeights.cbegin(), lineHeights.cend(), 0) / lineHeights.size();
			if (abs(newAvgLineHeight - avgLineHeight) < elemHeight) {
				nextIter = false;  // �����ε����������һ��֮��С�ڽṹԪ�صĸߣ�����ֹ����
			}
			avgLineHeight = newAvgLineHeight;
			cout << sy << ": " << avgLineHeight << "\n";
		} else {  // ��δ������Ч�иߣ����ܷ����˴���
			cerr << "Error may have occurred during calculation of the height of lines.";
		}		
	}
	
	lineHeight = avgLineHeight;
	DEBUG cout<<" FINISH CALLINEHEIGHT"<<endl;
	return avgLineHeight;
}

int Dewarpper::getTextLine(const int &lineHt) {
	//�����������ͼ������û�м��й���������ȥ�ߵĹ���û��ʹ���ϣ�ͼ���õ���ret�������ӵ����Ա��ԭ������Ҫ֮ǰ���и�ģ��������ͼƬ��
	int avgLineHeight = lineHt;
	pair<int, int> sz = ret.getSize();
	int wid = sz.first, ht = sz.second;
	const int startpoint = 0.5*wid;   //ȷ��ģ���������λ��
	uchar *middleColomn = ret.getColomn(startpoint);  //�õ�ģ��������ʼ�еĲ���
	int  i = 0, CountLine = 0;
	Caltextline textline[MAXLINE];   
	bool LineJudge = true;
	list<double> temp;
	while (LineJudge) {
		if (middleColomn[i] == 0) {    //��⵽�µ�һ��
			i = i;
			list<int> pointX;    //������¼ÿ�����������
			list<int> pointY;
			int pointYstart = 0, pointYstart2=0, pointYHeight = 1.4*avgLineHeight;
			int pointXright = startpoint, pointXleft = startpoint;  //�������ҽ���ģ�����������
			for (int j = i + pointYHeight; j > i; j--) {
				if (middleColomn[j] == 0) { 
					pointYstart2 = j;
					break; 
				}
			}
			pointYstart = (pointYstart2 + i) / 2;   //�õ�ģ��������ʼ�е�Y����
			int tempHeight = pointYstart2 - i;
			pointX.push_back(startpoint);
			pointY.push_back(pointYstart);

			while (true) {  //�������ģ������
				pointXleft -= avgLineHeight;
				if (pointXleft <= 0) break;  //��ֹԽ��
				int pointY1 = 0, pointY2 = 0;
				uchar *leftcol = ret.getColomn(pointXleft);
				int draftuppoint = pointYstart - 0.7*avgLineHeight, draftdownpoint = pointYstart + 0.7*avgLineHeight;
				int uppoint = max(0, draftuppoint), downpoint = min(ht, draftdownpoint); //ȷ��ÿ��ģ�������Ͻ����½죬�Ҳ�Խ���߽�
																						 //�ֱ�Ϊ���Ͻ翪ʼ���¼�������½翪ʼ���ϼ������ҵ���ɫ���ؼ�ֹͣ
				for (int Y1 = uppoint; Y1 < downpoint; Y1++) {
					if (leftcol[Y1] == 0) {
						pointY1 = Y1;
						break;
					}
				}
				for (int Y2 = downpoint; Y2 > uppoint; Y2--) {
					if (leftcol[Y2] == 0) {
						pointY2 = Y2;
						break;
					}
				}
				pointYstart = (pointY1 + pointY2) / 2;
				pointX.push_back(pointXleft);
				pointY.push_back(pointYstart);
			
				//�ж�ģ��������ֹ����������߽����һ��ģ���ⲻ����ɫ���أ�
				bool judge = false;
				if ((pointXleft - avgLineHeight) > 0) {
					uchar *nextcol = ret.getColomn(pointXleft - avgLineHeight);
					for (int j = uppoint; j < downpoint; j++) {
						if (nextcol[j] == 0) {
							judge = true;
							break;
						}
					}
				} else break;
				if (!judge) break;
			}
		
			while (true) {  //���ҽ���ģ������
				pointXright += avgLineHeight;
				if (pointXright >= wid) break; //��ֹԽ��
				int pointY1 = 0, pointY2 = 0;
				uchar *rightcol = ret.getColomn(pointXright);
				int draftuppoint = pointYstart - 0.7*avgLineHeight, draftdownpoint = pointYstart + 0.7*avgLineHeight;
				int uppoint=max(0,draftuppoint), downpoint=min(ht,draftdownpoint); //ȷ��ÿ��ģ�������Ͻ����½죬�Ҳ�Խ��ͼƬ��Χ
				//�ֱ�Ϊ���Ͻ翪ʼ���¼�������½翪ʼ���ϼ������ҵ���ɫ���ؼ�ֹͣ
				for (int Y1 = uppoint; Y1 < downpoint; Y1++) {
					if (rightcol[Y1] == 0) {			
						pointY1 = Y1;
						break;
					}
				}
				for (int Y2 = downpoint; Y2 > uppoint; Y2--) {
					if (rightcol[Y2] == 0) {
						pointY2 = Y2;
						break;
					}
				}
				pointYstart = (pointY1 + pointY2) / 2;
				pointX.push_back(pointXright);
				pointY.push_back(pointYstart);

				//�ж�ģ��������ֹ����������߽����һ��ģ���ⲻ����ɫ���أ�
				bool judge = false;
				if ((pointXright + avgLineHeight) < wid) {
					uchar *nextcol = ret.getColomn(pointXright + avgLineHeight);
					for (int j = uppoint; j < downpoint; j++) {
						if (nextcol[j] == 0) {
							judge = true;
							break;
						}
					}
				} else break;
				if (!judge) break;
			}		
		
			//����ĳһ�м������ĵ�Ĵ���
			/*
			if (CountLine == 0) {
				cout << endl;
				auto l = pointX.begin();
				while (l != pointX.end()) {
					cout << *l << '\t';
					l++;
				}
				cout << endl;
				auto k = pointY.begin();
				while (k != pointY.end()) {
					cout << *k << '\t';
					k++;
				}
				cout << endl;
			}	
			*/
			textline[CountLine].Fitting(pointX, pointY); //�������
			temp.clear();
			DEBUG cout << "��" << CountLine+1 << "�У�" << '\t';
			for (int k = 0; k < FittingN; k++) 
				{
					DEBUG cout << textline[CountLine].Coefficient[k] << '\t';
					temp.push_front(textline[CountLine].Coefficient[k]);
			}
			cout << endl;
			CountLine++;   //������һ
			coefficientM.push_front(temp);
			i += pointYHeight; //����Ŀǰ�еĺڵ���������������
		}
		else if (i < ht ) i++;
	    else LineJudge = false; //���������ֹ����
	}
	DEBUG cout<<" FINISH CALLINES"<<endl;
	return 0;
}

int Dewarpper::reshape(const int &lineHt) {
	//����֮ǰ�Ĺ�����û������Ȥ������������������ʱд�ľ�Ĭ��ͼ���Ǽ��кõ���Ȥ����������
	pair<int, int> size = ret.getSizeActual();
	//Mat source = *(img.getImg());
	Mat map_x,map_y;
	int cols = size.first;
	int rows = size.second;
	map_x = Mat::zeros(rows,cols,CV_32FC1);
	map_y = Mat::zeros(rows,cols,CV_32FC1);

	double aCoefficient,bCoefficient,cCoefficient,dCoefficient;
	while(!coefficientM.empty())
	{
	 auto mapCoefficient = coefficientM.front();
	 list<double>::iterator iter = mapCoefficient.begin();
	 aCoefficient = *(iter++);
	 bCoefficient = *(iter++);
	 cCoefficient = *(iter++);
	 dCoefficient = *(iter);
	 DEBUG cout<<aCoefficient<<"     "<<bCoefficient<<"      "<<cCoefficient<<"     "<<dCoefficient<<endl;
	 int middleRow = dCoefficient;
	 int highRow = middleRow + 0.9*lineHt;
	 int lowRow  = middleRow - 0.7*lineHt;
	 if (lowRow < 0) lowRow = 0;
	 if (highRow > rows) highRow = rows;

	 for ( int iCol = 0; iCol< cols;iCol++)
	 {
		 for( int iRow = lowRow; iRow < highRow;iRow++)
		 {
			 map_x.at<float>(iRow,iCol) = iCol;
			 map_y.at<float>(iRow,iCol) = aCoefficient*iCol*iCol*iCol + bCoefficient*iCol*iCol+cCoefficient*iCol+iRow;
		 }
	 }
	 coefficientM.pop_front();
	}

	PageImage dst = img.bwRemap(map_x,map_y);
	doSave(&dst, string("testOut/CHS001Final.tif").c_str());
	DEBUG 
		{
			img.showImage("source");
			dst.showImage("final");
			cout<<" FINISH REMAP"<<endl;
	}
	return 0;
}
void Dewarpper::save(const char *fileName) {
	/* ʹ���ṩ���ļ������浱ǰͼ��
	*/
	doSave(&img, fileName);
}

void Dewarpper::doSave(const PageImage *saveImg, const char *fileName) const {
	/* ˽�к�����ִ�б���ͼ��
	*/
	saveImg->saveImage(fileName);
	DEBUG cout<<"--- sucess do save work "<<endl;
}

bool Dewarpper::bufcnt(bool cond,int buf, int threshold,int maxBuf)
{
	static bool isIn = true;
	static int cycleCount = 0;
	static vector<int> record;
	if (isIn)
	{
		cycleCount = 0;
		record.clear();
	}
	record.push_back(cond);
	if(record.size()>buf)
	{
		int sum = accumulate(record.end()-buf,record.end(),0);
		if ( sum >= threshold)
		{
			isIn = true;
			return true;
		}
	}
	if( record.size()>maxBuf)
	{
		isIn = true;
	}
	return false;
}

	

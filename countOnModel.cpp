#include "countOnModel.h"

int main()
{
	Mat src, dst;
	//��ȡͼƬ������
	string photoName = "100-1920";
	readPicture(src, photoName);
	int upEdge = 0;
	int downEdge = 0;
	dst = src.clone();
	vector<int> list(src.rows, 0);
	vector<int> countList;
	//�����Ҹְ�������ģ���С
	int modelWidth = 5;
	int modelHight = 9;
	//ͼƬ�ϵĲ���λ�õ�����
	int samplingNum = 30;
	//����λ�ã����������
	int positionShift = 0;
	//�����жϱ߽�Ĳ�������
	int edgeSamplingWidth = 30;
	//�趨��ֵ������Ϊģ��ο�ģ��,ģ���������㼸������֮���ֵ��
	int modelNum = 4;
	//�����жϱ�Ե�ķŴ�ϵ��
	float edgeGainParament = 0.5;

	vector<int> upEdgeList;
	vector<int> downEdgeList;
	vector<int> list1;
	vector<int> list2;

	int step = src.cols / samplingNum;
	//Ԥ���������޹���Ϣ
	blur(src, src, Size(10, 10));	//5
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));	//10
	morphologyEx(src, src, MORPH_CLOSE, element);
	
	namedWindow("Ԥ����", WINDOW_NORMAL);
	resizeWindow("Ԥ����", 640, src.rows * 640 / src.cols);
	imshow("Ԥ����", src);

	getUpdateEdge(src, upEdgeList, downEdgeList, modelNum, edgeSamplingWidth, edgeGainParament,
		samplingNum, step, list1, list2);
	src = dst.clone();
	for (int k = 1; k < samplingNum; k++)
	{
		cout << "��" << k << "��:" << endl;
		positionShift = k*step;
		upEdge = upEdgeList[k - 1];
		downEdge = downEdgeList[k - 1];

		if (list1[k - 1] == 0)
		{
			cout << "δ��ȷ��ȡ���߽磬ʹ���������ϱ߽�" << endl;
		}
		if (list2[k - 1] == 0)
		{
			cout << "δ��ȷ��ȡ���߽磬ʹ���������±߽�" << endl;
		}
		drawEdgeLine(src, upEdge, downEdge, positionShift);
		for (int i = 0; i < src.rows; i++)
		{
			uchar* data = src.ptr<uchar>(i);
			for (int k = 0; k < modelWidth; k++)
			{
				list[i] += data[k + positionShift];
			}
			list[i] = list[i] / modelWidth;
		}
		/*if (k == 1)
		{
			getModelHight(src.rows, upEdge, downEdge, modelHight, list);
			cout << modelHight;
		}*/
		int count = countMaxInModel(src, upEdge, downEdge, modelHight, list, positionShift);
		drawCountNum(src, upEdge, downEdge, positionShift, count);
		dst = src.clone();
		cout << count << endl;
		countList.emplace_back(count);
	}

	//Ѱ������
	map<int, int> findMaxShownCount;
	for (auto i : countList)
	{
		if (findMaxShownCount.count(i))
		{
			findMaxShownCount[i] += 1;
		}
		else
		{
			findMaxShownCount.insert({ i, 1 });
		}
	}
	int theCount = 0;
	int theShowTimes = 0;
	for (auto i = findMaxShownCount.begin(); i != findMaxShownCount.end(); i++)
	{
		int times = i->second;
		if (theShowTimes < times)
		{
			theShowTimes = times;
			theCount = i->first;
		}
	}
	cout << "��������������������������������" << endl << theCount << endl;
	
	string layer = "layer:" + to_string((int)theCount);

	cv::putText(src, layer, Point(50, src.rows -50), FONT_HERSHEY_COMPLEX, 0.6, Scalar(255));
		

	namedWindow("���", WINDOW_NORMAL);
	resizeWindow("���", 640, src.rows * 640 / src.cols);
	cv::imshow("���", src);
	writePicture(src, photoName);
	cv::waitKey(0);
}
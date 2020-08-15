#include "countOnModel.h"

int main()
{
	Mat src, dst;
	//读取图片的名字
	string photoName = "100-1920";
	readPicture(src, photoName);
	int upEdge = 0;
	int downEdge = 0;
	dst = src.clone();
	vector<int> list(src.rows, 0);
	vector<int> countList;
	//设置找钢板数量的模板大小
	int modelWidth = 5;
	int modelHight = 9;
	//图片上的采样位置的数量
	int samplingNum = 30;
	//采样位置（计算层数）
	int positionShift = 0;
	//用于判断边界的采样列数
	int edgeSamplingWidth = 30;
	//设定峰值数量作为模板参考模型,模板数（计算几个波峰之间均值）
	int modelNum = 4;
	//用于判断边缘的放大系数
	float edgeGainParament = 0.5;

	vector<int> upEdgeList;
	vector<int> downEdgeList;
	vector<int> list1;
	vector<int> list2;

	int step = src.cols / samplingNum;
	//预处理，减少无关信息
	blur(src, src, Size(10, 10));	//5
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));	//10
	morphologyEx(src, src, MORPH_CLOSE, element);
	
	namedWindow("预处理", WINDOW_NORMAL);
	resizeWindow("预处理", 640, src.rows * 640 / src.cols);
	imshow("预处理", src);

	getUpdateEdge(src, upEdgeList, downEdgeList, modelNum, edgeSamplingWidth, edgeGainParament,
		samplingNum, step, list1, list2);
	src = dst.clone();
	for (int k = 1; k < samplingNum; k++)
	{
		cout << "第" << k << "列:" << endl;
		positionShift = k*step;
		upEdge = upEdgeList[k - 1];
		downEdge = downEdgeList[k - 1];

		if (list1[k - 1] == 0)
		{
			cout << "未正确获取到边界，使用修正的上边界" << endl;
		}
		if (list2[k - 1] == 0)
		{
			cout << "未正确获取到边界，使用修正的下边界" << endl;
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

	//寻找众数
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
	cout << "————————————————" << endl << theCount << endl;
	
	string layer = "layer:" + to_string((int)theCount);

	cv::putText(src, layer, Point(50, src.rows -50), FONT_HERSHEY_COMPLEX, 0.6, Scalar(255));
		

	namedWindow("输出", WINDOW_NORMAL);
	resizeWindow("输出", 640, src.rows * 640 / src.cols);
	cv::imshow("输出", src);
	writePicture(src, photoName);
	cv::waitKey(0);
}
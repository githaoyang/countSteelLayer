#include "countOnModel.h"

void getUpdateEdge(Mat src, vector<int>& upEdgeList, vector<int>& downEdgeList, 
	int modelNum, int edgeSamplingWidth, float edgeGainParament, int samplingNum, int step,
	vector<int>& list1, vector<int>& list2)
{
	int upEdge = 0;
	int downEdge = 0;
	int positionShift = 0;
	
	for (int k = 1; k < samplingNum; k++)
	{
		positionShift = k * step;
		findEdges(src, upEdge, downEdge, modelNum, positionShift, edgeSamplingWidth, edgeGainParament);
		upEdgeList.emplace_back(upEdge);
		downEdgeList.emplace_back(downEdge);
	}

	//箱型图，剔除离群点，并设置为0
	list1 = upEdgeList;
	list2 = downEdgeList;
	sort(list1.begin(), list1.end());

	sort(list2.begin(), list2.end());
	int oneQuarter = list1[list1.size() / 4];
	int threeQuarter = list1[list1.size() * 3 / 4];
	int upEdgeHigherLimit = threeQuarter + (threeQuarter - oneQuarter) * 1.5;
	int upEdgeLowerLimit = oneQuarter - (threeQuarter - oneQuarter) * 1.5;


	oneQuarter = list2[list2.size() / 4];
	threeQuarter = list2[list2.size() * 3 / 4];
	int downEdgeHigherLimit = threeQuarter + (threeQuarter - oneQuarter) * 1.5;
	int downEdgeLowerLimit = oneQuarter - (threeQuarter - oneQuarter) * 1.5;
	for (int i = 0; i < upEdgeList.size(); i++)
	{
		if ((upEdgeList[i] < upEdgeLowerLimit) || (upEdgeList[i] > upEdgeHigherLimit))
		{
			upEdgeList[i] = 0;
		}
		if ((downEdgeList[i] < downEdgeLowerLimit) || (downEdgeList[i] > downEdgeHigherLimit))
		{
			downEdgeList[i] = 0;
		}
	}

	//通过直线拟合（最小二乘法），补全之前剔除的点坐标
	vector<Point> upEdgePoints;
	vector<Point> downEdgePoints;
	//找出可用点坐标
	for (int k = 1; k < samplingNum; k++)
	{
		positionShift = k * step;
		if (upEdgeList[k - 1] == 0)
		{
		}
		else
		{
			upEdgePoints.emplace_back(Point(positionShift, upEdgeList[k - 1]));
		}
		if (downEdgeList[k - 1] == 0)
		{
		}
		else
		{
			downEdgePoints.emplace_back(Point(positionShift, downEdgeList[k - 1]));
		}
	}
	//补全
	list1 = upEdgeList;
	list2 = downEdgeList;
	vector<double> upParament;
	vector<double> downParament;
	fitLineByLeastSquares(upParament, upEdgePoints);
	fitLineByLeastSquares(downParament, downEdgePoints);
	for (int k = 1; k < samplingNum; k++)
	{
		positionShift = k * step;
		if (upEdgeList[k - 1] == 0)
		{
			upEdgeList[k - 1] = upParament[0] * positionShift + upParament[1];
		}
		if (downEdgeList[k - 1] == 0)
		{
			downEdgeList[k - 1] = downParament[0] * positionShift + downParament[1];
		}
	}
}
#pragma once
#include <iostream>
#include <opencv.hpp>
#include <unordered_map>

using namespace std;
using namespace cv;

int countMaxInModel(Mat src, int upEdge, int downEdge, int modelHight, vector<int> list, int positionShift);
int drawEdgeLine(Mat& src, int upEdge, int downEdge, int positionShift);
int findEdges(Mat src, int& upEdge, int& downEdge, int modelNum);
int findEdges(Mat src, int& upEdge, int& downEdge, int modelNum, int positionShift, int edgeSamplingWidth, float edgeGainParament);
void drawCountNum(Mat& src, int upEdge, int downEdge, int positionShift, int count);
bool fitLineByLeastSquares(std::vector<double>& parameters, std::vector<Point>& data);
int readPicture(Mat& src, string photoName);
int writePicture(Mat src, string photoName);
void getUpdateEdge(Mat src, vector<int>& upEdgeList, vector<int>& downEdgeList,
	int modelNum, int edgeSamplingWidth, float edgeGainParament, int samplingNum, int step,
	vector<int>& list1, vector<int>& list2);
void getModelHight(int photoHight, int upEdge, int downEdge, int& modelHight, vector<int> list);


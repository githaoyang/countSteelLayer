#include "countOnModel.h"

int drawLinesAndCircles(Mat& src, int upEdge, int downEdge, vector<int> list, vector<Point2i> peaksPoint)
{
	int x, y, a, b;
	for (x = y = a = b = 0; x < src.rows; x++)
	{
		y = list[x];
		Point s1(b, a);
		Point s2(y, x);
		line(src, s1, s2, Scalar(255));
		a = x;
		b = y;
	}
	for (auto i : peaksPoint)
	{
		circle(src, i, 2, Scalar(255));
	}
	
	int layerSize = peaksPoint.size();
	string layer = "layer:" + to_string(layerSize);

	putText(src, layer, Point(src.cols / 2, src.rows / 2), FONT_HERSHEY_COMPLEX, 0.7, Scalar(255));
	cout << peaksPoint.size();

	return 1;
}

int drawEdgeLine(Mat& src, int upEdge, int downEdge, int positionShift)
{
	cout << "上边缘为：" << upEdge << endl;
	line(src, Point(positionShift - 15, upEdge), Point(positionShift + 15, upEdge), Scalar(255));

	std::cout << "下边缘为：" << downEdge << endl;
	cv::line(src, Point(positionShift - 15, downEdge), Point(positionShift + 15, downEdge), Scalar(255));
	return 1;
}

void drawCountNum(Mat& src, int upEdge, int downEdge, int positionShift, int count)
{
	putText(src, to_string(count), Point(positionShift - 5, upEdge - 5), FONT_HERSHEY_COMPLEX, 0.4, Scalar(0));
	putText(src, to_string(count), Point(positionShift - 5, downEdge + 15), FONT_HERSHEY_COMPLEX, 0.4, Scalar(255));
}
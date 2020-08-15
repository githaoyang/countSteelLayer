#include "countOnModel.h"

void getModelHight(int photoHight, int upEdge, int downEdge, int& modelHight, vector<int> list)
{
	int flag = false;
	//被判定为钢板的列表
	vector<int> candidateList(photoHight, 0);
	for (int i = upEdge; i < downEdge; i++)
	{
		int max = list[i];
		int maxPosition = i;
		flag = false;
		for (int j = i + 1; (j < i + modelHight) && (j < downEdge); j++)
		{
			if (list[j] > max)
			{
				max = list[j];
				maxPosition = j;
				flag = true;
			}
		}
		if (flag && (list[maxPosition] >= list[maxPosition + 1]))
		{
			candidateList[maxPosition] = list[maxPosition];
		}
	}
	//找第一块钢板的位置
	int theFirstSteel = upEdge;
	int theLastSteel = downEdge;
	for (int i = upEdge + 1; i < downEdge; i++)
	{
		if (candidateList[i] != 0)
		{
			theFirstSteel = i;
			break;
		}
	}
	//计算钢板间距的众数和平均数
	vector<int> peaksWidth;
	int widthForSum = 0;
	int lastPeakPosition = theFirstSteel;
	for (int i = theFirstSteel + 1; i < downEdge; i++)
	{
		int peakPosition = i;
		if (candidateList[i] != 0)
		{
			peaksWidth.emplace_back(peakPosition - lastPeakPosition);
			widthForSum += peakPosition - lastPeakPosition;
			lastPeakPosition = peakPosition;
		}
	}
	map<int, int> findMaxShownWidth;
	for (auto i : peaksWidth)
	{
		if (findMaxShownWidth.count(i))
		{
			findMaxShownWidth[i] += 1;
		}
		else
		{
			findMaxShownWidth.insert({ i, 1 });
		}
	}
	double theWidth = 0;
	int theShowTimes = 0;
	for (auto i = findMaxShownWidth.begin(); i != findMaxShownWidth.end(); i++)
	{
		int times = i->second;
		if (theShowTimes < times)
		{
			theShowTimes = times;
			theWidth = i->first;
		}
	}
	modelHight = theWidth * 1.2;
}


int countMaxInModel(Mat src, int upEdge, int downEdge, int modelHight, vector<int> list, int positionShift)
{
	int flag = false;
	int count = 0;
	//被判定为钢板的列表
	vector<int> candidateList(src.rows, 0);
	for (int i = upEdge; i < downEdge; i++)
	{
		int max = list[i];
		int maxPosition = i;
		flag = false;
		for (int j = i + 1; (j < i + modelHight) && (j < downEdge); j++)
		{
			if (list[j] > max)
			{
				max = list[j];
				maxPosition = j;
				flag = true;
			}
		}
		if (flag && (list[maxPosition] >= list[maxPosition + 1]))
		{
			candidateList[maxPosition] = list[maxPosition];
		}
	}
	//找第一块钢板的位置
	int theFirstSteel = upEdge;
	int theLastSteel = downEdge;
	for (int i = upEdge + 1;i < downEdge; i++)
	{
		if (candidateList[i] != 0)
		{
			theFirstSteel = i;
			break;
		}
	}
	for (int i = downEdge - 1; i > upEdge; i--)
	{
		if (candidateList[i] != 0)
		{
			theLastSteel = i;
			break;
		}
	}
	//计算钢板间距的众数和平均数
	vector<int> peaksWidth;
	int peakValuesSum = 0;
	int widthForSum = 0;
	int lastPeakPosition = theFirstSteel;
	for (int i = theFirstSteel + 1; i < downEdge; i++)
	{
		int peakPosition = i;
		if (candidateList[i] != 0)
		{
			peakValuesSum += candidateList[i];
			count++;
			peaksWidth.emplace_back(peakPosition - lastPeakPosition);
			widthForSum += peakPosition - lastPeakPosition;
			lastPeakPosition = peakPosition;
		}
	}
	int averagePeakValue = peakValuesSum / count;
	map<int, int> findMaxShownWidth;
	for (auto i : peaksWidth)
	{
		if (findMaxShownWidth.count(i))
		{
			findMaxShownWidth[i] += 1;
		}
		else
		{
			findMaxShownWidth.insert({ i, 1 });
		}
	}
	double theWidth = 0;
	int theShowTimes = 0;
	for (auto i = findMaxShownWidth.begin(); i != findMaxShownWidth.end(); i++)
	{
		int times = i->second;
		if (theShowTimes < times)
		{
			theShowTimes = times;
			theWidth = i->first;
		}
	}
	int averageWidth = widthForSum / count;
	//如果众数和平均数差距过大，使用平均数
	if (abs(theWidth- averageWidth) >= averageWidth * 0.5)
	{
		theWidth = averageWidth;
	}
	//处理第一块钢板检测不到的问题
	if ((theFirstSteel - upEdge) > theWidth)
	{
		candidateList[(theFirstSteel + upEdge) / 2] = 1;
	}
	//处理最后一块钢板多检测的问题
	if ((downEdge - theLastSteel) < theWidth / 2)
	{
		candidateList[theLastSteel] = 0;
	}

	//如果间距过大，则加入层数
	int lastPosition = theFirstSteel;
	for (int i = theFirstSteel + theWidth; i < downEdge; i++)
	{
		if (candidateList[i] != 0)
		{
			int thePosition = i;
			if ((thePosition - lastPosition -theWidth > 0)&&((thePosition - lastPosition - theWidth) > theWidth * 0.5))
			{
				int middle = (thePosition + lastPosition) / 2;
				candidateList[middle] = 1;
				i = middle - 1;
				thePosition = i;
				continue;
			}
			lastPosition = thePosition;
		}
	}
	//如果间距过小，则删除层数
	lastPosition = theFirstSteel;
	for (int i = theFirstSteel + 1; i < downEdge; i++)
	{
		if (candidateList[i] != 0)
		{
			if ((i - lastPosition) < theWidth * 0.5)
			{
				candidateList[i] = 0;
				continue;
			}
			lastPosition = i;
		}
	}
	
	for (int i = theFirstSteel; i < downEdge; i++)
	{
		if (candidateList[i] != 0)
		{
			if (abs(candidateList[i] - averagePeakValue) > averagePeakValue)
			{
				candidateList[i] = 0;
			}
		}
	}


	count = 0;
	for (int i = upEdge; i < downEdge; i++)
	{
		if (candidateList[i] != 0)
		{
			count++;
			line(src, Point(positionShift, i), Point(positionShift + 5, i), Scalar(255));
		}
	}
	return count;
}
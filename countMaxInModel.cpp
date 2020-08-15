#include "countOnModel.h"

void getModelHight(int photoHight, int upEdge, int downEdge, int& modelHight, vector<int> list)
{
	int flag = false;
	//���ж�Ϊ�ְ���б�
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
	//�ҵ�һ��ְ��λ��
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
	//����ְ����������ƽ����
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
	//���ж�Ϊ�ְ���б�
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
	//�ҵ�һ��ְ��λ��
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
	//����ְ����������ƽ����
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
	//���������ƽ����������ʹ��ƽ����
	if (abs(theWidth- averageWidth) >= averageWidth * 0.5)
	{
		theWidth = averageWidth;
	}
	//�����һ��ְ��ⲻ��������
	if ((theFirstSteel - upEdge) > theWidth)
	{
		candidateList[(theFirstSteel + upEdge) / 2] = 1;
	}
	//�������һ��ְ���������
	if ((downEdge - theLastSteel) < theWidth / 2)
	{
		candidateList[theLastSteel] = 0;
	}

	//�����������������
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
	//�������С����ɾ������
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
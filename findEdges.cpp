#include "countOnModel.h"

int findEdges(Mat src, int& upEdge, int& downEdge, int modelNum, int positionShift, int edgeSamplingWidth, float edgeGainParament)
{
	int count = 0;
	vector<int> list(src.rows, 0);
	
	
	int middleRow = src.rows / 2;
	int averageGrayScaleValue = 0;
	bool flag1 = false;
	bool flag2 = false; 
	int k = 0;
	upEdge = middleRow;
	downEdge = middleRow;
	
	for (int i = 0; i < src.rows; i++)
	{
		uchar* data = src.ptr<uchar>(i);
		for (int k =  0; k < edgeSamplingWidth; k++)
		{
			list[i] += data[k + positionShift - edgeSamplingWidth/2];
		}
		list[i] = list[i] / edgeSamplingWidth;
	}
	int modelWidth = 0;
	for (int i = middleRow; k < modelNum; i++)
	{
		if (list[i] > list[i - 1])
		{
			flag1 = true;
		}
		else
		{
			flag1 = false;
		}
		//�߶���
		if (flag1 == false && flag2 == true)
		{
			k++;
		}
		averageGrayScaleValue += list[i];
		flag2 = flag1;
		modelWidth++;
	}
	averageGrayScaleValue = averageGrayScaleValue / modelWidth;

	for (int i = middleRow; i < src.rows; i++)
	{
		if (abs(averageGrayScaleValue - list[i]) > averageGrayScaleValue * edgeGainParament)
		{
			if (abs(averageGrayScaleValue - list[i+1]) > averageGrayScaleValue * edgeGainParament)
			{
				downEdge = i;
				break;
			}
		}
	}
	for (int i = middleRow; i > 0; i--)
	{
		if (abs(averageGrayScaleValue - list[i]) > averageGrayScaleValue * edgeGainParament)
		{
			if (abs(averageGrayScaleValue - list[i - 1]) > averageGrayScaleValue * edgeGainParament)
			{
				upEdge = i;
				break;
			}
		}
	}
	return 1;
}






//��ȡ���±߽�
int findEdges(Mat src, int& upEdge, int& downEdge, int modelNum)
{
	int count = 0;
	int k = 0;
	vector<int> list(src.rows, 0);
	
	bool flag1 = false;
	bool flag2 = false;
	bool flag3 = false;
	//��������
	int num = 10;
	//��Ե����
	int amplifierParament = 15;
	//��������ģ����
	//modelNum = 6;

	int step = src.cols / num;
	//�����߶Ȳ�
	int lowDepth = 0;
	int highDepth = 0;
	
	//ƽ���߶Ȳ�
	double avegLowToHigh = 0;
	double avegHighToLow = 0;
	int middleRow = src.rows / 2;
	int fromLowToHigh = 0;
	int fromHighToLow = 0;
	upEdge = middleRow;
	downEdge = middleRow;

	//Ԥ���������޹���Ϣ
	blur(src, src, Size(5, 5));
	Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
	morphologyEx(src, src, MORPH_CLOSE, element);

	
	for (int i = 0; i < src.rows; i++)
	{
		uchar* data = src.ptr<uchar>(i);
		for (int k = 0; k < num; k++)
		{
			list[i] += data[k * step];
		}
		list[i] = list[i] / num;
	}

	lowDepth = list[src.rows / 2];
	highDepth = list[src.rows / 2];
	//������м������ҵ�modelNum���������嵱��ģ�壬�ҳ������Ŀ����ģ��
	//�����ȶ�һ������ȡƽ��
	for (int i = middleRow; k < modelNum; i++)
	{
		if (list[i] > list[i - 1])
		{
			flag1 = true;
		}
		else
		{
			flag1 = false;
		}
		//�߶���
		if (flag1 == false && flag2 == true)
		{
			k++;
			highDepth = list[i - 1];
			avegLowToHigh += highDepth - lowDepth;
		}
		//�͵�
		if (flag1 == true && flag2 == false)
		{
			lowDepth = list[i - 1];
			avegHighToLow += highDepth - lowDepth;
		}
		flag2 = flag1;
	}
	avegLowToHigh = avegLowToHigh / modelNum;
	if (list[middleRow] > list[middleRow - 1])
	{
		avegHighToLow = avegHighToLow / (modelNum - 1);
	}
	else
	{
		avegHighToLow = avegHighToLow / modelNum;
	}

	//Ѱ���±߽�--------------------
	flag1 = false;
	flag2 = false;
	flag3 = false;
	lowDepth = list[src.rows / 2];
	highDepth = list[src.rows / 2];
	
	for (int i = middleRow; i < src.rows; i++)
	{
		int lastPeak = src.rows / 2;
		if (list[i] > list[i - 1])
		{
			flag1 = true;
		}
		else
		{
			flag1 = false;
		}
		if (list[i] == list[i - 1])
		{
			flag3 = true;
		}
		//�߶���
		if (flag1 == false && flag2 == true)
		{
			highDepth = list[i - 1];
			fromLowToHigh = highDepth - lowDepth;
			if (fromLowToHigh > avegLowToHigh* amplifierParament)
			{
				downEdge = i;
				break;
			}
		}
		//�͵�
		if ((flag1 == true && flag2 == false) || (flag1 == false && flag2 == false && flag3 == true))
		{
			lowDepth = list[i - 1];
			fromHighToLow = highDepth - lowDepth;
			if (fromHighToLow > avegHighToLow* amplifierParament)
			{
				downEdge = i;
				break;
			}
			flag3 = false;
		}
		flag2 = flag1;
	}


	//Ѱ���ϱ߽�--------------------
	flag1 = false;
	flag2 = false;
	flag3 = false;
	for (int i = middleRow; i > 0; i--)
	{

		int lastPeak = src.rows / 2;
		if (list[i] >= list[i + 1])
		{
			flag1 = true;
		}
		else
		{
			flag1 = false;
		}
		if (list[i] == list[i + 1])
		{
			flag3 = true;
		}
		//�߶���
		if ((flag1 == false && flag2 == true) || (flag1 == true && flag2 == true && flag3 == true))
		{
			highDepth = list[i + 1];
			fromLowToHigh = highDepth - lowDepth;
			if (fromLowToHigh > avegLowToHigh* amplifierParament)
			{
				upEdge = i;
				break;
			}
			flag3 = false;
		}
		//�͵�
		if (flag1 == true && flag2 == false)
		{
			lowDepth = list[i + 1];
			fromHighToLow = highDepth - lowDepth;
			if (fromHighToLow > avegHighToLow* amplifierParament)
			{
				upEdge = i;
				break;
			}
		}
		flag2 = flag1;
	}
	return 1;
}
#include "countOnModel.h"



bool fitLineByLeastSquares(std::vector<double>& parameters, std::vector<Point>& data)
{
	try
	{
		parameters.clear();
		if (data.size() < 2)
			return false;

		double A = 0.0, B = 0.0, C = 0.0;

		int xjPcount = data.size();
		double meanX = 0.0, meanY = 0.0;
		double sumXX = 0.0, sumXY = 0.0, sumYY = 0.0;
		for (int i = 0; i < xjPcount; i++)
		{
			meanX += data[i].x;
			meanY += data[i].y;

			sumXX += data[i].x * data[i].x;
			sumXY += data[i].x * data[i].y;
			sumYY += data[i].y * data[i].y;
		}
		meanX /= xjPcount;
		meanY /= xjPcount;

		sumXX -= xjPcount * meanX * meanX;
		sumXY -= xjPcount * meanX * meanY;
		sumYY -= xjPcount * meanY * meanY;
		if (sumXX < 1e-15)
		{
			A = 1.0;
			B = 0.0;
		}
		else
		{
			double ev = (sumXX + sumYY + sqrt((sumXX - sumYY) * (sumXX - sumYY) + 4 * sumXY * sumXY)) / 2.0;
			A = -sumXY;
			B = ev - sumYY;
			double norm = sqrt(A * A + B * B);
			A /= norm;
			B /= norm;
		}

		parameters.push_back(A);
		parameters.push_back(B);
		C = -(A * meanX + B * meanY);
		parameters.push_back(C);

		double a = -parameters[0] / parameters[1];
		double b = -parameters[2] / parameters[1];
		parameters.clear();
		parameters.push_back(a);
		parameters.push_back(b);

		return true;
	}
	catch (const std::exception&)
	{
		return false;
	}
}
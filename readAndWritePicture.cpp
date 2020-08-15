#include "countOnModel.h"
#include <random>
#include <time.h>

int readPicture(Mat& src, string photoName)
{
	string path = "./ԭͼ/" + photoName + ".jpg";
	src = imread(path, IMREAD_GRAYSCALE); 	

	return 1;
}

int writePicture(Mat src, string photoName)
{
	/*default_random_engine e;
	uniform_int_distribution<int> u(0, 100);*/
	time_t ti;
	ti = time(NULL);
	string path = "./���/output" + photoName
		+ "--" + to_string(ti%1000) + ".jpg";
	imwrite(path, src);

	return 1;
}
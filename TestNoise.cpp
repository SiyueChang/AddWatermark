#include <random>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
using namespace std;
using namespace cv;
double DistanceNM(int a1,int a2,int b1 ,int b2)
{
	return sqrt((a1 - b1) * (a1 - b1) + (a2 - b2) * (a2 - b2));
}

void interpolation(Scalar (&color)[100][100],int a1,int a2,int b1 ,int b2)
{
	float measure;
	Scalar colorA = color[a1][a2];
	Scalar colorB = color[b1][b2];
	for(int i = a1;i < b1;i++)
	{
		for(int j = a2;j < b2;j++)
		{
			measure = ((i-a1)/(b1-a1))*((j-a2)/(b2-a2));
			color[i][j] = colorA *measure + colorB *(1-measure);
		}
	}
}
int main(){
	int num = 100;
	int l = 2;
	int numx[l];
	int numy[l];
	Mat img = Mat::zeros(num * 10, num * 10, CV_8UC3);
	Scalar color[num][num];
	default_random_engine e;
	uniform_int_distribution<double> u1(0,100);
	uniform_int_distribution<double> u2(0,255); 
	e.seed(time(0));
	
	for(int i = 0; i < l ; i++)
	{
		numx[i] = u1(e);
		numy[i] = u1(e);
		color[numx(i)][numy(i)] = Scalar(u2(e),u2(e),u2(e));
	}
	interpolation(color,numx(0),numy(0),numx(1),numy(1));
	for(int i = 0; i < num; i++)
	{
		rectangle(img, point, point2, Color[i], -1);
	}
	imshow("PointsinImage", img);
	waitKey(0);
	return 0;
}
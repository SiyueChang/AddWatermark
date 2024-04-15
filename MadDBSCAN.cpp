#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include "DBSCAN.h"

void OutVector(std::vector<float> outVector)
{
	for(auto i : outVector)
	{
        std::cout << i << "  ";
	}
}
std::vector<float> DBSCAN(std::vector<float> num)
{
	std::vector<point> dataset;

    for (int i = 1 ; i < num.size() - 1 ; ++i)
    {
        dataset.push_back(point(num[i]));
    }
    DBSCAN(dataset, 3, 2);
    for(auto x:dataset)
    {
        
    }
}

std::vector<float> QuartileRange(std::vector<float> num)
{
    int n = num.size();
    int q2 = n / 2;
    int q1 = q2 / 2;
    int q3 = q1 + q2;
    float Q1 = num[q1];
    float Q3 = num[q3];
    float IQR = (Q3 - Q1) * 1.5;
    float low = Q1 - IQR;
    float up = Q3 + IQR;
    std::vector<float> OutVec;
    for (auto x : num)
    {
        if(x > low && x < up )
        {
            OutVec.push_back(x);
        }
    }
    return OutVec;
}
std::vector<float> ZScore(std::vector<float> num)
{

    float sum = std::accumulate(std::begin(num), std::end(num), 0.0);
    float mean = sum / num.size();

    float accum = 0.0;
    std::for_each(std::begin(num), std::end(num), [&](const double d) {accum += (d - mean) * (d - mean); });

    float stD = sqrt(accum / (num.size() - 1));

    std::vector<float> OutVec;
    float zs = 0.0;
    float size = 1;
    for (auto x : num)
    {
        zs = (x - mean) / stD;
        if(zs < size && zs > -1 * size)
        {

            OutVec.push_back(x);
        }
    }
    return OutVec;
}

std::vector<float> ZScoreQuartileRange(std::vector<float> num)
{

    float sum = std::accumulate(std::begin(num), std::end(num), 0.0);
    float mean = sum / num.size();

    float accum = 0.0;
    std::for_each(std::begin(num), std::end(num), [&](const double d) {accum += (d - mean) * (d - mean); });

    float stD = sqrt(accum / (num.size() - 1));

    int n = num.size();
    int q2 = n / 2;
    int q1 = q2 / 2;
    int q3 = q1 + q2;
    float Q1 = num[q1];
    float Q3 = num[q3];
    float IQR = (Q3 - Q1) * 1.5;
    float low = Q1 - IQR;
    float up = Q3 + IQR;

    std::vector<float> OutVec;
    float zs = 0.0;
    float size = 1;
    for (auto x : num)
    {
        zs = (x - mean) / stD;
        if (zs < size && zs > -1 * size)
        {
            if (x > low && x < up)
            {
                OutVec.push_back(x);
            }
        }
    }
    return OutVec;
}

std::vector<float> Forest(std::vector<float> num, const int div = 8)
{
    if(div > num.size())
    {
        return num;
    }

    int n = num.size() / div;
    float area = (num.front() + num.back()) / div;
    int numIndex = 0;
    int areaIndex = 1;
    std::vector<float> OutVec;
    std::vector<float> temporaryVec;
	for (auto x : num)
	{
		if (area * areaIndex + num.front() < x)
		{
            areaIndex++;
            if (numIndex > n)
            {
                OutVec.insert(OutVec.end(),temporaryVec.begin(),temporaryVec.end());
            }
            std::vector<float>().swap(temporaryVec);
            numIndex = 0;
		}
		else
		{
            numIndex++;
            temporaryVec.push_back(x);
		}

    }
    return OutVec;
}
int main()
{
	//std::vector<float> num = { 0.1,0.0,0.2,0.3,0.6,0.3,0.9,0.3,0.4,3,6,0.3,0.5,0.3,0.1 ,0.3,0.5,0.3,0.9,6,5,8,57223,475524,72572752,7,34,5345,23,425,23};
    std::vector<float> num = {54654,1518918,1561871,164616,181641,919151,9181951,5615191,5144561,618716,164141,5164161,516416,5,121,1561,1516,1671,16156,1611,2156,16,1616,165,16,1312,51,651,6,1123213,61,65,112321,65,1123213,651,65,112312,61,6,123211,8,16,51,6,51,56,11312,651,6,156,112312,6,4,561,16,521,6,5,5,1,65,1231,651,546};
    std::sort(num.begin(), num.end());
    OutVector(num);
    std::cout << std::endl;
    OutVector(QuartileRange(num));
    std::cout << std::endl;
    OutVector(ZScore(num));
    std::cout << std::endl;
    OutVector(ZScoreQuartileRange(num));
    std::cout << std::endl;
    OutVector(Forest(num));
}
